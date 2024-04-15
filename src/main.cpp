#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//std
#include <stdexcept>

int main() {
	if(!glfwVulkanSupported()) {
		throw std::runtime_error("Device doesn't support Vulkan");
    }

	if(GLFW_FALSE == glfwInit())
        throw std::runtime_error("Failed to initialize GLFW.");

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "M4X dev build", nullptr, nullptr);

    if(!window)
        throw std::runtime_error("Failed to create window.");


	// VkSurfaceKHR surface;
	// VkResult err = glfwCreateWindowSurface()

	while(!glfwWindowShouldClose(window));

	glfwDestroyWindow(window);
	
	return EXIT_SUCCESS;
}
