#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QSlider>

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
    //Sphere Morph
    QObject::connect(ui->StrangeBox, &QCheckBox::stateChanged, this, &MainWindow::StrangeMorph);
    //
    //Sphere Morph
    QObject::connect(ui->NMBox, &QCheckBox::stateChanged, this, &MainWindow::NoMorph);
    //
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sliderSet()
{
    ui->widget->prop = static_cast<float>(ui->PropSlider->value())/100;
}
void MainWindow::NoMorph()
{
    if(ui->NMBox->checkState()){
        ui->widget->morph_type=0;
    ui->StrangeBox->setChecked(false);
    ui->SphereMorphBox->setChecked(false);
    ui->NMBox->setChecked(true);
    }
}
void MainWindow::SphereMorph()
{
    if(ui->SphereMorphBox->checkState()){
        ui->widget->morph_type=1;
    ui->StrangeBox->setChecked(false);
    ui->SphereMorphBox->setChecked(true);
    ui->NMBox->setChecked(false);
    }
}

void MainWindow::StrangeMorph()
{
    if(ui->StrangeBox->checkState()){
        ui->widget->morph_type=2;
    ui->StrangeBox->setChecked(true);
    ui->SphereMorphBox->setChecked(false);
    ui->NMBox->setChecked(false);
    }
}
void MainWindow::FillMode()
{
    ui->widget->figure_fill=ui->FillBox->checkState();
    ui->widget->figure_line=ui->LineBox->checkState();
    ui->widget->dc_state = ui->DCBox->checkState();
}
