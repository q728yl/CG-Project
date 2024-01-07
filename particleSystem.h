#pragma once

#include "tumbler.h"
#include "ball.h"
#include <glad/glad.h> // holds all OpenGL type declarations
#include <string>
#include <vector>
#include <glm/gtc/random.hpp>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
float maxZ = -2.0f;
extern float ballRadius;
extern vector<Tumbler> tumblers;
extern int ashTexture;
const int numPoints = 500;
float avgBallSize = 15.0f;
extern float fireBallRadius;
extern vector<Ball> balls;

struct Particle {
    glm::vec3 position;     // ����λ��
    glm::vec3 velocity;     // �����ٶ�
    glm::vec4 color;        // ������ɫ (ʹ��vec4��ʾRGBA)
    float life;             // ������������
    float size;             // ���Ӵ�С

    Particle()
        : position(-0.0f), velocity(0.0f), color(1.0f), life(1.0f), size(1.0f) {}
};
std::vector<glm::vec3> generateRandomPointsInSphere(int numPoints, float radius) {
    std::vector<glm::vec3> points;
    for (int i = 0; i < numPoints; ++i) {
        // �� [-radius, radius] ��Χ�������������
        float x = glm::linearRand(-radius, radius);
        float y = glm::linearRand(-radius+0.05, radius+0.05);
        float z = glm::linearRand(-radius, radius);

        // �����Ƿ������ڲ�
        glm::vec3 point(x, y, z);
        if (glm::length(point- glm::vec3(0.0f, 0.05f, 0.0f)) <= radius) {
            points.push_back(point);
        }
        else {
            // ����㲻�����ڲ������������µ������
            --i;  // ���³������������
        }
    }
    return points;
}
class ParticleSystem {
public:
    std::vector<Particle> particles; // ��������
    glm::vec3 position;              // ����ϵͳλ��
    glm::vec3 velocity;              // ����ϵͳ�ٶ�
    unsigned int VBO, VAO;           // ������Ⱦ��OpenGL����������
    std::vector<glm::vec3> particleVertices; // ���ڴ洢����λ�õ�����
    std::vector<glm::vec4> particleColors;   // ���ڴ洢������ɫ������
    std::vector<float> particleSizes;   // ���ڴ洢���Ӵ�С������

public:
    ParticleSystem() {
        position = glm::vec3(0.0f, 0.05f, 0.0f);
        velocity = glm::vec3(0.0f, 0.0f, -0.8f);
        particles.clear();
        particles.resize(numPoints);
        cout << "333sizeof(particles):" << particles.size() << endl;
        std::vector<glm::vec3> pointP = generateRandomPointsInSphere(numPoints, fireBallRadius);
        for(int i = 0; i < numPoints; i++){
			particles[i].position = pointP[i];
			//particles[i].velocity = glm::vec3(0.4f,0.6f,-0.8f);
            // �������ӵ����ĵľ���
            double distanceToCenter = glm::distance(particles[i].position , position);
            cout<<"distanceToCenter:"<<distanceToCenter<<endl;
             // ʹ������ӳ��������͸����
            float alpha = 1.0f - (distanceToCenter / fireBallRadius);  // ͸���ȴ�1��0

            // ȷ��͸�����ں���ķ�Χ��
            alpha = glm::clamp(alpha, 0.1f, 1.0f);  // ��С͸������Ϊ0.1�����͸������Ϊ1.0
            cout<<"alpha:"<<alpha<<endl;
			particles[i].color = glm::vec4(0.8f, 0.3, 0.1,alpha);
			particles[i].life = 1.0f;
			particles[i].size = avgBallSize;
		}
        //cout<<"222sizeof(particles):"<<particles.size()<<endl;
        glGenVertexArrays(1, &VAO);
      
      
       
    }

    ~ParticleSystem() {
        // ����OpenGL��Դ
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // �����µ�����
    void emitParticle(const Particle& particle) {
        particles.push_back(particle);
    }

    // ��������ϵͳ
    void update(float deltaTime) {
      //  std::cout<<"11"<<"sizeof(particles):"<< particles.size() <<std::endl;
      
        position += velocity * deltaTime;
        cout<<"����λ��"<<position.x<<" "<<position.y <<" "<< position.z << endl;
            
            if (position.z- fireBallRadius< -0.4f) {
				//particle.position.z = 0.5f;
                velocity.z = -velocity.z;
				
			}
            if (position.x - fireBallRadius < -0.4) {
                //particle.position.x = -0.4 + 0.01;
                velocity.x = -velocity.x;
               
                
            }
            if (position.x+ fireBallRadius > 0.4) {
               // particle.position.x = 0.4 - 0.01;
                velocity.x = -velocity.x;
               
            }
            if (position.y- fireBallRadius < -0.049879f) {
                //particle.position.y = -0.049879f + 0.01;
                velocity.y = -velocity.y;
            }
            if (position.y+ fireBallRadius > 0.4) {
                //particle.position.y = 0.4 - 0.01;

                velocity.y = -velocity.y;
               
            }
            if(position.z+ fireBallRadius >0.4) {   
				velocity.z = -velocity.z;
            }
            for (int i = 0; i < 30; i++) {
                if (glm::distance(position, balls[i].position) < ballRadius+ fireBallRadius) {
                    //��ӦС��Ӧ�����ϻҽ���������ɫΪ��ɫ��ֻʣ����y�����µ����������˶�
                    balls[i].textureID = ashTexture;
					balls[i].velocity = glm::vec3(0.0f,0.0f,0.0f);
				}
            }
            //�ж��Ƿ�ײ������
            for (int i = 0; i < 5; i++) {
               glm::vec3 normal(0.0f, 0.0f, 0.0f);
               Tumbler tumbler = tumblers[i];
               if (tumbler.checkFireBallCollapse(position, fireBallRadius,normal)) {				  
                   velocity *= -0.9;
                   position += glm::normalize(normal) * fireBallRadius;
                   cout<<"�����̱�ײ��"<<endl;
                   if (position.z > maxZ) {
                       maxZ = position.z;

                   }
                   cout << "���z" << maxZ << endl;
                   break;
			   }
              
            }
            for (auto& particle : particles) {
                // �����������ԣ�λ�á��������ڵȣ�
                // ...
                particle.position += velocity * deltaTime;
                particle.life -= deltaTime;

            }
           
        
       
        //cout<<"0deλ��"<<particles[0].position.z<<endl;
        // �Ƴ����������Ѿ�����������
     /*   particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0.0f; }), particles.end());*/
    }

    // ��Ⱦ����ϵͳ
    void render(Shader& shader,float deltaTime) {
        update(deltaTime);

        particleVertices.clear();
        particleColors.clear();
        particleSizes.clear();
        for (const auto& particle : particles) {
            particleVertices.push_back(particle.position);
			particleColors.push_back(particle.color);
			particleSizes.push_back(particle.size);
        }

        // ����uniform����
        glBindVertexArray(VAO);
        shader.setVec4Array("colors", numPoints, particleColors.data());
        shader.setVec3Array("vertices", numPoints, particleVertices.data());
        shader.setFloatArray("sizes", numPoints, particleSizes.data());

        cout<<"chuanruzhuoseqi"<<particleVertices[0].z<<endl;
        glEnable(GL_PROGRAM_POINT_SIZE);
        // �ڻ���֮ǰ���û��
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArraysInstanced(GL_POINTS, 0, particles.size(), numPoints);
        // �ڻ���֮����û��
        glDisable(GL_BLEND);
        glBindVertexArray(0);

      
    }
};
