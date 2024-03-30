#include "Model/Model.h"

#include <stb_image.h>

#include "Error.h"

Render::Model::Model(const std::string &path) {
    m_LoadModel(path);
}

void Render::Model::Draw(Render::Shader &shader) {
    for (auto &mesh : m_meshes)
        mesh.Draw(shader);
}

void Render::Model::m_LoadModel(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate
    | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        ASSERT(false, "Failed to load a model!");

    m_directory = path.substr(0, path.find_last_of('/'));
    m_ProcessNode(scene->mRootNode, scene);
}

void Render::Model::m_ProcessNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(m_ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        m_ProcessNode(node->mChildren[i], scene);
}

Render::Mesh Render::Model::m_ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals()) {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;

            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;

            vertex.Bitangent.x = mesh->mBitangents[i].x;
            vertex.Bitangent.y = mesh->mBitangents[i].y;
            vertex.Bitangent.z = mesh->mBitangents[i].z;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    m_LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
    m_LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures);
    m_LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", textures);
    m_LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height", textures);

    return {vertices, indices, textures};
}

void Render::Model::m_LoadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName,
                                           std::vector<Texture> &textures) {
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        auto it = m_loadedTexturesMap.find(str.C_Str());
        if (it != m_loadedTexturesMap.end()) {
            textures.push_back(it->second);
            skip = true;
        }

        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_loadedTexturesMap[str.C_Str()] = texture;
        }
    }
}

unsigned int Render::TextureFromFile(const char *filename, const std::string& directory) {
    std::string fullPath(directory + '/' + filename);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
        ASSERT(false, "Failed to load texture image!");

    stbi_image_free(data);

    return textureID;
}
