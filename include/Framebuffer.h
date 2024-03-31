#pragma once

#include <memory>

#include "Shader.h"

namespace Render {
    class Framebuffer {
    public:
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer(Framebuffer&&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;
        Framebuffer& operator=(Framebuffer&&) = delete;

        static Framebuffer& GetInstance() {
            static Framebuffer instance;
            return instance;
        }

        void Init();
        void Clear();

        void Bind() const;
        void Unbind();
    private:
        Framebuffer() = default;

        Shader *m_shader{};

        unsigned int VAO{}, VBO{}, RBO{};
        unsigned int m_framebuffer{};
        unsigned int m_textureColorBuffer{};
    };
}
