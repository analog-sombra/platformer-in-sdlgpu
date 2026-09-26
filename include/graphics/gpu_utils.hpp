#ifndef __GPU_UTILS_HPP__
#define __GPU_UTILS_HPP__

#include <SDL3/SDL_gpu.h>
#include <vector>
namespace Engine::Graphics
{
    struct TextureVertex
    {
        float x, y, z; // vec3 position
        float u, v;    // texture coordinates
    };

    SDL_GPUBuffer *CreateGPUBuffer(SDL_GPUDevice *device, std::vector<TextureVertex> vertices);
}

#endif // __GPU_UTILS_HPP__