#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "UseCudaApi.h"  // 添加这个头文件
#include <QElapsedTimer>
#include <cstring>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mA(new quint8[2048*2048])
    , mB(new quint8[2048*2048])
    , mC(new quint8[2048*2048])
    , testTimer(new QElapsedTimer())
{
    ui->setupUi(this);

    // 初始化测试数据
    for (int i = 0; i < 2048*2048; i++) {
        mA[i] = i % 256;      // 0-255的循环
        mB[i] = (i * 2) % 256; // 0-255的循环
        mC[i] = 0;
    }

    // 获取并显示CUDA设备信息
    char* deviceInfo = getCudaDeviceInfo();
    qDebug() << "CUDA设备信息:" << deviceInfo;
    free(deviceInfo);  // 释放内存
}

MainWindow::~MainWindow()
{
    delete[] mA;
    delete[] mB;
    delete[] mC;
    delete testTimer;
    delete ui;
}

void MainWindow::testGPU()
{
    testTimer->start();

    // 调用CUDA函数
    bool success = runCudaVectorAdd(mA, mB, mC, 2048*2048);

    qint64 elapsed = testTimer->elapsed();

    if (success) {
        ui->label_time->setText(QString::number(elapsed) + " 毫秒");

        // 验证前10个结果
        qDebug() << "GPU计算结果（前10个）:";
        for (int i = 0; i < 10; i++) {
            qDebug() << i << ":" << (int)mA[i] << "+" << (int)mB[i] << "=" << (int)mC[i];
        }
    } else {
        ui->label_time->setText("CUDA计算失败");
    }
}

void MainWindow::testCPU()
{
    testTimer->start();

    // CPU版本的计算
    for (int i = 0; i < 2048*2048; i++) {
        mC[i] = (mA[i] + mB[i]) % 256;  // 同样使用模运算防止溢出
    }

    qint64 elapsed = testTimer->elapsed();
    ui->label_time->setText(QString::number(elapsed) + " 毫秒");

    // 验证前10个结果
    qDebug() << "CPU计算结果（前10个）:";
    for (int i = 0; i < 10; i++) {
        qDebug() << i << ":" << (int)mA[i] << "+" << (int)mB[i] << "=" << (int)mC[i];
    }
}

void MainWindow::on_pushButtonCPU_clicked()
{
    qDebug() << "开始CPU计算...";
    testCPU();
}

void MainWindow::on_pushButtonGPU_clicked()
{
    qDebug() << "开始GPU计算...";
    testGPU();
}
