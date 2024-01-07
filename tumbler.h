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
const float momentOfInertia = 0.1;//ת������
const float dampingFactor = 0.95f;  // ����ϵ��
const float resistanceFactor = 0.1f;  // �ֿ���������
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
    glm::vec3 angularVelocity;  // ���ٶ�
    glm::vec3 angularAcceleration;  // �Ǽ��ٶ�
    glm::vec3 linearVelocity;   // �ٶ�
    glm::mat4 modelMatrix;      // ģ�;���
    int checkedValue{0};

public:
    // ���캯��
    Tumbler()
        : angularVelocity(glm::vec3(0.0f,0.0f,0.0f)), angularAcceleration(glm::vec3(0.0f, 0.0f, 0.0f)), linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(glm::mat4(1.0f)) {}

    Tumbler(const glm::mat4& mMatrix)
        : angularVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), angularAcceleration(glm::vec3(0.0f, 0.0f, 0.0f)),linearVelocity(glm::vec3(0.0f, 0.0f, 0.0f)), modelMatrix(modelMatrix) {}

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
    int getCheckedValue() const { return checkedValue; }
    void setCheckedValue(int check) { checkedValue = check; }
    // �� modelMatrix ��ȡ�����������ת�Ƕ�
    glm::vec3 extractRotationFromModelMatrix() {
        // ��ʼ�����Ϊ0
        glm::vec3 rotationAngles(0.0f);

        // ��ȡ��ת���ֵ�������
        glm::vec3 rowX(modelMatrix[0]);
        glm::vec3 rowY(modelMatrix[1]);
        glm::vec3 rowZ(modelMatrix[2]);

        // ����Y����ת�Ƕȣ���Y�����ת��
        rotationAngles.y = glm::atan(-rowX.z, glm::sqrt(rowX.x * rowX.x + rowX.y * rowX.y));

        // ����Z����ת�Ƕȣ���Z�����ת��
        float cosY = glm::cos(rotationAngles.y);
        rotationAngles.z = glm::atan(rowY.x / cosY, rowZ.x / cosY);

        // ����X����ת�Ƕȣ���X�����ת��
        rotationAngles.x = glm::atan(rowZ.y, rowZ.z);

        // ������ת��Ϊ��
        rotationAngles = glm::degrees(rotationAngles);

        return rotationAngles;
    }
    glm::vec3 reflectVelocity(const glm::vec3& velocity, const glm::vec3& normal) {
        return velocity - 2.0f * glm::dot(velocity, normal) * normal;
    }
    //ʹ��model�İ�Χ�н�����ײ���
    //����1���������ת������2�������ƽ�ƣ���������û�е����model
    int checkMouseClicked(Model& model, glm::vec3 mousePosition) {
        // ������λ���Ƿ���ģ�͵İ�Χ����
        //����Χ�о���ģ�;���任���ٽ��м��
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
        // ���ȣ�����ģ�;���Ϊ��λ����
        modelMatrix = glm::mat4(1.0f);

        // ���ΰ���x��y��z����ת
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f)); // ��x����ת
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f)); // ��y����ת
        modelMatrix = glm::rotate(modelMatrix, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f)); // ��z����ת
    }
    void stayStable() {
        glm::vec3 angle = extractRotationFromModelMatrix();
        cout<<"��ǰ��б�ĽǶ�"<<angle.x<<" "<<angle.y<<" "<<angle.z<<endl;
        const float maxTiltAngle = 60.0f;  // ����һ��������б�Ƕȣ���λΪ��

        // ���x�����б
        if (glm::abs(angle.x) > maxTiltAngle) {
            angle.x = (angle.x > 0) ? maxTiltAngle-2 : 2-maxTiltAngle;
        }

        // ���y�����б
        if (glm::abs(angle.y) > maxTiltAngle) {
            angle.y = (angle.y > 0) ? maxTiltAngle - 2 : 2 - maxTiltAngle;
        }

        // ���z�����б
        if (glm::abs(angle.z) > maxTiltAngle) {
            angle.z = (angle.z > 0) ? maxTiltAngle - 2 : 2 - maxTiltAngle;
        }
        angularVelocity *=-1;
        // ����ģ�;���������������
        updateModelMatrixWithNewRotation(angle);
       
        cout << "���������б�Ƕ�" << angle.x << " " << angle.y << " " << angle.z << endl;
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
        // �������ĵ�������������
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

        // �������ĵ������ľ���
        float distance = glm::length(ballCenter - closestPoint);

        // ����������С�ڻ������İ뾶���������������ཻ
        if (distance <= ballRadius) {
            // ����������ĵ�
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
    //�������Ƿ���ģ���ཻ
    bool checkFireBallCollapse(glm::vec3 ballCenter, float fireballRadius, glm::vec3& newNormal) {
        cout<<"������������"<<ballCenter.x<<" "<<ballCenter.y<<" "<<ballCenter.z<<endl;
        Model model = modelInstances[0];
        glm::vec3 min = glm::vec3(model.boundingBox.minX, model.boundingBox.minY, model.boundingBox.minZ);
        glm::vec3 max = glm::vec3(model.boundingBox.maxX, model.boundingBox.maxY, model.boundingBox.maxZ);
        glm::vec4 modelCoords = glm::inverse(modelMatrix) * glm::vec4(ballCenter, 1.0f);

        glm::vec3 newballCenter = glm::vec3(modelCoords);
        cout<<"newballCenter"<<newballCenter.x<<" "<<newballCenter.y<<" "<<newballCenter.z<<endl;
        //��������Ƿ��ڰ�Χ����
        glm::vec3 collapsPoint = glm::vec3(-1, -1, -1);
        Face collisionFace = Face::NONE;
        if (checkSphereCubeIntersection(newballCenter, fireballRadius, min, max, collapsPoint, collisionFace)) {
            //������ײ����model�ռ�ķ�����
            cout<<"��ײ11111111111111111111111"<<endl;
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
            //��������Ƿ��ڰ�Χ����
            glm::vec3 collapsPoint = glm::vec3(-1, -1, -1);
            Face collisionFace = Face::NONE;
            if (checkSphereCubeIntersection(ballCenter, ballRadius, min, max, collapsPoint,collisionFace)) {
                balls[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
                balls[i].velocity *= -0.8;
                //������ײ����model�ռ�ķ�����
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
        //��ÿ������ģ�;���任
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
        // �������Ѿ���ģ�;�������ȡ�˵�ǰ����ת�Ƕȣ���������һ����Ϊ currentRotationAngles �� glm::vec3 �����С�
        glm::vec3 currentRotationAngles = extractRotationFromModelMatrix();

        // ����Ŀ����ת�Ƕȣ����������ϣ�������̵�������Y��ƽ�С�
        glm::vec3 targetRotationAngles(0.0f, currentRotationAngles.y, 0.0f);

        // ������ϣ���Ļָ�ʱ�䡣���ֵ���Ը��������Ҫ������
        float tau = 2.0f;  // ����ָ���Ŀ��״̬��ʱ��Ϊ1�롣

        // ����Ǽ��ٶ�
        glm::vec3 a = (targetRotationAngles - currentRotationAngles) / (tau * tau);
        angularAcceleration = -a;
        cout<<"�Ǽ��ٶ�"<<angularAcceleration.x<<" "<<angularAcceleration.y<<" "<<angularAcceleration.z<<endl;
        // ������ת�Ƕ�
        //currentRotationAngles += angularAcceleration * deltaTime;  // deltaTime �Ǵ��ϴθ��µ���ǰ��ʱ������
        //float tau = 1.0f;  // ����ָ���Ŀ��״̬��ʱ��Ϊ1�롣
        ////��ȡģ�Ͱ�Χ�о���ģ�ͱ任�����͵������
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
        //����֧�������������õ�Ĳ�ͬ�������������,�ø�����ȷ��tumbler�ĽǼ��ٶ�
       
      /*  glm::vec3 myGraPoint = glm::vec3(modelMatrix * glm::vec4(gravityPoint, 1.0f));
        cout<<"myGraPoint"<<myGraPoint.x<<" "<<myGraPoint.y<<" "<<myGraPoint.z<<endl;
        glm::vec3 gravityDirection = glm::normalize(gravityPoint);
        glm::vec3 torque = glm::cross(glm::vec3(0,-ballGravity,0), bottomPoint - myGraPoint);
        glm::vec3 angleAcc = torque/momentOfInertia;
        angularAcceleration = angleAcc;*/

        //return angleAcc;
       /* glm::vec3 angle = extractRotationFromModelMatrix();
        cout << "��ǰ��б�ĽǶ�" << angle.x << " " << angle.y << " " << angle.z << endl;
        angularAcceleration = glm::vec3(angle.x/10, angle.y/10,angle.z/10);*/
    }
    void updateTumbler(Model model) {

        if (glm::length(angularVelocity) == 0 && glm::length(angularAcceleration) == 0) {
            computeAngleAcc(model);
            angularVelocity += angularAcceleration * deltaTime;
            return;
        }
       // cout<<"��Ҫupdate"<<endl;
		// ���½Ƕ�
		modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * 2*deltaTime, angularVelocity);
        //resetTiltIfNeeded();
        //stayStable();
        computeAngleAcc(model);
        // ���½��ٶ�
        angularVelocity += angularAcceleration * deltaTime;
        upToFloor(model);
		// ���½Ǽ��ٶ�
        angularVelocity *= dampingFactor;
		// �������ٶ�
		//linearVelocity += glm::vec3(0.0f, -0.05f, 0.0f) * deltaTime;
		// ����λ��
		//modelMatrix = glm::translate(modelMatrix, linearVelocity * deltaTime);
	}
    float trace(const glm::mat4& matrix) {
        return matrix[0][0] + matrix[1][1] + matrix[2][2] + matrix[3][3];
    }

    float getRotationAngle(const glm::mat4& modelMatrix) {
        // ��ȡ��ת���ֵľ���
        glm::mat3 rotationMatrix = glm::mat3(modelMatrix);

        // ������ת�����ת�Ƕ�
        float angle = acos((trace(rotationMatrix) - 1.0f) / 2.0f);

        // ������ת��Ϊ��
        angle = glm::degrees(angle);

        return angle;
    }

    glm::vec3 getRotationAxis() {
        // ��ȡ��ת���ֵľ���
        glm::mat3 rotationMatrix = glm::mat3(modelMatrix);

        // ������ת��
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
            axis = glm::vec3(1.0f, 0.0f, 0.0f);  // ����תʱ������Ĭ����
        }

        return axis;
    }



// ��鲻���̵���б�ǶȲ��������
    void resetTiltIfNeeded() {
        // ��ģ�;�������ȡY�᷽��
        glm::vec3 yAxis = glm::vec3(modelMatrix[1]);
        float dx = modelMatrix[3][0];
        float dy = modelMatrix[3][1];
        float dz = modelMatrix[3][2];
        // ����Y�᷽����Y��������֮��ļнǣ����ȣ�
        float angle = glm::acos(glm::dot(yAxis, glm::vec3(0.0f, 1.0f, 0.0f)));

        // ������ת��Ϊ��
        float angleDegrees = glm::degrees(angle);

        // ����Ƕ�С��5�ȣ��������
        if (angleDegrees < 5.0f&& angleDegrees > 0.0f) {
            // ��ȡ��ǰ����ת�Ƕ�
            glm::vec3 currentRotationAngles = extractRotationFromModelMatrix();  // ʹ����֮ǰ�ṩ�ĺ���

            // ��X��Z�����ת�Ƕȹ���
            currentRotationAngles.x = 0.0f;
            currentRotationAngles.z = 0.0f;

            // ���µ���ת�Ƕ�Ӧ�õ�ģ�;�����
            modelMatrix = glm::mat4(1.0f);  // ����Ϊ��λ����
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