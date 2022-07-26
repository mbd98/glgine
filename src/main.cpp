#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <defaults.hpp>
#include <callbacks.hpp>
#include <camera.hpp>
#include <renderutils.hpp>
#include <shaders.hpp>
#include <textures.hpp>

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

static GLFWwindow *window;
static Camera camera;

static double cursorLastX;
static double cursorLastY;
static double lastTime;
static double dt;

int main(int argc, char *argv[])
{
	int width = 1024, height = 768;
	if (const char *ws = std::getenv("WINDOW_WIDTH"))
		width = std::stoi(ws);
	if (const char *hs = std::getenv("WINDOW_HEIGHT"))
		height = std::stoi(hs);

	if (glfwInit() == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return EXIT_FAILURE;
	}
	glfwSetErrorCallback(handleGlfwError);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	window = glfwCreateWindow(width, height, "Mark Boudreau (40078697)", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		std::cerr << "Failed to initialize window" << std::endl;
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, handleGlfwKey);
	glfwSetCursorPosCallback(window, handleGlfwCursorPos);
	glfwSetCursorEnterCallback(window, handleGlfwCursorEnter);
	glfwSetMouseButtonCallback(window, handleGlfwMouseButton);
	glfwSetScrollCallback(window, handleGlfwScroll);
	glfwSetFramebufferSizeCallback(window, handleGlfwFramebufferSize);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSwapInterval(1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	camera.setAspectRatio((float)width / (float)height);
	camera.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	key_handlers.push_back([](GLFWwindow*, int key, int, int action, int mods){
		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translateForward(dt);
		}
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translateForward(-dt);
		}
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translateRight(-dt);
		}
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translateRight(dt);
		}
		if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			camera.translateUp((mods & GLFW_MOD_CONTROL ? -1.0f : 1.0f) * dt);
		}
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		if ((mods & GLFW_MOD_CONTROL) && key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		{
			camera.setTranslateSpeed(5.0f);
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			camera.setTranslateSpeed(1.0f);
		}
	});
	fb_size_handlers.push_back([](GLFWwindow*, int width, int height){
		camera.setAspectRatio((float)width / (float)height);
	});
	mouse_button_handlers.push_back([](GLFWwindow*, int button, int action, int mods){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	});
	glfwGetCursorPos(window, &cursorLastX, &cursorLastY);
	cursor_pos_handlers.push_back([](GLFWwindow*, double x, double y) {
		double dx = x - cursorLastX;
		double dy = y - cursorLastY;
		cursorLastX = x;
		cursorLastY = y;
		if (dx != 0)
			camera.rotateRight(dx * dt);
		if (dy != 0)
			camera.rotateUp(dy * dt);
	});

	GLuint depthMap, depthMapFbo;

	Renderable floor, cube;
	GLuint snowTexture = loadTexture("assets/textures/snow.png");
	GLuint sceneShader = loadShader("assets/shaders/illuminated.vert", "assets/shaders/illuminated.frag");
	GLuint shadowShader = loadShader("assets/shaders/shadow.vert", "assets/shaders/shadow.frag");

	createSquare(floor, sceneShader, shadowShader, snowTexture);
	createCube(cube, sceneShader, shadowShader, snowTexture);
	floor.model = glm::scale(floor.model, glm::vec3(100.0f));
	cube.model = glm::translate(cube.model, glm::vec3(0.0f, 2.0f, 0.0f));

	glm::vec3 lightPosition(0.0f, 6.0f, 0.0f);
	glm::vec3 lightFocus(0.0f, 0.0f, 0.0f);
	glm::vec3 lightDirection = glm::normalize(lightFocus - lightPosition);
	float lightNear = 1.0f;
	float lightFar = 180.0f;

	glGenFramebuffers(1, &depthMapFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		int scrWidth, scrHeight;
		glfwGetWindowSize(window, &scrWidth, &scrHeight);
		dt = glfwGetTime() - lastTime;
		lastTime += dt;
		setUniformMat4(sceneShader, "light", glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNear, lightFar)
		                                     * glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f)));
		setUniformMat4(shadowShader, "light", glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNear, lightFar)
		                                     * glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f)));
		setUniformVec3(sceneShader, "lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		setUniformVec3(sceneShader, "lightPosition", lightPosition);
		setUniformVec3(sceneShader, "viewPosition", camera.getPosition());
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);
		shade(cube, camera, depthMap);
		shade(floor, camera, depthMap);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, scrWidth, scrHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render(cube, camera, depthMap);
		render(floor, camera, depthMap);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return EXIT_SUCCESS;
}
