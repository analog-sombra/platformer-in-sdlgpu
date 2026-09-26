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
        GpuPipeline(SDL_Window *window);
        ~GpuPipeline();

        void CreatePipeline(SDL_GPUDevice *device);
        void BindGpuPipeline(SDL_GPURenderPass *renderPass);
        SDL_GPUGraphicsPipeline *GetPipeline() { return pipeline; }

    private:
        SDL_Window *window;
        SDL_GPUGraphicsPipeline *pipeline;
    };
}
#endif // __GRAPHICS_PIPELINE_HPP__