#ifndef __GRAPHICS_DELETER_HPP__
#define __GRAPHICS_DELETER_HPP__

#include <SDL3/SDL_gpu.h>
#include <memory>

namespace Engine::Graphics
{
    // Custom Deleters for SDL3 GPU Handles
    struct GPUDeviceDeleter
    {
        void operator()(SDL_GPUDevice *device) const
        {
            if (device)
            {
                SDL_WaitForGPUIdle(device);
                SDL_DestroyGPUDevice(device);
            }
        }
    };

    struct WindowDeleter
    {
        void operator()(SDL_Window *window) const
        {
            if (window)
                SDL_DestroyWindow(window);
        }
    };

    // Type Aliases
    using DevicePtr = std::unique_ptr<SDL_GPUDevice, GPUDeviceDeleter>;
    using WindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;
}

#endif