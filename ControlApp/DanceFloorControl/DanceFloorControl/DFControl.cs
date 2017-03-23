using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace DanceFloorControl
{
    public class DFControl
    {
        private enum Commands
        {
            EffectAlpha = 'A',
            BaseEffect = 'B',
            EffectColor = 'C',
            BaseColor = 'D',
            SecondColor = 'F',
            SetEffect = 'E',

            Spotlight = 'M',

            Strobe = 'Q',
            EffectBigness = 'S',
            SendPresses = 'P',
            TapTempo = 'T',
            Reboot = 'R',

            ReleaseAll = 'Z',

        }


        IPAddress int_floorIP;
        Socket floorComm = new Socket(System.Net.Sockets.AddressFamily.InterNetwork, System.Net.Sockets.SocketType.Dgram, System.Net.Sockets.ProtocolType.Udp);
        int Port = 9934;
        IPEndPoint endPoint;

        // Holds the only copy of this class that is allowed to exist.
        private static DFControl instance;

        private DFControl() { } // Prevent overriding

        public static DFControl Instance
        {
            get
            {
                if (instance == null) instance = new DFControl();
                return instance;
            }
        }




        public string floorIP
        {
            set
            {
                int_floorIP = IPAddress.Parse(value);
                endPoint = new IPEndPoint(int_floorIP, Port);
            }
            get
            {
                return int_floorIP.ToString();
            }
        }
        void Send(List<byte> inBuf)
        {
            // packet format:
            // FF 01 (data) FF 02
            // FF codes:
            // FF - Escape
            // 01 - Start of packet
            // 02 - End of packet
            // 10 - Literal FF

            List<byte> data = new List<byte>();
            data.Add(0xFF);
            data.Add(0x01);

            for (int i = 0; i < inBuf.Count; i++)
            {
                if (inBuf[i] == 0xFF)
                {
                    data.Add(0xFF);
                    data.Add(0x10);
                }
                else
                {
                    data.Add(inBuf[i]);
                }
            }
            data.Add(0xFF);
            data.Add(0x02);

            floorComm.SendTo(data.ToArray(), endPoint);
        }

        public void SetEffect(byte EffectNum)
        {
            // Send the new effect num to the floor.
            List<byte> data = new List<byte>();

            // Command is: "E", effect number
            data.Add((byte)Commands.SetEffect);
            data.Add(EffectNum);

            Send(data);

        }

        public void SetBaseEffect(byte EffectNum)
        {
            // Send the new effect num to the floor.
            List<byte> data = new List<byte>();

            // Command is: "E", effect number
            data.Add((byte)Commands.BaseEffect);
            data.Add(EffectNum);

            Send(data);

        }


        public void SetEffectColor(byte r, byte g, byte b)
        {
            // Send the new effect num to the floor.
            List<byte> data = new List<byte>();

            // Command is: "C", r, g, b
            data.Add((byte)Commands.EffectColor);
            data.Add(r);
            data.Add(g);
            data.Add(b);

            Send(data);

        }

        public void SetBaseColor(byte r, byte g, byte b)
        {
            // Send the new effect num to the floor.
            List<byte> data = new List<byte>();

            // Command is: "C", r, g, b
            data.Add((byte)Commands.BaseColor);
            data.Add(r);
            data.Add(g);
            data.Add(b);

            Send(data);

        }

        public void SetSecondColor(byte r, byte g, byte b)
        {
            // Send the new effect num to the floor.
            List<byte> data = new List<byte>();

            // Command is: "C", r, g, b
            data.Add((byte)Commands.SecondColor);
            data.Add(r);
            data.Add(g);
            data.Add(b);

            Send(data);

        }

        public void TapTempo()
        {
            // Tempo tap
            List<byte> data = new List<byte>();

            // Command is: "T"
            data.Add((byte)Commands.TapTempo);
            Send(data);

        }


        public void Spotlight(byte x, byte y, byte size)
        {
            // Display a spotlight at this location
            List<byte> data = new List<byte>();

            // Command is: "T"
            data.Add((byte)Commands.Spotlight);
            data.Add(x);
            data.Add(y);
            data.Add(size);

            Send(data);

        }

        public void SendPressesTo(IPAddress Addr)
        {
            List<byte> data = new List<byte>();

            // Command is: "P" followed by 4 bytes of IP
            data.Add((byte)Commands.SendPresses);
            data.Add(Addr.GetAddressBytes()[0]);
            data.Add(Addr.GetAddressBytes()[1]);
            data.Add(Addr.GetAddressBytes()[2]);
            data.Add(Addr.GetAddressBytes()[3]);
            Send(data);

        }

        public void RebootTeensy()
        {
            // Reboot
            List<byte> data = new List<byte>();

            // Command is: "R"
            data.Add((byte)Commands.Reboot);
            Send(data);


        }

        public void ReleaseAllButtons()
        {
            // Reboot
            List<byte> data = new List<byte>();

            // Command is: "R"
            data.Add((byte)Commands.ReleaseAll);
            Send(data);
        }

        public void Strobe()
        {
            // Reboot
            List<byte> data = new List<byte>();

            // Command is: "R"
            data.Add((byte)Commands.Strobe);
            Send(data);
        }


        public void SetEffectAlpha(byte b)
        {
            // Reboot
            List<byte> data = new List<byte>();

            // Command is: "A"
            data.Add((byte)Commands.EffectAlpha);
            data.Add(b);
            Send(data);

        }

        public void SetEffectBigness(byte b)
        {
            // Reboot
            List<byte> data = new List<byte>();

            // Command is: "B"
            data.Add((byte)Commands.EffectBigness);
            data.Add(b);
            Send(data);

        }


    }
}
