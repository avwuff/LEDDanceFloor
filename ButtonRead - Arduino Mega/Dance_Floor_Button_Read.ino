#include <Wire.h>

#define PIN_CLOCK 11
#define PIN_SHIFTLOAD 10
#define PIN_LED 13
#define BUFSIZE 90

// Input pins are pins 22-29


unsigned long currStat[8];
unsigned long lastStat[8];
unsigned long timer;

byte txBuffer[BUFSIZE];
byte bufWritePos = 0; // Where the buffer is being written to right now 
byte bufReadPos = 0; // Where the buffer is being read from
// Data format:
// 3 bytes <row> <button> <state>
// row is from 0 to 7
// button is from 0 to 31
// state is 0 for unpressed or 1 for pressed.

int count = 0;
int LEDState = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_SHIFTLOAD, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_SHIFTLOAD, LOW);
  
  
  Wire.begin(34);
  Wire.onRequest(requestEvent); // register event

  pinMode(5, INPUT);
  digitalWrite(5, HIGH);// TEMP
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  

  for (int i = 22; i < 30; i++)
  {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH); // Internal pullup
  }

  Serial.begin(115200);
  Serial.println("Ready.");

}

void loop() {
  // put your main code here, to run repeatedly:

  uint32_t changed = 0;
  uint32_t id;
  byte row;
  
  readRegisters();

  for (row = 0; row < 8; row++)
  {  
    
  // TEMPTEMP: ONLY TEMPOARY TO READ ONLY ROW 2
  
    //if (row != 2) continue;
    
    if (currStat[row] != lastStat[row])
    {
      // At least one of the buttons changed.
      // Get a mask as to which button changed.
      changed = currStat[row] ^ lastStat[row];
    
    
      
    
      // Cycle through the buttons.
      for (int i = 0; i < 32; i++)
      {
        // Find the one that changed.
        id = (1UL << i);
        if (changed & id)
        {

          // Add this to the buffer.
          txBuffer[bufWritePos] = row;
          txBuffer[bufWritePos + 1] = i;

  
          
          Serial.print("Row ");
          Serial.print(row, DEC);
          Serial.print(" Button ");
          Serial.print(i, DEC);
          if (currStat[row] & id) 
          {
            txBuffer[bufWritePos + 2] = 0;
            Serial.println(" UP");
          }
          else
          {
            txBuffer[bufWritePos + 2] = 1;
            Serial.println(" DOWN");
          }     
          bufWritePos += 3;
          if (bufWritePos >= BUFSIZE - 3) bufWritePos = 0; // Loop back around for next write

          //Serial.print("Buf write pos ");
          //Serial.println(bufWritePos, DEC);


        }
      }
      
      lastStat[row] = currStat[row];
    }
  }
  
  count++;
  if (millis() - timer >= 1000 || timer > millis())
  {
    timer = millis();
    Serial.print("Scans completed: ");
    Serial.println(count, DEC);
    count = 0;

    digitalWrite(PIN_LED, LEDState);
    LEDState = 1 - LEDState; // Invert
    
  }
  
}

void readRegisters()
{
  byte b;
  byte inp;
  byte j;


  //cli(); // disable interrupts

  byte badRead[8]; // When 1, the read is bad and should be tossed.

  

  // I only care about the first 4 bytes from each shift register.
  // Now clock out the data one bit at a time.

  unsigned long newState[8];

  // Blank the output.
  for (j = 0; j < 8; j++)
  {
    newState[j] = 0;
    badRead[j] = 0; // This means good.
  }

  byte buf[64];

  // Load the data into the buffer.
  digitalWrite(PIN_SHIFTLOAD, LOW); // set LOW to latch in data 
  delay(1);
  digitalWrite(PIN_SHIFTLOAD, HIGH);  // set HIGH to shift data out 
  delay(10);


  
  for (int i = 0; i < 64; i++)
  {

    //digitalWrite(7, HIGH); 
    //byte b = digitalRead(5);
    //digitalWrite(7, LOW);
    
    //if (b == HIGH) digitalWrite(7, HIGH);
    //digitalWrite(7, LOW);
    
    //buf[i] = b;
  

    if ((i & 4)) // I only care about the first 4 bits out of each register -- change this to 1 for the last 4 bits
    {
      
       
       
      // Read all 8 inputs at once.
      inp = PINA;
  
      // Each of the 8 channels comes in as a bit on each of PINA input.  This corresponds to pins 22 to 29.
      for (j = 0; j < 8; j++)
      {
        // Shift left
        newState[j] = newState[j] << 1;
        newState[j] |= (inp & 1);
        inp = inp >> 1; // Shift to get the next value
      }

      
    }
    else
    {
      // Integrity check -- if ANY one of these bits is anything other than 0, it means we've had a bad read.
      // They should always be zero because they are tied to ground on the shift registers.
      // Thus, if they are 1, it means that something got double-clocked.

      // Read all 8 inputs at once.
      inp = PINA;
  
      // Each of the 8 channels comes in as a bit on each of PINA input.  This corresponds to pins 22 to 29.
      for (j = 0; j < 8; j++)
      {
        // Shift left
        if ((inp & 1) == 1)
        {
          // BAD READ
          badRead[j]++; // We will just continue the read
          
        }
        inp = inp >> 1; // Shift to get the next value
      }
    }


    

    // Tickle the clock to get the next value.
    digitalWrite(PIN_CLOCK, HIGH);// set CLOCK to HIGH
    delayMicroseconds(40);
    digitalWrite(PIN_CLOCK, LOW); // set CLOCK to LOW
    delayMicroseconds(10);
  }

  digitalWrite(PIN_SHIFTLOAD, LOW); // TURN shiftload back off


  // Successful read -- copy to current status.
  for (j = 0; j < 8; j++)
  {
    if (badRead[j] == 0)
    {
      currStat[j] = newState[j];
    } 
    else if (badRead[j] == 32)
    {
      // Just not connected.
      Serial.print("Not connected? Row ");
      Serial.println(j);
    }
    else
    {
      Serial.print("Bad Read! Row ");
      Serial.print(j);
      Serial.print(", # bad: ");
      Serial.println(badRead[j]);
    }
  }

  

  /*
  Serial.print("Read: ");
  for (int i = 0; i < 64; i++)
  {
     Serial.print(buf[i], BIN);

  }
  Serial.println(";");
  */
  
  // sei(); // enable interrupts

  //delay(50);
  
}

void requestEvent() 
{

  // Called from the Teensy so that button data can be retrieved.
  // Is there an event?
  if (bufWritePos != bufReadPos) // If these are the same there is no event
  {
    Wire.write(txBuffer[bufReadPos]);
    Wire.write(txBuffer[bufReadPos + 1]);
    Wire.write(txBuffer[bufReadPos + 2]);

    bufReadPos += 3;
    if (bufReadPos >= BUFSIZE - 3) bufReadPos = 0; // Loop back around for next read
  }
  else
  {
    // No event. Send three 255's
    Wire.write(0xFF);
    Wire.write(0xFF);
    Wire.write(0xFF);
  }
}




