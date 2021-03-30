#include "Physics.h"
#include "Helpers/Util.h"

btDiscreteDynamicsWorld* Physics::s_dynamicsWorld;
GLDebugDrawer Physics::s_debugDraw;
btDefaultCollisionConfiguration* Physics::s_collisionConfiguration;
btCollisionDispatcher* Physics::s_dispatcher;
btDbvtBroadphase* Physics::s_broadphase;
btSequentialImpulseConstraintSolver* Physics::s_sol;
btConstraintSolver* Physics::s_solver;

void Physics::Init()
{
	// Setup bullet physics 
	s_broadphase = new btDbvtBroadphase();
	s_collisionConfiguration = new btDefaultCollisionConfiguration();
	s_dispatcher = new btCollisionDispatcher(s_collisionConfiguration);
	s_solver = new btSequentialImpulseConstraintSolver();
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

