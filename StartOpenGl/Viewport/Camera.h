#pragma once

namespace Viewport {
   class Camera final
    {
    public:
        Camera();

        void SetTransform(const glm::mat4& transform) { _transform = transform; }
        const glm::mat4 &GetTransform() const { return _transform; }

        void MoveForward(float augment);

    private:
        glm::mat4 _transform;

        glm::vec4 &_right;
        glm::vec4 &_up;
        glm::vec4 &_forward;
        glm::vec4 &_translation;
   };

}
