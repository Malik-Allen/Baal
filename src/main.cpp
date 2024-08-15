#include "Baal.h"
#include "../src/core/vulkan/tests/TestRenderer.h"
#include "../src/utility/DebugLog.h"
#include <GLFW/glfw3.h>

using namespace Baal;

int main()
{
	DEBUG_INIT();

	GLFWwindow* window;
	
	glfwInit();
	
	glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
	
	window = glfwCreateWindow( 1280, 720, "Vulkan", nullptr, nullptr );

	VK::TestRenderer renderer;

	renderer.Startup("Test Flight", window);

	int i = 0;
	bool bIsRunning = true;
	while(bIsRunning)
	{
		glfwPollEvents();

		if (i == 500) 
		{
			renderer.DestroyTarget();
			i = 0;
		}

		renderer.Render();

		if(glfwWindowShouldClose(window) == true || glfwGetKey(window, static_cast<int>(GLFW_KEY_ESCAPE)) == GLFW_PRESS)
		{
			bIsRunning = false;
		}
		++i;
	}
	
	renderer.Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}