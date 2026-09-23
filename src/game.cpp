#include "game.hpp"

void SDLCALL MyLogCallback(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    switch (priority)
    {
    case SDL_LOG_PRIORITY_CRITICAL:
    case SDL_LOG_PRIORITY_ERROR:
        spdlog::error("[SDL] {}", message);
        break;
    case SDL_LOG_PRIORITY_WARN:
        spdlog::warn("[SDL] {}", message);
        break;
    case SDL_LOG_PRIORITY_INFO:
        spdlog::info("[SDL] {}", message);
        break;
    default:
        spdlog::debug("[SDL] {}", message);
    }
}

namespace Engine
{
    Game::Game()
    {
        Initialize();
    }

    Game::~Game()
    {
    }

    void Game::Initialize()
    {

        SDL_SetLogPriorities(SDL_LOG_PRIORITY_WARN);
        SDL_SetLogOutputFunction(MyLogCallback, NULL);

        SDL_Init(SDL_INIT_VIDEO);

        auto game_win = SDL_CreateWindow("Hello SDL3", 800, 600, SDL_WINDOW_RESIZABLE);
        window = Engine::Graphics::WindowPtr(game_win);

        if (!window)
        {
            spdlog::error("Failed to create SDL window");
        }

        auto game_device = SDL_CreateGPUDevice(SDL_ShaderCross_GetSPIRVShaderFormats(), false, NULL);
        device = Engine::Graphics::DevicePtr(game_device);

        if (!device)
        {
            spdlog::error("Failed to create SDL GPU device {}", SDL_GetError());
        }

        SDL_ClaimWindowForGPUDevice(device.get(), window.get());

        renderer = Engine::Graphics::Render(window.get(), device.get());
    }

    void Game::run()
    {
        while (isRunning)
        {
            HandleInput();
            Update();
            Render();
        }
    }

    void Game::Update()
    {
        // Update implementation
    }

    void Game::Render()
    {
        renderer.Acquire();
        renderer.StartRender();
        renderer.EndRender();
    }

    void Game::HandleInput()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
        }
    }
}