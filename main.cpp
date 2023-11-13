#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);                          //Deklaracje
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;                                                                 // Ustawienie szerokosci
const unsigned int SCR_HEIGHT = 600;                                                                // Ustawienie wysokosci


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    glfwInit();                                                                                     //inicjalizacja glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                                  //      |
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                                                  //      | konfiguracja wersji 3.3 core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                                  //      |

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);                                            //      Potrzebne dla apple
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);        //tworzenie okna
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);                              // ustawienie funkcji zmiany wilkosci okna

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))                                        // inicjalizacja glad
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(window))                                                          // petla renderowania
    {
        processInput(window);                                                                       //wywolanie funkcji input

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);                                                       //tworzenie bufferru
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);                                                                    //zmiana bufferu
        glfwPollEvents();                                                                           //zaciagniecie eventow(np. nacisniecie klawiszy/ myszki)
    }

    glfwTerminate();                                                                                //usuniecie zaalokowanych odwolan glfw
    return 0;
}

void processInput(GLFWwindow* window)                                                               //funkcja input
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)                                          //gdy klikniety esc to
        glfwSetWindowShouldClose(window, true);                                                     //wywolaj zamkniecie okna
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)                           // glfw: funkcja wywolywana za kazdym razem przy zmianie wielkosci okna
{
    glViewport(0, 0, width, height);                                                                // zmiana wielkosci viewporta
}