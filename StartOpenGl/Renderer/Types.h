#pragma once

namespace Renderer
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    typedef std::vector<Vertex> Vertices;
}
