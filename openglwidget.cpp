#include "openglwidget.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>

#include <QMouseEvent>
#include <QKeyEvent>

#include <QColorDialog>
static const char* vertexShaderSource =
"attribute highp vec4 posAttr;\n"
"attribute lowp vec4 colAttr;\n"
"varying lowp vec4 col;\n"
"uniform highp mat4 matrix;\n"
"void main() {\n"
"   col = colAttr;\n"
"   gl_Position = matrix * posAttr;\n"
"}\n";

static const char* fragmentShaderSource =
"varying lowp vec4 col;\n"
"void main() {\n"
"   gl_FragColor = col;\n"
"}\n";

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //

    //
    setFocusPolicy(Qt::StrongFocus);
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);


    vao = new QOpenGLVertexArrayObject(this);
    vao->create();
    vao->bind();

    arrayBuf = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    arrayBuf.create();
    arrayBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    arrayBuf.bind();
    arrayBuf.allocate(cube.vertices.data(), cube.vertices.size() * sizeof(float));

    indexBuf = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexBuf.create();
    indexBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    indexBuf.bind();
    indexBuf.allocate(cube.indices.data(), cube.indices.size() * sizeof(GLushort));

    m_program->enableAttributeArray(m_posAttr);
    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0, 3, 6 * sizeof(float));

    m_program->enableAttributeArray(m_colAttr);
    m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    //key control
    pressed_button.assign(60, false);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* e)
{
    mousePressPosition = QVector2D(e->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void OpenGLWidget::timerEvent(QTimerEvent*)
{
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    }
    else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    }
    // Request an update
    update();
}


void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_POLYGON_OFFSET_FILL);
    glCullFace(GL_BACK);//GL_BACK
    ////good to add later glPolygonMode(GL_FRONT, GL_LINE); and smthing like this
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT, GL_FILL);
    m_program->bind();
    QMatrix4x4 matrix;
    matrix.perspective(vertical_angle, aspectRatio, 0.1f, 100.0f);
    matrix.translate(x_coord, y_coord, z_coord);
    matrix.rotate(rotation);
    //keyEvent
    keyevent();
    for (int i = 0; i < 8; ++i)
    {
        cube.vertices[i * 6 + 3] += color_change;
        cube.vertices[i * 6 + 4] += color_change;
        cube.vertices[i * 6 + 5] += color_change;
    }
    color_change = 0;
    m_program->setUniformValue(m_matrixUniform, matrix);
    vao->bind();
    arrayBuf.write(0, cube.vertices.data(), cube.vertices.size() * sizeof(float));

    glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_SHORT, 0);
    vao->release();
    m_program->release();

    ++m_frame;
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    doneCurrent();
    arrayBuf.destroy();
    indexBuf.destroy();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event) {

    auto key_event = event->key();
    if (key_event == Qt::Key_Escape && pressed_button[0] == false)
        close();
    if (key_event == Qt::Key_F1 && pressed_button[1] == false)
        pressed_button[1] = true;
    if (key_event == Qt::Key_PageUp && pressed_button[2] == false)
        pressed_button[2] = true;
    if (key_event == Qt::Key_PageDown && pressed_button[3] == false)
        pressed_button[3] = true;
    if (key_event == Qt::Key_R && pressed_button[4] == false)
        pressed_button[4] = true;
    if (key_event == Qt::Key_F && pressed_button[5] == false)
        pressed_button[5] = true;
    if (key_event == Qt::Key_T && pressed_button[6] == false)
        pressed_button[6] = true;
    if (key_event == Qt::Key_G && pressed_button[7] == false)
        pressed_button[7] = true;
    if (key_event == Qt::Key_Y && pressed_button[8] == false)
        pressed_button[8] = true;
    if (key_event == Qt::Key_H && pressed_button[9] == false)
        pressed_button[9] = true;
    if (key_event == Qt::Key_U && pressed_button[10] == false)
        pressed_button[10] = true;
    if (key_event == Qt::Key_J && pressed_button[11] == false)
        pressed_button[11] = true;
    if (key_event == Qt::Key_Z && pressed_button[12] == false)
        pressed_button[12] = true;
    if (key_event == Qt::Key_X && pressed_button[13] == false)
        pressed_button[13] = true;
    if (key_event == Qt::Key_W && pressed_button[15] == false)
        pressed_button[15] = true;
    if (key_event == Qt::Key_A && pressed_button[16] == false)
        pressed_button[16] = true;
    if (key_event == Qt::Key_S && pressed_button[17] == false)
        pressed_button[17] = true;
    if (key_event == Qt::Key_D && pressed_button[18] == false)
        pressed_button[18] = true;
    if (key_event == Qt::Key_Q && pressed_button[19] == false)
        pressed_button[19] = true;
    if (key_event == Qt::Key_E && pressed_button[20] == false)
        pressed_button[20] = true;
    if (key_event == Qt::Key_C)
        pressed_button[21] = !pressed_button[21];
    if (key_event == Qt::Key_V)
        pressed_button[22] = !pressed_button[22];
    if (key_event == Qt::Key_B)
        pressed_button[23] = !pressed_button[23];
    if (key_event == Qt::Key_1 && pressed_button[24] == false)
        pressed_button[24] = true;
    if (key_event == Qt::Key_2 && pressed_button[25] == false)
        pressed_button[25] = true;
    if (key_event == Qt::Key_3 && pressed_button[26] == false)
        pressed_button[26] = true;
    if (key_event == Qt::Key_4 && pressed_button[27] == false)
        pressed_button[27] = true;
    if (key_event == Qt::Key_5 && pressed_button[28] == false)
        pressed_button[28] = true;
    if (key_event == Qt::Key_6 && pressed_button[29] == false)
        pressed_button[29] = true;
    if (key_event == Qt::Key_7 && pressed_button[30] == false)
        pressed_button[30] = true;
    if (key_event == Qt::Key_8 && pressed_button[31] == false)
        pressed_button[31] = true;
    if (key_event == Qt::Key_9 && pressed_button[32] == false)
        pressed_button[32] = true;
    if (key_event == Qt::Key_0 && pressed_button[33] == false)
        pressed_button[33] = true;
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event) {

    auto key_event = event->key();
    if (key_event == Qt::Key_F1)
        pressed_button[1] = false;
    if (key_event == Qt::Key_PageUp)
        pressed_button[2] = false;
    if (key_event == Qt::Key_PageDown)
        pressed_button[3] = false;
    if (key_event == Qt::Key_R)
        pressed_button[4] = false;
    if (key_event == Qt::Key_F)
        pressed_button[5] = false;
    if (key_event == Qt::Key_T)
        pressed_button[6] = false;
    if (key_event == Qt::Key_G)
        pressed_button[7] = false;
    if (key_event == Qt::Key_Y)
        pressed_button[8] = false;
    if (key_event == Qt::Key_H)
        pressed_button[9] = false;
    if (key_event == Qt::Key_U)
        pressed_button[10] = false;
    if (key_event == Qt::Key_J)
        pressed_button[11] = false;
    if (key_event == Qt::Key_Z)
        pressed_button[12] = false;
    if (key_event == Qt::Key_X)
        pressed_button[13] = false;
    if (key_event == Qt::Key_W)
        pressed_button[15] = false;
    if (key_event == Qt::Key_A)
        pressed_button[16] = false;
    if (key_event == Qt::Key_S)
        pressed_button[17] = false;
    if (key_event == Qt::Key_D)
        pressed_button[18] = false;
    if (key_event == Qt::Key_Q)
        pressed_button[19] = false;
    if (key_event == Qt::Key_E)
        pressed_button[20] = false;
    if (key_event == Qt::Key_1)
        pressed_button[24] = false;
    if (key_event == Qt::Key_2)
        pressed_button[25] = false;
    if (key_event == Qt::Key_3)
        pressed_button[26] = false;
    if (key_event == Qt::Key_4)
        pressed_button[27] = false;
    if (key_event == Qt::Key_5)
        pressed_button[28] = false;
    if (key_event == Qt::Key_6)
        pressed_button[29] = false;
    if (key_event == Qt::Key_7)
        pressed_button[30] = false;
    if (key_event == Qt::Key_8)
        pressed_button[31] = false;
    if (key_event == Qt::Key_9)
        pressed_button[32] = false;
    if (key_event == Qt::Key_0)
        pressed_button[33] = false;
}

void OpenGLWidget::keyevent()
{
    if (pressed_button[1])
    {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    }
    if (pressed_button[2])
    {
        color_change += 0.1f;
    }
    if (pressed_button[3])
    {
        color_change -= 0.1f;
    }
    if (pressed_button[4])//R
    {
        vertical_angle -= 1.0f;
    }
    if (pressed_button[5])//F
    {
        vertical_angle += 1.0f;
    }
    if (pressed_button[6])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 1) * rotation;
    }
    if (pressed_button[7])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -1) * rotation;
    }
    if (pressed_button[8])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 1) * rotation;
    }
    if (pressed_button[9])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), -1) * rotation;
    }
    if (pressed_button[10])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 1) * rotation;
    }
    if (pressed_button[11])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), -1) * rotation;
    }
    if (pressed_button[12])
    {
        aspectRatio += 0.1f;
    }
    if (pressed_button[13])
    {
        aspectRatio -= 0.1f;
    }
    if (pressed_button[14])
    {

    }
    if (pressed_button[15])
    {
        z_coord += 0.1f;
    }
    if (pressed_button[16])
    {
        x_coord += 0.1f;
    }
    if (pressed_button[17])
    {
        z_coord -= 0.1f;
    }
    if (pressed_button[18])
    {
        x_coord -= 0.1f;
    }
    if (pressed_button[19])
    {
        y_coord += 0.1f;
    }
    if (pressed_button[20])
    {
        y_coord -= 0.1f;
    }
    if (pressed_button[21])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 1) * rotation;
    }
    if (pressed_button[22])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 1) * rotation;
    }
    if (pressed_button[23])
    {
        rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 1), 1) * rotation;
    }
    if (pressed_button[24])//1 is red
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 1;
            cube.vertices[i * 6 + 4] = 0;
            cube.vertices[i * 6 + 5] = 0;
        }
    }
    if (pressed_button[25])//2 is green
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 0;
            cube.vertices[i * 6 + 4] = 1;
            cube.vertices[i * 6 + 5] = 0;
        }
    }
    if (pressed_button[26])//3 is blue
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 0;
            cube.vertices[i * 6 + 4] = 0;
            cube.vertices[i * 6 + 5] = 1;
        }
    }
    if (pressed_button[27])//4 is aqua
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 0;
            cube.vertices[i * 6 + 4] = 1;
            cube.vertices[i * 6 + 5] = 1;
        }
    }
    if (pressed_button[28])//5 is pink
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 1;
            cube.vertices[i * 6 + 4] = 0;
            cube.vertices[i * 6 + 5] = 1;
        }
    }
    if (pressed_button[29])//6 is yellow
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 1;
            cube.vertices[i * 6 + 4] = 1;
            cube.vertices[i * 6 + 5] = 0;
        }
    }
    if (pressed_button[30])//7 is black and white
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = (float)i / 8;
            cube.vertices[i * 6 + 4] = (float)i / 8;
            cube.vertices[i * 6 + 5] = (float)i / 8;
        }
    }
    if (pressed_button[31])//8 is inverse black and white
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 1.0 - (float)i / 8;
            cube.vertices[i * 6 + 4] = 1.0 - (float)i / 8;
            cube.vertices[i * 6 + 5] = 1.0 - (float)i / 8;
        }
    }
    if (pressed_button[32])//9 is color inversion
    {
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 3] = 1.0 - cube.vertices[i * 6 + 3];
            cube.vertices[i * 6 + 4] = 1.0 - cube.vertices[i * 6 + 4];
            cube.vertices[i * 6 + 5] = 1.0 - cube.vertices[i * 6 + 5];
        }
        pressed_button[32] = !pressed_button[32];
    }
    if (pressed_button[33])//return standart colors
    {
        int a = 0;
        int b = 0;
        int c = 0;
        for (int i = 0; i < 8; ++i)
        {
            cube.vertices[i * 6 + 5] = a;
            cube.vertices[i * 6 + 4] = b;
            cube.vertices[i * 6 + 3] = c;
            ++a;
            if (a == 2) {
                a = 0;
                ++b;
                if (b == 2) {
                    b = 0;
                    ++c;
                }
            }
        }
    }
}
