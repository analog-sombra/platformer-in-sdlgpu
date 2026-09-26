#ifndef __GRAPHICS_INDICES_HPP__
#define __GRAPHICS_INDICES_HPP__

#include <SDL3/SDL_gpu.h>
#include <vector>

namespace Engine::Graphics
{

    class IndicesGpuBuffer
    {

    private:
        SDL_GPUDevice *device;
        std::vector<uint32_t> indices;
        SDL_GPUBuffer *buffer;
        SDL_GPUTransferBuffer *transferBuffer;

    public:
        IndicesGpuBuffer();
        IndicesGpuBuffer(SDL_GPUDevice *device, std::vector<uint32_t> indices);
        ~IndicesGpuBuffer();

        void CreateGPUBuffer();
        void TransferToGPUBuffer();
        void UploadToGPUBuffer(SDL_GPUCopyPass *copyPass);
        void BindGPUBuffer(SDL_GPURenderPass *renderPass);

        SDL_GPUBuffer *GetIndexBuffer() { return buffer; }
    };

}

#endif // __GRAPHICS_VERTEX_HPP__