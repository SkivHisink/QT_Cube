#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_XSlider_valueChanged(int value);

    void on_XSlider_sliderMoved(int position);

    void sliderSet();
    void SphereMorph();
    void StrangeMorph();
    void NoMorph();
    void FillMode();
private:
    Ui::MainWindow *ui;
    int prev_x_pos=0;
    int prev_y_pos=0;
    int prev_z_pos=0;
    bool sphere_morph=false;
    bool strange_morph=false;
};
#endif // MAINWINDOW_H
