#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class cShader
{
public:
    unsigned int ID;
    int wgWidth;
    int wgHeight;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    cShader(std::string computePath, int wid, int hig)
    {
        wgWidth = wid;
        wgHeight = hig;
        // 1. retrieve the vertex/fragment source code from filePath
        std::string computeCode;
        std::fstream cShaderFile;
        int vShaderLines = 1;

        //open file tester
        cShaderFile.open(computePath, std::ios::in);
        std::string cline;
        while(getline(cShaderFile, cline)){
            vShaderLines ++;
        }
        cShaderFile.close(); 
        
        //read files
        cShaderFile.open(computePath, std::ios::in);
        std::string cline2;
        while(getline(cShaderFile, cline2)){
            vShaderLines --;
            if(vShaderLines > 1){
                computeCode += cline2 + "\n";
            }
            else{
                computeCode += cline2 + "\0";    
            }
        }
        cShaderFile.close();

        const char* cShaderCode = computeCode.c_str();
        
        unsigned int compute = glCreateShader(GL_COMPUTE_SHADER);

        glShaderSource(compute, 1, &cShaderCode, NULL);
        glCompileShader(compute);
        checkCompileErrors(compute, "COMPUTE");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, compute);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(compute);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
        glDispatchCompute((int)ceil((unsigned int)wgWidth/8), (int)ceil((unsigned int)wgHeight/8), 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
    int getID(){
        return ID;
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        //std::cout << name << std::endl; 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setVec2(const std::string &name, float value1, float value2) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
    }

    void setMat4(const std::string &name, glm::mat4 &value) const
    { 
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); 
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- ----------------------------------------------------- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- ----------------------------------------------------- " << std::endl;
            }
        }
    }
};