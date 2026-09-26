#ifndef __GRAPHICS_SAMPLER_HPP__
#define __GRAPHICS_SAMPLER_HPP__

#include <SDL3/SDL_gpu.h>

namespace Engine::Graphics
{

    class GpuSampler
    {
    private:
        SDL_GPUSampler *sampler;

    public:
        GpuSampler();
        ~GpuSampler();
        void CreateSampler(SDL_GPUDevice *device);
        SDL_GPUSampler *GetSampler() { return sampler; }
    };

}
#endif // __GRAPHICS_SAMPLER_HPP__