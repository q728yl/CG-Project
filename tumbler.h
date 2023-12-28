#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "model.h"
#include "ball.h"
extern vector<Ball> balls;
extern float centerPointy;
extern glm::vec3 centerPoint;
extern glm::vec3 gravityPoint;
extern float floorY;
const float momentOfInertia = 0.1;//转动惯量
const float dampingFactor = 0.75f;  // 阻尼系数
const float resistanceFactor = 0.1f;  // 抵抗力矩因子
extern float deltaTime;
const float ballGravity = 1.0f;
extern float ballRadius;
extern 
class Tumbler {
private:
    glm::vec3 angularVelocity;  // 角速度
    glm::vec3 angularAcceleration;  // 角加速度
    glm::vec3 linearVelocity;   // 速度
    glm::mat4 modelMatrix;      // 模型矩阵
    int checkedValue{0};

public:
    // 构造函数
    Tumbler()
        : angularVelocity(glm::vec3(0.0f,0.0f,0.0f)), angularAcceleration(glm::vec3(0.0f, 0.0f, 0.0f)), linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(glm::mat4(1.0f)) {}

    Tumbler(const glm::mat4& mMatrix)
        : angularVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), angularAcceleration(glm::vec3(0.0f, 0.0f, 0.0f)),linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(modelMatrix) {}

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
    int getCheckedValue() const { return checkedValue; }
    void setCheckedValue(int check) { checkedValue = check; }
    // 从 modelMatrix 提取三个方向的旋转角度
    glm::vec3 extractRotationFromModelMatrix() {
        // 初始化结果为0
        glm::vec3 rotationAngles(0.0f);

        // 提取旋转部分的行向量
        glm::vec3 rowX(modelMatrix[0]);
        glm::vec3 rowY(modelMatrix[1]);
        glm::vec3 rowZ(modelMatrix[2]);

        // 计算Y轴旋转角度（绕Y轴的旋转）
        rotationAngles.y = glm::atan(-rowX.z, glm::sqrt(rowX.x * rowX.x + rowX.y * rowX.y));

        // 计算Z轴旋转角度（绕Z轴的旋转）
        float cosY = glm::cos(rotationAngles.y);
        rotationAngles.z = glm::atan(rowY.x / cosY, rowZ.x / cosY);

        // 计算X轴旋转角度（绕X轴的旋转）
        rotationAngles.x = glm::atan(rowZ.y, rowZ.z);

        // 将弧度转换为度
        rotationAngles = glm::degrees(rotationAngles);

        return rotationAngles;
    }
    glm::vec3 reflectVelocity(const glm::vec3& velocity, const glm::vec3& normal) {
        return velocity - 2.0f * glm::dot(velocity, normal) * normal;
    }
    //使用model的包围盒进行碰撞检测
    //返回1是上面的旋转，返回2是下面的平移，返回零是没有点击到model
    int checkMouseClicked(Model& model, glm::vec3 mousePosition) {
        // 检查鼠标位置是否在模型的包围盒内
        //将包围盒经过模型矩阵变换后再进行检测
        glm::vec4 modelCoords = glm::inverse(modelMatrix) * glm::vec4(mousePosition, 1.0f);
        bool mingzhong = (modelCoords.x >= model.boundingBox.minX && modelCoords.x <= model.boundingBox.maxX &&
            modelCoords.y >= model.boundingBox.minY && modelCoords.y <= model.boundingBox.maxY &&
            modelCoords.z >= model.boundingBox.minZ && modelCoords.z <= model.boundingBox.maxZ);
        if (!mingzhong)
            return 0;
        else if (mingzhong && (mousePosition.y > centerPointy)) {
            return 1;
        }
        else
        {
            return 2;
        }

    }
    glm::vec3 calNormal(const std::vector<glm::vec3>& face) {
        glm::vec3 v1 = face[0];
        glm::vec3 v2 = face[1];
        glm::vec3 v3 = face[2];

        // 计算两个边向量
        glm::vec3 edge1 = v2 - v1;
        glm::vec3 edge2 = v3 - v1;

        // 计算法线（使用右手法则）
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        return normal;
    }
    float distanceToPlane(const std::vector<glm::vec3>& face, const glm::vec3& center) {
        // 计算法线（使用右手法则）
        glm::vec3 normal = calNormal(face);

        // 现在，计算球心到平面的距离
        float distanceToPlane = glm::dot(center - face[0], normal);
        return distanceToPlane;
    }
    bool checkSpherePlaneCollision(const glm::vec3& center, float radius, const std::vector<glm::vec3>& face) {
       
        // 计算法线（使用右手法则）
        float d= distanceToPlane(face, center);

        // 如果球心到平面的距离小于球的半径，则存在碰撞
        if (glm::abs(d) <= radius) {
            return true;
        }
        return false;
    }
    void updateBallToTumbler(glm::vec3 cPoint) {
        glm::vec4 GravityPoint = modelMatrix * glm::vec4(gravityPoint, 1.0f);
        glm::vec3 MyGravityPoint = glm::vec3(GravityPoint);
        glm::vec3 deltaAngleV = glm::cross(glm::vec3(0, -ballGravity, 0), cPoint - MyGravityPoint);
        angularVelocity -= deltaAngleV / momentOfInertia;
    }
    //记录了碰撞点位置
    void checkBallCollapsing(Model& model) {
        vector<glm::vec3> vers = model.boundingBox.getVertices();
        //对每个点做模型矩阵变换
        for (int i = 0; i < 8; i++) {
            glm::vec3 ver = vers[i];
            glm::vec4 newBox = modelMatrix * glm::vec4(ver, 1.0f);
            vers[i] = glm::vec3(newBox);
        }
        std::vector<std::vector<glm::vec3>> faces = {
            // 前面
            {vers[0], vers[1], vers[3], vers[2]},

            // 后面
            {vers[4], vers[5], vers[7], vers[6]},

            // 左侧
            {vers[0], vers[2], vers[6], vers[4]},

            // 右侧
            {vers[1], vers[3], vers[7], vers[5]},

            // 顶部
            {vers[2], vers[3], vers[7], vers[6]},

            // 底部
            {vers[0], vers[1], vers[5], vers[4]}
        };
        for (int i = 0; i < balls.size(); i++) {
           //如果ball跟包围盒的八个顶点构成的立方体有交点，则会发生碰撞，需要速度反向，并记录交点坐标，根据小球撞击前的速度决定tumbler的状态变化
            glm::vec3 ballCenter = balls[i].position;
            for (const auto& face : faces) {
                if (checkSpherePlaneCollision(balls[i].position, ballRadius, face)) {
                    // 碰撞点是球的中心减去与平面的距离乘以法线
                    glm::vec3 collisionPoint = balls[i].position - ballRadius * calNormal(face);
                    //更新不倒翁的角速度
                    updateBallToTumbler(collisionPoint);
                    // 反射速度
                    balls[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
                    balls[i].velocity *= -0.8;
                }
            }

        }

	}
    void upToFloor(Model& model) {
        vector<glm::vec3> vers =model.boundingBox.getVertices();
        float miny = vers[0].y;
        //对每个点做模型矩阵变换
        for (int i = 0; i < 8;i++) {
            glm::vec3 ver = vers[i];
            glm::vec4 newBox = modelMatrix * glm::vec4(ver, 1.0f);
            vers[i] = glm::vec3(newBox.x, newBox.y, newBox.z);
            if (newBox.y < miny)
                miny = newBox.y;
        }
       if (miny < floorY) {
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, floorY - miny, 0.0f));
        }
       if (miny > floorY) {
           modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, floorY - miny, 0.0f));
       }

    }
   /* float computeAngle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
        glm::vec3 v1 = p1 - p0;
        glm::vec3 v2 = p2 - p0;

        float dotProduct = glm::dot(v1, v2);
        float lengthProduct = glm::length(v1) * glm::length(v2);

        return glm::acos(dotProduct / lengthProduct);
    }*/
  
    void computeAngleAcc(Model model) {

        //获取模型包围盒经过模型变换后的最低点的坐标
        vector<glm::vec3> vers = model.boundingBox.getVertices();
        glm::vec3 bottomPoint = vers[0];
        float miny = vers[0].y;
        for (int i = 0; i < 8; i++) {
            glm::vec3 ver = vers[i];
            glm::vec4 newBox = modelMatrix * glm::vec4(ver, 1.0f);
            if (newBox.y < miny) {
                bottomPoint = glm::vec3(newBox.x,newBox.y,newBox.z);
                miny = newBox.y;
            }
        }
        cout << "bottomPoint" << bottomPoint.x << " " << bottomPoint.y << " " << bottomPoint.z << endl; 
        //根据支持力与重力作用点的不同计算出反抗力矩,用该力矩确定tumbler的角加速度
       
      /*  glm::vec3 myGraPoint = glm::vec3(modelMatrix * glm::vec4(gravityPoint, 1.0f));
        cout<<"myGraPoint"<<myGraPoint.x<<" "<<myGraPoint.y<<" "<<myGraPoint.z<<endl;
        glm::vec3 gravityDirection = glm::normalize(gravityPoint);
        glm::vec3 torque = glm::cross(glm::vec3(0,-ballGravity,0), bottomPoint - myGraPoint);
        glm::vec3 angleAcc = torque/momentOfInertia;
        angularAcceleration = angleAcc;*/

        //return angleAcc;
        glm::vec3 angle = extractRotationFromModelMatrix();
        cout << "当前倾斜的角度" << angle.x << " " << angle.y << " " << angle.z << endl;
        angularAcceleration = glm::vec3(angle.x/100, angle.y/100,angle.z/100);
    }
    void updateTumbler(Model model) {

        if (glm::length(angularVelocity) == 0 && glm::length(angularAcceleration) == 0) {
            computeAngleAcc(model);
            angularVelocity += angularAcceleration * deltaTime;
            return;
        }
        cout<<"需要update"<<endl;
		// 更新角度
		modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * 2*deltaTime, angularVelocity);
        computeAngleAcc(model);
        // 更新角速度
        angularVelocity += angularAcceleration * deltaTime;
        upToFloor(model);
		// 更新角加速度
        angularVelocity *= dampingFactor;
		// 更新线速度
		//linearVelocity += glm::vec3(0.0f, -0.05f, 0.0f) * deltaTime;
		// 更新位置
		//modelMatrix = glm::translate(modelMatrix, linearVelocity * deltaTime);
	}
    float trace(const glm::mat4& matrix) {
        return matrix[0][0] + matrix[1][1] + matrix[2][2] + matrix[3][3];
    }

    float getRotationAngle(const glm::mat4& modelMatrix) {
        // 提取旋转部分的矩阵
        glm::mat3 rotationMatrix = glm::mat3(modelMatrix);

        // 计算旋转轴和旋转角度
        float angle = acos((trace(rotationMatrix) - 1.0f) / 2.0f);

        // 将弧度转换为度
        angle = glm::degrees(angle);

        return angle;
    }

    glm::vec3 getRotationAxis() {
        // 提取旋转部分的矩阵
        glm::mat3 rotationMatrix = glm::mat3(modelMatrix);

        // 计算旋转轴
        float angle = getRotationAngle(modelMatrix);
        glm::vec3 axis;

        if (angle > 0.0f) {
            axis = glm::normalize(glm::vec3(
                rotationMatrix[2][1] - rotationMatrix[1][2],
                rotationMatrix[0][2] - rotationMatrix[2][0],
                rotationMatrix[1][0] - rotationMatrix[0][1]
            ) / (2.0f * sin(glm::radians(angle))));
        }
        else {
            axis = glm::vec3(1.0f, 0.0f, 0.0f);  // 无旋转时，返回默认轴
        }

        return axis;
    }


};
