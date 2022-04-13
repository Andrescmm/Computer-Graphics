// Computacion Grafica : CCOMP 7-1
// Andres Cusirramos Marquez Mares
// 12/04/2022


// P -> POINTS
// L -> LINES
// T -> TRIANGLES

// 1 -> TRIANGLE
// 2 -> STAR
// 3 -> PIZZA
// 4 -> HOUSE

// R -> RED COLOR
// G -> GREEN COLOR
// B -> BLUE COLOR
// M -> MAGENTA COLOR
// Y -> YELLOW COLOR
// C -> CIAN COLOR

// W -> UP
// S -> DOWN
// D -> RIGHT
// A -> LEFT


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

float rValue = 0.0f, gValue = 0.0f, bValue = 0.0f;
int num = 0;
int vertx = 0;
int vertx1 = 0;

vector<float>left = {-1.0f,0.0f,0.0f};
vector<float>right = {1.0f,0.0f,0.0f};
vector<float>up = {0.0f,1.0f,0.0f};
vector<float>down = {0.0f,-1.0f,0.0f};

enum primitives { POINTS, LINE_STRIP, TRIANGLES };
primitives PRIMITIVE_TYPE = primitives::TRIANGLES;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);


void move (vector<float> &figure, vector<float>direction){
    
}

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0f);\n"
    "}\0";

const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
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
    glfwSetKeyCallback(window, key_callback);

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
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    // Triangle Vertices
    vector<float> triangle = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };

    // Star Vertices
    vector<float> star = {

       -0.19,0.5f,0.0f, //C
       -0.6f,0.4f,0.0f, //E
       -0.28f,0.2f,0.0f, //F
       -0.35f,-0.16f,0.0f, //I
        0.0f,0.04f,0.0f, //J
        0.37f,-0.16f,0.0f, //H
        0.29f,0.2f,0.0f, //G
        0.6f,0.4f,0.0f, //D
        0.2f,0.5f,0.0f, //B
        0.0f,0.8f,0.0f //A

    };

    // Pizza Vertices
    vector<float> pizza= {
        // 1
        0.0f, 0.0f, 0.0f, //A
        0.0f, 0.4f, 0.0f, //B
        0.3f,  0.3f, 0.0f, //C
        // 2
        0.0f, 0.0f, 0.0f,  //A_2
        0.3f,  0.3f, 0.0f, //C_2
        0.4f,  0.0f, 0.0f,  //D
        // 3
        0.0f, 0.0f, 0.0f,   //A_3
        0.4f,  0.0f, 0.0f,  //D_2
        0.3f, -0.3f, 0.0f,   //E
        //4
        0.0f, 0.0f, 0.0f,   //A_4
        0.3f, -0.3f, 0.0f,  //E_2
        0.0f, -0.4f, 0.0f,   //F
        //5
        0.0f, 0.0f, 0.0f,   //A_5
        0.0f, -0.4f, 0.0f,  //F_2
       -0.3f, -0.3f, 0.0f,  //G
        //6
        0.0f, 0.0f, 0.0f,   //A_6
       -0.3f, -0.3f, 0.0f,  //G_2
       -0.4f,  0.0f, 0.0f,  //H
        //7
        0.0f, 0.0f, 0.0f,   //A_7
       -0.4f,  0.0f, 0.0f,  //H_2
       -0.3f,  0.3f, 0.0f,  //I
        //8
        0.0f, 0.0f, 0.0f,   //A_8
       -0.3f,  0.3f, 0.0f,  //I_2
        0.0f, 0.4f, 0.0f   //B_2
    };


    //Flecha Vertices
    vector<float> flecha1 = {
       -0.5f, 0.2f, 0.0f,
        0.5f, 0.2f, 0.0f,
        0.0f,  0.8f, 0.0f
    };

    vector<float> flecha2 = {
        -0.3f, -0.5f, 0.0f,
        -0.3f, 0.2f, 0.0f,
         0.3f,  0.2f, 0.0f,
        -0.3f, -0.5f, 0.0f,
         0.3f,  0.2f, 0.0f,
         0.3f, -0.5f, 0.0f
    };


    // Initial
    unsigned int VBO[5], VAO[5];
    glGenBuffers(5, VBO);
    glGenVertexArrays(5, VAO);

    // Triangle
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, triangle.size() * sizeof(float), &triangle[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Stars
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, star.size() * sizeof(float), &star[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Pizza
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, pizza.size() * sizeof(float), &pizza[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Flecha 1
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, flecha1.size() * sizeof(float), &flecha1[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Flecha 2
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, flecha2.size() * sizeof(float), &flecha2[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPointSize(4);
    glLineWidth(8);


    // Render Loop
    while (true) {

        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, rValue, gValue, bValue, 1.0f);

        switch (PRIMITIVE_TYPE) {
        case primitives::POINTS:

            glBindVertexArray(VAO[num]);
            glDrawArrays(GL_POINTS, 0, vertx);
            if (num == 3) {
                glBindVertexArray(VAO[4]);
                glDrawArrays(GL_POINTS, 0, vertx1);
            }
            break;

        case primitives::LINE_STRIP:

            glBindVertexArray(VAO[num]);
            glDrawArrays(GL_LINE_LOOP, 0, vertx);
            if (num == 3) {
                glBindVertexArray(VAO[4]);
                glDrawArrays(GL_LINE_LOOP, 0, vertx1);
            }
            break;

        case primitives::TRIANGLES:

            glBindVertexArray(VAO[num]);
            glDrawArrays(GL_TRIANGLE_FAN, 0, vertx);
            if (num == 3) {
                glBindVertexArray(VAO[4]);
                glDrawArrays(GL_TRIANGLE_FAN, 0, vertx1);
            }
            break;
        }

        //glBindVertexArray(VAO[num]);
       // glDrawArrays(GL_POINTS, 0, vertx);
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



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        rValue = 255.0f;
        gValue = 0.0f;
        bValue = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        rValue = 0.0f;
        gValue = 255.0f;
        bValue = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        rValue = 0.0f;
        gValue = 0.0f;
        bValue = 255.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        rValue = 0.0f;
        gValue = 255.0f;
        bValue = 255.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        rValue = 255.0f;
        gValue = 255.0f;
        bValue = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        rValue = 255.0f;
        gValue = 0.0f;
        bValue = 255.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { // Triangle
        num = 0;
        vertx = 3;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {// Star
        num = 1;
        vertx = 10;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {// Pizza
        num = 2;
        vertx = 24;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {// House
        num = 3;
        vertx = 3;
        vertx1 = 6;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
     
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
   
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
       
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
       
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        PRIMITIVE_TYPE = primitives::POINTS;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        PRIMITIVE_TYPE = primitives::LINE_STRIP;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        PRIMITIVE_TYPE = primitives::TRIANGLES;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

