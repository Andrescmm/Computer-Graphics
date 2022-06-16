 //ANDRES CUSIRRAMOS & ANTHONY FERNANDEZ
//Computacion Grafica - CCOMP7-1
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Solver/random.h"
#include "Solver/solve.h"

#include <iostream>
#include "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P2/headers/shader_m.h"
#include "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P2/headers/cube_structure.h"


using namespace std;

//GLOBAL VARIABLES------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

vector<string> cubo;

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

//for walk around
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 7.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//MAIN PROGRAM------------------------------------------------------------------------------------------------------------------------------

// translation vector
glm::vec3 translationVector;
int main()
{
    //INITIAL SET UP*************************************************************************************************************************
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // COMPILE SHADER PROGRAM
    Shader ourShader("C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P2/shaders/cube.vs", "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P2/shaders/cube.fs");

    //SETUP FOR THE CUBES******************************************************************************************************
    unsigned int VBO[26], VAO[26];
    glGenVertexArrays(26, VAO);
    glGenBuffers(26, VBO);

    //creation
    CCube MagicCube;
    MagicCube.createLittleCubes();

    //sent to buffer
    
    MagicCube.drawCube(&VBO[0], &VAO[0]);
    

    // activate shader
    ourShader.use();

    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);


    glfwSetKeyCallback(window, key_callback);


    

    //vector<string> solution = get_solution(to_cube_not(cubo));
   


    //MAIN RENDER LOOP************************************************************************************************************************
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);   //cambiar con kewWordCallBack

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();

        // camera/view transformation
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // render boxes
        for (unsigned int i = 0; i < 26; i++)
        {
            glBindVertexArray(VAO[i]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, MagicCube.littleCubes[i].initialPosition);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //FINAL CONFIGURATION*****************************************************************************************************************
    // de-allocate resources
    glDeleteVertexArrays(26, VAO);
    glDeleteBuffers(26, VBO);

    // terminate, clearing allocated GLFW resources.
    glfwTerminate();
    return 0;
}

//KEYS FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::vec3(0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 0.0f, 0.1f);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 0.0f, 0.1f);
    //global coordenates
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraFront += glm::vec3(0.0f, 0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraFront -= glm::vec3(0.0f, 0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraFront -= glm::vec3(0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraFront += glm::vec3(0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        cameraFront -= glm::vec3(0.0f, 0.0f, 0.1f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        cameraFront += glm::vec3(0.0f, 0.0f, 0.1f);

    // moves
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        cubo.push_back("R");
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        cubo.push_back("L");
    }

    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        cubo.push_back("U");
    }

    if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        cubo.push_back("D");
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
        cubo.push_back("F");
    }

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        cubo.push_back("B");
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        
        vector<string> solution = get_solution(to_cube_not(cubo));
        //print
        cout << "Solution " << endl;
        for (int i = 0; i < solution.size(); i++) {
            if (i == 0) {
                cout << solution[i];
            }
            cout <<" , " << solution[i];
        }
        // end print

        cubo = vector<string>();
    }
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void rotateM(vector<float>& figure, int angle) {
    glm::mat2 translate(glm::vec2(cos(angle), -sin(angle)), glm::vec2(sin(angle), cos(angle)));

    for (int i = 0; i < figure.size(); i += 3) {
        glm::vec2 temp(figure[i], figure[i + 1]);
        temp = translate * temp;

        figure[i] = temp[0];
        figure[i + 1] = temp[1];
    }
}