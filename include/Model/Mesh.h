#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

namespace Render {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        Mesh(std::vector<Vertex> &vs, std::vector<unsigned int> &ind,
             std::vector<Texture> &tex);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        void Draw(Shader &shader);
        void SetGlslIdentifierPrefix(const std::string &prefix);

        void Delete();
    private:
        unsigned int VAO{}, VBO{}, EBO{};
        std::string m_glslIdentifierPrefix;

        void m_SetupMesh();
    };
}
