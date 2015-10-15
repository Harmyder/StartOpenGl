#pragma once

namespace Renderer
{
    class Renderer final
    {
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

    public:
        Renderer();
        ~Renderer();

    public:
        void Initialize(HWND wnd);
        void Deinitialize(HWND wnd);
        void SetCameraTransform(...);

        void BeginScene(uint32 width, uint32 height);
        void EndScene();

        void Render();

        float GetCameraAspectRatio() const { return static_cast<float>(_width) /_height; }
        float GetCameraFOV() const { return _fov; }
        float GetCameraNearClipPlane() const { return _nearPlane; }
        float GetCameraFarClipPlane() const { return _farPlane; }

        void SetCameraTransform(const glm::mat4 &transform) { _viewTransform = transform; }

        void DrawBoxShape(const glm::mat4 &transform, const glm::vec3 &halfExtents, glm::vec4 &color);

    private:
        GLuint ReadEffects();
        void SetWindowSize(int width, int height);

        void SetupWorldViewProjTransform();

        void BuildCube();

        void DeviceDrawShape(GLuint buffer, const glm::mat4 &transform, glm::vec4 &color);

    private:
        HDC _hdc;
        HGLRC _hglrc;

        const float _fov;
        const float _nearPlane;
        const float _farPlane;
        int _width;
        int _height;

        glm::mat4 _worldTransform;
        glm::mat4 _viewTransform;
        glm::mat4 _projTransform;

        GLuint _boxVBO;
		GLuint _boxVAO;

        GLuint _renderingProgram;
    };
}