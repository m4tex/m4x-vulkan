/** @file */

//
// Created by m4tex on 6/8/24.
//

// TODO: refactor into OOP

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// std
#include <vector>
#include <optional>
#include <string>

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
        VkSurfaceCapabilitiesKHR capabilities;
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
         * @param instance [out] The created instance
         */
        static void CreateVkInstance(VkInstance* instance);

        /**
         * Picks a suitable GPU
         * @param instance [in] Instance to query
         * @param surface [in] Surface the device has to be compatible with
         * @param physicalDevice [out] The GPU found
         */
        static void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice* physicalDevice);

        /**
         * Creates a logical device to interface with
         * @param physicalDevice [in] The physical device to use
         * @param surface [in] Surface that the device will be working with
         * @param device [out] The created device
         */
        static void CreateLogicalDevice(VkPhysicalDevice physicalDevice, QueueFamilyIndices indices, VkDevice* device);

        /**
         * Finds needed queue families
         * @param device [in] Device we want to query
         * @param surface [in] Used for checking presenting compatibility, if nullptr no presentFamily will be looked for
         * @return Queue family indices
         */
        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

        /**
         * Creates a swapChain config based on the physical device's capabilities
         * @param device [in] Device for which the swapChain will be created
         * @param surface [in] Surface that the swapChain will be connected to
         * @param window [in] Swapchain's window
         * @return Swapchain configuration
         */
        static SwapChainConfiguration RetrieveSwapChainConfig(VkPhysicalDevice device, VkSurfaceKHR surface, GLFWwindow* window);

        /**
         * Creates a swapChain
         * @param device [in] Logical device for the swapChain
         * @param surface [in] Surface that the swapChain will be displaying onto
         * @param indices [in] Queue family indices for the swapChain
         * @param config [in] Configuration to create from
         * @param swapchain
         */
        static void CreateSwapChain(VkDevice device, VkSurfaceKHR surface, QueueFamilyIndices indices,
                                    SwapChainConfiguration config,  VkSwapchainKHR *swapchain);

        static void CreateImageViews(std::vector<VkImage>& swapChainImages, VkDevice device, VkFormat format, std::vector<VkImageView>& views);

        static std::vector<char> ReadShader(const std::string& filename);

        static void
        CreateGraphicsPipeline(VkDevice device, VkExtent2D extent, VkFormat format, VkPipelineLayout *layout,
                               VkRenderPass *renderPass, VkPipeline *graphicsPipeline);

        static VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice device);

        static void
        CreateFramebuffers(VkDevice device, std::vector<VkFramebuffer> &framebuffers, VkExtent2D extent,
                           std::vector<VkImageView> &views, VkRenderPass renderPass);

        static void CreateCommandPool(VkDevice device, uint32_t graphicsQueueFamilyIndex, VkCommandPool *commandPool);
    private:

        /**
         * Checks if the device supports engine extensions
         * @param device [in] Device we want to check on
         * @return If the device supports the defined extensions
         */
        static bool deviceExtensionSupport(VkPhysicalDevice device);

        /**
         * Checks if the defined validation layers are supported
         * @return If all requested layers are supported
         */
        static bool validationLayerSupport();

        /**
         * Retrieves supported features for the swapChain by the device
         * @param device [in] Device to query
         * @param surface [in] Surface to ensure compatibility
         * @return Supported features and other info needed for creation
         */
        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

        /**
         * Selects out of available properties for the creation of a swapChain
         * @param properties [in] The available properties
         * @param window [in] The window the swapChain will be created for
         * @return A swapChain config that could be used for creation
         */
        static SwapChainConfiguration selectSwapChainProperties(const SwapChainSupportDetails& properties, GLFWwindow* window);

        /**
         * Checks if the device supports all the required operations
         * @param device [in] Device to check
         * @param surface [in] Surface to check compatibility with
         * @return If device is suitable for engine operations
         */
        static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

    };
} // m4x
