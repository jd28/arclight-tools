#include "areaview.h"
#include "ui_areaview.h"

#include "nw/objects/Area.hpp"

AreaView::AreaView(nw::Area* area, QWidget* parent)
    : ArclightView(parent)
    , ui(new Ui::AreaView)
    , area_{area}
{
    ui->setupUi(this);
    ui->openGLWidget->makeCurrent();
    connect(ui->openGLWidget, &ModelView::initialized, this, &AreaView::load_model);
}

AreaView::~AreaView()
{
    delete ui;
}

void AreaView::load_model()
{
    ui->openGLWidget->makeCurrent();
    area_model_ = new BasicTileArea(area_);
    area_model_->load_tile_models(ui->openGLWidget->funcs());
    ui->openGLWidget->setNode(area_model_);
    ui->openGLWidget->setFocus();
}
