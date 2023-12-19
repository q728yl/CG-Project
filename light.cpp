#include "light.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include"model.h"
#include"Mesh.h"


const GLfloat  PI = 3.14159265358979323846f;
const int Y_SEGMENTS = 50;
const int X_SEGMENTS = 50;

Light::Light(glm::vec3 position,float radius)
    : position(position), radius(radius) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 360, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}
void Light::draw() {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    for (int lat = 0; lat <= Y_SEGMENTS / 2; lat++) {
        float theta = lat * PI / Y_SEGMENTS;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= X_SEGMENTS; lon++) {
            float phi = lon * 2 * PI / X_SEGMENTS;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            vertices.push_back(glm::vec3(position.x + radius * x, position.y - radius * y, position.z + radius * z));
        }
    }

    for (int lat = 0; lat < Y_SEGMENTS / 2; lat++) {
        for (int lon = 0; lon < X_SEGMENTS; lon++) {
            int first = lat * (X_SEGMENTS + 1) + lon;
            int second = first + 1;
            int third = (lat + 1) * (X_SEGMENTS + 1) + lon;
            int fourth = third + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(third);

            indices.push_back(second);
            indices.push_back(fourth);
            indices.push_back(third);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



