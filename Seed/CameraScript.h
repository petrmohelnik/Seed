#pragma once
#include "Script.h"

class CameraScript : public Script
{
public:
    using Script::Script;

    virtual void OnFrameUpdate() {};
    glm::mat4 getProjection();

private:

};

glm::mat4 CameraScript::getProjection()
{
    return glm::mat4(0.0);
}