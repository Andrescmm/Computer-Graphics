// Computacion Grafica : CCOMP 7-1
// Andres Cusirramos Marquez Mares
// 21/04/2022


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);


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
" FragColor = vec4( 0.143f, 0.197f, 0.187f, 1.f);\n"
"}\n";

const char* fragmentShaderBlack =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);\n"
"}\n";

const char* fragmentShaderRed =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4(0.8f, 0.0f, 0.0f, 1.0f);\n"
"}\n";

const char* fragmentShader1Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.90f, 0.94f, 0.85f, 1.0f);\n"
"}\n\0";

const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.75f, 0.83f, 0.47f, 0.5f);\n"
"}\n\0";



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

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
   
    // fragment shaders
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int fragmentBlue = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentBlue, 1, &fragmentShaderBlue, NULL);
    glCompileShader(fragmentBlue);

   
    unsigned int fragmentBlack = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentBlack, 1, &fragmentShaderBlack, NULL);
    glCompileShader(fragmentBlack);

    // link shaders

    unsigned int shaderBlue;
    shaderBlue = glCreateProgram();
    glAttachShader(shaderBlue, vertexShader);
    glAttachShader(shaderBlue, fragmentBlue);
    glLinkProgram(shaderBlue);

    unsigned int shaderBlack;
    shaderBlack = glCreateProgram();
    glAttachShader(shaderBlack, vertexShader);
    glAttachShader(shaderBlack, fragmentBlack);
    glLinkProgram(shaderBlack);
  
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Vertices Marco 
   const float verticesMarco[] = {
     -0.8f, 0.9f, 0.0f,  // A
      0.8f, 0.9f, 0.0f,  // B
      0.8f, -0.9f, 0.0f, // C

     -0.8f,  0.9f, 0.0f, // A
     -0.8f, -0.9f, 0.0f, // D
      0.8f, -0.9f, 0.0f // C
    };


   // Vertices Fondo
   const float verticesFondo[] = {
      -0.78f, 0.88f, 0.0f, // A
      0.78f, 0.88f, 0.0f,  // B
      0.78f, -0.88f, 0.0f, // C

     -0.78f, 0.88f, 0.0f,  // A
     -0.78f, -0.88f, 0.0f, // D
      0.78f, -0.88f, 0.0f, // C
   };

   // Vertices Tallo
   const float verticesTallo[] = {
        0.03f,-0.86f,0.0f,
       -0.05f,-0.86f,0.0f,
       -0.04f,-0.12f,0.0f,

        0.03f,-0.12f,0.0f,
       -0.04f,-0.12f,0.0f,
        0.03f,-0.86f,0.0f
   };

   // Vertices Borde hojas
   const float verticesHojaD[] = {
       //0
       0.05f, -0.86f, 0.0f,
       0.1f,  -0.85f, 0.0f,
       0.26f,  -0.76f, 0.0f,
       0.47f, -0.57f, 0.0f,
       0.65f, -0.34f, 0.0f,
       0.72f, -0.17f, 0.0f,
       0.77f, -0.01f, 0.0f,
       0.77f, 0.1f , 0.0f,
       0.76f, 0.21f, 0.0f,
       0.68f, 0.12f, 0.0f,
       0.57f, -0.04f, 0.0f,
       0.36f, -0.35f, 0.0f,
       0.2f,  -0.6f, 0.0f,
       0.1f,  -0.77f, 0.0f,
       0.05f, -0.86f, 0.0f
     
       //15

       //43
   };

   const float verticesHojaD1[] = {
       0.05f, -0.86f, 0.0f,
       0.1f,  -0.77f, 0.0f,
       0.2f,  -0.6f, 0.0f,
       0.36f, -0.35f, 0.0f,
       0.57f, -0.04f, 0.0f,
       0.68f, 0.12f, 0.0f,
       0.76f, 0.21f, 0.0f,
       0.65f,  0.18f, 0.0f, //G1
       0.05f,  0.12f, 0.0f, //H1
       0.39f,  0.06f, 0.0f, //J1
       0.26f, -0.04f, 0.0f, //L1
       0.17f, -0.19f, 0.0f, //M1
       0.11f, -0.35f, 0.0f, //N1
       0.08f, -0.48f, 0.0f, //O1
       0.05f, -0.63f, 0.0f, //P1
       0.03f, -0.78f, 0.0f, //Q1
       0.02f, -0.86f, 0.0f, //R1
       0.05f, -0.86f, 0.0f  
       //18
   };


   // Vertices Hojas
   const float verticesHojas[] = {
      -0.5f, 0.1f, 0.0f,
      -0.25f, 0.05f, 0.0f,
      -0.35f, -0.22f, 0.0f,
      -0.12f, -0.05f, 0.0f,
      -0.25f, -0.5f, 0.0f,
       0.0f, -0.2f, 0.0f,
      -0.14f, -0.63f, 0.0f,
       0.0f, -0.7f, 0.0f,

       0.14f, -0.63f, 0.0f,
       0.0f, -0.2f, 0.0f,
       0.25f, -0.5f, 0.0f,
       0.12f, -0.05f, 0.0f,
       0.35f, -0.22f, 0.0f,
       0.25f, 0.05f, 0.0f,
       0.5f, 0.1f, 0.0f
   };
     
 
    // Initial
    unsigned int VBO[5], VAO[5];
    glGenBuffers(5, VBO);
    glGenVertexArrays(5, VAO);

    // Vertices Marco
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesMarco), verticesMarco, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Fondo
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFondo), verticesFondo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Tallo
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTallo), verticesTallo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   
    // Vertices Hoja
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesHojaD), verticesHojaD, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertices Hoja
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesHojaD1), verticesHojaD1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glPointSize(4);
    glLineWidth(16);


    // Render Loop
    while (true) {

  
        processInput(window);
        glClearColor(153.0f, 191.0f, 227.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderBlack);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(shaderBlue);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(shaderBlack);
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_LINE_LOOP, 0, 15);

        glUseProgram(shaderBlack);
        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_LINE_STRIP, 0, 18);

      
           
   
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(5, VAO);
    glDeleteBuffers(5, VBO);
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


