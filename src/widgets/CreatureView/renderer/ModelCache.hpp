#pragma once

#include "model.hpp"

#include <absl/container/flat_hash_map.h>

#include <memory>
#include <string_view>

struct ModelPayload {
    std::unique_ptr<nw::model::Mdl> original_;
    uint32_t refcount_ = 0;
};

struct ModelCache {
    std::unique_ptr<Model> load(std::string_view resref, QOpenGLFunctions_3_3_Core* gl);

    absl::flat_hash_map<std::string, ModelPayload> map_;
};
