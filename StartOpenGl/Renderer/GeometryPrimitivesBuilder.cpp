#include "stdafx.h"

#include "GeometryPrimitivesBuilder.h"

using glm::vec3;
using glm::vec2;

namespace Renderer
{
    void GeometryPrimitivesBuilder::BuildCube(Vertices& output)
    {
        const float side = 0.5f;
        vec3 P0(-side, -side, -side);
        vec3 P1(-side, side, -side);
        vec3 P2(side, side, -side);
        vec3 P3(side, -side, -side);
        vec3 P4(-side, -side, side);
        vec3 P5(-side, side, side);
        vec3 P6(side, side, side);
        vec3 P7(side, -side, side);

        vec3 trianglesVertices[] =
        {
            // front face
            P0, P1, P2,
            P0, P2, P3,

            // back face
            P4, P6, P5,
            P4, P7, P6,

            // left face
            P4, P5, P1,
            P4, P1, P0,

            // right face
            P3, P2, P6,
            P3, P6, P7,

            // top face
            P1, P5, P6,
            P1, P6, P2,

            // bottom face
            P4, P0, P3,
            P4, P3, P7
        };

        vec3 normals[6] =
        {
            // front face
            vec3(0.f, 0.f, -.5f),
            // back face
            vec3(0.f, 0.f, .5f),
            // left face
            vec3(.5f, 0.f, 0.f),
            // right face
            vec3(-.5f, 0.f, 0.f),
            // top face
            vec3(0.f, 5.f, 0.f),
            // bottom face
            vec3(0.f, -5.f, 0.f),
        };

        output.reserve(36);
        for (uint32 i = 0; i < 36; ++i)
        {
            output.emplace_back(trianglesVertices[i], normals[i / 6]);
        }
    }
}