#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractItemView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MCTable;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void deleteSelectedItem(QAbstractItemView *view);

private slots:
    void on_pbtnAddList_clicked();

    void on_pbtnDeleteList_clicked();

    void on_pbtnAddTable_clicked();

    void on_pbtnDeleteTable_clicked();

    void slotSelectModelChanged();

private:
    Ui::MainWindow *ui;

    QStringListModel* mListModel;

    MCTable*mtable;

    int mListModelCnts = 0;

    int counter = 0;
};

#endif // MAINWINDOW_H
