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

        // texture = Engine::Graphics::ImageToGPUTexture(device, "./assets/bg.jpg");
        Engine::Graphics::TextureData textureData = Engine::Graphics::ImageToGPUTexture(device, "./assets/bg.jpg");
        texture = textureData.texture;
        SDL_Surface *rgbaSurface = textureData.surface;

        // create transfer buffers to upload to GPU buffers
        vertexBuffer.TransferToGPUBuffer();
        indexBuffer.TransferToGPUBuffer();

        SDL_GPUTransferBufferCreateInfo transferInfo{};
        transferInfo.size = static_cast<Uint32>(rgbaSurface->w * rgbaSurface->h * 4);
        transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        SDL_GPUTransferBuffer *textureTransferBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);


        // upload texture data
        void *mapped = SDL_MapGPUTransferBuffer(device, textureTransferBuffer, false);
        SDL_memcpy(mapped, rgbaSurface->pixels, rgbaSurface->h * rgbaSurface->pitch);
        SDL_UnmapGPUTransferBuffer(device, textureTransferBuffer);

        // start a copy pass
        SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(commandBuffer);

        vertexBuffer.UploadToGPUBuffer(copyPass);
        indexBuffer.UploadToGPUBuffer(copyPass);

        SDL_GPUTextureTransferInfo source{};
        source.transfer_buffer = textureTransferBuffer;
        source.offset = 0;
        source.pixels_per_row = rgbaSurface->w;
        source.rows_per_layer = rgbaSurface->h;

        SDL_GPUTextureRegion destination{};
        destination.texture = texture;
        destination.mip_level = 0;
        destination.layer = 0;

        destination.x = 0;
        destination.y = 0;
        destination.z = 0;

        destination.w = rgbaSurface->w;
        destination.h = rgbaSurface->h;
        destination.d = 1;
        SDL_UploadToGPUTexture(copyPass, &source, &destination, false);

        // end the copy pass
        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(commandBuffer);

              // creating the GPU sampler
        Engine::Graphics::GpuSampler gpuSampler;
        gpuSampler.CreateSampler(device);
        sampler = gpuSampler.GetSampler();

        gpuPipeline = Engine::Graphics::GpuPipeline(window);
        gpuPipeline.CreatePipeline(device);
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
        SDL_GPUTextureSamplerBinding textureBinding{};
        textureBinding.texture = texture;
        textureBinding.sampler = sampler;
        SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);

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
}