#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <defaults.hpp>
#include <callbacks.hpp>
#include <camera.hpp>
#include <complexrenderable.hpp>
#include <shaders.hpp>
#include <model.hpp>
#include <renderutils.hpp>

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

static GLFWwindow *window;
static Camera freeCamera, trainCamera, *currentCamera;

static double cursorLastX;
static double cursorLastY;
static double lastTime;
static double dt;
static bool doShadows = true;
static bool doLights = true;
static bool doTrainLoop = true;
static bool isTrainCamera = false;

static void key(GLFWwindow*, int key, int, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		freeCamera.translateForward(dt);
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		freeCamera.translateForward(-dt);
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		freeCamera.translateRight(-dt);
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		freeCamera.translateRight(dt);
	}
	if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		freeCamera.translateUp((mods & GLFW_MOD_CONTROL ? -1.0f : 1.0f) * dt);
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
		freeCamera.setTranslateSpeed(5.0f);
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		freeCamera.setTranslateSpeed(1.0f);
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		doShadows = !doShadows;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		doLights = !doLights;
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		freeCamera.setPosition(glm::vec3(0.0f));
		freeCamera.setYaw(glm::radians(-90.0f));
		freeCamera.setPitch(0.0f);
		currentCamera = &freeCamera;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		doTrainLoop = !doTrainLoop;
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		isTrainCamera = !isTrainCamera;
		if (isTrainCamera)
			currentCamera = &trainCamera;
		else
			currentCamera = &freeCamera;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		std::cerr << "Current camera at (" <<
		currentCamera->getPosition().x << "," <<
		currentCamera->getPosition().y << "," <<
		currentCamera->getPosition().z << ")" << std::endl;
	}
}

static void framebuf(GLFWwindow*, int width, int height)
{
	freeCamera.setAspectRatio((float)width / (float)height);
}

static void mouseclick(GLFWwindow*, int button, int action, int mods)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

static void cursor(GLFWwindow*, double x, double y)
{
	double dx = x - cursorLastX;
	double dy = y - cursorLastY;
	cursorLastX = x;
	cursorLastY = y;
	if (dx != 0)
		currentCamera->rotateRight(dx * dt);
	if (dy != 0)
		currentCamera->rotateUp(dy * dt);
}

int main(int argc, char *argv[])
{
	// init
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

	key_handlers.push_back(key);
	fb_size_handlers.push_back(framebuf);
	mouse_button_handlers.push_back(mouseclick);
	glfwGetCursorPos(window, &cursorLastX, &cursorLastY);
	cursor_pos_handlers.push_back(cursor);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


	glfwSwapInterval(1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// camera
	freeCamera.setAspectRatio((float)width / (float)height);
	freeCamera.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));

	// renderable objects
	Reusable *train, *track;
	Model *preTrain, *preTrack, *floor, *backWall, *ceiling;
	ComplexRenderable *portal0, *portal1, *trainCamMarker;
	GLuint sceneShader = loadShader("assets/shaders/illuminated.vert", "assets/shaders/illuminated.frag");
	GLuint shadowShader = loadShader("assets/shaders/shadow.vert", "assets/shaders/shadow.frag");

	preTrain = new Model("train");
	preTrain->setScales(glm::vec3(1.1f));
	preTrain->setAngles(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
	preTrain->setPosition(glm::vec3(0.0f, 2.3f, 0.0f));
	train = new Reusable(preTrain);
	glm::vec3 train0pos;
	glm::vec3 train1pos;
	uint trainSteps = 0;

	preTrack = new Model("track");
	preTrack->setAngles(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
	preTrack->setScales(glm::vec3(0.25f));
	track = new Reusable(preTrack);
	for (int i = -2; i < 5; i++)
	{
		track->insertTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, i * -6.4f)));
	}

	floor = new Model("wall");
	floor->setScales(glm::vec3(3.0f, 1.0f, 10.0f));

	ceiling = new Model("wall");
	ceiling->setScales(glm::vec3(3.0f, 1.0f, 10.0f));
	ceiling->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	ceiling->setAngles(glm::vec3(0.0f, 0.0f, glm::radians(180.0f)));

	backWall = new Model("wall");
	backWall->setScales(glm::vec3(3.0f, 1.0f, 10.0f));
	backWall->setAngles(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));
	backWall->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));

	portal0 = new SimpleComplexRenderable(createCuboid(glm::vec3(-0.5f), glm::vec3(1.0f)));
	portal0->setScales(glm::vec3(6.0f, 6.0f, 0.01f));
	portal0->setPosition(glm::vec3(0.0f, 2.0f, -8.0f));
	portal1 = new SimpleComplexRenderable(createCuboid(glm::vec3(-0.5f), glm::vec3(1.0f)));
	portal1->setScales(glm::vec3(6.0f, 6.0f, 0.01f));
	portal1->setPosition(glm::vec3(0.0f, 2.0f, 8.0f));
	portal1->setAngles(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
	//portal1->setAngles(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));

	trainCamMarker = new SimpleComplexRenderable(createCuboid(glm::vec3(-0.1f), glm::vec3(0.2f)));
	trainCamMarker->setTexture(loadTexture("assets/models/wall/floortile1_baseColor.png"));

	// lights
	glm::vec3 lightPosition(0.0f, 4.0f, 30.0f);
	glm::vec3 lightFocus(0.0f, 0.0f, 0.0f);
	glm::vec3 lightDirection = glm::normalize(lightFocus - lightPosition);
	float lightInner = glm::radians(20.0f);
	float lightOuter = glm::radians(50.0f);
	float lightNear = 1.0f;
	float lightFar = 180.0f;

	// shadow framebuffer
	GLuint depthMap, depthMapFbo;
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

	// portal framebuffers
	GLuint portal0fbo, portal0tex, portal0depth, portal1fbo, portal1tex, portal1depth;
	glGenFramebuffers(1, &portal0fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, portal0fbo);
	glGenTextures(1, &portal0tex);
	glBindTexture(GL_TEXTURE_2D, portal0tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenRenderbuffers(1, &portal0depth);
	glBindRenderbuffer(GL_RENDERBUFFER, portal0depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, portal0depth);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, portal0tex, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &portal1fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, portal1fbo);
	glGenTextures(1, &portal1tex);
	glBindTexture(GL_TEXTURE_2D, portal1tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenRenderbuffers(1, &portal1depth);
	glBindRenderbuffer(GL_RENDERBUFFER, portal1depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, portal1depth);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, portal1tex, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// portal cameras
	Camera portal0cam;
	portal0cam.setPosition(portal0->getPosition());
	portal0cam.setFar(10.0f);
	portal0cam.setAspectRatio(1.0f);
	portal0cam.setFov(glm::radians(45.0f));

	Camera portal1cam;
	portal1cam.setPosition(portal1->getPosition());
	portal1cam.setFar(10.0f);
	portal1cam.setAspectRatio(1.0f);
	portal1cam.setFov(glm::radians(45.0f));
	portal1cam.setYaw(glm::radians(90.0f));

	// time
	lastTime = glfwGetTime();
	currentCamera = &freeCamera;

	std::cerr << "Done loading, begin render loop" << std::endl;
	// loop
	while (!glfwWindowShouldClose(window))
	{
		int scrWidth, scrHeight;
		glfwGetWindowSize(window, &scrWidth, &scrHeight);
		dt = glfwGetTime() - lastTime;
		lastTime += dt;

		// train loop
		if (doTrainLoop)
		{
			train0pos = glm::vec3(0.0f, 0.0f, trainSteps * 0.1f);
			train1pos = glm::vec3(0.0f, 0.0f, -17.0f + trainSteps * 0.1f);
			if (train0pos.z >= portal1->getPosition().z)
				trainCamera.setPosition(train1pos + glm::vec3(-0.6372f, 3.28f, 0.0f));
			else
				trainCamera.setPosition(train0pos + glm::vec3(-0.6372f, 3.28f, 0.0f));
			trainCamMarker->setPosition(trainCamera.getPosition());

			std::vector<glm::mat4> trainTransforms = {
					glm::translate(glm::mat4(1.0f), train0pos),
					glm::translate(glm::mat4(1.0f), train1pos)};
			train->setTransforms(trainTransforms);
			trainSteps = (trainSteps + 1) % 170;
		}

		// draw depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(shadowShader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		setUniformInt(shadowShader, SHADOW_MAP, 0);
		setUniformMat4(shadowShader, LIGHT, glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNear, lightFar)
		                                    * glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f)));

		if (doShadows)
		{
			track->render(shadowShader);
			train->render(shadowShader);
			floor->render(shadowShader);
			ceiling->render(shadowShader);
			backWall->render(shadowShader);
		}

		// draw portal projection
		glUseProgram(sceneShader);
		setUniformMat4(sceneShader, PROJECTION, portal0cam.getProjection());
		setUniformMat4(sceneShader, VIEW, portal0cam.getView());
		setUniformVec3(sceneShader, VIEW_POSITION, portal0->getPosition());
		glBindFramebuffer(GL_FRAMEBUFFER, portal0fbo);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		track->render(sceneShader);
		train->render(sceneShader);
		portal0->render(sceneShader);
		portal0->setTexture(portal1tex);
		floor->render(sceneShader);
		ceiling->render(sceneShader);
		backWall->render(sceneShader);

		setUniformMat4(sceneShader, PROJECTION, portal1cam.getProjection());
		setUniformMat4(sceneShader, VIEW, portal1cam.getView());
		setUniformVec3(sceneShader, VIEW_POSITION, portal1->getPosition());
		glBindFramebuffer(GL_FRAMEBUFFER, portal1fbo);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		track->render(sceneShader);
		train->render(sceneShader);
		portal1->render(sceneShader);
		portal1->setTexture(portal0tex);
		floor->render(sceneShader);
		ceiling->render(sceneShader);
		backWall->render(sceneShader);

		// draw scene
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, scrWidth, scrHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		setUniformInt(sceneShader, SHADOW_MAP, 0);
		setUniformMat4(sceneShader, VIEW, currentCamera->getView());
		setUniformMat4(sceneShader, PROJECTION, currentCamera->getProjection());
		setUniformMat4(sceneShader, LIGHT, glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, lightNear, lightFar)
		                                   * glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f)));
		setUniformVec3(sceneShader, LIGHT_COLOR, glm::vec3(1.0f));
		setUniformVec3(sceneShader, LIGHT_POSITION, lightPosition);
		setUniformVec3(sceneShader, LIGHT_DIRECTION, lightDirection);
		setUniformVec3(sceneShader, VIEW_POSITION, currentCamera->getPosition());
		setUniformInt(sceneShader, DO_LIGHTING, doLights);
		setUniformFloat(sceneShader, LIGHT_INNER, glm::cos(lightInner));
		setUniformFloat(sceneShader, LIGHT_OUTER, glm::cos(lightOuter));
		// TODO: Change this back to a reasonable value when testing complete
		setUniformFloat(sceneShader, AMBIENT_STRENGTH, 1.0f);
		setUniformFloat(sceneShader, DIFFUSE_STRENGTH, 0.6f);
		setUniformFloat(sceneShader, SPECULAR_STRENGTH, 0.3f);


		track->render(sceneShader);
		train->render(sceneShader);
		portal0->render(sceneShader);
		portal1->render(sceneShader);
		trainCamMarker->render(sceneShader);
		floor->render(sceneShader);
		ceiling->render(sceneShader);
		backWall->render(sceneShader);

		// push pixels
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete train;
	delete track;
	delete portal0;
	delete portal1;
	glfwTerminate();
	return EXIT_SUCCESS;
}
