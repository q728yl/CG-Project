#pragma once
#include <vector>
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <glm/glm.hpp>
#include<vector>
extern glm::vec3 wallColor1;
extern glm::vec3 wallColor2;
extern glm::vec3 wallColor3;
extern glm::vec3 wallColor4;
extern glm::vec3 wallColor5;
extern glm::mat4 model;
extern int ashTexture;
extern float deltaTime;
class Ball {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 color;
    glm::mat4 modelMatrix;
    int shouldDraw = 1;
    unsigned int textureID;
    void updateBallPosition(int floorTextureId) {
           velocity.y -= 0.05 * deltaTime;
           position +=velocity * deltaTime;
           if (textureID == ashTexture&& position.y - 0.01 < -0.049879f) {
               shouldDraw = 0;
			   
           }
            if (position.x - 0.01 < -0.4) {
               position.x = -0.4 + 0.01;
               velocity.x = -velocity.x;
               color = wallColor2;
            }
            else if (position.x + 0.01 > 0.4) {
               position.x = 0.4 - 0.01;
               velocity.x = -velocity.x;
               color = wallColor3;
            }
            if (position.y - 0.01 < -0.049879f) {
               position.y = -0.049879f + 0.01;
               velocity.y = -velocity.y;
               color = wallColor5;
               textureID = floorTextureId;
            }
            else if (position.y + 0.01 > 0.4) {
               position.y = 0.4 - 0.01;
               velocity.y = -velocity.y;
               color = wallColor4;
            }

            if (position.z - 0.01 < -0.4) {
               position.z = -0.4 + 0.01;
               velocity.z = -velocity.z;
               color = wallColor1;
            }
            else if (position.z + 0.01 > 0.4) {
               position.z = 0.4 - 0.01;
               velocity.z = -velocity.z;
            }
            
            modelMatrix = glm::translate(model, position);
        }


    
};