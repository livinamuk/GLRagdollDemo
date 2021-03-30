#pragma once
#include <map>
#include <vector>
#include <assert.h>
#include "header.h"
#include "Renderer/Mesh.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Physics/Ragdoll.h"


class SkinnedModel
{
public:
    SkinnedModel();
    ~SkinnedModel();

    void Render(Shader* shader, const glm::mat4& modelMatrix);
      
    glm::mat4 m_CameraMatrix = glm::mat4(1);
    Skeleton m_skeleton;
    const char* m_filename;

    std::vector<Animation*> m_animations;

    void BoneTransform(float Time, std::vector<glm::mat4>& Transforms, std::vector<glm::mat4>& DebugAnimatedTransforms);
   

public:

    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;
        glm::mat4 ModelSpace_AnimatedTransform;
        glm::mat4 DebugMatrix_BindPose;
        std::string BoneName;

        BoneInfo()
        {
            BoneOffset = glm::mat4(0);
            FinalTransformation = glm::mat4(0);
            DebugMatrix_BindPose = glm::mat4(1);
            ModelSpace_AnimatedTransform = glm::mat4(1);
        }
    };


    void UpdateBoneTransform(std::vector<glm::mat4>& Transforms, std::vector<glm::mat4>& DebugAnimatedTransforms, Ragdoll* ragdoll);

    void CalcInterpolatedScaling(glm::vec3& Out, float AnimationTime, const AnimatedNode* animatedNode);
    void CalcInterpolatedRotation(glm::quat& Out, float AnimationTime, const AnimatedNode* animatedNode);
    void CalcInterpolatedPosition(glm::vec3& Out, float AnimationTime, const AnimatedNode* animatedNode);
    int FindAnimatedNodeIndex(float AnimationTime, const AnimatedNode* animatedNode);
        
    const AnimatedNode* FindAnimatedNode(Animation* animation, const char* NodeName);
     
        
public:

     

public:
    GLuint m_VAO;
    GLuint m_Buffers[10];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            Name = "";
        }
    public:
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        std::string Name;
    };

public:
    std::vector<MeshEntry> m_meshEntries;
    std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index

public:
    unsigned int m_NumBones;
    std::vector<BoneInfo> m_BoneInfo;
    glm::mat4 m_GlobalInverseTransform;

    unsigned int currentAnimationIndex = 0; // should probably delete this for safety and pass it in as param from the animated entity
};
