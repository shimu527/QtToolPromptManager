#include "promptmodel.h"

// PromptModel::PromptModel(QObject* parent)
//     : QAbstractTableModel(parent), headers({"ID", "Content", "Title", "Description","Tags","Version", ",Cover Image","Created At", "Updated At"}) {}

PromptModel::PromptModel(const QList<QList<QVariant>>& data, QObject* parent)
    : QAbstractTableModel(parent), _data(data),headers({"ID", "Content", "Title", "Description","Tags","Version", "Cover Image","Created At", "Updated At"}) {}

int PromptModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _data.size()+1;
}

int PromptModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return headers.size();
}


QVariant PromptModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    if (isNewPageRow(row)) {
        if (role == Qt::DisplayRole && col == 0) {
            return "+ New Row";
        } else if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        }
        return QVariant();
    }

    if (row >= _data.size())
        return QVariant();

    const QList<QVariant>& rowData = _data.at(row);

    if (role == Qt::DisplayRole) {
        switch (col) {
        case 0: return rowData.at(0); // ID
        case 1: return rowData.at(1); // Content
        case 2: return rowData.at(2); // Title
        case 3: return rowData.at(3); // Description
        case 4: return rowData.at(4); // Tags
        case 5: return rowData.at(5); // Version
        case 6: return rowData.at(6); // Cover Image
        case 7: return rowData.at(7).toDateTime().toString(); // Created At
        case 8: return rowData.at(8).toDateTime().toString(); // Updated At
        }
    }

    return QVariant();
}
QVariant PromptModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section < headers.size()) {
            return headers.at(section);
        }
    }

    return QVariant();
}

void PromptModel::addNewRow() {
    beginInsertRows(QModelIndex(), _data.size(), _data.size());
    _data.append({QVariant(), QVariant(), QVariant(), QVariant(), QVariant(), QVariant(), QVariant(), QDateTime::currentDateTime(), QDateTime::currentDateTime()});
    endInsertRows();
}

bool PromptModel::isNewPageRow(int row) const {
    return row == _data.size();
}
