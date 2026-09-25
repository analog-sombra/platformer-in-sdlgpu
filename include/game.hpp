#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include <SDL3_shadercross/SDL_shadercross.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include "graphics/deleter.hpp"
#include "graphics/render.hpp"
#include "elements/button.hpp"

namespace Engine
{
    class Game
    {
    private:
        bool isRunning = true;

        Engine::Graphics::WindowPtr window = nullptr;
        Engine::Graphics::DevicePtr device = nullptr;

        Engine::Graphics::Render renderer;
        Engine::Elements::Button button;

    public:
        Game();
        ~Game();
        void Initialize();
        void run();
        void Update();
        void Render();
        void HandleInput();

        
    };
}

#endif // __GAME_HPP__