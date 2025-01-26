#ifndef PROMPTMANAGER_H
#define PROMPTMANAGER_H


#include <QMainWindow>
#include <QAction>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QStatusBar>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QApplication>
#include <QClipboard>
#include <QKeySequence>
#include <QDebug>

#include <QIcon>


#include "promptdb.h"
#include "promptmodel.h"
#include "ListView/promptlistview.h"
#include "GalleryView/promptgalleryview.h"
#include "prompttoolbar.h"
#include "promptdialog.h"

class PromptManager : public QMainWindow {
    Q_OBJECT

public:
    PromptManager(PromptDB* db, QWidget* parent = nullptr);

private slots:
    void loadData();
    void addPromptDialog();
    void addPrompt(const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QString& coverImagePath, QDialog* dialog);

    void editPromptDialog();
    void editPrompt(int promptId, const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QString& coverImagePath, QDialog* dialog);
    void deletePrompt();
    void searchPrompts(const QString& keyword);
    void importPromptsDialog();
    void importPrompts(const QString& text, QDialog* dialog);
    void importPromptsFromFile();
    void exportPrompts();
    void advancedSearchDialog();
    void showPromptHistory(int promptId);
    void copyPrompt(const QString& text);

    void tableViewDoubleClicked(const QModelIndex& index);

private:
    void initUI();

    PromptDB* db;
    QTableView* tableView;
    PromptModel* model;
    QStatusBar* statusBar;

    PromptListView* listView;
    PromptGalleryView* galleryView;
    PromptToolBar* toolBar;

};

#endif // PROMPTMANAGER_H
