#include "elements/button.hpp"

namespace Engine::Elements
{
    Button::Button() {}
    Button::Button(SDL_Window *window, SDL_GPUDevice *device) : window(window), device(device)
    {

        struct Transform
        {
            glm::vec3 position{0.0f, 0.0f, 0.0f};
            glm::vec3 rotation{0.0f, 0.0f, 0.0f};
            glm::vec3 scale{1.0f, 1.0f, 1.0f};
        };

        std::vector<Engine::Graphics::TextureVertex> vertices = {
            {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f}, // bottom left
            {0.5f, -0.5f, 0.0f, 1.0f, 0.0f},  // bottom right
            {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},   // top right
            {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f}   // top left
        };

        std::vector<uint32_t> indices = {
            0, 1, 2,
            3, 0, 2};

        struct TransformUniform
        {
            glm::mat4 model;
        };

        Transform transform;

        transform.position = {0.0f, 0.0f, 0.0f};
        transform.rotation = {0.0f, 0.0f, 0.0f};
        transform.scale = {0.6f, 1.0f, 1.0f};

        model = glm::mat4(1.0f);

        model = glm::translate(
            model,
            transform.position);

        model = glm::rotate(
            model,
            glm::radians(transform.rotation.z),
            glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::scale(
            model,
            transform.scale);

        TransformUniform transformData;
        transformData.model = model;

        // create the vertex buffer
        vertexBuffer = Engine::Graphics::VertexGpuBuffer(device, vertices);
        vertexBuffer.CreateGPUBuffer();

        // create the index buffer
        indexBuffer = Engine::Graphics::IndicesGpuBuffer(device, indices);
        indexBuffer.CreateGPUBuffer();

        // create the texture buffer
        textureBuffer = Engine::Graphics::TextureGpuBuffer(device, "./assets/bg.jpg");
        textureBuffer.CreateGPUBuffer();

        // create transfer buffers to upload to GPU buffers
        vertexBuffer.TransferToGPUBuffer();
        indexBuffer.TransferToGPUBuffer();
        textureBuffer.TransferToGPUBuffer();

        // start a copy pass
        SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(commandBuffer);

        vertexBuffer.UploadToGPUBuffer(copyPass);
        indexBuffer.UploadToGPUBuffer(copyPass);
        textureBuffer.UploadToGPUBuffer(copyPass);

        // end the copy pass
        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(commandBuffer);

        // creating the GPU sampler
        textureBuffer.CreateSampler();

        gpuPipeline = Engine::Graphics::GpuPipeline(window, device);
        gpuPipeline.CreatePipeline();
    }

    Button::~Button() {}

    void Button::render(SDL_GPURenderPass *renderPass, SDL_GPUCommandBuffer *commandBuffer)
    {
        // bind the graphics pipeline
        gpuPipeline.BindGpuPipeline(renderPass);

        // bind the vertex buffer
        vertexBuffer.BindGPUBuffer(renderPass);

        // bind the index buffer
        indexBuffer.BindGPUBuffer(renderPass);

        // bind the sampler
        textureBuffer.BindGPUBuffer(renderPass);

        SDL_PushGPUVertexUniformData(
            commandBuffer,
            0,
            &model,
            sizeof(model));

        // issue an indexed draw call (6 indices = 2 triangles for a rectangle)
        SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);
    }

    void Button::update() {}

    void Button::handleEvent() {}

    void Button::Cleanup()
    {
        vertexBuffer.Cleanup();
    }
}