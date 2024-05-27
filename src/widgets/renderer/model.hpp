#pragma once

#include <nw/formats/Plt.hpp>
#include <nw/model/Mdl.hpp>
#include <nw/objects/Appearance.hpp>

#include <vector>

#include <QOpenGLFunctions_3_3_Core>

#define CHECK_GL_ERRORS()                          \
    do {                                           \
        auto err = gl->glGetError();               \
        if (err) {                                 \
            LOG_F(ERROR, "openGL error: {}", err); \
        }                                          \
    } while (0)

struct ShaderPrograms;
struct Model;

namespace nw {
struct Area;
} // namespace nw

struct Node {
    virtual ~Node() = default;

    virtual void draw(ShaderPrograms& shader, const glm::mat4x4& mtx, QOpenGLFunctions_3_3_Core* gl);
    glm::mat4 get_transform() const;
    virtual void reset() { }

    Model* owner_ = nullptr;
    nw::model::Node* orig_ = nullptr;
    Node* parent_ = nullptr;
    glm::mat4 inverse_{1.0f};
    glm::vec3 position_{0.0f};
    glm::quat rotation_{};
    glm::vec3 scale_ = glm::vec3(1.0);
    std::vector<Node*> children_;
    bool has_transform_ = false;
    bool no_render_ = false;
};

struct Mesh : public Node {
    virtual void reset() override { }
    virtual void draw(ShaderPrograms& shader, const glm::mat4x4& mtx, QOpenGLFunctions_3_3_Core* gl) override;

    unsigned int vao_, vbo_, ebo_;
    uint16_t num_vertices_ = 0;
    uint8_t* vertices_ = nullptr;
    uint32_t num_indices_ = 0;
    uint16_t* indices_ = nullptr;
    nw::PltColors plt_colors_{};

    unsigned int texture0;
    bool texture0_is_plt = false;
};

struct Skin : public Node {
    virtual void reset() override { }
    // Submits mesh data to the GPU
    virtual void draw(ShaderPrograms& shader, const glm::mat4x4& mtx, QOpenGLFunctions_3_3_Core* gl) override;

    void build_inverse_binds();
    unsigned int vao_, vbo_, ebo_;
    uint16_t num_vertices_ = 0;
    uint8_t* vertices_ = nullptr;
    uint32_t num_indices_ = 0;
    uint16_t* indices_ = nullptr;
    nw::PltColors plt_colors_{};
    std::vector<glm::mat4> inverse_bind_pose_;
    std::array<glm::mat4, 64> joints_;

    unsigned int texture0;
    bool texture0_is_plt = false;
};

struct Model : public Node {
    nw::model::Model* mdl_ = nullptr;
    nw::model::Animation* anim_ = nullptr;
    int32_t anim_cursor_ = 0;
    std::vector<std::unique_ptr<Node>> nodes_;

    /// Finds a node by name
    Node* find(std::string_view name);

    /// Initialize skin meshes & joints
    void initialize_skins();

    /// Loads model from a NWN model
    bool load(nw::model::Model* mdl, QOpenGLFunctions_3_3_Core* gl);

    /// Loads an animation
    bool load_animation(std::string_view anim);

    /// Updates the animimation by ``dt`` milliseconds.
    void update(int32_t dt);

    virtual void draw(ShaderPrograms& shader, const glm::mat4x4& mtx, QOpenGLFunctions_3_3_Core* gl) override;

private:
    // Internal node loading
    Node* load_node(nw::model::Node* node, QOpenGLFunctions_3_3_Core* gl, Node* parent = nullptr);
};

std::unique_ptr<Model> load_model(std::string_view resref, QOpenGLFunctions_3_3_Core* gl);

// == BasicTileArea ===========================================================
// ============================================================================

class BasicTileArea : public Node {
public:
    BasicTileArea(nw::Area* area);

    virtual void draw(ShaderPrograms& shader, const glm::mat4x4& mtx, QOpenGLFunctions_3_3_Core* gl) override;
    void load_tile_models(QOpenGLFunctions_3_3_Core* gl);

    /// Updates the animimation by ``dt`` milliseconds.
    void update(int32_t dt);

    nw::Area* area_ = nullptr;
    std::vector<std::unique_ptr<Model>> tile_models_;
};
