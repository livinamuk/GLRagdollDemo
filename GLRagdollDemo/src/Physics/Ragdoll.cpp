#include "Ragdoll.h"
#include <math.h>


Ragdoll::Ragdoll()
{
}

void Ragdoll::RemovePhysicsObjects()
{
	for (RigidComponent& rigid : m_rigidComponents)
		Physics::s_dynamicsWorld->removeRigidBody(rigid.rigidBody);

	for (JointComponent& joint : m_jointComponents)
		Physics::s_dynamicsWorld->removeConstraint(joint.coneTwist);
}


void Ragdoll::BuildFromJsonFile(std::string filename, Transform spawnLocation)
{
	RemovePhysicsObjects();

	// Load file
	std::string fileName = "res/" + filename;
	FILE* pFile = fopen(fileName.c_str(), "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document document;
	document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

	// Check for errors
	if (document.HasParseError())
		std::cout << "Error  : " << document.GetParseError() << '\n' << "Offset : " << document.GetErrorOffset() << '\n';

	// Loop over entities
	for (auto& entity : document["entities"].GetObject())
	{
		auto const components = entity.value["components"].GetObject();
		if (components.HasMember("RigidComponent"))
		{
			RigidComponent rigidComponent;
			rigidComponent.ID = entity.value["id"].GetInt();
			rigidComponent.name = components["NameComponent"].GetObject()["members"].GetObject()["value"].GetString();
			rigidComponent.restMatrix = Util::Mat4FromJSONArray(components["RestComponent"].GetObject()["members"].GetObject()["matrix"].GetObject()["values"].GetArray());
			rigidComponent.scaleAbsoluteVector = Util::Vec3FromJSONArray(components["ScaleComponent"].GetObject()["members"].GetObject()["absolute"].GetObject()["values"].GetArray());
			rigidComponent.capsuleRadius = components["GeometryDescriptionComponent"].GetObject()["members"].GetObject()["radius"].GetFloat();
			rigidComponent.capsuleLength = components["GeometryDescriptionComponent"].GetObject()["members"].GetObject()["length"].GetFloat();
			rigidComponent.shapeType = components["GeometryDescriptionComponent"].GetObject()["members"].GetObject()["type"].GetString();
			rigidComponent.boxExtents = Util::Vec3FromJSONArray(components["GeometryDescriptionComponent"].GetObject()["members"].GetObject()["extents"].GetObject()["values"].GetArray());
			rigidComponent.offset = Util::Vec3FromJSONArray(components["GeometryDescriptionComponent"].GetObject()["members"].GetObject()["offset"].GetObject()["values"].GetArray());
			rigidComponent.rotation = Util::BtQuatFromJSONArray(components["GeometryDescriptionComponent"].GetObject()["members"].GetObject()["rotation"].GetObject()["values"].GetArray());
			rigidComponent.mass = components["RigidComponent"].GetObject()["members"].GetObject()["mass"].GetFloat();
			rigidComponent.friction = components["RigidComponent"].GetObject()["members"].GetObject()["friction"].GetFloat();
			rigidComponent.restitution = components["RigidComponent"].GetObject()["members"].GetObject()["restitution"].GetFloat();
			rigidComponent.linearDamping = components["RigidComponent"].GetObject()["members"].GetObject()["linearDamping"].GetFloat();
			rigidComponent.angularDamping = components["RigidComponent"].GetObject()["members"].GetObject()["angularDamping"].GetFloat();
			rigidComponent.sleepThreshold = components["RigidComponent"].GetObject()["members"].GetObject()["sleepThreshold"].GetFloat();
			rigidComponent.correspondingJointName = FindParentJointName(components["NameComponent"].GetObject()["members"].GetObject()["path"].GetString());
			
			if (rigidComponent.name != "rSceneShape")
				m_rigidComponents.push_back(rigidComponent);
		}

		if (components.HasMember("JointComponent"))
		{
			JointComponent jointComponent;
			jointComponent.name = components["NameComponent"].GetObject()["members"].GetObject()["value"].GetString();
			jointComponent.parentID = components["JointComponent"].GetObject()["members"].GetObject()["parent"].GetInt();
			jointComponent.childID = components["JointComponent"].GetObject()["members"].GetObject()["child"].GetInt();
			jointComponent.parentFrame = Util::Mat4FromJSONArray(components["JointComponent"].GetObject()["members"].GetObject()["parentFrame"].GetObject()["values"].GetArray());
			jointComponent.childFrame = Util::Mat4FromJSONArray(components["JointComponent"].GetObject()["members"].GetObject()["childFrame"].GetObject()["values"].GetArray());
			jointComponent.drive_angularDamping = components["DriveComponent"].GetObject()["members"].GetObject()["angularDamping"].GetFloat();
			jointComponent.drive_angularStiffness = components["DriveComponent"].GetObject()["members"].GetObject()["angularStiffness"].GetFloat();
			jointComponent.drive_linearDampening = components["DriveComponent"].GetObject()["members"].GetObject()["linearDamping"].GetFloat();
			jointComponent.drive_linearStiffness = components["DriveComponent"].GetObject()["members"].GetObject()["linearStiffness"].GetFloat();
			jointComponent.target = Util::Mat4FromJSONArray(components["DriveComponent"].GetObject()["members"].GetObject()["target"].GetObject()["values"].GetArray());
			jointComponent.limit_angularDampening = components["LimitComponent"].GetObject()["members"].GetObject()["angularDamping"].GetFloat();
			jointComponent.limit_angularStiffness = components["LimitComponent"].GetObject()["members"].GetObject()["angularStiffness"].GetFloat();
			jointComponent.limit_linearDampening = components["LimitComponent"].GetObject()["members"].GetObject()["linearDamping"].GetFloat();
			jointComponent.limit_linearStiffness = components["LimitComponent"].GetObject()["members"].GetObject()["linearStiffness"].GetFloat();
			jointComponent.twist = components["LimitComponent"].GetObject()["members"].GetObject()["twist"].GetFloat();
			jointComponent.swing1 = components["LimitComponent"].GetObject()["members"].GetObject()["swing1"].GetFloat();
			jointComponent.swing2 = components["LimitComponent"].GetObject()["members"].GetObject()["swing2"].GetFloat();
			jointComponent.limit.x = components["LimitComponent"].GetObject()["members"].GetObject()["x"].GetFloat();
			jointComponent.limit.y = components["LimitComponent"].GetObject()["members"].GetObject()["y"].GetFloat();
			jointComponent.limit.z = components["LimitComponent"].GetObject()["members"].GetObject()["z"].GetFloat();
			jointComponent.enabled = components["LimitComponent"].GetObject()["members"].GetObject()["enabled"].GetBool();
			m_jointComponents.push_back(jointComponent);
		}
	}

	for (RigidComponent& rigid : m_rigidComponents)
	{
		// Skip the scene rigid (it's outputted in the JSON export)
		if (rigid.name == "rSceneShape")
			continue;

		// APPLY SCALE
		if (rigid.shapeType == "Capsule") {
			rigid.capsuleRadius *= rigid.scaleAbsoluteVector.x;
			rigid.capsuleLength *= rigid.scaleAbsoluteVector.y;
		}
		else if (rigid.shapeType == "Box") {
			rigid.boxExtents.x *= rigid.scaleAbsoluteVector.x;
			rigid.boxExtents.y *= rigid.scaleAbsoluteVector.y;
			rigid.boxExtents.z *= rigid.scaleAbsoluteVector.z;
		}

		// Rigid world pos/rot
		btTransform transform;
		transform.setIdentity(); 
		transform.setOrigin(Util::glmVec3_to_btVec3(Util::GetTranslationFromMatrix((rigid.restMatrix))));
		transform.setRotation(Util::GetRotationFromBoneMatrix(rigid.restMatrix));

		// Do you wanna create this ragdoll somewhere other than (0,0,0)? Probably
		btTransform spawnTransform;
		spawnTransform.setIdentity();
		spawnTransform.setOrigin(Util::glmVec3_to_btVec3(spawnLocation.position));
		spawnTransform.setRotation(Util::glmVec3_to_btQuat(spawnLocation.rotation));
		transform = spawnTransform * transform;

		// The offset shape requires a 90 degree rotate because PhysX and Bullet don't agree.
		// That's what that quaternion down below in setRotation() is doing
		btTransform OffsetTransform;
		OffsetTransform.setIdentity();
		OffsetTransform.setRotation(rigid.rotation * btQuaternion(0, 0, 0.7070999742, 0.7070999742));  
		OffsetTransform.setOrigin(Util::glmVec3_to_btVec3(rigid.offset* rigid.scaleAbsoluteVector));

		// Shape
		btCollisionShape* shape;
		btCompoundShape* compoundShape;
		if (rigid.shapeType == "Box")
		{
			glm::vec3 extents(rigid.boxExtents.y, rigid.boxExtents.x, rigid.boxExtents.z);
			shape = new btBoxShape(Util::glmVec3_to_btVec3(extents) * 0.5);
			compoundShape = new btCompoundShape();
			compoundShape->addChildShape(OffsetTransform, shape);
		}
		else if (rigid.shapeType == "Capsule")
		{
			shape = new btCapsuleShape(rigid.capsuleRadius, rigid.capsuleLength);
			compoundShape = new btCompoundShape();
			compoundShape->addChildShape(OffsetTransform, shape);
		}

		// Create rigid body
		btVector3 localInertia(0, 0, 0);
		shape->calculateLocalInertia(1, localInertia);
		btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(rigid.mass, myMotionState, compoundShape, localInertia);
		rbInfo.m_friction = rigid.friction;
		rbInfo.m_restitution = rigid.restitution;
		rbInfo.m_linearDamping = rigid.linearDamping;
		rbInfo.m_angularDamping = rigid.angularDamping;
		rbInfo.m_angularSleepingThreshold = 1;// rigid.sleepThreshold;
		rbInfo.m_linearSleepingThreshold = 1;//rigid.sleepThreshold;
		rigid.rigidBody = new btRigidBody(rbInfo);
		EntityData* entityData = new EntityData();
		entityData->type = PhysicsObjectType::RAGDOLL;
		rigid.rigidBody->setUserPointer(entityData);
		int group = CollisionGroups::ENEMY;
		int mask = CollisionGroups::TERRAIN | CollisionGroups::ENEMY | CollisionGroups::HOUSE;
		// Hard coded some values cause my JSON has shitty ones
		rigid.rigidBody->setDamping(0.50f, 3.0f);
		rigid.rigidBody->setDeactivationTime(5.8f);
		rigid.rigidBody->setFriction(0.8);
		btDynamicsWorld* m_ownerWorld = Physics::s_dynamicsWorld;
		m_ownerWorld->addRigidBody(rigid.rigidBody, group, mask);
	}

	for (JointComponent& joint : m_jointComponents)
	{
		// Get pointers to the parent and child rigid bodies
		RigidComponent* parentRigid;
		RigidComponent* childRigid;
		for (RigidComponent& rigid : m_rigidComponents)
		{
			if (rigid.ID == joint.parentID)
				parentRigid = &rigid;
			if (rigid.ID == joint.childID)
				childRigid = &rigid;
		}

		// Create parent frame
		btTransform localA;
		localA.setIdentity();
		localA.setOrigin(Util::glmVec3_to_btVec3(Util::GetTranslationFromMatrix(joint.parentFrame)));
		localA.setBasis(Util::BtMat3FromGlmMatrix(joint.parentFrame));

		// Create child frame
		btTransform localB;
		localB.setIdentity();
		localB.setOrigin(Util::glmVec3_to_btVec3(Util::GetTranslationFromMatrix(joint.childFrame)));
		localB.setBasis(Util::BtMat3FromGlmMatrix(joint.childFrame));

		// Create the actual cone twist constraint
		joint.coneTwist = new btConeTwistConstraint(*parentRigid->rigidBody, *childRigid->rigidBody, localA, localB);
		joint.coneTwist->setLimit(joint.swing1, joint.swing2, joint.twist);
		Physics::s_dynamicsWorld->addConstraint(joint.coneTwist, true);
	}
}

std::string Ragdoll::FindParentJointName(std::string query)
{
	size_t occuranceCount = std::count(query.begin(), query.end(), '|'); // #include <algorithm>
	if (occuranceCount < 2)
		return "undefined";

	std::size_t lastOccurance = query.rfind("|");
	std::size_t secondLastOccurance = query.substr(0, lastOccurance).rfind("|") + 1;
	return query.substr(secondLastOccurance, lastOccurance - secondLastOccurance);
}

RigidComponent* Ragdoll::GetRigidByName(std::string name)
{
	for (RigidComponent& rigidComponent : m_rigidComponents) {
		if (rigidComponent.correspondingJointName == name) {
			return &rigidComponent;
		}
	}
	return nullptr;
}