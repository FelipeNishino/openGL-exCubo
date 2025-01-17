﻿#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string.h>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Window* window;
Camera camera;
Texture brickTexture;
Texture dirtTexture;
Texture cubeTexture;

void error_glfw(int error, const char* desc) {
    printf("%d: %s\n", error, desc);
}

static const char* vertexLocation = "VertexShader.glsl";
static const char* fragmentLocation = "FragmentShader.glsl";

void CriaTriangulos() {
    GLfloat vertices[] = {
        //x      y     z
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // Vertice 0 (x, y, z, u, v)
        0.0f, 1.0f, 0.0f,   0.5f, 1.0f,  // Vertice 1 (x, y, z, u, v)
        1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // Vertice 2 (x, y, z, u, v)
        0.0f, -1.0f, 1.0f,  0.5f, 0.0f   // Vertice 3 (x, y, z, u, v)
    };

    unsigned int indices[] = {
        0, 1, 2,        //Base da triangulo
        1, 2, 3,        //Base da direita
        0, 1, 3,        //Base da esquerda
        0, 2, 3         //Base de baixo
    };

    GLfloat floorVertices[] = {
        //x     y      z      u     v
        -10.0f, 0.0f, -10.0f, 0.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,  10.0f, 0.0f,
        10.0f, 0.0f, 10.0f,   10.0f, 10.0f,
        10.0f, 0.0f, -10.0f,  0.0f, 10.0f
    };

    unsigned int floorIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLfloat cubeVertices[] = {
        //x     y      z      u     v
        // Base
         1.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 0
         -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
         -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 2
         1.0f, 0.0f, -1.0f, 1.0f, 0.0f,// 3

         // Leste
         1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 4
         1.0f, 0.0f, -1.0f, 1.0f, 0.0f, // 5
         1.0f, 2.0f, 1.0f, 0.0f, 1.0f, // 6
         1.0f, 2.0f, -1.0f, 1.0f, 1.0f,// 7

         // Norte
         1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 8
         -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 9
         1.0f, 2.0f, 1.0f, 0.0f, 1.0f, // 10
         -1.0f, 2.0f, 1.0f, 1.0f, 1.0f,// 11

        // Oeste
        -1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 12
         -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, // 13
         -1.0f, 2.0f, 1.0f, 0.0f, 1.0f, // 14
         -1.0f, 2.0f, -1.0f, 1.0f, 1.0f,// 15

         // Sul
         -1.0f, 0.0f, -1.0f,  0.0f, 0.0f,  // 16
         1.0f, 0.0f, -1.0f, 1.0f, 0.0f, // 17
         -1.0f, 2.0f, -1.0f, 0.0f, 1.0f, // 18
         1.0f, 2.0f, -1.0f, 1.0f, 1.0f,// 19
         
         // Topo
         1.0f, 2.0f, 1.0f, 1.0f, 1.0f,  // 20
         -1.0f, 2.0f, 1.0f, 0.0f, 1.0f, // 21
         -1.0f, 2.0f, -1.0f, 0.0f, 0.0f, // 22
         1.0f, 2.0f, -1.0f, 1.0f, 0.0f  // 23
    };

    unsigned int cubeIndices[] = {
        // Base
        0, 1, 3,
        1, 2, 3,

        // Leste
        4, 5, 7,
        4, 6, 7,

        // Norte
        8, 9, 11,
        8, 10, 11,

        // Oeste
        12, 13, 15,
        12, 14, 15,

        // Sul
        16, 17, 19,
        16, 18, 19,

        // Topo
        20, 21, 23,
        21, 22, 23,
    };

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, sizeof(vertices), indices, sizeof(indices));
    meshList.push_back(obj2);

    Mesh* obj3 = new Mesh();
    obj3->CreateMesh(floorVertices, sizeof(floorVertices), floorIndices, sizeof(floorIndices));
    meshList.push_back(obj3);

    Mesh* obj4 = new Mesh();
    obj4->CreateMesh(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices));
    meshList.push_back(obj4);
}

void CreateShader() {
    Shader* shader = new Shader();
    shader->CreateFromFile(vertexLocation, fragmentLocation);
    shaderList.push_back(shader);
}

int main() {
    glfwSetErrorCallback(error_glfw);
    window = new Window(1024,768);
    window->Initialize();

    //Criar meu triangulo
    CriaTriangulos();
    CreateShader();

    //Criar nossa camera
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.05f, 0.01f);

    brickTexture = Texture((char *) "Textures/brick.png");
    dirtTexture = Texture((char*) "Textures/dirt.png");
    cubeTexture = Texture((char*)"Textures/cube.png");
    brickTexture.loadTexture();
    dirtTexture.loadTexture();
    cubeTexture.loadTexture();

    //Declaração de variaveis para movimentação do triangulo
    bool direction = true, sizeDirection = true, angleDirection = true;
    float triOffset = 0.0f, maxOffset = 0.7f, minOffset = -0.7f, incOffset = 0.01f;
    float size = 0.4f, maxSize = 0.7f, minSize = 0.0f, incSize = 0.01f;
    float angle = 0.0f, maxAngle = 360.0f, minAngle = 0.0f, incAngle = 0.1f;

    while (!window->shouldClose())
    {
        //Iniciar os eventos
        glfwPollEvents();

        //Camera Controlar o teclado e o mouse
        camera.KeyControl(window->getKeys());
        camera.MouseControl(window->getXChange(), window->getYChange());

        //Definindo uma cor para nossa janela
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Recriar a função
        Shader * shader = shaderList[0];
        shader->UseProgram();
                 
                /*
                * Calcula a fisica do nosso jogo
                */
                triOffset += direction ? incOffset : incOffset * -1;
                if (triOffset >= maxOffset || triOffset <= minOffset)
                    direction = !direction;

                size += sizeDirection ? incSize : incSize * -1;
                if (size >= maxSize || size <= minSize)
                    sizeDirection = !sizeDirection;

                angle += angleDirection ? incAngle : incAngle * -1;
                if (angle >= maxAngle || angle <= minAngle)
                    angleDirection = !angleDirection;

                /*
                * Piramide 1
                */
                //criar uma matriz 4x4 (1.0f)
                glm::mat4 model(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.5f)); //Movimentações do triangulo
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); //Tamanho do triangulo               
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotação
                
                glUniformMatrix4fv(shader->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(model)); //Passa os valores para a entrada uniform Model
                brickTexture.useTexture();
                meshList[0]->RenderMesh(); //Desenha o triangulo 3D (Renderização do piramide)

                /*
                * Piramide 2
                */
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, 0.75f, -2.5f)); //Movimentações do triangulo
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f)); //Tamanho do triangulo               
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, -1.0f, 0.0f)); //Rotação
                glUniformMatrix4fv(shader->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(model)); //Passa os valores para a entrada uniform Model
                brickTexture.useTexture();
                meshList[1]->RenderMesh();

                /*
                * Floor
                */
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -0.91f, 0.0f)); //Movimentações do chão
                glUniformMatrix4fv(shader->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(model)); //Passa os valores para a entrada uniform Model
                dirtTexture.useTexture();
                meshList[2]->RenderMesh();
                
                /*
                * Cube
                */
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.5f, -0.90f, -4.0f));
                glUniformMatrix4fv(shader->GetUniformModel(), 1, GL_FALSE, glm::value_ptr(model)); //Passa os valores para a entrada uniform Model
                cubeTexture.useTexture();
                meshList[3]->RenderMesh();

                /*
                * Calcula a Camera de Projeção 3D
                */
                //Calcular a projeção
                glm::mat4 projection = glm::perspective(1.0f, window->getBufferWidth() / window->getBufferHeigth(), 0.1f, 100.0f);
                //Passar o valor para a entrada uniform Projection
                glUniformMatrix4fv(shader->GetUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection));

                //Modificação da camera
                glUniformMatrix4fv(shader->GetUniformView(), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        glUseProgram(0);

        window->swapBuffers();
    }
}