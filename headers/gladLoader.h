#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GladLoader
{
public:
	inline static void LoadGlad();
	
private:
	inline static bool isGladLoaderAvailable();
	

	inline static void terminateWindow(const std::string&& message);
	

	inline static void load();
};