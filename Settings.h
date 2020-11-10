/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QUrl>
#include <QSettings>
#include <QList>


class Settings : public QObject
{
    Q_OBJECT
public:

    enum class Reason
    {
        Undefined = 0,
        Assign = 1,
        Mention = 2,
        ReviewRequested = 4,
        SecurityAlert = 8,
        Manual = 16,
        StateChange = 32,
        Subscribed = 64
    };

    Q_DECLARE_FLAGS(Reasons, Reason)
    Q_FLAG(Reasons)

    static Settings &Instance();

    QString User() const;
    void SetUser(const QString &user);

    QString Token() const;
    void SetToken(const QString &token);

    QUrl Url() const;
    void SetUrl(const QUrl &url);

    Reasons GetReasons() const;
    void SetReasons(const Reasons &reasons);
    void AddReason(Reason reason);

    bool RunOnStartup() const;
    void SetRunOnStartup(bool value);

    int RefreshRate() const;
    void SetRefreshRate(int rate);

    bool ProvideLink() const;
    void SetProvideLink(bool value);

    qint64 LastID() const;
    void SetLastID(qint64 ID);

    static QString Reason2String(Reason reason);
    static Settings::Reason String2Reason(const QString &name);

    bool HasCredentials();

protected:
    Settings();
    bool Init();

private:
    QSettings m_settings;
    bool m_initialized = false;

    QString m_user = "";
    QString m_token = "";
    QUrl m_url = QUrl("api.github.com/notifications");
    Reasons m_reasons = Reason::Undefined;
    bool m_runOnStartup = false;
    int m_refreshRate = 60;
    bool m_provideLink = true;

    qint64 m_lastID = 0;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Settings::Reasons)
Q_DECLARE_METATYPE(Settings::Reason)

constexpr uint64_t mix(char m, uint64_t s)
{
    return ((s << 7) + ~(s >> 3)) + static_cast<uint64_t>(~m);
}

constexpr uint64_t _(const char* str)
{
    return (*str) ? mix(*str,_(str + 1)) : 0;
}

#endif // SETTINGS_H
