#ifndef PROMPTLISTVIEW_H
#define PROMPTLISTVIEW_H

#include <QListView>
#include <QStringListModel>
#include <QWidget>

class PromptListView : public QWidget {
    Q_OBJECT

public:
    explicit PromptListView(QWidget* parent = nullptr);

    void setData(const QStringList& data);

signals:
    void itemClicked(const QString& itemText);

private:
    QListView* listView;
    QStringListModel* model;
};

#endif // PROMPTLISTVIEW_H
