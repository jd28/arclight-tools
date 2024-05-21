#include "ModelCache.hpp"

#include <nw/kernel/Resources.hpp>

std::unique_ptr<Model> ModelCache::load(std::string_view resref, QOpenGLFunctions_3_3_Core* gl)
{

    absl::string_view sv{resref.data(), resref.size()};
    auto it = map_.find(sv);
    if (it != std::end(map_)) {
        ++it->second.refcount_;
        auto result = new Model();
        result->load(&it->second.original_->model, gl);
        return std::unique_ptr<Model>(result);
    }

    auto data = nw::kernel::resman().demand({resref, nw::ResourceType::mdl});
    if (data.bytes.size() == 0) {
        LOG_F(ERROR, "Failed to find model: {}", resref);
        return nullptr;
    }

    auto model = std::make_unique<nw::model::Mdl>(std::move(data));
    if (!model->valid()) {
        LOG_F(ERROR, "Failed to parse model: {}", resref);
        return nullptr;
    }

    auto mdl = new Model;
    if (!mdl->load(&model.get()->model, gl)) {
        LOG_F(ERROR, "Failed to load model: {}", resref);
        return nullptr;
    }
    map_.emplace(std::string(resref), ModelPayload{std::move(model), 1});
    return std::unique_ptr<Model>(mdl);
}
