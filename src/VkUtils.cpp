//
// Created by m4tex on 6/8/24.
//

#include "VkUtils.h"

//std
#include <stdexcept>
#include <cstring>

namespace m4x {

    bool VkUtils::ValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (auto layerName : validationLayers ) {
            bool layerFound = false;

            for (auto layerProperties : availableLayers )
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }

            if (!layerFound) return false;
        }

        return true;
    }

    void VkUtils::CreateVkInstance(VkInstance* instance) {
        if (enableValidationLayers && !ValidationLayerSupport()) {
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

    void VkUtils::PickPhysicalDevice(VkInstance& instance, VkPhysicalDevice *physicalDevice) {
        *physicalDevice = nullptr;
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("No GPUs supporting Vulkan found.");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if(isDeviceSuitable(device)) {
                *physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("No suitable GPU found.");
        }
    }

    // TODO: make device picking score based or make an implementation that lets the user pick
    bool VkUtils::isDeviceSuitable(VkPhysicalDevice device) {
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        // Currently no criteria

        return true;
    }

} // m4x