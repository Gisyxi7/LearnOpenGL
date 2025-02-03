cmake_minimum_required(VERSION 3.27)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

function(My_add_executable target_name)
    add_executable(${target_name} ${target_name}.cpp src/glad.c)
    target_link_libraries(${target_name} ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)
endfunction()

My_add_executable(main)
My_add_executable(HelloWindow)
My_add_executable(HelloTriangle)
My_add_executable(Shaders)
My_add_executable(Texture)
My_add_executable(Transformations)