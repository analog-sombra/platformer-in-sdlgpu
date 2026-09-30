#ifndef __GRAPHICS_TEXTURE_HPP__
#define __GRAPHICS_TEXTURE_HPP__

#include <SDL3/SDL_gpu.h>
#include <string>
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include <SDL3_image/SDL_image.h>
#include "graphics/sampler.hpp"

namespace Engine::Graphics
{

    class TextureGpuBuffer
    {

    private:
        SDL_GPUDevice *device;
        SDL_GPUTexture *texture;
        std::string imagePath;
        SDL_Surface *rgbaSurface;
        SDL_GPUTransferBuffer *transferBuffer;
        Engine::Graphics::GpuSampler sampler;

    public:
        TextureGpuBuffer();
        TextureGpuBuffer(SDL_GPUDevice *device, const std::string &filePath);
        ~TextureGpuBuffer();

        void CreateGPUBuffer();
        void TransferToGPUBuffer();
        void UploadToGPUBuffer(SDL_GPUCopyPass *copyPass);
        void BindGPUBuffer(SDL_GPURenderPass *renderPass);

        void CreateSampler();

        SDL_GPUTexture *GetTexture() { return texture; }

        void Cleanup();
    };

}

#endif // __GRAPHICS_TEXTURE_HPP__