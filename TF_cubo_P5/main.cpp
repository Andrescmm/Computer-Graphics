//ANDRES CUSIRRAMOS & ANTHONY FERNANDEZ
//Computacion Grafica - CCOMP7-1
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "../../src/TF_cubo_P4/headers/shader_m.h"
#include "../../src/TF_cubo_P4/headers/cube_structure.h"
#include "../../src/TF_cubo_P4/Solver/solve.h"
#include "../../src/TF_cubo_P4/Solver/random.h"

#include <vector>
#include <stb_image.h>
#include <random>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>       
using namespace std;

//GLOBAL VARIABLES------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool condR = false;
bool condL = false;
bool condU = false;
bool condD = false;
bool condF = false;
bool condB = false;
void solvecube(vector<string> moves);
void randomMoves(vector<string>&cubo);
template<typename T>
T random(std::vector<T> const& v);

vector<char> movementsList;
float angle;
vector<string> cubo;
vector<string> cubitoR;
vector<string> movesR;
bool randomSolu = false;



// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

//for walk around
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 7.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//MAIN PROGRAM------------------------------------------------------------------------------------------------------------------------------
int main(){

    //cout << "---------------------------------------------------------------" << endl;
    cout << "---------------------Computacion Grafica-----------------------" << endl;
    cout << "---------------------------CCOMP7-1----------------------------" << endl;
    cout << "-------------ANDRES CUSIRRAMOS & ANTHONY FERNANDEZ-------------" << endl;
    cout << "---------------------------------------------------------------" << endl;
    cout << "---------------------TECLA R: Movimiento R --------------------" << endl;
    cout << "---------------------TECLA L: Movimiento L --------------------" << endl;
    cout << "---------------------TECLA U: Movimiento U --------------------" << endl;
    cout << "---------------------TECLA D: Movimiento D --------------------" << endl;
    cout << "---------------------TECLA B: Movimiento B --------------------" << endl;
    cout << "---------------------TECLA F: Movimiento F --------------------" << endl;
    cout << "-----------TECLA A: SE DESARMARA EL CUBO ALEATORIAMENTE--------" << endl;
    cout << "-------------------TECLA S: SE ORDENARA EL CUBO----------------" << endl;
   // cout << "---------------------------------------------------------------" << endl;

   
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cubito", NULL, NULL);
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
    Shader ourShader("C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P4/shaders/cube.vs", "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P4/shaders/cube.fs");

    //SETUP FOR THE CUBES******************************************************************************************************
    unsigned int VBO[26], VAO[26];
    glGenVertexArrays(26, VAO);
    glGenBuffers(26, VBO);
    //creation
    CCube MagicCube;
    MagicCube.createLittleCubes();

    //sent to buffer
    MagicCube.drawCube(&VBO[0], &VAO[0]);

    //upload texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P4/textures/completeTexture.jpg", &width, &height, &nrChannels, 0);
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
    ourShader.setInt("ourTexture", 0);

    // activate shader
    ourShader.use();

    // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);


    //std::vector<std::string> cubitoR;

   // cout << "random init" << endl;
    //randomMoves(cubitoR);

  

      

    //MAIN RENDER LOOP************************************************************************************************************************
    while (!glfwWindowShouldClose(window))
    {
        //draw cubes+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // input
        processInput(window);

        // render
        glClearColor(0.96f, 0.97f, 0.91f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // activate shader
        ourShader.use();

        // camera/view transformation
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // render little cubes
        for (unsigned int i = 0; i < 26; i++)
        {
            glBindVertexArray(VAO[i]);
            glm::mat4 model = glm::mat4(1.0f);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //movements for the MagicCube++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (!movementsList.empty()) {
            //read actual movement
            if (movementsList[0] == 'R') {
                condR = true;
                condL = condF = condB = condU = condD = false;
            }
            if (movementsList[0] == 'L') {
                condL = true;
                condR = condF = condB = condU = condD = false;
            }
            if (movementsList[0] == 'F') {
                condF = true;
                condL = condR = condB = condU = condD = false;
            }
            if (movementsList[0] == 'B') {
                condB = true;
                condL = condF = condR = condU = condD = false;
            }
            if (movementsList[0] == 'U') {
                condU = true;
                condL = condF = condB = condR = condD = false;
            }
            if (movementsList[0] == 'D') {
                condD = true;
                condL = condF = condB = condU = condR = false;
            }
            //make the move
            if (condR) {
                if (angle < 90) {
                    MagicCube.moveR(-1);
                    angle++;
                }
                else {
                    condR = false;
                    movementsList.erase(movementsList.begin());
                    angle = 0;
                    //update groups
                    /*
                    * RUF -> RBU
                    * RUS -> RBE
                    * RUB -> RBD
                    * REF -> RSU
                    * RES -> RSE*
                    * REB -> RSD
                    * RDF -> RFU
                    * RDS -> RFE
                    * RDB -> RFD
                    */
                    bool searchR = false;
                    for (unsigned int indexCube = 0; indexCube < 26; indexCube++)
                    {
                        //R
                        for (int i = 0; i < 3; i++) {
                            if (MagicCube.littleCubes[indexCube].camadas[i] == 'R') {
                                searchR = true;
                                break;
                            }
                        }
                        if (searchR) {
                            //U
                            bool searchU = false;
                            for (int j = 0; j < 3; j++) {
                                if (MagicCube.littleCubes[indexCube].camadas[j] == 'U') {
                                    searchU = true;
                                    break;
                                }
                            }
                            if (searchU) {
                                bool searchF = false;
                                bool searchS = false;
                                bool searchB = false;
                                //FSB
                                for (int k = 0; k < 3; k++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                        searchF = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                        searchS = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                        searchB = true;
                                }
                                if (searchF) {
                                    vector<char> camadaTemp = { 'R', 'B', 'U' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    //cout << "RUF -> RBU" << endl;
                                }
                                if (searchS) {
                                    vector<char> camadaTemp = { 'R', 'B', 'E' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    //cout << "RUS -> RBE" << endl;
                                }
                                if (searchB) {
                                    vector<char> camadaTemp = { 'R', 'B', 'D' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    //cout << "RUB -> RBD" << endl;
                                }
                                searchU = false;
                            }
                            else {
                                //E
                                bool searchE = false;
                                for (int j = 0; j < 3; j++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[j] == 'E') {
                                        searchE = true;
                                        break;
                                    }
                                }
                                if (searchE) {
                                    bool searchF = false;
                                    bool searchS = false;
                                    bool searchB = false;
                                    //FSB
                                    for (int k = 0; k < 3; k++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                            searchF = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                            searchS = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                            searchB = true;
                                    }
                                    if (searchF) {
                                        vector<char> camadaTemp = { 'R', 'S', 'U' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        //cout << "REF -> RSU" << endl;
                                    }
                                    if (searchS) {
                                        vector<char> camadaTemp = { 'R', 'S', 'E' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        //cout << "RES -> RSE" << endl;
                                    }
                                    if (searchB) {
                                        vector<char> camadaTemp = { 'R', 'S', 'D' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        //cout << "REB -> RSD" << endl;
                                    }
                                    searchE = false;
                                }
                                else {
                                    //D
                                    bool searchD = false;
                                    for (int j = 0; j < 3; j++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[j] == 'D') {
                                            searchD = true;
                                            break;
                                        }
                                    }
                                    if (searchD) {
                                        bool searchF = false;
                                        bool searchS = false;
                                        bool searchB = false;
                                        //FSB
                                        for (int k = 0; k < 3; k++) {
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                                searchF = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                                searchS = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                                searchB = true;
                                        }
                                        if (searchF) {
                                            vector<char> camadaTemp = { 'R', 'F', 'U' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                            //cout << "RDF -> RFU" << endl;
                                        }
                                        if (searchS) {
                                            vector<char> camadaTemp = { 'R', 'F', 'E' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                            //cout << "RDS -> RFE" << endl;
                                        }
                                        if (searchB) {
                                            vector<char> camadaTemp = { 'R', 'F', 'D' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                            //cout << "RDB -> RFD" << endl;
                                        }
                                        searchD = false;
                                    }
                                }
                            }
                            searchR = false;
                        }
                    }
                }
            }
            if (condL) {
                if (angle < 90) {
                    MagicCube.moveL(1);
                    angle++;
                }
                else {
                    condL = false;
                    movementsList.erase(movementsList.begin());
                    angle = 0;
                    //update groups
                    /*
                    * LUF -> LFD
                    * LUS -> LFE
                    * LUB -> LFU
                    * LEF -> LSD
                    * LES -> LSE
                    * LEB -> LSU
                    * LDF -> LBD
                    * LDS -> LBE
                    * LDB -> LBU
                    */
                    bool searchL = false;
                    for (unsigned int indexCube = 0; indexCube < 26; indexCube++)
                    {
                        //L
                        for (int i = 0; i < 3; i++) {
                            if (MagicCube.littleCubes[indexCube].camadas[i] == 'L') {
                                searchL = true;
                                break;
                            }
                        }
                        if (searchL) {
                            //U
                            bool searchU = false;
                            for (int j = 0; j < 3; j++) {
                                if (MagicCube.littleCubes[indexCube].camadas[j] == 'U') {
                                    searchU = true;
                                    break;
                                }
                            }
                            if (searchU) {
                                bool searchF = false;
                                bool searchS = false;
                                bool searchB = false;
                                //FSB
                                for (int k = 0; k < 3; k++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                        searchF = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                        searchS = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                        searchB = true;
                                }
                                if (searchF) {
                                    vector<char> camadaTemp = { 'L', 'F', 'D' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchS) {
                                    vector<char> camadaTemp = { 'L', 'F', 'E' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchB) {
                                    vector<char> camadaTemp = { 'L', 'F', 'U' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                searchU = false;
                            }
                            else {
                                //E
                                bool searchE = false;
                                for (int j = 0; j < 3; j++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[j] == 'E') {
                                        searchE = true;
                                        break;
                                    }
                                }
                                if (searchE) {
                                    bool searchF = false;
                                    bool searchS = false;
                                    bool searchB = false;
                                    //FSB
                                    for (int k = 0; k < 3; k++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                            searchF = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                            searchS = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                            searchB = true;
                                    }
                                    if (searchF) {
                                        vector<char> camadaTemp = { 'L', 'S', 'D' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchS) {
                                        vector<char> camadaTemp = { 'L', 'S', 'E' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchB) {
                                        vector<char> camadaTemp = { 'L', 'S', 'U' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    searchE = false;
                                }
                                else {
                                    //D
                                    bool searchD = false;
                                    for (int j = 0; j < 3; j++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[j] == 'D') {
                                            searchD = true;
                                            break;
                                        }
                                    }
                                    if (searchD) {
                                        bool searchF = false;
                                        bool searchS = false;
                                        bool searchB = false;
                                        //FSB
                                        for (int k = 0; k < 3; k++) {
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                                searchF = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                                searchS = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                                searchB = true;
                                        }
                                        if (searchF) {
                                            vector<char> camadaTemp = { 'L', 'B', 'D' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchS) {
                                            vector<char> camadaTemp = { 'L', 'B', 'E' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchB) {
                                            vector<char> camadaTemp = { 'L', 'B', 'U' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        searchD = false;
                                    }
                                }
                            }
                            searchL = false;
                        }
                    }
                }
            }
            if (condU) {
                if (angle < 90) {
                    MagicCube.moveU(-1);
                    angle++;
                }
                else {
                    condU = false;
                    movementsList.erase(movementsList.begin());
                    angle = 0;
                    //update groups
                    /*
                    * URF -> UFL
                    * URS -> UFM
                    * URB -> UFR
                    * UMF -> USL
                    * UMS -> USM
                    * UMB -> USR
                    * ULF -> UBL
                    * ULS -> UBM
                    * ULB -> UBR
                    */
                    bool searchU = false;
                    for (unsigned int indexCube = 0; indexCube < 26; indexCube++)
                    {
                        //U
                        for (int i = 0; i < 3; i++) {
                            if (MagicCube.littleCubes[indexCube].camadas[i] == 'U') {
                                searchU = true;
                                break;
                            }
                        }
                        if (searchU) {
                            //R
                            bool searchR = false;
                            for (int j = 0; j < 3; j++) {
                                if (MagicCube.littleCubes[indexCube].camadas[j] == 'R') {
                                    searchR = true;
                                    break;
                                }
                            }
                            if (searchR) {
                                bool searchF = false;
                                bool searchS = false;
                                bool searchB = false;
                                //FSB
                                for (int k = 0; k < 3; k++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                        searchF = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                        searchS = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                        searchB = true;
                                }
                                if (searchF) {
                                    vector<char> camadaTemp = { 'U', 'F', 'L' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchS) {
                                    vector<char> camadaTemp = { 'U', 'F', 'M' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchB) {
                                    vector<char> camadaTemp = { 'U', 'F', 'R' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                searchR = false;
                            }
                            else {
                                //M
                                bool searchM = false;
                                for (int j = 0; j < 3; j++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[j] == 'M') {
                                        searchM = true;
                                        break;
                                    }
                                }
                                if (searchM) {
                                    bool searchF = false;
                                    bool searchS = false;
                                    bool searchB = false;
                                    //FSB
                                    for (int k = 0; k < 3; k++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                            searchF = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                            searchS = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                            searchB = true;
                                    }
                                    if (searchF) {
                                        vector<char> camadaTemp = { 'U', 'S', 'L' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchS) {
                                        vector<char> camadaTemp = { 'U', 'S', 'M' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchB) {
                                        vector<char> camadaTemp = { 'U', 'S', 'R' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    searchM = false;
                                }
                                else {
                                    //L
                                    bool searchL = false;
                                    for (int j = 0; j < 3; j++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[j] == 'L') {
                                            searchL = true;
                                            break;
                                        }
                                    }
                                    if (searchL) {
                                        bool searchF = false;
                                        bool searchS = false;
                                        bool searchB = false;
                                        //FSB
                                        for (int k = 0; k < 3; k++) {
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                                searchF = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                                searchS = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                                searchB = true;
                                        }
                                        if (searchF) {
                                            vector<char> camadaTemp = { 'U', 'B', 'L' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchS) {
                                            vector<char> camadaTemp = { 'U', 'B', 'M' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchB) {
                                            vector<char> camadaTemp = { 'U', 'B', 'R' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        searchL = false;
                                    }
                                }
                            }
                            searchU = false;
                        }
                    }

                }
            }
            if (condD) {
                if (angle < 90) {
                    MagicCube.moveD(1);
                    angle++;
                }
                else {
                    condD = false;
                    movementsList.erase(movementsList.begin());
                    angle = 0;
                    //update groups
                    /*
                    * DRF -> DBR
                    * DRS -> DBM
                    * DRB -> DBL
                    * DMF -> DSR
                    * DMS -> DSM
                    * DMB -> DSL
                    * DLF -> DFR
                    * DLS -> DFM
                    * DLB -> DFL
                    */
                    bool searchD = false;
                    for (unsigned int indexCube = 0; indexCube < 26; indexCube++)
                    {
                        //D
                        for (int i = 0; i < 3; i++) {
                            if (MagicCube.littleCubes[indexCube].camadas[i] == 'D') {
                                searchD = true;
                                break;
                            }
                        }
                        if (searchD) {
                            //R
                            bool searchR = false;
                            for (int j = 0; j < 3; j++) {
                                if (MagicCube.littleCubes[indexCube].camadas[j] == 'R') {
                                    searchR = true;
                                    break;
                                }
                            }
                            if (searchR) {
                                bool searchF = false;
                                bool searchS = false;
                                bool searchB = false;
                                //FSB
                                for (int k = 0; k < 3; k++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                        searchF = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                        searchS = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                        searchB = true;
                                }
                                if (searchF) {
                                    vector<char> camadaTemp = { 'D', 'B', 'R' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchS) {
                                    vector<char> camadaTemp = { 'D', 'B', 'M' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchB) {
                                    vector<char> camadaTemp = { 'D', 'B', 'L' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                searchR = false;
                            }
                            else {
                                //M
                                bool searchM = false;
                                for (int j = 0; j < 3; j++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[j] == 'M') {
                                        searchM = true;
                                        break;
                                    }
                                }
                                if (searchM) {
                                    bool searchF = false;
                                    bool searchS = false;
                                    bool searchB = false;
                                    //FSB
                                    for (int k = 0; k < 3; k++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                            searchF = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                            searchS = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                            searchB = true;
                                    }
                                    if (searchF) {
                                        vector<char> camadaTemp = { 'D', 'S', 'R' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchS) {
                                        vector<char> camadaTemp = { 'D', 'S', 'M' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchB) {
                                        vector<char> camadaTemp = { 'D', 'S', 'L' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    searchM = false;
                                }
                                else {
                                    //L
                                    bool searchL = false;
                                    for (int j = 0; j < 3; j++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[j] == 'L') {
                                            searchL = true;
                                            break;
                                        }
                                    }
                                    if (searchL) {
                                        bool searchF = false;
                                        bool searchS = false;
                                        bool searchB = false;
                                        //FSB
                                        for (int k = 0; k < 3; k++) {
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'F')
                                                searchF = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'S')
                                                searchS = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'B')
                                                searchB = true;
                                        }
                                        if (searchF) {
                                            vector<char> camadaTemp = { 'D', 'F', 'R' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchS) {
                                            vector<char> camadaTemp = { 'D', 'F', 'M' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchB) {
                                            vector<char> camadaTemp = { 'D', 'F', 'L' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        searchL = false;
                                    }
                                }
                            }
                            searchD = false;
                        }
                    }

                }
            }
            if (condF) {
                if (angle < 90) {
                    MagicCube.moveF(1);
                    angle++;
                }
                else {
                    condF = false;
                    movementsList.erase(movementsList.begin());
                    angle = 0;
                    //update groups
                    /*
                    * FRU -> FDR
                    * FRE -> FDM
                    * FRD -> FDL
                    * FMU -> FER
                    * FME -> FEM
                    * FMD -> FEL
                    * FLU -> FUR
                    * FLE -> FUM
                    * FLD -> FUL
                    */
                    bool searchF = false;
                    for (unsigned int indexCube = 0; indexCube < 26; indexCube++)
                    {
                        //F
                        for (int i = 0; i < 3; i++) {
                            if (MagicCube.littleCubes[indexCube].camadas[i] == 'F') {
                                searchF = true;
                                break;
                            }
                        }
                        if (searchF) {
                            //R
                            bool searchR = false;
                            for (int j = 0; j < 3; j++) {
                                if (MagicCube.littleCubes[indexCube].camadas[j] == 'R') {
                                    searchR = true;
                                    break;
                                }
                            }
                            if (searchR) {
                                bool searchU = false;
                                bool searchE = false;
                                bool searchD = false;
                                //UED
                                for (int k = 0; k < 3; k++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'U')
                                        searchU = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'E')
                                        searchE = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'D')
                                        searchD = true;
                                }
                                if (searchU) {
                                    vector<char> camadaTemp = { 'F', 'D', 'R' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchE) {
                                    vector<char> camadaTemp = { 'F', 'D', 'M' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchD) {
                                    vector<char> camadaTemp = { 'F', 'D', 'L' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                searchR = false;
                            }
                            else {
                                //M
                                bool searchM = false;
                                for (int j = 0; j < 3; j++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[j] == 'M') {
                                        searchM = true;
                                        break;
                                    }
                                }
                                if (searchM) {
                                    bool searchU = false;
                                    bool searchE = false;
                                    bool searchD = false;
                                    //UED
                                    for (int k = 0; k < 3; k++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'U')
                                            searchU = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'E')
                                            searchE = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'D')
                                            searchD = true;
                                    }
                                    if (searchU) {
                                        vector<char> camadaTemp = { 'F', 'E', 'R' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchE) {
                                        vector<char> camadaTemp = { 'F', 'E', 'M' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchD) {
                                        vector<char> camadaTemp = { 'F', 'E', 'L' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    searchM = false;
                                }
                                else {
                                    //L
                                    bool searchL = false;
                                    for (int j = 0; j < 3; j++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[j] == 'L') {
                                            searchL = true;
                                            break;
                                        }
                                    }
                                    if (searchL) {
                                        bool searchU = false;
                                        bool searchE = false;
                                        bool searchD = false;
                                        //UED
                                        for (int k = 0; k < 3; k++) {
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'U')
                                                searchU = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'E')
                                                searchE = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'D')
                                                searchD = true;
                                        }
                                        if (searchU) {
                                            vector<char> camadaTemp = { 'F', 'U', 'R' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchE) {
                                            vector<char> camadaTemp = { 'F', 'U', 'M' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchD) {
                                            vector<char> camadaTemp = { 'F', 'U', 'L' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        searchL = false;
                                    }
                                }
                            }
                            searchF = false;
                        }
                    }
                }
            }
            if (condB) {
                if (angle < 90) {
                    MagicCube.moveB(-1);
                    angle++;
                }
                else {
                    condB = false;
                    movementsList.erase(movementsList.begin());
                    angle = 0;
                    //update groups
                    /*
                    * BRU -> BUL
                    * BRE -> BUM
                    * BRD -> BUR
                    * BMU -> BEL
                    * BME -> BEM
                    * BMD -> BER
                    * BLU -> BDL
                    * BLE -> BDM
                    * BLD -> BDR
                    */
                    bool searchB = false;
                    for (unsigned int indexCube = 0; indexCube < 26; indexCube++)
                    {
                        //B
                        for (int i = 0; i < 3; i++) {
                            if (MagicCube.littleCubes[indexCube].camadas[i] == 'B') {
                                searchB = true;
                                break;
                            }
                        }
                        if (searchB) {
                            //R
                            bool searchR = false;
                            for (int j = 0; j < 3; j++) {
                                if (MagicCube.littleCubes[indexCube].camadas[j] == 'R') {
                                    searchR = true;
                                    break;
                                }
                            }
                            if (searchR) {
                                bool searchU = false;
                                bool searchE = false;
                                bool searchD = false;
                                //UED
                                for (int k = 0; k < 3; k++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'U')
                                        searchU = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'E')
                                        searchE = true;
                                    if (MagicCube.littleCubes[indexCube].camadas[k] == 'D')
                                        searchD = true;
                                }
                                if (searchU) {
                                    vector<char> camadaTemp = { 'B', 'U', 'L' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchE) {
                                    vector<char> camadaTemp = { 'B', 'U', 'M' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                if (searchD) {
                                    vector<char> camadaTemp = { 'B', 'U', 'R' };
                                    MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                }
                                searchR = false;
                            }
                            else {
                                //M
                                bool searchM = false;
                                for (int j = 0; j < 3; j++) {
                                    if (MagicCube.littleCubes[indexCube].camadas[j] == 'M') {
                                        searchM = true;
                                        break;
                                    }
                                }
                                if (searchM) {
                                    bool searchU = false;
                                    bool searchE = false;
                                    bool searchD = false;
                                    //UED
                                    for (int k = 0; k < 3; k++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'U')
                                            searchU = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'E')
                                            searchE = true;
                                        if (MagicCube.littleCubes[indexCube].camadas[k] == 'D')
                                            searchD = true;
                                    }
                                    if (searchU) {
                                        vector<char> camadaTemp = { 'B', 'E', 'L' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchE) {
                                        vector<char> camadaTemp = { 'B', 'E', 'M' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    if (searchD) {
                                        vector<char> camadaTemp = { 'B', 'E', 'R' };
                                        MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                    }
                                    searchM = false;
                                }
                                else {
                                    //L
                                    bool searchL = false;
                                    for (int j = 0; j < 3; j++) {
                                        if (MagicCube.littleCubes[indexCube].camadas[j] == 'L') {
                                            searchL = true;
                                            break;
                                        }
                                    }
                                    if (searchL) {
                                        bool searchU = false;
                                        bool searchE = false;
                                        bool searchD = false;
                                        //UED
                                        for (int k = 0; k < 3; k++) {
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'U')
                                                searchU = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'E')
                                                searchE = true;
                                            if (MagicCube.littleCubes[indexCube].camadas[k] == 'D')
                                                searchD = true;
                                        }
                                        if (searchU) {
                                            vector<char> camadaTemp = { 'B', 'D', 'L' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchE) {
                                            vector<char> camadaTemp = { 'B', 'D', 'M' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        if (searchD) {
                                            vector<char> camadaTemp = { 'B', 'D', 'R' };
                                            MagicCube.littleCubes[indexCube].camadas = camadaTemp;
                                        }
                                        searchL = false;
                                    }
                                }
                            }
                            searchB = false;
                        }
                    }

                }
            }
        }
             

        //update values for the cube
        MagicCube.drawCube(&VBO[0], &VAO[0]);

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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 0.1f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos += glm::vec3(0.1f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 0.0f, 0.1f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 0.0f, 0.1f);
    
    //MOVEMENTS----------------------------------------------------
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cubo.push_back("R");
        movementsList.push_back('R');
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        cubo.push_back("L");
        movementsList.push_back('L');
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        cubo.push_back("U");
        movementsList.push_back('U');
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cubo.push_back("D");
        movementsList.push_back('D');
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        cubo.push_back("F");
        movementsList.push_back('F');
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        cubo.push_back("B");
        movementsList.push_back('B');
    }
    //solver
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //print moves

        if (!randomSolu) {
            cout << endl;
            cout << "Moves -> ";
            for (int i = 0; i < cubo.size(); i++) {
                if (i == 0) {
                    cout << cubo[i];
                }
                else {
                    cout << " ," << cubo[i];
                }
            }
            cout << endl;

            // Print solution
            std::vector<std::string> moves = get_solution(to_cube_not(cubo));
            cout << "Solution -> ";

            for (int i = 0; i < moves.size(); i++) {
                if (i == 0) {
                    cout << moves[i];
                }
                else {
                    cout << " ," << moves[i];
                }
            }
            solvecube(moves);
            cout << endl;

        }
        else{
            randomSolu = false;
            movesR = get_solution(to_cube_not(cubitoR));
            cout << "Solution -> ";
            for (int i = 0; i < movesR.size(); i++) {
                if (i == 0) {
                    cout << movesR[i];
                }
                else {
                    cout << " ," << movesR[i];
                }
            }
            solvecube(movesR);
        }
        cubo = vector<string>();
        cubitoR = vector<string>();
        movesR = vector<string>();
    }


    //random
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        randomSolu = true;

        randomMoves(cubitoR);
        cout << endl;
        cout << "Random Moves -> ";
        for (int i = 0; i < cubitoR.size(); i++) {
            if (i == 0) {
                cout << cubitoR[i];
            }
            else {
                cout << " ," << cubitoR[i];
            }
        }
        solvecube(cubitoR);
        cout << endl;
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



template<typename T>
T random(std::vector<T> const& v)
{
    int r = rand() % v.size();
    return v[r];
}


void randomMoves(vector<string>&cubo) {

    vector<string> cuboRandom = {"R", "R" ,"L","U","D","F","B","R2","L2","U2","D2","F2","B2","R\'","L\'" ,"U\'" ,"D\'" ,  "F\'","B\'"};

    int randNum = rand() % cuboRandom.size();
    for (int i = 0; i < 20; i++) {
        cubo.push_back(random(cuboRandom));
    }


}


void solvecube(vector<string> moves) {
    for (int i = 0; i < moves.size(); i++) {
        if (moves[i] == "R") {
            movementsList.push_back('R');
        }
        if (moves[i] == "L") {
            movementsList.push_back('L');
        }
        if (moves[i] == "U") {
            movementsList.push_back('U');
        }
        if (moves[i] == "D") {
            movementsList.push_back('D');
        }
        if (moves[i] == "F") {
            movementsList.push_back('F');
        }
        if (moves[i] == "B") {
            movementsList.push_back('B');
        }
        if (moves[i] == "R2") {
            movementsList.push_back('R');
            movementsList.push_back('R');
        }
        if (moves[i] == "L2") {
            movementsList.push_back('L');
            movementsList.push_back('L');
        }
        if (moves[i] == "U2") {
            movementsList.push_back('U');
            movementsList.push_back('U');
        }
        if (moves[i] == "D2") {
            movementsList.push_back('D');
            movementsList.push_back('D');
        }
        if (moves[i] == "F2") {
            movementsList.push_back('F');
            movementsList.push_back('F');
        }
        if (moves[i] == "B2") {
            movementsList.push_back('B');
            movementsList.push_back('B');
        }
        if (moves[i] == "R\'") {
            movementsList.push_back('R');
            movementsList.push_back('R');
            movementsList.push_back('R');
        }
        if (moves[i] == "L\'") {
            movementsList.push_back('L');
            movementsList.push_back('L');
            movementsList.push_back('L');
        }
        if (moves[i] == "U\'") {
            movementsList.push_back('U');
            movementsList.push_back('U');
            movementsList.push_back('U');
        }
        if (moves[i] == "D\'") {
            movementsList.push_back('D');
            movementsList.push_back('D');
            movementsList.push_back('D');
        }
        if (moves[i] == "F\'") {
            movementsList.push_back('F');
            movementsList.push_back('F');
            movementsList.push_back('F');
        }
        if (moves[i] == "B\'") {
            movementsList.push_back('B');
            movementsList.push_back('B');
            movementsList.push_back('B');
        }
    }
}