#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader/shader.h>

#include <iostream>
#include <valarray>

// 窗口尺寸变化时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理用户输入的函数
void processInput(GLFWwindow *window);

// 设置窗口的宽度和高度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: 初始化并配置
    // ------------------------------
    glfwInit();  // 初始化 GLFW 库
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // 设置 OpenGL 主要版本为 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 设置 OpenGL 次版本为 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 设置 OpenGL 使用核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS 系统需要设置兼容性
#endif

    // 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();  // 创建失败时终止 GLFW
        return -1;
    }
    glfwMakeContextCurrent(window);  // 创建当前窗口的 OpenGL 上下文
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 注册窗口大小变化的回调函数

    // glad: 加载所有 OpenGL 函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 编译和构建着色器程序
    // ------------------------------------
    Shader ourshader("shader.vert","shader.frag");

    // 设置顶点数据并配置顶点属性
    // ------------------------------------------------------------------
    float vertices[] = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top

    };

    // 创建 VAO、VBO 和 EBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);  // 创建 VAO
    glGenBuffers(1, &VBO);  // 创建 VBO

    // 绑定 VAO，先绑定 VAO，再绑定和设置顶点缓冲区及元素缓冲区，最后配置顶点属性
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 将顶点数据传输到 GPU

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 解绑缓冲区，防止意外修改
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解绑 VBO
    // 不需要解绑 EBO，因为它已经绑定到 VAO 中

    // 解绑 VAO
    glBindVertexArray(0);  // 解绑 VAO，防止不小心修改当前的 VAO 配置

    ourshader.use();
    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 输入处理
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 设置背景色
        glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓冲区

        glBindVertexArray(VAO);  // 绑定 VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0);  // 不需要每次都解绑 VAO，除非有特殊情况

        // 交换缓冲区并处理事件（例如键盘、鼠标等）
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ourshader.deleteProgramme();

    // 终止 GLFW，释放所有资源
    glfwTerminate();
    return 0;
}

// 处理输入：例如按下 ESC 键时关闭窗口
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);  // 更新视口大小
}
