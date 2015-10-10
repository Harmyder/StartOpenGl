#pragma once

#include "World\Shapes\IViewportVisitor.h"

namespace World
{
    class Shape
    {
    public:
        Shape(glm::mat4 transform) : _transform(transform) {}
        virtual ~Shape() {}

        const glm::mat4& GetTransform() const { return _transform; }
        void SetTransform(glm::mat4 transform) { _transform = transform; }

        virtual void Accept(IViewportVisitor& vv) = 0;

    private:
        glm::mat4 _transform;
    };
}
