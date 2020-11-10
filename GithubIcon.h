/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/
#ifndef GITHUBICON_H
#define GITHUBICON_H

#include <QSystemTrayIcon>
#include <QMenu>
#include "GithubRequest.h"
#include "Settings.h"


class GithubIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    GithubIcon(QObject *parent = nullptr);

protected:
    QMenu m_contextMenu;
    GithubRequest m_request;

    struct MessageData
    {
        qint64 ID;
        QString title;
        Settings::Reason reason;
        QString type;
        QDateTime datetime;
        QUrl link;
        QString repository;
    };

    int m_errorCounter = 0;

private:
    bool Init();
    void SettingsTriggered(bool checked);
    void QuitTriggered(bool checked);
    MessageData ParseJsonObject(const QJsonObject &object);
    void PopupMessage(const QVector<GithubIcon::MessageData> &messages);
    void PopupMessage(const QString &message);

public slots:
    void onDataReady(const QByteArray &data);

signals:
    void onExit();

};

#endif // GITHUBICON_H
