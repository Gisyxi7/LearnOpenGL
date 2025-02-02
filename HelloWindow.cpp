#include <glad/glad.h>  // 要放在GLFW头文件之前包含
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // 1. 初始化 GLFW 库
    if (!glfwInit()) {
        std::cerr << "GLFW 初始化失败！" << std::endl;
        return -1;
    }

    // 2. 设置 GLFW 的窗口提示，指定 OpenGL 的版本和上下文属性
    // 这里设置使用 OpenGL 3.3 核心模式
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);          // 主版本号：3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);          // 次版本号：3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式

    // 3. 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL窗口 (使用GLAD)", nullptr, nullptr);
    if (!window) {
        std::cerr << "窗口创建失败！" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 4. 将窗口的上下文设置为当前线程的上下文
    glfwMakeContextCurrent(window);
    // 设置视口回调函数，确保视口保持和窗口大小一致
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 5. 初始化 GLAD，加载 OpenGL 函数指针
    // glfwGetProcAddress 是一个获取 OpenGL 函数地址的函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD 初始化失败！" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 7. 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 7.1 处理用户输入（例如：按下 ESC 键退出程序）
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // 7.2 渲染操作
        // 设置清除颜色（背景颜色），例如深青色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清除颜色缓冲区
        glClear(GL_COLOR_BUFFER_BIT);

        // 7.3 交换缓冲区，将渲染结果显示到屏幕上
        glfwSwapBuffers(window);

        // 7.4 处理事件（如键盘、鼠标、窗口大小改变等）
        glfwPollEvents();
    }

    // 8. 清理资源，关闭程序
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}