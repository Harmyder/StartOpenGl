#pragma once

namespace Viewport {
   class Camera
    {
    public:
        Camera();

        void SetTransform(const glm::mat4& transform) { _transform = transform; }
        const glm::mat4 &GetTransform() const { return _transform; }

    private:
        glm::mat4 _transform;

   };

}
