#include "promptlistview.h"
#include <QVBoxLayout>

PromptListView::PromptListView(QWidget* parent) : QWidget(parent) {
    listView = new QListView(this);
    model = new QStringListModel(this);

    listView->setModel(model);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(listView);
    setLayout(layout);

    connect(listView, &QListView::clicked, this, [this](const QModelIndex& index) {
        emit itemClicked(model->data(index, Qt::DisplayRole).toString());
    });
}

void PromptListView::setData(const QStringList& data) {
    model->setStringList(data);
}
