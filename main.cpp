#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// 用于存储矩形的位置
float playerPosX = SCREEN_WIDTH / 2.0f;
float playerPosY = SCREEN_HEIGHT / 2.0f;
float playerSpeed = 0.1f;

// 顶点数据
float vertices[] = {
        // 位置          // 颜色
        -0.05f, -0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        0.05f,  0.05f, 0.0f, 0.0f, 1.0f,

        -0.05f, -0.05f, 1.0f, 0.0f, 0.0f,
        0.05f,  0.05f, 0.0f, 0.0f, 1.0f,
        -0.05f,  0.05f, 0.0f, 1.0f, 0.0f,
};

// 着色器代码
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    ourColor = aColor;
})";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(ourColor, 1.0);
})";
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
// 初始化 GLFW 和 GLAD
GLFWwindow* initWindow(int width, int height, const char* title) {
    // 初始化 GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // 创建窗口
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}



GLuint createShader(const char* vertexSource, const char* fragmentSource) {
    // 编译着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    // 创建程序并链接着色器
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        playerPosY += playerSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        playerPosY -= playerSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        playerPosX -= playerSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        playerPosX += playerSpeed;
}

int main() {
    // 初始化窗口
    GLFWwindow* window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple Game");
    if (!window)
        return -1;

    // 创建着色器程序
    GLuint shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);

    // 创建 VAO 和 VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 游戏主循环
    while (!glfwWindowShouldClose(window)) {
        // 处理输入
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 激活着色器
        glUseProgram(shaderProgram);

        // 更新矩形位置
        glBindVertexArray(VAO);
        glPushMatrix();
        glTranslatef(playerPosX / SCREEN_WIDTH, playerPosY / SCREEN_HEIGHT, 0.0f); // 归一化坐标
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glPopMatrix();
        glBindVertexArray(0);

        // 交换缓冲区和轮询事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
