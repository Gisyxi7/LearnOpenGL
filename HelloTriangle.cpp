#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// 窗口尺寸变化时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理用户输入的函数
void processInput(GLFWwindow *window);

// 设置窗口的宽度和高度
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器代码（GLSL）
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"  // 顶点位置输入
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"  // 设置顶点位置
                                 "}\0";

// 片段着色器代码（GLSL）
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"  // 输出颜色变量
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"  // 设置片段的颜色为橙色
                                   "}\n\0";

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
    // 顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);  // 设置顶点着色器源码
    glCompileShader(vertexShader);  // 编译顶点着色器
    // 检查编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);  // 设置片段着色器源码
    glCompileShader(fragmentShader);  // 编译片段着色器
    // 检查编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 链接着色器程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);  // 将顶点着色器附加到程序
    glAttachShader(shaderProgram, fragmentShader);  // 将片段着色器附加到程序
    glLinkProgram(shaderProgram);  // 链接着色器程序
    // 检查链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);  // 删除编译好的顶点着色器
    glDeleteShader(fragmentShader);  // 删除编译好的片段着色器

    // 设置顶点数据并配置顶点属性
    // ------------------------------------------------------------------
    float vertices[] = {
            0.5f,  0.5f, 0.0f,  // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f,  // 左下角
            -0.5f,  0.5f, 0.0f   // 左上角
    };
    unsigned int indices[] = {  // 注意：索引从 0 开始！
            0, 1, 3,  // 第一个三角形
            1, 2, 3   // 第二个三角形
    };

    // 创建 VAO、VBO 和 EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);  // 创建 VAO
    glGenBuffers(1, &VBO);  // 创建 VBO
    glGenBuffers(1, &EBO);  // 创建 EBO

    // 绑定 VAO，先绑定 VAO，再绑定和设置顶点缓冲区及元素缓冲区，最后配置顶点属性
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 将顶点数据传输到 GPU

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // 将索引数据传输到 GPU

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  // 配置顶点位置属性（位置索引为 0）
    glEnableVertexAttribArray(0);  // 启用顶点属性

    // 解绑缓冲区，防止意外修改
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // 解绑 VBO
    // 不需要解绑 EBO，因为它已经绑定到 VAO 中

    // 解绑 VAO
    glBindVertexArray(0);  // 解绑 VAO，防止不小心修改当前的 VAO 配置

    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 输入处理
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 设置背景色
        glClear(GL_COLOR_BUFFER_BIT);  // 清除颜色缓冲区

        // 使用着色器程序并绘制图形
        glUseProgram(shaderProgram);  // 使用着色器程序
        glBindVertexArray(VAO);  // 绑定 VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // 使用 EBO 绘制图形
        // glBindVertexArray(0);  // 不需要每次都解绑 VAO，除非有特殊情况

        // 交换缓冲区并处理事件（例如键盘、鼠标等）
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

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
