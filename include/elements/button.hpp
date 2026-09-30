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
#include <memory>

namespace Engine::Elements
{

    struct TransformUniform
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };
    class Button
    {

    private:
        SDL_Window *window;
        SDL_GPUDevice *device;

        std::unique_ptr<Engine::Graphics::VertexGpuBuffer> vertexBuffer;
        std::unique_ptr<Engine::Graphics::IndicesGpuBuffer> indexBuffer;
        std::unique_ptr<Engine::Graphics::TextureGpuBuffer> textureBuffer;
        std::unique_ptr<Engine::Graphics::GpuPipeline> gpuPipeline;

        // Transform components
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 rotation = glm::vec3(0.0f); // Euler angles in radians
        glm::vec3 scale = glm::vec3(1.0f);
        
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

    public:
        // Button();
        Button(SDL_Window *window, SDL_GPUDevice *device);
        ~Button();

        void render(SDL_GPURenderPass *renderPass, SDL_GPUCommandBuffer *commandBuffer);
        void update();
        void handleEvent();

        // void Cleanup();
    };
}
#endif // __ELEMENTN_BUTTON_HPP__