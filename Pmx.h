#ifndef __PMX_H__
#define __PMX_H__

#include "MathVector.h"
#include "Utils.h"

namespace Pmx
{
  enum class DeformType:
  {
    BDEF1,
    BDEF2,
    BDEF4,
    SDEF,
    QDEF
  };

  struct BoneWeight
  {
    int bone;
    float value;

    BoneWeight();
    bool is_valid() const;
  };

  struct Vertex
  {
    Vector3 position, normal;
    Vector2 uv;
    Vector4 uva[4];
    Vector3 c0, r0, r1, rw0, rw1; // only for SDEF
    DeformType deform;
    BoneWeight weight[4];
    float edge_scale;

    Vertex();
    void read();
  };

  enum MaterialFlag
  {
			None = 0,
			DrawBoth = 1,
			Shadow = 2,
			SelfShadowMap = 4,
			SelfShadow = 8,
			Edge = 0x10,
			VertexColor = 0x20
			PointDraw = 0x40,
			LineDraw = 0x80,
  };

  enum class SphereModeType
  {
    None, 
    Mul,
    Add,
    SubTex
  };

  struct Material
  {
    std::string name, name_en, memo;
    Vector4 diffuse, edge_color;
    Vector3 specular, ambient;
    float specular_intensity, edge_size;
    MaterialFlag flag;
    SphereModeType sphere_mode;
    int diffuse_texture_index;
    int sphere_texture_index;
    int toon_texture_index;
    int n_faces;

    Material();
    void read();
  };

  enum BoneFlag
  {
			None = 0,
			ToBone = 1,
			Rotation = 1 << 1,
			Translation = 1 << 2,
			Visible = 1 << 3,
			Enable = 1 << 4,
			IK = 1 << 5,
			AppendLocal = 1 << 7, // not sure
			AppendRotation = 1 << 8,
			AppendTranslation = 1 << 9,
			FixAxis = 1 << 10,
			LocalCoord = 1 << 11,
			AfterPhysics = 1 << 12,
			ExtParent = 1 << 13,
  };
}

#endif