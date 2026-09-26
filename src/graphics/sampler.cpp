#include "graphics/sampler.hpp"

namespace Engine::Graphics
{
    SDL_GPUSampler *CreateSampler(SDL_GPUDevice *device)
    {
        SDL_GPUSamplerCreateInfo samplerInfo{};
        samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
        samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;

        samplerInfo.mipmap_mode =
            SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;

        samplerInfo.address_mode_u =
            SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        samplerInfo.address_mode_v =
            SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        samplerInfo.address_mode_w =
            SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

        return SDL_CreateGPUSampler(device, &samplerInfo);
    }
}