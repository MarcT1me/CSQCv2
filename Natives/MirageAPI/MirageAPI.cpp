#include "pch.h"
#include "MirageAPI.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace MirageAPI
{
    bool MirageSystem::init(int major, int minor)
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        return true;
    }

    void MirageSystem::shutdown()
    {
    }
}
