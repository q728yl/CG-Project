#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Tumbler {
private:
    glm::vec3 angularVelocity;  // 角速度
    glm::vec3 linearVelocity;   // 速度
    glm::mat4 modelMatrix;      // 模型矩阵

public:
    // 构造函数
    Tumbler()
        : angularVelocity(glm::vec3(0.0f,0.0f,0.0f)), linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(glm::mat4(1.0f)) {}

    Tumbler(const glm::mat4& mMatrix)
        : angularVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(modelMatrix) {}

    // 获取和设置角速度
    glm::vec3 getAngularVelocity() const { return angularVelocity; }
    void setAngularVelocity(const glm::vec3& angVel) { angularVelocity = angVel; }

    // 获取和设置线速度
    glm::vec3 getLinearVelocity() const { return linearVelocity; }
    void setLinearVelocity(const glm::vec3& linVel) { linearVelocity = linVel; }

    // 获取模型矩阵
    glm::mat4 getModelMatrix() const { return modelMatrix; }
    void setModelMatrix(const glm::mat4& mMatrix) { modelMatrix = mMatrix; }
    // 更新模型矩阵的函数（例如，基于当前速度和角速度）
    void updateModelMatrix(float deltaTime) {
        // 例如，基于角速度和线速度来更新模型矩阵
        // 这里只是一个简化的示例，您可能需要根据实际需求进行调整
        modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * deltaTime, angularVelocity);
        modelMatrix = glm::translate(modelMatrix, linearVelocity * deltaTime);
    }
};
