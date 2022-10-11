//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "shader.h"
#include "computeshader.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "cube.h"
#include "textures.h"
#include "camera.h"

//using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);   
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data);

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const int screenw = 2560;
const int screenh = 1440;
bool firstMouse = true;
int toggle = 0;
float dt = 0;
float rays = 0;
float spUp = 0;
float spinParameter = 0;
Camera cam(0.0f, 0.0f, 0.0f);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(screenw, screenh, "Blackhole", glfwGetPrimaryMonitor(), NULL);
    cam.setWindow(window);

    if(window == NULL)
    {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    //glDebugMessageCallback(GLDebugMessageCallback, NULL);

    glViewport(0, 0, screenw, screenh);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSwapInterval(0);

    Shader ourShader("v_shader.vert", "f_shader.frag");

    const unsigned int gridX = 1000, gridY = 562;
    const unsigned int gridX2 = 512, gridY2 = 512;

    //raymarching compute shader
    cShader comp("raymarch(safish gen1).comp", gridX, gridY);

    //particle simulation compute shader
    cShader posComp("particle.comp", gridX2, gridY2);

    cShader noise("noise.comp", gridX2, gridY2);

    unsigned int render, particles, atlas;

    glGenTextures(1, &render);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, gridX, gridY, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(0, render, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F); 

    glGenTextures(1, &particles);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, particles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 3*gridX2, gridY2, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(1, particles, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F); 

    glGenTextures(1, &atlas);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4096, 4096, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindImageTexture(2, atlas, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F); 

    //creates cube object
    //Cube cube(gridX, gridY);

    //unsigned char *data;
    //texture.addTexture("container.jpg", data, 0);
    //texture.addTexture("awesomeface.png", data, 1);
    Texture texture(ourShader);
    unsigned char *data;
    //unsigned char *datao;
    texture.addTexture("galaxy.png", data, 1);
    texture.addTexture("noiseTexture.png", data, 1);
    texture.addTexture("lowResNoise.png", data, 1);
    Shader shader("v_shader.vert", "f_shader.frag");

    float vertices[] = {
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top right
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    shader.setInt("tex", 0);
    glm::mat4 view = glm::mat4(1.0f);

    comp.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render);
    comp.setVec2("res", float(gridX), float(gridY));
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    texture.activateTexture(0);
    comp.setInt("testSample", 1);
    comp.setInt("particleAtlas", 2);
    comp.setInt("noise", 4);
    comp.setInt("lowResNoise", 5);
    comp.setInt("galaxy", 3);

    posComp.use();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, particles);
    posComp.setInt("prevSample", 1);
    texture.activateTexture(0);
    posComp.setInt("noise", 4);

    noise.use();
    noise.setInt("particle", 1);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearTexImage(atlas, 0, GL_RGBA, GL_FLOAT, NULL);
        processInput(window);
        view = cam.updateView();

        posComp.use();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, particles);
        posComp.setFloat("time", (float)glfwGetTime());
        posComp.setFloat("speedUp", spUp);
        posComp.setMat4("view", view);
        if(firstMouse){
            firstMouse = false;
            posComp.setInt("tru", 1);
        }
        else{
            posComp.setInt("tru", 0);    
        }

        /*noise.use();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, particles);
        noise.setInt("particle", 1);
        texture.activateTexture(1);
        noise.setInt("noise", 4);*/

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, atlas);

        // render
        comp.use();
        comp.setInt("testSample", 1);
        comp.setInt("particleAtlas", 2);
        comp.setMat4("view", view);
        comp.setFloat("time", float(glfwGetTime()));
        comp.setFloat("timeDT", dt);
        comp.setFloat("rays", rays);
        comp.setFloat("aP", spinParameter);
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //texture.activateTexture(0);

        shader.use();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture1);
        shader.setInt("tex", 0);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            dt += 0.002;
            std::cout << "dt: " << dt + 0.03 << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            dt -= 0.002;
            std::cout << "dt: " << dt + 0.03 << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
            rays += 0.1;
            std::cout << "rays: " << 1000*rays << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
            rays -= 0.1;
            std::cout << "rays: " << 1000*rays << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
            dt = 0;
            std::cout << "dt set to 0" << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
            rays = 0;
            std::cout << "rays set to 0" << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            spUp += 0.01;
            std::cout << "time scale: " << 1+spUp << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
            spUp -= 0.01;
            std::cout << "time scale: " << 1+spUp<< "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            spUp += 0.0003;
            std::cout << "time scale: " << 1+spUp << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
            spUp -= 0.0003;
            std::cout << "time scale: " << 1+spUp<< "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
            spUp = -0.999;
            std::cout << "time scale: " << 1+spUp<< "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
            spUp = 0;
            std::cout << "time scale: " << 1+spUp<< "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
            spinParameter = 0.00;
            std::cout << "Spin Parameter: " << spinParameter << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
            spinParameter = 1.00;
            std::cout << "Spin Parameter: " << spinParameter << "\n";
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && toggle == 3){
        toggle = 0;
    }

    if(glfwGetKey(window, GLFW_KEY_6) == GLFW_RELEASE && toggle <= 1){
        toggle = 1;
        cam.updateDirection(xpos, ypos, firstMouse);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        spUp = 0;
    }

    else if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && toggle == 1){
        toggle = 2;
        spUp = -0.999;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if(glfwGetKey(window, GLFW_KEY_6) == GLFW_RELEASE && toggle == 2)
    {
        toggle = 3;
    }
}