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
private:
    void bubbleSort(int*srcArr,int size);
private:
    Ui::MainWindow *ui;

    // 原始数据
    int srcArr[100];
};
#endif // MAINWINDOW_H
