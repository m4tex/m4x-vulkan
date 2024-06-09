/** @file */

//
// Created by m4tex on 4/15/24.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace m4x {
    /**
     * A class holding all the application's logic.
     * @fn run Runs all the separate functions in order
     * @fn createWindow Creates a GLFW window and sets it to _window
     * @fn initVulkan initializes a vulkan instance and window surface
     * @fn mainLoop
     */
    class M4xApp {
    public:
        void run();
    private:
        GLFWwindow* _window;
        VkInstance _instance;
        VkPhysicalDevice physicalDevice;

        void createWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();
    };
} // m4x
