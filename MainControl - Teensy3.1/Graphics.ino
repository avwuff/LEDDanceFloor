
void fastLine(int x1, int y1, int x2, int y2, byte r, byte g, byte b, byte a)
{
  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  sdx=sgn(dx);
  sdy=sgn(dy);
  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;

  setEffectPixelLarge(px, py, r, g, b, a);

  if (dxabs>=dyabs) /* the line is more horizontal than vertical */
  {
    for(i=0;i<dxabs;i++)
    {
      y+=dyabs;
      if (y>=dxabs)
      {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;

      setEffectPixelLarge(px, py, r, g, b, a);
    }
  }
  else /* the line is more vertical than horizontal */
  {
    for(i=0;i<dyabs;i++)
    {
      x+=dxabs;
      if (x>=dyabs)
      {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      
      setEffectPixelLarge(px, py, r, g, b, a);
    }
  }
}

// Test algorithm for drawing a circle.
void FCircle(int x, int y, int radius, int r, int g, int b, int a)
{
  int balance = -radius, xoff = 0, yoff = radius;

  balance = -radius;

  do {
    setEffectPixelLarge(x + xoff, y + yoff, r, g, b, a);
    setEffectPixelLarge(x - xoff, y + yoff, r, g, b, a);
    setEffectPixelLarge(x - xoff, y - yoff, r, g, b, a);
    setEffectPixelLarge(x + xoff, y - yoff, r, g, b, a);
    setEffectPixelLarge(x + yoff, y + xoff, r, g, b, a);
    setEffectPixelLarge(x - yoff, y + xoff, r, g, b, a);
    setEffectPixelLarge(x - yoff, y - xoff, r, g, b, a);
    setEffectPixelLarge(x + yoff, y - xoff, r, g, b, a);

    if ((balance += xoff++ + xoff) >= 0)
      balance -= --yoff + yoff;

  } while (xoff <= yoff);
} // FCircle //
