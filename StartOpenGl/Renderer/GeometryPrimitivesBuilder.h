#pragma once

#include "Renderer\Types.h"

namespace Renderer
{
    class GeometryPrimitivesBuilder final
    {
    public:
        static void BuildCube(Vertices &output);
        // Cylinder is aligned along y-axis
        static void BuildCylinder(Vertices &output, float bottomRadius, float topRadius, float height, uint32 slicesCount, uint32 stacksCount);
        static void BuildCone(Vertices &output, float bottomRadius, float height, uint32 slicesCount, uint32 stacksCount);
    };
}

