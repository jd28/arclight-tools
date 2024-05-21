#include "TextureCache.hpp"

#include <nw/kernel/Resources.hpp>

#define CHECK_GL_ERRORS()                          \
    do {                                           \
        auto err = glGetError();                   \
        if (err) {                                 \
            LOG_F(ERROR, "openGL error: {}", err); \
        }                                          \
    } while (0)

std::pair<unsigned int, bool> load_texture(std::string_view resref, QOpenGLFunctions_3_3_Core* gl)
{
    unsigned int texture = std::numeric_limits<unsigned int>::max();

    auto data = nw::kernel::resman().demand_in_order(resref,
        {nw::ResourceType::dds, nw::ResourceType::plt, nw::ResourceType::tga});

    if (data.bytes.size() == 0) {
        LOG_F(ERROR, "Failed to locate image: {}", resref);
        return {texture, false};
    }

    if (data.name.type != nw::ResourceType::plt) {
        nw::Image img{std::move(data)};
        if (!img.valid()) {
            LOG_F(ERROR, "Failed to load image: {}.{}", resref, nw::ResourceType::to_string(data.name.type));
            return {texture, false};
        }
        gl->glGenTextures(1, &texture);
        gl->glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto format = img.channels() == 4 ? GL_RGBA : GL_RGB;

        gl->glTexImage2D(GL_TEXTURE_2D, 0, format, img.width(), img.height(), 0, format, GL_UNSIGNED_BYTE,
            img.data());
        gl->glGenerateMipmap(GL_TEXTURE_2D);
        return {texture, false};
    } else {
        nw::Plt plt{std::move(data)};
        if (!plt.valid()) {
            LOG_F(ERROR, "Failed to load image: {}.{}", resref, nw::ResourceType::to_string(data.name.type));
            return {texture, false};
        }
        gl->glGenTextures(1, &texture);
        gl->glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto format = GL_RG;

        gl->glTexImage2D(GL_TEXTURE_2D, 0, format, plt.width(), plt.height(), 0, format, GL_UNSIGNED_BYTE,
            plt.pixels());
        gl->glGenerateMipmap(GL_TEXTURE_2D);
        return {texture, true};
    }

    return {texture, false};
}

void TextureCache::load_placeholder(QOpenGLFunctions_3_3_Core* gl)
{
    place_holder_image_ = std::make_unique<nw::Image>("assets/templategrid_albedo.png");

    unsigned int texture = std::numeric_limits<unsigned int>::max();
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto format = place_holder_image_->channels() == 4 ? GL_RGBA : GL_RGB;

    gl->glTexImage2D(GL_TEXTURE_2D, 0, format,
        place_holder_image_->width(),
        place_holder_image_->height(), 0, format, GL_UNSIGNED_BYTE,
        place_holder_image_->data());
    gl->glGenerateMipmap(GL_TEXTURE_2D);

    place_holder_ = texture;
}

void TextureCache::load_palette_texture(QOpenGLFunctions_3_3_Core* gl)
{
    for (size_t i = 0; i < 10; ++i) {
        auto img = nw::kernel::resman().palette_texture(static_cast<nw::PltLayer>(i));

        unsigned int texture = std::numeric_limits<unsigned int>::max();
        gl->glGenTextures(1, &texture);
        gl->glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto format = img->channels() == 4 ? GL_RGBA : GL_RGB;

        gl->glTexImage2D(GL_TEXTURE_2D, 0, format,
            img->width(),
            img->height(), 0, format, GL_UNSIGNED_BYTE,
            img->data());
        gl->glGenerateMipmap(GL_TEXTURE_2D);

        palette_texture_[i] = texture;
    }
}

std::optional<std::pair<unsigned int, bool>> TextureCache::load(std::string_view resref, QOpenGLFunctions_3_3_Core* gl)
{
    absl::string_view needle{resref.data(), resref.size()};
    auto it = map_.find(needle);
    if (it == std::end(map_)) {
        // Create
        auto [texture, is_plt] = load_texture(resref, gl);
        if (texture == std::numeric_limits<unsigned int>::max()) {
            return std::make_pair(place_holder_, false);
        }
        auto i = map_.emplace(resref, TexturePayload{{}, texture, is_plt, 1});
        return std::make_pair(i.first->second.handle_, i.first->second.is_plt);
    } else {
        ++it->second.refcount_;
        return std::make_pair(it->second.handle_, it->second.is_plt);
    }
    // Failure
    return std::make_pair(place_holder_, false);
}
