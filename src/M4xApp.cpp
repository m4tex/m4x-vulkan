//
// Created by m4tex on 4/15/24.
//

#include <stdexcept>
#include "M4xApp.h"
#include "VkUtils.h"

namespace m4x {
    void M4xApp::run() {
        createWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    void M4xApp::createWindow() {
        if(GLFW_FALSE == glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        _window = glfwCreateWindow(800, 800, "M4X dev build", nullptr, nullptr);

        if(!_window)
            throw std::runtime_error("Failed to create window.");
    }

    void M4xApp::initVulkan() {
        if(GLFW_FALSE == glfwVulkanSupported()) {
            throw std::runtime_error("Device doesn't support Vulkan.");
        }

        VkUtils::CreateVkInstance(&_instance);
    }

    void M4xApp::mainLoop() {
        while(!glfwWindowShouldClose(_window)) {
            glfwPollEvents();
        }
    }

    void M4xApp::cleanup() {
        vkDestroyInstance(_instance, nullptr);

        glfwDestroyWindow(_window);
        glfwTerminate();
    }

} // m4x