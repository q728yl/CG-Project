#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "shader.h"
#include"wall.h"

#include"Mesh.h"
#include "model.h"
#include"light.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    glm::vec3 wallColor1 = glm::vec3(0.2f, 0.2f, 0.3f);
    glm::vec3 wallColor2 = glm::vec3(0.4f, 0.2f, 0.5f);
    glm::vec3 wallColor3 = glm::vec3(0.8f, 0.5f, 0.7f);
    glm::vec3 wallColor4 = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 wallColor5 = glm::vec3(0.8f, 0.8f, 0.8f);
    
    //灯
    Light light(lightPos,0.08f);

    /*Model ourModel("model/tumbler.obj");*/
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
    for (int i = 0; i < 5; ++i) {
        Model model("model/tumbler.obj");
        modelInstances.push_back(model);    
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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
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
        for (size_t i = 0; i < modelInstances.size(); ++i) {
            // 使用每个模型实例的独立位置创建世界变换矩阵
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, modelPositions[i]);

            // 在这里，你可能还需要处理其他的世界变换，例如旋转和缩放

            // 将世界变换矩阵传递给着色器
            tumblerShader.use();
            tumblerShader.setVec3("lightPos", lightPos);
            tumblerShader.setVec3("lightColor", lightColor);
            tumblerShader.setVec3("viewPos", camera.Position);
            tumblerShader.setMat4("projection", projection);
            tumblerShader.setMat4("view", view);
            tumblerShader.setMat4("model", modelMatrix);

            // 渲染当前模型实例
            modelInstances[i].Draw_out(tumblerShader);
        }


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
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
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