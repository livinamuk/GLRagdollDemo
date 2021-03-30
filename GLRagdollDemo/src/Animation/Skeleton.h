#pragma once
#include "SkeletonJoint.h"
#include "Header.h"

    class Skeleton
    {
    public: // methods
        Skeleton();

    public: // fields
        //unsigned int m_jointCount;
        std::vector<SkeletonJoint> m_joints;
    };