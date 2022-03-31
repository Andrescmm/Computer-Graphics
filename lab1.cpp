
// Computacion Grafica : CCOMP 7-1
// Andres Cusirramos Marquez Mares
// 31/03/2022

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


int main(){

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
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
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
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
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
    float verticesTriangle[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };
    
    // Star Vertices
    float verticesStar[] = {
        
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
    float verticesPizza[] = {
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
    float verticesFlecha1[] = {
      -0.5f, 0.2f, 0.0f,
        0.5f, 0.2f, 0.0f,
        0.0f,  0.8f, 0.0f
      };

    float verticesFlecha2[] = {
      -0.3f, -0.5f, 0.0f,
        -0.3f, 0.2f, 0.0f,
        0.3f,  0.2f, 0.0f,
      -0.3f, -0.5f, 0.0f,
        0.3f,  0.2f, 0.0f,
        0.3f, -0.5f, 0.0f
    };
    

    // Triangle
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle), verticesTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    // Stars
    unsigned int VBO_1, VAO_1;
    glGenVertexArrays(1, &VAO_1);
    glGenBuffers(1, &VBO_1);
    glBindVertexArray(VAO_1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesStar), verticesStar, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Pizza
    unsigned int VBO_2, VAO_2;
    glGenVertexArrays(1, &VAO_2);
    glGenBuffers(1, &VBO_2);
    glBindVertexArray(VAO_2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPizza), verticesPizza, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    // Flecha
    unsigned int VBO_3[2], VAO_3[2];
    glGenBuffers(2, VBO_3);
    glGenVertexArrays(2, VAO_3);
    //Flecha 1
    glBindVertexArray(VAO_3[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_3[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFlecha1), verticesFlecha1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Felcha 2
    glBindVertexArray(VAO_3[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_3[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesFlecha2), verticesFlecha2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Render Loop
    glPointSize(8);
    glLineWidth(8);
    
    cout<<" -- Computacion Grafica -- "<<endl;
    cout<<endl;
    
    int option;
    int option1;
    int option2;
    
    
    while (true) {
       
        cout<<"Que figura desea dibujar"<<endl;
        cout<<"1 --> Triangulo"<<endl;
        cout<<"2 --> Estrella"<<endl;
        cout<<"3 --> Pizza"<<endl;
        cout<<"4 --> Flecha"<<endl;
        cout<<"5 --> Salir"<<endl;
        cout<<" --> ";
        cin>>option;
        
        // Triangulo
        if(option == 1){
                    
                    cout<<"Que tipo de Triangulo deseea?"<<endl;
                    cout<<"1 --> Triangulo con Puntos"<<endl;
                    cout<<"2 --> Triangulo con Lineas"<<endl;
                    cout<<"3 --> Triangulo con Color"<<endl;
                    cout<<"4 --> Salir"<<endl;
                    cout<<" --> ";
                    cin>>option1;
                if(option1 == 1){
                    while (!glfwWindowShouldClose(window)){
  
                        processInput(window);
                        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT);
                        glUseProgram(shaderProgram);
                        glBindVertexArray(VAO);
                        glDrawArrays(GL_POINTS, 0, 3);
                        glfwSwapBuffers(window);
                        glfwPollEvents();
                        break;
                     }
                }
                if(option1 == 2){
                    while (!glfwWindowShouldClose(window)){
                        
                        processInput(window);
                        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT);
                        glUseProgram(shaderProgram);
                        glBindVertexArray(VAO);
                        glDrawArrays(GL_LINE_LOOP, 0, 3);
                        glfwSwapBuffers(window);
                        glfwPollEvents();
                        break;
                     }
                }
                if(option1 == 3){
                        while (!glfwWindowShouldClose(window)){
                         
                            processInput(window);
                            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                            glClear(GL_COLOR_BUFFER_BIT);
                            glUseProgram(shaderProgram);
                            glBindVertexArray(VAO);
                            glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
                            glfwSwapBuffers(window);
                            glfwPollEvents();
                            break;
                         }
                }
          }
            
         // Estrella
         if(option == 2){
                    
                            cout<<"1 --> Estrella con Puntos"<<endl;
                            cout<<"2 --> Estrella con Lineas"<<endl;
                            cout<<"3 --> Estrella con Color"<<endl;
                            cout<<"4 --> Salir"<<endl;
                            cout<<" --> ";
                            cin>>option2;
                        if(option2 == 1){
                            while (!glfwWindowShouldClose(window)){
                                processInput(window);
                                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                                glClear(GL_COLOR_BUFFER_BIT);
                                glUseProgram(shaderProgram);
                                glBindVertexArray(VAO_1);
                                glDrawArrays(GL_POINTS, 0, 10);
                                glfwSwapBuffers(window);
                                glfwPollEvents();
                                break;
                             }
                        }
                        if(option2 == 2){
                            while (!glfwWindowShouldClose(window)){

                                processInput(window);
                                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                                glClear(GL_COLOR_BUFFER_BIT);
                                glUseProgram(shaderProgram);
                                glBindVertexArray(VAO_1);
                                glDrawArrays(GL_LINE_LOOP, 0, 10);
                                glfwSwapBuffers(window);
                                glfwPollEvents();
                                break;
                             }
                        }
                        if(option2 == 3){
                                while (!glfwWindowShouldClose(window)){

                                    processInput(window);
                                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                                    glClear(GL_COLOR_BUFFER_BIT);
                                    glUseProgram(shaderProgram);
                                    glBindVertexArray(VAO_1);
                                    glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
                                    glfwSwapBuffers(window);
                                    glfwPollEvents();
                                    break;
                                 }
                        }
                        if(option2 == 4){
                               break;
                                 }
                            
        }
        
        // Pizza
        if(option == 3){
                
                           while (!glfwWindowShouldClose(window)){
                               processInput(window);
                               glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                               glClear(GL_COLOR_BUFFER_BIT);
                               glUseProgram(shaderProgram);
                               glBindVertexArray(VAO_2);
                               glDrawArrays(GL_LINE_LOOP, 0, 24);
                               glfwSwapBuffers(window);
                               glfwPollEvents();
                               break;
                            }
        }
        
        //Flecha
        if(option == 4){
    
                while (!glfwWindowShouldClose(window)){
                 
                    processInput(window);
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);
                    glUseProgram(shaderProgram);
                    glBindVertexArray(VAO_3[0]);
                    glDrawArrays(GL_TRIANGLES, 0, 3);
                    glBindVertexArray(VAO_3[1]);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    glfwSwapBuffers(window);
                    glfwPollEvents();
                    break;
                 }
        
        }
                    
        
        // Close
        if(option == 5){
            cout<<" -- Adios -- "<<endl;
            break;
        }
        
        for(int i=0;i<25;i++){cout<<endl;}
    }
        
            

    
    
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
