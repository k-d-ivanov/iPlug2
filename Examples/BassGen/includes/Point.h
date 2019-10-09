#pragma once

/** Point class is used in interactive Graphs controls */
class Point
{
public:
  Point(unsigned int id , float x, float y)
    : mU_id{ id }, mCordX{ x }, mCordY{ y }
  {};

  void setPointID(float id) { mU_id = id; };
  void setPointCordX(float x) { mCordX = x; };
  void setPointCordY(float y) { mCordY = y; };

  const unsigned getPointID() const { return mU_id; };
  const float getPointCordX() const { return mCordX; };
  const float getPointCordY() const { return mCordY; };

  bool operator==(const Point& point) const
  {
    if (mU_id == point.mU_id) return true;
   
    return false;
  };


  ~Point() {
    setPointID(0);
    setPointCordX(0);
    setPointCordY(0);
  };
private:
  unsigned int mU_id;
  float mCordX, mCordY;
};