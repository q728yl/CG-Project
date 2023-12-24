#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "model.h"
class Tumbler {
private:
    glm::vec3 angularVelocity;  // 角速度
    glm::vec3 linearVelocity;   // 速度
    glm::mat4 modelMatrix;      // 模型矩阵
    bool isChecked{0};

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
        modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * deltaTime, angularVelocity);
        modelMatrix = glm::translate(modelMatrix, linearVelocity * deltaTime);
    }
    bool getIsChecked() const { return isChecked; }
    void setIsChecked(bool check) { isChecked = check; }
    //使用model的包围盒进行碰撞检测
    bool checkMouseClicked(Model& model, glm::vec3 mousePosition) {
        // 检查鼠标位置是否在模型的包围盒内
        //将包围盒经过模型矩阵变换后再进行检测
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
