// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define GLEW_STATIC
#include <GL/glew.h>
// 위에 것이 있어야 opengl 높은 버전을 쓸 수 있음.
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
    //윈도우 생성
    glfwMakeContextCurrent(window); 
    // 어떤 놈이 그리는 건지 정해주는거 , 컨택스트 잘 만들어졌나
    glewInit();
    // 오픈gl 최신버전 쓸 수 있음

    init();

    while (!glfwWindowShouldClose(window)) {
        // 윈도우가 닫힐 때까지. 그니까 안닫히면 저게 펄스임
        render(window);
        glfwPollEvents();
            // 움직임을 읽어서 보내줘라
    }

    glfwDestroyWindow(window);
    // 윈도우 없애고
    glfwTerminate();
    // glfw 종료
}


GLuint triBuffer = 0; // 전역변수 선언 이닛이랑 렌더에서 쓰려고
GLuint va = 0;
GLuint vao = 0; //버택스 어레이 오브젝트
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


    for (int i = 0; i<=360; i++) {
    float angle = i*3.141592 /180;
    float x = (float)cos(angle);
    float y = (float)sin(angle);
    triVertData.push_back(vec3(x, y, 0));
    triData.push_back(uvec3(0, i, i+1));
    ::count += 3;
    }

//    버텍스 데이터만 들어가있는 버퍼 1개만듬
    glGenBuffers(1, &triBuffer); // 버퍼 1개 트라이버퍼에 만듬
    glBindBuffer(GL_ARRAY_BUFFER, triBuffer); // 뭘 하려면 일단 바인드부터
    glBufferData(GL_ARRAY_BUFFER, triVertData.size() * sizeof(vec3), triVertData.data(), GL_STATIC_DRAW);//실제로 데이터를 보내줌
                                  //백터가 몇갠지 여기선 3개.//바이트 수로 넘겨줘야해서 sizeof를 사용함. 

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
