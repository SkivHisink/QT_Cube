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

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    using QOpenGLWidget::QOpenGLWidget;
    ~OpenGLWidget();
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

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
private:
    QBasicTimer timer;

    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
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
    //keycontrol
    std::vector<bool> pressed_button;
};

#endif // OPENGLWIDGET_H
