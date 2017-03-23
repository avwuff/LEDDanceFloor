byte baseEffectPos = 0;

float beatPulse; // A value from 0 to 1 representing the time until the next beat.
float beSin;

int baseBeatCount;


void BaseEffectTick()
{

  baseEffectPos++;
  if (baseEffectPos >= 10) baseEffectPos = 0;

  switch (BASE_EFFECT)
  {
    case 50: // Beat pulse -- Calculate the current beat time index.
      beatPulse = (float)(millis() - lastBeat) / (float)beatTempo;

      // from 0 to 0.5: 1 to 0.8
      // from 0.5 to 0: 0.8 to 1
      if (beatPulse < 0.5)
      {
        beatPulse = 1 - beatPulse;
      }
      else
      {
        //beatPulse = beatPulse);

      }
      beatPulse = 1 - (pow(beatPulse, 4) / 2);
      

      break;
  
    case 51: case 52: // beat pulse from center
      beatPulse = (float)(millis() - lastBeat) / (float)beatTempo;

      break;
    case 53: // formulas: (2x -1)^2
      beatPulse = (float)(millis() - lastBeat) / (float)beatTempo;

      break;

    case 60: case 61: case 62: case 63:
      effect1.Tick(); // Run the tick event on the explosion effect.
      break;
     
    case 80:
      BaseRainbow += 3;
      break;

      
    
  }

  
}

void BaseBeat()
{
  baseBeatCount++; // Increase the beat count.

  switch (BASE_EFFECT)
  {
    case 60: 
      effect1.NewBaseExplosion(8, 8, 1);
      break;
      
    case 61: 
      effect1.NewBaseExplosion(8, 8, 5);
      break;

    case 62: 
      effect1.NewBaseExplosion(0, 0, 1);
      effect1.NewBaseExplosion(16, 0, 1);
      effect1.NewBaseExplosion(0, 16, 1);
      effect1.NewBaseExplosion(16, 16, 1);
      break;

    case 63: 
      effect1.NewBaseExplosion(0, 0, 5);
      effect1.NewBaseExplosion(16, 0, 5);
      effect1.NewBaseExplosion(0, 16, 5);
      effect1.NewBaseExplosion(16, 16, 5);
      break;

    
  }

  
}

void BaseEffectReadyPixel(byte *x, byte *y)
{
  int d; 
  // beSin = (sin(beatPulse * 3.141 * 2 + *x  + *y) + 1) / 2; 
  switch (BASE_EFFECT)
  {
    case 51: // Adjust the beat pulse based on the location of the pixel.
      // calculate distance from center.
      d = sqrt(pow(8 - *x, 2) + pow(8 - *y, 2));

      beSin = beatPulse - ((float)d / 12) * 1;
      if (beSin < 0) beSin++;
      if (beSin > 1) beSin--;

      break;

    case 52: // Adjust the beat pulse based on the location of the pixel.
      // calculate distance from center.
      d = sqrt(pow(8 - *x, 2));

      beSin = beatPulse - ((float)d / 12) * 1;
      if (beSin < 0) beSin++;

      break;

    case 53: // Adjust the beat pulse based on the location of the pixel.
      // calculate distance from center.
      d = sqrt(pow(8 - *x, 2) + pow(8 - *y, 2));

      beSin = beatPulse - ((float)d / 12) * 1;
      if (beSin < 0) beSin++;

      beSin = 1 - pow(beSin * 2 - 1, 2);


      break;

  }
}
  
void ApplyBaseEffect(byte *r, byte *g, byte *b, byte *x, byte *y, byte *i)
{
  // i is an index of 0 to 9 for the subpixel.

  // Is there a base effect to apply?
  switch (BASE_EFFECT)
  {
    case 1: // Rotate the base color.
      if (*i == baseEffectPos)
      {
        // leave normal
      }
      else
      {
        // darken slightly.
        *r = *r * 0.8;
        *g = *g * 0.8;
        *b = *b * 0.8;
      }
      break;
  
    case 10: // Change one LED to the effect color
      if (*i == baseEffectPos)
      {
        // Change to effect color
        GetBaseColor(r, g, b);
      }
      break;
  
    case 50: // Pulse in time to the beat!
      *r = *r * beatPulse;
      *g = *g * beatPulse;
      *b = *b * beatPulse;
      break;

    case 51: case 52: case 53: // Center beat pulse -- expanding circles from the center.

      // To render this effect, the center pixels 
      *r = *r * beSin;
      *g = *g * beSin;
      *b = *b * beSin;


      break;


    case 55: // checkerboard 2x2 in time to the beat
      
      if ((*x + (*y % 2)) % 2 == baseBeatCount % 2)
      {
        GetBaseColor(r, g, b);
      }
      else
      {
        GetSecondColor(r, g, b);
      }
      break;
      
    case 56: // checkerboard 4x4 in time to the beat
      
      if ((*x/2 + (*y/2 % 2)) % 2 == baseBeatCount % 2)
      {
        GetBaseColor(r, g, b);
      }
      else
      {
        GetSecondColor(r, g, b);
      }
      break;


    case 80: // nice color spin in each tile
      
      Wheel(BaseRainbow + *i * 8, r, g, b); // Pick a random color   
      break;
      
    
  }
  

  
}

