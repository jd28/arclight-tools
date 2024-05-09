#include "util.hpp"

#include "glm/ext/quaternion_float.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <nw/log.hpp>

// std::filesystem::path get_shader_path()
// {
//     std::filesystem::path path;
//     switch (bgfx::getRendererType()) {
//     case bgfx::RendererType::Noop:
//     case bgfx::RendererType::Direct3D9:
//         path = "shaders/dx9/";
//         break;
//     case bgfx::RendererType::Direct3D11:
//     case bgfx::RendererType::Direct3D12:
//         path = "shaders/dx11/";
//         break;
//     case bgfx::RendererType::Agc:
//     case bgfx::RendererType::Gnm:
//         path = "shaders/pssl/";
//         break;
//     case bgfx::RendererType::Metal:
//         path = "shaders/metal/";
//         break;
//     case bgfx::RendererType::Nvn:
//         path = "shaders/nvn/";
//         break;
//     case bgfx::RendererType::OpenGL:
//         path = "shaders/glsl/";
//         break;
//     case bgfx::RendererType::OpenGLES:
//         path = "shaders/essl/";
//         break;
//     case bgfx::RendererType::Vulkan:
//         path = "shaders/spirv/";
//         break;
//     case bgfx::RendererType::WebGPU:
//         path = "shaders/spirv/";
//         break;

//     case bgfx::RendererType::Count:
//         BX_ASSERT(false, "You should not be here!");
//         break;
//     }
//     return path;
// }

void log_matrix(const float* mtx)
{
    LOG_F(INFO, "\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]",
        mtx[0], mtx[1], mtx[2], mtx[3],
        mtx[4], mtx[5], mtx[6], mtx[7],
        mtx[8], mtx[9], mtx[10], mtx[11],
        mtx[12], mtx[13], mtx[14], mtx[15]);
}

void log_matrix(const glm::mat4& mtx)
{
    log_matrix(glm::value_ptr(mtx));
}
