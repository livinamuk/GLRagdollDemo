#pragma once
#include "Renderer/Decal.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Model.h"
#include "Core/Camera.h"
#include "Core/CoreGL.h"
#include "Core/Entity.h"
#include "Physics/physics.h"
#include "Renderer/GBuffer.h"
#include "Physics/Ragdoll.h"
#include "Core/GameCharacter.h"

class Renderer
{
	
public: // functions
	static void Init();
	static void RenderFrame(std::vector<GameCharacter>& gameCharacters, Entity& couch, Camera* camera);
	static void DrawGrid(Shader* shader);
	//static void DrawCouch(Shader* shader);
	static void RenderFinalImage();
	static void TextBlitterPass(Shader* shader);
	static void DrawLine(Shader* shader, Line line, glm::mat4 modelMatrix);
	static void HotLoadShaders();
	//static void DrawTangentDebugAxis(Shader* shader, glm::mat4 modelMatrix, float lineLength);
	//static void DrawAnimatedEntityDebugBones_Animated(Shader* shader, Ragdoll* ragdoll);
	static void DrawPoint(Shader* shader, glm::vec3 position, glm::vec3 color);
	//static void NewGameCharacter();
	static void RenderJointAxis(Shader*, GameCharacter* gameCharacter);

public: // variables
	static Shader s_test_shader;
	static Shader s_solid_color_shader;
	static Shader s_textued_2D_quad_shader;
	static Shader s_skinned_model_shader;
	static GBuffer s_gBuffer;
};

