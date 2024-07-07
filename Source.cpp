#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource[2] = { 
{"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.41176f, 0.70588f, 1.0f);\n"   // Pink
"}\0"},

{"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0f, 0.749f, 1.0f, 1.0f);\n"   // Blue
"}\0"}
};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // =============================================================
    // -------------------- Vertex Shader --------------------------
    // =============================================================

    //  vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // checking if compilation was sucessfull
    int  success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // =============================================================
    // -------------------- Fragment Shader ------------------------
    // =============================================================

    unsigned int fragmentShader[2];
    for (int i = 0; i < 2; i++)
    {
        fragmentShader[i] = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader[i], 1, &fragmentShaderSource[i], NULL);
        glCompileShader(fragmentShader[i]);

        // checking if compilation of Fragment shader was successful
        glGetShaderiv(fragmentShader[i], GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader[i], 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT" << i << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    // ============================================================
    // -------------------- Shader Program ------------------------
    // ============================================================
    unsigned int shaderProgram[2];
    for (int i = 0; i < 2; i++)
    {
        //creating the shader program
        shaderProgram[i] = glCreateProgram();

        // Attaching the created shaders to the program and then linking them
        glAttachShader(shaderProgram[i], vertexShader);
        glAttachShader(shaderProgram[i], fragmentShader[i]);
        glLinkProgram(shaderProgram[i]);

        //checking if linking was succesfull
        glGetProgramiv(shaderProgram[i], GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram[i], 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM" << i << "::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glUseProgram(shaderProgram[i]);
        glDeleteShader(fragmentShader[i]);
    }
        glDeleteShader(vertexShader);
    //      Linking Vertex Attributes

    float vertices[] = {
        // First Rectangle
        -0.9f, -0.5f, 0.0f,     //Bottom left   v0
        -.4f, -0.5f, 0.0f,       //Bottom Right  v1
        -0.4f, 0.5f, 0.0f,       //Top Right     v2
        -0.9f, 0.5f, 0.0f,      //Top Left      v3

        //Second Rectangle
        0.9f, -0.5f, 0.0f,     //Bottom left    v4
        0.4f, -0.5f, 0.0f,      //Bottom Right   v5
        0.4f, 0.5f, 0.0f,       //Top Right      v6
        0.9f, 0.5f, 0.0f       //Top Left       v7
    };

    unsigned int indices[] = {
        // First Rectangle
        0, 1, 2,
        2, 3, 0,

        //Second Rectangle
        4, 5, 7,
        5, 6, 7
    };

    unsigned int VAO, VBO[2], EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // First Rectangle
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Second Rectangle
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Draw in "wireframe mode"
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        glUseProgram(shaderProgram[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgram[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6*sizeof(GLuint)));
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2, &VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(shaderProgram[0]);
    glDeleteProgram(shaderProgram[1]);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
