#include "graphics/gpu_utils.hpp"

namespace Engine::Graphics
{

    SDL_GPUBuffer *CreateGPUBuffer(SDL_GPUDevice *device, std::vector<TextureVertex> vertices)
    {
        SDL_GPUBufferCreateInfo bufferInfo{};
        bufferInfo.size = vertices.size() * sizeof(TextureVertex);
        bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        return SDL_CreateGPUBuffer(device, &bufferInfo);
    }
}