#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "model.h"
class Tumbler {
private:
    glm::vec3 angularVelocity;  // ���ٶ�
    glm::vec3 linearVelocity;   // �ٶ�
    glm::mat4 modelMatrix;      // ģ�;���
    bool isChecked{0};

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
        modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * deltaTime, angularVelocity);
        modelMatrix = glm::translate(modelMatrix, linearVelocity * deltaTime);
    }
    bool getIsChecked() const { return isChecked; }
    void setIsChecked(bool check) { isChecked = check; }
    //ʹ��model�İ�Χ�н�����ײ���
    bool checkMouseClicked(Model& model, glm::vec3 mousePosition) {
        // ������λ���Ƿ���ģ�͵İ�Χ����
        //����Χ�о���ģ�;���任���ٽ��м��
        if (mousePosition.y > 0.00494931f) {
            return 0;
        }
        glm::vec4 min = modelMatrix * glm::vec4(model.boundingBox.minX, model.boundingBox.minY, model.boundingBox.minZ, 1.0f);
        glm::vec4 max = modelMatrix * glm::vec4(model.boundingBox.maxX, model.boundingBox.maxY, model.boundingBox.maxZ, 1.0f);

        return (mousePosition.x >= min.x && mousePosition.x <= max.x &&
            mousePosition.y >= min.y && mousePosition.y <= max.y &&
            mousePosition.z >= min.z && mousePosition.z <= max.z);

    }
    
};
