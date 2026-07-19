#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "spotifymanager.h"
#include "registerdialog.h"

LoginDialog::LoginDialog(QWidget *parent, SpotifyManager *manager) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    db(manager) // کلاسِ پاس داده شده را اینجا ذخیره می‌کنیم
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked()
{
    // ۱. خواندن متن‌هایی که کاربر تایپ کرده است
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // ۲. بررسی ساده برای اینکه فیلدها خالی نباشند
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً نام کاربری و رمز عبور را وارد کنید!");
        return;
    }

    // ۳. بررسی ورود کاربر با استفاده از try-catch
    try {
        // ❌ خط SpotifyManager manager; پاک شد تا از نمونه اصلی استفاده شود

        // تلاش برای ورود با استفاده از اشاره‌گر db
        db->loginUser(username.toStdString(), password.toStdString());

        // اگر بدون خطا وارد شود، یعنی یوزرنیم و پسورد درست بوده است:
        QMessageBox::information(this, "موفقیت", "خوش آمدید!");
        this->accept();

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "خطای ورود", QString::fromStdString(e.what()));
    }
}

void LoginDialog::on_registerButton_clicked()
{
    // ساختن نمونه از پنجره ثبت‌نام و پاس دادن دیتابیس مشترک (&*db یا همان db)
    RegisterDialog registerWin(this, db);

    // نمایش پنجره ثبت‌نام به صورت مدال (تا زمانی که این پنجره باز است کاربر به صفحه لاگین دسترسی ندارد)
    registerWin.exec();
}
