// TODO:
// find a way to get nicely with programID of every object

#define GL3W_IMPLEMENTATION
#include "../include/GL/gl3w.h" // https://github.com/gingerBill/gl3w-Single-File

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "mathOpengl.h"
#include "camera.h"
#include "input.h"
#include "model.h"
#include "cube.h"


const float screenWidth = 1200.0;
const float screenHeight = 1000.0;

float lastMillisTime, deltaTime;

// AXIS FOR DEBUGGING
GLuint axisIndices[] =
{
    0, 2, 1, // axis x
    0, 3, 1, // axis y
    0, 4, 1  // axis z
};
GLuint axisVAO, axisVBO, axisEBO;
void initAxis();
void drawAxis();
// END OF AXIS FOR DEBUGGING


int main(void)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Can't initialize glfw");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL test", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (gl3w_init() != 0)
    {
        fprintf(stderr, "failed to initialize OpenGL\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "OpenGL initialize succeeded\n");
    }

    if (gl3w_is_supported(3, 3) != 1)
    {
        fprintf(stderr, "OpenGL 3.3 not supported\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(stdout, "OpenGL 3.3 supported\n");
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));


    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    CameraHandler *cameraHandler = cameraCreate(screenWidth, screenHeight);
    InputHandler *inputHandler = inputCreate(window, cameraHandler);
    inputInit(inputHandler);

    // Setup time
    lastMillisTime = glfwGetTime() * 1000.0;
    deltaTime = lastMillisTime;

    // Generate cubes
    vec3f cubePositions[] = 
    {
        {-1.5,  3.5, -2.0},
        { 1.5,  2.5, -4.0},
        { 0.0,  1.0, -6.0},
        { 2.5,  2.0, -8.0},
        { 4.5,  2.0, -0.0}
    };

    cubeInit();
    Cube *cubes[5];
    for (int i = 0; i < 5; i++)
    {
        cubes[i] = cubeCreate();
        cubeSetPosition(cubes[i], cubePositions[i]);
    }


    Model *tmpModel = modelCreate();
    Shader *shader = shaderCreateFromFile("resources/shaders/model_vs.glsl", "resources/shaders/model_fs.glsl");
    modelSetShader(tmpModel, shader);
    modelLoad(tmpModel, "resources/models/penguin.obj");




    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.15, 1);

        inputUpdate(inputHandler, deltaTime);

        mat4f view, perspective;
        cameraUpdate(cameraHandler);
        cameraGetViewMatrix(cameraHandler, view);
        cameraGetPerspectiveMatrix(cameraHandler, perspective);

        modelUpdateProjection(tmpModel, view, perspective);
        modelRender(tmpModel);  

        // draw every cube
        cubeUpdateProjection(view, perspective);
        for (int i = 0; i < 5; i++) 
        {
            // cubeTranslate(cubes[i], 0, 0, 0.025);
            cubeRotate(cubes[i], 0, 1, 0, 1*DEG2RAD);
            // cubeScale(cubes[i], -0.001, -0.001, -0.001);
            cubeRender(cubes[i]);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update delta time
        double tempTime = (float)glfwGetTime() * 1000.0;
        deltaTime = tempTime - lastMillisTime;
        lastMillisTime = tempTime;
        // printf("dt = %f\n", deltaTime);
    }

    printf("Exiting...\n");

    for (int i = 0; i < 5; i++)
        cubeDestroy(cubes[i]);

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}


void initAxis()
{
    vec3f axisVertices[] = 
    {
        // axis
        { 0.0f,  0.0f,  0.0f },  {0.7f, 0.7f, 0.7f },  // 8
        { 0.2f,  0.2f,  0.0f },  {0.7f, 0.7f, 0.7f },  // 9
        { 10.0f, 0.0f,  0.0f },  {1.0f, 0.0f, 0.0f },  // x axis // 10
        { 0.0f,  10.0f, 0.0f },  {0.0f, 1.0f, 0.0f },  // y axis // 11
        { 0.0f,  0.0f,  10.0f},  {0.0f, 0.0f, 1.0f },  // z axis // 12
    };

    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    glGenBuffers(1, &axisEBO);

    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    // Positions (first 3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors (second 3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axisIndices), axisIndices, GL_STATIC_DRAW); 
}

void drawAxis(GLuint programID)
{
    glBindVertexArray(axisVAO);
    mat4f axisTranslation;
    mat4fIdentity(axisTranslation);
    GLint uniModel = glGetUniformLocation(programID, "model");
    glUniformMatrix4fv(uniModel, 1, GL_TRUE, axisTranslation);

    glDrawElements(GL_TRIANGLES,
                   sizeof(axisIndices) / sizeof(axisIndices[0]),
                   GL_UNSIGNED_INT,
                   (void *)0);
}