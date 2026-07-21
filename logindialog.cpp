#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "spotifymanager.h"
#include "registerdialog.h"
#include "mainwindow.h"

LoginDialog::LoginDialog(QWidget *parent, SpotifyManager *manager) :
    QDialog(parent),
    ui(new Ui::LoginDialog),
    db(manager)// ادرس مدیریت سیستم
{
    ui->setupUi(this);// رابط گرافیکی
}

LoginDialog::~LoginDialog()
{
    delete ui;
}
// تابع کلیک روی دکمه ورود
void LoginDialog::on_loginButton_clicked()
{
    // متن یوزر و پسورد که کاربر وارد کرده را داخلش می زاریک
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    // اگه چیزی خالی بود
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً نام کاربری و رمز عبور را وارد کنید!");
        return;
    }
    // هندل کردن خطا ها
    try {
       // تبدیل نام کاربری و رمز عبور از کیو استرینگ ه استرینگ
        db->loginUser(username.toStdString(), password.toStdString());

        // گرفتن اکانت کاربری که تازه لاگین کرده
        auto currentAccount = db->getCurrentAccount();

        if (currentAccount != nullptr) {
            // دریافت نقش کاربر (شنونده یا هنرمند)
            std::string role = currentAccount->getRole();

            QMessageBox::information(this, "موفقیت", "خوش آمدید!");

            if (role == "Listener") {
                // اگر کاربر شنونده بود، صفحه شنونده (MainWindow) را باز کن
                MainWindow *listenerWin = new MainWindow(nullptr, db); // پاس دادن db به پنجره بعدی
                listenerWin->setAttribute(Qt::WA_DeleteOnClose); // پاک‌سازی خودکار حافظه هنگام بستن پنجره
                listenerWin->show();

                this->accept(); // بستن پنجره لاگین
            }
            else if (role == "Artist") {
                QMessageBox::information(this, "پنل هنرمند", "در حال انتقال به پنل اختصاصی هنرمند...");
            }
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "خطای ورود", QString::fromStdString(e.what()));
    }
}

void LoginDialog::on_registerButton_clicked()
{
    RegisterDialog registerWin(this, db);
    registerWin.exec();
}
