#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "spotifymanager.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr, SpotifyManager *manager = nullptr);
    ~RegisterDialog();

private slots:
    // این اسلات‌ها (توابع) هنگام کلیک روی دکمه‌های OK و Cancel اجرا می‌شوند
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::RegisterDialog *ui;
    SpotifyManager *db; // ذخیره اشاره‌گر دیتابیس در بخش Private کلاس
};

#endif // REGISTERDIALOG_H
