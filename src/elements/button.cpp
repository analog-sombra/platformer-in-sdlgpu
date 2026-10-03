#include "elements/button.hpp"

namespace Engine::Elements
{
    Button::Button(SDL_Window *window, SDL_GPUDevice *device) : window(window), device(device)
    {

        std::vector<Engine::Graphics::TextureVertex> vertices = {
            {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f}, // bottom left
            {0.5f, -0.5f, 0.0f, 1.0f, 0.0f},  // bottom right
            {0.5f, 0.5f, 0.0f, 1.0f, 1.0f},   // top right
            {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f}   // top left
        };

        std::vector<uint32_t> indices = {
            0, 1, 2,
            3, 0, 2};

        model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);

        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)); // Move quad back

        // create the vertex buffer
        vertexBuffer = std::make_unique<Engine::Graphics::VertexGpuBuffer>(device, vertices);
        vertexBuffer->CreateGPUBuffer();

        // create the index buffer
        indexBuffer = std::make_unique<Engine::Graphics::IndicesGpuBuffer>(device, indices);
        indexBuffer->CreateGPUBuffer();

        // create the texture buffer
        // textureBuffer = std::make_unique<Engine::Graphics::TextureGpuBuffer>(device, "./assets/bg.jpg");
        textureBuffer = std::make_unique<Engine::Graphics::TextTextureGpuBuffer>(device, "Play");
        textureBuffer->CreateGPUBuffer();

        // create transfer buffers to upload to GPU buffers
        vertexBuffer->TransferToGPUBuffer();
        indexBuffer->TransferToGPUBuffer();
        textureBuffer->TransferToGPUBuffer();

        // start a copy pass
        SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(commandBuffer);

        vertexBuffer->UploadToGPUBuffer(copyPass);
        indexBuffer->UploadToGPUBuffer(copyPass);
        textureBuffer->UploadToGPUBuffer(copyPass);

        // end the copy pass
        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(commandBuffer);

        // creating the GPU sampler
        textureBuffer->CreateSampler();

        gpuPipeline = std::make_unique<Engine::Graphics::GpuPipeline>(window, device);
        gpuPipeline->CreatePipeline();
    }

    Button::~Button() {}

    void Button::render(SDL_GPURenderPass *renderPass, SDL_GPUCommandBuffer *commandBuffer)
    {
        // bind the graphics pipeline
        gpuPipeline->BindGpuPipeline(renderPass);

        // bind the vertex buffer
        vertexBuffer->BindGPUBuffer(renderPass);

        // bind the index buffer
        indexBuffer->BindGPUBuffer(renderPass);

        // bind the sampler
        textureBuffer->BindGPUBuffer(renderPass);

        TransformUniform uniformData;
        uniformData.model = model;
        uniformData.projection = projection;
        uniformData.view = view;

        SDL_PushGPUVertexUniformData(
            commandBuffer,
            0,
            &uniformData,
            sizeof(TransformUniform));

        // issue an indexed draw call (6 indices = 2 triangles for a rectangle)
        SDL_DrawGPUIndexedPrimitives(renderPass, 6, 1, 0, 0, 0);
    }

    void Button::update()
    {
        float aspect = 1280.0f / 720.0f; // width / height
        float zoom = 45.0f;
        projection = glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
    }

    void Button::handleEvent() {}
}