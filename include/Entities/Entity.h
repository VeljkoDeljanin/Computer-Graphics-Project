#pragma once

#include <memory>
#include <utility>

#include "Shader.h"

namespace Entities {
    class Entity {
    public:
        Entity() = default;

        virtual void Update() = 0;
        virtual ~Entity() = default;
    };
}
