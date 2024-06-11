//
// Created by m4tex on 6/8/24.
//

#include "VkUtils.h"

//std
#include <stdexcept>
#include <cstring>
#include <optional>
#include <set>
#include <iostream>
#include <limits>
#include <algorithm>

namespace m4x {

    bool VkUtils::validationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const auto& layerName : validationLayers ) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers )
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }

            if (!layerFound) return false;
        }

        return true;
    }

    void VkUtils::CreateVkInstance(VkInstance* instance) {
        if (enableValidationLayers && !validationLayerSupport()) {
            throw std::runtime_error("Requested validation layers not available.");
        }

        // Create app info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "M4X Application";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "M4X Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Create instance creation info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        // Get required extensions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        // Create instance
        if(VK_SUCCESS != vkCreateInstance(&createInfo, nullptr, instance))
            throw std::runtime_error("VkUtils: failed to create a Vulkan instance");
    }

    void VkUtils::PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice *physicalDevice) {
        *physicalDevice = VK_NULL_HANDLE;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("No GPUs supporting Vulkan found.");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if(isDeviceSuitable(device, surface)) {
                *physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("No suitable GPU found.");
        }
    }

    // TODO: make device picking score based or make an implementation that lets the user pick
    bool VkUtils::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices = FindQueueFamilies(device, surface);

        bool extensionsSupported = deviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails details = querySwapChainSupport(device, surface);
            swapChainAdequate = !details.formats.empty() && !details.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    QueueFamilyIndices VkUtils::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }

            if(indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;
    }

    void VkUtils::CreateLogicalDevice(VkPhysicalDevice physicalDevice, QueueFamilyIndices indices, VkDevice* device) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        // In case the queue families overlap, we remove the duplicate indices
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        for (const auto& queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // All set to VK_FALSE by default
        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (VK_SUCCESS != vkCreateDevice(physicalDevice, &createInfo, nullptr, device)) {
            throw std::runtime_error("Failed to create a logical device.");
        }
    }

    bool VkUtils::deviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensionProperties(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensionProperties.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : extensionProperties) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    SwapChainSupportDetails VkUtils::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    // TODO: look into the HDR extension and immediate mode
    SwapChainConfiguration VkUtils::selectSwapChainProperties(const SwapChainSupportDetails& properties, GLFWwindow* window) {
        SwapChainConfiguration config{};

        std::optional<VkSurfaceFormatKHR> format;

        for (const auto& surfaceFormat : properties.formats) {
            if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                format = surfaceFormat;
            }
        }

        if (!format.has_value()) format = properties.formats[0];

        config.surfaceFormat = format.value();
        config.presentMode = VK_PRESENT_MODE_FIFO_KHR;

        if (properties.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            config.extent = properties.capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D extent = {static_cast<uint32_t>(width),
                                 static_cast<uint32_t>(height)};

            extent.width = std::clamp(extent.width, properties.capabilities.minImageExtent.width,
                                      properties.capabilities.maxImageExtent.width);

            extent.height = std::clamp(extent.height, properties.capabilities.minImageExtent.height,
                                       properties.capabilities.maxImageExtent.height);
        }

        return config;
    }

    void VkUtils::CreateSwapChain(VkPhysicalDevice device, VkSurfaceKHR surface, GLFWwindow* window, VkSwapchainKHR *swapchain) {
        selectSwapChainProperties(querySwapChainSupport(device, surface), window);
    }

} // m4x