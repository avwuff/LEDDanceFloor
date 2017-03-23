
//#include "effect_synch.h"


Effect_Synch::Effect_Synch()
{

  
}

// Tick
void Effect_Synch::Tick()
{
  int x, y;

  long ms = millis();
  // Draw the scores fade from red-green as score 0-1
  for (byte i = 0; i < NUM_SYNC; i++)
  {
      SQUARE * s = &squares[i];
    
      if(s->last_seen + last_beat_period * 3 >= ms) //If we've seen this square recently
      {
        Serial.print("sq ");
        Serial.print(i);
        Serial.print(" score ");
        Serial.println(s->score);
        
        setEffectPixel(s->x, s->y, 255 * (1 - s->score), 255 * s->score, 0, 255);
      }

  }

  fadeEffectsBuffer(25);
 
 
}


void Effect_Synch::buttonChange(byte x, byte y, byte state)
{
  if (state)
  {
    switch (EFFECTS_MODE)
    {
      case 60:
        TrackStep(x, y);
        break;
    }
    
  }
  
}

void Effect_Synch::TrackStep(byte x, byte y)
{
    int found = 0;
    SQUARE * s;
    int sq;
    int LRU = 0; //least recently used
    for(sq=0; sq<NUM_SYNC; sq++)
    {
        s = &squares[sq];
        if(s->x == x && s->y == y)
        {
            found = 1;
            break;
        }
        if(s->last_seen < squares[LRU].last_seen)
        {
            LRU = sq;
        }
    }
    
    long ms = millis();
    if(!found)
    {
        if(ms - squares[LRU].last_seen > 3 * last_beat_period)
            sq = LRU; //Overwrite least recently used;
        else
            return; //Ignore
    }

    Serial.print("TrackStep, sq: ");
    Serial.print(sq);
    
    
    s = &squares[sq];
    s->x = x;
    s->y = y;
    s->periods[s->current_period] = ms - s->last_seen;
    s->last_seen = ms;
    s->current_period = (s->current_period + 1) % NUM_PERIODS;

    Serial.print(", Period: ");
    Serial.print(s->current_period);

    Serial.print(", per: ");
    Serial.print(s->periods[s->current_period]);
    
    Serial.println();

    
    int p;
    s->avg_period = 0;
    for(p=0; p<NUM_PERIODS; p++)
        s->avg_period += s->current_period;
    s->avg_period /= NUM_PERIODS;
    
    
    int beat_period_err = abs(s->periods[s->current_period] - last_beat_period);
    if(beat_period_err < last_beat_period / 4)
    {
        //Period close - within 25% - improve score.
        s->score += (1 - s->score) * 0.3;
    }
    else
    {
        //Period far - reduce score.
        s->score -= s->score * 0.4;
    }
    

}


void Effect_Synch::Beat()
{
  // Called on every beat.
  static int last_beat_millis = 0;
  
  long ms = millis();
  if(last_beat_millis != 0)
  {
    if(ms > last_beat_millis) //Sanity check
        last_beat_period = ms - last_beat_millis;
  }
  last_beat_millis = ms;
}



