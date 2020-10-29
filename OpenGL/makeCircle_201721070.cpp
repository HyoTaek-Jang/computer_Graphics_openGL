// 201721070 장효택 원그리기 과제
//
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "toys.h"
#include <vector>



void render(GLFWwindow* window);
void init();

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 800, "201721070", 0, 0);
    glfwMakeContextCurrent(window);
    glewInit();

    init();

    while (!glfwWindowShouldClose(window)) {
        render(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

GLuint triBuffer = 0;
GLuint va = 0;
GLuint vao = 0;
GLuint elementArray = 0;
int count = 0;

Program program;

using namespace glm;
using namespace std;

void init() {
    program.loadShaders("shader.vert", "shader.frag");
    vector<vec3> triVertData;
    std::vector<uvec3> triData;
    triVertData.push_back(vec3(0, 0, 0));

    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.141592 / 180;
        float x = (float)cos(angle);
        float y = (float)sin(angle);
        triVertData.push_back(vec3(x, y, 0));
        triData.push_back(uvec3(0, i, i + 1));
        ::count += 3;
    }

    glGenBuffers(1, &triBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
    glBufferData(GL_ARRAY_BUFFER, triVertData.size() * sizeof(vec3), triVertData.data(), GL_STATIC_DRAW);


    glGenVertexArrays(1, &va);
    glBindVertexArray(va);
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &elementArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArray);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triData.size() * sizeof(uvec3), triData.data(), GL_STATIC_DRAW);
}


void render(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program.programID);
    glBindVertexArray(va);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArray);
    glDrawElements(GL_TRIANGLES, ::count, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

}
