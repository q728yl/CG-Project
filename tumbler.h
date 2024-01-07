#pragma once
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
const float dampingFactor = 0.95f;  // 阻尼系数
const float resistanceFactor = 0.1f;  // 抵抗力矩因子
extern float deltaTime;
const float ballGravity = 1.0f;
extern float ballRadius;
extern std::vector<Model> modelInstances;
enum class Face {
    NONE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    FRONT,
    BACK
};
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
    void updateModelMatrixWithNewRotation(const glm::vec3& angles) {
        // 首先，重置模型矩阵为单位矩阵
        modelMatrix = glm::mat4(1.0f);

        // 依次按照x、y、z轴旋转
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f)); // 绕x轴旋转
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f)); // 绕y轴旋转
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f)); // 绕z轴旋转
    }
    void stayStable() {
        glm::vec3 angle = extractRotationFromModelMatrix();
        cout<<"当前倾斜的角度"<<angle.x<<" "<<angle.y<<" "<<angle.z<<endl;
        const float maxTiltAngle = 60.0f;  // 设置一个最大的倾斜角度，单位为度

        // 检查x轴的倾斜
        if (glm::abs(angle.x) > maxTiltAngle) {
            angle.x = (angle.x > 0) ? maxTiltAngle-2 : 2-maxTiltAngle;
        }

        // 检查y轴的倾斜
        if (glm::abs(angle.y) > maxTiltAngle) {
            angle.y = (angle.y > 0) ? maxTiltAngle - 2 : 2 - maxTiltAngle;
        }

        // 检查z轴的倾斜
        if (glm::abs(angle.z) > maxTiltAngle) {
            angle.z = (angle.z > 0) ? maxTiltAngle - 2 : 2 - maxTiltAngle;
        }
        angularVelocity *=-1;
        // 更新模型矩阵或其他相关数据
        updateModelMatrixWithNewRotation(angle);
       
        cout << "修正后的倾斜角度" << angle.x << " " << angle.y << " " << angle.z << endl;
    }
    void updateBallToTumbler(glm::vec3 cPoint) {
        glm::vec4 GravityPoint = modelMatrix * glm::vec4(gravityPoint, 1.0f);
        glm::vec3 MyGravityPoint = glm::vec3(GravityPoint);
        glm::vec4 ccPoint = modelMatrix * glm::vec4(cPoint, 1.0f);
        glm::vec3 MyCPoint = glm::vec3(ccPoint);
        glm::vec3 deltaAngleV = glm::cross(glm::vec3(0, -ballGravity, 0), MyCPoint - MyGravityPoint);
        angularVelocity -= glm::vec3(deltaAngleV.x *1.2, deltaAngleV.y * 1.2, deltaAngleV.z * 1.2);
    }
    bool checkSphereCubeIntersection(const glm::vec3& ballCenter, float ballRadius,
        const glm::vec3& minPoint, const glm::vec3& maxPoint,
        glm::vec3& intersectionPoint, Face& collisionFace) {
        // 计算球心到正方体的最近点
        glm::vec3 closestPoint(0.0f);
        float minDistance = std::numeric_limits<float>::max();

        if (ballCenter.x < minPoint.x) {
            closestPoint.x = minPoint.x;
            minDistance = std::min(minDistance, minPoint.x - ballCenter.x);
            collisionFace = Face::LEFT;
        }
        else if (ballCenter.x > maxPoint.x) {
            closestPoint.x = maxPoint.x;
            minDistance = std::min(minDistance, ballCenter.x - maxPoint.x);
            collisionFace = Face::RIGHT;
        }
        else {
            closestPoint.x = ballCenter.x;
        }

        if (ballCenter.y < minPoint.y) {
            closestPoint.y = minPoint.y;
            minDistance = std::min(minDistance, minPoint.y - ballCenter.y);
            collisionFace = Face::BOTTOM;
        }
        else if (ballCenter.y > maxPoint.y) {
            closestPoint.y = maxPoint.y;
            minDistance = std::min(minDistance, ballCenter.y - maxPoint.y);
            collisionFace = Face::TOP;
        }
        else {
            closestPoint.y = ballCenter.y;
        }

        if (ballCenter.z < minPoint.z) {
            closestPoint.z = minPoint.z;
            minDistance = std::min(minDistance, minPoint.z - ballCenter.z);
            collisionFace = Face::FRONT;
        }
        else if (ballCenter.z > maxPoint.z) {
            closestPoint.z = maxPoint.z;
            minDistance = std::min(minDistance, ballCenter.z - maxPoint.z);
            collisionFace = Face::BACK;
        }
        else {
            closestPoint.z = ballCenter.z;
        }

        // 计算球心到最近点的距离
        float distance = glm::length(ballCenter - closestPoint);

        // 如果这个距离小于或等于球的半径，则球与正方体相交
        if (distance <= ballRadius) {
            // 交点是最近的点
            intersectionPoint = closestPoint;
            return true;
        }

        collisionFace = Face::NONE;
        return false;
    }
    //
    glm::vec3 calNoramlOfTri(vector<glm::vec3>points) {
        glm::vec3 v1 = points[1] - points[0];
		glm::vec3 v2 = points[2] - points[0];
		glm::vec3 normal = glm::cross(v1, v2);
		return normal;
    }
    //检测火球是否与模型相交
    bool checkFireBallCollapse(glm::vec3 ballCenter, float fireballRadius, glm::vec3& newNormal) {
        cout<<"火球中心坐标"<<ballCenter.x<<" "<<ballCenter.y<<" "<<ballCenter.z<<endl;
        Model model = modelInstances[0];
        glm::vec3 min = glm::vec3(model.boundingBox.minX, model.boundingBox.minY, model.boundingBox.minZ);
        glm::vec3 max = glm::vec3(model.boundingBox.maxX, model.boundingBox.maxY, model.boundingBox.maxZ);
        glm::vec4 modelCoords = glm::inverse(modelMatrix) * glm::vec4(ballCenter, 1.0f);

        glm::vec3 newballCenter = glm::vec3(modelCoords);
        cout<<"newballCenter"<<newballCenter.x<<" "<<newballCenter.y<<" "<<newballCenter.z<<endl;
        //检测球心是否在包围盒内
        glm::vec3 collapsPoint = glm::vec3(-1, -1, -1);
        Face collisionFace = Face::NONE;
        if (checkSphereCubeIntersection(newballCenter, fireballRadius, min, max, collapsPoint, collisionFace)) {
            //计算碰撞点在model空间的法向量
            cout<<"碰撞11111111111111111111111"<<endl;
            glm::vec3 normal = glm::vec3(0.0f);

            if (collisionFace == Face::LEFT) {
                normal = glm::vec3(-1.0, 0, 0);
            }
            else if (collisionFace == Face::RIGHT) {
                normal = glm::vec3(1.0, 0, 0);
            }
            else if (collisionFace == Face::TOP) {
                normal = glm::vec3(0, 1.0, 0);
            }
            else if (collisionFace == Face::BOTTOM) {
                normal = glm::vec3(0, -1.0, 0);
            }
            else if (collisionFace == Face::FRONT) {
                normal = glm::vec3(0, 0, -1.0);
            }
            else if (collisionFace == Face::BACK) {
                normal = glm::vec3(0, 0, 1.0);
            }
            newNormal = glm::vec3(modelMatrix * glm::vec4(normal, 1.0));
            //cout<<"newNormal2222222222222222"<<newNormal.x<<" "<<newNormal.y<<" "<<newNormal.z<<endl;
            updateBallToTumbler(collapsPoint);
            return true;
        }
        return false;
    }
 
    void checkBallCollapsing(Model& model) {
        glm::vec3 min = glm::vec3(model.boundingBox.minX, model.boundingBox.minY, model.boundingBox.minZ);
        glm::vec3 max = glm::vec3(model.boundingBox.maxX, model.boundingBox.maxY, model.boundingBox.maxZ);
      
        for (int i = 0; i < balls.size(); i++) {
            glm::vec3 ballCenter = balls[i].position;
            glm::vec4 modelCoords = glm::inverse(modelMatrix) * glm::vec4(ballCenter, 1.0f);
            ballCenter = glm::vec3(modelCoords);
            //检测球心是否在包围盒内
            glm::vec3 collapsPoint = glm::vec3(-1, -1, -1);
            Face collisionFace = Face::NONE;
            if (checkSphereCubeIntersection(ballCenter, ballRadius, min, max, collapsPoint,collisionFace)) {
                balls[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
                balls[i].velocity *= -0.8;
                //计算碰撞点在model空间的法向量
                glm::vec3 normal = glm::vec3(0.0f);

                if (collisionFace == Face::LEFT) {
                    normal = glm::vec3(-1.0, 0, 0);
				}
                else if (collisionFace == Face::RIGHT) {
                    normal = glm::vec3(1.0, 0, 0);
				}
                else if (collisionFace == Face::TOP) {
                    normal = glm::vec3(0, 1.0, 0);
				}
                else if (collisionFace == Face::BOTTOM) {
                    normal = glm::vec3(0, -1.0, 0);
				}
                else if (collisionFace == Face::FRONT) {
                    normal = glm::vec3(0, 0, -1.0);
				}
                else if (collisionFace == Face::BACK) {
                    normal =glm::vec3(0,0,1.0) ;
				}
                glm::vec3 newNormal = modelMatrix * glm::vec4(normal, 1.0);
                balls[i].position+=glm::normalize(newNormal)*ballRadius*0.5f;
				updateBallToTumbler(collapsPoint);
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
        // 假设你已经从模型矩阵中提取了当前的旋转角度，并保存在一个名为 currentRotationAngles 的 glm::vec3 变量中。
        glm::vec3 currentRotationAngles = extractRotationFromModelMatrix();

        // 定义目标旋转角度，这里假设你希望不倒翁的竖轴与Y轴平行。
        glm::vec3 targetRotationAngles(0.0f, currentRotationAngles.y, 0.0f);

        // 定义你希望的恢复时间。这个值可以根据你的需要调整。
        float tau = 2.0f;  // 假设恢复到目标状态的时间为1秒。

        // 计算角加速度
        glm::vec3 a = (targetRotationAngles - currentRotationAngles) / (tau * tau);
        angularAcceleration = -a;
        cout<<"角加速度"<<angularAcceleration.x<<" "<<angularAcceleration.y<<" "<<angularAcceleration.z<<endl;
        // 更新旋转角度
        //currentRotationAngles += angularAcceleration * deltaTime;  // deltaTime 是从上次更新到当前的时间间隔。
        //float tau = 1.0f;  // 假设恢复到目标状态的时间为1秒。
        ////获取模型包围盒经过模型变换后的最低点的坐标
        //vector<glm::vec3> vers = model.boundingBox.getVertices();
        //glm::vec3 bottomPoint = vers[0];
        //float miny = vers[0].y;
        //for (int i = 0; i < 8; i++) {
        //    glm::vec3 ver = vers[i];
        //    glm::vec4 newBox = modelMatrix * glm::vec4(ver, 1.0f);
        //    if (newBox.y < miny) {
        //        bottomPoint = glm::vec3(newBox.x,newBox.y,newBox.z);
        //        miny = newBox.y;
        //    }
        //}
        //cout << "bottomPoint" << bottomPoint.x << " " << bottomPoint.y << " " << bottomPoint.z << endl; 
        //根据支持力与重力作用点的不同计算出反抗力矩,用该力矩确定tumbler的角加速度
       
      /*  glm::vec3 myGraPoint = glm::vec3(modelMatrix * glm::vec4(gravityPoint, 1.0f));
        cout<<"myGraPoint"<<myGraPoint.x<<" "<<myGraPoint.y<<" "<<myGraPoint.z<<endl;
        glm::vec3 gravityDirection = glm::normalize(gravityPoint);
        glm::vec3 torque = glm::cross(glm::vec3(0,-ballGravity,0), bottomPoint - myGraPoint);
        glm::vec3 angleAcc = torque/momentOfInertia;
        angularAcceleration = angleAcc;*/

        //return angleAcc;
       /* glm::vec3 angle = extractRotationFromModelMatrix();
        cout << "当前倾斜的角度" << angle.x << " " << angle.y << " " << angle.z << endl;
        angularAcceleration = glm::vec3(angle.x/10, angle.y/10,angle.z/10);*/
    }
    void updateTumbler(Model model) {

        if (glm::length(angularVelocity) == 0 && glm::length(angularAcceleration) == 0) {
            computeAngleAcc(model);
            angularVelocity += angularAcceleration * deltaTime;
            return;
        }
       // cout<<"需要update"<<endl;
		// 更新角度
		modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * 2*deltaTime, angularVelocity);
        //resetTiltIfNeeded();
        //stayStable();
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



// 检查不倒翁的倾斜角度并将其归零
    void resetTiltIfNeeded() {
        // 从模型矩阵中提取Y轴方向
        glm::vec3 yAxis = glm::vec3(modelMatrix[1]);
        float dx = modelMatrix[3][0];
        float dy = modelMatrix[3][1];
        float dz = modelMatrix[3][2];
        // 计算Y轴方向与Y轴正方向之间的夹角（弧度）
        float angle = glm::acos(glm::dot(yAxis, glm::vec3(0.0f, 1.0f, 0.0f)));

        // 将弧度转换为度
        float angleDegrees = glm::degrees(angle);

        // 如果角度小于5度，则将其归零
        if (angleDegrees < 5.0f&& angleDegrees > 0.0f) {
            // 获取当前的旋转角度
            glm::vec3 currentRotationAngles = extractRotationFromModelMatrix();  // 使用你之前提供的函数

            // 将X和Z轴的旋转角度归零
            currentRotationAngles.x = 0.0f;
            currentRotationAngles.z = 0.0f;

            // 将新的旋转角度应用到模型矩阵上
            modelMatrix = glm::mat4(1.0f);  // 重置为单位矩阵
            modelMatrix = glm::translate(modelMatrix, glm::vec3(dx, dy, dz));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));
           angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
            //modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f));
           /* angularVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
            angularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);*/
        }
    }
};