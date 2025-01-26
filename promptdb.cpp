#include "promptdb.h"

//构造函数
PromptDB::PromptDB(const QString& dbName) : dbName(dbName){
    //添加db
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    //预防
    if(!db.open()){
        qDebug() << "Error: connection with database failed";
    } else {
        qDebug()<< "db connection ok";
    }
}

//析构函数
PromptDB::~PromptDB()
{
    if(!db.isOpen()){
        db.close();
    }
}

void PromptDB::createTable()
{
    QSqlQuery query;
    // query.exec("CREATE TABLE IF NOT EXISTS prompts ("
    //            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //            "content TEXT NOT NULL, "
    //            "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
    //            "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP)");

    // query.exec("CREATE TABLE IF NOT EXISTS prompt_history ("
    //            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //            "prompt_id INTEGER NOT NULL, "
    //            "content TEXT NOT NULL, "
    //            "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
    //            "FOREIGN KEY (prompt_id) REFERENCES prompts(id))");
    // 创建 prompts 表
    query.exec("CREATE TABLE IF NOT EXISTS prompts ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "content TEXT NOT NULL, "
               "title TEXT, " // 新增：标题
               "description TEXT, " // 新增：描述
               "tags TEXT, " // 新增：标签
               "version TEXT, " // 新增：版本
               "cover_image BLOB, " // 新增：封面图片（存储为二进制数据）
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
               "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP)");

    // 创建 prompt_history 表
    query.exec("CREATE TABLE IF NOT EXISTS prompt_history ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "prompt_id INTEGER NOT NULL, "
               "content TEXT NOT NULL, "
               "title TEXT, " // 新增：标题
               "description TEXT, " // 新增：描述
               "tags TEXT, " // 新增：标签
               "version TEXT, " // 新增：版本
               "cover_image BLOB, " // 新增：封面图片（存储为二进制数据）
               "updated_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
               "FOREIGN KEY (prompt_id) REFERENCES prompts(id))");
}

void PromptDB::addPrompt(const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QByteArray& coverImage) {
    QSqlQuery query;
    query.prepare("INSERT INTO prompts (content, title, description, tags, version, cover_image) "
                  "VALUES (:content, :title, :description, :tags, :version, :cover_image)");
    query.bindValue(":content", content);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":tags", tags);
    query.bindValue(":version", version);
    query.bindValue(":cover_image", coverImage);

    if (!query.exec()) {
        qDebug() << "Add prompt failed:" << query.lastError().text();
    }
}
void PromptDB::deletePrompt(int promptId) {
    QSqlQuery query;
    query.prepare("DELETE FROM prompts WHERE id = :id");
    query.bindValue(":id", promptId);
    if (!query.exec()) {
        qDebug() << "Delete prompt failed:" << query.lastError().text();
    }
}

void PromptDB::updatePrompt(int promptId, const QString& content, const QString& title, const QString& description, const QString& tags, const QString& version, const QByteArray& coverImage) {
    QSqlQuery query;
    query.prepare("UPDATE prompts SET content = :content, title = :title, description = :description, tags = :tags, version = :version, cover_image = :cover_image "
                  "WHERE id = :id");
    query.bindValue(":content", content);
    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":tags", tags);
    query.bindValue(":version", version);
    query.bindValue(":cover_image", coverImage);
    query.bindValue(":id", promptId);

    if (!query.exec()) {
        qDebug() << "Update prompt failed:" << query.lastError().text();
    }
}

QList<QList<QVariant>> PromptDB::getPrompts(const QString& keyword) {
    QSqlQuery query;
    if (!keyword.isEmpty()) {
        query.prepare("SELECT * FROM prompts WHERE content LIKE :keyword OR title LIKE :keyword OR description LIKE :keyword OR tags LIKE :keyword");
        query.bindValue(":keyword", "%" + keyword + "%");
    } else {
        query.prepare("SELECT * FROM prompts");
    }

    QList<QList<QVariant>> result;
    if (query.exec()) {
        while (query.next()) {
            QList<QVariant> row;
            row << query.value(0)  // ID
                << query.value(1)  // Content
                << query.value(2)  // Title
                << query.value(3)  // Description
                << query.value(4)  // Tags
                << query.value(5)  // Version
                << query.value(6)  // Cover Image
                << query.value(7)  // Created At
                << query.value(8); // Updated At
            result.append(row);
        }
    } else {
        qDebug() << "Get prompts failed:" << query.lastError().text();
    }
    return result;
}
QList<QVariantList> PromptDB::getPromptHistory(int promptId) {
    QSqlQuery query;
    query.prepare("SELECT * FROM prompt_history WHERE prompt_id = :id");
    query.bindValue(":id", promptId);

    QList<QVariantList> result;
    if (query.exec()) {
        while (query.next()) {
            QVariantList row;
            row << query.value(0) << query.value(1) << query.value(2) << query.value(3);
            result.append(row);
        }
    } else {
        qDebug() << "Get prompt history failed:" << query.lastError().text();
    }
    return result;
}

void PromptDB::close() {
    if (db.isOpen()) {
        db.close();
    }
}
