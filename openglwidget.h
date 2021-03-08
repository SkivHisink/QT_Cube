#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
//
#include "cube.h"

#include <QSlider>

#include <chrono>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    using QOpenGLWidget::QOpenGLWidget;
    ~OpenGLWidget();
    void setProp(const double prop_n){
        prop = prop_n;
    }
    void setMorphType(const int mt_n){
        morph_type=mt_n;
    }
    void setFigureFill(const bool fill_n){
        figure_fill=fill_n;
    }
    void setFigureLine(const bool line_n){
        figure_line =line_n;
    }
    void setDepthState(const bool depth_n){
        depth_state=depth_n;
    }
    void setCullingState(const bool cull_n){
        culling_state=cull_n;
    }
    double getFPS(){
        return fps;
    }
    void setNumberOfObjectsX(const int val_n){
        nox=val_n;
    }
    void setNumberOfObjectsY(const int val_n){
        noy=val_n;
    }
    void setNumberOfObjectsZ(const int val_n){
        noz=val_n;
    }
protected:
   void mousePressEvent(QMouseEvent *e) override;
   void mouseReleaseEvent(QMouseEvent *e) override;
   void timerEvent(QTimerEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void initializeGL() override;
   // void resizeGL(int w, int h) override;
    void paintGL() override;
    void setAnimating(bool animating);
  //  void initShaders();
  //  void initTextures();
    void keyevent();
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void updateFPS();
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void showFPS();

private:
    void middle_point(int i, int j, int* max_indx);
    QBasicTimer timer;

    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_geom=0;
    GLint m_matrixUniform = 0;
    GLint m_matrixTrate = 0;
    GLfloat color_change=0.0f;
    QOpenGLShaderProgram *m_program = nullptr;
    QVector2D mousePressPosition;
    int m_frame = 0;

    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;
    float vertical_angle=60.0f;
    float aspectRatio=4.0f/3.0f;
    float x_coord=0;
    float y_coord=0;
    float z_coord=-4;
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLVertexArrayObject* vao;
    //
    Cube cube;
    //
    float prop = 0.0;
    int morph_type = 0;
    bool figure_fill = true;
    bool figure_line = false;
    bool depth_state = true;
    bool culling_state = true;
    //keycontrol
    std::vector<bool> pressed_button;
    //
    double fps = 0.0;
    std::chrono::high_resolution_clock::time_point begin;
    int nox=1;
    int noy=1;
    int noz=1;
};

#endif // OPENGLWIDGET_H
