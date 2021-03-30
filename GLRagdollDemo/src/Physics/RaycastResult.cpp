#include "RaycastResult.h"
#include "Core/CoreGL.h"

RaycastResult::RaycastResult()
{

}

void RaycastResult::CastRay(glm::vec3 rayOrigin, glm::vec3 rayDirection, float rayLength)
{
	// Bail if NAN origin`
	if (rayOrigin.x != rayOrigin.x)
		return;

	// Reset
	m_distance = 0;
	m_hitPoint = glm::vec3(0);;
	m_objectPtr = nullptr;
	m_surfaceNormal = glm::vec3(0);
	m_objectType = PhysicsObjectType::UNDEFINED;
		
	// End of ray
	glm::vec3 rayEnd = rayOrigin + (rayDirection * glm::vec3(rayLength));

	btCollisionWorld::ClosestRayResultCallback rayCallback(
		btVector3(rayOrigin.x, rayOrigin.y, rayOrigin.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z)
	);

	// Ingore player capsule
	//rayCallback.m_collisionFilterMask &= ~btBroadphaseProxy::CharacterFilter;
	//rayCallback.m_collisionFilterMask &= CollisionGroups::HOUSE | CollisionGroups::PROJECTILES;
	rayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	rayCallback.m_collisionFilterMask =  ~(CollisionGroups::PLAYER | CollisionGroups::EDITOR_ONLY);

	Physics::s_dynamicsWorld->rayTest(
		btVector3(rayOrigin.x, rayOrigin.y, rayOrigin.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z),
		rayCallback
	);

	if (rayCallback.hasHit())
	{
		// Collision object
		btVector3 objectCOM = rayCallback.m_collisionObject->getWorldTransform().getOrigin();
		btVector3 RayCastOffsetFromCOM = objectCOM - rayCallback.m_hitPointWorld;

		// Find rigid body
		int RayCastWorldArrayIndex = (int)rayCallback.m_collisionObject->getWorldArrayIndex();

		m_hitPoint = Util::btVec3_to_glmVec3(rayCallback.m_hitPointWorld);
		m_distance = (rayCallback.m_hitPointWorld - Util::glmVec3_to_btVec3(rayOrigin)).length();
		m_surfaceNormal = Util::btVec3_to_glmVec3(rayCallback.m_hitNormalWorld);

		btRigidBody* rigidBody = (btRigidBody*)rayCallback.m_collisionObject;
		if (rigidBody)
			m_rigidBody = rigidBody;

		EntityData* entityData = (EntityData*)rigidBody->getUserPointer();
		if (entityData) {
			//m_elementIndex = entityData->vectorIndex;
			m_objectType = entityData->type;
			m_objectPtr = entityData->ptr;
		}
	}
}

RaycastResult RaycastResult::CastMouseRay(Camera* camera)
{
	glm::vec4 lRayStart_NDC(
		((float)Input::s_mouseX / (float)CoreGL::s_windowWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)(CoreGL::s_windowHeight - Input::s_mouseY) / (float)CoreGL::s_windowHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		((float)Input::s_mouseX / (float)CoreGL::s_windowWidth - 0.5f) * 2.0f,
		((float)(CoreGL::s_windowHeight - Input::s_mouseY) / (float)CoreGL::s_windowHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
	);

	glm::mat4 InverseProjectionMatrix = camera->m_inversePprojectionMatrix;

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = camera->m_inverseViewMatrix;//

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;
		
	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);

//	std::cout << "RAY: " << Util::Vec3_to_String(lRayDir_world) << "\n";

	glm::vec3 out_origin = glm::vec3(lRayStart_world);
	glm::vec3 out_direction = lRayDir_world;
	glm::vec3 out_end = out_origin + out_direction * 1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(out_origin.x, out_origin.y, out_origin.z),
		btVector3(out_end.x, out_end.y, out_end.z)
	);

	RayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
	RayCallback.m_collisionFilterMask = ~CollisionGroups::PLAYER | ~CollisionGroups::ENEMY;

	Physics::s_dynamicsWorld->rayTest(
		btVector3(out_origin.x, out_origin.y, out_origin.z),
		btVector3(out_end.x, out_end.y, out_end.z),
		RayCallback
	);

	// init
	RaycastResult raycastResult;
	//raycastResult.m_elementIndex = -1;
	raycastResult.m_distance = 0;
	raycastResult.m_hitPoint = glm::vec3(0);;
	raycastResult.m_surfaceNormal = glm::vec3(0);
	raycastResult.m_objectType = PhysicsObjectType::UNDEFINED;
	raycastResult.m_rigidBody = nullptr;
	raycastResult.m_objectPtr = nullptr;


	if (RayCallback.hasHit())
	{
		//std::cout << "HIT: ";

		// Collision object
		btVector3 objectCOM = RayCallback.m_collisionObject->getWorldTransform().getOrigin();
		btVector3 RayCastOffsetFromCOM = objectCOM - RayCallback.m_hitPointWorld;

		// Find rigid body
		int RayCastWorldArrayIndex = (int)RayCallback.m_collisionObject->getWorldArrayIndex();


		raycastResult.m_hitPoint = Util::btVec3_to_glmVec3(RayCallback.m_hitPointWorld);
		raycastResult.m_distance = (RayCallback.m_hitPointWorld - Util::glmVec3_to_btVec3(out_origin)).length();
		raycastResult.m_surfaceNormal = Util::btVec3_to_glmVec3(RayCallback.m_hitNormalWorld);

		btRigidBody* rigidBody = (btRigidBody*)RayCallback.m_collisionObject;
		if (rigidBody)
			raycastResult.m_rigidBody = rigidBody;

		EntityData* entityData = (EntityData*)rigidBody->getUserPointer();
		if (entityData) {
			//raycastResult.m_elementIndex = entityData->vectorIndex;
			raycastResult.m_objectPtr = entityData->ptr;
			raycastResult.m_objectType = entityData->type;
				
			//std::cout << Util::PhysicsObjectEnumToString(raycastResult.m_objectType) << "\n";
		}
	}
	return raycastResult;
}
