#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "FileSystem.h"

namespace Data {
    struct WindowData {
        static inline int screenWidth = 1600;
        static inline int screenHeight = 900;
        static inline const char *windowTitle = "ParkGL";
    };

    struct CubeData {
        static inline float vertices[288] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
    };

    struct SkyboxData {
        static inline std::vector<std::string> faces {
                FileSystem::GetPath("resources/textures/skybox/right.png"),
                FileSystem::GetPath("resources/textures/skybox/left.png"),
                FileSystem::GetPath("resources/textures/skybox/top.png"),
                FileSystem::GetPath("resources/textures/skybox/bottom.png"),
                FileSystem::GetPath("resources/textures/skybox/front.png"),
                FileSystem::GetPath("resources/textures/skybox/back.png")
        };

        static inline float vertices[108] = {
                // positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
        };
    };

    struct FramebufferData {
        static inline float vertices[24] = {
                // positions   // texCoords
                -1.0f,  1.0f,  0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                1.0f, -1.0f,  1.0f, 0.0f,

                -1.0f,  1.0f,  0.0f, 1.0f,
                1.0f, -1.0f,  1.0f, 0.0f,
                1.0f,  1.0f,  1.0f, 1.0f
        };

        static inline int framebufferWidth = 1600;
        static inline int framebufferHeight = 900;
    };

    struct TreeData {
        static inline unsigned int numOfTrees = 174;
        static inline std::array<glm::vec3, 174> treePositions {
            glm::vec3(2.75f, 0.0f, -5.75f),
            glm::vec3(2.75f, 0.0f, -7.75f),
            glm::vec3(2.75f, 0.0f, -9.75f),
            glm::vec3(2.75f, 0.0f, -11.75f),
            glm::vec3(2.75f, 0.0f, -13.75f),
            glm::vec3(4.75f, 0.0f, -6.75f),
            glm::vec3(4.75f, 0.0f, -8.75f),
            glm::vec3(4.75f, 0.0f, -10.75f),
            glm::vec3(4.75f, 0.0f, -12.75f),
            glm::vec3(6.75f, 0.0f, -13.75f),
            glm::vec3(6.75f, 0.0f, -11.75f),
            glm::vec3(6.75f, 0.0f, -9.75f),
            glm::vec3(6.75f, 0.0f, -7.75f),
            glm::vec3(6.75f, 0.0f, -5.75f),
            glm::vec3(5.75f, 0.0f, -3.75f),
            glm::vec3(5.75f, 0.0f, -1.75f),
            glm::vec3(5.75f, 0.0f, 0.25f),
            glm::vec3(5.75f, 0.0f, 2.25f),
            glm::vec3(5.75f, 0.0f, 4.25f),
            glm::vec3(5.75f, 0.0f, 6.25f),
            glm::vec3(5.75f, 0.0f, 8.25f),
            glm::vec3(5.75f, 0.0f, 10.25f),
            glm::vec3(5.75f, 0.0f, 12.25f),
            glm::vec3(5.75f, 0.0f, 14.25f),
            glm::vec3(7.75f, 0.0f, -2.75f),
            glm::vec3(7.75f, 0.0f, -0.75f),
            glm::vec3(7.75f, 0.0f, 1.25f),
            glm::vec3(7.75f, 0.0f, 3.25f),
            glm::vec3(7.75f, 0.0f, 5.25f),
            glm::vec3(7.75f, 0.0f, 7.25f),
            glm::vec3(7.75f, 0.0f, 9.25f),
            glm::vec3(7.75f, 0.0f, 11.25f),
            glm::vec3(7.75f, 0.0f, 13.25f),
            glm::vec3(9.75f, 0.0f, -13.75f),
            glm::vec3(9.75f, 0.0f, -11.75f),
            glm::vec3(9.75f, 0.0f, -9.75f),
            glm::vec3(9.75f, 0.0f, -7.75f),
            glm::vec3(9.75f, 0.0f, -5.75f),
            glm::vec3(9.75f, 0.0f, -3.75f),
            glm::vec3(9.75f, 0.0f, -1.75f),
            glm::vec3(9.75f, 0.0f, 0.25f),
            glm::vec3(9.75f, 0.0f, 2.25f),
            glm::vec3(9.75f, 0.0f, 4.25f),
            glm::vec3(9.75f, 0.0f, 6.25f),
            glm::vec3(9.75f, 0.0f, 8.25f),
            glm::vec3(9.75f, 0.0f, 10.25f),
            glm::vec3(9.75f, 0.0f, 12.25f),
            glm::vec3(9.75f, 0.0f, 14.25f),
            glm::vec3(11.75f, 0.0f, -12.75f),
            glm::vec3(11.75f, 0.0f, -10.75f),
            glm::vec3(11.75f, 0.0f, -8.75f),
            glm::vec3(11.75f, 0.0f, -6.75f),
            glm::vec3(11.75f, 0.0f, -4.75f),
            glm::vec3(11.75f, 0.0f, -2.75f),
            glm::vec3(11.75f, 0.0f, -0.75f),
            glm::vec3(11.75f, 0.0f, 1.25f),
            glm::vec3(11.75f, 0.0f, 3.25f),
            glm::vec3(11.75f, 0.0f, 5.25f),
            glm::vec3(11.75f, 0.0f, 7.25f),
            glm::vec3(11.75f, 0.0f, 9.25f),
            glm::vec3(11.75f, 0.0f, 11.25f),
            glm::vec3(11.75f, 0.0f, 13.25f),
            glm::vec3(13.75, 0.0f, -13.75f),
            glm::vec3(13.75, 0.0f, -11.75f),
            glm::vec3(13.75, 0.0f, -9.75),
            glm::vec3(13.75, 0.0f, -7.75f),
            glm::vec3(13.75, 0.0f, -5.75f),
            glm::vec3(13.75, 0.0f, -3.75f),
            glm::vec3(13.75, 0.0f, -1.75f),
            glm::vec3(13.75, 0.0f, 0.25f),
            glm::vec3(13.75, 0.0f, 2.25f),
            glm::vec3(13.75, 0.0f, 4.25f),
            glm::vec3(13.75, 0.0f, 6.25f),
            glm::vec3(13.75, 0.0f, 8.25f),
            glm::vec3(13.75, 0.0f, 10.25f),
            glm::vec3(13.75, 0.0f, 12.25f),
            glm::vec3(13.75, 0.0f, 14.25f),
            glm::vec3(-2.75f, 0.0f, -5.75f),
            glm::vec3(-2.75f, 0.0f, -7.75f),
            glm::vec3(-2.75f, 0.0f, -9.75f),
            glm::vec3(-2.75f, 0.0f, -11.75f),
            glm::vec3(-2.75f, 0.0f, -13.75f),
            glm::vec3(-4.75f, 0.0f, -6.75f),
            glm::vec3(-4.75f, 0.0f, -8.75f),
            glm::vec3(-4.75f, 0.0f, -10.75f),
            glm::vec3(-4.75f, 0.0f, -12.75f),
            glm::vec3(-6.75f, 0.0f, -13.75f),
            glm::vec3(-6.75f, 0.0f, -11.75f),
            glm::vec3(-6.75f, 0.0f, -9.75f),
            glm::vec3(-6.75f, 0.0f, -7.75f),
            glm::vec3(-6.75f, 0.0f, -5.75f),
            glm::vec3(-5.75f, 0.0f, -3.75f),
            glm::vec3(-5.75f, 0.0f, -1.75f),
            glm::vec3(-5.75f, 0.0f, 0.25f),
            glm::vec3(-5.75f, 0.0f, 2.25f),
            glm::vec3(-5.75f, 0.0f, 4.25f),
            glm::vec3(-5.75f, 0.0f, 6.25f),
            glm::vec3(-5.75f, 0.0f, 8.25f),
            glm::vec3(-5.75f, 0.0f, 10.25f),
            glm::vec3(-5.75f, 0.0f, 12.25f),
            glm::vec3(-5.75f, 0.0f, 14.25f),
            glm::vec3(-7.75f, 0.0f, -2.75f),
            glm::vec3(-7.75f, 0.0f, -0.75f),
            glm::vec3(-7.75f, 0.0f, 1.25f),
            glm::vec3(-7.75f, 0.0f, 3.25f),
            glm::vec3(-7.75f, 0.0f, 5.25f),
            glm::vec3(-7.75f, 0.0f, 7.25f),
            glm::vec3(-7.75f, 0.0f, 9.25f),
            glm::vec3(-7.75f, 0.0f, 11.25f),
            glm::vec3(-7.75f, 0.0f, 13.25f),
            glm::vec3(-9.75f, 0.0f, -13.75f),
            glm::vec3(-9.75f, 0.0f, -11.75f),
            glm::vec3(-9.75f, 0.0f, -9.75f),
            glm::vec3(-9.75f, 0.0f, -7.75f),
            glm::vec3(-9.75f, 0.0f, -5.75f),
            glm::vec3(-9.75f, 0.0f, -3.75f),
            glm::vec3(-9.75f, 0.0f, -1.75f),
            glm::vec3(-9.75f, 0.0f, 0.25f),
            glm::vec3(-9.75f, 0.0f, 2.25f),
            glm::vec3(-9.75f, 0.0f, 4.25f),
            glm::vec3(-9.75f, 0.0f, 6.25f),
            glm::vec3(-9.75f, 0.0f, 8.25f),
            glm::vec3(-9.75f, 0.0f, 10.25f),
            glm::vec3(-9.75f, 0.0f, 12.25f),
            glm::vec3(-9.75f, 0.0f, 14.25f),
            glm::vec3(-11.75f, 0.0f, -12.75f),
            glm::vec3(-11.75f, 0.0f, -10.75f),
            glm::vec3(-11.75f, 0.0f, -8.75f),
            glm::vec3(-11.75f, 0.0f, -6.75f),
            glm::vec3(-11.75f, 0.0f, -4.75f),
            glm::vec3(-11.75f, 0.0f, -2.75f),
            glm::vec3(-11.75f, 0.0f, -0.75f),
            glm::vec3(-11.75f, 0.0f, 1.25f),
            glm::vec3(-11.75f, 0.0f, 3.25f),
            glm::vec3(-11.75f, 0.0f, 5.25f),
            glm::vec3(-11.75f, 0.0f, 7.25f),
            glm::vec3(-11.75f, 0.0f, 9.25f),
            glm::vec3(-11.75f, 0.0f, 11.25f),
            glm::vec3(-11.75f, 0.0f, 13.25f),
            glm::vec3(-13.75, 0.0f, -13.75f),
            glm::vec3(-13.75, 0.0f, -11.75f),
            glm::vec3(-13.75, 0.0f, -9.75),
            glm::vec3(-13.75, 0.0f, -7.75f),
            glm::vec3(-13.75, 0.0f, -5.75f),
            glm::vec3(-13.75, 0.0f, -3.75f),
            glm::vec3(-13.75, 0.0f, -1.75f),
            glm::vec3(-13.75, 0.0f, 0.25f),
            glm::vec3(-13.75, 0.0f, 2.25f),
            glm::vec3(-13.75, 0.0f, 4.25f),
            glm::vec3(-13.75, 0.0f, 6.25f),
            glm::vec3(-13.75, 0.0f, 8.25f),
            glm::vec3(-13.75, 0.0f, 10.25f),
            glm::vec3(-13.75, 0.0f, 12.25f),
            glm::vec3(-13.75, 0.0f, 14.25f),
            glm::vec3(3.75f, 0.0f, 5.75f),
            glm::vec3(1.75f, 0.0f, 5.75f),
            glm::vec3(-0.25f, 0.0f, 5.75f),
            glm::vec3(-2.25f, 0.0f, 5.75f),
            glm::vec3(-4.25f, 0.0f, 5.75f),
            glm::vec3(2.75f, 0.0f, 7.75f),
            glm::vec3(0.25f, 0.0f, 7.75f),
            glm::vec3(-1.75f, 0.0f, 7.75f),
            glm::vec3(-3.75f, 0.0f, 7.75f),
            glm::vec3(3.75f, 0.0f, 9.75f),
            glm::vec3(0.75f, 0.0f, 9.75f),
            glm::vec3(-1.25f, 0.0f, 9.75f),
            glm::vec3(-3.25f, 0.0f, 9.75f),
            glm::vec3(0.75f, 0.0f, 11.75f),
            glm::vec3(-1.25f, 0.0f, 11.75f),
            glm::vec3(-3.25f, 0.0f, 11.75f),
            glm::vec3(1.75f, 0.0f, 13.75f),
            glm::vec3(-0.25f, 0.0f, 13.75f),
            glm::vec3(-2.25f, 0.0f, 13.75f),
            glm::vec3(-4.25f, 0.0f, 13.75f)
        };
    };
}
