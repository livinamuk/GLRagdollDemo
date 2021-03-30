#pragma once
#include "header.h"
#include "Animation/SkinnedModel.h"
#include "Physics/Ragdoll.h"

enum SkinningMethod { RAGDOLL, BINDPOSE};

class GameCharacter
{
public:
    GameCharacter();
    ~GameCharacter();

  //  void SkinFromBindpose();
  //  void SkinFromRagdoll();
    void RenderSkinnedModel(Shader* shader);

    Transform m_transform;
    Ragdoll m_ragdoll;
    SkinnedModel* m_skinnedModel;
    SkinningMethod m_skinningMethod;

    std::vector<glm::mat4> m_animatedTransforms; 
    std::vector<glm::mat4> m_animatedDebugTransforms_Animated;

private: 
   // void FindBindPoseTransforms(const aiNode* pNode);
};