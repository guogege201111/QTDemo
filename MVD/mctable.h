#ifndef MCTABLE_H
#define MCTABLE_H

#include <QAbstractTableModel>

class MCTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MCTable(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // 使模型可编辑
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // 添加行操作的方法
    void addRow(const QVector<QVariant> &rowData);
    void insertRow(int row, const QVector<QVariant> &rowData);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;// 删除单行
    void removeRows(const QList<int> &rows);// 删除多行
    void clearAll();

    // 批量添加数据
    void setData(const QVector<QVector<QVariant>> &newData);

private:
       QVector<QVector<QVariant>> m_data;  // 存储表格数据
       QStringList m_headers;              // 表头
};

#endif // MCTABLE_H
