#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GladLoader
{
public:
	static void TryLoadGlad();
	
private:
	static bool isGladLoaderAvailable();

	static void terminateWindow(const std::string&& message);
	
	static void load();
};