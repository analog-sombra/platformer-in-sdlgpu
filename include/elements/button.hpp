#ifndef __ELEMENTN_BUTTON_HPP__
#define __ELEMENTN_BUTTON_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL3/SDL_gpu.h>

#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include <SDL3_image/SDL_image.h>
#include "graphics/shader.hpp"
#include "graphics/sampler.hpp"
#include "graphics/gpu_utils.hpp"
#include <vector>

namespace Engine::Elements
{

    class Button
    {

    private:
        SDL_Window *window;
        SDL_GPUDevice *device;

        SDL_GPUBuffer *vertexBuffer;
        SDL_GPUBuffer *indexBuffer;
        SDL_GPUGraphicsPipeline *graphicsPipeline;
        SDL_GPUSampler *sampler;
        SDL_GPUTexture *texture;

        glm::mat4 model;

    public:
        Button();
        Button(SDL_Window *window, SDL_GPUDevice *device);
        ~Button();

        void render(SDL_GPURenderPass *renderPass, SDL_GPUCommandBuffer *commandBuffer);
        void update();
        void handleEvent();
    };
}
#endif // __ELEMENTN_BUTTON_HPP__