// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#include <stdio.h>
#include <tchar.h>

#include <assert.h>

#include <Windows.h>
#include "3rdParty\gl_core_4_5.hpp"
#include <gl\gl.h>
#include <gl\glu.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define GLM_FORCE_CXX03
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>

#include <memory>

#include <vector>

#include "SDK\Types.h"

