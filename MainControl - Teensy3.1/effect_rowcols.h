
class Effect_RowCol {
private:  
  byte pulsePos;
  
public:
 Effect_RowCol();
 void Tick();
 void buttonChange(byte x, byte y, byte state);
};

