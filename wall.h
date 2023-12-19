#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
std::vector<glm::vec2> textureCoords = {
   glm::vec2(0.0f, 1.0f), // 左上
   glm::vec2(0.0f, 0.0f), // 左下
   glm::vec2(1.0f, 1.0f), // 右上
   glm::vec2(1.0f, 0.0f)  // 右下
};
class Wall {
public:
    Wall() : VAO(0), VBO(0), EBO(0) {};

    void SetVerticesAndColor(const std::vector<glm::vec3>& vertices) {
        verticesVector.clear();

        for (size_t i = 0; i < vertices.size(); ++i) {
            verticesVector.push_back(vertices[i].x);
            verticesVector.push_back(vertices[i].y);
            verticesVector.push_back(vertices[i].z);

            // Add texture coordinates
            verticesVector.push_back(textureCoords[i].x);
            verticesVector.push_back(textureCoords[i].y);
        }

        unsigned int indices[] = {
            // Indices definition
            0, 1, 2, // First triangle
            1, 2, 3  // Second triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesVector.size() * sizeof(float), verticesVector.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Set the vertex position attribute pointer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set the texture coordinate attribute pointer
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void wallDraw(int tId) const {
        glBindVertexArray(VAO);

        if (hasTexture) {
            glEnable(GL_TEXTURE_2D);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tId);
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (hasTexture) {
            glDisable(GL_TEXTURE_2D);
        }

        glBindVertexArray(0);
    }

    void setHasTexture() {
        this->hasTexture = true;
    }

private:
    std::vector<float> verticesVector;
    unsigned int VAO, VBO, EBO;
    bool hasTexture{ 0 };
};
