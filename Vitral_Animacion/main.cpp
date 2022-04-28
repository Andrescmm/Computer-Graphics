// Computacion Grafica : CCOMP 7-1
// Andres Cusirramos Marquez Mares
// 21/04/2022


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

unsigned int VBO[15], VAO[15];
float x = 0.0f;
float y = 0.0f;
char action = 'A';

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);


void transM(vector<float>& figure, float x, float y) {
    glm::mat3 translate(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(x, y, 1));

    for (int i = 0; i < figure.size(); i += 3) {
        glm::vec3 temp(figure[i], figure[i + 1], 1);
        temp = translate * temp;

        figure[i] = temp[0];
        figure[i + 1] = temp[1];
    }
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

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

const char* fragmentShaderBlue =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4( 0.12f, 0.38f, 0.85f, 1.f);\n"
"}\n";

const char* fragmentShaderBlack =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
"}\n";

const char* fragmentShaderGreen =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(0.16f, 0.46f, 0.19f, 1.0f);\n"
"}\n";

const char* fragmentShaderWhite =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(0.52f, 0.76f, 0.71f, 1.0f);\n"
"}\n";

const char* fragmentShaderBrown =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(0.61f, 0.37f, 0.21f, 1.0f);\n"
"}\n";


unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int fragmentBlue;
unsigned int fragmentBlack;
unsigned int fragmentGreen;
unsigned int fragmentWhite;
unsigned int fragmentBrown;
unsigned int shaderProgram;
unsigned int shaderBlue;
unsigned int shaderBlack;
unsigned int shaderGreen;
unsigned int shaderWhite;
unsigned int shaderBrown;

// Vertices Marco
vector<float>  verticesMarco = {
  -0.8f, 0.9f, 0.0f,  // A
   0.8f, 0.9f, 0.0f,  // B
   0.8f, -0.9f, 0.0f, // C

  -0.8f,  0.9f, 0.0f, // A
  -0.8f, -0.9f, 0.0f, // D
   0.8f, -0.9f, 0.0f // C
};

// Vertices Fondo
vector<float>  verticesFondo = {
   -0.78f, 0.88f, 0.0f, // A
   0.78f, 0.88f, 0.0f,  // B
   0.78f, -0.88f, 0.0f, // C

  -0.78f, 0.88f, 0.0f,  // A
  -0.78f, -0.88f, 0.0f, // D
   0.78f, -0.88f, 0.0f, // C
};

// Vertices Tallo
vector<float>  verticesTallo = {
     0.03f,-0.86f,0.0f,
    -0.05f,-0.86f,0.0f,
    -0.04f,-0.12f,0.0f,

     0.03f,-0.12f,0.0f,
    -0.04f,-0.12f,0.0f,
     0.03f,-0.86f,0.0f
};

// Vertices Borde Central
vector<float>  verticesCentralD = {
    // Rigth
     0.76f, 0.21f, 0.0f,
     0.68f, 0.12f, 0.0f,
     0.57f, -0.04f, 0.0f,
     0.36f, -0.35f, 0.0f,
     0.2f,  -0.6f, 0.0f,
     0.1f,  -0.77f, 0.0f,
     0.05f, -0.86f, 0.0f
};
vector<float> verticesCentralI = {
    //Left
   -0.07f, -0.83f, 0.0f,
   -0.23f, -0.63f, 0.0f,
   -0.4f,  -0.4f, 0.0f,
   -0.51f, -0.2f, 0.0f,
   -0.65f,  0.0f, 0.0f,
   -0.76f,  0.18f, 0.0f
};

// Vertices Hojas
vector<float>  verticesHojasI = {
    //Left
   -0.77f, 0.24f, 0.0f,
   -0.44f, 0.11f, 0.0f,
   -0.21f, -0.12f, 0.0f,
   -0.13f, -0.35f, 0.0f,
   -0.07f, -0.63f, 0.0f, // I1
   -0.06f, -0.87f, 0.0f,
   -0.29f, -0.76f, 0.0f,
   -0.55f, -0.53f, 0.0f,
   -0.74f, -0.16f, 0.0
};
vector<float> verticesHojasD = {
    //Rigth
    0.76f, 0.22f, 0.0f,
    0.41f, 0.07f, 0.0f,
    0.21f, -0.13f, 0.0f,
    0.11f, -0.35f, 0.0f,
    0.05f, -0.63f, 0.0f, // I1
    0.03f, -0.86f, 0.0f,
    0.15f, -0.82f, 0.0f,
    0.27f, -0.75f, 0.0f,
    0.49f, -0.56f, 0.0f,
    0.71f, -0.19f, 0.0
};


//Vertices Petalos
vector<float>  verticesPetaloI = {
    //Left
    -0.39f,  0.81f, 0.0f,  //K2
    -0.41f,  0.63f, 0.0f,  //M2
    -0.45f,  0.43f, 0.0f,  //L2
    -0.44f,  0.21f, 0.0f,  //N2
    -0.34f,  0.03f, 0.0f,  //O2
    -0.2f,  -0.07f, 0.0f,  //Q2
    -0.01f, -0.12f, 0.0f,  //L1
     0.0f,   0.0f,  0.0f,  //S2
     0.0f,   0.15f, 0.0f,  //R2
    -0.01f,  0.37f, 0.0f,  //I2
    -0.15f,  0.58f, 0.0f,  //J2
};
vector<float>  verticesPetaloD = {
    //Rigth
     0.36f,  0.81f,  0.0f,  //G2
     0.39f,  0.61f, 0.0f,   //Q1
     0.42f,  0.41f, 0.0f,   //P1
     0.41f,  0.24f, 0.0f,   //O1
     0.37f,  0.09f, 0.0f,   //N1
     0.2f,  -0.08f, 0.0f,   //M1
    -0.01f, -0.12f, 0.0f,   //L1
    -0.03f,  0.0f , 0.0f,   //T2
    -0.03f,  0.15f, 0.0f,   //U2
    -0.01f,  0.37f, 0.0f,   //I2
     0.13f,  0.6f,  0.0f    //H2
};
vector<float>  verticesPetaloC = {
    //Center
    -0.01f,  0.84f,  0.0f,  //W2
    -0.12f,  0.68f, 0.0f,   //Z2
    -0.15f,  0.58f, 0.0f,   //J2
    -0.01f,  0.37f, 0.0f,   //I2
     0.13f,  0.6f,  0.0f,   //H2
     0.09f,  0.68f, 0.0f    //A3
};

// Function to Draw
void drawFlower() {
    // Initial
    unsigned int VBO[15], VAO[15];
    glGenBuffers(15, VBO);
    glGenVertexArrays(15, VAO);

    // Vertices Marco
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesMarco.size()*sizeof(float), &verticesMarco[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Vertices Fondo
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, verticesFondo.size()*sizeof(float), &verticesFondo[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   

    // Vertices Tallo 
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, verticesTallo.size()*sizeof(float), &verticesTallo[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  

    // Vertices Hoja Cebtral Derecha
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, verticesCentralD.size()*sizeof(float), &verticesCentralD[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Hoja Central Izquierda
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, verticesCentralI.size()*sizeof(float), &verticesCentralI[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Hoja Izquierdo
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, verticesHojasI.size()*sizeof(float), &verticesHojasI[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Hoja Derecha
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, verticesHojasD.size() * sizeof(float), &verticesHojasD[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Petalo Izquierdo
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, verticesPetaloI.size()*sizeof(float), &verticesPetaloI[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Petalo Derecho
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, verticesPetaloD.size()*sizeof(float), &verticesPetaloD[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Petalo Derecho
    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, verticesPetaloC.size()*sizeof(float), &verticesPetaloC[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(shaderBlue);
    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(shaderBrown);
    glBindVertexArray(VAO[2]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[2]);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glUseProgram(shaderGreen);
    glBindVertexArray(VAO[5]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 9);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[5]);
    glDrawArrays(GL_LINE_LOOP, 0, 9);

    glUseProgram(shaderGreen);
    glBindVertexArray(VAO[6]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 10);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[6]);
    glDrawArrays(GL_LINE_LOOP, 0, 10);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[3]);
    glDrawArrays(GL_LINE_STRIP, 0, 7);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[4]);
    glDrawArrays(GL_LINE_STRIP, 0, 6);

    glUseProgram(shaderWhite);
    glBindVertexArray(VAO[7]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 11);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[7]);
    glDrawArrays(GL_LINE_LOOP, 0, 11);

    glUseProgram(shaderWhite);
    glBindVertexArray(VAO[8]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 11);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[8]);
    glDrawArrays(GL_LINE_LOOP, 0, 11);

    glUseProgram(shaderWhite);
    glBindVertexArray(VAO[9]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    glUseProgram(shaderBlack);
    glBindVertexArray(VAO[9]);
    glDrawArrays(GL_LINE_LOOP, 0, 6);

}






int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Only for Apple
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shaders
   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    fragmentBlue = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentBlue, 1, &fragmentShaderBlue, NULL);
    glCompileShader(fragmentBlue);


   fragmentBlack = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentBlack, 1, &fragmentShaderBlack, NULL);
    glCompileShader(fragmentBlack);

    fragmentGreen = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentGreen, 1, &fragmentShaderGreen, NULL);
    glCompileShader(fragmentGreen);

    fragmentWhite = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentWhite, 1, &fragmentShaderWhite, NULL);
    glCompileShader(fragmentWhite);

    fragmentBrown = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentBrown, 1, &fragmentShaderBrown, NULL);
    glCompileShader(fragmentBrown);



    // link shaders

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    
    shaderBlue = glCreateProgram();
    glAttachShader(shaderBlue, vertexShader);
    glAttachShader(shaderBlue, fragmentBlue);
    glLinkProgram(shaderBlue);

   
    shaderBlack = glCreateProgram();
    glAttachShader(shaderBlack, vertexShader);
    glAttachShader(shaderBlack, fragmentBlack);
    glLinkProgram(shaderBlack);

    
    shaderGreen = glCreateProgram();
    glAttachShader(shaderGreen, vertexShader);
    glAttachShader(shaderGreen, fragmentGreen);
    glLinkProgram(shaderGreen);

    
    shaderWhite = glCreateProgram();
    glAttachShader(shaderWhite, vertexShader);
    glAttachShader(shaderWhite, fragmentWhite);
    glLinkProgram(shaderWhite);

    
    shaderBrown = glCreateProgram();
    glAttachShader(shaderBrown, vertexShader);
    glAttachShader(shaderBrown, fragmentBrown);
    glLinkProgram(shaderBrown);


    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


   

    glPointSize(4);
    glLineWidth(8);


    // Render Loop
    while (true) {


        processInput(window);
        glClearColor(0.54f, 0.74f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       
       if (action == 'D') {
           /*
           glDrawArrays(GL_TRIANGLES, 0, 6);
           glDrawArrays(GL_TRIANGLES, 0, 6);
           glDrawArrays(GL_TRIANGLES, 0, 6);
           glDrawArrays(GL_LINE_LOOP, 0, 4);
           glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
           glDrawArrays(GL_LINE_LOOP, 0, 9);
           glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
           glDrawArrays(GL_LINE_LOOP, 0, 10);
           glDrawArrays(GL_LINE_STRIP, 0, 7);
           glDrawArrays(GL_LINE_STRIP, 0, 6);
           glDrawArrays(GL_TRIANGLE_FAN, 0, 11);
           glDrawArrays(GL_LINE_LOOP, 0, 11);
           glDrawArrays(GL_TRIANGLE_FAN, 0, 11);
           glDrawArrays(GL_LINE_LOOP, 0, 11);
           glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
           glDrawArrays(GL_LINE_LOOP, 0, 6);
           */
        }

       

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(15, VAO);
    glDeleteBuffers(15, VBO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        action = 'D';
        cout << action << endl;
        drawFlower();
    }


    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}