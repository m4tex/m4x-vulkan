/** @file */

//
// Created by m4tex on 6/8/24.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

/**
 * Validation layers used in debug builds
 */
const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

// Enable validation layers for debug builds

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace m4x {

    /**
     * Vulkan utilities in form of static functions.
     * It will use the engine's default as the input parameters.
     */
    class VkUtils {
    public:

        /**
         * Creates a Vulkan instance with engine defaults
         * @param instance [out] Where to store the creates instance
         */
        static void CreateVkInstance(VkInstance* instance);

        /**
         * Checks if the defined validation layers are supported
         * @return If all requested layers are supported
         */
        static bool ValidationLayerSupport();

        /**
         * Picks a suitable GPU
         */
         static void PickPhysicalDevice(VkInstance& instance, VkPhysicalDevice* physicalDevice);

    private:

        /**
         * Checks if the device supports all the required operations
         * @param device Device to check
         * @return If device is suitable for engine operations
         */
        static bool isDeviceSuitable(VkPhysicalDevice device);
    };
} // m4x
