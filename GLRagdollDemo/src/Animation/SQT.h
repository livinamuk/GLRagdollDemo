#pragma once
#include "Header.h"

class SQT
{
public: // methods
    SQT();

public: // fields
    glm::quat rotation;
    glm::vec3 positon;
    float scale;
    float timeStamp = -1;
    const char* jointName;
};