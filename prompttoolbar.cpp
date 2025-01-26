#include "prompttoolbar.h"
#include <QAction>
#include <QIcon>
#include <QKeySequence>

PromptToolBar::PromptToolBar(QWidget* parent) : QToolBar(parent) {
    // Add Action
    QAction* addAction = new QAction(QIcon(":/icons/add.svg"), "Add", this);
    addAction->setShortcut(QKeySequence("Ctrl+N"));
    addAction->setToolTip("Add a new prompt");
    connect(addAction, &QAction::triggered, this, &PromptToolBar::addPromptTriggered);
    this->addAction(addAction);

    // Edit Action
    QAction* editAction = new QAction(QIcon(":/icons/edit.svg"), "Edit", this);
    editAction->setShortcut(QKeySequence("Ctrl+E"));
    editAction->setToolTip("Edit the selected prompt");
    connect(editAction, &QAction::triggered, this, &PromptToolBar::editPromptTriggered);
    this->addAction(editAction);

    // Delete Action
    QAction* deleteAction = new QAction(QIcon(":/icons/delete.svg"), "Delete", this);
    deleteAction->setShortcut(QKeySequence("Ctrl+D"));
    deleteAction->setToolTip("Delete the selected prompts");
    connect(deleteAction, &QAction::triggered, this, &PromptToolBar::deletePromptTriggered);
    this->addAction(deleteAction);

    // Load Action
    QAction* loadAction = new QAction(QIcon(":/icons/load.svg"), "Load", this);
    loadAction->setShortcut(QKeySequence("Ctrl+L"));
    loadAction->setToolTip("Reload prompts from the database");
    connect(loadAction, &QAction::triggered, this, &PromptToolBar::loadDataTriggered);
    this->addAction(loadAction);

    // Import Action
    QAction* importAction = new QAction(QIcon(":/icons/import.svg"), "Import", this);
    importAction->setShortcut(QKeySequence("Ctrl+I"));
    importAction->setToolTip("Import prompts from text");
    connect(importAction, &QAction::triggered, this, &PromptToolBar::importPromptsTriggered);
    this->addAction(importAction);

    // Import from File Action
    QAction* importFileAction = new QAction(QIcon(":/icons/import_file.svg"), "Import from File", this);
    importFileAction->setToolTip("Import prompts from a text file");
    connect(importFileAction, &QAction::triggered, this, &PromptToolBar::importPromptsFromFileTriggered);
    this->addAction(importFileAction);

    // Export Action
    QAction* exportAction = new QAction(QIcon(":/icons/export.svg"), "Export", this);
    exportAction->setShortcut(QKeySequence("Ctrl+S"));
    exportAction->setToolTip("Export prompts to a CSV file");
    connect(exportAction, &QAction::triggered, this, &PromptToolBar::exportPromptsTriggered);
    this->addAction(exportAction);

    // Advanced Search Action
    QAction* advancedSearchAction = new QAction(QIcon(":/icons/search.svg"), "Advanced Search", this);
    advancedSearchAction->setShortcut(QKeySequence("Ctrl+Shift+F"));
    advancedSearchAction->setToolTip("Open advanced search dialog");
    connect(advancedSearchAction, &QAction::triggered, this, &PromptToolBar::advancedSearchTriggered);
    this->addAction(advancedSearchAction);

    // Search Bar
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Search prompts...");
    connect(m_searchBar, &QLineEdit::textChanged, this, &PromptToolBar::searchTextChanged);
    addWidget(m_searchBar);

    // Sort ComboBox
    m_sortCombo = new QComboBox(this);
    m_sortCombo->addItems({"ID", "Content", "Title", "Description", "Tags", "Version", "Cover Image", "Created At", "Updated At"});
    connect(m_sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PromptToolBar::sortIndexChanged);
    m_sortCombo->setToolTip("Sort prompts by selected criteria");
    addWidget(m_sortCombo);
}

QLineEdit* PromptToolBar::searchBar() const {
    return m_searchBar;
}

QComboBox* PromptToolBar::sortCombo() const {
    return m_sortCombo;
}
