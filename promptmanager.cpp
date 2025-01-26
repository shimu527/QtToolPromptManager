#include "promptmanager.h"
#include "backupmanager.h"
#include "tagfilterwidget.h"

PromptManager::PromptManager(PromptDB* db, QWidget* parent)
    : QMainWindow(parent), db(db), model(nullptr),listView(nullptr), galleryView(nullptr){
    
    // 创建备份管理器
    backupManager = new BackupManager(db->getDatabasePath(), this);
    connect(backupManager, &BackupManager::backupCreated, this, &PromptManager::onBackupCreated);
    connect(backupManager, &BackupManager::backupFailed, this, &PromptManager::onBackupFailed);
    connect(backupManager, &BackupManager::backupRestored, this, &PromptManager::onBackupRestored);
    
    // 启动自动备份（每30分钟）
    backupManager->startAutoBackup(30);
    
    initUI();
    loadData();
}

void PromptManager::initUI() {
    setWindowTitle("Prompt Manager");
    setGeometry(100, 100, 1024, 768);  // 调整窗口大小

    // 创建工具栏
    toolBar = new PromptToolBar(this);
    addToolBar(toolBar);

    // 连接工具栏信号到槽函数
    connect(toolBar, &PromptToolBar::addPromptTriggered, this, &PromptManager::addPromptDialog);
    connect(toolBar, &PromptToolBar::editPromptTriggered, this, &PromptManager::editPromptDialog);
    connect(toolBar, &PromptToolBar::deletePromptTriggered, this, &PromptManager::deletePrompt);
    connect(toolBar, &PromptToolBar::loadDataTriggered, this, &PromptManager::loadData);
    connect(toolBar, &PromptToolBar::importPromptsTriggered, this, &PromptManager::importPromptsDialog);
    connect(toolBar, &PromptToolBar::importPromptsFromFileTriggered, this, &PromptManager::importPromptsFromFile);
    connect(toolBar, &PromptToolBar::exportPromptsTriggered, this, &PromptManager::exportPrompts);
    connect(toolBar, &PromptToolBar::advancedSearchTriggered, this, &PromptManager::advancedSearchDialog);
    connect(toolBar, &PromptToolBar::searchTextChanged, this, &PromptManager::searchPrompts);
    connect(toolBar, &PromptToolBar::sortIndexChanged, this, &PromptManager::loadData);

    // 创建主窗口的中心部件
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // 创建左侧标签筛选部件
    tagFilter = new TagFilterWidget(this);
    tagFilter->setMaximumWidth(200);  // 设置最大宽度
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(new QLabel("标签筛选", this));
    leftLayout->addWidget(tagFilter);
    leftLayout->addStretch();  // 添加弹性空间

    // 创建右侧视图区域
    QTabWidget* viewsTabWidget = new QTabWidget(this);

    // 创建表格视图
    tableView = new QTableView(this);
    tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    viewsTabWidget->addTab(tableView, "表格视图");

    //暂时不删除这个connect连接注释
    // connect(tableView, &QTableView::doubleClicked, this, &PromptManager::editPromptDialog);

    // 创建列表视图
    listView = new PromptListView(this);
    viewsTabWidget->addTab(listView, "列表视图");

    // 创建画廊视图
    galleryView = new PromptGalleryView(this);
    viewsTabWidget->addTab(galleryView, "画廊视图");

    // 将左侧标签筛选和右侧视图添加到主布局
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(viewsTabWidget, 1);  // 1 表示拉伸因子

    // 设置主窗口的中心部件
    setCentralWidget(centralWidget);

    // 状态栏
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

    // 连接表格视图的双击信号
    connect(tableView, &QTableView::doubleClicked, this, &PromptManager::tableViewDoubleClicked);

    // 连接标签选择信号
    connect(tagFilter, &TagFilterWidget::tagsSelected, 
            this, &PromptManager::filterByTags,
            Qt::QueuedConnection);// 使用队列连接以避免可能的死锁
}


void PromptManager::loadData() {
    try {
        QList<QList<QVariant>> prompts = db->getPrompts();
        QString sortColumn = toolBar->sortCombo()->currentText();

        // 根据选择的排序字段进行排序
        if (sortColumn == "ID") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[0].toInt() < b[0].toInt();
            });
        } else if (sortColumn == "Content") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[1].toString() < b[1].toString();
            });
        } else if (sortColumn == "Title") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[2].toString() < b[2].toString();
            });
        } else if (sortColumn == "Description") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[3].toString() < b[3].toString();
            });
        } else if (sortColumn == "Tags") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[4].toString() < b[4].toString();
            });
        } else if (sortColumn == "Version") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[5].toString() < b[5].toString();
            });
        } else if (sortColumn == "Created At") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[7].toDateTime() < b[7].toDateTime();
            });
        } else if (sortColumn == "Updated At") {
            std::sort(prompts.begin(), prompts.end(), [](const QList<QVariant>& a, const QList<QVariant>& b) {
                return a[8].toDateTime() < b[8].toDateTime();
            });
        }

        // 创建模型并设置到表格视图
        model = new PromptModel(prompts, this);
        tableView->setModel(model);

        // 更新列表视图
        QStringList titles;
        for (const auto& row : prompts) {
            if (row.size() > 2) {
                titles.append(row[2].toString()); // 使用标题作为列表项
            }
        }
        listView->setData(titles);

        // 更新画廊视图
        galleryView->setData(prompts);

        updateTagList();

        statusBar->showMessage(QString("Loaded %1 prompts").arg(prompts.size()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load data: %1").arg(e.what()));
    }
}

void PromptManager::addPromptDialog() {
    PromptDialog dialog(this);
    dialog.setWindowTitle("Add Prompt");

    if (dialog.exec() == QDialog::Accepted) {
        addPrompt(
            dialog.getContent(),
            dialog.getTitle(),
            dialog.getDescription(),
            dialog.getTags(),
            dialog.getVersion(),
            dialog.getCoverImagePath(),
            &dialog
            );
    }
}

void PromptManager::addPrompt(const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QString& coverImagePath, QDialog* dialog) {
    if (content.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Prompt content cannot be empty.");
        return;
    }

    QByteArray coverImageData;
    if (!coverImagePath.isEmpty()) {
        QFile file(coverImagePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "Failed to open cover image file.");
            return;
        }
        coverImageData = file.readAll();
    }

    try {
        db->addPrompt(content, title, description, tags, version, coverImageData);
        loadData();
        dialog->accept();
        statusBar->showMessage("Prompt added");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to add prompt: %1").arg(e.what()));
    }
}








void PromptManager::editPromptDialog() {
    QModelIndex currentIndex = tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Warning", "Please select a prompt to edit.");
        return;
    }

    int promptId = model->data(model->index(currentIndex.row(), 0), Qt::DisplayRole).toInt();
    QString content = model->data(model->index(currentIndex.row(), 1), Qt::DisplayRole).toString();
    QString title = model->data(model->index(currentIndex.row(), 2), Qt::DisplayRole).toString();
    QString description = model->data(model->index(currentIndex.row(), 3), Qt::DisplayRole).toString();
    QString tags = model->data(model->index(currentIndex.row(), 4), Qt::DisplayRole).toString();
    QString version = model->data(model->index(currentIndex.row(), 5), Qt::DisplayRole).toString();

    PromptDialog dialog(this);
    dialog.setWindowTitle("Edit Prompt");
    dialog.setTitle(title);
    dialog.setContent(content);
    dialog.setDescription(description);
    dialog.setTags(tags);
    dialog.setVersion(version);

    if (dialog.exec() == QDialog::Accepted) {
        editPrompt(
            promptId,
            dialog.getContent(),
            dialog.getTitle(),
            dialog.getDescription(),
            dialog.getTags(),
            dialog.getVersion(),
            dialog.getCoverImagePath(),
            &dialog
            );
    }
}

void PromptManager::editPrompt(int promptId, const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QString& coverImagePath, QDialog* dialog) {
    if (content.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Prompt content cannot be empty.");
        return;
    }

    QByteArray coverImageData;
    if (!coverImagePath.isEmpty()) {
        QFile file(coverImagePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "Failed to open cover image file.");
            return;
        }
        coverImageData = file.readAll();
    }

    try {
        db->updatePrompt(promptId, content, title, description, tags, version, coverImageData);
        loadData();
        dialog->accept();
        statusBar->showMessage("Prompt updated");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to update prompt: %1").arg(e.what()));
    }
}


void PromptManager::deletePrompt() {
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select prompts to delete.");
        return;
    }

    QList<int> promptIds;
    for (const QModelIndex& index : selectedIndexes) {
        int promptId = model->data(model->index(index.row(), 0), Qt::DisplayRole).toInt();
        promptIds.append(promptId);
    }

    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Delete", "Are you sure you want to delete the selected prompts?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (confirm == QMessageBox::Yes) {
        try {
            for (int promptId : promptIds) {
                db->deletePrompt(promptId);
            }
            loadData();
            statusBar->showMessage("Prompts deleted");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("Failed to delete prompts: %1").arg(e.what()));
        }
    }
}

void PromptManager::searchPrompts(const QString& keyword) {
    try {
        QList<QList<QVariant>> prompts = db->getPrompts(keyword);
        model = new PromptModel(prompts, this);
        tableView->setModel(model);
        statusBar->showMessage(QString("Found %1 prompts matching the search criteria").arg(prompts.size()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to search prompts: %1").arg(e.what()));
    }
}

void PromptManager::importPromptsDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Import Prompts");
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QTextEdit* textEdit = new QTextEdit(&dialog);
    layout->addWidget(textEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* importButton = new QPushButton("Import", &dialog);
    connect(importButton, &QPushButton::clicked, [&]() {
        importPrompts(textEdit->toPlainText(), &dialog);
    });
    buttonLayout->addWidget(importButton);

    QPushButton* cancelButton = new QPushButton("Cancel", &dialog);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);
    dialog.setLayout(layout);
    dialog.exec();
}


void PromptManager::importPrompts(const QString& text, QDialog* dialog) {
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No prompts to import.");
        return;
    }

    try {
        QStringList prompts = text.split('\n', Qt::SkipEmptyParts);
        for (const QString& prompt : prompts) {
            // 假设每行文本的格式为："标题|内容|描述|标签|版本"
            QStringList parts = prompt.split('|');
            if (parts.size() >= 5) {
                QString title = parts[0].trimmed();
                QString content = parts[1].trimmed();
                QString description = parts[2].trimmed();
                QString tags = parts[3].trimmed();
                QString version = parts[4].trimmed();

                // 封面图片可以为空
                QByteArray coverImage;

                db->addPrompt(content, title, description, tags, version, coverImage);
            } else {
                QMessageBox::warning(this, "Warning", QString("Invalid prompt format: %1").arg(prompt));
            }
        }
        loadData();
        dialog->accept();
        statusBar->showMessage(QString("Imported %1 prompts").arg(prompts.size()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to import prompts: %1").arg(e.what()));
    }
}


void PromptManager::importPromptsFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Import Prompts from File", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) {
        return;
    }

    try {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Failed to open file.");
            return;
        }

        QTextStream in(&file);
        QStringList prompts;
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                prompts.append(line);
            }
        }

        for (const QString& prompt : prompts) {
            // 假设每行文本的格式为："标题|内容|描述|标签|版本"
            QStringList parts = prompt.split('|');
            if (parts.size() >= 5) {
                QString title = parts[0].trimmed();
                QString content = parts[1].trimmed();
                QString description = parts[2].trimmed();
                QString tags = parts[3].trimmed();
                QString version = parts[4].trimmed();

                // 封面图片可以为空
                QByteArray coverImage;

                db->addPrompt(content, title, description, tags, version, coverImage);
            } else {
                QMessageBox::warning(this, "Warning", QString("Invalid prompt format: %1").arg(prompt));
            }
        }
        loadData();
        statusBar->showMessage(QString("Imported %1 prompts from file").arg(prompts.size()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to import prompts from file: %1").arg(e.what()));
    }
}
void PromptManager::exportPrompts() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export Prompts", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    try {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Failed to open file for writing.");
            return;
        }

        QTextStream out(&file);
        out << "ID,Content,Created At,Updated At\n";

        QList<QList<QVariant>> prompts = db->getPrompts();
        for (const QList<QVariant>& prompt : prompts) {
            out << prompt[0].toString() << ","
                << prompt[1].toString() << ","
                << prompt[2].toDateTime().toString(Qt::ISODate) << ","
                << prompt[3].toDateTime().toString(Qt::ISODate) << "\n";
        }

        statusBar->showMessage(QString("Exported %1 prompts to %2").arg(prompts.size()).arg(fileName));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to export prompts: %1").arg(e.what()));
    }
}

void PromptManager::advancedSearchDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Advanced Search");
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* keywordLabel = new QLabel("Keyword:", &dialog);
    layout->addWidget(keywordLabel);

    QLineEdit* keywordEdit = new QLineEdit(&dialog);
    layout->addWidget(keywordEdit);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* searchButton = new QPushButton("Search", &dialog);
    connect(searchButton, &QPushButton::clicked, [&]() {
        searchPrompts(keywordEdit->text());
        dialog.accept();
    });
    buttonLayout->addWidget(searchButton);

    QPushButton* cancelButton = new QPushButton("Cancel", &dialog);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);

    layout->addLayout(buttonLayout);
    dialog.setLayout(layout);
    dialog.exec();
}

void PromptManager::showPromptHistory(int promptId) {
    QList<QList<QVariant>> history = db->getPromptHistory(promptId);

    QDialog dialog(this);
    dialog.setWindowTitle("Prompt History");
    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QTableWidget* historyTable = new QTableWidget(&dialog);
    historyTable->setColumnCount(3);
    historyTable->setHorizontalHeaderLabels({"ID", "Content", "Updated At"});
    historyTable->setRowCount(history.size());

    for (int i = 0; i < history.size(); ++i) {
        const QList<QVariant>& record = history[i];
        historyTable->setItem(i, 0, new QTableWidgetItem(record[0].toString()));
        historyTable->setItem(i, 1, new QTableWidgetItem(record[2].toString()));
        historyTable->setItem(i, 2, new QTableWidgetItem(record[3].toDateTime().toString(Qt::ISODate)));
    }

    layout->addWidget(historyTable);
    dialog.setLayout(layout);
    dialog.exec();
}

void PromptManager::copyPrompt(const QString& text) {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(text);
    statusBar->showMessage("Prompt copied to clipboard");
}

void PromptManager::tableViewDoubleClicked(const QModelIndex& index) {
    if (model->isNewPageRow(index.row())) {
        // 如果点击的是最后一行（"+ New Row"），则调用 addPromptDialog
        addPromptDialog();
    } else {
        // 否则，调用 editPromptDialog
        editPromptDialog();
    }
}

void PromptManager::onBackupCreated(const QString& path) {
    statusBar->showMessage(QString("Backup created: %1").arg(path), 3000);
}

void PromptManager::onBackupFailed(const QString& error) {
    QMessageBox::warning(this, "Backup Failed", error);
}

void PromptManager::onBackupRestored() {
    statusBar->showMessage("Backup restored successfully", 3000);
    loadData(); // 重新加载数据
}

void PromptManager::updateTagList() {
    QStringList allTags = db->getAllTags();
    tagFilter->updateTags(allTags);
}

void PromptManager::filterByTags(const QStringList& tags) {
    qDebug() << "Filtering by tags:" << tags;
    
    if (tags.isEmpty()) {
        loadData();
        return;
    }
    
    QList<QList<QVariant>> filteredData = db->getPromptsByTags(tags);
    qDebug() << "Filtered data count:" << filteredData.size();
    
    // 更新表格视图
    if (model) {
        model->setModelData(filteredData);
    }
    
    // 更新列表视图
    QStringList titles;
    for (const auto& row : filteredData) {
        if (row.size() > 2) {
            titles.append(row[2].toString());
        }
    }
    if (listView) {
        listView->setData(titles);
    }
    
    // 更新画廊视图
    if (galleryView) {
        galleryView->setData(filteredData);
    }
    
    statusBar->showMessage(QString("Found %1 prompts with selected tags").arg(filteredData.size()));
}

