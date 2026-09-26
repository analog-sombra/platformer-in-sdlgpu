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

    // ------------------Texture------------------

    struct TextureData
    {
        SDL_Surface *surface;
        SDL_GPUTexture *texture;
    };

    TextureData ImageToGPUTexture(SDL_GPUDevice *device, const std::string &imagePath);
}

#endif // __GPU_UTILS_HPP__