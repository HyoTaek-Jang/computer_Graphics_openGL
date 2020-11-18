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
void mouseButtonCallback(GLFWwindow*, int, int, int);
void cursorMotionCallback(GLFWwindow*, double, double);

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 800, "201721070", 0, 0);
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorMotionCallback);

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
float cameraDistance = 3;
glm::vec3 sceneCenter = glm::vec3(0, 0, 0);
float cameraYaw = 0.f;
float cameraPitch = 0.f;
int lastX=0, lastY=0;
float cameraFov = 60.f;//필드오브뷰 카메라가 보는 각도, 크면 많이보이공ㅇ

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

    mat4 projMat = perspective(cameraFov * 3.141592f / 180, w / float(h), 0.01f, 100.f);

    vec3 cameraPosition = vec3(0,0,cameraDistance);
    cameraPosition = vec3(rotate(cameraPitch, vec3(-1, 0, 0)) * vec4(cameraPosition, 1));
    cameraPosition = vec3(rotate(cameraYaw, vec3(0, 1, 0)) * vec4(cameraPosition, 1));
    mat4 viewMat = lookAt(cameraPosition, sceneCenter, vec3(0, 1, 0));


    rotAngle += 0.1 / 180.f * 3.141592;
    GLuint loc = glGetUniformLocation(program.programID, "modelMat");

    mat4 rotMat = rotate(rotAngle, glm::vec3(0, 1, 0));
   // glUniformMatrix4fv(loc, 1, 0, value_ptr(rotMat));

    glUniformMatrix4fv(loc, 1, 0, value_ptr(rotate(90/180.f*3.141592f,vec3(1,0,0))));
   
    loc = glGetUniformLocation(program.programID, "viewMat");
    glUniformMatrix4fv(loc, 1, 0, value_ptr(viewMat));

    loc = glGetUniformLocation(program.programID, "projMat");
    glUniformMatrix4fv(loc, 1, 0, value_ptr(projMat));

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArray);
    glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = int(xpos);
        lastY = int(ypos);

        printf("%d %d \n", lastX, lastX);
    }
}



 void cursorMotionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            cameraFov += (ypos - lastY) / 300;
        }
        else {
        cameraPitch+= (ypos - lastY) / 300; //300픽셀 움직이면 1라디안
        cameraPitch = glm::clamp(cameraPitch, -2.f, 2.f); // 제한 걸어두는거
        cameraYaw -= (xpos - lastX) / 300;
        lastX = int(xpos);
        lastY = int(ypos);

        }
        }
    }

