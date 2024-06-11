//
// Created by m4tex on 4/15/24.
//

#include <stdexcept>
#include "M4xApp.h"


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

        window = glfwCreateWindow(800, 800, "M4X dev build", nullptr, nullptr);

        if(!window)
            throw std::runtime_error("Failed to create window.");
    }

    void M4xApp::initVulkan() {
        if(GLFW_FALSE == glfwVulkanSupported()) {
            throw std::runtime_error("Device doesn't support Vulkan.");
        }

        VkUtils::CreateVkInstance(&instance);
        createSurface();
        VkUtils::PickPhysicalDevice(instance, surface, &physicalDevice);

        queueFamilyIndices = VkUtils::FindQueueFamilies(physicalDevice, surface);
        VkUtils::CreateLogicalDevice(physicalDevice, queueFamilyIndices, &device);

        vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.presentFamily.value(), 0, &presentQueue);

        VkUtils::CreateSwapChain(physicalDevice, surface, window, &swapchain);
    }

    void M4xApp::mainLoop() {
        while(!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void M4xApp::cleanup() {
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void M4xApp::createSurface() {
        if (VK_SUCCESS != glfwCreateWindowSurface(instance, window, nullptr, &surface)) {
            throw std::runtime_error("Failed to create a window surface.");
        }
    }

} // m4x