//201721070 장효택 컴퓨터그래픽스 과제 2. Rotating Bunny


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "toys.h"
#include <vector>
#define GLM_EXPERIMENTAL
#include <j3a.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>



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
GLuint vertexArray = 0;
GLuint vao = 0;
GLuint elementArray = 0;
int count = 0;

Program program;

using namespace glm;
using namespace std;

void init() {

    loadJ3A("bunny.j3a");
    program.loadShaders("shader.vert", "shader.frag");

    glGenBuffers(1, &triBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &elementArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArray);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);

}

float rotAngle = 0;

void render(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program.programID);
    glBindVertexArray(vertexArray);

    rotAngle += 0.1 / 180.f * 3.141592;
    GLuint loc = glGetUniformLocation(program.programID, "modelMat");
    mat4 rotMat = rotate(rotAngle, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(loc, 1, 0, value_ptr(rotMat));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArray);
    glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

}
