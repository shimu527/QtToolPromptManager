#ifndef PROMPTTOOLBAR_H
#define PROMPTTOOLBAR_H

#include <QToolBar>
#include <QLineEdit>
#include <QComboBox>

class PromptToolBar : public QToolBar {
    Q_OBJECT

public:
    explicit PromptToolBar(QWidget* parent = nullptr);

    QLineEdit* searchBar() const;
    QComboBox* sortCombo() const;

signals:
    void addPromptTriggered();
    void editPromptTriggered();
    void deletePromptTriggered();
    void loadDataTriggered();
    void importPromptsTriggered();
    void importPromptsFromFileTriggered();
    void exportPromptsTriggered();
    void advancedSearchTriggered();
    void searchTextChanged(const QString& text);
    void sortIndexChanged(int index);

private:
    QLineEdit* m_searchBar;
    QComboBox* m_sortCombo;
};

#endif // PROMPTTOOLBAR_H
