#include "graphics/indices.hpp"

namespace Engine::Graphics
{
    IndicesGpuBuffer::IndicesGpuBuffer()
    {
    }

    IndicesGpuBuffer::IndicesGpuBuffer(SDL_GPUDevice *device, std::vector<uint32_t> indices)
        : device(device), indices(indices)
    {
    }

    IndicesGpuBuffer::~IndicesGpuBuffer()
    {
    }

    void IndicesGpuBuffer::CreateGPUBuffer()
    {
        SDL_GPUBufferCreateInfo indexBufferInfo{};
        indexBufferInfo.size = sizeof(indices);
        indexBufferInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        buffer = SDL_CreateGPUBuffer(device, &indexBufferInfo);
    }

    void IndicesGpuBuffer::TransferToGPUBuffer()
    {
        SDL_GPUTransferBufferCreateInfo indexTransferInfo{};
        indexTransferInfo.size = sizeof(indices);
        indexTransferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        transferBuffer = SDL_CreateGPUTransferBuffer(device, &indexTransferInfo);

        uint32_t *indexData = (uint32_t *)SDL_MapGPUTransferBuffer(device, transferBuffer, false);
        SDL_memcpy(indexData, indices.data(), sizeof(indices));
        SDL_UnmapGPUTransferBuffer(device, transferBuffer);
    }

    void IndicesGpuBuffer::UploadToGPUBuffer(SDL_GPUCopyPass *copyPass)
    {
        // upload index buffer
        SDL_GPUTransferBufferLocation indexLocation{};
        indexLocation.transfer_buffer = transferBuffer;
        indexLocation.offset = 0;

        SDL_GPUBufferRegion indexRegion{};
        indexRegion.buffer = buffer;
        indexRegion.size = sizeof(indices);
        indexRegion.offset = 0;

        SDL_UploadToGPUBuffer(copyPass, &indexLocation, &indexRegion, true);
    }

    void IndicesGpuBuffer::BindGPUBuffer(SDL_GPURenderPass *renderPass)
    {
        SDL_GPUBufferBinding indexBufferBinding{};
        indexBufferBinding.buffer = buffer;
        indexBufferBinding.offset = 0;

        SDL_BindGPUIndexBuffer(renderPass, &indexBufferBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
    }

}