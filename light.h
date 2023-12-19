#include <vector>
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma once

#include <glm/glm.hpp>
#include<vector>

class Light {
public:
    Light(glm::vec3 position, float radius);
   
    glm::vec3 getPosition(Light light) {
        return light.position;
    }
    float getRadius(Light light) {
        return light.radius;
    }

    void draw();

private:

    glm::vec3 position;
    float radius;
    unsigned int VAO, VBO, EBO;
};
