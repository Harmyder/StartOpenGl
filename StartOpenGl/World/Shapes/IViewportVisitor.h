#pragma once

namespace World
{
    class Shape;

    struct IViewportVisitor
    {
        virtual void DrawBox(const Shape& shape) = 0;
    };
}