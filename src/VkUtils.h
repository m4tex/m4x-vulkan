/** @file */

//
// Created by m4tex on 6/8/24.
//

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <vector>
#include <optional>

/**
 * Validation layers used in debug builds
 */
const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};

/**
 * Extensions used by the engine
 */
const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME
};

// Enable validation layers for debug builds

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace m4x {
    /**
     * Holds indices to required queue families
     */
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    /**
     * For querying swap chain properties
     */
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR        capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   presentModes;
    };

    /**
     * Essentially similar to SwapChainSupportDetails but with single chosen entries
     */
    struct SwapChainConfiguration {
        VkSurfaceFormatKHR  surfaceFormat;
        VkPresentModeKHR    presentMode;
        VkExtent2D          extent;
    };

    /**
     * Vulkan utilities in form of static functions.
     * It will use the engine's defaults as the input parameters.
     */
    class VkUtils {
    public:

        /**
         * Creates a Vulkan instance with engine defaults
         * @param instance [out] Where to store the creates instance
         */
        static void CreateVkInstance(VkInstance* instance);

        /**
         * Picks a suitable GPU
         * @param instance [in] Instance to query
         * @param surface [in] Surface the device has to be compatible with
         * @param physicalDevice [out] Found GPU
         */
        static void PickPhysicalDevice(VkInstance& instance, VkSurfaceKHR& surface, VkPhysicalDevice* physicalDevice);

        /**
         * Creates a logical device to interface with
         * @param physicalDevice [in] The physical device to use
         * @param surface [in] Surface that the device will be working with
         * @param device [out] The created device
         */
        static void CreateLogicalDevice(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface, VkDevice* device);



    private:

        /**
         * Checks if the device supports engine extensions
         * @param device Device we want to check on
         * @return If the device supports the defined extensions
         */
        static bool deviceExtensionSupport(VkPhysicalDevice& device);

        /**
         * Checks if the defined validation layers are supported
         * @return If all requested layers are supported
         */
        static bool validationLayerSupport();

        /**
         * Finds needed queue families
         * @param device [in] Device we want to query
         * @param surface [in] Used for checking presenting compatibility, if nullptr no presentFamily will be looked for
         * @return Queue family indices
         */
         static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device, VkSurfaceKHR& surface);

         static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device, VkSurfaceKHR& surface);

         static SwapChainConfiguration selectSwapChainProperties(SwapChainSupportDetails properties);

        /**
         * Checks if the device supports all the required operations
         * @param device [in] Device to check
         * @param surface [in] Surface to check compatibility with
         * @return If device is suitable for engine operations
         */
        static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR& surface);

    };
} // m4x
