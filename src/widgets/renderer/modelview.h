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

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

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
    double azimuth_;
    double declination_;
    double distance_;
};

#endif // MODELVIEW_H
