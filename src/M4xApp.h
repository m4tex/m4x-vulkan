/** @file */

//
// Created by m4tex on 15/4/24.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VkUtils.h"

namespace m4x {
    /**
     * A class holding all the application's logic.
     * @fn run Runs all the separate functions in order
     * @fn createWindow Creates a GLFW window and sets it to window
     * @fn initVulkan Initializes vulkan and creates objects before drawing
     * @fn mainLoop Main program loop
     * @fn cleanup Deallocates all vulkan objects and terminates all processes
     */
    class M4xApp {
    public:
        void run();
    private:
        GLFWwindow* window;

        VkInstance instance;
        VkSurfaceKHR surface;
        VkPhysicalDevice physicalDevice;
        VkDevice device;

        QueueFamilyIndices queueFamilyIndices;
        VkQueue graphicsQueue;
        VkQueue presentQueue;

        SwapChainConfiguration swapChainConfiguration;
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        void createWindow();
        void initVulkan();
        void createSurface();
        void mainLoop();
        void cleanup();
    };
} // m4x
