#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>
#include <sstream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->PropSlider, &QSlider::sliderMoved, this, &MainWindow::sliderSet);
    //for moving x axis
    QObject::connect(ui->XSlider, &QSlider::valueChanged, ui->widget, &OpenGLWidget::setXRotation);
    QObject::connect(ui->widget, &OpenGLWidget::xRotationChanged, ui->XSlider, &QSlider::setValue);
    //for moving y axis
    QObject::connect(ui->YSlider, &QSlider::valueChanged, ui->widget, &OpenGLWidget::setYRotation);
    QObject::connect(ui->widget, &OpenGLWidget::yRotationChanged, ui->YSlider, &QSlider::setValue);
    ////for moving z axis
    QObject::connect(ui->ZSlider, &QSlider::valueChanged, ui->widget, &OpenGLWidget::setZRotation);
    QObject::connect(ui->widget, &OpenGLWidget::zRotationChanged, ui->ZSlider, &QSlider::setValue);
    //Sphere Morph
    QObject::connect(ui->SphereMorphBox, &QCheckBox::stateChanged, this, &MainWindow::SphereMorph);
    //
    QObject::connect(ui->FillBox, &QCheckBox::stateChanged, this, &MainWindow::FillMode);
    QObject::connect(ui->LineBox, &QCheckBox::stateChanged, this, &MainWindow::FillMode);
    QObject::connect(ui->DCBox, &QCheckBox::stateChanged, this, &MainWindow::FillMode);
    QObject::connect(ui->CullingBox, &QCheckBox::stateChanged, this, &MainWindow::FillMode);
    //Sphere Morph
    QObject::connect(ui->StrangeBox, &QCheckBox::stateChanged, this, &MainWindow::StrangeMorph);
    //
    //Sphere Morph
    QObject::connect(ui->NMBox, &QCheckBox::stateChanged, this, &MainWindow::NoMorph);

    //QObject::connect(ui->label_fps);
    QObject::connect(ui->widget,&OpenGLWidget::showFPS, this, &MainWindow::showFPS);
    //Numbers of objects on scene
    QObject::connect(ui->NumbersOfObjectsX, &QSlider::valueChanged, this, &MainWindow::changeNumberOfObj);
    QObject::connect(ui->NumbersOfObjectsY, &QSlider::valueChanged, this, &MainWindow::changeNumberOfObj);
    QObject::connect(ui->NumbersOfObjectsZ, &QSlider::valueChanged, this, &MainWindow::changeNumberOfObj);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeNumberOfObj()
{
    ui->widget->setNumberOfObjectsX(ui->NumbersOfObjectsX->value());
    ui->widget->setNumberOfObjectsY(ui->NumbersOfObjectsY->value());
    ui->widget->setNumberOfObjectsZ(ui->NumbersOfObjectsZ->value());
}

void MainWindow::showFPS()
{
    std::string lel="FPS:"+std::to_string(ui->widget->getFPS());
    ui->label_fps->clear();
    ui->label_fps->setText(lel.c_str());
    ui->label_fps->update();
}

void MainWindow::sliderSet()
{
    ui->widget->setProp(static_cast<float>(ui->PropSlider->value())/100);
}

void MainWindow::NoMorph()
{
    if(ui->NMBox->checkState()){
        ui->widget->setMorphType(0);
    ui->StrangeBox->setChecked(false);
    ui->SphereMorphBox->setChecked(false);
    ui->NMBox->setChecked(true);
    }
}

void MainWindow::SphereMorph()
{
    if(ui->SphereMorphBox->checkState()){
        ui->widget->setMorphType(1);
    ui->StrangeBox->setChecked(false);
    ui->SphereMorphBox->setChecked(true);
    ui->NMBox->setChecked(false);
    }
}

void MainWindow::StrangeMorph()
{
    if(ui->StrangeBox->checkState()){
        ui->widget->setMorphType(2);
    ui->StrangeBox->setChecked(true);
    ui->SphereMorphBox->setChecked(false);
    ui->NMBox->setChecked(false);
    }
}
void MainWindow::FillMode()
{
    ui->widget->setFigureFill(ui->FillBox->checkState());
    ui->widget->setFigureLine(ui->LineBox->checkState());
    ui->widget->setDepthState(ui->DCBox->checkState());
    ui->widget->setCullingState(ui->CullingBox->checkState());
}
