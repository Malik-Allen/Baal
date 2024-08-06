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
	
	window = glfwCreateWindow( 800, 600, "Vulkan", nullptr, nullptr );

	VK::TestRenderer renderer("Test Flight", window);

	renderer.Startup();

	for (int i = 0; i < 1000; ++i)
	{
		glfwPollEvents();
		renderer.Render();

		if (i % 60 == 0)
		{
			DEBUG_LOG(LOG::INFO, "Hello FPS");
		}

		if (i == 500) 
		{
			renderer.DestroyTarget();
		}
	}
	
	renderer.Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}