#include "promptdialog.h"
#include <QFileDialog>

PromptDialog::PromptDialog(QWidget* parent) : QDialog(parent) {
    setupCommonDialog();
}

void PromptDialog::setupCommonDialog() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Title
    QLabel* titleLabel = new QLabel("Title:", this);
    layout->addWidget(titleLabel);
    titleEdit = new QLineEdit(this);
    layout->addWidget(titleEdit);

    // Content
    QLabel* contentLabel = new QLabel("Content:", this);
    layout->addWidget(contentLabel);
    contentEdit = new QTextEdit(this);
    layout->addWidget(contentEdit);

    // Description
    QLabel* descriptionLabel = new QLabel("Description:", this);
    layout->addWidget(descriptionLabel);
    descriptionEdit = new QTextEdit(this);
    layout->addWidget(descriptionEdit);

    // Tags
    QLabel* tagsLabel = new QLabel("Tags:", this);
    layout->addWidget(tagsLabel);
    tagsEdit = new QLineEdit(this);
    layout->addWidget(tagsEdit);

    // Version
    QLabel* versionLabel = new QLabel("Version:", this);
    layout->addWidget(versionLabel);
    versionEdit = new QLineEdit(this);
    layout->addWidget(versionEdit);

    // Cover Image
    QLabel* coverImageLabel = new QLabel("Cover Image:", this);
    layout->addWidget(coverImageLabel);
    coverImageEdit = new QLineEdit(this);
    coverImageEdit->setReadOnly(true);
    layout->addWidget(coverImageEdit);
    coverImageButton = new QPushButton("Select File", this);
    layout->addWidget(coverImageButton);

    connect(coverImageButton, &QPushButton::clicked, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Select Cover Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            coverImageEdit->setText(fileName);
        }
    });

    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* saveButton = new QPushButton("Save", this);
    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);

    buttonLayout->addWidget(saveButton);
    QPushButton* cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(layout);
}

void PromptDialog::setTitle(const QString& title) {
    titleEdit->setText(title);
}

QString PromptDialog::getTitle() const {
    return titleEdit->text();
}

void PromptDialog::setContent(const QString& content) {
    contentEdit->setText(content);
}

QString PromptDialog::getContent() const {
    return contentEdit->toPlainText();
}

void PromptDialog::setDescription(const QString& description) {
    descriptionEdit->setText(description);
}

QString PromptDialog::getDescription() const {
    return descriptionEdit->toPlainText();
}

void PromptDialog::setTags(const QString& tags) {
    tagsEdit->setText(tags);
}

QString PromptDialog::getTags() const {
    return tagsEdit->text();
}

void PromptDialog::setVersion(const QString& version) {
    versionEdit->setText(version);
}

QString PromptDialog::getVersion() const {
    return versionEdit->text();
}

void PromptDialog::setCoverImagePath(const QString& path) {
    coverImageEdit->setText(path);
}

QString PromptDialog::getCoverImagePath() const {
    return coverImageEdit->text();
}
