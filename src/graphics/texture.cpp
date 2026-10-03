#include "graphics/texture.hpp"

namespace Engine::Graphics
{

    TextureGpuBuffer::TextureGpuBuffer(SDL_GPUDevice *device, const std::string &filePath) : device(device), imagePath(filePath)
    {
    }

    TextureGpuBuffer::~TextureGpuBuffer()
    {
        Cleanup();
    }

    void TextureGpuBuffer::CreateGPUBuffer()
    {
        SDL_Surface *imageData = IMG_Load(imagePath.c_str());
        if (imageData == NULL)
        {
            spdlog::error("Could not load image data: {}", SDL_GetError());
        }
        SDL_FlipSurface(imageData, SDL_FLIP_VERTICAL);

        rgbaSurface = SDL_ConvertSurface(imageData, SDL_PIXELFORMAT_RGBA32);

        if (rgbaSurface == NULL)
        {
            spdlog::error("Could not convert image: {}", SDL_GetError());
        }

        SDL_DestroySurface(imageData);

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

        texture = SDL_CreateGPUTexture(device, &textureInfo);
        if (texture == NULL)
        {
            spdlog::error("Could not create GPU texture: {}", SDL_GetError());
        }
    }
    void TextureGpuBuffer::TransferToGPUBuffer()
    {
        SDL_GPUTransferBufferCreateInfo transferInfo{};
        transferInfo.size = static_cast<Uint32>(rgbaSurface->w * rgbaSurface->h * 4);
        transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

        // upload texture data
        void *mapped = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
        SDL_memcpy(mapped, rgbaSurface->pixels, rgbaSurface->h * rgbaSurface->pitch);
        SDL_UnmapGPUTransferBuffer(device, transferBuffer);
    }
    void TextureGpuBuffer::UploadToGPUBuffer(SDL_GPUCopyPass *copyPass)
    {

        SDL_GPUTextureTransferInfo source{};
        source.transfer_buffer = transferBuffer;
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
    }
    void TextureGpuBuffer::BindGPUBuffer(SDL_GPURenderPass *renderPass)
    {
        SDL_GPUTextureSamplerBinding textureBinding{};
        textureBinding.texture = texture;
        textureBinding.sampler = sampler.GetSampler();
        SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);
    }

    void TextureGpuBuffer::CreateSampler()
    {
        sampler.CreateSampler(device);
    }

    void TextureGpuBuffer::Cleanup()
    {
        if (device && texture)
        {
            SDL_ReleaseGPUTexture(device, texture);
        }
        if (device && transferBuffer)
        {
            SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        }
        if (rgbaSurface)
        {
            SDL_DestroySurface(rgbaSurface);
        }
        sampler.Cleanup();
    }

}