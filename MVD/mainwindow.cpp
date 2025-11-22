#include "mainwindow.h"
#include "mctable.h"
#include "ui_mainwindow.h"
#include <QItemSelectionModel>
#include <QVariant>

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
    QModelIndexList selectList = selectModel->selectedIndexes();

    // 3. 如果没有选中项，直接返回
    if (selectList.isEmpty())
        return;

    // 4. 获取数据模型
    QAbstractItemModel *model = view->model();
    // 5. 删除选中项（注意：要从后往前删除，避免索引变化）
    for(int i = selectList.count()-1;i>= 0;i--)
    {
        QModelIndex index = selectList.at(i);
        if(index.isValid())
            model->removeRow(index.row(),index.parent());
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
