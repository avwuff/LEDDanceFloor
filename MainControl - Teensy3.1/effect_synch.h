
#define NUM_SYNC 100
#define NUM_PERIODS 3
class Effect_Synch {
private:  

  typedef struct Square
  {
      long last_seen;
      byte x, y;
      int periods[NUM_PERIODS];
      byte current_period;
      int avg_period;
      float score;
  } SQUARE;

  int last_beat_period = 0;
  Square squares[NUM_SYNC]; // Keep track of this many squares to synchronize
  
  
  void TrackStep(byte x, byte y);

  
public:
 Effect_Synch();
 void Tick();
 void Beat();
 void buttonChange(byte x, byte y, byte state);
};

