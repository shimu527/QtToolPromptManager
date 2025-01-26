#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QSettings>

class BackupManager : public QObject {
    Q_OBJECT

public:
    explicit BackupManager(const QString& dbPath, QObject* parent = nullptr);
    
    void startAutoBackup(int intervalMinutes = 30);
    void stopAutoBackup();
    bool createBackup(const QString& customName = QString());
    QString getBackupDir() const;
    void setBackupDir(const QString& path);
    QStringList getBackupList() const;
    bool restoreBackup(const QString& backupPath);

signals:
    void backupCreated(const QString& path);
    void backupFailed(const QString& error);
    void backupRestored();

private slots:
    void performAutoBackup();

private:
    QString dbPath;
    QString backupDir;
    QTimer* backupTimer;
    
    QString generateBackupName() const;
    void cleanOldBackups(int keepCount = 10);
    bool copyFile(const QString& source, const QString& destination);
};

#endif // BACKUPMANAGER_H 