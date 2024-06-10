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
struct Placeable;
}

class BasicModelView : public QOpenGLWidget {
    Q_OBJECT
public:
    BasicModelView(QWidget* parent = nullptr);

    QOpenGLFunctions_3_3_Core* funcs() const { return funcs_; }
    void setCreature(nw::Creature* creature);
    void setModel(std::unique_ptr<Model> model);

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

signals:
    void initialized();

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
