/*****************************************************************************
 * Author : Ram
 * Date : 7/June/2019
 * Email : ramkalath@gmail.com
 * Breif Description : instancing
 * Detailed Description :
 *****************************************************************************/
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, thus closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        cout << "Window is closing because u pressed the ESC key" << endl;
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main()
{
    // initializing glfw*****************************************************************
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Rectangle from Triangles", nullptr, nullptr);
    if(window == nullptr)
    {
        cout << "Failed to create a GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initializing glew*******************************************************************
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetKeyCallback(window, key_callback);

    // build a shader program***************************************************************
    // Vertex Shader
            GLuint vs = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vs, 1, &vertexShaderSource, NULL);
            glCompileShader(vs);

            // check for compile time errors
            GLint success;
            GLchar infoLog[512];
            glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(vs, 512, NULL, infoLog);
                cout << "ERROR: Vertex shader compilation failed." << infoLog << endl;
            }

    // Fragment Shader
            GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fs, 1, &fragmentShaderSource, NULL);
            glCompileShader(fs);

            // check for compile time errors
            glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(fs, 512, NULL, infoLog);
                cout << "ERROR: Fragment shader compilation failed." << infoLog << endl;
            }

    // Linking the Shaders
            GLuint program = glCreateProgram();
            glAttachShader(program, vs);
            glAttachShader(program, fs);
            glLinkProgram(program);

            // check for linking errors
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(program, 512, NULL, infoLog);
                cout << "ERROR: Shader program failed to link" << infoLog << endl;
            }

    // Delete shaders after compilation
            glDeleteShader(vs);
            glDeleteShader(fs);
    //****************************************************************************************
    // let us now write code for the actual data points for the rectangle and add bind it with a VBO. VAO is then used to encapsulate VBO
    GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,   /* left bottom */
                           -0.5f,  0.5f, 0.0f,   /* left top */
							0.5f, -0.5f, 0.0f,   /* right bottom */

                           -0.5f,  0.5f, 0.0f,   /* left top */
                            0.5f,  0.5f, 0.0f,   /* right top */     
							0.5f, -0.5f, 0.0f};  /* right bottom */

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); // Bind vertex array objects first before VBOs

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);               // unbinding VAO

    //**********************************************************************************************
    // Game Loop
    while(!glfwWindowShouldClose(window))
    {
        // glfw main loop ************************************************
        glfwPollEvents();

        glClearColor(0.09f, 0.105f, 0.11f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a rectangle
        glUseProgram(program);
        glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
     }
    // Deleting the used resources*****************************************
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glDeleteShader(program);
    //****************************************************************************
    glfwTerminate();
    return 0;
}
