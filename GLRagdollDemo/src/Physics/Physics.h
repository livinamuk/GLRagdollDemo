#pragma once
#include "bullet/src/btBulletCollisionCommon.h"
#include "bullet/src/LinearMath/btVector3.h"
#include "bullet/src/BulletDynamics/Character/btKinematicCharacterController.h"

#include "Renderer/GLDebugDraw.h"
#include "Core/Camera.h"

class Physics
{
public: // fields
		
	static GLDebugDrawer s_debugDraw;
	static btDefaultCollisionConfiguration* s_collisionConfiguration;
	static btCollisionDispatcher* s_dispatcher;
	static btDbvtBroadphase* s_broadphase;
	static btSequentialImpulseConstraintSolver* s_sol;
	static btConstraintSolver* s_solver;
	static btDiscreteDynamicsWorld* s_dynamicsWorld;
	 
	static int s_subSteps;

public: // methods 
	static void Init();
	static void DebugDraw(Shader* shader);
	static void DragRagdoll(Camera* camera);
	static void Update(float deltatime);
};
