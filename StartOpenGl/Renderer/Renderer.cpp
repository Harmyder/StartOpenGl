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

		BuildCube();

        RECT rect;
        GetClientRect(wnd, &rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        SetWindowSize(width, height);
    }

    void Renderer::Deinitialize(HWND wnd)
    {
		gl::DeleteVertexArrays(1, &_boxVAO);
		gl::DeleteBuffers(1, &_boxVBO);

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
		GLuint vertexShader = gl::CreateShader(gl::VERTEX_SHADER);
		ReadEffect(vertexShader, "vertex_shader.glsl");

		GLuint fragmentShader = gl::CreateShader(gl::FRAGMENT_SHADER);
		ReadEffect(fragmentShader, "fragment_shader.glsl");
		
        GLuint program = gl::CreateProgram();
        gl::AttachShader(program, vertexShader);
        gl::AttachShader(program, fragmentShader);
        gl::LinkProgram(program);

        gl::DeleteShader(vertexShader);
        gl::DeleteShader(fragmentShader);
        return program;
    }

	void Renderer::ReadEffect(GLuint shader, const char* file) const
	{
		stringstream buffer;
		ifstream shaderFile(file);
		buffer << shaderFile.rdbuf();
		const string shaderSourceStr = buffer.str();
		const GLchar* shaderSource = shaderSourceStr.c_str();

		gl::ShaderSource(shader, 1, &shaderSource, NULL);
		gl::CompileShader(shader);
		GLint param;
		gl::GetShaderiv(shader, gl::COMPILE_STATUS, &param);
		if (param != TRUE)
		{
			GLint length;
			GLchar buf[1024];
			gl::GetShaderInfoLog(shader, 1024, &length, buf);
			OutputDebugStringA(buf); OutputDebugStringA("\n");
			OutputDebugStringA(shaderSource); OutputDebugStringA("\n");
		}
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

		gl::CreateVertexArrays(1, &_boxVAO);
	
		gl::VertexArrayAttribFormat(_boxVAO, 1, 3, gl::FLOAT, FALSE, offsetof(Vertex, position));
		gl::VertexArrayAttribBinding(_boxVAO, 1, 0);
		gl::EnableVertexArrayAttrib(_boxVAO, 1);

		gl::VertexArrayAttribFormat(_boxVAO, 2, 3, gl::FLOAT, FALSE, offsetof(Vertex, normal));
		gl::VertexArrayAttribBinding(_boxVAO, 2, 0);
		gl::EnableVertexArrayAttrib(_boxVAO, 2);

		gl::VertexArrayVertexBuffer(_boxVAO, 0, _boxVBO, 0, sizeof(Vertex)); // bind to vertex array
	}

    void Renderer::DrawBoxShape(const glm::mat4 &transform, const glm::vec3 &halfExtents, glm::vec4 &color)
    {
        glm::mat4 scaled;
        glm::scale(scaled, halfExtents * 2.f);
        scaled = scaled * transform;

		DeviceDrawShape(_boxVBO, scaled, color);
    }

    void Renderer::DeviceDrawShape(GLuint , const glm::mat4 &transform, glm::vec4 &color)
    {
        _worldTransform = transform;
		SetupWorldViewProjTransform();

		gl::VertexAttrib4fv(0, &color[0]);

		gl::BindVertexArray(_boxVAO);
		gl::DrawArrays(gl::TRIANGLES, 0, 16);
    }
}