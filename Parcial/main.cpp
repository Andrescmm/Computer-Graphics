// Computacion Grafica : CCOMP 7-1
// Andres Cusirramos Marquez Mares
// 09/05/2022

// Construccion de Matrices en matriz.h

//Instrucciones

// A -> Activar y desactivar la animacion
// T -> Escalar el triangulo
// R -> Cambia el angulo del cuadrado
// ARRIBA -> Acelera la animacion
// ABAJO  -> Desacelara de la animacion
// 


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Matriz.h"
#include <cmath> 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 700;
const int dim_size = 4;

int angle = 90;
int animacion = 10;
float length = 0.1;
float space = 0.25;
float radio = space + length;


vector<float> vertices = {


     // Triangulo
     0.1f,  -0.1f, 0.0f,  
     -0.1f, -0.1f, 0.0f,  
     0.0f,   0.1f, 0.0f,

    // Square
     0.25f + 0.1f,  0.1f, 0.0f,  
     0.25f + 0.1f,  -0.1f, 0.0f,  
     0.25f + 3 * 0.1f,  -0.1f, 0.0f,  
     0.25f + 3 * 0.1f,  0.1f, 0.0f,  
     0.25f + 0.1f,  0.1f, 0.0f,  
     0.25f + 3 * 0.1f,  -0.1f, 0.0f,  

     // Diamond
     0.25f + 1 * 0.1f + 0.25f ,  0.0f, 0.0f,  
     0.25f + 1 * 0.1f + 0.25f + 0.1f,  0.1f, 0.0f,  
     0.25f + 1 * 0.1f + 0.25f + 0.1f,  -0.1f, 0.0f,  
     0.25f + 1 * 0.1f + 0.25f + 2 * 0.1f ,  0.0f, 0.0f,  
     0.25f + 1 * 0.1f + 0.25f + 0.1f,  0.1f, 0.0f,  
     0.25f + 1 * 0.1f + 0.25f + 0.1f,  -0.1f, 0.0f,  

    // Star
    0.55f + 0.3,-0.1,0.0f,     
    0.6f + 0.3,0.1f,0.0f,    
    0.63f + 0.3,0.0f,0.0f,    
    0.48f + 0.3,0.04f,0.0f,   
    0.72f + 0.3,0.04f,0.0f,   
    0.6f + 0.3,-0.02f,0.0f,   
    0.6f + 0.3,0.1f,0.0f,     
    0.65f + 0.3,-0.1f,0.0f,   
    0.57f + 0.3,0.0f,0.0f,    
    0.60f,0.0193f,0.0f

};



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.5);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

const char* fragmentShaderRed =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4( 0.98f, 0.01f, 0.01f, 1.0f);\n"
"}\n";

const char* fragmentShaderYellow =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4( 0.99f, 0.79f, 0.39f, 1.0f);\n"
"}\n";

const char* fragmentShaderGreen1 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4( 0.0f, 0.69f, 0.31f, 1.0f);\n"
"}\n";

const char* fragmentShaderGreen2 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4( 0.57f, 0.80f, 0.33f, 1.0f);\n"
"}\n";

const char* fragmentShaderBlue =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4( 0.09f, 0.56f, 0.72f, 1.0f);\n"
"}\n";

const char* fragmentShaderPink =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
" FragColor = vec4( 1.0f, 0.18f, 0.34f, 1.0f);\n"
"}\n";



matrix create_m_vertices(vector<float> vertices, int index){
    vector<float> m_vertices = vector<float>(vertices.begin() + index, vertices.begin() + index + 3);
    m_vertices.push_back(1.0f);
    return matrix(m_vertices);
}


void mover(matrix m, int num_tri, int indice){

    for (int i = indice; i < 9 * num_tri; i += 3){

        matrix m_vertices = create_m_vertices(vertices, i);
        matrix res = m.mult(m_vertices);

        vertices[i] = res.m[0][0];
        vertices[i + 1] = res.m[1][0];
        vertices[i + 2] = res.m[2][0];
    }
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}


// Escala Triangulo
bool grow = true;
void scaleT(float variation){

    float size = abs(vertices[0] - vertices[1]);

    if (size > 0.5){
        grow = false;
    }
    if (!grow) {
        variation = variation * -1;
    }

    if (size < 0.2) {
        grow = true;
    }

    vector<float> st = { 1.0f + variation, 1.0f + variation,1.0f };
    matrix S = matrix::GenScale(st, dim_size);
    mover(S, 1, 0);

}


// Giro del cuadrado
bool rotation = true;
void moveSquare(){

    // creacion de la matriz
    matrix RZ = matrix::GenRotate(angle, 'z', dim_size);

    if (rotation) {
        mover(RZ, 3, 9);
    }
}


float diamond_variationx = 0.1;
float diamond_variationy = 0.1;
bool up = true;
bool tleft = true;

void moveDiamond(){

    float variationx = 0;
    float variationy = 0;

    if (vertices[28] <= 1 - 4.5 * length && up) {

        variationy = diamond_variationy;
    }
    else if (vertices[27] >= -0.6 && tleft)    
    {
        //moving = true;

        variationx = -0.1;
    }
    else if (vertices[28] >= -1 + 4.2 * length) {
        up = false;
        variationy = -diamond_variationy;
    }

    else if (vertices[27] <= 1 - 4.2 * length) {
        tleft = false;
        variationx = diamond_variationx;
    }

    else {
        up = true;
        tleft = true;
    }

    vector<float> sd = { variationx,variationy,0.0f };
    matrix T = matrix::GenTranslation(sd, dim_size);
    mover(T, 5, 27);
}


// Mover estrella
bool rotation1 = true;
float star_variationx1 = 0.5;
float star_variationy1 = length;
float star_variationx = 0.1;
float star_variationy = 0.1;
bool up1 = true;
bool tleft1 = true;
void moveStar()                            
{
    float variationx = 0;
    float variationy = 0;

    if (vertices[46] <= 0.9 - length && up1){
        variationy = star_variationy;
    }
    else if (vertices[45] >= -0.9 && tleft1){

        variationx = -star_variationx;
    }
    else if (vertices[46] >= -1.1 + length){
        up1 = false;
        variationy = -star_variationy;
    }

    else if (vertices[45] <= 1 - 2 * length) {
        tleft1 = false;
        variationx = star_variationx;

    }
    else {
        up1 = true;
        tleft1 = true;
    }
    vector<float> st_axis = { variationx,variationy,0.0f };
    matrix T = matrix::GenTranslation(st_axis, dim_size);
    mover(T, 8, 45); 
}


void animation(float inc){
    moveSquare();
    moveDiamond();
    moveStar();
}

// Main
// Activar o desactivar la animacion
float ani = 0.1;

int main(){
 

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

    // build and compile our shader program
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

      // MY SHADERS

    unsigned int fragmentRed = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentRed, 1, &fragmentShaderRed, NULL);
    glCompileShader(fragmentRed);

    unsigned int fragmentYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentYellow, 1, &fragmentShaderYellow, NULL);
    glCompileShader(fragmentYellow);

    unsigned int fragmentGreen1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentGreen1, 1, &fragmentShaderGreen1, NULL);
    glCompileShader(fragmentGreen1);

    unsigned int fragmentGreen2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentGreen2, 1, &fragmentShaderGreen2, NULL);
    glCompileShader(fragmentGreen2);

    unsigned int fragmentBlue = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentBlue, 1, &fragmentShaderBlue, NULL);
    glCompileShader(fragmentBlue);

    unsigned int fragmentPink = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentPink, 1, &fragmentShaderPink, NULL);
    glCompileShader(fragmentPink);


    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


      // MY LINK SHADERS

    unsigned int shaderRed = glCreateProgram();
    glAttachShader(shaderRed, vertexShader);
    glAttachShader(shaderRed, fragmentRed);
    glLinkProgram(shaderRed);

    unsigned int shaderYellow = glCreateProgram();
    glAttachShader(shaderYellow, vertexShader);
    glAttachShader(shaderYellow, fragmentYellow);
    glLinkProgram(shaderYellow);

    unsigned int shaderGreen1 = glCreateProgram();
    glAttachShader(shaderGreen1, vertexShader);
    glAttachShader(shaderGreen1, fragmentGreen1);
    glLinkProgram(shaderGreen1);

    unsigned int shaderGreen2 = glCreateProgram();
    glAttachShader(shaderGreen2, vertexShader);
    glAttachShader(shaderGreen2, fragmentGreen2);
    glLinkProgram(shaderGreen2);


    unsigned int shaderBlue = glCreateProgram();
    glAttachShader(shaderBlue, vertexShader);
    glAttachShader(shaderBlue, fragmentBlue);
    glLinkProgram(shaderBlue);

    unsigned int shaderPink = glCreateProgram();
    glAttachShader(shaderPink, vertexShader);
    glAttachShader(shaderPink, fragmentPink);
    glLinkProgram(shaderPink);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    int animacionR = 0;
    int limit = 100;
    float ani = 0.1;
    glLineWidth(5);

    while (!glfwWindowShouldClose(window))
    {
        
        processInput(window);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Triangle
        glUseProgram(shaderYellow);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderRed);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP, 0, 3);

        // Square
        glUseProgram(shaderGreen1);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 3, 6);

        glUseProgram(shaderGreen2);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 3, 6);

        // Diamond
        glUseProgram(shaderBlue);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 6, 9);

        // Star
        glUseProgram(shaderPink);
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 1.0f, 0.18f, 0.34f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 9, 16);

     


        if (animacionR >= 100) {

            animacionR %= limit;
            animation(ani);
        }

        animacionR += animacion;

  
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int fin;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){


    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        if (animacion != 0) {
            fin = animacion;
            animacion = 0;
        }
        else
            animacion = fin;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        animacion += 5;
        animacion = animacion % 50;
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        animacion -= 5;
        if (animacion == 0){
            animacion = 5;
        }
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        angle += 90;
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        angle -= 90;
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        rotation = !rotation;
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        scaleT(ani);
    }
}