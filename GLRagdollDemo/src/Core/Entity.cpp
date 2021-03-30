#include "Entity.h"
#include "Helpers/Util.h"


Entity::Entity()
{
}

Entity::Entity(Model* model, GLuint albedoTextureID)
{
	this->m_model = model;
	this->m_albedoTextureID = albedoTextureID;
	ConfigureDecalMap(1024, 1024);
}

void Entity::Update(float deltaTime)
{

}

void Entity::DrawEntity(Shader* shader)
{
	shader->setBool("u_hasDecalMap", true);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_albedoTextureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_decalMapID);
	m_model->Draw(shader, m_transform.to_mat4());
	shader->setBool("u_hasDecalMap", false);
}

void Entity::UpdateCollisionObject()
{
	//return;
	static bool hasBeenCreatedBefore = false;
	if (hasBeenCreatedBefore) {
		Physics::s_dynamicsWorld->removeCollisionObject(m_collisionObject);
		delete m_triangleMesh;
		delete m_triangleMeshShape;
		delete m_collisionObject;
		hasBeenCreatedBefore = true;
	}

	m_triangleMesh = new btTriangleMesh();

	int indexCount = m_model->m_meshes[0]->indices.size();

	for (int i = 0; i < indexCount; i += 3)
	{
		std::vector<Vertex>* vertices = &m_model->m_meshes[0]->vertices;
		std::vector<unsigned int>* indices = &m_model->m_meshes[0]->indices;

		glm::vec3 scale = m_transform.scale;
		btVector3 vertA = Util::glmVec3_to_btVec3(vertices->at(indices->at(i)).Position);
		btVector3 vertB = Util::glmVec3_to_btVec3(vertices->at(indices->at(i + 1)).Position);
		btVector3 vertC = Util::glmVec3_to_btVec3(vertices->at(indices->at(i + 2)).Position);
		m_triangleMesh->addTriangle(vertA, vertB, vertC);
	}

	m_triangleMeshShape = new btBvhTriangleMeshShape(m_triangleMesh, true, true);

	btTransform meshTransform;
	meshTransform.setIdentity();
	meshTransform.setOrigin(Util::glmVec3_to_btVec3(m_transform.position));
	meshTransform.setRotation(Util::glmVec3_to_btQuat(m_transform.rotation));

	m_collisionObject = new btCollisionObject();
	m_collisionObject->setCollisionShape(m_triangleMeshShape);
	m_collisionObject->setWorldTransform(meshTransform);
	m_collisionObject->setFriction(0.5);
	m_collisionObject->setCustomDebugColor(btVector3(1, 0, 0));

	EntityData* entityData = new EntityData();
	entityData->type = PhysicsObjectType::MISC_MESH;
	entityData->ptr = this;

	m_collisionObject->setUserPointer(entityData);
	m_collisionObject->getCollisionShape()->setLocalScaling(Util::glmVec3_to_btVec3(m_transform.scale));

	int group = CollisionGroups::HOUSE;
	int mask = CollisionGroups::ENTITY | CollisionGroups::ENEMY;

	Physics::s_dynamicsWorld->addCollisionObject(m_collisionObject, group, mask);

	m_collisionObject->setCollisionFlags(m_collisionObject->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

void Entity::RemoveCollisionObject()
{
	Physics::s_dynamicsWorld->removeCollisionObject(m_collisionObject);
}

void Entity::ConfigureDecalMap(int width, int height)
{
	glGenFramebuffers(1, &m_decalMapFBO);
	glGenTextures(1, &m_decalMapID);

	glBindFramebuffer(GL_FRAMEBUFFER, m_decalMapFBO);

	glBindTexture(GL_TEXTURE_2D, m_decalMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_decalMapID, 0);
	//	glGenerateMipmap(GL_TEXTURE_2D); // dunno about this, is it neccessary??

	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	// Clear it to test it works
	float clearcolor[] = { 0, 0, 1, 1 };
	glClearTexImage(m_decalMapID, 0, GL_RGBA, GL_FLOAT, clearcolor);

	glClearColor(1, 1, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}