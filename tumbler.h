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
const float dampingFactor = 0.75f;  // ����ϵ��
const float resistanceFactor = 0.1f;  // �ֿ���������
extern float deltaTime;
const float ballGravity = 1.0f;
extern float ballRadius;
extern 
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
        angularVelocity -= glm::vec3(deltaAngleV.x / 10, deltaAngleV.y / 10, deltaAngleV.z / 10);
    }
    bool checkSphereCubeIntersection(const glm::vec3& ballCenter, float ballRadius, const glm::vec3& minPoint, const glm::vec3& maxPoint, glm::vec3& intersectionPoint) {
        // �������ĵ�������������
        glm::vec3 closestPoint(0.0f);

        if (ballCenter.x < minPoint.x) closestPoint.x = minPoint.x;
        else if (ballCenter.x > maxPoint.x) closestPoint.x = maxPoint.x;
        else closestPoint.x = ballCenter.x;

        if (ballCenter.y < minPoint.y) closestPoint.y = minPoint.y;
        else if (ballCenter.y > maxPoint.y) closestPoint.y = maxPoint.y;
        else closestPoint.y = ballCenter.y;

        if (ballCenter.z < minPoint.z) closestPoint.z = minPoint.z;
        else if (ballCenter.z > maxPoint.z) closestPoint.z = maxPoint.z;
        else closestPoint.z = ballCenter.z;

        // �������ĵ������ľ���
        float distance = glm::length(ballCenter - closestPoint);

        // ����������С�ڻ������İ뾶���������������ཻ
        if (distance <= ballRadius) {
            // ����������ĵ�
            intersectionPoint = closestPoint;
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
            if (checkSphereCubeIntersection(ballCenter, ballRadius, min, max, collapsPoint)) {
                balls[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
                balls[i].velocity *= -0.8;
				balls[i].position += balls[i].velocity * deltaTime;
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

        //��ȡģ�Ͱ�Χ�о���ģ�ͱ任�����͵������
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
        //����֧�������������õ�Ĳ�ͬ�������������,�ø�����ȷ��tumbler�ĽǼ��ٶ�
       
      /*  glm::vec3 myGraPoint = glm::vec3(modelMatrix * glm::vec4(gravityPoint, 1.0f));
        cout<<"myGraPoint"<<myGraPoint.x<<" "<<myGraPoint.y<<" "<<myGraPoint.z<<endl;
        glm::vec3 gravityDirection = glm::normalize(gravityPoint);
        glm::vec3 torque = glm::cross(glm::vec3(0,-ballGravity,0), bottomPoint - myGraPoint);
        glm::vec3 angleAcc = torque/momentOfInertia;
        angularAcceleration = angleAcc;*/

        //return angleAcc;
        glm::vec3 angle = extractRotationFromModelMatrix();
        cout << "��ǰ��б�ĽǶ�" << angle.x << " " << angle.y << " " << angle.z << endl;
        angularAcceleration = glm::vec3(angle.x/10, angle.y/10,angle.z/10);
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


};
