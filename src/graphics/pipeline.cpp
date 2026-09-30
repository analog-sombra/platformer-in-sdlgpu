#include "graphics/pipeline.hpp"

namespace Engine::Graphics
{
    GpuPipeline::GpuPipeline(SDL_Window *window, SDL_GPUDevice *device) : window(window), device(device)
    {
    }
    GpuPipeline::~GpuPipeline()
    {
    }
    void GpuPipeline::CreatePipeline()
    {
        Engine::Graphics::Shader shader = Engine::Graphics::Shader();

        SDL_GPUShader *vertexShader = shader.CreateShader(device, Engine::Graphics::VERTEX_SHADER, "assets/shaders/texturevertex.spv");
        SDL_GPUShader *fragmentShader = shader.CreateShader(device, Engine::Graphics::FRAGMENT_SHADER, "assets/shaders/texturefragment.spv");

        SDL_GPUGraphicsPipelineCreateInfo pipelineInfo{};

        // bind shaders
        pipelineInfo.vertex_shader = vertexShader;
        pipelineInfo.fragment_shader = fragmentShader;

        // draw triangles
        pipelineInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;

        // describe the vertex buffers
        SDL_GPUVertexBufferDescription vertexBufferDesctiptions[1];
        vertexBufferDesctiptions[0].slot = 0;
        vertexBufferDesctiptions[0].input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
        vertexBufferDesctiptions[0].instance_step_rate = 0;
        vertexBufferDesctiptions[0].pitch = sizeof(Engine::Graphics::TextureVertex);

        pipelineInfo.vertex_input_state.num_vertex_buffers = 1;
        pipelineInfo.vertex_input_state.vertex_buffer_descriptions = vertexBufferDesctiptions;

        // describe the vertex attribute
        SDL_GPUVertexAttribute vertexAttributes[2];

        // a_position
        vertexAttributes[0].buffer_slot = 0;                             // fetch data from the buffer at slot 0
        vertexAttributes[0].location = 0;                                // layout (location = 0) in shader
        vertexAttributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3; // vec3
        vertexAttributes[0].offset = 0;                                  // start from the first byte from current buffer position

        // a_color
        vertexAttributes[1].buffer_slot = 0;                             // use buffer at slot 0
        vertexAttributes[1].location = 1;                                // layout (location = 1) in shader
        vertexAttributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2; // vec2
        vertexAttributes[1].offset = sizeof(float) * 3;                  // 4th float from current buffer position

        pipelineInfo.vertex_input_state.num_vertex_attributes = 2;
        pipelineInfo.vertex_input_state.vertex_attributes = vertexAttributes;

        // describe the color target
        SDL_GPUColorTargetDescription colorTargetDescriptions[1];
        colorTargetDescriptions[0] = {};
        colorTargetDescriptions[0].blend_state.enable_blend = true;
        colorTargetDescriptions[0].blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
        colorTargetDescriptions[0].blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
        colorTargetDescriptions[0].blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        colorTargetDescriptions[0].blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        colorTargetDescriptions[0].blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
        colorTargetDescriptions[0].blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
        colorTargetDescriptions[0].format = SDL_GetGPUSwapchainTextureFormat(device, window);

        pipelineInfo.target_info.num_color_targets = 1;
        pipelineInfo.target_info.color_target_descriptions = colorTargetDescriptions;

        // create the pipeline
        pipeline = SDL_CreateGPUGraphicsPipeline(device, &pipelineInfo);

        // we don't need to store the shaders after creating the pipeline
        SDL_ReleaseGPUShader(device, vertexShader);
        SDL_ReleaseGPUShader(device, fragmentShader);
    }

    void GpuPipeline::BindGpuPipeline(SDL_GPURenderPass *renderPass)
    {
        SDL_BindGPUGraphicsPipeline(renderPass, pipeline);
    }

    void GpuPipeline::Cleanup()
    {
        if (device && pipeline)
        {
            SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
        }
    }
}