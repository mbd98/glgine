#pragma once

#include <list>

// List of key press handlers
extern std::list<GLFWkeyfun> key_handlers;
// List of mouse position handlers
extern std::list<GLFWcursorposfun> cursor_pos_handlers;
// List of cursor-enters-window handlers
extern std::list<GLFWcursorenterfun> cursor_enter_handlers;
// List of mouse click handlers
extern std::list<GLFWmousebuttonfun> mouse_button_handlers;
// List of mouse scroll handlers
extern std::list<GLFWscrollfun> scroll_handlers;
// List of framebuffer resize handlers
extern std::list<GLFWframebuffersizefun> fb_size_handlers;

extern void handleGlfwError(int, const char*);
extern void handleGlfwKey(GLFWwindow*, int, int, int, int);
extern void handleGlfwCursorPos(GLFWwindow*, double, double);
extern void handleGlfwCursorEnter(GLFWwindow*, int);
extern void handleGlfwMouseButton(GLFWwindow*, int, int, int);
extern void handleGlfwScroll(GLFWwindow*, double, double);
extern void handleGlfwFramebufferSize(GLFWwindow*, int, int);
