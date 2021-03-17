#include "openglwidget.h"

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QScreen>
#include <QtMath>

#include <QMouseEvent>
#include <QKeyEvent>

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void OpenGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void OpenGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void OpenGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}
void OpenGLWidget::middle_point(int i, int j, int* max_indx)
{
    auto& vert=cube.vertices;
    auto ver_1_1=vert[(i)*6+0];
    auto ver_1_2=vert[(j)*6+0];
    auto ver_2_1=vert[(i)*6+1];
    auto ver_2_2=vert[(j)*6+1];
    auto ver_3_1=vert[(i)*6+2];
    auto ver_3_2=vert[(j)*6+2];

        vert.push_back((ver_1_1+ver_1_2)/2);
        vert.push_back((ver_2_1+ver_2_2)/2);
        vert.push_back((ver_3_1+ver_3_2)/2);
        vert.push_back(abs(vert[(i)*6+3]+vert[(j)*6+3])/2);
        vert.push_back(abs(vert[(i)*6+4]+vert[(j)*6+4])/2);
        vert.push_back(abs(vert[(i)*6+5]+vert[(j)*6+5])/2);

    ++*max_indx;
}
int number_of_triangle_breaking(int num)
{
    int numb=0;
    for(int i=0;i<num;++i){
        numb=numb+static_cast<int>(6*pow(2, 2*i+1));
    }
    return numb;
}
void OpenGLWidget::initializeGL()
{

    initializeOpenGLFunctions();
    setFocusPolicy(Qt::StrongFocus);
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\Skiv2\\Documents\\GitHub\\QT_Cube\\vertex.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Geometry, "C:\\Users\\Skiv2\\Documents\\GitHub\\QT_Cube\\geometry.gsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,"C:\\Users\\Skiv2\\Documents\\GitHub\\QT_Cube\\fragment.fsh");
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    m_matrixTrate = m_program->uniformLocation("trate");
    Q_ASSERT(m_matrixUniform != -1);
    m_program->log();
    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
    //
   // int num=3;
    int max_indx=8;
    auto& indxs= cube.indices;
    //!create new vector!
    int numb_of_crop=number_of_triangle_breaking(4);//12 |60|252|1020|formula: prev+6*2^(iter_num+2)
    std::vector<float> new_vert;
    for(int i=0;i<numb_of_crop;++i){
        middle_point(indxs[i*3+0],indxs[i*3+1],&max_indx);
        middle_point(indxs[i*3+0],indxs[i*3+2],&max_indx);
        middle_point(indxs[i*3+1],indxs[i*3+2],&max_indx);
        //
        cube.indices.push_back(indxs[i*3+0]);
        cube.indices.push_back(max_indx-3);
        cube.indices.push_back(max_indx-2);
             //
             cube.indices.push_back(max_indx-3);
             cube.indices.push_back(indxs[i*3+1]);
             cube.indices.push_back(max_indx-1);
                 //
                 cube.indices.push_back(max_indx-1);
                 cube.indices.push_back(max_indx-2);
                 cube.indices.push_back(max_indx-3);
                      //
                      cube.indices.push_back(max_indx-2);
                      cube.indices.push_back(max_indx-1);
                      cube.indices.push_back(indxs[i*3+2]);

    }
    for(int i=0;i<numb_of_crop;++i)
    {
        cube.indices.erase(cube.indices.begin());
        cube.indices.erase(cube.indices.begin());
        cube.indices.erase(cube.indices.begin());
    }
    //
   // cube.vertices=new_vert;
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
   // parent()
    //
    begin= std::chrono::high_resolution_clock::now();

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
    if(depth_state){
    glEnable(GL_DEPTH_TEST);
    }
    else{
        glDisable(GL_DEPTH_TEST);
    }
    if(culling_state){
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);//GL_BACK
    }
    else{
        glDisable(GL_CULL_FACE);
    }
    if(figure_fill){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if(figure_line){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glShadeModel(GL_SMOOTH);
    m_program->bind();
    QMatrix4x4 matrix;
    matrix.perspective(vertical_angle, aspectRatio, 0.1f, 10000.0f);
    std::vector<QMatrix4x4> trate_cont;//translate+rotate=trate
    for(int i=0;i<nox;++i){
        for(int j=0;j<noy;++j){
            for(int k=0;k<noz;++k){
            QMatrix4x4 mat1;
            mat1.translate(-i*4, k*4, -j*4);
            mat1.rotate(rotation);
            mat1.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
            mat1.rotate(m_yRot / 16.0f, 0, 1, 0);
            mat1.rotate(m_zRot / 16.0f, 0, 0, 1);
            trate_cont.push_back(mat1);
        }
        }
    }
    matrix.translate(x_coord, y_coord, z_coord);
    //keyEvent
    keyevent();
    if(color_change!=0){
    for (int i = 0; i < 8; ++i)
    {
        cube.vertices[i * 6 + 3] += color_change;
        cube.vertices[i * 6 + 4] += color_change;
        cube.vertices[i * 6 + 5] += color_change;
    }
    color_change = 0;

    }
    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setUniformValue("radius", 2.0f);
    m_program->setUniformValue("morphForce", prop);
    m_program->setUniformValue("morph_type", morph_type);

    vao->bind();
    arrayBuf.write(0, cube.vertices.data(), cube.vertices.size() * sizeof(float));
    for(auto trate:trate_cont){
        m_program->setUniformValue(m_matrixTrate, trate);
        glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_SHORT, 0);

    }
    vao->release();
    m_program->release();

    ++m_frame;
    //fps
    auto timet = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - begin);
    if(timet.count() > 9.5) {
        begin= std::chrono::high_resolution_clock::now();
        fps = static_cast<float>(m_frame)/timet.count();
        m_frame=0;
        updateFPS();
    }
}
void OpenGLWidget::updateFPS(){
    emit showFPS();
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
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = 1;
            cube.vertices[i * 6 + 4] = 0;
            cube.vertices[i * 6 + 5] = 0;
        }
    }
    if (pressed_button[25])//2 is green
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = 0;
            cube.vertices[i * 6 + 4] = 1;
            cube.vertices[i * 6 + 5] = 0;
        }
    }
    if (pressed_button[26])//3 is blue
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = 0;
            cube.vertices[i * 6 + 4] = 0;
            cube.vertices[i * 6 + 5] = 1;
        }
    }
    if (pressed_button[27])//4 is aqua
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = 0;
            cube.vertices[i * 6 + 4] = 1;
            cube.vertices[i * 6 + 5] = 1;
        }
    }
    if (pressed_button[28])//5 is pink
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = 1;
            cube.vertices[i * 6 + 4] = 0;
            cube.vertices[i * 6 + 5] = 1;
        }
    }
    if (pressed_button[29])//6 is yellow
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = 1;
            cube.vertices[i * 6 + 4] = 1;
            cube.vertices[i * 6 + 5] = 0;
        }
    }
    if (pressed_button[30])//7 is black and white
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = (float)i / 8;
            cube.vertices[i * 6 + 4] = (float)i / 8;
            cube.vertices[i * 6 + 5] = (float)i / 8;
        }
    }
    if (pressed_button[31])//8 is inverse black and white
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
        {
            cube.vertices[i * 6 + 3] = 1.0 - (float)i / 8;
            cube.vertices[i * 6 + 4] = 1.0 - (float)i / 8;
            cube.vertices[i * 6 + 5] = 1.0 - (float)i / 8;
        }
    }
    if (pressed_button[32])//9 is color inversion
    {
        for (int i = 0; i < cube.vertices.size()/6; ++i)
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
        for (int i = 0; i < 48/6; ++i)
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
        for(int i=48/6;i<cube.vertices.size()/6;++i){
            cube.vertices[(i+0) * 6 + 3]=abs(cube.vertices[(i-5)*6+3]+cube.vertices[(i-6)*6+3])/2;
            cube.vertices[(i+0) * 6 + 4]=abs(cube.vertices[(i-5)*6+4]+cube.vertices[(i-6)*6+4])/2;
            cube.vertices[(i+0) * 6 + 5]=abs(cube.vertices[(i-5)*6+5]+cube.vertices[(i-6)*6+5])/2;
        }
    }
}
