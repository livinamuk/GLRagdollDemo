#include "Header.h"
#include "Core/CoreGL.h"
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Core/Entity.h"
#include "Helpers/Util.h"
#include "Helpers/FileImporter.h"
#include "Renderer/Decal.h"
#include "Renderer/Model.h"
#include "Renderer/Texture.h"
#include "Renderer/TextBlitter.h"
#include "Renderer/Renderer.h"
#include "Physics/physics.h"
#include "Physics/RaycastResult.h"


// Welcome to this example.
//
// Key files
//  - Main.cpp
//  - Renderer/Renderer.cpp
//  - Physics/Ragdoll.cpp.
//
// Email me at livinamuk@protonmail.com if ya get stuck.


void ResetScene();
void NewRagdoll();
std::vector<GameCharacter> gameCharacters; // each character has a ragdoll


/////////////////////
//   Entry Point   //

int main()
{
    CoreGL::InitGL(SCR_WIDTH, SCR_HEIGHT);
    Physics::Init();
    Renderer::Init();

    Camera camera;
    CoreGL::SetCamera(&camera);

    // Create couch (very important)
    Model couchModel = Model("Couch.obj");
    Entity couchEntity = Entity(&couchModel, Texture::GetTexIDByTexName("Couch_ALB.png"));    
    couchEntity.m_transform.position.z -= 1;
    couchEntity.m_transform.scale = glm::vec3(0.07000000029802323f);
    couchEntity.UpdateCollisionObject();

    float lastFrame = CoreGL::GetGLTime();
    ResetScene();

    ///////////////////////
    //   Main game loop  //

    while (CoreGL::IsRunning() && !Input::s_keyDown[HELL_KEY_ESCAPE])
    {
        float deltaTime = CoreGL::GetGLTime() - lastFrame;
        lastFrame = CoreGL::GetGLTime();
        camera.Update(deltaTime);

        // Physics Step
        int substeps = 2;
        btScalar timeStep = 1.0 / 60.0;
        for (auto s = 0U; s < substeps; s++)
            Physics::s_dynamicsWorld->stepSimulation(deltaTime / substeps, 1, timeStep / substeps);

        // Update OpenGL and get keyboard state
        CoreGL::OnUpdate();
        CoreGL::ProcessInput();
        Input::HandleKeypresses();

        // Hotload shader?
        if (Input::s_keyPressed[HELL_KEY_H])
            Renderer::HotLoadShaders();

        // New Ragdoll?
        if (Input::s_keyPressed[HELL_KEY_SPACE])
            NewRagdoll();

        // Reset scene?
        if (Input::s_keyPressed[HELL_KEY_R])
            ResetScene();

        // Shoot ragdoll  
        RaycastResult raycastResult;
        raycastResult.CastRay(camera.m_transform.position, camera.m_Front, 10);
        if (Input::s_leftMousePressed && raycastResult.m_objectType == PhysicsObjectType::RAGDOLL)
        {
            float FORCE_SCALING_FACTOR = 22;
            raycastResult.m_rigidBody->activate(true);
            btVector3 centerOfMass = raycastResult.m_rigidBody->getCenterOfMassPosition();
            btVector3 hitPoint = Util::glmVec3_to_btVec3(raycastResult.m_hitPoint);
            btVector3 force = Util::glmVec3_to_btVec3(camera.m_Front) * FORCE_SCALING_FACTOR;
            raycastResult.m_rigidBody->applyImpulse(force, hitPoint - centerOfMass);
        }

        // Text to blit...
        TextBlitter::BlitLine("Ray Hit: " + Util::PhysicsObjectEnumToString(raycastResult.m_objectType));
        TextBlitter::BlitLine("Space: spawn ragdoll");
        TextBlitter::BlitLine("B: toggle debug view");
        TextBlitter::BlitLine("Left click: shoot");
        TextBlitter::BlitLine("R: reset scene");
        TextBlitter::BlitLine("WSAD: walk");

        // RENDER FRAME
        Renderer::RenderFrame(gameCharacters, couchEntity, &camera);
        CoreGL::SwapBuffersAndPollEvents();
    }

    CoreGL::Terminate();
    return 0;
}

void ResetScene()
{
    while (gameCharacters.size() > 0) {
        gameCharacters[0].m_ragdoll.RemovePhysicsObjects();
        gameCharacters.erase(gameCharacters.begin());
    }
    GameCharacter gameCharacter;
    gameCharacter.m_transform.position = glm::vec3(0, 0, -0.5f);
    gameCharacter.m_skinnedModel = FileImporter::LoadSkinnedModel("NurseGuy.fbx");
    gameCharacter.m_ragdoll.BuildFromJsonFile("ragdoll.json", gameCharacter.m_transform);
    gameCharacter.m_skinningMethod = SkinningMethod::RAGDOLL;
    gameCharacters.push_back(gameCharacter);
}

void NewRagdoll()
{
    GameCharacter gameCharacter;
    gameCharacter.m_transform.position = glm::vec3(0, 5, 0);
    gameCharacter.m_transform.rotation.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HELL_PI)));
    gameCharacter.m_transform.rotation.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HELL_PI)));
    gameCharacter.m_transform.rotation.z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HELL_PI)));
    gameCharacter.m_skinnedModel = FileImporter::LoadSkinnedModel("NurseGuy.fbx");
    gameCharacter.m_ragdoll.BuildFromJsonFile("ragdoll.json", gameCharacter.m_transform);
    gameCharacter.m_skinningMethod = SkinningMethod::RAGDOLL;
    gameCharacters.push_back(gameCharacter);
}
