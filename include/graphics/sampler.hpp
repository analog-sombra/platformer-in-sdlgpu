#ifndef __GRAPHICS_SAMPLER_HPP__
#define __GRAPHICS_SAMPLER_HPP__

#include <SDL3/SDL_gpu.h>

namespace Engine::Graphics
{
    SDL_GPUSampler *CreateSampler(SDL_GPUDevice *device);
}
#endif // __GRAPHICS_SAMPLER_HPP__