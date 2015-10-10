#pragma once

#include "World\Shapes\Shape.h"

namespace World
{
    class Box final : public Shape
    {
    public:
        Box(glm::vec3 halfExtents, glm::mat4 transform) : Shape(transform), _halfExtents(halfExtents) {}

        const glm::vec3& GetHalfExtents() const { return _halfExtents; }

        void Accept(IViewportVisitor& vv) override;

    private:
        glm::vec3 _halfExtents;
    };
}
