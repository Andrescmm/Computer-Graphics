//ANDRES CUSIRRAMOS & ANTHONY FERNANDEZ
//Computacion Grafica - CCOMP7-1
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P3/headers/shader_m.h"
#include "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P3/headers/cube_structure.h"
#include "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P3/Solver/solve.h"
#include "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P3/Solver/random.h"

#include <vector>
#include <io.h>
#include <ctime>
#include <stdio.h>
#include <windows.h>
#include <queue>
using namespace std;


//GLOBAL VARIABLES------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool setAnimacion(string movimiento_);
void solvecube(string moves);
   

//Solver

bool Animacion = true;
string AnimacionActual;
int ContadorDeAnimacion;
int TiempoDeAnimacion;
vector<string> TempMovimientos;
queue<string> ColaDesordenada;
queue<string> ColaDeSolucion;


// Velocidad


//Mouse
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 8.0f;	// time between current frame and last frame
float lastFrame = 8.0f;

// Camadas
bool condR = false;
bool condL = false;
bool condU = false;
bool condD = false;
bool condF = false;
bool condB = false;

float angle;

vector<string> cubo;

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

//for walk around
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 7.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

///MAIN PROGRAM------------------------------------------------------------------------------------------------------------------------------
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Cubo Rubik", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
   

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // COMPILE SHADER PROGRAM
    Shader ourShader("C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P3/shaders/cube.vs", "C:/Users/Carlos C/Downloads/Laboratorio_CG/GLFW_GLAD_GLUT_GLEW_cmake_project/src/TF_cubo_P3/shaders/cube.fs");

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
    //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);

    

    

    //MAIN RENDER LOOP************************************************************************************************************************
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.96f, 0.97f, 0.91f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        //camera/view transformation
        //glm::mat4 view = glm::mat4(1.0f);
        // view = glm::lookAt(cameraPos, cameraFront, cameraUp);
        //ourShader.setMat4("view", view);
        glm::mat4 view = glm::lookAt(cameraPos, cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        

       

        // render little cubes
        for (unsigned int i = 0; i < 26; i++)
        {
            glBindVertexArray(VAO[i]);
            glm::mat4 model = glm::mat4(1.0f);
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //movements for the MagicCube
        if (condR) {
            if (angle < 90) {
                MagicCube.moveR(-1);
                angle++;
            }
            else {
                condR = false;
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
    // Camera -----------------------------------------------------

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 1.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos -= glm::vec3(1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos += glm::vec3(1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        cameraPos -= glm::vec3(0.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        cameraPos += glm::vec3(0.0f, 0.0f, 1.0f);


    ;
    //MOVEMENTS----------------------------------------------------
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        
        
        condL = true;
        cubo.push_back("R");
        
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        condR = true;
        cubo.push_back("L");
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        condU = true;
        cubo.push_back("U");
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        condD = true;
        cubo.push_back("D");
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        condB = true;
        cubo.push_back("F");
        
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        condF = true;
        cubo.push_back("B");
        
    }

    // Solution
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {


        //print moves
        cout << "Cubo" << endl;
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
        cout << "Solution " << endl;

        for (int i = 0; i < moves.size(); i++) {
            if (i == 0) {
                cout << moves[i];
                ColaDeSolucion.push(moves[i]);
            }
            else {
                cout << " ," << moves[i];
                ColaDeSolucion.push(moves[i]);
            }
        }

        cout << endl;

        cubo = vector<string>();
    }



    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
       // while (Animacion) {
            if (!ColaDeSolucion.empty()) {
                //if (!Animacion) {
                    AnimacionActual = ColaDeSolucion.front();
                    cout <<"ANI -> " << AnimacionActual << endl;
                    solvecube(AnimacionActual);
                    ColaDeSolucion.pop();
               // }
            }
            //else {
                //Animacion = false;
            //}
        //}
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



bool setAnimacion(string movimiento_) {
    if (!Animacion) {
        AnimacionActual = movimiento_;
        Animacion = true;
        ContadorDeAnimacion = 0;
        if (AnimacionActual.length() == 2 && AnimacionActual[1] == '2')
            TiempoDeAnimacion = 60;
        else
            TiempoDeAnimacion = 30;
        return true;
    }
    return false;
}


void solvecube(string moves) {
    // for (int i = 0; i < moves.size(); i++) {

    if (moves == "R") {
        cout << "R";
        condL = true;


    }
    if (moves == "L") {
        cout << "L";
        condR = true;

    }
    if (moves == "U") {
        cout << "U";
        condU = true;


    }
    if (moves == "D") {
        cout << "D";
        condD = true;

    }
    if (moves == "F") {
        cout << "F";
        condB = true;


    }
    if (moves == "B") {
        cout << "B";
        condF = true;


    }
    if (moves == "R2") {
        cout << "R2";
        
         condL = true;
         if (condF = false)
             condF = true;

      

    }
    if (moves == "L2") {
        cout << "L2";
  
            condR = true;
            if (condR = false)
                condR = true;


    }
    if (moves == "U2") {
        cout << "U2";
     
        condU = true;
        if (condU = false)
            condU = true;

       
    }
    if (moves == "D2") {
        cout << "D2";
        condD = true;
        if (condD = false)
            condD = true;
    }

    if (moves == "F2") {
            cout << "F2";
          

            condB = true;
            if (condB = false)
                condB = true;


     }
    if (moves == "B2") {
            cout << "B2";
           
           condF = true;
           if (condF = false)
                 condF = true;

      }
    if (moves == "R\'") {
            cout << "R\'";
            for (int i = 0; i < 3; i++) {

                condL = true;

            }
       }
     if (moves == "L\'") {
            cout << "L\'";
            for (int i = 0; i < 3; i++) {

                condR = true;

            }
        }
        if (moves == "U\'") {
            cout << "U\'";
            for (int i = 0; i < 3; i++) {

                condU = true;

            }
        }
        if (moves == "D\'") {
            cout << "D\'";
            for (int i = 0; i < 3; i++) {

                condD = true;

            }
        }
        if (moves == "F\'") {
            cout << "F\'";
            for (int i = 0; i < 3; i++) {

                condB = true;

            }
        }

        if (moves == "B\'") {
            cout << "B\'";
            for (int i = 0; i < 3; i++) {

                condF = true;

            }
        }
        //}
    //}
}