#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // Framework for window and context management
#include <QApplication>
#include <QWidgets>
#include <QPushButton>
#include <iostream>

// Window size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Error callback for GLFW
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error: " << description << std::endl;
}

int main(int argc, char *argv[]) {

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set up error callback
    glfwSetErrorCallback(errorCallback);\

    // Create a GLFW window with OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
                         "3D Grapher", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    // tells OpenGL to use this window for rendering
    glfwMakeContextCurrent(window); 

    // Initialize GLEW (after creating the OpenGL context)
    // glewInit() loads OpenGL functions after the context is set.
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // set the viewport to match the window dimensions
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen with a color
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Dark blue bg
        glClear(GL_COLOR_BUFFER_BIT); // clears screen buffer

        // Swap the buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    QApplication app(argc, argv);

    QPushButton button("Hello, Qt!");
    button.resize(200, 100);
    button.show();

    return app.exec();

    // // Clean up and exit
    // glfwDestroyWindow(window);
    // glfwTerminate();
    // return 0;
}
