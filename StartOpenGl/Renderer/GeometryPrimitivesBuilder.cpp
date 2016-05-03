#include "stdafx.h"

#include "Renderer\GeometryPrimitivesBuilder.h"

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
            output.emplace_back(Vertex{ trianglesVertices[i], normals[i / 6] });
        }
    }

    // Create cylinder along Y-axis
    void GeometryPrimitivesBuilder::BuildCylinder(Vertices &output, float bottomRadius, float topRadius, float height, uint32 slicesCount, uint32 stacksCount)
    {
        const float minimumAllowedRadius = 0.01f;
        assert(slicesCount > 2 && stacksCount > 0 && bottomRadius >= minimumAllowedRadius);

        float stackHeight = height / stacksCount;

        // Amount to increment radius as we move up each stack level from bottom to top.
        float radiusStep = (topRadius - bottomRadius) / stacksCount;

        uint32 ringsCount = stacksCount + 1;

        const uint32 baseVerticesCount = ringsCount * slicesCount;
        vec3 *baseVertices = new vec3[baseVerticesCount];

        // Compute all cylinder vertices starting at the bottom and moving up.
        uint32 vertexIndex = 0;
        for (uint32 i = 0; i < ringsCount; i++)
        {
            float y = -0.5f * height + i * stackHeight;
            float r = bottomRadius + i * radiusStep;

            if (r > minimumAllowedRadius)
            {
                // vertices of ring
                float dTheta = 2.0f * glm::pi<float>() / slicesCount;
                for (uint32 j = 0; j < slicesCount; ++j)
                {
                    Vertex vertex;

                    float c = cosf(j * dTheta);
                    float s = sinf(j * dTheta);

                    baseVertices[vertexIndex++] = vec3(r * c, y, r * s);
                }
            }
            else
            {
                for (uint32 j = 0; j < slicesCount; ++j)
                {
                    Vertex vertex;
                    baseVertices[vertexIndex++] = vec3(0.f, y, 0.f);
                }
            }
        }
        assert(vertexIndex == baseVerticesCount);

        const uint32 verticesCount = slicesCount * 3 * 2 + slicesCount * 3 * 2 * stacksCount;
        output.resize(verticesCount);

        // Handle lateral surface first
        vertexIndex = 0;
        for (uint32 i = 0; i < stacksCount; i++)
        {
            for (uint32 j = 0; j < slicesCount; j++)
            {
                // B--D
                // |\ |
                // | \|
                // A--C
                const vec3 &posA = baseVertices[i * slicesCount + j];
                const vec3 &posC = baseVertices[i * slicesCount + (j + 1) % slicesCount];
                const vec3 &posB = baseVertices[(i + 1) * slicesCount + j];
                const vec3 &posD = baseVertices[(i + 1) * slicesCount + (j + 1) % slicesCount];

                // Both triangles have the same normal
                vec3 sideAB = posB - posA;
                vec3 sideAC = posC - posA;
                vec3 normal = glm::cross(sideAB, sideAC);

                for (uint32 n = 0; n < 6; n++)
                {
                    output[vertexIndex + n].normal = normal;
                }

                output[vertexIndex++].position = posA;
                output[vertexIndex++].position = posB;
                output[vertexIndex++].position = posC;

                output[vertexIndex++].position = posC;
                output[vertexIndex++].position = posB;
                output[vertexIndex++].position = posD;
            }
        }

        // Handle bottom surface
        const vec3 bottomCenter(0.f, -0.5f * height, 0.f);
        const vec3 bottomNormal(0.f, -1.f, 0.f);
        for (uint32 n = 0; n < slicesCount * 3; n++)
        {
            output[vertexIndex + n].normal = bottomNormal;
        }
        for (uint32 j = 0; j < slicesCount; j++)
        {
            //    C(enter)
            //   / \
            //  /   \
            // A-----B

            const vec3 &posA = baseVertices[j];
            const vec3 &posB = baseVertices[(j + 1) % slicesCount];

            output[vertexIndex++].position = posA;
            output[vertexIndex++].position = posB;
            output[vertexIndex++].position = bottomCenter;
        }

        // Handle top surface
        if (topRadius > minimumAllowedRadius)
        {
            const vec3 topCenter(0.f, 0.5f * height, 0.f);
            const vec3 topNormal(0.f, 1.f, 0.f);
            for (uint32 n = 0; n < slicesCount * 3; n++)
            {
                output[vertexIndex + n].normal = topNormal;
            }
            for (uint32 j = 0; j < slicesCount; j++)
            {
                //    C(enter)
                //   / \
                //  /   \
                // B-----A
                const vec3 &posA = baseVertices[stacksCount * slicesCount + j];
                const vec3 &posB = baseVertices[stacksCount * slicesCount + (j + 1) % slicesCount];

                output[vertexIndex++].position = posB;
                output[vertexIndex++].position = posA;
                output[vertexIndex++].position = topCenter;
            }
        }

        delete[] baseVertices;
    }

    void GeometryPrimitivesBuilder::BuildCone(Vertices &vertices, float bottomRadius, float height, uint32 slicesCount, uint32 stacksCount)
    {
        BuildCylinder(vertices, bottomRadius, 0.0f, height, slicesCount, stacksCount);
    }
}