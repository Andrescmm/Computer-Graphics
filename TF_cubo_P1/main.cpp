//ANDRES CUSIRRAMOS & ANTHONY FERNANDEZ
//Computacion Grafica - CCOMP7-1
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/dep/include/shader_m.h"

#include <vector>
#include <stb_image.h>
using namespace std;

//GLOBAL VARIABLES------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned int VBO[26], VAO[26];


// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

//for walk around
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 7.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

 //SPATIAL INFORMATION*******************************************************************************************************************
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 iPlittleCubes[] = {
    glm::vec3(-1.2f, 1.2f, 1.2f),   //0
    glm::vec3(0.0f, 1.2f, 1.2f),   //1
    glm::vec3(1.2f, 1.2f, 1.2f),   //2

    glm::vec3(-1.2f, 0.0f, 1.2f),  //3
    glm::vec3(0.0f, 0.0f, 1.2f),  //4
    glm::vec3(1.2f, 0.0f, 1.2f),  //5

    glm::vec3(-1.2f, -1.2f, 1.2f),  //6
    glm::vec3(0.0f, -1.2f, 1.2f),  //7
    glm::vec3(1.2f, -1.2f, 1.2f),  //8

    glm::vec3(-1.2f, 1.2f, 0.0f),  //9
    glm::vec3(0.0f, 1.2f, 0.0f),  //10
    glm::vec3(1.2f, 1.2f, 0.0f),  //11

    glm::vec3(-1.2f, 0.0f, 0.0f), //12
    glm::vec3(1.2f, 0.0f, 0.0f), //13

    glm::vec3(-1.2f, -1.2f, 0.0f), //14
    glm::vec3(0.0f, -1.2f, 0.0f), //15
    glm::vec3(1.2f, -1.2f, 0.0f), //16

    glm::vec3(-1.2f, 1.2f, -1.2f),  //17
    glm::vec3(0.0f, 1.2f, -1.2f),  //18
    glm::vec3(1.2f, 1.2f, -1.2f),  //19

    glm::vec3(-1.2f, 0.0f, -1.2f), //20
    glm::vec3(0.0f, 0.0f, -1.2f), //21
    glm::vec3(1.2f, 0.0f, -1.2f), //22

    glm::vec3(-1.2f, -1.2f, -1.2f), //23
    glm::vec3(0.0f, -1.2f, -1.2f), //24
    glm::vec3(1.2f, -1.2f, -1.2f)  //25
};


//STRUCTURE FOR EACH CUBE----------------------------------------------------------------------------------------------------------------
//id, idCamada, color(6), vectorPosicion
struct SLittleCube
{
    int id;
    glm::vec3 initialPosition;
    //glm::mat4 actualPosition;
    vector<float> vertices;
    int typeOfCube; //1 -> 1 color || 2 -> 2 color || 3 -> 3 color
    vector<int> listOfColors;
    vector<char> camadas;
    SLittleCube(int _id, glm::vec3 iP, vector<float> _vertices) {
        id = _id;
        initialPosition = iP;
        vertices = _vertices;
    };
};

class CCube
{
public:
    vector<SLittleCube> littleCubes;
    //methods
    void moveR();
    void moveL();
    void moveU();
    void moveD();
    void moveF();
    void moveB();
    void moveM();
    void moveE();
    void moveS();

    CCube();
    ~CCube();

private:
    
};

CCube::CCube()
{
    
}

CCube::~CCube()
{
}

//MAIN PROGRAM------------------------------------------------------------------------------------------------------------------------------
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
    Shader ourShader("C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P1/cube.vs", "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P1/cube.fs");


    //SETUP FOR THE CUBES******************************************************************************************************
   
    glGenVertexArrays(26, VAO);
    glGenBuffers(26, VBO);
    CCube MagicCube;

    for (int i = 0; i < 26; i++) {
        vector<float> vecCube;
        copy(&vertices[0], &vertices[sizeof(vertices) / sizeof(float)], back_inserter(vecCube));
        SLittleCube cube(i, iPlittleCubes[i], vecCube);
        MagicCube.littleCubes.push_back(cube);
    }

    for (int i = 0; i < 26; i++) {
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &(MagicCube.littleCubes[i].vertices[0]), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    vector<char> camada0 = {'R', 'U', 'F'};
    MagicCube.littleCubes[0].camadas = camada0;
    vector<char> camada1 = { 'R', 'U', 'S' };
    MagicCube.littleCubes[1].camadas = camada1;
    vector<char> camada2 = { 'R', 'U', 'B' };
    MagicCube.littleCubes[2].camadas = camada2;

    vector<char> camada3 = { 'R', 'E', 'F' };
    MagicCube.littleCubes[3].camadas = camada3;
    vector<char> camada4 = { 'R', 'E', 'S' };
    MagicCube.littleCubes[4].camadas = camada4;
    vector<char> camada5 = { 'R', 'E', 'B' };
    MagicCube.littleCubes[5].camadas = camada5;

    vector<char> camada6 = { 'R', 'D', 'F' };
    MagicCube.littleCubes[6].camadas = camada6;
    vector<char> camada7 = { 'R', 'D', 'S' };
    MagicCube.littleCubes[7].camadas = camada7;
    vector<char> camada8 = { 'R', 'D', 'B' };
    MagicCube.littleCubes[8].camadas = camada8;

    vector<char> camada9 = { 'M', 'U', 'F' };
    MagicCube.littleCubes[9].camadas = camada9;
    vector<char> camada10 = { 'M', 'U', 'S' };
    MagicCube.littleCubes[10].camadas = camada10;
    vector<char> camada11 = { 'M', 'U', 'B' };
    MagicCube.littleCubes[11].camadas = camada11;

    vector<char> camada12 = { 'M', 'E', 'F' };
    MagicCube.littleCubes[12].camadas = camada12;
    vector<char> camada13 = { 'M', 'E', 'B' };
    MagicCube.littleCubes[13].camadas = camada13;

    vector<char> camada14 = { 'M', 'D', 'F' };
    MagicCube.littleCubes[14].camadas = camada14;
    vector<char> camada15 = { 'M', 'D', 'S' };
    MagicCube.littleCubes[15].camadas = camada15;
    vector<char> camada16 = { 'M', 'D', 'B' };
    MagicCube.littleCubes[16].camadas = camada16;

    vector<char> camada17 = { 'L', 'U', 'F' };
    MagicCube.littleCubes[17].camadas = camada17;
    vector<char> camada18 = { 'L', 'U', 'S' };
    MagicCube.littleCubes[18].camadas = camada18;
    vector<char> camada19 = { 'L', 'U', 'B' };
    MagicCube.littleCubes[19].camadas = camada19;

    vector<char> camada20 = { 'L', 'E', 'F' };
    MagicCube.littleCubes[20].camadas = camada20;
    vector<char> camada21 = { 'L', 'E', 'S' };
    MagicCube.littleCubes[21].camadas = camada21;
    vector<char> camada22 = { 'L', 'E', 'B' };
    MagicCube.littleCubes[22].camadas = camada22;

    vector<char> camada23 = { 'L', 'D', 'F' };
    MagicCube.littleCubes[23].camadas = camada23;
    vector<char> camada24 = { 'L', 'D', 'S' };
    MagicCube.littleCubes[24].camadas = camada24;
    vector<char> camada25 = { 'L', 'D', 'B' };
    MagicCube.littleCubes[25].camadas = camada25;

    //FOR TEXTURE
    // load and create a texture 
    unsigned int texture1, texture2;

    // texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P1/texture.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P1/texture2.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    //MAIN RENDER LOOP************************************************************************************************************************
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);   //cambiar con kewWordCallBack

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

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
// Key CallBack: query GLFW whether relevant keys are pressed/released this frame and react accordingly

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 0.10f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 0.10f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.10f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::vec3(0.10f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 0.0f, 0.10f);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 0.0f, 0.10f);
    //global coordenates
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraFront += glm::vec3(0.0f, 0.10f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraFront -= glm::vec3(0.0f, 0.10f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraFront -= glm::vec3(0.10f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraFront += glm::vec3(0.10f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        cameraFront -= glm::vec3(0.0f, 0.0f, 0.10f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        cameraFront += glm::vec3(0.0f, 0.0f, 0.10f);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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