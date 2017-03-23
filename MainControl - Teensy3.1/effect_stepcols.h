
class Effect_StepCols {
private:  
  byte pPos;
  
public:
 Effect_StepCols();
 void Tick();
 void Ready();
 void buttonChange(byte x, byte y, byte state);
};

