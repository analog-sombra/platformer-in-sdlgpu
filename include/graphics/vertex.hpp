#ifndef __GRAPHICS_VERTEX_HPP__
#define __GRAPHICS_VERTEX_HPP__

#include <SDL3/SDL_gpu.h>
#include <vector>

namespace Engine::Graphics
{

    struct TextureVertex
    {
        float x, y, z; // vec3 position
        float u, v;    // texture coordinates
    };

    class VertexGpuBuffer
    {

    private:
        SDL_GPUDevice *device;
        std::vector<TextureVertex> vertices;
        SDL_GPUBuffer *buffer;
        SDL_GPUTransferBuffer *transferBuffer;

    public:
        VertexGpuBuffer();
        VertexGpuBuffer(SDL_GPUDevice *device, std::vector<TextureVertex> vertices);
        ~VertexGpuBuffer();

        void CreateGPUBuffer();
        void TransferToGPUBuffer();
        void UploadToGPUBuffer(SDL_GPUCopyPass *copyPass);
        void BindGPUBuffer(SDL_GPURenderPass *renderPass);

        SDL_GPUBuffer *GetVertexBuffer() { return buffer; }
    };

}

#endif // __GRAPHICS_VERTEX_HPP__