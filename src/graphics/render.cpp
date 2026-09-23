#include "graphics/render.hpp"

namespace Engine::Graphics
{
    void Render::Acquire()
    {
        commandBuffer = SDL_AcquireGPUCommandBuffer(device);
        if (commandBuffer == NULL)
        {
            spdlog::error("Failed to acquire GPU command buffer");
        }

        Uint32 width, height;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, &width, &height))
        {
            spdlog::error("Failed to acquire GPU swapchain texture {}", SDL_GetError());
        }
    }

    void Render::StartRender()
    {
        SDL_GPUColorTargetInfo colorTargetInfo{};
        // blue sky color
        colorTargetInfo.clear_color = {0.0f, 0.5f, 1.0f, 1.0f};
        colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
        colorTargetInfo.texture = swapchainTexture;

        // begin a render pass
        renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorTargetInfo, 1, NULL);
    }

    void Render::EndRender()
    {
        // end the render pass
        SDL_EndGPURenderPass(renderPass);

        // submit the command buffer
        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }
} // namespace Engine::Graphics