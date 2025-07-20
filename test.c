// OpenGL ES 2.0 ile GL_DITHER ve GL_SAMPLE_ALPHA_TO_COVERAGE test uygulaması
// Ekranın sol yarısı: DITHER kapalı, sağ yarısı: DITHER açık
// Ekranın üst kısmı: SAMPLE_ALPHA_TO_COVERAGE kapalı, alt kısmı açık

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include "screen.h"

void init(GLFWwindow **window);
GLint create_shader(const char *shaderSource, GLenum type);
GLint create_program(unsigned int vertexShader, unsigned int fragmentShader);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void draw(GLint programID, unsigned int VBO, int size, float color[3]);

int g_width = 1280, g_height = 720;

int main() {
    GLFWwindow *window;
    init(&window);
    float navy[3] = {0.0f, 0.125f, 0.376f};
    float yellow[3] = {1.0f, 0.835f, 0.0f};
    float green[3] = {0.0f, 0.65f, 0.2f};

    // Fragment shader with uniform color control
    const char *FSsource = "#version 100\n"
                           "precision mediump float;\n"
                           "uniform vec3 uColor;\n"
                           "void main()\n"
                           "{\n"
                           "    gl_FragColor = vec4(uColor, 1.0);\n"
                           "}\n";

    // Vertex shader
    const char *VSsource = "#version 100\n"
                           "attribute vec3 aPos;\n"
                           "void main()\n"
                           "{\n"
                           "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                           "}";

    // Create shaders
    GLint vertexShader = create_shader(VSsource, GL_VERTEX_SHADER);
    GLint fragmentShader = create_shader(FSsource, GL_FRAGMENT_SHADER);
    GLint programID = create_program(vertexShader, fragmentShader);

    // Triangle vertices
    float triangleVertices[] = {
            -0.61f, -0.6f, 0.0f, // left
            0.6f, -0.6f, 0.0f, // right
            0.0f,  0.6f, 0.0f  // top
    };

    // Rectangle vertices
    float rectangleVertices[] = {
            -0.6f, -0.3f, 0.0f, // bottom left
            0.6f, -0.3f, 0.0f, // bottom right
            -0.6f,  0.3f, 0.0f, // top left
            0.6f, -0.3f, 0.0f, // bottom right
            0.6f,  0.3f, 0.0f, // top right
            -0.6f,  0.3f, 0.0f  // top left
    };

    // Little triangle vertices
    float littleTriangleVertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f, // right
            0.0f,  0.5f, 0.0f  // top
    };

    // Create buffers
    unsigned int triangleVBO, rectangleVBO, littleTriangleVBO;
    glGenBuffers(1, &triangleVBO);
    glGenBuffers(1, &rectangleVBO);
    glGenBuffers(1, &littleTriangleVBO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, littleTriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(littleTriangleVertices), littleTriangleVertices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glViewport(0, 0, g_width, g_height);
        draw(programID, rectangleVBO, 6, yellow);

        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &triangleVBO);
    glDeleteBuffers(1, &rectangleVBO);
    glDeleteBuffers(1, &littleTriangleVBO);
    glDeleteProgram(programID);

    glfwTerminate();
    printf("Program terminated.\n");
    return 0;
}

void init(GLFWwindow **window) {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    *window = glfwCreateWindow(g_width, g_height, "GL_DITHER vs No DITHER", NULL, NULL);
    if (*window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        exit(-1);
    }
}

GLint create_shader(const char *shaderSource, GLenum type) {
    GLint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
        exit(-1);
    }
    return shaderID;
}

GLint create_program(unsigned int vertexShader, unsigned int fragmentShader) {
    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindAttribLocation(shaderProgram, 0, "aPos");
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::LINK_FAILED\n%s\n", infoLog);
        exit(-1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    g_height = height;
    g_width = width;
    glViewport(0, 0, width, height);
}

void draw(GLint programID, unsigned int VBO, int size, float color[3]) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glUniform3fv(glGetUniformLocation(programID, "uColor"), 1, color);
    glDrawArrays(GL_TRIANGLES, 0, size);
}