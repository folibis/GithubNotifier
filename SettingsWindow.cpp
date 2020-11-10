/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/

#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"
#include "Settings.h"
#include <QMetaEnum>


SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    LoadSettings();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::LoadSettings()
{
    try
    {
        Settings &settings = Settings::Instance();
        ui->urlBox->setText(settings.Url().toString());
        ui->userBox->setText(settings.User());
        ui->tokenBox->setText(settings.Token());
        ui->startupCheck->setChecked(settings.RunOnStartup());
        ui->linkCheck->setChecked(settings.ProvideLink());
        ui->refreshBox->setValue(settings.RefreshRate());

        Settings::Reasons reasons = settings.GetReasons();
        QMetaEnum enums = QMetaEnum::fromType<Settings::Reasons>();
        for(int i = 0;i < enums.keyCount();i ++)
        {
            Settings::Reason reason = static_cast<Settings::Reason>(enums.value(i));
            if(reason == Settings::Reason::Undefined)
            {
                continue;
            }
            QCheckBox *checkBox = new QCheckBox(ui->groupBox);
            checkBox->setText(Settings::Reason2String(reason));
            checkBox->setProperty("enum", QVariant::fromValue(reason));
            checkBox->setChecked(reasons.testFlag(reason));
            ui->reasonsLayout->addWidget(checkBox);
        }
    }
    catch(...)
    {

    }
}

void SettingsWindow::on_pushButton_clicked()
{
    Settings &settings = Settings::Instance();
    settings.SetUrl(QUrl(ui->urlBox->text()));
    settings.SetUser(ui->userBox->text());
    settings.SetToken(ui->tokenBox->text());
    settings.SetRunOnStartup(ui->startupCheck->isChecked());
    settings.SetProvideLink(ui->linkCheck->isChecked());
    settings.SetRefreshRate(ui->refreshBox->value());

    accept();
}
