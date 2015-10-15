#include "stdafx.h"

#include "Renderer\Renderer.h"
#include "Renderer\GeometryPrimitivesBuilder.h"

using namespace std;

namespace Renderer
{
    Renderer::Renderer() :
        _fov(glm::quarter_pi<float>()),
        _nearPlane(1.f),
        _farPlane(200.f)
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Initialize(HWND wnd)
    {
        _hdc = GetDC(wnd);

        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cDepthBits = 16;
        pfd.iLayerType = PFD_MAIN_PLANE;
        int iFormat = ChoosePixelFormat(_hdc, &pfd);
        SetPixelFormat(_hdc, iFormat, &pfd);

        _hglrc = wglCreateContext(_hdc);
        wglMakeCurrent(_hdc, _hglrc);

        gl::sys::LoadFunctions();

        GLint major = gl::sys::GetMajorVersion();
        GLint minor = gl::sys::GetMinorVersion();        
        assert(major >= 4);
        assert(minor >= 5);

        _renderingProgram = ReadEffects();

        gl::CreateVertexArrays(1, &_boxVAO);
        gl::BindVertexArray(_boxVAO);

        RECT rect;
        GetClientRect(wnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        SetWindowSize(width, height);
    }

    void Renderer::Deinitialize(HWND wnd)
    {
        gl::DeleteVertexArrays(1, &_boxVAO);
        gl::DeleteProgram(_renderingProgram);

        if (_hglrc) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(_hglrc);
        }
        if (_hdc)
            ReleaseDC(wnd, _hdc);
    }

    GLuint Renderer::ReadEffects()
    {
        stringstream buffer;

        ifstream vertexShaderFile("vertex_shader.glsl");
        buffer << vertexShaderFile.rdbuf();
        const GLchar* vertexShaderSource = buffer.str().c_str();
        
        ifstream fragmentShaderFile("fragment_shader.glsl");
        buffer << fragmentShaderFile.rdbuf();
        const GLchar* fragmentShaderSource = buffer.str().c_str();

        GLuint vertexShader = gl::CreateShader(gl::VERTEX_SHADER);
        gl::ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        gl::CompileShader(vertexShader);
        
        GLuint fragmentShader = gl::CreateShader(gl::FRAGMENT_SHADER);
        gl::ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        gl::CompileShader(fragmentShader);
        
        GLuint program = gl::CreateProgram();
        gl::AttachShader(program, vertexShader);
        gl::AttachShader(program, fragmentShader);
        gl::LinkProgram(program);

        gl::DeleteShader(vertexShader);
        gl::DeleteShader(fragmentShader);
        return program;
    }

    void Renderer::SetWindowSize(int width, int height)
    {
        if (_width != width || _height != height)
        {
            _width = width;
            _height = height;
            _projTransform = glm::perspective(
                GetCameraFOV(), 
                GetCameraAspectRatio(),
                GetCameraNearClipPlane(),
                GetCameraFarClipPlane()
                );
        }
    }

    void Renderer::BeginScene(uint32 width, uint32 height)
    {
        SetWindowSize(width, height);

        const GLfloat color[] = { 0.5f, 0.5f, 0.0f, 0.7f };
        gl::ClearBufferfv(gl::COLOR, 0, color);
    }

    void Renderer::EndScene()
    {
        gl::UseProgram(_renderingProgram);
        SwapBuffers(_hdc);
    }

    void Renderer::SetupWorldViewProjTransform()
    {
		glm::mat4 worldViewProj = _projTransform * _viewTransform * _worldTransform;

		GLint model = gl::GetUniformLocation(_renderingProgram, "Model");
		gl::UniformMatrix4fv(model, 1, FALSE, glm::value_ptr(worldViewProj));
	}

    void Renderer::Render()
    {
    }

    void Renderer::BuildCube()
    {
        Vertices vertices;
        GeometryPrimitivesBuilder::BuildCube(vertices);

        gl::CreateBuffers(1, &_boxVBO);
        gl::NamedBufferStorage(_boxVBO, sizeof(vertices[0]) * vertices.size(), vertices.data(), 0);

		gl::VertexArrayAttribBinding(_boxVAO, 0, 0);
		gl::VertexArrayAttribFormat(_boxVAO, 0, 3, gl::FLOAT, FALSE, offsetof(Vertex, position));
		gl::EnableVertexArrayAttrib(_boxVAO, 0);

		gl::VertexArrayAttribBinding(_boxVAO, 1, 0);
		gl::VertexArrayAttribFormat(_boxVAO, 1, 3, gl::FLOAT, FALSE, offsetof(Vertex, normal));
		gl::EnableVertexArrayAttrib(_boxVAO, 1);

		gl::VertexArrayVertexBuffer(_boxVAO, 0, _boxVBO, 0, sizeof(Vertex));
    }

    void Renderer::DrawBoxShape(const glm::mat4 &transform, const glm::vec3 &halfExtents, glm::vec4 &color)
    {
        glm::mat4 scaled;
        glm::scale(scaled, halfExtents * 2.f);
        scaled = scaled * transform;

		gl::BindVertexArray(_boxVAO); 
		DeviceDrawShape(_boxVBO, scaled, color);
    }

    void Renderer::DeviceDrawShape(GLuint , const glm::mat4 &transform, glm::vec4 &)
    {
        _worldTransform = transform;
		SetupWorldViewProjTransform();

		gl::DrawArrays(gl::TRIANGLES, 0, 1);
    }
}