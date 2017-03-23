
class Effect_Explo {
private:  
  byte currExplo;
  byte exploXY[20][2]; // Up to 20 explosions can be rendered at once.
  byte exploState[20]; // How big is the explosion.
  byte ExploColor[20][3]; // The colors of the explo.
  byte exploStyle[20]; // The style of the explosion
  
  void FSquare(int x, int y, int radius, int r, int g, int b, int a);
  void FCross(int x, int y, int radius, int r, int g, int b, int a);
  void FSpiral(int x, int y, int radius, int r, int g, int b, int a);
  void FStar(int x, int y, int radStart, byte r, byte g, byte b, byte a);
  void FTriangle(int x, int y, int radStart, byte r, byte g, byte b, byte a);
  void NewExplosion(byte x, byte y, byte style);
  
public:
 Effect_Explo();
 void Tick();
 void Beat();
 void buttonChange(byte x, byte y, byte state);
 void NewBaseExplosion(byte x, byte y, byte style);
};

