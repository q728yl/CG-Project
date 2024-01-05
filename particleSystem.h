#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations
#include <string>
#include <vector>
#include <glm/gtc/random.hpp>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
const int numPoints = 500;
float avgBallSize = 12.0f;
const float fireBallRadius = 0.015f;

struct Particle {
    glm::vec3 position;     // 粒子位置
    glm::vec3 velocity;     // 粒子速度
    glm::vec4 color;        // 粒子颜色 (使用vec4表示RGBA)
    float life;             // 粒子生命周期
    float size;             // 粒子大小

    Particle()
        : position(-0.0f), velocity(0.0f), color(1.0f), life(1.0f), size(1.0f) {}
};
std::vector<glm::vec3> generateRandomPointsInSphere(int numPoints, float radius) {
    std::vector<glm::vec3> points;
    for (int i = 0; i < numPoints; ++i) {
        // 在 [-radius, radius] 范围内生成随机坐标
        float x = glm::linearRand(-radius, radius);
        float y = glm::linearRand(-radius+0.2, radius+0.2);
        float z = glm::linearRand(-radius, radius);

        // 检查点是否在球内部
        glm::vec3 point(x, y, z);
        if (glm::length(point- glm::vec3(0.0f, 0.2f, 0.0f)) <= radius) {
            points.push_back(point);
        }
        else {
            // 如果点不在球内部，重新生成新的随机点
            --i;  // 重新尝试生成这个点
        }
    }
    return points;
}
class ParticleSystem {
private:
    std::vector<Particle> particles; // 粒子数组
    unsigned int VBO, VAO;           // 用于渲染的OpenGL缓冲区对象
    std::vector<glm::vec3> particleVertices; // 用于存储粒子位置的数据
    std::vector<glm::vec4> particleColors;   // 用于存储粒子颜色的数据
    std::vector<float> particleSizes;   // 用于存储粒子大小的数据

public:
    ParticleSystem() {
        particles.clear();
        particles.resize(numPoints);
        cout << "333sizeof(particles):" << particles.size() << endl;
        std::vector<glm::vec3> pointP = generateRandomPointsInSphere(numPoints, fireBallRadius);
        for(int i = 0; i < numPoints; i++){
			particles[i].position = pointP[i];
			particles[i].velocity = glm::vec3(0.0f,0.0f,-0.1f);
            // 计算粒子到球心的距离
            double distanceToCenter = glm::distance(particles[i].position , glm::vec3(0.0f, 0.2f, 0.0f));
            cout<<"distanceToCenter:"<<distanceToCenter<<endl;
             // 使用线性映射来计算透明度
            float alpha = 1.0f - (distanceToCenter / fireBallRadius);  // 透明度从1到0

            // 确保透明度在合理的范围内
            alpha = glm::clamp(alpha, 0.1f, 1.0f);  // 最小透明度设为0.1，最大透明度设为1.0
            cout<<"alpha:"<<alpha<<endl;
			particles[i].color = glm::vec4(0.8f, 0.3, 0.1,alpha);
			particles[i].life = 1.0f;
			particles[i].size = avgBallSize;
		}
        //cout<<"222sizeof(particles):"<<particles.size()<<endl;
        glGenVertexArrays(1, &VAO);
      
      
       
    }

    ~ParticleSystem() {
        // 清理OpenGL资源
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    // 创建新的粒子
    void emitParticle(const Particle& particle) {
        particles.push_back(particle);
    }

    // 更新粒子系统
    void update(float deltaTime) {
      //  std::cout<<"11"<<"sizeof(particles):"<< particles.size() <<std::endl;
        for (auto& particle : particles) {
            // 更新粒子属性（位置、生命周期等）
            // ...
            particle.position += particle.velocity * deltaTime;
            particle.life -=   deltaTime;

        }
        //cout<<"0de位置"<<particles[0].position.z<<endl;
        // 移除生命周期已经结束的粒子
     /*   particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0.0f; }), particles.end());*/
    }

    // 渲染粒子系统
    void render(Shader& shader,float deltaTime) {

        particleVertices.clear();
        particleColors.clear();
        particleSizes.clear();
        for (const auto& particle : particles) {
            particleVertices.push_back(particle.position);
			particleColors.push_back(particle.color);
			particleSizes.push_back(particle.size);
        }

        // 设置uniform数据
        glBindVertexArray(VAO);

        // 更新uniform数据
    /*    GLuint verticesLocation = glGetUniformLocation(shader.ID, "vertices");
        GLuint colorsLocation = glGetUniformLocation(shader.ID, "colors");

        glUniform4fv(verticesLocation, numPoints, particleVertices.data());
        glUniform4fv(colorsLocation, numPoints, particleColors.data());*/
        shader.setVec4Array("colors", numPoints, particleColors.data());
        shader.setVec3Array("vertices", numPoints, particleVertices.data());
        shader.setFloatArray("sizes", numPoints, particleSizes.data());

        cout<<"chuanruzhuoseqi"<<particleVertices[0].z<<endl;
        glEnable(GL_PROGRAM_POINT_SIZE);
        /*glPointSize(5.0f);*/
        // 在绘制之前启用混合
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArraysInstanced(GL_POINTS, 0, particles.size(), numPoints);
        // 在绘制之后禁用混合
        glDisable(GL_BLEND);
        glBindVertexArray(0);

       // update(deltaTime);
    }
};
