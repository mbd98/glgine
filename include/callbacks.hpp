#pragma once

#include <list>

extern std::list<GLFWkeyfun> key_handlers;
extern std::list<GLFWcursorposfun> cursor_pos_handlers;
extern std::list<GLFWcursorenterfun> cursor_enter_handlers;
extern std::list<GLFWmousebuttonfun> mouse_button_handlers;
extern std::list<GLFWscrollfun> scroll_handlers;
extern std::list<GLFWframebuffersizefun> fb_size_handlers;

extern void handleGlfwError(int, const char*);
extern void handleGlfwKey(GLFWwindow*, int, int, int, int);
extern void handleGlfwCursorPos(GLFWwindow*, double, double);
extern void handleGlfwCursorEnter(GLFWwindow*, int);
extern void handleGlfwMouseButton(GLFWwindow*, int, int, int);
extern void handleGlfwScroll(GLFWwindow*, double, double);
extern void handleGlfwFramebufferSize(GLFWwindow*, int, int);
