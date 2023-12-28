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
    glm::vec3 calNormal(const std::vector<glm::vec3>& face) {
        glm::vec3 v1 = face[0];
        glm::vec3 v2 = face[1];
        glm::vec3 v3 = face[2];

        // ��������������
        glm::vec3 edge1 = v2 - v1;
        glm::vec3 edge2 = v3 - v1;

        // ���㷨�ߣ�ʹ�����ַ���
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        return normal;
    }
    float distanceToPlane(const std::vector<glm::vec3>& face, const glm::vec3& center) {
        // ���㷨�ߣ�ʹ�����ַ���
        glm::vec3 normal = calNormal(face);

        // ���ڣ��������ĵ�ƽ��ľ���
        float distanceToPlane = glm::dot(center - face[0], normal);
        return distanceToPlane;
    }
    bool checkSpherePlaneCollision(const glm::vec3& center, float radius, const std::vector<glm::vec3>& face) {
       
        // ���㷨�ߣ�ʹ�����ַ���
        float d= distanceToPlane(face, center);

        // ������ĵ�ƽ��ľ���С����İ뾶���������ײ
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
    //��¼����ײ��λ��
    void checkBallCollapsing(Model& model) {
        vector<glm::vec3> vers = model.boundingBox.getVertices();
        //��ÿ������ģ�;���任
        for (int i = 0; i < 8; i++) {
            glm::vec3 ver = vers[i];
            glm::vec4 newBox = modelMatrix * glm::vec4(ver, 1.0f);
            vers[i] = glm::vec3(newBox);
        }
        std::vector<std::vector<glm::vec3>> faces = {
            // ǰ��
            {vers[0], vers[1], vers[3], vers[2]},

            // ����
            {vers[4], vers[5], vers[7], vers[6]},

            // ���
            {vers[0], vers[2], vers[6], vers[4]},

            // �Ҳ�
            {vers[1], vers[3], vers[7], vers[5]},

            // ����
            {vers[2], vers[3], vers[7], vers[6]},

            // �ײ�
            {vers[0], vers[1], vers[5], vers[4]}
        };
        for (int i = 0; i < balls.size(); i++) {
           //���ball����Χ�еİ˸����㹹�ɵ��������н��㣬��ᷢ����ײ����Ҫ�ٶȷ��򣬲���¼�������꣬����С��ײ��ǰ���ٶȾ���tumbler��״̬�仯
            glm::vec3 ballCenter = balls[i].position;
            for (const auto& face : faces) {
                if (checkSpherePlaneCollision(balls[i].position, ballRadius, face)) {
                    // ��ײ����������ļ�ȥ��ƽ��ľ�����Է���
                    glm::vec3 collisionPoint = balls[i].position - ballRadius * calNormal(face);
                    //���²����̵Ľ��ٶ�
                    updateBallToTumbler(collisionPoint);
                    // �����ٶ�
                    balls[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
                    balls[i].velocity *= -0.8;
                }
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
        angularAcceleration = glm::vec3(angle.x/100, angle.y/100,angle.z/100);
    }
    void updateTumbler(Model model) {

        if (glm::length(angularVelocity) == 0 && glm::length(angularAcceleration) == 0) {
            computeAngleAcc(model);
            angularVelocity += angularAcceleration * deltaTime;
            return;
        }
        cout<<"��Ҫupdate"<<endl;
		// ���½Ƕ�
		modelMatrix = glm::rotate(modelMatrix, glm::length(angularVelocity) * 2*deltaTime, angularVelocity);
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
