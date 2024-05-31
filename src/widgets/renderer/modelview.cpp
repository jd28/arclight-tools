#include "modelview.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <QMouseEvent>
#include <QOpenGLDebugLogger>
#include <QOpenGLVersionFunctionsFactory>
#include <QTimer>
#include <QWheelEvent>

extern TextureCache s_textures;

ModelView::ModelView(QWidget* parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat fmt;
    fmt.setSamples(16);
    fmt.setDepthBufferSize(24);
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(fmt);

    // QTimer* timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, &ModelView::onUpdateModelAnimation);
    // timer->start(16);
}

void ModelView::setNode(Node* node)
{
    node_ = node;
    setFocus();
    makeCurrent();
    update();
}

void ModelView::initializeGL()
{
    context_ = context();
    funcs_ = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(context_);

    funcs_->glEnable(GL_DEPTH_TEST);
    funcs_->glEnable(GL_CULL_FACE);

    QOpenGLDebugLogger* logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, [](const QOpenGLDebugMessage& msg) {
        LOG_F(INFO, "open gl error: {}", msg.message().toStdString());
    });
    logger->startLogging();

    s_textures.load_placeholder(funcs_);
    s_textures.load_palette_texture(funcs_);
    shader_.basic.load(R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec3 aNormal;
        layout (location = 3) in vec4 aTangent;
        
        out vec2 TexCoord;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main()
        {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        })",
        R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec2 TexCoord;
        
        uniform sampler2D ourTexture;
        
        void main()
        {
            FragColor = texture(ourTexture, TexCoord);
        })",
        funcs_);

    shader_.skin.load(R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec2 aTexCoord;
        layout(location = 2) in vec3 aNormal;
        layout(location = 3) in vec4 aTangent;
        layout(location = 4) in ivec4 aIndices;
        layout(location = 5) in vec4 aWeights;
        
        out vec2 TexCoord;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        const int MAX_BONES = 64;
        const int MAX_BONE_INFLUENCE = 4;
        uniform mat4 joints[MAX_BONES];
        
        void main()
        {
            vec4 localPosition = vec4(0.0f);
            for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
            {
                if(aIndices[i] == -1) 
                    continue;
                if(aIndices[i] >= MAX_BONES) 
                {
                    localPosition = vec4(aPos,1.0f);
                    break;
                }
                mat4 boneTransform = joints[aIndices[i]];
                vec4 posePosition = boneTransform * vec4(aPos,1.0f);
                localPosition += posePosition * aWeights[i];
            }
                	
            gl_Position =  projection *  view * model * localPosition;
            TexCoord = aTexCoord;
        })",
        R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec2 TexCoord;
        
        uniform sampler2D ourTexture;
        
        void main()
        {
            FragColor = texture(ourTexture, TexCoord);
        })",
        funcs_);

    // Initialize camera parameters
    cameraPosition = glm::vec3(0.0f, 50.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    yaw = -90.0f;
    pitch = -89.0f;
    updateCameraVectors();

    // Set camera movement and rotation speeds
    movementSpeed = 2.5f;
    rotationSpeed = 2.5f;

    emit initialized();
}

void ModelView::onUpdateModelAnimation()
{
    update();
}

void ModelView::resizeGL(int w, int h)
{
    funcs_->glViewport(0, 0, w, h);
    height_ = float(h);
    width_ = float(w);
}

void ModelView::keyPressEvent(QKeyEvent* event)
{
    Qt::KeyboardModifiers modifiers = event->modifiers();

    switch (event->key()) {
    case Qt::Key_W:
        moveCameraForward();
        break;
    case Qt::Key_S:
        moveCameraBackward();
        break;
    case Qt::Key_A:
        moveCameraLeft();
        break;
    case Qt::Key_D:
        moveCameraRight();
        break;
    case Qt::Key_Left:
        yawCameraLeft();
        break;
    case Qt::Key_Right:
        yawCameraRight();
        break;
    }

    // Handle pitch control (Ctrl + Up/Down Arrow)
    if (modifiers & Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_Up:
            increasePitch();
            break;
        case Qt::Key_Down:
            decreasePitch();
            break;
        }
    } else {
        switch (event->key()) {
        case Qt::Key_Up:
            moveCameraUp();
            break;
        case Qt::Key_Down:
            moveCameraDown();
            break;
        }
    }

    LOG_F(INFO, "cameraPosition: {} {} {}", cameraPosition.x, cameraPosition.y, cameraPosition.z);

    update();
}

void ModelView::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        cameraPosition += cameraFront * movementSpeed;
    } else {
        cameraPosition -= cameraFront * movementSpeed;
    }
    update();
}

void ModelView::paintGL()
{
    auto gl = funcs_;
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    if (node_) {
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        float aspect = width_ / height_;
        auto proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);

        shader_.skin.use(gl);
        CHECK_GL_ERRORS();
        shader_.skin.set_uniform("view", view, gl);
        CHECK_GL_ERRORS();
        shader_.skin.set_uniform("projection", proj, gl);
        CHECK_GL_ERRORS();
        gl->glUseProgram(0);

        shader_.basic.use(gl);
        CHECK_GL_ERRORS();
        shader_.basic.set_uniform("view", view, gl);
        CHECK_GL_ERRORS();
        shader_.basic.set_uniform("projection", proj, gl);
        CHECK_GL_ERRORS();
        gl->glUseProgram(0);

        glm::mat4 mtx = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), {0.0f, 0.0f, 1.0f});
        mtx = glm::rotate(mtx, glm::radians(90.0f), {1.0f, 0.0f, 0.0f});

        node_->draw(shader_, mtx, gl);
        gl->glUseProgram(0);
    }
}

void ModelView::moveCameraForward()
{
    glm::vec3 forward = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * cameraFront);
    cameraPosition = cameraPosition + forward * movementSpeed;
}

void ModelView::moveCameraBackward()
{
    glm::vec3 forward = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * cameraFront);
    cameraPosition = cameraPosition - forward * movementSpeed;
}

void ModelView::moveCameraLeft()
{
    cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * movementSpeed;
}

void ModelView::moveCameraRight()
{
    cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * movementSpeed;
}

void ModelView::moveCameraUp()
{
    cameraPosition += cameraUp * movementSpeed;
}

void ModelView::moveCameraDown()
{
    cameraPosition -= cameraUp * movementSpeed;
}

void ModelView::yawCameraLeft()
{
    yaw -= rotationSpeed;
    updateCameraVectors();
}

void ModelView::yawCameraRight()
{
    yaw += rotationSpeed;
    updateCameraVectors();
}

void ModelView::increasePitch()
{
    pitch += rotationSpeed;
    if (pitch > 89.0f) pitch = 89.0f;
    updateCameraVectors();
}

void ModelView::decreasePitch()
{
    pitch -= rotationSpeed;
    if (pitch < -89.0f) pitch = -89.0f;
    updateCameraVectors();
}

void ModelView::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}
