#ifndef PROMPTGALLERYVIEW_H
#define PROMPTGALLERYVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QList>
#include <QVariant>

class PromptGalleryView : public QWidget {
    Q_OBJECT

public:
    explicit PromptGalleryView(QWidget* parent = nullptr);

    void setData(const QList<QList<QVariant>>& data);

private:
    QGraphicsView* graphicsView;
    QGraphicsScene* scene;
};

#endif // PROMPTGALLERYVIEW_H
