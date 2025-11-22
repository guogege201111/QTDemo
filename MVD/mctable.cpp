#include "mctable.h"

MCTable::MCTable(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_headers<< "col1"<<"col2"<<"col3"<<"col4";
}

QVariant MCTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section < m_headers.size())
            {
                return m_headers[section];
            }
        }
        else
        {
            return section + 1;  // 垂直表头显示行号
        }
    }
}

int MCTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

     return m_data.size();

    // FIXME: Implement me!
}

int MCTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

     return m_headers.size();
    // FIXME: Implement me!
}

QVariant MCTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        int row = index.row();
        int col = index.column();

        if (row < m_data.size() && col < m_data[row].size())
        {
            return m_data[row][col];
        }
    }

    return QVariant();
}

bool MCTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    int row = index.row();
    int col = index.column();

    // 确保行存在
    if (row >= m_data.size())
        return false;

    // 确保列存在，如果不存在则扩展
    while (col >= m_data[row].size())
    {
        m_data[row].append(QVariant());
    }

    m_data[row][col] = value;

    // 发出数据改变信号
    emit dataChanged(index, index, {role});

    return true;
}

Qt::ItemFlags MCTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
           return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void MCTable::addRow(const QVector<QVariant> &rowData)
{
     insertRow(m_data.size(), rowData);
}

void MCTable::insertRow(int row, const QVector<QVariant> &rowData)
{
    if (row < 0 || row > m_data.size())
            return;

    beginInsertRows(QModelIndex(), row, row);
    m_data.insert(row, rowData);
    endInsertRows();
}

void MCTable::removeRow(int row)
{
    if (row < 0 || row >= m_data.size())
            return;

    beginRemoveRows(QModelIndex(), row, row);
    m_data.removeAt(row);
    endRemoveRows();
}

void MCTable::clearAll()
{
    if (m_data.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
    m_data.clear();
    endRemoveRows();
}

void MCTable::setData(const QVector<QVector<QVariant> > &newData)
{
    // 清除现有数据
    if (!m_data.isEmpty())
    {
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }

    // 添加新数据
    if (!newData.isEmpty())
    {
        beginInsertRows(QModelIndex(), 0, newData.size() - 1);
        m_data = newData;
        endInsertRows();
    }
}
