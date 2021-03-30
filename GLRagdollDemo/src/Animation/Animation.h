#pragma once
#include "Header.h"
#include "AnimatedNode.h"

class Animation
{
public: // methods
    Animation(const char* Filename);
    ~Animation();

public: // fields                
    float m_duration;
    float m_ticksPerSecond;
    float m_finalTimeStamp;
    const char* m_filename;

    std::vector<AnimatedNode> m_animatedNodes;
    std::map<const char*, unsigned int> m_NodeMapping; // maps a node name to its index
};
