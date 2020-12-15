//201721070 장효택 컴퓨터그래픽스 과제 5. Texture Mapping + Bump Mapping

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "toys.h"
#include <glm/glm.hpp>
#include <vector>
#define GLM_EXPERIMENTAL
#include <j3a.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace glm;
using namespace std;

GLuint vertexArray = 0;
GLuint triBuffer = 0;
GLuint vao = 0;
GLuint elementArray = 0;
GLuint normalVBO = 0;
GLuint texCoordVBO = 0;
GLuint diffTexID = 0;
GLuint bumpTexID = 0;
float cameraDistance = 5;
glm::vec3 sceneCenter = glm::vec3(0, 0, 0);
float cameraYaw = 0.f;
float cameraPitch = 0.f;
int lastX = 0, lastY = 0;
float cameraFov = 60.f;
vec3 lightColor = vec3(1, 1, 1);
vec3 ambientLightColor = vec3(0.1, 0.09, 0.03);
vec4 diffusecolor = vec4(0.45, 1, 0.7, 1);
vec3 lightPos = vec3(3, 3, 3);
float shineness = 10;
vec4 specularMaterial = vec4(1);


Program program;


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



void init() {

    loadJ3A("Trex_m.j3a");
    program.loadShaders("shader.vert", "shader.frag");

    int texWidth, texHeight, texChannels;
    void* buffer = stbi_load(diffuseMap[0].c_str(), &texWidth, &texHeight, &texChannels, 4);

    glGenTextures(1, &diffTexID);
    glBindTexture(GL_TEXTURE_2D, diffTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(buffer);

    buffer = stbi_load(bumpMap[0].c_str(), &texWidth, &texHeight, &texChannels, 4);

    glGenTextures(1, &bumpTexID);
    glBindTexture(GL_TEXTURE_2D, bumpTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(buffer);


    glGenBuffers(1, &triBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), vertices[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec3), normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(glm::vec2), texCoords[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &elementArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArray);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(glm::u32vec3), triangles[0], GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB); //SRGB로 보여줘!
}


void render(GLFWwindow* window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program.programID);

    GLuint loc = glGetUniformLocation(program.programID, "projMat");
    mat4 projMat = perspective(cameraFov * 3.141592f / 180, w / float(h), 0.01f, 100.f);
    glUniformMatrix4fv(loc, 1, 0, value_ptr(projMat));

    loc = glGetUniformLocation(program.programID, "viewMat");
    vec3 cameraPosition = vec3(0, 0, cameraDistance);
    cameraPosition = vec3(rotate(cameraPitch, vec3(-1, 0, 0)) * vec4(cameraPosition, 1));
    cameraPosition = vec3(rotate(cameraYaw, vec3(0, 1, 0)) * vec4(cameraPosition, 1));
    mat4 viewMat = lookAt(cameraPosition, sceneCenter, vec3(0, 1, 0));
    glUniformMatrix4fv(loc, 1, 0, value_ptr(viewMat));


    loc = glGetUniformLocation(program.programID, "cameraPos");
    glUniform3fv(loc, 1, value_ptr(cameraPosition));

    loc = glGetUniformLocation(program.programID, "shineness");
    glUniform1f(loc, shineness);

    loc = glGetUniformLocation(program.programID, "diffusecolor");
    glUniform4fv(loc, 1, value_ptr(diffusecolor));

    loc = glGetUniformLocation(program.programID, "lightPos");
    glUniform3fv(loc, 1, value_ptr(lightPos));

    loc = glGetUniformLocation(program.programID, "lightColor");
    glUniform3fv(loc, 1, value_ptr(lightColor));

    loc = glGetUniformLocation(program.programID, "specularMaterial");
    glUniform4fv(loc, 1, value_ptr(specularMaterial));

    loc = glGetUniformLocation(program.programID, "ambientLightColor");
    glUniform3fv(loc, 1, value_ptr(ambientLightColor));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffTexID);
    loc = glGetUniformLocation(program.programID, "diffTex");
    glUniform1i(loc, 0);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bumpTexID);
    loc = glGetUniformLocation(program.programID, "bumpTex");
    glUniform1i(loc, 1);

    glBindVertexArray(vertexArray);
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

float pitchAngleN = -3.141592 / 2 + 0.01f;
float pitchAngleP = 3.141592 / 2 - 0.01f;


void cursorMotionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            //cameraFov += (ypos - lastY) / 300;
            cameraDistance *= pow(1.001, ypos - lastY);
            lastY = int(ypos);
        }
        else {
            cameraPitch += (ypos - lastY) / 300;
            cameraPitch = glm::clamp(cameraPitch, pitchAngleN, pitchAngleP);
            cameraYaw -= (xpos - lastX) / 300;
            lastX = int(xpos);
            lastY = int(ypos);

        }
    }
}

