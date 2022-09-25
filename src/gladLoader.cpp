#include <gladLoader.h>

inline void GladLoader::LoadGlad()
{
	if (isGladLoaderAvailable)
		load();
}

inline bool GladLoader::isGladLoaderAvailable()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		terminateWindow("Failed to initialize GLAD.");
		return false;
	}
	return true;
}

inline void GladLoader::terminateWindow(const std::string&& message)
{
	std::cout << message + " Terminating...\n";
	glfwTerminate();
}

inline void GladLoader::load()
{
	gladLoadGL();
}
