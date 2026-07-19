#include "mainwindow.h"
#include "logindialog.h"
#include "spotifymanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ۱. یک نمونه مشترک و واحد از مدیریت کاربران می‌سازیم
    SpotifyManager mainManager;

    // ۲. آدرس این نمونه مشترک (&mainManager) را به پنجره لاگین می‌دهیم
    LoginDialog login(nullptr, &mainManager);

    if (login.exec() == QDialog::Accepted) {
        // ۳. در آینده می‌توانیم همین mainManager را به MainWindow هم بدهیم تا آهنگ‌ها را برای همین کاربر لود کند!
        MainWindow w;
        w.show();
        return a.exec();
    }

    return 0;
}
