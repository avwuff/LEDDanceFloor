
Effect_RowCol::Effect_RowCol()
{
 //this->down_at = 0;
 //this->down = 0;
}

// Tick
void Effect_RowCol::Tick()
{

  byte r, g, b;
  
  
  // Look for buttons that are down.
  for (byte x = 0; x < 16; x++)
  {
    for (byte y = 0; y < 16; y++)
    {
      if (butDown[x][y])
      {

        GetEffectColor(&r, &g, &b);

        switch (EFFECTS_MODE)
        {
          case 40:
            // DRAW THE ROW
            for (byte i = 0; i < 16; i++)
            {
              setEffectPixel(i, y, 255, 255, 255, 255);
              setEffectPixel(x, i, 255, 255, 255, 255);
            }
            break;


          case 41: // Draw lines to all the other buttons that are down.


            for (byte xx = 0; xx < 16; xx++)
            {
              for (byte yy = 0; yy < 16; yy++)
              {
                // If this is down, draw a line.
                if (xx != x && yy != y && butDown[xx][yy])
                {
                  fastLine(x * 3, y * 3, xx * 3, yy * 3, r, g, b, 255);
                }
              }
            }




            break;
            
        }

        
      }
    }
  }
  
  fadeEffectsBuffer(25);
 
}

void Effect_RowCol::buttonChange(byte x, byte y, byte state)
{
  if (state)
  {
    
  }
  
}

