#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::bubbleSort(int *srcArr, int size)
{
    bool swapped;

    for(int i = 0;i < size - 1;i++)
    {
        swapped = false;
        for(int j = 0;j<size -1 -i;j++ )
        {
            if (srcArr[j] > srcArr[j + 1])
            {
                // 交换相邻元素
                int temp = srcArr[j];
                srcArr[j] = srcArr[j + 1];
                srcArr[j + 1] = temp;
                swapped = true;
            }
        }

        // 如果这一轮没有发生交换，说明数组已经有序，提前结束
        if (!swapped)
            break;

    }
}

void MainWindow::selectSort(int *srcArr, int size)
{
    for(int i = 0; i < size - 1; i++)
    {
        int minIndex = i;
        // 找到未排序部分中最小值
        for(int j = i + 1; j < size; j++)
        {
            if(srcArr[j] < srcArr[minIndex])
               minIndex = j;
        }
        // 如果未排序最小值下标和已排序最小值不一样，则交换两者位置
        if(minIndex != i)
        {
            int temp;
            temp = srcArr[minIndex];
            srcArr[minIndex] = srcArr[i];
            srcArr[i] = temp;
        }
    }
}
