#pragma once

#include <iostream>
#include <glad/glad.h>

#define LOG(stream) stream << "[" << __FILE__ << ", " << __func__ << ", " << __LINE__ << "] "
#define BREAK_IF_FALSE(x) if (!(x)) __builtin_trap()
#define ASSERT(x, msg) do { if (!(x)) { std::cerr << msg << '\n'; BREAK_IF_FALSE(false); } } while(0)
#define GLCALL(x) \
do{ Error::ClearAllOpenGlErrors(); x; BREAK_IF_FALSE(Error::WasPreviousOpenGLCallSuccessful(__FILE__, __LINE__, #x)); } while (0)

namespace Error {
    void ClearAllOpenGlErrors();
    const char* OpenGLErrorToString(GLenum error);
    bool WasPreviousOpenGLCallSuccessful(const char* file, int line, const char* call);
}
