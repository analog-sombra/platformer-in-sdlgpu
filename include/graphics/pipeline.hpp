#ifndef __GRAPHICS_PIPELINE_HPP__
#define __GRAPHICS_PIPELINE_HPP__

#include <SDL3/SDL_gpu.h>
#include "graphics/shader.hpp"
#include "graphics/vertex.hpp"

namespace Engine::Graphics
{
    class GpuPipeline
    {
    public:
        GpuPipeline() {};
        GpuPipeline(SDL_Window *window, SDL_GPUDevice *device);
        ~GpuPipeline();

        void CreatePipeline();
        void BindGpuPipeline(SDL_GPURenderPass *renderPass);
        SDL_GPUGraphicsPipeline *GetPipeline() { return pipeline; }

        void Cleanup();

    private:
        SDL_Window *window;
        SDL_GPUDevice *device;
        SDL_GPUGraphicsPipeline *pipeline;
    };
}
#endif // __GRAPHICS_PIPELINE_HPP__