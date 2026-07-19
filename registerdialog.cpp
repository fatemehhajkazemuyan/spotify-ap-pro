#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>
#include <cstdlib>

RegisterDialog::RegisterDialog(QWidget *parent, SpotifyManager *manager) :
    QDialog(parent),
    ui(new Ui::RegisterDialog),
    db(manager)
{
    ui->setupUi(this);

    //  اعمال تم
    this->setStyleSheet(
        "QDialog { background-color: #FAFAFA; }"
        "QLabel { color: #333333; font-family: 'Segoe UI'; font-size: 13px; }"
        "QLineEdit, QTextEdit {"
        "   border: 2px solid #FFE6EE;"
        "   border-radius: 6px;"
        "   padding: 4px;"
        "   background-color: white;"
        "}"
        "QLineEdit:focus, QTextEdit:focus { border: 2px solid #FF6699; }"
        "QPushButton {"
        "   background-color: #FF6699; color: white;"
        "   border-radius: 6px; padding: 6px 14px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #FF4D88; }"
        "QRadioButton { color: #333333; font-size: 13px; }"
        "QRadioButton::indicator:checked { background-color: #FF6699; border: 1px solid #FF4D88; border-radius: 6px; }"
    );
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_okButton_clicked()
{
    // خواندن اطلاعات از فیلدهای متنd
    string name = ui->nameLineEdit->text().toStdString();
    string username = ui->usernameLineEdit->text().toStdString();
    string password = ui->passwordLineEdit->text().toStdString();
    string biography = ui->biographyTextEdit->toPlainText().toStdString();

    // تشخیص نقش کاربر
    string role = "";
    if (ui->artistRadioButton->isChecked()) {
        role = "Artist";
    } else if (ui->listenerRadioButton->isChecked()) {
        role = "Listener";
    }

    // بررسی خالی نبودن فیلدها)
    if (name.empty() || username.empty() || password.empty() || role.empty()) {
        QMessageBox::warning(this, "خطا", "لطفاً تمام فیلدهای ستاره‌دار و نقش خود را انتخاب کنید!");
        return;
    }
    try {
        int randomId = rand() % 9000 + 1000; // تولید یک آیدی ۴ رقمی فرضی

        // صدا زدن متد اصلی ثبت‌نام در SpotifyManager
        db->registerUser(randomId, username, password, name, role, biography);

        QMessageBox::information(this, "موفقیت", "ثبت‌نام با موفقیت انجام شد!");
        this->accept(); // بستن پنجره
    } catch (const std::exception& e) {
        // اگر نام کاربری تکراری باشد
        QMessageBox::critical(this, "خطای ثبت‌نام", QString::fromStdString(e.what()));
    }
}

void RegisterDialog::on_cancelButton_clicked()
{
    this->reject(); // بستن پنجره بدون انجام هیچ کاری (Rejected)
}
