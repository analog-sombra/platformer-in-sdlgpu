#include "graphics/gpu_utils.hpp"

namespace Engine::Graphics
{

    GpuBufferUtils::GpuBufferUtils()
    {
    }

    GpuBufferUtils::GpuBufferUtils(SDL_GPUDevice *device, std::vector<TextureVertex> vertices)
        : device(device), vertices(vertices)
    {
    }

    GpuBufferUtils::~GpuBufferUtils()
    {
        // SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        // SDL_ReleaseGPUBuffer(device, buffer);
    }

    void GpuBufferUtils::CreateGPUBuffer()
    {
        SDL_GPUBufferCreateInfo bufferInfo{};
        bufferInfo.size = vertices.size() * sizeof(TextureVertex);
        bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        buffer = SDL_CreateGPUBuffer(device, &bufferInfo);
    }

    void GpuBufferUtils::TransferToGPUBuffer()
    {
        SDL_GPUTransferBufferCreateInfo vertexTransferInfo{};
        vertexTransferInfo.size = vertices.size() * sizeof(TextureVertex);
        vertexTransferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transferBuffer = SDL_CreateGPUTransferBuffer(device, &vertexTransferInfo);

        TextureVertex *vertexData = (TextureVertex *)SDL_MapGPUTransferBuffer(device, transferBuffer, false);
        SDL_memcpy(vertexData, vertices.data(), vertices.size() * sizeof(TextureVertex));

        // release the mapped transfer buffer before unmapping
        SDL_UnmapGPUTransferBuffer(device, transferBuffer);
    }

    void GpuBufferUtils::UploadToGPUBuffer(SDL_GPUCopyPass *copyPass)
    {
        // upload vertex buffer
        SDL_GPUTransferBufferLocation vertexLocation{};
        vertexLocation.transfer_buffer = transferBuffer;
        vertexLocation.offset = 0;

        SDL_GPUBufferRegion vertexRegion{};
        vertexRegion.buffer = buffer;
        vertexRegion.size = vertices.size() * sizeof(Engine::Graphics::TextureVertex);
        vertexRegion.offset = 0;

        SDL_UploadToGPUBuffer(copyPass, &vertexLocation, &vertexRegion, false);
    }

    void GpuBufferUtils::BindGPUBuffer(SDL_GPURenderPass *renderPass)
    {
        SDL_GPUBufferBinding bufferBindings[1];
        bufferBindings[0].buffer = buffer; // index 0 is slot 0 in this example
        bufferBindings[0].offset = 0;      // start from the first byte

        SDL_BindGPUVertexBuffers(renderPass, 0, bufferBindings, 1); // bind one buffer starting from slot 0
    }

    TextureData ImageToGPUTexture(SDL_GPUDevice *device, const std::string &imagePath)
    {
        SDL_Surface *imageData = IMG_Load(imagePath.c_str());
        if (imageData == NULL)
        {
            spdlog::error("Could not load image data: {}", SDL_GetError());
        }
        SDL_FlipSurface(imageData, SDL_FLIP_VERTICAL);

        SDL_Surface *rgbaSurface =
            SDL_ConvertSurface(imageData, SDL_PIXELFORMAT_RGBA32);

        if (rgbaSurface == NULL)
        {
            spdlog::error("Could not convert image: {}", SDL_GetError());
        }

        // create the GPU texture from the image data
        SDL_GPUTextureCreateInfo textureInfo{};

        textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
        textureInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;

        textureInfo.width = rgbaSurface->w;
        textureInfo.height = rgbaSurface->h;

        textureInfo.layer_count_or_depth = 1;
        textureInfo.num_levels = 1;
        textureInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;

        // // return SDL_CreateGPUTexture(device, &textureInfo);
        // TextureData textureData{};
        // textureData.surface = rgbaSurface;
        // textureData.texture = SDL_CreateGPUTexture(device, &textureInfo);
        // return textureData;

        return TextureData{rgbaSurface, SDL_CreateGPUTexture(device, &textureInfo)};
    }

}