#include "graphics/vertex.hpp"

namespace Engine::Graphics
{
    VertexGpuBuffer::VertexGpuBuffer()
    {
    }

    VertexGpuBuffer::VertexGpuBuffer(SDL_GPUDevice *device, std::vector<TextureVertex> vertices)
        : device(device), vertices(vertices)
    {
    }

    VertexGpuBuffer::~VertexGpuBuffer()
    {
        // SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        // SDL_ReleaseGPUBuffer(device, buffer);
    }

    void VertexGpuBuffer::CreateGPUBuffer()
    {
        SDL_GPUBufferCreateInfo bufferInfo{};
        bufferInfo.size = vertices.size() * sizeof(TextureVertex);
        bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        buffer = SDL_CreateGPUBuffer(device, &bufferInfo);
    }

    void VertexGpuBuffer::TransferToGPUBuffer()
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

    void VertexGpuBuffer::UploadToGPUBuffer(SDL_GPUCopyPass *copyPass)
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

    void VertexGpuBuffer::BindGPUBuffer(SDL_GPURenderPass *renderPass)
    {
        SDL_GPUBufferBinding bufferBindings[1];
        bufferBindings[0].buffer = buffer; // index 0 is slot 0 in this example
        bufferBindings[0].offset = 0;      // start from the first byte

        SDL_BindGPUVertexBuffers(renderPass, 0, bufferBindings, 1); // bind one buffer starting from slot 0
    }

}