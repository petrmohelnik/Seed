#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>

#include "imgui/imgui.h""
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <experimental/vector>
#include <fstream>
#include <sstream>
#include <cmath>
