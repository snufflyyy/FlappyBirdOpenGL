#pragma once

struct GLFWwindow;

extern int windowWidth;
extern int windowHeight;
extern struct GLFWwindow* window;
extern bool justResized;

void createWindow(int width, int height, const char* title);