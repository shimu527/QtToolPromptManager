#ifndef TAGFILTERWIDGET_H
#define TAGFILTERWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSet>
#include <QLineEdit>
#include <QPushButton>

class TagFilterWidget : public QWidget {
    Q_OBJECT

public:
    explicit TagFilterWidget(QWidget* parent = nullptr);
    void updateTags(const QStringList& tags);
    QStringList getSelectedTags() const;
    void clearSelection();

signals:
    void tagsSelected(const QStringList& tags);

private slots:
    void onConfirmClicked();

private:
    QListWidget* tagList;
    void initUI();
};

#endif // TAGFILTERWIDGET_H 