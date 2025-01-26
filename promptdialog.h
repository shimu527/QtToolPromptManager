#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class PromptDialog : public QDialog {
    Q_OBJECT

public:
    explicit PromptDialog(QWidget* parent = nullptr);

    // 设置和获取字段
    void setTitle(const QString& title);
    QString getTitle() const;

    void setContent(const QString& content);
    QString getContent() const;

    void setDescription(const QString& description);
    QString getDescription() const;

    void setTags(const QString& tags);
    QString getTags() const;

    void setVersion(const QString& version);
    QString getVersion() const;

    void setCoverImagePath(const QString& path);
    QString getCoverImagePath() const;

    // 公共对话框逻辑
    void setupCommonDialog();

private:
    QLineEdit* titleEdit;
    QTextEdit* contentEdit;
    QTextEdit* descriptionEdit;
    QLineEdit* tagsEdit;
    QLineEdit* versionEdit;
    QLineEdit* coverImageEdit;
    QPushButton* coverImageButton;
};

#endif // PROMPTDIALOG_H
