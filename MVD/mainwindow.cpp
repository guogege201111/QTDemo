#include "mainwindow.h"
#include "mctable.h"
#include "ui_mainwindow.h"
#include <QItemSelectionModel>
#include <QVariant>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , mListModel(new QStringListModel(this))
  , mtable(new MCTable(this))
{
    ui->setupUi(this);

    ui->listView->setModel(mListModel);

    ui->tableView->setModel(mtable);
    // 设置选择行为为选择整行
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);    // 扩展选择（Ctrl+点击多选）

    // 设置可多选
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(ui->listView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(slotSelectModelChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deleteSelectedItem(QAbstractItemView *view)
{
    // 1. 获取选择模型
    QItemSelectionModel *selectModel = view->selectionModel();
    // 2. 获取所有选中的索引
    QModelIndexList selectedRows = selectModel->selectedRows();

     qDebug() << "selectedIndexes :" << selectedRows.size();
    // 3. 如果没有选中项，直接返回
    if (selectedRows.isEmpty())
        return;

    // 4. 获取数据模型
    QAbstractItemModel *model = view->model();

    // 5. 提取行号并排序（从大到小）
    QList<int> rowsToDelete;
    for (const QModelIndex &index : selectedRows) {
        rowsToDelete.append(index.row());
        qDebug() << "selectedIndexes :" << index;
    }

    // 排序并从大到小删除
    std::sort(rowsToDelete.begin(), rowsToDelete.end(), std::greater<int>());
    rowsToDelete.erase(std::unique(rowsToDelete.begin(), rowsToDelete.end()), rowsToDelete.end());

    // 6. 删除行
    for (int row : rowsToDelete) {
        model->removeRow(row);  // 不传递parent参数，使用默认值
    }

    // 6. 清除选择
    selectModel->clearSelection();
}

void MainWindow::on_pbtnAddList_clicked()
{
    QStringList list=mListModel->stringList();

    list  << QString("%1_66666").arg(mListModelCnts);

    mListModel->setStringList(list);

    mListModelCnts++;
}

void MainWindow::on_pbtnDeleteList_clicked()
{
    deleteSelectedItem(ui->listView);
    ui->textBrowser->clear();

}

void MainWindow::on_pbtnAddTable_clicked()
{
    QVector<QVariant> rowData;
    rowData << counter
            << QString("col2 %1").arg(counter)
            << QString("col3 %1").arg(counter)
            << QString("col4 %1").arg(counter);
    mtable->addRow(rowData);
    counter++;

}

void MainWindow::on_pbtnDeleteTable_clicked()
{
//    QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedRows();
//    if (selectedIndexes.isEmpty()) {
//        return;
//    }

//    for(int i = selectedIndexes.count()-1;i>=0;i--){
//        QModelIndex index = selectedIndexes.at(i);
//        mtable->removeRow(index.row());
//    }

    // 获取选中的行号
//    QList<int> selectedRows;
//    for (const QModelIndex &index : selectedIndexes) {
//        selectedRows.append(index.row());
//    }
//    mtable->removeRows(selectedRows);

    deleteSelectedItem(ui->tableView);
}

void MainWindow::slotSelectModelChanged()
{
    ui->textBrowser->clear();

    QItemSelectionModel *selectItem = ui->listView->selectionModel();

    QModelIndexList selectItemList = selectItem->selectedIndexes();

    for(int i =0;i<selectItemList.size();i++)
    {
        QModelIndex Index = selectItemList.at(i);

        if (Index.isValid())
        {
            QVariant data0 = mListModel->data(Index);
            QString temp = data0.toString();
            ui->textBrowser->append(temp);
        }
    }
}
