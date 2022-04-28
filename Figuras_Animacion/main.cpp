// Computacion Grafica : CCOMP 7-1
// Andres Cusirramos Marquez Mares
// 27/04/2022


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

// Variables
unsigned int VBO[5], VAO[5];

float rValue = 0.0f, gValue = 0.0f, bValue = 0.0f;
int modo = 0;
int vertx = 0;
int vertx1 = 0;
float angle2;
float k = 0.5f;

char figure = 'P';

//float X = 0.3f;
//float Y = 0.3f;

// Matrices
float X, Y;
vector < vector<float> >matrixTranslate = { {1.0f,0.0f,X},{0.0f,1.0f,Y},{0.0f,0.0f,1.0f} };

float W, H;
vector < vector<float> >matrixScale = { {W,0.0f,0.0f},{0.0f,H,0.0f},{0.0f,0.0f,1.0f} };

vector < vector<float> >matrix = { {glm::cos(angle2),glm::sin(angle2),0.0f},{glm::sin(angle2), glm::cos(angle2),0.0f},{0.0f,0.0f,1.0f} };

int angle;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
void transM(vector<float>& figure, float x, float y);
void scaleM(vector<float>& figure, float w, float h);
void rotateM(vector<float>& figure, int angle);





void moveMatrix(vector<float>& figure, vector<vector<float>>& matrix) {

 

    for (int i = 0; i < figure.size(); i += 3) {

        figure[i] = ((matrix[0][0] * figure[i]) + (matrix[0][1] * figure[1]) + (matrix[0][2] * figure[i+2]));
        figure[i + 1] = ((matrix[1][0] * figure[i]) + (matrix[1][1] * figure[i+1]) + (matrix[1][2] * figure[i+2]));

        cout <<"figure " << i << " -> " << figure[i] << endl;
        cout << "figure " << i+1 << " -> " << figure[i+1] << endl;
      
    }
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
vector<float> pizza = {
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



// Drawing
/// Triangle
void drawTriangle() {
    // Initial
    unsigned int VBO[5], VAO[5];
    glGenBuffers(5, VBO);
    glGenVertexArrays(5, VAO);

    // Triangle
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, triangle.size() * sizeof(float), &triangle[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO[0]);
}

/// Star
void drawStar() {
    // Initial
    unsigned int VBO[5], VAO[5];
    glGenBuffers(5, VBO);
    glGenVertexArrays(5, VAO);

    // Star
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, star.size() * sizeof(float), &star[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO[1]);

}

/// Pizza
void drawPizza() {
    // Initial
    unsigned int VBO[5], VAO[5];
    glGenBuffers(5, VBO);
    glGenVertexArrays(5, VAO);
    // Pizza
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, pizza.size() * sizeof(float), &pizza[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO[2]);
}



// Main
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


    // Sizes
    glPointSize(4);
    glLineWidth(8);


    // Render Loop
    while (true) {

        processInput(window);
        glClearColor(0.54f, 0.74f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, rValue, gValue, bValue, 1.0f);

        // Pizza
        if (figure == 'P') {
            if (modo == 0) {
                glDrawArrays(GL_LINE_LOOP, 0,24);
            }
            if (modo == 1) {
                glDrawArrays(GL_TRIANGLE_FAN, 0, 24);
            }
            if (modo == 2) {
                glDrawArrays(GL_POINTS,0,24);
            }
        }
        // Star
        if (figure == 'S') {
            if (modo == 0) {
                glDrawArrays(GL_LINE_LOOP, 0, 10);
            }
            if (modo == 1) {
                glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
            }
            if (modo == 2) {
                glDrawArrays(GL_POINTS, 0, 10);
            }
        }
        if (figure == 'T') {
            //PIZZA
            if (modo == 0) {
                glDrawArrays(GL_LINE_LOOP, 0, 3);
            }
            if (modo == 1) {
                glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
            }
            if (modo == 2) {
                glDrawArrays(GL_POINTS, 0, 3);
            }
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

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {// Triangle
        figure = 'T';
        drawTriangle();
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {// Star
        figure = 'S';
        drawStar();

    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {// Pizza
        figure = 'P';
        drawPizza();
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        modo = 2;
    }
        

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        modo = 0;
    }
        

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        modo = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        
        X += 0.001f;
        Y += 0.001f;
        if (figure == 'T') {
            transM(triangle, X, Y);
            drawTriangle();
        }
        if (figure == 'S') {
            transM(star, X, Y);
            drawStar();
        }
        if (figure == 'P') {
            transM(pizza, X, Y);
            drawPizza();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        X -= 0.001f;
        Y -= 0.001f;
        if (figure == 'T') {
            transM(triangle, X, Y);
            drawTriangle();
        }
        if (figure == 'S') {
            transM(star, X, Y);
            drawStar();
        }
        if (figure == 'P') {
            transM(pizza, X, Y);
            drawPizza();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {

        W = 0.99f;
        H = 0.99f;
        if (figure == 'T') {
            scaleM(triangle, W, H);
            drawTriangle();
        }
        if (figure == 'S') {
            scaleM(star, W, H);
            drawStar();
        }
        if (figure == 'P') {
            scaleM(pizza, W, H);
            drawPizza();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {

        W = W + 0.001f;
        H = H + 0.001f;
        if (figure == 'T') {
            scaleM(triangle, W, H);
            drawTriangle();
        }
        if (figure == 'S') {
            scaleM(star, W, H);
            drawStar();
        }
        if (figure == 'P') {
            scaleM(pizza, W, H);
            drawPizza();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        angle = 30;
        if (figure == 'T') {
            rotateM(triangle,angle);
            drawTriangle();
        }
        if (figure == 'S') {
            rotateM(star, angle);
            drawStar();
        }
        if (figure == 'P') {
            rotateM(pizza, angle);
            drawPizza();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        angle = -30;
        if (figure == 'T') {
            rotateM(triangle, angle);
            drawTriangle();
        }
        if (figure == 'S') {
            rotateM(star, angle);
            drawStar();
        }
        if (figure == 'P') {
            rotateM(pizza, angle);
            drawPizza();
        }
    }
       

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void transM(vector<float>& figure, float x, float y) {
    glm::mat3 translate(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(x, y, 1));

    for (int i = 0; i < figure.size(); i += 3) {
        glm::vec3 temp(figure[i], figure[i + 1], 1);
        temp = translate * temp;

        figure[i] = temp[0];
        figure[i + 1] = temp[1];
    }
}

void scaleM(vector<float>& figure, float w, float h) {
    glm::mat2 translate(glm::vec2(w, 0), glm::vec2(0, h));

    for (int i = 0; i < figure.size(); i += 3) {
        glm::vec2 temp(figure[i], figure[i + 1]);
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