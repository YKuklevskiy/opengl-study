#include <gladLoader.h>

void GladLoader::TryLoadGlad()
{
	if (isGladLoaderAvailable)
		load();
}

bool GladLoader::isGladLoaderAvailable()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		terminateWindow("Failed to initialize GLAD.");
		return false;
	}
	return true;
}

void GladLoader::terminateWindow(const std::string&& message)
{
	std::cout << message + " Terminating...\n";
	glfwTerminate();
}

void GladLoader::load()
{
	gladLoadGL();
}
