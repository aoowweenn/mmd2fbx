#ifndef __PMX_H__
#define __PMX_H__

#include "MathVector.h"
#include "Utils.h"

class Pmx
{
  public:
  enum class DeformType
  {
    BDEF1,
    BDEF2,
    BDEF4,
    SDEF,
    QDEF
  };

  class BoneWeight
  {
    public:
    int bone;
    float value;
  };

  class Vertex
  {
    Vector
  }
}

#endif