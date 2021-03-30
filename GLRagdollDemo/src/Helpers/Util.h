#pragma once
#include "Header.h"
#include "Renderer/Transform.h"
#include "Renderer/Shader.h"
#include "bullet/src/btBulletCollisionCommon.h"
#include "bullet/src/btBulletDynamicsCommon.h"
#include "bullet/src/LinearMath/btVector3.h"
#include "bullet/src/LinearMath/btAlignedObjectArray.h"

#include "rapidjson/document.h"
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include "assimp/matrix4x4.h"
#include "assimp/matrix3x3.h"

class Util
{
public:
	static void PrintMat4(glm::mat4 m);
	static void PrintVec3(glm::vec3 v);
	static std::string FileNameFromPath(std::string filepath);
	static FileType FileTypeFromPath(std::string filepath);
	static void SetNormalsAndTangentsFromVertices(Vertex* vert0, Vertex* vert1, Vertex* vert2);
	static glm::vec3 NormalFromTriangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2);

	static btVector3 glmVec3_to_btVec3(glm::vec3 vector);
	static btQuaternion glmVec3_to_btQuat(glm::vec3 vector);
	static glm::vec3 btVec3_to_glmVec3(btVector3);
	static std::string PhysicsObjectEnumToString(PhysicsObjectType objectType); 
	static btQuaternion GetRotationFromBoneMatrix(glm::mat4 matrix);
	
	static std::string BoolToString(bool boolean);

	static glm::mat4 Mat4FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);
	static glm::vec3 Vec3FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);
	static btQuaternion BtQuatFromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);
	static btMatrix3x3 BtMat3FromGlmMatrix(glm::mat4 matrix);

	static glm::vec3 GetTranslationFromMatrix(glm::mat4 matrix);

	static void InterpolateQuaternion(glm::quat& pOut, const glm::quat& Start, const glm::quat& End, float Factor);		
	static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
	static glm::mat4 aiMatrix3x3ToGlm(const aiMatrix3x3& from);
	static glm::mat4 Mat4InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	static glm::mat4 Mat4InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	static glm::mat4 Mat4InitTranslationTransform(float x, float y, float z);		
	static bool StrCmp(const char* queryA, const char* queryB); 
	static const char* CopyConstChar(const char* text);
	static glm::mat4 btTransformToMat4(btTransform& trans);
};
