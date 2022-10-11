#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static float scale = 5000000.0f;

static std::vector<float> vertices = {
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  1.0f, 0.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  1.0f, 1.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  1.0f, 1.0f,
    -0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 1.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,

    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  1.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 1.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 1.0f,
    -0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 1.0f,
    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,

    -0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 0.0f,
    -0.5f*scale,  0.5f*scale, -0.5f*scale,  1.0f, 1.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 1.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 1.0f,
    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 0.0f,

     0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 0.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  1.0f, 1.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 1.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 1.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 0.0f,

    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 1.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  1.0f, 1.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  1.0f, 0.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  1.0f, 0.0f,
    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 1.0f,

    -0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 1.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  1.0f, 1.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  1.0f, 0.0f,
    -0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 1.0f
};

static std::vector<float> vertices2 = {
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,

    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,

    -0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,

     0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,

    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale, -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,

    -0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  0.5f*scale, -0.5f*scale,  0.0f, 0.0f
};

static std::vector<float> vertices3 = {
     0.5f*scale,   0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  -0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,   0.5f*scale,  0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,   0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,  -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
    -0.5f*scale,  -0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
     0.5f*scale,   0.5f*scale, -0.5f*scale,  0.0f, 0.0f,
};

/*static glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f),  
};*/

//static glm::vec3 cubePositions[100][100];

static glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 0.0f,  10.0f, 0.0f), 
};

class Cube
{
    public:
    int xamt = 1;
    int yamt = 1;
    Cube(int xamount, int yamount)
    {
        for(int i = 0; i < xamount; i ++){
            for(int j = 0; j < yamount; j++){
                vertices2.insert(vertices2.end(), 
                {-0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                  0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                  0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),
                  0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),
                 -0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),
                 -0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),

                -0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),
                 0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),
                 0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                 0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),

                -0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),
                -0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                -0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                -0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),

                0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),
                0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),
                0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),

                -0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                 0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),
                 0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),
                 0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale, -0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale, -0.5f*scale, -0.5f*scale,  float(i), float(j),

                -0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),
                 0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),
                 0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                 0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale,  0.5f*scale,  0.5f*scale,  float(i), float(j),
                -0.5f*scale,  0.5f*scale, -0.5f*scale,  float(i), float(j),});
            }
        }
        xamt = xamount;
        yamt = yamount;
    }

    int getAmt(){
        return xamt;
    }

    float *getVertices(){
        return &vertices2[0];
    }

    int getSize(){
        return vertices2.size()*sizeof(float);
    }

    glm::vec3 *getPositions(int num){
        return cubePositions;
    }

    glm::vec3 *getPositions2(){
        return cubePositions;
    }

    private:
};