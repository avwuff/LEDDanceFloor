
Effect_StepCols::Effect_StepCols()
{
}

// Tick
void Effect_StepCols::Tick()
{ 
  byte r, g, b;

  switch (EFFECTS_MODE)
  {
    case 70:
      fadeEffectsBuffer(1); 
      break;

    case 71: // Pulse more often

      pPos++;

      for (byte x = 0; x < 16; x++)
      {
        for (byte y = 0; y < 16; y++)
        {
          // Should this one be pulsing?
          if (EffectMem[x][y] > 0  && pPos % (255 / EffectMem[x][y]) == 0)
          {
            GetEffectColor(&r, &g, &b);
            setEffectPixel(x, y, r, g, b, 255);
          }
        }
      }

      
      fadeEffectsBuffer(25); 
      break;
  }
}

void Effect_StepCols::Ready()
{
  // Called when the effect starts, maybe
  
} 


void Effect_StepCols::buttonChange(byte x, byte y, byte state)
{
  byte r, g, b;
  
  
  if (state)
  {
    // Update the displayed effect on this tile.

    switch (EFFECTS_MODE)
    {
      case 70:

        EffectMem[x][y] += 5;
        Wheel(EffectMem[x][y], &r, &g, &b);

        setEffectPixel(x, y, r, g, b, 255);
        break;

      case 71: // Flash more often the more a tile gets stepped on.

        if (EffectMem[x][y] < 255) EffectMem[x][y] += 1; // Increase effect amount
        break;
      
    }
  }
  
}

