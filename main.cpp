#include "logindialog.h"
#include "spotifymanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SpotifyManager manager;

    // در ابتدا فقط صفحه لاگین باز می‌شود
    LoginDialog loginDialog(nullptr, &manager);
    loginDialog.show();

    return a.exec();
}
