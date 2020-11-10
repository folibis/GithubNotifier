/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/
#include "Settings.h"

#define ORGANIZATION "GithubNotifier"
#define APPLICATION "GithubNotifier"


Settings &Settings::Instance()
{
    static Settings instance;
    if(!instance.m_initialized)
    {
        instance.Init();
    }

    return instance;
}

Settings::Settings() :
    m_settings(ORGANIZATION, APPLICATION)
{
    m_user = "";
    m_token = "";
    m_url = QUrl();
    m_runOnStartup = false;
    m_refreshRate = 10;
    m_provideLink = true;
}

bool Settings::Init()
{
    m_user = m_settings.value("settings/user", "").toString();
    m_token = m_settings.value("settings/token", "").toString();
    m_url = m_settings.value("settings/url", "").toUrl();
    m_reasons = static_cast<Reasons>(m_settings.value("settings/reasons", "").toInt());
    m_runOnStartup = m_settings.value("settings/runonstartup", "").toBool();
    m_refreshRate = m_settings.value("settings/refreshrate", "").toBool();
    m_provideLink = m_settings.value("settings/providelink", "").toBool();
    m_lastID = m_settings.value("settings/lastid", "").toLongLong();

    return true;
}

QString Settings::User() const
{
    return m_user;
}

void Settings::SetUser(const QString &user)
{
    m_user = user;
    m_settings.setValue("settings/user", m_user);
    m_settings.sync();
}

QString Settings::Token() const
{
    return m_token;
}

void Settings::SetToken(const QString &token)
{
    m_token = token;
    m_settings.setValue("settings/token", m_token);
    m_settings.sync();
}

QUrl Settings::Url() const
{
    return m_url;
}

void Settings::SetUrl(const QUrl &url)
{
    m_url = url;
    m_settings.setValue("settings/url", m_url);
    m_settings.sync();
}

Settings::Reasons Settings::GetReasons() const
{
    return m_reasons;
}

void Settings::SetReasons(const Reasons &reasons)
{
    m_reasons = reasons;
    m_settings.setValue("settings/reasons", static_cast<int>(m_reasons));
    m_settings.sync();
}

void Settings::AddReason(Settings::Reason reason)
{
    m_reasons |= reason;
    m_settings.setValue("settings/reasons", static_cast<int>(m_reasons));
    m_settings.sync();
}

bool Settings::RunOnStartup() const
{
    return m_runOnStartup;
}

void Settings::SetRunOnStartup(bool value)
{
    m_runOnStartup = value;
    m_settings.setValue("settings/runonstartup", m_runOnStartup);
    m_settings.sync();
}

int Settings::RefreshRate() const
{
    return m_refreshRate;
}

void Settings::SetRefreshRate(int rate)
{
    m_refreshRate = rate;
    m_settings.setValue("settings/refreshrate", m_refreshRate);
    m_settings.sync();
}

bool Settings::ProvideLink() const
{
    return m_provideLink;
}

void Settings::SetProvideLink(bool value)
{
    m_provideLink = value;
    m_settings.setValue("settings/providelink", m_provideLink);
    m_settings.sync();
}

qint64 Settings::LastID() const
{
    return m_lastID;
}

void Settings::SetLastID(qint64 ID)
{
    m_lastID = ID;
    m_settings.setValue("settings/lastid", m_lastID);
    m_settings.sync();
}

QString Settings::Reason2String(Settings::Reason reason)
{
    switch(reason)
    {
        case Settings::Reason::Assign:
            return QString("You were assigned to the issue");
        case Settings::Reason::Mention:
            return QString("You were specifically in the content");
        case Settings::Reason::ReviewRequested:
            return QString("You were requested to review a pull request");
        case Settings::Reason::SecurityAlert:
            return QString("Security alert");
        case Settings::Reason::Manual:
            return QString("You subscribed to the thread");
        case Settings::Reason::StateChange:
            return QString("You changed the thread state");
        case Settings::Reason::Subscribed:
            return QString("You're watching the repository");
        default: break;
    }

    return QString();
}

Settings::Reason Settings::String2Reason(const QString &name)
{
    switch(_(name.toLocal8Bit().data()))
    {
        case _("assign"): return Reason::Assign;
        case _("mention"): return Reason::Mention;
        case _("review_requested"): return Reason::ReviewRequested;
        case _("security_alert"): return Reason::SecurityAlert;
        case _("manual"): return Reason::Manual;
        case _("state_change"): return Reason::StateChange;
        case _("subscribed"): return Reason::Subscribed;
        default: break;
    }

    return Reason::Undefined;
}


bool Settings::HasCredentials()
{
    return (!m_url.isEmpty() && !m_user.isEmpty() && !m_token.isEmpty());
}
