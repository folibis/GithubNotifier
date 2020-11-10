/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/
#include "GithubRequest.h"
#include <QtNetworkAuth>
#include "Settings.h"
#include <QtDebug>



GithubRequest::GithubRequest()
{
    QObject::connect(&m_networkManager, &QNetworkAccessManager::authenticationRequired, this, &GithubRequest::authenticationRequired);
    QObject::connect(&m_timer, &QTimer::timeout, this, &GithubRequest::timerTriggered);
}

bool GithubRequest::Request()
{
    try
    {
        Settings &settings = Settings::Instance();

        QNetworkRequest request;
        request.setUrl(settings.Url());

        QString concatenated = settings.User() + ":" + settings.Token();
        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        m_Reply = m_networkManager.get(request);        
        connect(m_Reply, &QNetworkReply::finished, this, &GithubRequest::onReady);
        connect(m_Reply, &QNetworkReply::errorOccurred,this, &GithubRequest::onError);
        connect(m_Reply, &QNetworkReply::sslErrors, this, &GithubRequest::onSslErrors);

        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool GithubRequest::Update()
{
    Stop();

    Settings &settings = Settings::Instance();

    if(settings.HasCredentials())
    {        
        m_timer.setInterval(settings.RefreshRate() * 1000);
        m_timer.start();
    }

    return true;
}

void GithubRequest::Stop()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
    }
}

void GithubRequest::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(reply);
    Settings &settings = Settings::Instance();
    authenticator->setUser(settings.User());
    authenticator->setPassword(settings.Token());
}

void GithubRequest::onReady()
{
    if(m_Reply != nullptr)
    {       
        emit datReady(m_Reply->readAll());
    }
}

void GithubRequest::onError(QNetworkReply::NetworkError error)
{
    qDebug() << "error: " << error;
}

void GithubRequest::onSslErrors(const QList<QSslError> &)
{
    qDebug() << "SSL error: ";
}

void GithubRequest::timerTriggered()
{
    Request();
}
