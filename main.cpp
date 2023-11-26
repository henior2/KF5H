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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

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

Game* Gry;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //wlaczenie depth testingu
    glEnable(GL_DEPTH_TEST);

    //SHADERY

    Gry = new Game(window, SCR_WIDTH, SCR_HEIGHT, SoundEngine);

    glfwSetCursorPosCallback(window, mouse_callback);
    

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

        Gry->Update(deltaTime);

        glfwSwapBuffers(window);  // zmiana bufferu
    }

    // usuniecie zaalokowanych odwolan glfw
    SoundEngine->drop();
    glfwTerminate();
    return 0;
}

// funkcja input
void processInput(GLFWwindow* window) {
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

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Gry->mouse_callback(window, xposIn, yposIn);
}

// glfw: funkcja wywolywana za kazdym razem przy zmianie wielkosci okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height)                           
{
    float w = width / 16.0f;
    float h = height / 9.0f;
    if (w > h) {
        w = h * 16.0f;
        h *= 9.0f;
    }
    else {
        h = w * 9.0f;
        w *= 16.0f;
    }

    float x = (width - w) / 2;
    float y = (height - h) / 2;
    glViewport(x, y, w, h); // zmiana wielkosci viewporta
}