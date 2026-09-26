#ifndef __GPU_UTILS_HPP__
#define __GPU_UTILS_HPP__

#include <SDL3/SDL_gpu.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

namespace Engine::Graphics
{

    struct TextureVertex
    {
        float x, y, z; // vec3 position
        float u, v;    // texture coordinates
    };

    class GpuBufferUtils
    {

    private:
        SDL_GPUDevice *device;
        std::vector<TextureVertex> vertices;
        SDL_GPUBuffer *buffer;
        SDL_GPUTransferBuffer *transferBuffer;

    public:
        GpuBufferUtils();
        GpuBufferUtils(SDL_GPUDevice *device, std::vector<TextureVertex> vertices);
        ~GpuBufferUtils();

        void CreateGPUBuffer();
        void TransferToGPUBuffer();
        void UploadToGPUBuffer(SDL_GPUCopyPass *copyPass);
        void BindGPUBuffer(SDL_GPURenderPass *renderPass);

        SDL_GPUBuffer *GetBuffer() { return buffer; }
    };

    // ------------------Texture------------------

    struct TextureData
    {
        SDL_Surface *surface;
        SDL_GPUTexture *texture;
    };

    TextureData ImageToGPUTexture(SDL_GPUDevice *device, const std::string &imagePath);
}

#endif // __GPU_UTILS_HPP__