#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "spotifymanager.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    // کد قبلی را با این جایگزین کن:
    explicit LoginDialog(QWidget *parent = nullptr, SpotifyManager *manager = nullptr);
    ~LoginDialog();

private slots:
    void on_loginButton_clicked();

    void on_registerButton_clicked();

private:
    Ui::LoginDialog *ui;
    SpotifyManager *db; // تبدیل به اشاره‌گر شد تا آدرسِ کلاس اصلی را نگه دارد
};

#endif // LOGINDIALOG_H
