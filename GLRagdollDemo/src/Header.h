#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/hash.hpp"
#include <iostream>
#include <vector>
#include "Renderer/Shader.h"
#include "Core/keycodes.h"
#include <stb_image.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Renderer/Transform.h"
#include <map>


//#define SCR_WIDTH 1280//1920//2880 //1920//1280
//#define SCR_HEIGHT 720//1080//1620//1080//720
#define SCR_WIDTH 1920//1920//2880 //1920//1280
#define SCR_HEIGHT 1080//1080//1620//1080//720
#define NEAR_PLANE 0.01f
#define FAR_PLANE 50.0f
#define HELL_PI	(float)3.14159265358979323846

enum FileType { FBX, OBJ, PNG, JPG, UNKNOWN };

struct Vertex2D {
	glm::vec2 Position;
	glm::vec2 TexCoords;
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	unsigned int BlendingIndex[4];
	glm::vec4 BlendingWeight;
	unsigned int MaterialID;

	bool operator==(const Vertex& other) const {
		return Position == other.Position && Normal == other.Normal && TexCoords == other.TexCoords;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.Position) ^ (hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.TexCoords) << 1);
		}
	};
}

struct Line {
	glm::vec3 start_pos;
	glm::vec3 end_pos;
	glm::vec3 start_color;
	glm::vec3 end_color;
};

enum class PhysicsObjectType { UNDEFINED, WALL, EDITOR_WINDOW, DOOR, FLOOR, STAIRS, PLAYER, RAGDOLL, CEILING, SHELL_PROJECTILE, MISC_MESH, GLASS };

struct EntityData {
	unsigned int enumValue = 0;
	PhysicsObjectType type;
	void* ptr;
};

enum CollisionGroups {
	NONE = 0,
	HOUSE = 1 << 0,
	PLAYER = 1 << 1,
	PROJECTILES = 1 << 2,
	ENTITY = 2 << 3,
	ENEMY = 3 << 4,
	EDITOR_ONLY = 4 << 5,
	TERRAIN = 5 << 6
};

#define WORLD_GROUND_SIZE	150.0f
#define WORLD_GROUND_HEIGHT	1.0f

#define DEBUG_COLOR_DOOR btVector3(0.4f, 0.5f, 0.5f)
#define DEBUG_COLOR_WALL btVector3(0.6f, 0.5f, 0.5f)
#define DEBUG_COLOR_RAMP btVector3(0.9f, 0.8f, 0.8f)
#define DEBUG_COLOR_YELLOW btVector3(1.0f, 1.0f, 0.0f)
#define DEBUG_COLOR_GROUND btVector3(0.2f, 0.2f, 0.2f)
#define DEBUG_COLOR_STATIC_ENTITY btVector3(1, 1, 0)

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
#define ToRadian(x) (float)(((x) * HELL_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / HELL_PI)) 

#define POSITION_LOCATION    0
#define NORMAL_LOCATION		 1
#define TEX_COORD_LOCATION   2
#define TANGENT_LOCATION     3
#define BITANGENT_LOCATION   4
#define BONE_ID_LOCATION     5
#define BONE_WEIGHT_LOCATION 6

enum VB_TYPES {
	INDEX_BUFFER,
	POS_VB,
	NORMAL_VB,
	TEXCOORD_VB,
	TANGENT_VB,
	BITANGENT_VB,
	BONE_VB,
	NUM_VBs
};  