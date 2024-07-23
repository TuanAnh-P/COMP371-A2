#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // Input vertex attribute
"uniform mat4 transform;\n" // Uniform matrix for transformations
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n" // Apply transformation
"}\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" // Output fragment color
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n" // Set color to a specific value
"}\n\0";

// Function to process input and update transformation matrix
void processInput(GLFWwindow* window, glm::mat4 &transform);

int main()
{
    // Initialize GLFW library
    glfwInit();

    // Specify OpenGL version 3.3 and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 800, "A2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW to manage OpenGL extensions
    glewInit();

    // Set the viewport to cover the entire window
    glViewport(0, 0, 800, 800);

    // Compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders to create a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete the shader objects after linking them into the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define the vertices for a triangle
    GLfloat verticesTriangle[] =
    {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    // Generate and bind Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind and set the VBO's data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangle), verticesTriangle, GL_STATIC_DRAW);

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO to avoid unintended modifications
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Initialize the transformation matrix to the identity matrix
    glm::mat4 transform = glm::mat4(1.0f);

    // Main loop to keep the window open until the user closes it
    while (!glfwWindowShouldClose(window))
    {
        // Process user input and update the transformation matrix
        processInput(window, transform);

        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Set the transformation matrix in the shader
        GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // Draw the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Swap the front and back buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up and delete all the objects we've created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// Function to process input and update transformation matrix
void processInput(GLFWwindow* window, glm::mat4 &transform) {
    // Translation distance per frame
    const float translationDistance = 0.01f;
    // Rotation angle in radians
    const float rotationAngle = glm::radians(30.0f);
    // Scaling factor per frame
    const float scaleFactor = 1.01f;

    // Close window if ESC key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // Move up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(0.0f, translationDistance, 0.0f));
    // Move down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(0.0f, -translationDistance, 0.0f));
    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(-translationDistance, 0.0f, 0.0f));
    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform = glm::translate(transform, glm::vec3(translationDistance, 0.0f, 0.0f));
    // Rotate clockwise
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        transform = glm::rotate(transform, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    // Rotate counterclockwise
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        transform = glm::rotate(transform, -rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
    // Scale up
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        transform = glm::scale(transform, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    // Scale down
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        transform = glm::scale(transform, glm::vec3(1.0f / scaleFactor, 1.0f / scaleFactor, 1.0f / scaleFactor));
}
