#pragma once

#include <vector>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

namespace Render {
    class Model {
    public:
        explicit Model(const std::string &path);
        ~Model();

        std::vector<Mesh> meshes;
        std::unordered_map<std::string, Texture> loadedTexturesMap;

        void Draw(Shader &shader);
        void SetShaderTextureNamePrefix(const std::string &path);
    private:
        std::string m_directory;

        void m_LoadModel(const std::string &path);
        void m_ProcessNode(aiNode *node, const aiScene *scene);
        Mesh m_ProcessMesh(aiMesh *mesh, const aiScene *scene);
        void m_LoadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName,
                                    std::vector<Texture> &textures);
    };

    unsigned int TextureFromFile(const char *filename, const std::string& directory);
}
