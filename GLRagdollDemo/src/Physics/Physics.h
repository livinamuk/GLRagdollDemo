#pragma once
#include "bullet/src/btBulletCollisionCommon.h"
#include "bullet/src/LinearMath/btVector3.h"
//#include "bullet/src/LinearMath/btAlignedObjectArray.h"
#include "bullet/src/BulletDynamics/Character/btKinematicCharacterController.h"

#include "Renderer/GLDebugDraw.h"

class Physics
{
public: // fields
		
	static GLDebugDrawer s_debugDraw;
	static btDefaultCollisionConfiguration* s_collisionConfiguration;
	static btCollisionDispatcher* s_dispatcher;
	static btDbvtBroadphase* s_broadphase;
	static btSequentialImpulseConstraintSolver* s_sol;
	static btConstraintSolver* s_solver;
	//static btAlignedObjectArray<btCollisionShape*> s_collisionShapes;
	static btDiscreteDynamicsWorld* s_dynamicsWorld;
	//static btAlignedObjectArray<btRigidBody*> s_rigidBodies;
	//static btAlignedObjectArray<btCollisionObject*> s_collisionObjects;
	//static std::vector<glm::vec3> s_points;
	//static std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> s_objectsCollisions;

public: // methods 
	static void Init();
	static void Update(float deltaTime);
	static void DebugDraw(Shader* shader);

};
