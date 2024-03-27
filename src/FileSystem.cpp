#include "FileSystem.h"

std::string FileSystem::GetPath(const std::string &path) {
    static std::string(*pathBuilder)(std::string const &) = GetPathBuilder();
    return (*pathBuilder)(path);
}

std::string const &FileSystem::GetRoot() {
    static char const * envRoot = getenv("LOGL_ROOT_PATH");
    static char const * givenRoot = (envRoot != nullptr ? envRoot : logl_root);
    static std::string root = (givenRoot != nullptr ? givenRoot : "");
    return root;
}

FileSystem::Builder FileSystem::GetPathBuilder() {
    if (!GetRoot().empty())
        return &FileSystem::GetPathRelativeRoot;
    else
        return &FileSystem::GetPathRelativeBinary;
}

std::string FileSystem::GetPathRelativeRoot(const std::string &path) {
    return GetRoot() + std::string("/") + path;
}

std::string FileSystem::GetPathRelativeBinary(const std::string &path) {
    return path;
}
