#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Game.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <time.h>

//Deklaracje
void framebuffer_size_callback(GLFWwindow* window, int width, int height);                          
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;  // Ustawienie szerokosci
const unsigned int SCR_HEIGHT = 600;  // Ustawienie wysokosci

bool fullscreen = false;
bool fullscreenCtx = true;

// zczytywanie danych o monitorze - potrzebne do fullscreen
GLFWmonitor* monitor;
const GLFWvidmode* monitorMode;

// width, height i pozycja dla trybu okienkowego
int windowPrevW, windowPrevH;
int windowPrevX, windowPrevY;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    srand(time(NULL));
    // inicjalizacja glfw
    //      | konfiguracja wersji 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    monitor = glfwGetPrimaryMonitor();
    monitorMode = glfwGetVideoMode(monitor);

    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "KF5H - Gry wektorowe", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // ustawienie funkcji zmiany wilkosci okna

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // inicjalizacja glad
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //wlaczenie depth testingu
    glEnable(GL_DEPTH_TEST);


    //SHADERY


    Shader ourProgram("VertexShader.txt", "FragmentShader.txt");

    Game Gry(window, &ourProgram);

    //WERTEX


    //wertexy
    float vertices[] = {
    // --pozycja         -- kolor          --
        0.5f,  0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  // top right front
         0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // bottom right front
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // bottom left front
        -0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // top left front
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // top right back
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right back
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom left back
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f  // top left back
    };

    //buffer
    unsigned int indices[] = {
        0, 1, //front
        1, 2,
        2, 3,
        3, 0,

        4, 5, //back
        5, 6,
        6, 7,
        7, 4,

        0, 4, //front to back
        1, 5,
        2, 6,
        3, 7
    };

    //pozycje szescianow w swiecie
    std::vector<glm::vec3> cubePositions;

    for (int i = 0; i < 346; i++) {
        float x = ((float)(rand() % 50) - 25.0f);
        float y = ((float)(rand() % 50) - 25.0f);
        float z = ((float)(rand() % 100) - 100.0f);
        cubePositions.push_back(glm::vec3(x, y, z));
    }


    for (int i = 0; i < 4; i++) {
        float z = ((float)(rand() % 100) - 100.0f);
        cubePositions.push_back(glm::vec3(0.0f, 0.0f, z));
    }
    

    //rysowanie w wireframe mode.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ustawienie szerokosci lini
    glLineWidth(10.0f);
    glPointSize(10.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;


    //PETLA


    while (!glfwWindowShouldClose(window))  // petla renderowania
    {
        //creating deltatime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();  // zaciagniecie eventow(np. nacisniecie klawiszy/ myszki)

        processInput(window);  // wywolanie funkcji input

        //aktywacja programu

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // tworzenie bufferru
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourProgram.use();

        //tworzenie transformow
        glm::mat4 viev = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        viev = glm::translate(viev, glm::vec3(0.0f, 0.0f, 0.0f));

        //przekazanie transformow do shaderow
        ourProgram.setMat4("projection", projection);
        ourProgram.setMat4("viev", viev);

        Gry.Update(deltaTime);

        glfwSwapBuffers(window);  // zmiana bufferu
    }

    // usuniecie zaalokowanych odwolan glfw
    glfwTerminate();
    return 0;
}

// funkcja input
void processInput(GLFWwindow* window) {
    // gdy klikniety esc to wywolaj zamkniecie okna
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // gdy klikniety f11 lub f4 to przelacz fullscreen
    if ((glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS) && fullscreenCtx)
    {
        fullscreenCtx = false;
        fullscreen = !fullscreen;
        if (fullscreen)
        {
            glfwGetWindowSize(window, &windowPrevW, &windowPrevH);
            glfwGetWindowPos(window, &windowPrevX, &windowPrevY);
            glfwSetWindowMonitor(window, monitor, 0, 0, monitorMode->width, monitorMode->height, monitorMode->refreshRate);
            framebuffer_size_callback(window, monitorMode->width, monitorMode->height);

        }
        else
        {
            glfwSetWindowMonitor(window, nullptr, windowPrevX, windowPrevY, windowPrevW, windowPrevH, GLFW_DONT_CARE);
            framebuffer_size_callback(window, windowPrevW, windowPrevH);
        }
    }
    
    if ((glfwGetKey(window, GLFW_KEY_F11) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_F4) == GLFW_RELEASE))
    {
        fullscreenCtx = true;
    }
        
}

// glfw: funkcja wywolywana za kazdym razem przy zmianie wielkosci okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height)                           
{
    glViewport(0, 0, width, height); // zmiana wielkosci viewporta
}