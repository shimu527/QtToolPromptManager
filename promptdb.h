#ifndef PROMPTDB_H
#define PROMPTDB_H

#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QString>
#include <QVariant>
class PromptDB
{
public:
    PromptDB(const QString& dbName);
    ~PromptDB();

public:
    void createTable();
    void addPrompt(const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QByteArray& coverImage);
    void deletePrompt(int promptId);
    void updatePrompt(int promptId, const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QByteArray& coverImage);
    QList<QVariantList> getPrompts(const QString& keyword = QString());
    QList<QVariantList> getPromptHistory(int promptId);
    void close();

private:
    QString dbName;
    QSqlDatabase db;

};

#endif // PROMPTDB_H
