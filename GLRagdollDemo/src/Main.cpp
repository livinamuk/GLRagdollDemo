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
SkinnedModel* nurseModel;
std::vector<GameCharacter> gameCharacters; // each character has a ragdoll

/////////////////////
//   Entry Point   //

int main()
{
    CoreGL::InitGL(SCR_WIDTH, SCR_HEIGHT);
    Physics::Init();
    Renderer::Init();
    
    Input::s_showCursor = true;

    nurseModel = FileImporter::LoadSkinnedModel("NurseGuy.fbx");

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


        Physics::DragRagdoll(&camera);
        

      ///  if (Input::s_keyPressed[HELL_KEY_P])
            Physics::Update(deltaTime);

        // Update OpenGL and get keyboard state
        CoreGL::OnUpdate();
        CoreGL::ProcessInput();
        Input::HandleKeypresses();

        static int damp = gameCharacters[0].m_ragdoll.m_jointComponents[0].drive_angularDamping;// 400;
        static int stiff = gameCharacters[0].m_ragdoll.m_jointComponents[0].drive_angularStiffness;// 200;
        static int iterations = 100;

static float time = 0;
time += 0.1f;

        for (int j = 0; j  < gameCharacters[0].m_ragdoll.m_jointComponents.size(); j++)
        {
            auto joint = gameCharacters[0].m_ragdoll.m_jointComponents[j];

            if (j == 7)
            {
                
                float value =  (std::sin(time)) * HELL_PI ;   
             //   joint.constraint->setServoTarget(3, value);
           //     joint.constraint->setServoTarget(4, value);
           //     joint.constraint->setServoTarget(5, value);
           //     TextBlitter::BlitLine("value: " + std::to_string(value));


                float vel = 10;
                /*
joint.constraint->setMaxMotorForce(3, 10000);
joint.constraint->setTargetVelocity(3, vel);
joint.constraint->setMaxMotorForce(4, 10000);
joint.constraint->setTargetVelocity(4, vel);
joint.constraint->setMaxMotorForce(5, 10000);
joint.constraint->setTargetVelocity(5, vel);*/
            }

         //   std::cout << j << " " << joint.name << "\n";

            for (int i = 0; i < 6; i++) {
                joint.constraint->setDamping(i, damp);
                joint.constraint->setStiffness(i, stiff);
                joint.constraint->setOverrideNumSolverIterations(iterations);

            }
        }

        if (Input::s_keyDown[HELL_KEY_P])
            stiff++;
        if (Input::s_keyDown[HELL_KEY_O])
            stiff--;
        if (Input::s_keyDown[HELL_KEY_L])
            damp++;
        if (Input::s_keyDown[HELL_KEY_K])
            damp--;

        if (Input::s_keyPressed[HELL_KEY_I])
            Physics::s_subSteps++;
        if (Input::s_keyPressed[HELL_KEY_U])
            Physics::s_subSteps--;
        if (Input::s_keyPressed[HELL_KEY_J])
            iterations *= 10;
        if (Input::s_keyPressed[HELL_KEY_H])
            iterations *= 0.1;


        // Hotload shader?
        if (Input::s_keyPressed[HELL_KEY_H])
            Renderer::HotLoadShaders();

        // New Ragdoll?
        if (Input::s_keyPressed[HELL_KEY_SPACE])
            NewRagdoll();

        // Reset scene?
        if (Input::s_keyPressed[HELL_KEY_R])
            ResetScene();      

        TextBlitter::BlitLine("Ragdoll GL Demo");
        TextBlitter::BlitLine("Space: Spawn ragdoll");
        TextBlitter::BlitLine("Mouse: Click to drag");
        TextBlitter::BlitLine("R: Reset scene");
        TextBlitter::BlitLine("B: toggle debug");
        TextBlitter::BlitLine(" ");
        TextBlitter::BlitLine("Stiffness:  " + std::to_string(stiff));
        TextBlitter::BlitLine("Damping:    " + std::to_string(damp));
        TextBlitter::BlitLine("Substeps:   " + std::to_string(Physics::s_subSteps));
        TextBlitter::BlitLine("Iterations: " + std::to_string(iterations));

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
    gameCharacter.m_skinnedModel = nurseModel;
    gameCharacter.m_ragdoll.BuildFromJsonFile("ragdoll.json", gameCharacter.m_transform);
    gameCharacter.m_skinningMethod = SkinningMethod::RAGDOLL;
    gameCharacters.push_back(gameCharacter);
}
