#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QElapsedTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void testCPU();
    void testGPU();

private slots:
    void on_pushButtonCPU_clicked();
    void on_pushButtonGPU_clicked();

private:
    Ui::MainWindow *ui;

    quint8* mA;
    quint8* mB;
    quint8* mC;

    QElapsedTimer* testTimer;
};
#endif // MAINWINDOW_H
