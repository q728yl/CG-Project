#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "shader.h"
#include"wall.h"
#include "tumbler.h"
#include"Mesh.h"
#include "model.h"
#include"light.h"
#include <iostream>
glm::vec3 wallColor1;
glm::vec3 wallColor2;
glm::vec3 wallColor3;
glm::vec3 wallColor4;
glm::vec3 wallColor5;
bool keyS = 0;
struct Ball {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 color;
    unsigned int textureID;
};
std::vector<Ball> balls;
void drawBalls(int);
void updateBallPosition(int);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
// settings
const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 1600;
const GLfloat  PI = 3.14159265358979323846f;
const int Y_SEGMENTS = 20;
const int X_SEGMENTS = 20;
glm::mat4 projection;
glm::mat4 model;
glm::mat4 view;
// camera
Camera camera(glm::vec3(0.0f, 0.2f, 1.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing7
float deltaTime = 0.0f;
float lastFrame = 0.0f;



int main()
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // 你的光源颜色
    // lighting
    glm::vec3 lightPos(0.0f, 0.4f, 0.0f);
    Shader tumblerShader("1.model_loading.vs", "1.model_loading.fs");
    Shader wallShader("wall.vs", "wall.fs");
    Shader lightShader("light.vs", "light.fs");
 

    //后墙
    std::vector<glm::vec3> wallVertices1 = {
        glm::vec3(-0.4f, -0.049879f, -0.4f),
        glm::vec3(-0.4f, 0.4f, -0.4f),
        glm::vec3(0.4f, -0.049879f, -0.4f),
        glm::vec3(0.4f, 0.4f, -0.4f),
    };
    Wall wall1;
    wall1.SetVerticesAndColor(wallVertices1);

    //左墙
    std::vector<glm::vec3> wallVertices2 = {
        glm::vec3(-0.4f, 0.4f, -0.4f),
        glm::vec3(-0.4f, -0.049879f, -0.4f),
        glm::vec3(-0.4f, 0.4f, 0.4f),
        glm::vec3(-0.4f, -0.049879f, 0.4f)
    };
    Wall wall2;
    wall2.SetVerticesAndColor(wallVertices2);
    //右墙
    std::vector<glm::vec3> wallVertices3 = {
        glm::vec3(0.4f, 0.4f, -0.4f),
        glm::vec3(0.4f, -0.049879f, -0.4f),
        glm::vec3(0.4f, 0.4f, 0.4f),
        glm::vec3(0.4f, -0.049879f, 0.4f)
    };
    Wall wall3;
    wall3.SetVerticesAndColor(wallVertices3);

    //天花板
    std::vector<glm::vec3> wallVertices4 = {
       glm::vec3(0.4f, 0.4f, 0.4f),
       glm::vec3(0.4f, 0.4f, -0.4f),
       glm::vec3(-0.4f, 0.4f, 0.4f),
       glm::vec3(-0.4f, 0.4f, -0.4f)
    };
    Wall wall4;
    wall4.SetVerticesAndColor(wallVertices4);
    //地板
    std::vector<glm::vec3> wallVertices5 = {
       glm::vec3(0.4f, -0.049879f, 0.4f),
       glm::vec3(0.4f, -0.049879f, -0.4f),
       glm::vec3(-0.4f, -0.049879f, 0.4f),
       glm::vec3(-0.4f, -0.049879f, -0.4f)
    };
    Wall wall5;
    wall5.SetVerticesAndColor(wallVertices5);
    //墙体颜色设置
    wallColor1 = glm::vec3(0.2f, 0.2f, 0.3f);
    wallColor2 = glm::vec3(0.4f, 0.2f, 0.5f);
    wallColor3 = glm::vec3(0.8f, 0.5f, 0.7f);
    wallColor4 = glm::vec3(0.8f, 0.8f, 0.8f);
    wallColor5 = glm::vec3(0.8f, 0.8f, 0.8f);
    
    //灯
    Light light(lightPos,0.08f);
    std::vector<Model> modelInstances;
    std::vector<glm::vec3> modelPositions;
    glm::vec3 position1(-0.2f, 0.0f,-0.2f);
    glm::vec3 position2(0.2f, 0.0f, -0.2f);
    glm::vec3 position3(0.0f, 0.0f, 0.0f);
    glm::vec3 position4(-0.2f, 0.0f, 0.2f);
    glm::vec3 position5(0.2f, 0.0f, 0.2f);
    modelPositions.push_back(position1);
    modelPositions.push_back(position2);
    modelPositions.push_back(position3);
    modelPositions.push_back(position4);
    modelPositions.push_back(position5);
    std::vector<Tumbler> tumblers;
    Tumbler tumbler1;
    Tumbler tumbler2;
    Tumbler tumbler3;
    Tumbler tumbler4;
    Tumbler tumbler5;
    Model tummodel("model/tumbler.obj");
    glm::mat4 E = glm::mat4(1.0f);
    for (int i = 0; i < 5; ++i) {
        modelInstances.push_back(tummodel);
    }
    tumbler1.setModelMatrix(glm::translate(E, modelPositions[0]));
    tumbler2.setModelMatrix(glm::translate(E, modelPositions[1]));
    tumbler3.setModelMatrix(glm::translate(E, modelPositions[2]));
    tumbler4.setModelMatrix(glm::translate(E, modelPositions[3]));
    tumbler5.setModelMatrix(glm::translate(E, modelPositions[4]));
    tumblers.push_back(tumbler1);
    tumblers.push_back(tumbler2);
    tumblers.push_back(tumbler3);
    tumblers.push_back(tumbler4);
    tumblers.push_back(tumbler5);
    //30个小球
    int ballTexture = TextureFromFile("ball.png", "./");
    // 初始化30个小球
    for (int i = 0; i < 30; ++i) {
        Ball ball;

        // 设置随机位置
        float xPos = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.6f - 0.3f;  // 范围从-0.3到0.3
        float yPos = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.3f ;
        float zPos = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.6f - 0.3f;
        ball.position = glm::vec3(xPos, yPos, zPos);

        // 设置随机速度
        float xVel = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.3f - 0.15f;  // 范围从-0.15到0.15
        float yVel = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.3f - 0.15f;
        float zVel = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.3f - 0.15f;
        ball.velocity = glm::vec3(xVel, yVel, zVel);

        ball.color = glm::vec3(1.0f, 1.0f, 1.0f);  // 设置为白色

        // 加载并设置纹理ID
        ball.textureID = ballTexture;  // 请替换为你的纹理路径

        balls.push_back(ball);
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        /*    accumulatedTime = currentFrame - initTime;*/
            // input
            // -----
        processInput(window);
        int floorTexture = TextureFromFile("floor.png", "./");
        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //坐标变换
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        //墙体绘制
        wallShader.use();
        wallShader.setVec3("lightPos", lightPos);
        wallShader.setVec3("lightColor", lightColor);
        wallShader.setVec3("viewPos", camera.Position);
        wallShader.setMat4("projection", projection);
        wallShader.setMat4("view", view);
        wallShader.setMat4("model", model);
        wallShader.setVec3("wallColor", wallColor1);
        wallShader.setBool("hasTexture", false);
        wall1.wallDraw(floorTexture);
        wallShader.setVec3("wallColor", wallColor2);
        wall2.wallDraw(floorTexture);
        wallShader.setVec3("wallColor", wallColor3);
        wall3.wallDraw(floorTexture);
        wallShader.setVec3("wallColor", wallColor4);
        wall4.wallDraw(floorTexture);
        wallShader.setVec3("wallColor", wallColor5);
        wallShader.setBool("hasTexture", true);
        wall5.setHasTexture();
        wall5.wallDraw(floorTexture);
        // 灯绘制
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setMat4("model", model);
        lightShader.setVec3("lightColor", lightColor);
        lightShader.setVec3("lightPos", lightPos);
        lightShader.setVec3("viewPos", camera.Position);
    
        light.draw();
        
        // 渲染循环中遍历所有模型实例
        for (size_t i = 0; i < 5; ++i) {
            // 使用每个模型实例的独立位置创建model矩阵，会在model基础上进行更多变换
            // 将世界变换矩阵传递给着色器
            tumblerShader.use();
            tumblerShader.setVec3("lightPos", lightPos);
            tumblerShader.setVec3("lightColor", lightColor);
            tumblerShader.setVec3("viewPos", camera.Position);
            tumblerShader.setMat4("projection", projection);
            tumblerShader.setMat4("view", view);
            tumblerShader.setMat4("model", tumblers[i].getModelMatrix());

            // 渲染当前模型实例
            modelInstances[i].Draw_out(tumblerShader);
        }

        if(keyS)
            drawBalls(floorTexture);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        keyS = 1;
    }
        
}
glm::vec3 getViewPos(int x, int y, glm::mat4 pro, glm::mat4 view)
{
    // 将屏幕坐标转换为NDC（标准化设备坐标）
    float win_x = (float)x;
    float win_y = (float)SCR_HEIGHT - (float)y - 1.0f;
    float win_z;

    // 从深度缓冲区中读取深度值
    glReadBuffer(GL_BACK);
    glReadPixels(x, int(win_y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

    // 反投影屏幕坐标到世界坐标
    glm::vec3 winCoords(win_x, win_y, win_z);
    glm::vec4 viewport = glm::vec4(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glm::vec3 obj = glm::unProject(winCoords, view, pro, viewport);

    return obj;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // 转换屏幕坐标为3D世界坐标
        glm::vec3 worldPos = getViewPos((int)xpos, (int)ypos, projection, view);

        // 输出或使用worldPos
        std::cout << "Clicked 3D position: (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
    }
}

void drawBalls(int floorTextureId)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Shader ballShader("ball.vs", "ball.fs");
    ballShader.use();
    ballShader.setMat4("projection", projection);
    ballShader.setMat4("view", view);
    ballShader.setMat4("model", model);
    //glActiveTexture(GL_TEXTURE0); // 激活纹理单元0
    //int ballTexture = TextureFromFile("ball.png", "./");
    //glBindTexture(GL_TEXTURE_2D, floorTextureId); // 将纹理绑定到纹理单元0
    //ballShader.setInt("ourTexture", 0); // 或者你可以使用setUniform函数为纹理采样器设置值

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 计算需要的总大小，每个顶点有5个float（3个坐标 + 2个纹理坐标）
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 5 * 360 * balls.size(), nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    for (int i = 0; i < balls.size(); ++i) {
        ballShader.setVec3("ourColor", balls[i].color);
        vertices.clear();
        indices.clear();
        glActiveTexture(GL_TEXTURE0); // 激活纹理单元0
        glBindTexture(GL_TEXTURE_2D, balls[i].textureID); // 将纹理绑定到纹理单元0
        ballShader.setInt("ourTexture", 0);
        for (int lat = 0; lat <= Y_SEGMENTS; lat++) {
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

                float s = (float)lon / X_SEGMENTS;
                float t = (float)lat / Y_SEGMENTS;

                // 添加顶点坐标
                vertices.push_back(balls[i].position.x + 0.01 * x);
                vertices.push_back(balls[i].position.y + 0.01 * y);
                vertices.push_back(balls[i].position.z + 0.01 * z);
                // 添加纹理坐标
                vertices.push_back(s);
                vertices.push_back(t);
            }
        }
        for (int lat = 0; lat < Y_SEGMENTS; lat++) {
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    updateBallPosition(floorTextureId);
}

void updateBallPosition(int floorTextureId) {
    
    for (int i = 0; i < balls.size(); ++i) {
       /* glm::vec4 newP = projection* view* model* glm::vec4(balls[i].position, 1.0);
        glm::vec3 newP3 = glm::vec3(newP.x , newP.y, newP.z );*/

        balls[i].position.y -= 0.05 * deltaTime;
        balls[i].position += balls[i].velocity * deltaTime;
       
        if (balls[i].position.x - 0.01 < -0.4) {
            balls[i].position.x = -0.4 + 0.01;
            balls[i].velocity.x = -balls[i].velocity.x;
            balls[i].color = wallColor2;
        }
        else if (balls[i].position.x + 0.01 > 0.4) {
            balls[i].position.x = 0.4 - 0.01;
            balls[i].velocity.x = -balls[i].velocity.x;
            balls[i].color = wallColor3;
        }
        if (balls[i].position.y - 0.01 < -0.049879f) {
            balls[i].position.y = -0.049879f + 0.01;
            balls[i].velocity.y = -balls[i].velocity.y;
            balls[i].color = wallColor5;
            balls[i].textureID = floorTextureId;
        }
        else if (balls[i].position.y + 0.01 > 0.4) {
            balls[i].position.y = 0.4 - 0.01;
            balls[i].velocity.y = -balls[i].velocity.y;
            balls[i].color = wallColor4;
        }

        if (balls[i].position.z - 0.01 < -0.4) {
            balls[i].position.z = -0.4 + 0.01;
            balls[i].velocity.z = -balls[i].velocity.z;
            balls[i].color = wallColor1;
        }
        else if (balls[i].position.z + 0.01 > 0.4) {
            balls[i].position.z = 0.4 - 0.01;
            balls[i].velocity.z = -balls[i].velocity.z;
        }
    }
   
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}