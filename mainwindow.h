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

    void sliderSet();
    void SphereMorph();
    void StrangeMorph();
    void NoMorph();
    void FillMode();
    void showFPS();
    void changeNumberOfObj();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
