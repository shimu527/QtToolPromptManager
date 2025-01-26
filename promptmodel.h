#ifndef PROMPTMODEL_H
#define PROMPTMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QString>
#include <QList>
#include <QDateTime>
#include <QDebug>

class PromptModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    // PromptModel(QObject* parent = nullptr);
    PromptModel(const QList<QList<QVariant>>& data, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;


    //添加新行
    void addNewRow();
    // 判断是否是最后一行
    bool isNewPageRow(int row) const;

    // 重命名为 setModelData 以避免与基类的 setData 冲突
    void setModelData(const QList<QList<QVariant>>& newData);

private:
    QList<QList<QVariant>> _data;
    QList<QString> headers;

};

#endif // PROMPTMODEL_H
