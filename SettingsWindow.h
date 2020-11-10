/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

protected:
    void LoadSettings();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SettingsWindow *ui;
};
#endif // MAINWINDOW_H
