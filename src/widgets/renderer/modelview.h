#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "../renderer/TextureCache.hpp"
#include "../renderer/model.hpp"
#include "../renderer/shader.hpp"

#include <QOpenGLFunctions_3_3_Core>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include <memory>

class QMouseEvent;
class QWheelEvent;

class ModelView : public QOpenGLWidget {
    Q_OBJECT
public:
    ModelView(QWidget* parent = nullptr);
    void setNode(Node* node);

    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void focusInEvent(QFocusEvent* event) override
    {
        QOpenGLWidget::focusInEvent(event);
    }

    void focusOutEvent(QFocusEvent* event) override
    {
        QOpenGLWidget::focusOutEvent(event);
    }

    void onDataChanged();

    QOpenGLFunctions_3_3_Core* funcs() { return funcs_; };

public slots:
    void onUpdateModelAnimation();

signals:
    void initialized();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Node* node_ = nullptr;
    QOpenGLContext* context_ = nullptr;
    QOpenGLFunctions_3_3_Core* funcs_ = nullptr;
    ShaderPrograms shader_;
    float height_;
    float width_;
    QPoint last_pos_;

    // Camera parameters
    glm::vec3 cameraPosition{0.0f, 45.0f, 0.0f};
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    float yaw = 0.0f;
    float pitch = 9.0f;

    // Camera movement and rotation speeds
    float movementSpeed;
    float rotationSpeed;

    // Camera control functions
    void moveCameraForward();
    void moveCameraBackward();
    void moveCameraLeft();
    void moveCameraRight();
    void moveCameraUp();
    void moveCameraDown();
    void yawCameraLeft();
    void yawCameraRight();
    void increasePitch();
    void decreasePitch();
    void updateCameraVectors();
};

#endif // MODELVIEW_H
