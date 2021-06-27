#include "Physics.h"
#include "Helpers/Util.h"

btDiscreteDynamicsWorld* Physics::s_dynamicsWorld;
GLDebugDrawer Physics::s_debugDraw;
btDefaultCollisionConfiguration* Physics::s_collisionConfiguration;
btCollisionDispatcher* Physics::s_dispatcher;
btDbvtBroadphase* Physics::s_broadphase;
btSequentialImpulseConstraintSolver* Physics::s_sol;
btConstraintSolver* Physics::s_solver;
int Physics::s_subSteps = 64;

void Physics::Init()
{
	// Setup bullet physics 
	s_broadphase = new btDbvtBroadphase();
	s_collisionConfiguration = new btDefaultCollisionConfiguration();
	s_dispatcher = new btCollisionDispatcher(s_collisionConfiguration);
	s_solver = new btSequentialImpulseConstraintSolver();
                  // btSequentialImpulseConstraintSolver
    /*btVector3 worldAabbMin(-10000, -10000, -10000);
    btVector3 worldAabbMax(10000, 10000, 10000);
    auto m_solver = new btNNCGConstraintSolver;
    auto m_broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax);

    s_dynamicsWorld = new btDiscreteDynamicsWorld(s_dispatcher, m_broadphase, m_solver, s_collisionConfiguration);
    s_dynamicsWorld->getDispatchInfo().m_useContinuous = true;
    s_dynamicsWorld->getSolverInfo().m_numIterations = 100;
    */
      s_dynamicsWorld = new btDiscreteDynamicsWorld(s_dispatcher, s_broadphase, s_solver, s_collisionConfiguration);
	s_dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// Set up Bullet debug draw
	s_debugDraw.Init();
	s_dynamicsWorld->setDebugDrawer(&s_debugDraw);
	s_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_FastWireframe + btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawConstraintLimits);

	// Create the ground	
	btBoxShape* s_groundShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	s_groundShape->setLocalScaling(btVector3(WORLD_GROUND_SIZE, WORLD_GROUND_HEIGHT * 2, WORLD_GROUND_SIZE));
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0, -WORLD_GROUND_HEIGHT, 0));
	float friction = 0.5f;
	int collisionGroup = CollisionGroups::TERRAIN;
	int collisionMask = CollisionGroups::ENEMY;
	PhysicsObjectType objectType = PhysicsObjectType::UNDEFINED;
	btCollisionObject* collisionObject = new btCollisionObject();
	collisionObject->setCollisionShape(s_groundShape);
	collisionObject->setWorldTransform(transform);
	EntityData* entityData = new EntityData();
	entityData->type = objectType;
	entityData->ptr = nullptr;
	collisionObject->setUserPointer(entityData);
	collisionObject->setCollisionFlags(collisionObject->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	s_dynamicsWorld->addCollisionObject(collisionObject, collisionGroup, collisionMask);
}


void Physics::DebugDraw(Shader* shader)
{
	s_debugDraw.lines.clear();
	s_debugDraw.vertices.clear();
	Physics::s_dynamicsWorld->debugDrawWorld();

	for (int i = 0; i < s_debugDraw.lines.size() - 1; i++)
	{
		Vertex vert0, vert1;
		vert0.Position = s_debugDraw.lines[i].start_pos;
		vert0.Normal = s_debugDraw.lines[i].start_color;	// Yes. You are storing the color in the vertex normal spot.
		vert1.Position = s_debugDraw.lines[i].end_pos;
		vert1.Normal = s_debugDraw.lines[i].end_color;
		s_debugDraw.vertices.push_back(vert0);
		s_debugDraw.vertices.push_back(vert1);
	};

	glBindBuffer(GL_ARRAY_BUFFER, s_debugDraw.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * s_debugDraw.vertices.size(), &s_debugDraw.vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(s_debugDraw.VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	shader->setVec3("color", s_debugDraw.lines[0].start_color);
	shader->setMat4("model", glm::mat4(1));
	glBindVertexArray(s_debugDraw.VAO);
	glDrawArrays(GL_LINES, 0, s_debugDraw.vertices.size());
}




struct RayResult {
    btRigidBody* pBody;
    btVector3 hitPoint;
};




#include "Physics/RaycastResult.h"
#include "Core/CoreGL.h"

void Physics::DragRagdoll(Camera* camera)
{
    static btTypedConstraint* m_pPickConstraint;
    static btRigidBody* m_pPickedBody;
    static glm::vec3 grabPos;

    // User clicked mouse? Well then create a new constraint 
    if (Input::s_leftMousePressed)
    {
        RaycastResult mouseRay = RaycastResult::CastMouseRay(camera, Input::s_mouseX, Input::s_mouseY, CoreGL::s_windowWidth, CoreGL::s_windowHeight);

        if (mouseRay.m_objectType == PhysicsObjectType::RAGDOLL)
        {
            grabPos = mouseRay.m_hitPoint;
            m_pPickedBody = mouseRay.m_rigidBody;
            m_pPickedBody->setActivationState(DISABLE_DEACTIVATION);

            // get the hit position relative to the body we hit
            btVector3 localPivot = m_pPickedBody->getCenterOfMassTransform().inverse() * Util::glmVec3_to_btVec3(grabPos);

            // create a transform for the pivot point
            btTransform pivot;
            pivot.setIdentity();
            pivot.setOrigin(localPivot);

            // create the constraint object
            btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*m_pPickedBody, pivot, true);
            bool bLimitAngularMotion = true;
            if (bLimitAngularMotion) {
                dof6->setAngularLowerLimit(btVector3(-HELL_PI, -HELL_PI, -HELL_PI));
                dof6->setAngularUpperLimit(btVector3(HELL_PI, HELL_PI, HELL_PI));
            }

            Physics::s_dynamicsWorld->addConstraint(dof6, true);

            // store a pointer to our constraint
            m_pPickConstraint = dof6;

            // define the strength of each axis
            float cfm = 0.5f;
            dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 0);
            dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 1);
            dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 2);
            dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 3);
            dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 4);
            dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 5);

            // define the error reduction of each axis
            float erp = 0.5f;
            dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 0);
            dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 1);
            dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 2);
            dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 3);
            dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 4);
            dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 5);
        }
    }


    if (Input::s_leftMouseDown)
    {
        if (m_pPickedBody) {
            btGeneric6DofConstraint* pickCon = static_cast<btGeneric6DofConstraint*>(m_pPickConstraint);

            if (pickCon)
            {
                // Ray to plane intersection 
                float dd;
                glm::vec3 origin = camera->m_transform.position;
                glm::vec3 dirr = Util::GetMouseRay(camera->m_projectionMatrix, camera->m_viewMatrix, Input::s_mouseX, Input::s_mouseY, CoreGL::s_windowWidth, CoreGL::s_windowHeight);
                glm::vec3 plane_point = grabPos;
                //glm::vec3 normal = camera.m_Front * glm::vec3(-1);
                glm::vec3 normal = glm::vec3(0, 0, 1);

                if (Util::GetRayPlaneIntersecion(origin, dirr, plane_point, normal, dd))
                {
                    glm::vec3 location = origin + (dirr * dd);
                    
                    if (location.y < 0)
                        location.y = 0;
                    //TextBlitter::BlitLine("initial grab pos:        " + std::to_string(grabPos.x) + ", " + std::to_string(grabPos.y) + ", " + std::to_string(grabPos.z));
                    //TextBlitter::BlitLine("ray plane intersect pos: " + std::to_string(location.x) + ", " + std::to_string(location.y) + ", " + std::to_string(location.z));
                    pickCon->getFrameOffsetA().setOrigin(Util::glmVec3_to_btVec3(location));
                }
            }
        }
    }
    else
    {
        // exit in erroneous situations
        if (!m_pPickConstraint || !Physics::s_dynamicsWorld)
            return;

        // remove the constraint from the world
        Physics::s_dynamicsWorld->removeConstraint(m_pPickConstraint);

        // delete the constraint object
        delete m_pPickConstraint;

        // reactivate the body
        m_pPickedBody->forceActivationState(ACTIVE_TAG);
        m_pPickedBody->setDeactivationTime(0.f);

        // clear the pointers
        m_pPickConstraint = 0;
        m_pPickedBody = 0;
    }
}

void Physics::Update(float deltatime)
{
    // Physics Step
    //int substeps = 2;
    btScalar timeStep = 1.0 / 60.0;
    for (auto s = 0U; s < s_subSteps; s++)
        Physics::s_dynamicsWorld->stepSimulation(deltatime / s_subSteps, 1, timeStep / s_subSteps);
}



