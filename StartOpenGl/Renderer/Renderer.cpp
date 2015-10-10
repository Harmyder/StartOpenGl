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

        gl::CreateVertexArrays(1, &_vao);
        gl::BindVertexArray(_vao);

        RECT rect;
        GetClientRect(wnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        SetWindowSize(width, height);
    }

    void Renderer::Deinitialize(HWND wnd)
    {
        gl::DeleteVertexArrays(1, &_vao);
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
        XMMATRIX world = XMLoadFloat4x4(&m_WorldTransform);
        XMVECTOR determinant;
        XMMATRIX view = XMMatrixInverse(&determinant, XMLoadFloat4x4(&m_ViewTransform));
        XMMATRIX proj = XMLoadFloat4x4(&m_ProjTransform);

        // Exclude scale part from world transform
        XMMATRIX A = world;
        A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // zero out translation row
        XMVECTOR det = XMMatrixDeterminant(A);
        A = XMMatrixInverse(&det, A);
        XMMATRIX inverseWorld = A;

        XMMATRIX worldViewProj = XMMatrixTranspose(world * view * proj);

        XMStoreFloat4x4(&m_ConstantPerObjectBufferData.gWorld, XMMatrixTranspose(world));
        XMStoreFloat4x4(&m_ConstantPerObjectBufferData.gWorldInvTranspose, XMMatrixTranspose(inverseWorld));
        XMStoreFloat4x4(&m_ConstantPerObjectBufferData.gWorldViewProj, worldViewProj);
    }

    void Renderer::Render()
    {
    }

    void Renderer::BuildCube()
    {
        Vertices vertices;
        GeometryPrimitivesBuilder::BuildCube(vertices);

        gl::CreateBuffers(1, &_boxVB);
        gl::NamedBufferStorage(_boxVB, sizeof(vertices[0]) * vertices.size(), vertices.data(), 0);
        gl::BindBuffer(gl::ARRAY_BUFFER, _boxVB);
    }

    void Renderer::DrawBoxShape(const glm::mat4 &transform, const glm::vec3 &halfExtents, glm::vec4 &color)
    {
        glm::mat4 scaled;
        glm::scale(scaled, halfExtents * 2.f);
        scaled = scaled * transform;

        DeviceDrawShape(_boxVB, scaled, color);
    }

    void Renderer::DeviceDrawShape(GLuint buffer, const glm::mat4 &transform, glm::vec4 &color)
    {
        _worldTransform = transform;


    }
}