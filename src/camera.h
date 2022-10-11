#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    private:
        GLFWwindow *window;

    public:
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection;
    glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    float yaw = 0.0f, pitch = 0.0f;
    float lastX = 400, lastY = 300;

    Camera(float x, float y, float z)
    {
        cameraPos = glm::vec3(x, y, z);
        cameraDirection = glm::normalize(cameraPos - cameraTarget);
        cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
        //cameraUp = glm::cross(cameraDirection, cameraRight);
    }

    void setWindow(GLFWwindow *&win){
        window = win;
    }

    glm::mat4 updateView()
    {
        view = glm::mat4(1.0f);
        processCamInput(window);
        //float radius = 10.0f;
        //float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        //float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
        //view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //view = glm::lookAt(cameraPos, cameraFront, cameraUp);
        view[0] = rot[0];
        //std::cout << view[0][0] << std::endl;
        //std::cout << view[1][1] << std::endl;
        //std::cout << view[2][2] << std::endl;
        //std::cout << view[3][3] << std::endl;
        view[1] = rot[1];
        view[2] = rot[2];
        view[3] = glm::vec4(cameraPos, 1);
        //view[3] = glm::vec4(cameraPos, 1);
        return view;
    }

    void processCamInput(GLFWwindow *window)
    {
        float cameraSpeed = 0.5f; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            cameraSpeed = 0.002f;
        }
        else{
            cameraSpeed = 0.2f;    
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            cameraPos += cameraSpeed * cameraFront;
            //std::cout << "cam pos: " << cameraPos.x<< " " << cameraPos.y << " " << cameraPos.z << std::endl;
            //std::cout << "radial position: " << length(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z) - glm::vec3(0, 0, 0)) << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            cameraPos -= cameraSpeed * cameraFront;
            //std::cout << "cam pos: " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
            //std::cout << "radial position: " << length(glm::vec3(cameraPos.x , cameraPos.y, cameraPos.z) - glm::vec3(0, 0, 0)) << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            //std::cout << "cam pos: " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
            //std::cout << "radial position: " << length(glm::vec3(cameraPos.x, cameraPos.y, cameraPos.z) - glm::vec3(0, 0, 0)) << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            //std::cout << "cam pos: " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
            //std::cout << "radial position: " << length(glm::vec3(cameraPos.x , cameraPos.y, cameraPos.z) - glm::vec3(0, 0, 0)) << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraUp;
    }

    void updateDirection(float xpos, float ypos, bool &firstMouse){
        if (firstMouse) // initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;
        lastX = xpos;
        lastY = ypos;
        
        float sensitivity = 0.05f;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            sensitivity = 0.001f;
        }
        else{
            sensitivity = 0.05f;    
        }
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch -= yoffset;  

        glm::vec3 direction;
        direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);    

        rot = glm::mat4(1.0f);
        rot = glm::rotate(rot, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rot = glm::rotate(rot, glm::radians(-pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    }

};