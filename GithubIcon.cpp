/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/

#include "GithubIcon.h"
#include "SettingsWindow.h"
#include <QJsonDocument>

#define MAX_ERROR_COUNT 5


GithubIcon::GithubIcon(QObject *parent) : QSystemTrayIcon(parent)
{
    Init();
}

bool GithubIcon::Init()
{
    try
    {
        m_contextMenu.setTitle("Github actions");
        QAction *settings = m_contextMenu.addAction(QIcon(":/icons/settings.svg"), "Settings");
        connect(settings, &QAction::triggered, this, &GithubIcon::SettingsTriggered);

        QAction *quit = m_contextMenu.addAction(QIcon(":/icons/exit.svg"), "Quit");
        connect(quit, &QAction::triggered, this, &GithubIcon::QuitTriggered);

        setContextMenu(&m_contextMenu);

        QPixmap pixmap(32,32);
        pixmap.fill(QColor("red"));
        QIcon icon(":/icons/github.svg");
        setIcon(icon);

        QObject::connect(&m_request, &GithubRequest::datReady, this, &GithubIcon::onDataReady);
        m_request.Update();
    }
    catch (...)
    {
        PopupMessage(tr("Unexpected error occured during initialization"));
        m_request.Stop();
        return false;
    }
    return true;
}

void GithubIcon::SettingsTriggered(bool)
{
    SettingsWindow dialog;
    dialog.setModal(true);
    if(dialog.exec() == QDialog::Accepted)
    {
        m_request.Update();
    }
}

void GithubIcon::QuitTriggered(bool)
{
    emit onExit();
}

void GithubIcon::onDataReady(const QByteArray &data)
{
    try
    {
        Settings &settings = Settings::Instance();
        QVector<GithubIcon::MessageData> messages;

        qint64 lastID = settings.LastID();

        QJsonParseError error;

        QJsonDocument doc = QJsonDocument::fromJson(data, &error);

        qint64 id = 0;

        if(!doc.isNull() && !doc.isEmpty())
        {
            if(doc.isArray())
            {
                m_errorCounter = 0;
                QJsonArray array = doc.array();
                for (int i = 0; i < array.size(); ++i)
                {
                    QJsonObject obj = array[i].toObject();
                    GithubIcon::MessageData data = ParseJsonObject(obj);
                    if(lastID == 0)
                    {
                        lastID = data.ID;
                        settings.SetLastID(lastID);
                        return;
                    }
                    else
                    {
                        if(lastID >= data.ID)
                        {
                            break;
                        }

                        messages.append(data);

                        if(data.ID > id)
                        {
                            id = data.ID;
                            settings.SetLastID(id);
                        }
                    }
                }
            }
        }
        else
        {
            m_errorCounter ++;
            if(m_errorCounter >= MAX_ERROR_COUNT)
            {
                m_request.Stop();
                PopupMessage("Stoping during to communication errors");
            }
        }

        if(messages.length() > 0)
        {
            PopupMessage(messages);
        }
    }
    catch(...)
    {
        PopupMessage(tr("Unexpected error occured during request"));
        m_request.Stop();
    }
}

GithubIcon::MessageData GithubIcon::ParseJsonObject(const QJsonObject &object)
{
    GithubIcon::MessageData data;

    if(object.contains("id"))
    {
        data.ID = object["id"].toVariant().toLongLong();
    }

    if(object.contains("reason"))
    {
        data.reason = Settings::String2Reason(object["reason"].toString());
    }

    if(object.contains("updated_at"))
    {
        data.datetime = object["updated_at"].toVariant().toDateTime();
    }

    if(object.contains("subject") && object["subject"].isObject())
    {
        QJsonObject subject = object["subject"].toObject();
        if(subject.contains("title"))
        {
            data.title = subject["title"].toString("");
        }

        if(subject.contains("url"))
        {
            data.link = subject["url"].toVariant().toUrl();
        }

        if(subject.contains("type"))
        {
            data.type = subject["type"].toString("");
        }
    }

    if(object.contains("repository") && object["repository"].isObject())
    {
        QJsonObject subject = object["repository"].toObject();
        if(subject.contains("full_name"))
        {
            data.repository = subject["full_name"].toString("");
        }
    }

    return data;
}

void GithubIcon::PopupMessage(const QVector<GithubIcon::MessageData> &messages)
{
    QString str = "";

    foreach(const GithubIcon::MessageData &message, messages)
    {
        str += "Title: " + message.title + "\r\n";
        str += "Reason: " + Settings::Reason2String(message.reason) + "\r\n";
        str += "Repository: " + message.repository + "\r\n";
        str += "Date: " + message.datetime.toString("dd.MM.yyyy") + "\r\n";
        str += "Type: " + message.type + "\r\n";
    }

    showMessage("GitHub notification", str);
}

void GithubIcon::PopupMessage(const QString &message)
{
    showMessage("GitHub notification", message);
}


