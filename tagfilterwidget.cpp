#include "tagfilterwidget.h"
#include <QDebug>

TagFilterWidget::TagFilterWidget(QWidget* parent) : QWidget(parent) {
    initUI();
}

void TagFilterWidget::initUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // 减少边距
    
    // 设置列表部件的样式
    tagList = new QListWidget(this);
    tagList->setSelectionMode(QAbstractItemView::MultiSelection);
    tagList->setFrameShape(QFrame::NoFrame);  // 移除边框
    tagList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏水平滚动条
    tagList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // 需要时显示垂直滚动条
    
    // 可以添加一个搜索框来过滤标签
    QLineEdit* searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("搜索标签...");
    layout->addWidget(searchBox);
    
    layout->addWidget(tagList);
    
    // 创建按钮布局
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    // 清除选择按钮
    QPushButton* clearButton = new QPushButton("清除选择", this);
    buttonLayout->addWidget(clearButton);
    
    // 确定按钮
    QPushButton* confirmButton = new QPushButton("确定", this);
    confirmButton->setDefault(true);  // 设置为默认按钮
    buttonLayout->addWidget(confirmButton);
    
    // 添加按钮布局
    layout->addLayout(buttonLayout);
    
    // 连接信号和槽
    connect(clearButton, &QPushButton::clicked, this, &TagFilterWidget::clearSelection);
    connect(confirmButton, &QPushButton::clicked, this, &TagFilterWidget::onConfirmClicked);
    connect(searchBox, &QLineEdit::textChanged, this, [this](const QString& text) {
        // 实现标签过滤功能
        for(int i = 0; i < tagList->count(); ++i) {
            QListWidgetItem* item = tagList->item(i);
            item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
        }
    });
    
    setLayout(layout);
}

void TagFilterWidget::updateTags(const QStringList& tags) {
    // 保存当前选中的标签
    QStringList selectedTags = getSelectedTags();
    
    tagList->clear();
    QSet<QString> uniqueTags;
    
    // 分割标签并添加到列表
    for (const QString& tagGroup : tags) {
        QStringList individualTags = tagGroup.split(',', Qt::SkipEmptyParts);
        for (QString tag : individualTags) {
            tag = tag.trimmed();
            if (!tag.isEmpty() && !uniqueTags.contains(tag)) {
                uniqueTags.insert(tag);
                QListWidgetItem* item = new QListWidgetItem(tag, tagList);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                item->setCheckState(selectedTags.contains(tag) ? Qt::Checked : Qt::Unchecked);
            }
        }
    }
}

QStringList TagFilterWidget::getSelectedTags() const {
    QStringList selectedTags;
    for (int i = 0; i < tagList->count(); ++i) {
        QListWidgetItem* item = tagList->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedTags << item->text();
        }
    }
    return selectedTags;
}

void TagFilterWidget::clearSelection() {
    for (int i = 0; i < tagList->count(); ++i) {
        tagList->item(i)->setCheckState(Qt::Unchecked);
    }
}

void TagFilterWidget::onConfirmClicked() {
    QStringList selectedTags;
    for (int i = 0; i < tagList->count(); ++i) {
        QListWidgetItem* item = tagList->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedTags << item->text();
        }
    }
    qDebug() << "Confirmed selected tags:" << selectedTags;
    emit tagsSelected(selectedTags);
} 