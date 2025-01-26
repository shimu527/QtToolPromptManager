#include "backupmanager.h"
#include <QDebug>

BackupManager::BackupManager(const QString& dbPath, QObject* parent)
    : QObject(parent), dbPath(dbPath) {
    
    backupTimer = new QTimer(this);
    connect(backupTimer, &QTimer::timeout, this, &BackupManager::performAutoBackup);
    
    // 设置默认备份目录
    backupDir = QDir::current().filePath("backups");
    QDir().mkpath(backupDir);
}

void BackupManager::startAutoBackup(int intervalMinutes) {
    backupTimer->start(intervalMinutes * 60 * 1000); // 转换为毫秒
    qDebug() << "Auto backup started with interval:" << intervalMinutes << "minutes";
}

void BackupManager::stopAutoBackup() {
    backupTimer->stop();
    qDebug() << "Auto backup stopped";
}

bool BackupManager::createBackup(const QString& customName) {
    QString backupName = customName.isEmpty() ? generateBackupName() : customName;
    QString backupPath = QDir(backupDir).filePath(backupName);
    
    if (copyFile(dbPath, backupPath)) {
        emit backupCreated(backupPath);
        cleanOldBackups();
        return true;
    }
    
    emit backupFailed("Failed to create backup");
    return false;
}

QString BackupManager::getBackupDir() const {
    return backupDir;
}

void BackupManager::setBackupDir(const QString& path) {
    backupDir = path;
    QDir().mkpath(backupDir);
}

QStringList BackupManager::getBackupList() const {
    QDir dir(backupDir);
    return dir.entryList(QStringList() << "*.db", QDir::Files, QDir::Time);
}

bool BackupManager::restoreBackup(const QString& backupPath) {
    if (copyFile(backupPath, dbPath)) {
        emit backupRestored();
        return true;
    }
    return false;
}

void BackupManager::performAutoBackup() {
    createBackup();
}

QString BackupManager::generateBackupName() const {
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    return QString("backup_%1.db").arg(timestamp);
}

void BackupManager::cleanOldBackups(int keepCount) {
    QStringList backups = getBackupList();
    while (backups.size() > keepCount) {
        QString oldestBackup = QDir(backupDir).filePath(backups.last());
        QFile::remove(oldestBackup);
        backups.removeLast();
    }
}

bool BackupManager::copyFile(const QString& source, const QString& destination) {
    if (QFile::exists(destination)) {
        QFile::remove(destination);
    }
    return QFile::copy(source, destination);
} 