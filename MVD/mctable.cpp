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

     return QVariant();
}

int MCTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

     return m_data.size();
}

int MCTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

     return m_headers.size();
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

bool MCTable::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;  // 对于平面表格，parent 应该无效

    if (row < 0 || row >= m_data.size())
        return false;

    beginRemoveRows(parent, row, row);
    m_data.removeAt(row);
    endRemoveRows();

    return true;
}

void MCTable::removeRows(const QList<int> &rows)
{
    if (rows.isEmpty())
           return;

    // 将行号从大到小排序，这样删除时不会影响前面的行号
    QList<int> sortedRows = rows;
    std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

    // 删除重复的行号
    sortedRows.erase(std::unique(sortedRows.begin(), sortedRows.end()), sortedRows.end());

    for (int row : sortedRows)
    {
        if (row >= 0 && row < m_data.size())
        {
            beginRemoveRows(QModelIndex(), row, row);
            m_data.removeAt(row);
            endRemoveRows();
        }
    }
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
