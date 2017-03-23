
//#include "effect_explo.h"


Effect_Explo::Effect_Explo()
{
 //this->down_at = 0;
 //this->down = 0;
  this->currExplo = 0;
  
}

// Tick
void Effect_Explo::Tick()
{
  int x, y;

  byte bigness = (byte)(48 * (int)EFFECT_BIGNESS / 256);


  // Animate the explosions.
  for (byte i = 0; i < 20; i++)
  {
    if (this->exploState[i] < bigness)
    {
      // Draw a ring around the X,Y co-ord.
      this->exploState[i]++;
      byte c = 255 - this->exploState[i] * 3;

      byte cr, cg, cb;

      if (this->ExploColor[i][0] == 1 && this->ExploColor[i][1] == 1 && this->ExploColor[i][2] == 1) // Rainbow 1 effect
      {
        Wheel(this->exploState[i] * 25, &cr, &cg, &cb); // Pick a random color
      }
      else 
      {
        cr = this->ExploColor[i][0];
        cg = this->ExploColor[i][1];
        cb = this->ExploColor[i][2];      
      }
      
      

      /*
        for (float a = 0; a < 3.141 * 2; a += 0.1)
        {
        x = ((cos(a) * (float)exploState[i]) + ((float)exploXY[i][0] * 3) + 1);
        y = ((sin(a) * (float)exploState[i]) + ((float)exploXY[i][1] * 3) + 1);
        //Serial.print(x);
        //Serial.print(", ");
        //Serial.println(y);

        //setEffectPixel((byte)x, (byte)y, c, c, c, 255);
        setEffectPixelLarge((byte)x, (byte)y, c, c, c, 255);
        }*/

      //c = 0;

      // What style are we drawing?
      switch(this->exploStyle[i])
      {
        case 1: case 10: // Circle
          FCircle(this->exploXY[i][0] * 3 + 1, this->exploXY[i][1] * 3 + 1, this->exploState[i], cr, cg, cb, 255);
          break;

        case 5: case 15: // Square
          this->FSquare(this->exploXY[i][0] * 3 + 1, this->exploXY[i][1] * 3 + 1, this->exploState[i], cr, cg, cb, 255);
          break;
  
        case 6: case 16: // Cross
          this->FCross(this->exploXY[i][0], this->exploXY[i][1], this->exploState[i], cr, cg, cb, 255);
          break;
          
        case 7: case 17: // Spiral
          this->FSpiral(this->exploXY[i][0] * 3 + 1, this->exploXY[i][1] * 3 + 1, this->exploState[i], cr, cg, cb, 255);
          break;
          
        case 8: case 18: // Star
          this->FStar(this->exploXY[i][0] * 3 + 1, this->exploXY[i][1] * 3 + 1, this->exploState[i], cr, cg, cb, 255);
          break;

        case 9: case 19: // Spinning triangle
          this->FTriangle(this->exploXY[i][0] * 3 + 1, this->exploXY[i][1] * 3 + 1, this->exploState[i], cr, cg, cb, 255);
          break;


        case 30: // color bomb
          // Draw to the size
          for (byte j = 1; j < this->exploState[i]; j++)
          {
             FCircle(this->exploXY[i][0] * 3 + 1, this->exploXY[i][1] * 3 + 1, j, cr, cg, cb, 255);
          }

          // Randomely choose to end at this time
          if (this->exploState[i] > 5 && random(5) == 3) this->exploState[i] = 255;
          break;
        
      }
      

    }
  }

  fadeEffectsBuffer(25);
 
 
}


void Effect_Explo::buttonChange(byte x, byte y, byte state)
{
  if (state)
  {
    switch (EFFECTS_MODE)
    {
      case 1: case 5: case 6: case 7: case 8: case 9: 
      case 30:
        this->NewExplosion(x, y, EFFECTS_MODE);
        break;
    }
    
  }
  
}

void Effect_Explo::Beat()
{
  // Called on every beat.
  switch (EFFECTS_MODE)
  {
    case 10: case 15: case 16: case 17: case 18: case 19:

      // Make an explosion on every button that is down.
      for (byte x = 0; x < 16; x++)
      {
        for (byte y = 0; y < 16; y++)
        {
          if (butDown[x][y])
          {
            this->NewExplosion(x, y, EFFECTS_MODE);
          }
        }
      }

      break;
  }
  
}

void Effect_Explo::NewExplosion(byte x, byte y, byte style)
{

  // Explosion styles:
  // 0 - circle
  // 1 - square
  

  
  Serial.print("Explosion: ");
  Serial.print(x);
  Serial.print(", ");
  Serial.println(y);

  

  // Add an explosion to the floor here.
  this->currExplo++;
  if (this->currExplo >= 20) this->currExplo = 0;
  
  byte n = this->currExplo;
  
  this->exploXY[n][0] = x;
  this->exploXY[n][1] = y;
  this->exploState[n] = 0; // Reset state.
  this->exploStyle[n] = style;

  GetEffectColor(&this->ExploColor[n][0], &this->ExploColor[n][1], &this->ExploColor[n][2]);
  
}

void Effect_Explo::NewBaseExplosion(byte x, byte y, byte style)
{

  // Explosion styles:
  // 0 - circle
  // 1 - square
  
  // Add an explosion to the floor here.
  this->currExplo++;
  if (this->currExplo >= 20) this->currExplo = 0;
  
  byte n = this->currExplo;
  
  this->exploXY[n][0] = x;
  this->exploXY[n][1] = y;
  this->exploState[n] = 0; // Reset state.
  this->exploStyle[n] = style;

  GetBaseColor(&this->ExploColor[n][0], &this->ExploColor[n][1], &this->ExploColor[n][2]);
  
}




void Effect_Explo::FSquare(int x, int y, int radius, int r, int g, int b, int a)
{
  int r2 = (radius * 2) / 2;
  
  for (int px = x - r2; px < x + r2; px++)
  {
    setEffectPixelLarge(px, y - r2, r, g, b, a);
    setEffectPixelLarge(px, y + r2, r, g, b, a);
  }
  for (int py = y - r2; py < y + r2; py++)
  {
    setEffectPixelLarge(x - r2, py, r, g, b, a);
    setEffectPixelLarge(x + r2, py, r, g, b, a);
  }


} // FSquare //

void Effect_Explo::FCross(int x, int y, int radius, int r, int g, int b, int a)
{
  int r2 = radius / 2;
  
  setEffectPixel(x, y - r2, r, g, b, a);
  setEffectPixel(x, y + r2, r, g, b, a);
  setEffectPixel(x - r2, y, r, g, b, a);
  setEffectPixel(x + r2, y, r, g, b, a);
}


void Effect_Explo::FSpiral(int x, int y, int radius, int r, int g, int b, int a)
{
  float ang = (float)radius / 4;
  
  for (float i = 0; i < 10; i++)
  {
    float rGap = 0.05;
    
    int px = cosf(ang + i * rGap) * ((float)radius / 2);
    int py = sinf(ang + i * rGap) * ((float)radius / 2);
    
    setEffectPixelLarge(px + x, py + y, r, g, b, a);
    setEffectPixelLarge(-px + x, -py + y, r, g, b, a);
    
  }
}

void Effect_Explo::FStar(int x, int y, int radStart, byte r, byte g, byte b, byte a)
{
  float alpha = (2 * 3.14159) / 10; 
  int x1, y1, x2, y2;
  x2 = -999;
  y2 = -999;
  byte rRad;
  
  for (byte i = 11; i != 0; i--)
  {
      rRad = radStart * 1.5 * (i % 2 + 1)/2;
      float omega = alpha * i;

      x1 = (rRad * sinf(omega)) + x;
      y1 = (rRad * cosf(omega)) + y;

      if (x2 != -999) fastLine(x1, y1, x2, y2, r, g, b, a);
      x2 = x1;
      y2 = y1;
  }        

}



void Effect_Explo::FTriangle(int x, int y, int radStart, byte r, byte g, byte b, byte a)
{
  float alpha = (2 * 3.14159) / 3; 
  int x1, y1, x2, y2;
  x2 = -999;
  y2 = -999;
  byte rRad;
  
  for (byte i = 0; i < 4; i++)
  {
      rRad = radStart * 0.6;
      float omega = alpha * i + ((float)radStart / 15);

      x1 = (rRad * sinf(omega)) + x;
      y1 = (rRad * cosf(omega)) + y;

      if (x2 != -999) fastLine(x1, y1, x2, y2, r, g, b, a);
      x2 = x1;
      y2 = y1;
  }        

}

