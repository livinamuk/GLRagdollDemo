#include "GameCharacter.h"
#include "Renderer/Texture.h"

GameCharacter::GameCharacter()
{

}

GameCharacter::~GameCharacter()
{
}

void GameCharacter::RenderSkinnedModel(Shader* shader)
{   
    // SKIN THE MODEL

    m_animatedTransforms.resize(m_skinnedModel->m_BoneInfo.size());
    m_animatedDebugTransforms_Animated.resize(m_skinnedModel->m_BoneInfo.size());
    
    glm::mat4 modelMatrix;

    if (m_skinningMethod == SkinningMethod::BINDPOSE)
    {
        modelMatrix = m_transform.to_mat4();
        m_skinnedModel->UpdateBoneTransform(m_animatedTransforms, m_animatedDebugTransforms_Animated, NULL);
    }
    else if (m_skinningMethod == SkinningMethod::RAGDOLL) {
        modelMatrix = glm::mat4(1);
        m_skinnedModel->UpdateBoneTransform(m_animatedTransforms, m_animatedDebugTransforms_Animated, &m_ragdoll);
        //return;
    }

   // return;

    shader->setInt("hasAnimation", true);

    for (unsigned int i = 0; i < m_animatedTransforms.size(); i++)
        shader->setMat4("skinningMats[" + std::to_string(i) + "]", modelMatrix * m_animatedTransforms[i]);

    glBindVertexArray(m_skinnedModel->m_VAO);
    glActiveTexture(GL_TEXTURE0);

    for (int i = 0; i < m_skinnedModel->m_meshEntries.size(); i++) {
        if (m_skinnedModel->m_meshEntries[i].Name == "Body_LOD0")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Zombie_Face_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Tshirt_LOD0")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Zombie_Shirt_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Hair_LOD0")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Zombie_Hair_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "BTM_LOD0")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Zombie_Jeans_ALB.png"));

        if (m_skinnedModel->m_meshEntries[i].Name == "Ch16_Body1")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Nurse_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Ch16_Cap")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Nurse2_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Ch16_Eyelashes")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Nurse_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Ch16_Mask")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Nurse2_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Ch16_Pants")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Nurse_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Ch16_Shirt")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Nurse_ALB.png"));
        if (m_skinnedModel->m_meshEntries[i].Name == "Ch16_Shoes")
            glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("Nurse2_ALB.png"));

        glDrawElementsBaseVertex(GL_TRIANGLES, m_skinnedModel->m_meshEntries[i].NumIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * m_skinnedModel->m_meshEntries[i].BaseIndex), m_skinnedModel->m_meshEntries[i].BaseVertex);
    }

    shader->setInt("hasAnimation", false);
}
