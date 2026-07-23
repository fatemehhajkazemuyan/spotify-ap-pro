#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spotifymanager.h"
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, SpotifyManager *manager = nullptr);
    ~MainWindow();

    // 💡 متد اصلی برای بررسی نقش کاربر و بارگذاری پنل مخصوص به او
    void loadUserPanel();

private:
    Ui::MainWindow *ui;
    SpotifyManager *db; // ذخیره اشاره‌گر مدیریت دیتابیس برنامه

private slots:
    void onCreatePlaylistClicked();
    void onEditPlaylistClicked();
    void onDeletePlaylistClicked();
    void onLogoutClicked();
    void onCreateAlbumClicked();
    void onEditProfileClicked();
    void onAlbumItemDoubleClicked(QListWidgetItem *item);
    void on_playlistsListWidget_itemClicked(QListWidgetItem *item);
    void onSearchClicked();
       void onSortClicked();
       void onDeleteAccountClicked();
       void onExploreMusicClicked();
};

#endif // MAINWINDOW_H
