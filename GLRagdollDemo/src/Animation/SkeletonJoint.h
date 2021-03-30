#pragma once
#include "Header.h"

class SkeletonJoint
{
public: // methods
    SkeletonJoint();

public: // fields
    const char* m_name;
    int m_parentIndex;
    glm::mat4 m_inverseBindTransform;
    glm::mat4 m_currentFinalTransform;
};
