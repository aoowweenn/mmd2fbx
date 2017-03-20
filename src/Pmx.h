#ifndef __PMX_H__
#define __PMX_H__

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

using namespace glm;

namespace Pmx
{

struct Named
{
  std::string name, name_en;
};

enum class DeformType
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
  vec3 position, normal;
  vec2 uv;
  vec4 uva[4];
  vec3 c0, r0, r1, rw0, rw1; // only for SDEF
  DeformType deform;
  BoneWeight weight[4];
  float edge_scale;

  Vertex();
  void read();
  std::pair<vec3, vec3> calc_sdef_rw();
};

enum MaterialFlag
{
  None = 0,
  DrawBoth = 1,
  Shadow = 2,
  SelfShadowMap = 4,
  SelfShadow = 8,
  Edge = 0x10,
  VertexColor = 0x20,
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

struct Material : Named
{
  std::string memo;
  vec4 diffuse, edge_color;
  vec3 specular, ambient;
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

struct IKLink
{
  int bone;
  bool is_limit;
  vec3 low, high;

  IKLink();
  void read();
};

struct IK
{
  int target, n_loops;
  float angle;
  std::vector<IKLink> link_list;

  IK();
  void read();
};

struct Bone : Named
{
  vec3 pos, rel_pos, end_offset, axis;
  vec3 local_x, local_y, local_z;
  int parent, level, end_bone, append_parent, ext_key;
  float append_ratio;
  BoneFlag flag;
  IK ik;
  Bone *ref_parent;
  mat4 world, local;

  Bone();
  void read();
};

struct MorphBase
{
  int index;

  virtual void read() = 0;
};

struct MorphGroup : MorphBase
{
  float ratio;

  MorphGroup();
  void read();
};

struct MorphVertex : MorphBase
{
  vec3 offset;

  MorphVertex();
  void read();
};

struct MorphBone : MorphBase
{
  vec3 translation;
  vec4 rotation;

  MorphBone();
  void read();
};

struct MorphUV : MorphBase
{
  vec4 offset;

  MorphUV();
  void read();
};

enum class MatOpType
{
  Mul,
  Add
};

struct MorphMaterial : MorphBase
{
  MatOpType op;
  vec4 diffuse, specular;
  vec3 ambient;
  float edge_size;
  vec4 edge_color, diffuse_tex, sphere_tex, toon_tex;

  MorphMaterial();
  void read();
};

struct MorphImpulse : MorphBase
{
  bool local;
  vec3 torque, velocity;

  MorphImpulse();
  void read();
};

enum class OffsetKind
{
  Group,
  Vertex,
  Bone,
  UV,
  UVA1,
  UVA2,
  UVA3,
  UVA4,
  Material,
  Flip,
  Impulse
};

struct Morphing : Named
{
  int panel;
  OffsetKind kind;
  std::vector<MorphBase*> offset_list;

  Morphing();
  ~Morphing();
  void read();
};

enum class ElementType
{
  Bone,
  Morph
};

struct NodeElement
{
  ElementType type;
  int index;

  void read();
};

struct Node : Named
{
  bool is_system_node;
  std::vector<NodeElement> element_list;

  void read();
};

struct BodyPassGroup
{
  static const int flags_length = 16;
  bool flags[flags_length];

  void from_u16(uint16_t bits)
  {
    for(auto i = 0; i < flags_length; ++i)
      flags[i] = (bits & (1 << i)) <= 0;
  }
};

enum class BodyType
{
  Sphere,
  Box,
  Capsule
};

enum class ModeType
{
  Static,
  Dynamic,
  DynamicWithBone
};

struct Body : Named
{
  int bone, group;
  BodyPassGroup pass_group;
  BodyType type;
  vec3 box_size, position, rotation;
  float mass, damping_position, damping_rotation, restitution, friction;
  ModeType mode;

  void read();
};

enum class JointType
{
  Sp6DOF,
  G6DOF,
  P2P,
  ConeTwist,
  Slider,
  Hinge
};

struct Joint : Named
{
  JointType type;
  int body_A, body_B;
  vec3 position, rotation;
  vec3 limit_move_low, limit_move_high, limit_angle_low, limit_angle_high;
  vec3 spring_move, spring_rotate;

  void read();
};

enum class Shapetype
{
  Trimesh,
  Rope
};

enum class SoftBodyFlags
{
  GenerateBendingLinks = 1,
  GenerateClusters = 2,
  RandomizeConstraints = 4
};

struct SoftBodyConfig
{
		int AeroModel;
		float VCF;
		float DP;
		float DG;
		float LF;
		float PR;
		float VC;
		float DF;
		float MT;
		float CHR;
		float KHR;
		float SHR;
		float AHR;
		float SRHR_CL;
		float SKHR_CL;
		float SSHR_CL;
		float SR_SPLT_CL;
		float SK_SPLT_CL;
		float SS_SPLT_CL;
		int V_IT;
		int P_IT;
		int D_IT;
		int C_IT;

    SoftBodyConfig();
    void clear();
};

struct SoftBodyMaterialConfig
{
  float LST, AST, VST;

  SoftBodyMaterialConfig();
  void clear();
};

struct BodyAnchor
{
  int body;
  bool is_near;
  int vertex;
  
  bool operator < (const BodyAnchor & other) const;
};

struct VertexPin
{
  int vertex;

  bool operator < (const VertexPin & other) const;
};

struct SoftBody : Named
{
  Shapetype shape;
  int material, group;
  BodyPassGroup pass_group;
  SoftBodyFlags flags;
  int bending_link_distance, n_clusters;
  float total_mass, margin;
  SoftBodyConfig config;
  SoftBodyMaterialConfig material_config;
  std::vector<BodyAnchor> anchor_list;
  std::vector<VertexPin> pin_list;

  void read();
  void normalize_anchors();
  void normalize_pins();
};

}

#endif