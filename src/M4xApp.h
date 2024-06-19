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

        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;
        VkPipeline graphicsPipeline;

        std::vector<VkFramebuffer> swapChainFramebuffers;

        VkCommandPool commandPool;
        VkCommandBuffer commandBuffer;

        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;

        void createWindow();
        void initVulkan();
        void createSurface();

        /**
         * Retrieves needed device queues and saves them in the class
         */
        void getDeviceQueues();

        /**
         * Creates a swap chain, also creates an image view for each image in the swap chain
         */
        void createSwapChain();

        /**
         * Creates a pipeline and populates the class with its layout and render pass
         */
        void createPipeline();

        void createFramebuffers();

        void createCommandPool();

        void createCommandBuffer();

        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        void drawFrame();

        void createSyncObjects();

        /**
         * Main loop of the app
         */
        void mainLoop();

        /**
         * Deallocates all the app resources in order
         */
        void cleanup();
    };
} // m4x
