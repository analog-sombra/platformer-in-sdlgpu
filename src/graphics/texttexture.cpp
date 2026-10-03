#include "graphics/texttexture.hpp"

namespace Engine::Graphics
{

    TextTextureGpuBuffer::TextTextureGpuBuffer(SDL_GPUDevice *device, const std::string &text) : device(device), text(text)
    {
    }

    TextTextureGpuBuffer::~TextTextureGpuBuffer()
    {
        Cleanup();
    }

    void TextTextureGpuBuffer::CreateGPUBuffer()
    {

        TTF_Font *font = TTF_OpenFont("assets/fonts/candy.otf", 64);
        if (font == NULL)
        {
            spdlog::error("Could not load font: {}", SDL_GetError());
        }

        SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), 0, SDL_Color{255, 255, 255, 255});
        // SDL_Surface *imageData = IMG_Load(imagePath.c_str());
        if (textSurface == NULL)
        {
            spdlog::error("Could not load text surface: {}", SDL_GetError());
        }
        SDL_FlipSurface(textSurface, SDL_FLIP_VERTICAL);

        rgbaSurface = SDL_ConvertSurface(textSurface, SDL_PIXELFORMAT_RGBA32);

        if (rgbaSurface == NULL)
        {
            spdlog::error("Could not convert text surface: {}", SDL_GetError());
        }

        TTF_CloseFont(font);
        SDL_DestroySurface(textSurface);

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
    void TextTextureGpuBuffer::TransferToGPUBuffer()
    {

        const size_t rowSize =
            static_cast<size_t>(rgbaSurface->w) * 4;

        const size_t bufferSize =
            rowSize * static_cast<size_t>(rgbaSurface->h);

        SDL_GPUTransferBufferCreateInfo transferInfo{};
        transferInfo.size = static_cast<Uint32>(bufferSize);
        transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transferBuffer = SDL_CreateGPUTransferBuffer(device, &transferInfo);

        // upload texture data
        void *mapped = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
        // SDL_memcpy(mapped, rgbaSurface->pixels, rgbaSurface->h * rgbaSurface->pitch);
        auto *dst =
            static_cast<std::uint8_t *>(mapped);

        auto *src =
            static_cast<const std::uint8_t *>(rgbaSurface->pixels);

        for (int y = 0; y < rgbaSurface->h; ++y)
        {
            SDL_memcpy(
                dst + y * rowSize,
                src + y * rgbaSurface->pitch,
                rowSize);
        }
        SDL_UnmapGPUTransferBuffer(device, transferBuffer);
    }
    void TextTextureGpuBuffer::UploadToGPUBuffer(SDL_GPUCopyPass *copyPass)
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
    void TextTextureGpuBuffer::BindGPUBuffer(SDL_GPURenderPass *renderPass)
    {
        SDL_GPUTextureSamplerBinding textureBinding{};
        textureBinding.texture = texture;
        textureBinding.sampler = sampler.GetSampler();
        SDL_BindGPUFragmentSamplers(renderPass, 0, &textureBinding, 1);
    }

    void TextTextureGpuBuffer::CreateSampler()
    {
        sampler.CreateSampler(device);
    }

    void TextTextureGpuBuffer::Cleanup()
    {
        if (device && texture)
        {
            SDL_ReleaseGPUTexture(device, texture);
        }
        if (device && transferBuffer)
        {
            SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        }
        sampler.Cleanup();

        if (rgbaSurface)
        {
            SDL_DestroySurface(rgbaSurface);
        }
    }

}