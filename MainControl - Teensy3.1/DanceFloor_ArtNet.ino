// Dance Floor Controller
// Written by Av, www.avbrand.com


#include <Artnet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OctoWS2811.h>
#include <Wire.h>

// EFFECTS
#include "effect_explo.h"
#include "effect_rowcols.h"
#include "effect_synch.h"
#include "effect_stepcols.h"



#define MIX_ALPHA(c1, c2, a) (byte)(((int)c1 * (int)(255 - a) / 256) + ((int)c2 * (int)a / 256) )

Effect_Explo    effect1;
Effect_RowCol   effect4;
Effect_Synch    effectS;
Effect_StepCols    effectSC;


byte EFFECTS_MODE = 1; // Test effects mode, 1 is explode from press
// modes :
// 1 - explode
// 2 - green spin on press
// 3 - green on down, red on up
// 4 row col
// 5 - explode square

// Effects colours, if all are '1' it is rainbow.
byte EffectColor[3]; // R G B
byte BaseColor[3]; // R G B
byte SecondColor[3]; // R G B
byte EffectRainbow = 0; // Used for one of the effects modes
byte BaseRainbow = 0;

byte EffectMem[16][16]; // Memory buffer that any effect can use.


byte EFFECT_BIGNESS = 255; // How 'Big' the effect is
byte EFFECT_ALPHA = 255;   // How strong the effect is on the overlay


// BASE EFFECT
// This effect mode will take the base pixel data and do something more interesting with it.
// Such as spins, etc.
byte BASE_EFFECT = 0;
// Options: 
// 0 - normal
// 1 - rotate clockwise the current color with a slight shade


// Should restart Teensy 3, will also disconnect USB during restart
// From http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0337e/Cihcbadd.html
// Search for "0xE000ED0C"
// Original question http://forum.pjrc.com/threads/24304-_reboot_Teensyduino%28%29-vs-_restart_Teensyduino%28%29?p=35981#post35981
#define RESTART_ADDR       0xE000ED0C
#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

// OctoWS2811 settings
const int ledsPerStrip = 320; // change for your setup
const byte numStrips = 8; // change for your setup
const int numLeds = ledsPerStrip * numStrips;

// We're going to receive only 1/10th the number of actual channels, as we'll be grouping the LEDS into 3s.


const int numberOfChannels = (numLeds / 10) * 3; // Total number of channels you want to receive (1 led = 3 channels)

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_BRG | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Artnet settings

Artnet artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// Button receive
EthernetUDP cmdRecv;
#define CMD_RECV_PORT 9934
byte cmdBuf[20];
byte cmdBufIndex = 0;
byte cmdNextEscape = 0; // Next byte from command buffer is after escape.

// Tap Tempo
long lastTap = 0;
int tapCount = 0;
float beatTempo = 0;
long lastBeat = 0;



// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
int previousDataLength = 0;
long lastTick = 0;
byte spinIndex = 0;

// Change ip and mac address for your setup
byte ip[] = {2, 0, 0, 10};
byte gw[] = {2, 0, 0, 1};
byte mac[] = {0x04, 0xE9, 0x44, 0x33, 0x69, 0xEC};
byte snm[] = {255, 0, 0, 0};

byte floorDirty = 0; // When 1, we've received only half of the DMX data.

// SCREEN BUFFERS

uint8_t floorB[16][16][3]; // X, Y, RGB  -- DMX layer
uint8_t effectB[16][16][10][4]; // X, Y, subindex, RGBA -- Effects layer

uint8_t butDown[16][16]; // Which buttons are down right now


// Button Output!
EthernetUDP btnUDP;

byte SendPresses = 0; // If this is 1, send presses to the target IP.
IPAddress target(2, 0, 0, 243);

// Gamma function

#define GAMMA(c) ((uint16_t)(c * c)) >> 8

void setup()
{
  Serial.begin(115200);
  Serial.println("Startup. ");

  EffectColor[0] = 1; // Default to Rainbow.
  EffectColor[1] = 1;
  EffectColor[2] = 1;


  // TPM2 stuff
  //Ethernet.begin(mac,ip,gw,gw,snm);
  //Udp.begin(65506L);


  beatTempo = 1000;
  

  artnet.begin(mac, ip, gw, snm);
  leds.begin();
  Wire.begin(); // I2C is used to retrieve button presses from the Arduino

  btnUDP.begin(12232);

  // Listening port for commands
  cmdRecv.begin(CMD_RECV_PORT);
  


  Serial.print("Max universes: ");
  Serial.println(maxUniverses, DEC);



  initTest();
  //initTest2();
  
  // TURN THIS
 RowTest();



  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);

  //for (;;)
  // initTest();

  /*
    Serial.println("Testing x5, y9");
    Serial.println(coordToPixNum(5, 9), DEC);
    Serial.println("Testing x12, y6");
    Serial.println(coordToPixNum(12, 6), DEC);
    Serial.println("Testing x15, y1");
    Serial.println(coordToPixNum(15, 1), DEC);
  */


}


void loop()
{
  // we call the read function inside the loop
  artnet.read();

  // Check if any floor buttons have been pressed or released.
  checkButtons();
  
  
  //tpmRead();f
  checkCommands();
  
  long m = millis();

  if (lastBeat > m || m - lastBeat >= beatTempo)
  {
    lastBeat = m;
    songBeat();
  }
  


  // Tick on the serial port once a second, so we know the Teensy is alive.
  if (m - lastTick > 25 || m < lastTick)
  {
    if (!floorDirty)
    {
      lastTick = m;
      // Serial.println("Tick.");
  
      effectTick();
      renderFloor();
    }
  }
}

void checkCommands()
{
  uint16_t pSize = cmdRecv.parsePacket();
  byte b = 0;
  
  if (pSize > 0)
  {
    while (cmdRecv.available())
    {
      b = cmdRecv.read();
      if (b == 0xFF)
      {
        // Escape character.
        cmdNextEscape = 1;
      }
      else
      {
        if (cmdNextEscape)
        {
          cmdNextEscape = 0;
          // Escaped character.
          switch (b)
          {
            case 0x01: // Start of packet.
              cmdBufIndex = 0;
              break;
            case 0x02: // End of packet
              parseCmdPacket();
              break;
            
            case 0x10: // literal 0xFF
              if (cmdBufIndex < 20)
              {
                cmdBuf[cmdBufIndex] = 0xFF;
                cmdBufIndex++;
              }          

              break;            
          }
        }
        else
        {
          // Add to the buffer.
          if (cmdBufIndex < 20)
          {
            cmdBuf[cmdBufIndex] = b;
            cmdBufIndex++;
          }          
        }
      }
    }
  }
}

void parseCmdPacket()
{
  // Figure out what the command packet is telling us to do.
  switch (cmdBuf[0])
  {

    case 'A': // Effect Alpha
      Serial.print("Alpha ");
      Serial.println(EFFECT_ALPHA);
      
      EFFECT_ALPHA = cmdBuf[1];
      break;
      
    case 'B': // Base effect mode
      BASE_EFFECT = cmdBuf[1];
      break;

    case 'C': // Set effect color

      EffectColor[0] = cmdBuf[1];
      EffectColor[1] = cmdBuf[2];
      EffectColor[2] = cmdBuf[3];
      break;

    case 'D': // Set base color

      BaseColor[0] = cmdBuf[1];
      BaseColor[1] = cmdBuf[2];
      BaseColor[2] = cmdBuf[3];
      break;

    case 'E':  // Set effect!
      switchEffectMode(cmdBuf[1]);
      
      break;


    case 'F': // Set second color

      SecondColor[0] = cmdBuf[1];
      SecondColor[1] = cmdBuf[2];
      SecondColor[2] = cmdBuf[3];
      break;

    case 'T': // Tap tempo.
      // In order for tap tempo to work, we need four taps.
      // If it has been more than 3 seconds since the last tap, start the counter again.
      Serial.println("Tap!");
      
      if (lastTap > millis() || millis() - lastTap > 4000)
      {
        tapCount = 0;
        lastTap = millis();
        
      }
      else
      {
        tapCount++;
        // Once you get to 4 taps, calculate the tempo.
        if (tapCount >= 3)
        {
          setTempo((float)(millis() - lastTap) / (float)tapCount);
        }
      }

      break;

    case 'P': // Send presses to this IP address.

      SendPresses = 1;
      target = IPAddress(cmdBuf[1], cmdBuf[2], cmdBuf[3], cmdBuf[4]);
      break;

    case 'Q': // Strobe
      if (EFFECTS_MODE > 0) switchEffectMode(0);
      Strobe();
      break;

    case 'R': // Reboot chip
      WRITE_RESTART(0x5FA0004);
      break;

    case 'S': // Effect bigness
      EFFECT_BIGNESS = cmdBuf[1];
      break;

    case 'Z': // Release all buttons
      ReleaseAllButtons();
      break;

    case 'M': // Spotlight mode
      DrawSpotlight(cmdBuf[1], cmdBuf[2], cmdBuf[3]);
      break;
   
  }
}

void setTempo(float tempo)
{
  // Set the tempo and reset the next tempo beat.
  beatTempo = tempo;
  lastBeat = millis() - tempo;
  Serial.print("Setting tempo to ");
  Serial.println(beatTempo);
  
}

void DrawSpotlight(byte x, byte y, byte s)
{

  byte r, g, b;
  GetBaseColor(&r, &g, &b);

  // Paint a spotlight at this location (x and y is 0 to 47)
  for (byte i = 0; i < s; i++)
  {

    FCircle(x, y, i, r, g, b, 255);
  }
  
}

void switchEffectMode(byte m)
{

  EFFECTS_MODE = m;
  Serial.print("Effect changed to ");
  Serial.println(EFFECTS_MODE);

  // Reset the effects memory
  for (byte x = 0; x < 16; x++)
  {
    for (byte y = 0; y < 16; y++)
    {
      EffectMem[x][y] = 0; 
    }
  }
  
}

void checkButtons()
{
  byte num;
  byte packet[3];
  byte x, y;
  // char reply[4];

  // Check if there is data from the Button Arduino
  //Serial.println("Requesting...");

  // Read UP TO 3 buttons.
  byte bCount = 0;
  

  do
  {
    
    
    Wire.requestFrom(34, 3, false);
    if (Wire.available())
    {
      //Serial.println("Reading...");
      num = 0;
      while (Wire.available()) {
        packet[num] = Wire.receive();
        num++;
        if (num >= 3) break;
      }
  
      //Serial.println("Parsing");
      if (packet[0] != 255)
      {
        // Convert the button press to an (x, y) coordinate.
        // X is simple
        buttonNumToXY(packet[0], packet[1], &x, &y);
  
        if (butDown[x][y] != packet[2])
        {
          butDown[x][y] = packet[2];
          buttonChange(x, y, packet[2]);
        }
        Serial.print("Row ");
        Serial.print(packet[0], DEC);
        Serial.print(" Button ");
        Serial.print(packet[1], DEC);
        Serial.print(" State ");
        Serial.println(packet[2], DEC);
  
        //String ip       = "192.168.2.46"; // the remote IP address
        //int port        = 8888;        // the destination port
  
  
        //reply[0] = 'B';
        //reply[1] = x;
        //reply[2] = y;
        //reply[3] = packet[2];
  
  
        //Serial.println("Sending via UDP");
  
  
      }
  
      //Serial.println("Done");
  
    }
    else
    {
      // No buttons so just exit
      return;
      
    }
    bCount++;
  } while (bCount < 3);
}

void buttonChange(byte x, byte y, byte state)
{

  if (SendPresses)
  {
    long mStart = millis();
    
    btnUDP.beginPacket(target, 8888);
    btnUDP.write('B');
    btnUDP.write(x);
    btnUDP.write(y);
    btnUDP.write(state);
    btnUDP.endPacket();

    // Must not be connected, turn off sending presses.
    if (millis() - mStart > 600) SendPresses = 0;
    
  }

  
  switch (EFFECTS_MODE)
  {
    // Explosion ones
    case 1: case 5: case 6: case 7: case 8: case 9:
    case 10: case 15: case 16: case 17: case 18: case 19:
    case 30: 
      effect1.buttonChange(x, y, state); 
      break;

    case 3: // Button test
      if (state)
      {
        setEffectPixel(x, y, 0, 255, 0, 255);
      }
      else
      {
        setEffectPixel(x, y, 255, 0, 0, 255);
      }
      break;
            
    
    case 40: case 41:
      effect4.buttonChange(x, y, state); 
      break;


    case 60: // Will's Synch
      effectS.buttonChange(x, y, state); 
      break;

    case 70: case 71:
      effectSC.buttonChange(x, y, state); 
      break;
    
  }
   
}

void effectTick()
{
  switch (EFFECTS_MODE)
  {
    case 0:
      fadeEffectsBuffer(25);
      break;
    case 1: case 5: case 6: case 7: case 8: case 9:
    case 10: case 15: case 16: case 17: case 18: case 19:
    case 30:
      effect1.Tick(); 
      break;
    case 2:
      ButSpin(); // Spins any buttons that are held down.
      //RowTest();
      break;

    case 40: case 41:
      effect4.Tick(); 
      break;
    
    case 60: // Will's synch
      effectS.Tick(); 
      break;
    
    case 70: case 71: // Step Colors
      effectSC.Tick(); 
      break;

  }
}

void songBeat()
{

  BaseBeat();
  
  switch (EFFECTS_MODE)
  {
    case 10: case 15: case 16: case 17: case 18: case 19:
      effect1.Beat();
      break;
    
    case 60: // Will's synch
      effectS.Beat();
      break;

  }


  
}

void Strobe()
{
  byte r, g, b;
  GetBaseColor(&r, &g, &b);
  
  // Flash the effects buffer with the Base Effects Color.
  for (byte x = 0; x < 16; x++)
  {
    for (byte y = 0; y < 16; y++)
    {
      for (byte i = 0; i < 10; i++)
      {
        
        effectB[x][y][i][0] = r;
        effectB[x][y][i][1] = g;
        effectB[x][y][i][2] = b;
        effectB[x][y][i][3] = 255;
      }
    }
  }
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
/*
  Serial.print("dmx frame: ");
  Serial.print(universe, DEC);
  Serial.print(" : ");
  Serial.println(length, DEC);
*/

  sendFrame = 1;

  byte x;
  byte y;


  // Store which universe has got in
  if ((universe - startUniverse) < maxUniverses)
    universesReceived[universe - startUniverse] = 1;

  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      sendFrame = 0;
      floorDirty = 1; // Mark the floor as not ready for painting.
      break;
    }
  }

  // read universe and put into the right part of the display buffer
  // universe++; // REMOVE THIS

  for (int i = 0; i < length / 3; i++)
  {

    int led = i + (universe - startUniverse) * (previousDataLength / 3);

    //Serial.print("LED: ");
    //Serial.print(led);
    //Serial.print(" ");
    //Serial.println(data[i * 3]);

    // Convert this to pixel co-ordinates.
    x = led % 16;
    y = led / 16;

    if (x < 16 && y < 16)
    {

      //Serial.print(i);
      //Serial.print(" ");
      //Serial.println(data[i * 3 + 2]);

      floorB[x][y][0] = (byte)data[i * 3];
      floorB[x][y][1] = (byte)data[i * 3 + 1];
      floorB[x][y][2] = (byte)data[i * 3 + 2];

      // setFloorPixel(x, y, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);

      /*
        for (byte n = 0; n < 10; n++)
        {
        leds.setPixel(led * 10 + n, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
        //leds.setPixel(led * 10 + n, 255, 255, 255);
        }
      */

    }
  }
  previousDataLength = length;

  if (sendFrame)
  {
    lastTick = millis();
    floorDirty = 0; // Floor is intact and ready to be painted.

    effectTick();
    renderFloor();
    //leds.show();
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}

void setFloorPixel(byte x, byte y, uint8_t r, uint8_t g, uint8_t b)
{
  floorB[x][y][0] = r;
  floorB[x][y][1] = g;
  floorB[x][y][2] = b;
}

void setEffectPixel(byte x, byte y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  // Set a pixel in the effects buffer.
  if (x >= 0 && x < 16 && y >= 0 && y < 16)
  {
    for (byte i = 0; i < 10; i++)
    {
      effectB[x][y][i][0] = r;
      effectB[x][y][i][1] = g;
      effectB[x][y][i][2] = b;
      effectB[x][y][i][3] = a;
    }
  }
}

// We support the ability to have a larger "fake" resolution on the floor, which is interpolated by addressing the pixels in subgroups.
// The actual resolution of the floor is 16x16, but by splitting each tile into a 3x3 grid, we effectively have a resolution of 48x48.
void setEffectPixelLarge(byte x, byte y, byte r, byte g, byte b, byte a)
{
  // Convert the larger X and Y into the smaller X and Y and subpixel number.
  byte sx, sy, si, si2;
  locateSubPixel(x, y, &sx, &sy, &si, &si2);


  if (sx >= 0 && sx < 16 && sy >= 0 && sy < 16)
  {
    if (si < 10)
    {
      effectB[sx][sy][si][0] = r;
      effectB[sx][sy][si][1] = g;
      effectB[sx][sy][si][2] = b;
      effectB[sx][sy][si][3] = a;
    }
    if (si2 < 10)
    {
      effectB[sx][sy][si2][0] = r;
      effectB[sx][sy][si2][1] = g;
      effectB[sx][sy][si2][2] = b;
      effectB[sx][sy][si2][3] = a;
    }
  }
}

void locateSubPixel(byte x, byte y, byte *sx, byte *sy, byte *si, byte *si2)
{
  // The pixels are arranged like so:
  /*    0   1   2
       ___________
    0 |  7  8   9 |
      |6         0|
      |           |
    1 |5         1|
      |           |
    2 |4____3____2|
  */

  *sx = x / 3;
  *sy = y / 3;
  *si2 = 255;


  byte spx = x - (*sx * 3);
  byte spy = y - (*sy * 3);
  byte btemp;

  // Determine whether this is square 0, 1, 2 or 3 in the array.
  /* Squares:

      1 2
      0 3

      if the Y is even, it's either 1 or 2
      if the X is even, it's 1

  */

  if (*sy % 2 == 0) {   // Either squares 1 or 2
    if (*sx % 2 == 0) { // Square 1 is rotated 90 degrees clockwise from square 0.
      btemp = spy;
      spy = 2 - spx;
      spx = btemp;

    } else {            // Square 2 is rotated 180 degrees
      spy = 2 - spy;
      spx = 2 - spx;
    }
  } else { // Either squares 0 or 3
    if (*sx % 2 != 0) { // Square 3 is rotated 270 degrees
      btemp = spx;
      spx = 2 - spy;
      spy = btemp;
    }
  }

  switch (spx)
  {
    case 0:
      switch (spy)
      {
        case 0: *si = 6; *si2 = 7; break;
        case 1: *si = 5; break;
        case 2: *si = 4; break;
      }
      return;
    case 1:
      switch (spy)
      {
        case 0: *si = 8; break;
        case 1: *si = 255; break;
        case 2: *si = 3; break;
      }
      return;
    case 2:
      switch (spy)
      {
        case 0: *si = 9; *si2 = 0; break;
        case 1: *si = 1; break;
        case 2: *si = 2; break;
      }
      return;

  }


}

void renderFloor()
{

  // Take the data in the floor and render it.
  int n;
  byte i;
  byte a;

  byte r, g, b;

  BaseEffectTick();
  

  for (byte x = 0; x < 16; x++)
  {
    for (byte y = 0; y < 16; y++)
    {
      /*
        if (butDown[x][y])
        {
        floorB[x][y][0] = 255;
        floorB[x][y][1] = 255;
        floorB[x][y][2] = 255;
        }
      */

      BaseEffectReadyPixel(&x, &y);

      n = (int)coordToPixNum(x, y) * 10;
      for (i = 0; i < 10; i++)
      {
        r = floorB[x][y][0];
        g = floorB[x][y][1];
        b = floorB[x][y][2];

        ApplyBaseEffect(&r, &g, &b, &x, &y, &i);


        
        
        if (effectB[x][y][i][3] > 0)
        {
          // Perform alpha blending
          a = effectB[x][y][i][3];

          a = (byte)((int)a * (int)EFFECT_ALPHA / 256);
          
          
          leds.setPixel(n + i,
                        GAMMA((byte)(((int)r * (int)(255 - a) / 256) + ((int)effectB[x][y][i][0] * (int)a / 256) )),
                        GAMMA((byte)(((int)g * (int)(255 - a) / 256) + ((int)effectB[x][y][i][1] * (int)a / 256) )),
                        GAMMA((byte)(((int)b * (int)(255 - a) / 256) + ((int)effectB[x][y][i][2] * (int)a / 256) ))
                       ); 
/*
          leds.setPixel(n + i,
                        GAMMA(MIX_ALPHA(r, effectB[x][y][i][0], a)),
                        GAMMA(MIX_ALPHA(r, effectB[x][y][i][1], a)),
                        GAMMA(MIX_ALPHA(r, effectB[x][y][i][2], a))
                       );
  */      
        
        }
        else
        {
          leds.setPixel(n + i, GAMMA(r), GAMMA(g), GAMMA(b));
        }
      }
    }
  }


  leds.show();

}



byte coordToPixNum(byte x, byte y)
{
  // Converts an X and Y from 0-15 to the pixel number.
  // Excel formula:
  // =(INT((15-C24)/2)*4) + MOD((15-C24), 2) + MOD(C23, 2) + ((1-MOD((15-C24), 2)) * MOD(C23, 2) * 2) + B28*32

  // =(INT((15-y)/2)*4) + MOD((15-y), 2) + MOD(x, 2) + ((1-MOD((15-y), 2)) * MOD(x, 2) * 2) + (x/2)*32
  return ((15 - y) / 2 * 4) + (15 - y) % 2 + x % 2 + ((1 - (15 - y) % 2) * x % 2 * 2) + (x / 2) * 32;
}

void initTest2()
{

  for (byte y = 0; y < 16; y++)
  {
    for (byte x = 0; x < 16; x++)
    {
      floorB[x][y][0] = 50;
      floorB[x][y][1] = 50;
      floorB[x][y][2] = 50;
      renderFloor();
      delay(3);
    }
  }

}

void initTest()
{
  for (int i = 0 ; i < numLeds ; i++)
    leds.setPixel(i, 127, 0, 0);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds  ; i++)
    leds.setPixel(i, 0, 127, 0);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds  ; i++)
    leds.setPixel(i, 0, 0, 127);
  leds.show();
  delay(500);
  for (int i = 0 ; i < numLeds  ; i++)
    leds.setPixel(i, 0, 0, 0);
  leds.show();
}





void ReleaseAllButtons()
{
  for (byte y = 0; y < 16; y++)
  {
    for (byte x = 0; x < 16; x++)
    {
      if (butDown[x][y])
      {
        butDown[x][y] = 0;
        buttonChange(x, y, 0); // raise it. 
      }
    }
  }

  
}




// EFFECTS


void RowTest()
{

  for (byte y = 0; y < 60; y++)
  {
    fadeEffectsBuffer(25);

    if (y < 48)
    {
      for (byte x = 0; x < 48; x++)
      {
        setEffectPixelLarge(x, y, 255, 255, 255, 255);
        setEffectPixelLarge(y, x, 0, 0, 255, 255);
      }
    }
    renderFloor();
    delay(20);

  }
}


void fadeEffectsBuffer(int byAmount)
{
  for (byte y = 0; y < 16; y++)
  {
    for (byte x = 0; x < 16; x++)
    {
      for (byte i = 0; i < 10; i++)
      {
        if (effectB[x][y][i][3] > 0)
          effectB[x][y][i][3] = reduce(effectB[x][y][i][3], byAmount);
      }
    }
  }

}

byte reduce(byte v, int byAmount)
{
  if (v > byAmount) return v - byAmount;
  return 0;
}

void ButSpin()
{
  for (byte y = 0; y < 16; y++)
  {
    for (byte x = 0; x < 16; x++)
    {
      if (butDown[x][y])
      {
        effectB[x][y][spinIndex][0] = 0; // R
        effectB[x][y][spinIndex][1] = 255; // G
        effectB[x][y][spinIndex][2] = 0; // B
        effectB[x][y][spinIndex][3] = 255; // Alpha
        
      }
    
    }
  }


  spinIndex = (spinIndex + 1) % 10;


  fadeEffectsBuffer(25);


  
}


void GetEffectColor(byte *r, byte *g, byte *b)
{
  
  // Set color
  if (EffectColor[0] == 2 && EffectColor[1] == 2 && EffectColor[2] == 2) // Rainbow 1 effect
  {
    // Next color in the rainbow.
    Wheel(EffectRainbow * 20, r, g, b); // Pick a random color
    EffectRainbow++;
    
  } 
  else if (EffectColor[0] == 3 && EffectColor[1] == 3 && EffectColor[2] == 3) // Random
  {
    // Next color in the rainbow.
    Wheel(random(255), r, g, b); // Pick a random color    
  } 
  else
  {
    *r = EffectColor[0];
    *g = EffectColor[1];
    *b = EffectColor[2];      
  }
}


void GetBaseColor(byte *r, byte *g, byte *b)
{
  
  // Set color
  if (BaseColor[0] == 2 && BaseColor[1] == 2 && BaseColor[2] == 2) // Rainbow 1 effect
  {
    // Next color in the rainbow.
    Wheel(BaseRainbow * 20, r, g, b); // Pick a random color
    BaseRainbow++;
    
  } 
  else if (BaseColor[0] == 3 && BaseColor[1] == 3 && BaseColor[2] == 3) // Random
  {
    // Next color in the rainbow.
    Wheel(random(255), r, g, b); // Pick a random color    
  } 
  else
  {
    *r = BaseColor[0];
    *g = BaseColor[1];
    *b = BaseColor[2];      
  }
}


void GetSecondColor(byte *r, byte *g, byte *b)
{
  
  // Set color
  if (SecondColor[0] == 2 && SecondColor[1] == 2 && SecondColor[2] == 2) // Rainbow 1 effect
  {
    // Next color in the rainbow.
    Wheel(BaseRainbow * 20, r, g, b); // Pick a random color
    BaseRainbow++;
    
  } 
  else if (SecondColor[0] == 3 && SecondColor[1] == 3 && SecondColor[2] == 3) // Random
  {
    // Next color in the rainbow.
    Wheel(random(255), r, g, b); // Pick a random color    
  } 
  else
  {
    *r = SecondColor[0];
    *g = SecondColor[1];
    *b = SecondColor[2];      
  }
}


void buttonNumToXY(byte row, byte bNum, byte* outX, byte* outY)
{
  // Just use a lookup table, will be faster.
  switch (bNum)
  {
    case 0: *outX = 1; *outY = 1; break;
    case 1: *outX = 1; *outY = 0; break;
    case 2: *outX = 0; *outY = 0; break;
    case 3: *outX = 0; *outY = 1; break;
    case 4: *outX = 1; *outY = 3; break;
    case 5: *outX = 1; *outY = 2; break;
    case 6: *outX = 0; *outY = 2; break;
    case 7: *outX = 0; *outY = 3; break;
    case 8: *outX = 1; *outY = 5; break;
    case 9: *outX = 1; *outY = 4; break;
    case 10: *outX = 0; *outY = 4; break;
    case 11: *outX = 0; *outY = 5; break;
    case 12: *outX = 1; *outY = 7; break;
    case 13: *outX = 1; *outY = 6; break;
    case 14: *outX = 0; *outY = 6; break;
    case 15: *outX = 0; *outY = 7; break;
    case 16: *outX = 1; *outY = 9; break;
    case 17: *outX = 1; *outY = 8; break;
    case 18: *outX = 0; *outY = 8; break;
    case 19: *outX = 0; *outY = 9; break;
    case 20: *outX = 1; *outY = 11; break;
    case 21: *outX = 1; *outY = 10; break;
    case 22: *outX = 0; *outY = 10; break;
    case 23: *outX = 0; *outY = 11; break;
    case 24: *outX = 1; *outY = 13; break;
    case 25: *outX = 1; *outY = 12; break;
    case 26: *outX = 0; *outY = 12; break;
    case 27: *outX = 0; *outY = 13; break;
    case 28: *outX = 1; *outY = 15; break;
    case 29: *outX = 1; *outY = 14; break;
    case 30: *outX = 0; *outY = 14; break;
    case 31: *outX = 0; *outY = 15; break;
  }
  *outX = *outX + (row * 2);
}

uint32_t Wheel(byte WheelPos, byte* outR, byte* outG, byte* outB) {
  if(WheelPos < 85) {
   *outR = WheelPos * 3;
   *outG = 255 - WheelPos * 3;
   *outB = 0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   *outR = 255 - WheelPos * 3;
   *outG = 0;
   *outB = WheelPos * 3;
  } else {
   WheelPos -= 170;
   *outR = 0;
   *outG = WheelPos * 3;
   *outB = 255 - WheelPos * 3;
  }
}
int8_t sgn(int val) {
 if (val < 0) return -1;
 if (val==0) return 0;
 return 1;
}
