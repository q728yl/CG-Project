#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Tumbler {
private:
    glm::vec3 angularVelocity;  // ���ٶ�
    glm::vec3 linearVelocity;   // �ٶ�
    glm::mat4 modelMatrix;      // ģ�;���

public:
    // ���캯��
    Tumbler()
        : angularVelocity(glm::vec3(0.0f,0.0f,0.0f)), linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(glm::mat4(1.0f)) {}

    Tumbler(const glm::mat4& mMatrix)
        : angularVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(modelMatrix) {}

    // ��ȡ�����ý��ٶ�
    glm::vec3 getAngularVelocity() const { return angularVelocity; }
    void setAngularVelocity(const glm::vec3& angVel) { angularVelocity = angVel; }

    // ��ȡ���������ٶ�
    glm::vec3 getLinearVelocity() const { return linearVelocity; }
    void setLinearVelocity(const glm::vec3& linVel) { linearVelocity = linVel; }

    // ��ȡģ�;���
    glm::mat4 getModelMatrix() const { return modelMatrix; }
    void setModelMatrix(const glm::mat4& mMatrix) { modelMatrix = mMatrix; }
    // ����ģ�;���ĺ��������磬���ڵ�ǰ�ٶȺͽ��ٶȣ�
    void updateModelMatrix(float deltaTime) {
        // ���磬���ڽ��ٶȺ����ٶ�������ģ�;���
        // ����ֻ��һ���򻯵�ʾ������������Ҫ����ʵ��������е���
        modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * deltaTime, angularVelocity);
        modelMatrix = glm::translate(modelMatrix, linearVelocity * deltaTime);
    }
};
