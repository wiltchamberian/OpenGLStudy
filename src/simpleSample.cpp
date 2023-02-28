#include "tools.h"
#include "simpleSample.h"
#include <vector>
#include <iostream>
#include <thread>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void run_simple_sample() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    auto win = glfwCreateWindow(800, 600, "simpleSample", NULL, NULL);
    if (win == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        const char** description = nullptr;
        glfwGetError(description);
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw(std::runtime_error("open_load_fail!"));
	}

	init();

	float lastTime = 0;
	const float fps = 20;
	float framePeriod = 1000 / fps;
	float sleepDuration = 1;// 1ms
	while (!glfwWindowShouldClose(win)) {
		float startTime = globalMilliseconds();
		int time = globalMilliseconds();
		float dtime = (time - lastTime) * 0.001;
		lastTime = time;

		//rendering
		display(nullptr);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(win);

		//控制帧率，为了防止sleep超时，采用分段小间隔sleep的方式
		//到临近帧周期时，不间断轮询判断
		int endTime = globalMilliseconds();
		int frameTime = endTime - startTime;
		do {
			if (framePeriod - frameTime > sleepDuration) {
				std::this_thread::sleep_for(std::chrono::milliseconds((int)sleepDuration));
			}
			else {
				while (framePeriod > frameTime) {
					endTime = globalMilliseconds();
					frameTime = endTime - startTime;
				}
				break;
			}

			//
			endTime = globalMilliseconds();
			frameTime = endTime - startTime;
		} while (true);
	}

	return ;
}

void init() {
	std::vector<float> points = { -0.5,-0.5,0,0.5,-0.5,0,0.5,0.5,0,
	0.5,0.5,0,-0.5,0.5,0,-0.5,-0.5,0};

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, points.size()* sizeof(float), points.data(), GL_STATIC_DRAW);

	GLuint pro = createProgram("vertex.glsl", "fragment.glsl");
	glUseProgram(pro);

	GLuint vPos = glGetAttribLocation(pro, "vPosition");
	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void display(void* userData) {
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFlush();
}

