#ifndef __RENDER_HPP__
#define __RENDER_HPP__

#include <SDL3/SDL_gpu.h>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

namespace Engine::Graphics
{

    class Render
    {
    private:
        SDL_Window *window;
        SDL_GPUDevice *device;

        SDL_GPUCommandBuffer *commandBuffer;
        SDL_GPUTexture *swapchainTexture;
        SDL_GPURenderPass *renderPass;

    public:
        Render() {};
        Render(SDL_Window *window, SDL_GPUDevice *device) : window(window), device(device) {};
        ~Render() = default;

        void Acquire();
        void StartRender();
        void EndRender();
        
        SDL_GPURenderPass *GetRenderPass() { return renderPass; }
        SDL_GPUCommandBuffer *GetCommandBuffer() { return commandBuffer; }
    };
} // namespace Engine::Graphics

#endif // __RENDER_HPP__