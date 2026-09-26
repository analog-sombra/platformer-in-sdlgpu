#include "graphics/gpu_utils.hpp"

namespace Engine::Graphics
{

       TextureData ImageToGPUTexture(SDL_GPUDevice *device, const std::string &imagePath)
    {
        SDL_Surface *imageData = IMG_Load(imagePath.c_str());
        if (imageData == NULL)
        {
            spdlog::error("Could not load image data: {}", SDL_GetError());
        }
        SDL_FlipSurface(imageData, SDL_FLIP_VERTICAL);

        SDL_Surface *rgbaSurface =
            SDL_ConvertSurface(imageData, SDL_PIXELFORMAT_RGBA32);

        if (rgbaSurface == NULL)
        {
            spdlog::error("Could not convert image: {}", SDL_GetError());
        }

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

        // // return SDL_CreateGPUTexture(device, &textureInfo);
        // TextureData textureData{};
        // textureData.surface = rgbaSurface;
        // textureData.texture = SDL_CreateGPUTexture(device, &textureInfo);
        // return textureData;

        return TextureData{rgbaSurface, SDL_CreateGPUTexture(device, &textureInfo)};
    }

}