/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/
#ifndef GITHUBREQUEST_H
#define GITHUBREQUEST_H

#include <QtNetwork>
#include <QTimer>


class GithubRequest : public QObject
{
    Q_OBJECT
public:
    GithubRequest();

    bool Update();
    void Stop();

protected:
    bool Request();

    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void onReady();
    void onError(QNetworkReply::NetworkError error);
    void onSslErrors(const QList<QSslError> &errors);       

protected slots:
    void timerTriggered();

signals:
    void datReady(const QByteArray &data);

private:    
    QNetworkAccessManager m_networkManager;
    QNetworkReply *m_Reply = nullptr;
    int m_delay;
    QTimer m_timer;
};

#endif // GITHUBREQUEST_H
