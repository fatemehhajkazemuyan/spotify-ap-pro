#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>     // برای نشان دادن پیغام‌های کوچک هشدار یا خطا به کاربر
#include "SpotifyManager.h" // کلاس اصلی برنامه‌ات که کارهای ورود و ثبت‌نام را مدیریت می‌کند

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
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
        return; // اجرای تابع را متوقف کن تا بقیه کدها اجرا نشوند
    }

    // ۳. بررسی ورود کاربر با استفاده از try-catch (دقیقاً طبق داکیومنت پروژه)
    try {
        SpotifyManager manager; // یک نمونه موقت برای تست کدهایمان

        // چند اکانت تستی به صورت فرضی می‌سازیم تا بتوانیم کارکرد را تست کنیم
        manager.registerUser(1, "ali", "1234", "علی", "Listener");
        manager.registerUser(2, "reza", "5678", "رضا", "Artist", "بیوگرافی هنرمند");

        // تلاش برای ورود
        manager.loginUser(username.toStdString(), password.toStdString());

        // اگر بدون خطا وارد شود، یعنی یوزرنیم و پسورد درست بوده است:
        QMessageBox::information(this, "موفقیت", "خوش آمدید!");
        this->accept(); // بستن پنجره ورود و بازگشت با موفقیت

    } catch (const std::exception& e) {
        // اگر خطایی رخ دهد (مثلاً یوزرنیم نبود یا پسورد اشتباه بود)، پیام خطا نشان داده می‌شود
        QMessageBox::critical(this, "خطای ورود", QString::fromStdString(e.what()));
    }
}
void LoginDialog::on_registerButton_clicked()
{
    // ۱. خواندن یوزرنیم و پسورد وارد شده توسط کاربر
    // (اگر در مرحله قبل اسم فیلدها را تغییر ندادی، به جای usernameLineEdit از lineEdit استفاده کن)
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // ۲. بررسی اینکه فیلدها خالی نباشند
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً برای ثبت‌نام، نام کاربری و رمز عبور را وارد کنید!");
        return;
    }

    // ۳. تلاش برای ثبت‌نام کاربر جدید
    try {
        SpotifyManager manager;

        // کاربر را به عنوان شنونده (Listener) با یک آی‌دی ساده ثبت‌نام می‌کنیم
        // متدهای کلاس خودت را صدا می‌زنیم:
        manager.registerUser(3, username.toStdString(), password.toStdString(), username.toStdString(), "Listener");

        // نشان دادن پیغام موفقیت به کاربر
        QMessageBox::information(this, "ثبت‌نام موفق", "اکانت شما با موفقیت ساخته شد!\nحالا می‌توانید وارد شوید.");

    } catch (const std::exception& e) {
        // اگر نام کاربری تکراری باشد یا خطایی رخ دهد، اینجا پیام خطا نشان داده می‌شود
        QMessageBox::critical(this, "خطای ثبت‌نام", QString::fromStdString(e.what()));
    }
}
