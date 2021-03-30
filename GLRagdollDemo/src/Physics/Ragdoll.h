#pragma once

#include "rapidjson/document.h"
#include <rapidjson/filereadstream.h>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include "Header.h"
#include "Helpers/Util.h"
#include "Physics/Physics.h"


struct RigidComponent
{
public:
	int ID;
	std::string name, correspondingJointName, shapeType;
	glm::mat4 restMatrix;
	glm::vec3 scaleAbsoluteVector;
	glm::vec3 boxExtents, offset;
	float capsuleLength, capsuleRadius;
	btQuaternion rotation;
	btRigidBody* rigidBody;
	float mass, friction, restitution, linearDamping, angularDamping, sleepThreshold;
};

struct JointComponent
{
public:
	std::string name;
	int parentID, childID;
	glm::mat4 parentFrame, childFrame;

	// Drive component
	float drive_angularDamping, drive_angularStiffness, drive_linearDampening, drive_linearStiffness;
	glm::mat4 target;

	// Limit component
	float twist, swing1, swing2, limit_angularStiffness, limit_angularDampening, limit_linearStiffness, limit_linearDampening;
	btConeTwistConstraint* coneTwist;
	glm::vec3 limit;
	bool enabled;
};


class Ragdoll
{
public:
	Ragdoll();
	void RemovePhysicsObjects();
	std::vector<RigidComponent> m_rigidComponents;
	std::vector<JointComponent> m_jointComponents;
	std::vector<glm::mat4> m_animatedTransforms;
	std::vector<glm::mat4> m_animatedDebugTransforms_Animated;

	void BuildFromJsonFile(std::string filename, Transform spawnLocation);

	RigidComponent* GetRigidByName(std::string name);

private:
	std::string FindParentJointName(std::string query);
};

