#ifndef __GRAPHICS_SHADER_HPP__
#define __GRAPHICS_SHADER_HPP__

#include <glm/glm.hpp>
#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>
#include <string>
#include <spdlog/spdlog.h>
#include <fmt/core.h>

namespace Engine::Graphics
{

    enum ShaderType
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
    };

    class Shader
    {
    public:
        Shader();
        ~Shader();

        SDL_GPUShader *CreateShader(SDL_GPUDevice *device, ShaderType type, const std::string &source);
    };

}

#endif // __GRAPHICS_SHADER_HPP__