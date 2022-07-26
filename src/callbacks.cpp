#include <iostream>
#include <defaults.hpp>
#include <callbacks.hpp>

std::list<GLFWkeyfun> key_handlers;
std::list<GLFWcursorposfun> cursor_pos_handlers;
std::list<GLFWcursorenterfun> cursor_enter_handlers;
std::list<GLFWmousebuttonfun> mouse_button_handlers;
std::list<GLFWscrollfun> scroll_handlers;
std::list<GLFWframebuffersizefun> fb_size_handlers;

void handleGlfwError(int code, const char* msg)
{
	std::cerr << "GLFW error " << code << ": " << msg << std::endl;
}

void handleGlfwKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	for (auto h : key_handlers)
		h(window, key, scancode, action, mods);
}

void handleGlfwCursorPos(GLFWwindow *window, double x, double y)
{
	for (auto h : cursor_pos_handlers)
		h(window, x, y);
}

void handleGlfwCursorEnter(GLFWwindow *window, int entered)
{
	for (auto h : cursor_enter_handlers)
		h(window, entered);
}

void handleGlfwMouseButton(GLFWwindow *window, int button, int action, int mods)
{
	for (auto h : mouse_button_handlers)
		h(window, button, action, mods);
}

void handleGlfwScroll(GLFWwindow *window, double dx, double dy)
{
	for (auto h : scroll_handlers)
		h(window, dx, dy);
}

void handleGlfwFramebufferSize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	for (auto h : fb_size_handlers)
		h(window, width, height);
}
