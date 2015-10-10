#pragma once

namespace Renderer
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture;
    };

    typedef std::vector<Vertex> Vertices;
}
