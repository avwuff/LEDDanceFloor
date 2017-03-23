using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using XComponent.SliderBar;
using Midi;
using Microsoft.Win32;
using ArtNet.Packets;
using ArtNet.Sockets;

namespace DanceFloorControl
{
    public partial class frmControls : Form
    {
        public List<FloorEffect> Effects = new List<FloorEffect>();
        public List<FloorColor> Colors = new List<FloorColor>();
        public List<BaseEffect> BaseEffects = new List<BaseEffect>();
        public string FloorIP = "2.0.0.10";

        ArtNetSocket artnet = new ArtNet.Sockets.ArtNetSocket();
        IPAddress jinxIP = IPAddress.Parse("127.0.0.1");
        IPAddress signIP = IPAddress.Parse("2.0.0.242");


        InputDevice inputDevice;

        byte spotX = 0;
        byte spotY = 0;
        byte spotS = 8;



        byte[] _dmxData = new byte[10];
        byte[] _dmxDataSign = new byte[10];

        public frmControls()
        {
            InitializeComponent();
        }



        private void frmControls_Load(object sender, EventArgs e)
        {

            DFControl.Instance.floorIP = FloorIP;
            _dmxData[6] = 255; // default master brightness to full
            _dmxDataSign[6] = 255; // default master brightness to full

            string lastMidi = RegLoadSetting("DanceFloorControl", "Midi", "Device");

            // Look for a midi device.
            for (int i = 0; i < InputDevice.InstalledDevices.Count; i++)
            {
                cbMidi.Items.Add(InputDevice.InstalledDevices[i].Name);
                if (lastMidi == InputDevice.InstalledDevices[i].Name) cbMidi.SelectedIndex = cbMidi.Items.Count - 1; // Set to this index.
            }


            // Add some effects
            Effects.Add(new DanceFloorControl.FloorEffect("None", 0));
            Effects.Add(new DanceFloorControl.FloorEffect("Expanding Circle", 1));
            Effects.Add(new DanceFloorControl.FloorEffect("Expanding Square", 5));
            Effects.Add(new DanceFloorControl.FloorEffect("Expanding Cross", 6));
            Effects.Add(new DanceFloorControl.FloorEffect("Expanding Spiral", 7));
            Effects.Add(new DanceFloorControl.FloorEffect("Expanding Star", 8));
            Effects.Add(new DanceFloorControl.FloorEffect("Expanding Triangle", 9));

            Effects.Add(new DanceFloorControl.FloorEffect("BEAT: Circle", 10));
            Effects.Add(new DanceFloorControl.FloorEffect("BEAT: Square", 15));
            Effects.Add(new DanceFloorControl.FloorEffect("BEAT: Cross", 16));
            Effects.Add(new DanceFloorControl.FloorEffect("BEAT: Spiral", 17));
            Effects.Add(new DanceFloorControl.FloorEffect("BEAT: Star", 18));
            Effects.Add(new DanceFloorControl.FloorEffect("BEAT: Triange", 19));


            Effects.Add(new DanceFloorControl.FloorEffect("Color Bomb", 30));

            Effects.Add(new DanceFloorControl.FloorEffect("Step Spin", 2));
            Effects.Add(new DanceFloorControl.FloorEffect("Button Test", 3));

            Effects.Add(new DanceFloorControl.FloorEffect("Rows Cols", 40));
            Effects.Add(new DanceFloorControl.FloorEffect("Connect Lines", 41));

            Effects.Add(new DanceFloorControl.FloorEffect("Will's Synch", 60));



            // Effects 70: Things that take over the floow and are more interactive.
            Effects.Add(new DanceFloorControl.FloorEffect("Press Colors", 70));
            Effects.Add(new DanceFloorControl.FloorEffect("Press Beat", 71));



            BaseEffects.Add(new BaseEffect("No Effect", 0));
            BaseEffects.Add(new BaseEffect("Clockwise Spin", 1));

            BaseEffects.Add(new BaseEffect("Color Clockwise Spin", 10));


            BaseEffects.Add(new BaseEffect("Beat Pulse", 50));
            BaseEffects.Add(new BaseEffect("Center Pulse", 51));
            BaseEffects.Add(new BaseEffect("Center Lines", 52));
            BaseEffects.Add(new BaseEffect("Center Wave", 53));

            BaseEffects.Add(new BaseEffect("Beat Circles Center", 60));
            BaseEffects.Add(new BaseEffect("Beat Squares Center", 61));
            BaseEffects.Add(new BaseEffect("Beat Circles Corners", 62));
            BaseEffects.Add(new BaseEffect("Beat Squares Corners", 63));

            BaseEffects.Add(new BaseEffect("Checkerboard 2x2", 55));
            BaseEffects.Add(new BaseEffect("Checkerboard 4x4", 56));


            BaseEffects.Add(new BaseEffect("OVER Color Spin", 80));


            Colors.Add(new FloorColor(Color.FromArgb(1, 1, 1), "Change Rainbow"));
            Colors.Add(new FloorColor(Color.FromArgb(2, 2, 2), "Rainbow 2"));
            Colors.Add(new FloorColor(Color.FromArgb(3, 3, 3), "Random"));

            Colors.Add(new FloorColor(Color.Red));
            Colors.Add(new FloorColor(Color.Orange));
            Colors.Add(new FloorColor(Color.Brown));
            Colors.Add(new FloorColor(Color.DarkGoldenrod));
            Colors.Add(new FloorColor(Color.Yellow));
            Colors.Add(new FloorColor(Color.Green));
            Colors.Add(new FloorColor(Color.FromArgb(0,255,0)));
            Colors.Add(new FloorColor(Color.Cyan));
            Colors.Add(new FloorColor(Color.Blue));
            Colors.Add(new FloorColor(Color.DarkBlue));
            Colors.Add(new FloorColor(Color.Indigo));
            Colors.Add(new FloorColor(Color.Violet));
            Colors.Add(new FloorColor(Color.Fuchsia));
            Colors.Add(new FloorColor(Color.Black));
            Colors.Add(new FloorColor(Color.FromArgb(60, 60, 60)));
            Colors.Add(new FloorColor(Color.FromArgb(120, 120, 120)));
            Colors.Add(new FloorColor(Color.FromArgb(180, 180, 180)));
            Colors.Add(new FloorColor(Color.White));

            

            // Draw buttons for the effects.
            for (int i = 0; i < Effects.Count; i++)
            {
                Button b = new Button();
                b.Text = Effects[i].Name;
                b.Width = 100;
                b.Height = 40;
                b.Parent = flowEffects;

                b.Tag = i;
                b.Click += EffectClick;

            }

            // Draw buttons for the colors.
            for (int i = 0; i < Colors.Count; i++)
            {
                Button b = new Button();
                b.BackColor = Colors[i].Name != "" ? Color.White : Colors[i].color;
                b.Text = Colors[i].Name;
                b.Width = 70;
                b.Height = 70;
                b.Margin = new Padding(0);
                b.Parent = flowColors;
                

                b.Tag = i;
                b.Click += ColorClick;

            }

            // Buttons for the base effects
            for (int i = 0; i < BaseEffects.Count; i++)
            {
                Button b = new Button();
                b.Text = BaseEffects[i].Name;
                b.Width = 100;
                b.Height = 40;
                b.Parent = flowBaseEffects;

                b.Tag = i;
                b.Click += BaseEffectClick;

            }


            // Base Colors
            for (int i = 0; i < Colors.Count; i++)
            {
                Button b = new Button();
                b.BackColor = Colors[i].Name != "" ? Color.White : Colors[i].color;
            
                b.Text = Colors[i].Name;
                b.Width = 70;
                b.Height = 70;
                b.Margin = new Padding(0);
                b.Parent = flowBaseColor;

                b.Tag = i;
                b.Click += BaseColorClick;

            }

            // Second Colors
            for (int i = 0; i < Colors.Count; i++)
            {
                Button b = new Button();
                b.BackColor = Colors[i].Name != "" ? Color.White : Colors[i].color;

                b.Text = Colors[i].Name;
                b.Width = 70;
                b.Height = 70;
                b.Margin = new Padding(0);
                b.Parent = flowSecondColor;

                b.Tag = i;
                b.Click += SecondColorClick;

            }

        }

        private void BaseEffectClick(object sender, EventArgs e)
        {
            Button sB = (Button)sender;
            for (int j = 0; j < flowBaseEffects.Controls.Count; j++)
            {
                Button b = (Button)flowBaseEffects.Controls[j];
                b.BackColor = (b == sB) ? Color.LightBlue : Color.White;
            }
            int i = (int)sB.Tag;
            BaseEffects[i].Apply();
        }

        private void ColorClick(object sender, EventArgs e)
        {
            int i = (int)((Button)sender).Tag;
            Colors[i].Apply();
            flowColors.BackColor = Colors[i].color;
        }

        private void BaseColorClick(object sender, EventArgs e)
        {
            int i = (int)((Button)sender).Tag;
            Colors[i].BaseApply();
            flowBaseColor.BackColor = Colors[i].color;
        }
        private void SecondColorClick(object sender, EventArgs e)
        {
            int i = (int)((Button)sender).Tag;
            Colors[i].SecondApply();
        }

        private void EffectClick(object sender, EventArgs e)
        {
            Button sB = (Button)sender;
            for (int j = 0; j < flowEffects.Controls.Count; j++)
            {
                Button b = (Button)flowEffects.Controls[j];
                b.BackColor = (b == sB) ? Color.LightBlue : Color.White;
            }


            int i = (int)sB.Tag;
            Effects[i].Apply();
            
        }

        private void btnTapTempo_Click(object sender, EventArgs e)
        {
        }

        private void btnTapTempo_KeyUp(object sender, KeyEventArgs e)
        {
        }

        private void btnTapTempo_KeyDown(object sender, KeyEventArgs e)
        {
            DFControl.Instance.TapTempo();
        }


        private void btnSendPresses_Click(object sender, EventArgs e)
        {
            DFControl.Instance.SendPressesTo(IPAddress.Parse("2.0.0.243"));
        }

        private void btnReboot_Click(object sender, EventArgs e)
        {
            DFControl.Instance.RebootTeensy();

        }

        private void flowColors_Paint(object sender, PaintEventArgs e)
        {

        }

        private void flowBaseEffects_Paint(object sender, PaintEventArgs e)
        {

        }
        

        private void tbBigness_ValueChanged(object sender, decimal value)
        {
            DFControl.Instance.SetEffectBigness((byte)(((float)tbBigness.Value / 100) * 255));
        }

        private void tbAlpha_ValueChanged(object sender, decimal value)
        {
            DFControl.Instance.SetEffectAlpha((byte)(((float)tbAlpha.Value / 100) * 255));
        }

        private void btnRelease_Click(object sender, EventArgs e)
        {
            DFControl.Instance.ReleaseAllButtons();

        }

        private void button1_Click(object sender, EventArgs e)
        {
        }

        private void button1_MouseDown(object sender, MouseEventArgs e)
        {
            DFControl.Instance.Strobe();

        }


        // Quick functions to read and write the registry.
        static public void RegSaveSetting(string AppName, string Section, string Key, string Value)
        {
            RegistryKey SoftwareKey = Registry.CurrentUser.OpenSubKey("Software", true);
            RegistryKey AppNameKey = SoftwareKey.CreateSubKey(AppName);
            RegistryKey SectionKey = AppNameKey.CreateSubKey(Section);
            SectionKey.SetValue(Key, Value);
        }
        static public string RegLoadSetting(string AppName, string Section, string Key)
        {
            return RegLoadSetting(AppName, Section, Key, "");
        }
        static public string RegLoadSetting(string AppName, string Section, string Key, string Default)
        {
            RegistryKey SoftwareKey = Registry.CurrentUser.OpenSubKey("Software", true);
            RegistryKey AppNameKey = SoftwareKey.CreateSubKey(AppName);
            RegistryKey SectionKey = AppNameKey.CreateSubKey(Section);

            return (string)SectionKey.GetValue(Key, Default);
        }

        private void cbMidi_SelectedIndexChanged(object sender, EventArgs e)
        {
            int stName = cbMidi.SelectedIndex;
            if (stName > -1)
            {
                RegSaveSetting("DanceFloorControl", "Midi", "Device", InputDevice.InstalledDevices[stName].Name);

                try
                {
                    if (inputDevice != null)
                    {
                        inputDevice.StopReceiving();
                        inputDevice.Close();
                        inputDevice = null;
                    }
                } catch (Exception ee)
                {
                    Console.WriteLine(ee.ToString());
                }

                try
                {
                    inputDevice = InputDevice.InstalledDevices[stName];
                    inputDevice.Open();
                    inputDevice.NoteOn += new InputDevice.NoteOnHandler(NoteOn);
                    
                    inputDevice.ControlChange += new InputDevice.ControlChangeHandler(MidiCC);
                    inputDevice.StartReceiving(null);
                }
                catch (Exception ee)
                {

                    MessageBox.Show("Midi Open Error: " + ee.ToString());
                    inputDevice = null;
                }
                
            }
        }

        public void NoteOn(NoteOnMessage msg)
        {
            int noteID = (int)msg.Pitch;
            Console.WriteLine("NoteOn " + msg.Pitch + ", " + msg.Velocity);

            if (msg.Velocity > 0) // for some reason noteOff messages come in, so we must filter them out
            {
                switch (noteID)
                {
                    case 43: // Strobe
                        DFControl.Instance.Strobe();
                        break;

                    case 48: // Tap tempo

                        DFControl.Instance.TapTempo();
                        break;

                }
            }
        }

        public void MidiCC(ControlChangeMessage msg)
        {
            Console.WriteLine("CC: " + (int)msg.Control + ", " + msg.Value);
            int cc = (int)msg.Control;
            Color theCol;
            FloorColor c;

            switch (cc)
            {
                case 0: // Master
                    _dmxData[6] = (byte)(msg.Value * 2);
                    if (_dmxData[6] == 254) _dmxData[6] = 255; // max brightness
                    break;
                case 1: // Strobe
                    _dmxData[5] = (byte)(msg.Value * 2);
                    break;

                case 2: // Effect color
                        /*
                        theCol = DDRGame.ColorFuncs.ColorFromHSV(
                            (float)msg.Value / 127 * 300, 1, 1);

                        c = new FloorColor(theCol);
                        c.Apply();
                        flowColors.BackColor = theCol;
                        */

                    _dmxDataSign[6] = (byte)(msg.Value * 2);
                    if (_dmxDataSign[6] == 254) _dmxDataSign[6] = 255; // max brightness

                    SendArtNetToSign();

                    break;

                case 3: // Base color
                    theCol = DDRGame.ColorFuncs.ColorFromHSV(
                        (float)msg.Value / 127 * 300, 1, 1);

                    c = new FloorColor(theCol);
                    c.BaseApply();
                    flowBaseColor.BackColor = theCol;

                    break;

                case 4:
                    tbBigness.Value = (byte)(((float)msg.Value / 127) * 100);
                    break;

                case 5:
                    tbAlpha.Value = (byte)(((float)msg.Value / 127) * 100);
                    break;

                case 40: // Spotlight Size
                    spotS = (byte)((float)msg.Value / 127 * 30);
                    break;

                case 80: // Scene left
                    _dmxData[0] = (byte)msg.Value;
                    break;
                case 100: // Scene right
                    _dmxData[1] = (byte)msg.Value;
                    break;



                case 36: // Spotlight
                    spotX = (byte)((float)msg.Value / 127 * 47);
                    SendSpot();
                    break;
                case 38: // Spotlight
                    spotY = (byte)((float)msg.Value / 127 * 47);
                    SendSpot();
                    break;
            }

            SendArtNetToJinx();
        }

        public void SendArtNetToJinx()
        {
            var packet = new ArtNetDmxPacket();
            packet.Universe = 10;
            
            packet.DmxData = _dmxData;
            artnet.Send(packet, jinxIP);
        }

        public void SendArtNetToSign()
        {
            var packet = new ArtNetDmxPacket();
            packet.Universe = 10;

            packet.DmxData = _dmxDataSign;
            artnet.Send(packet, signIP);
        }


        public void SendSpot()
        {
            DFControl.Instance.Spotlight(spotX, spotY, spotS);
        }


    }

    



    public class FloorEffect
    {
        public string Name = "";
        public byte ID;

        public FloorEffect(string Name, byte ID)
        {
            this.Name = Name;
            this.ID = ID;
        }

        public void Apply()
        {
            // Apply this effect to the floor.
            DFControl.Instance.SetEffect(this.ID);

        }

    }

    public class BaseEffect
    {
        public string Name = "";
        public byte ID;

        public BaseEffect(string Name, byte ID)
        {
            this.Name = Name;
            this.ID = ID;
        }

        public void Apply()
        {
            // Apply this effect to the floor.
            DFControl.Instance.SetBaseEffect(this.ID);

        }

    }


    public class FloorColor
    {
        public Color color;
        public string Name = "";

        public FloorColor(Color c)
        {
            color = c;
        }
        public FloorColor(Color c, string Name)
        {
            color = c;
            this.Name = Name;
        }
        public void Apply()
        {
            DFControl.Instance.SetEffectColor(color.R, color.G, color.B);
            
        }

        public void BaseApply()
        {
            DFControl.Instance.SetBaseColor(color.R, color.G, color.B);
        }

        public void SecondApply()
        {
            DFControl.Instance.SetSecondColor(color.R, color.G, color.B);
        }

    }


}
