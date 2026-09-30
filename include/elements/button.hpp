#ifndef __ELEMENTN_BUTTON_HPP__
#define __ELEMENTN_BUTTON_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL3/SDL_gpu.h>

#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include "graphics/sampler.hpp"
#include "graphics/vertex.hpp"
#include "graphics/indices.hpp"
#include "graphics/texture.hpp"
#include "graphics/pipeline.hpp"
#include <vector>

namespace Engine::Elements
{

    class Button
    {

    private:
        SDL_Window *window;
        SDL_GPUDevice *device;

        Engine::Graphics::VertexGpuBuffer vertexBuffer;
        Engine::Graphics::IndicesGpuBuffer indexBuffer;
        Engine::Graphics::TextureGpuBuffer textureBuffer;

        Engine::Graphics::GpuPipeline gpuPipeline;
        Engine::Graphics::GpuSampler sampler;

        glm::mat4 model;

    public:
        Button();
        Button(SDL_Window *window, SDL_GPUDevice *device);
        ~Button();

        void render(SDL_GPURenderPass *renderPass, SDL_GPUCommandBuffer *commandBuffer);
        void update();
        void handleEvent();

        void Cleanup();
    };
}
#endif // __ELEMENTN_BUTTON_HPP__