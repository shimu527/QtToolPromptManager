#include "promptgalleryview.h"
#include <QVBoxLayout>
#include <QGraphicsPixmapItem>
#include <QPixmap>

PromptGalleryView::PromptGalleryView(QWidget* parent) : QWidget(parent) {
    graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    graphicsView->setScene(scene);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(graphicsView);
    setLayout(layout);
}

void PromptGalleryView::setData(const QList<QList<QVariant>>& data) {
    scene->clear();

    const int itemWidth = 150;
    const int itemHeight = 150;
    const int spacing = 10;
    int x = 0;
    int y = 0;

    for (const auto& row : data) {
        if (row.size() > 6 && !row[6].toByteArray().isEmpty()) {
            QPixmap pixmap;
            pixmap.loadFromData(row[6].toByteArray());
            pixmap = pixmap.scaled(itemWidth, itemHeight, Qt::KeepAspectRatio);

            QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
            item->setPos(x, y);

            x += itemWidth + spacing;
            if (x + itemWidth > graphicsView->width()) {
                x = 0;
                y += itemHeight + spacing;
            }
        }
    }
}
