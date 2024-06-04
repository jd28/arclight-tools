#ifndef CREATUREMODELVIEW_H
#define CREATUREMODELVIEW_H

#include "../renderer/model.hpp"
#include "../renderer/shader.hpp"

#include <QOpenGLFunctions_3_3_Core>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include <memory>

class QMouseEvent;
class QWheelEvent;

namespace nw {
struct Creature;
}

class BasicModelView : public QOpenGLWidget {
    Q_OBJECT
public:
    BasicModelView(QWidget* parent = nullptr);
    void setCreature(nw::Creature* creature);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void onDataChanged();

public slots:
    void onUpdateModelAnimation();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    nw::Creature* creature_ = nullptr;
    std::unique_ptr<Model> current_model_ = nullptr;
    QOpenGLContext* context_ = nullptr;
    QOpenGLFunctions_3_3_Core* funcs_;
    ShaderPrograms shader_;
    float height_;
    float width_;
    QPoint last_pos_;
    double azimuth_;
    double declination_;
    double distance_;
    int current_appearance_ = -1;
};

#endif // CREATUREMODELVIEW_H
