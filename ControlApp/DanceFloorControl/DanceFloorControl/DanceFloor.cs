using ArtNet.Packets;
using ArtNet.Sockets;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace DDRGame
{
    public class DanceFloor
    {

        UdpClient dfBtns;
        IPEndPoint dfIPe;

        ArtNetSocket artnet = new ArtNet.Sockets.ArtNetSocket();
        IPAddress floorIP = IPAddress.Parse("2.0.0.10");

        Color[,] floor = new Color[16, 16];
        byte[,] floorAlpha = new byte[16, 16];
        Color[,] baseFloor = new Color[16, 16];
        System.Timers.Timer ticker;

        public event ButtonEventHnadler ButtonState;
        public delegate void ButtonEventHnadler(object sender, ButtonEventArgs e);

        public DanceFloor()
        {
            ticker = new System.Timers.Timer(500);
            ticker.Elapsed += Ticker_Elapsed;
            ticker.Start();

            // Start listening for button events.
            dfIPe = new IPEndPoint(IPAddress.Any, 8888);
            dfBtns = new UdpClient(dfIPe);
            dfBtns.BeginReceive(new AsyncCallback(ReceiveCallback), null);

        }

        public void SetPixel(byte x, byte y, byte r, byte g, byte b, byte a)
        {
            floor[x, y] = Color.FromArgb(255, r, g, b);
            floorAlpha[x, y] = a;
        }
        public void SetPixel(int x, int y, byte r, byte g, byte b, byte a)
        {
            SetPixel((byte)x, (byte)y, r, g, b, a);
        }

        public void SetPixel(byte x, byte y, Color c, byte a)
        {
            floor[x, y] = c;
            floorAlpha[x, y] = a;
        }

        public void SetPixel(int x, int y, Color c, byte a)
        {
            floor[(byte)x, (byte)y] = c;
            floorAlpha[x, y] = a;
        }



        public void SetBasePixel(byte x, byte y, byte r, byte g, byte b)
        {
            baseFloor[x, y] = Color.FromArgb(255, r, g, b);
        }
        public void SetBasePixel(int x, int y, byte r, byte g, byte b)
        {
            SetBasePixel((byte)x, (byte)y, r, g, b);
        }

        public void SetBasePixel(byte x, byte y, Color c)
        {
            baseFloor[x, y] = c;
        }

        public void SetBasePixel(int x, int y, Color c)
        {
            baseFloor[(byte)x, (byte)y] = c;
        }



        public void FadeFloor(byte amount)
        {
            // Fade the entire floor out by this amount.
            for (byte x = 0; x < 16; x++)
            {
                for (byte y = 0; y < 16; y++)
                {
                    /*
                    Color c = Color.FromArgb(
                            Math.Max(floor[x, y].R - amount, 0),
                            Math.Max(floor[x, y].G - amount, 0),
                            Math.Max(floor[x, y].B - amount, 0));
                    floor[x, y] = c;
                    */

                    floorAlpha[x, y] = (byte)Math.Max((int)floorAlpha[x, y] - (int)amount, 0);
                }
            }

        }

        private void Ticker_Elapsed(object sender, ElapsedEventArgs e)
        {
            SendFrame();
        }


        public void SendFrame()
        {
            var packet = new ArtNetDmxPacket();
            packet.Universe = 0;
            byte[] _dmxData = new byte[510];
            packet.DmxData = _dmxData;

            int x = 0, y = 0;

            // First packet goes from 0 to 509 (for now)
            for (var i = 0; i < 509; i += 3)
            {

                packet.DmxData[i    ] = (byte)(((int)baseFloor[x, y].R * (int)(255 - floorAlpha[x, y]) / 256) + ((int)floor[x, y].R * (int)floorAlpha[x, y] / 256));
                packet.DmxData[i + 1] = (byte)(((int)baseFloor[x, y].G * (int)(255 - floorAlpha[x, y]) / 256) + ((int)floor[x, y].G * (int)floorAlpha[x, y] / 256));
                packet.DmxData[i + 2] = (byte)(((int)baseFloor[x, y].B * (int)(255 - floorAlpha[x, y]) / 256) + ((int)floor[x, y].B * (int)floorAlpha[x, y] / 256));

                //packet.DmxData[i] = floor[x, y].R;
                //packet.DmxData[i + 1] = floor[x, y].G;
                //packet.DmxData[i + 2] = floor[x, y].B;
                x++;
                if (x > 15) { x = 0; y++; }
            };
            artnet.Send(packet, floorIP);

            //Thread.Sleep(100);

            packet = new ArtNetDmxPacket();
            packet.Universe = 1;
            _dmxData = new byte[258];
            packet.DmxData = _dmxData;

            // Second packet, starts where first one left off.
            for (var i = 0; i < 257; i += 3)
            {
                packet.DmxData[i    ] = (byte)(((int)baseFloor[x, y].R * (int)(255 - floorAlpha[x, y]) / 256) + ((int)floor[x, y].R * (int)floorAlpha[x, y] / 256));
                packet.DmxData[i + 1] = (byte)(((int)baseFloor[x, y].G * (int)(255 - floorAlpha[x, y]) / 256) + ((int)floor[x, y].G * (int)floorAlpha[x, y] / 256));
                packet.DmxData[i + 2] = (byte)(((int)baseFloor[x, y].B * (int)(255 - floorAlpha[x, y]) / 256) + ((int)floor[x, y].B * (int)floorAlpha[x, y] / 256));
                x++;
                if (x > 15) { x = 0; y++; }
                if (y > 15) break;
            };
            artnet.Send(packet, floorIP);

        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            Byte[] data = dfBtns.EndReceive(ar, ref dfIPe);
            //Console.WriteLine("Received " + data.Length + " bytes");
            if (data.Length == 4)
            {
                Console.WriteLine("Recv: " + data[0] + ", " + data[1] + ", " + data[2] + ", " + data[3]);
                OnButtonState(new ButtonEventArgs(data[1], data[2], data[3] == 1));
            }

            dfBtns.BeginReceive(new AsyncCallback(ReceiveCallback), null);
        }

        protected virtual void OnButtonState(ButtonEventArgs e)
        {
            if (ButtonState != null)
                ButtonState(this, e);
        }

    }

    public class ButtonEventArgs : EventArgs
    {
        public int x;
        public int y;
        public bool down;

        public ButtonEventArgs(int x, int y, bool down)
        {
            this.x = x;
            this.y = y;
            this.down = down;
        }

    }


}
