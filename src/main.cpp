#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//std
#include <stdexcept>

int main(void) {

	if(!glfwVulkanSupported())
		std::runtime_error("Device doesn't support Vulkan");

	if(!glfwInit())
		std::runtime_error("Failed to initialize glfw.");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(800, 800, "M4X dev build", nullptr, nullptr);
		
	// VkSurfaceKHR surface;
	// VkResult err = glfwCreateWindowSurface()

	while(1);

	glfwDestroyWindow(window);
	
	return 0;
}
