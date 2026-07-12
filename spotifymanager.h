#ifndef SPOTIFYMANAGER_H
#define SPOTIFYMANAGER_H

#include <vector>
#include "Account.h"
#include "song.h"
#include "album.h"
#include "playlist.h"

using namespace std;

class SpotifyManager {
private:
    vector<Account> users;
    vector<Song> songs;
    vector<Album> albums;
    vector<Playlist> playlists;
    int currentUserIndex; // ذخیره ایندکس کاربری که الان لاگین کرده (-1 یعنی هیچکس)

public:
    SpotifyManager() {
        currentUserIndex = -1; // در ابتدا هیچ کاربری وارد نشده
    }

    // متدهای مدیریت کاربران (ثبت نام و ورود)
    bool registerUser(int id, string username, string password, string name, string role) {
        // ابتدا چک میکنیم نام کاربری تکراری نباشد
        for (auto &user : users) {
            if (user.getUsername() == username) {
                return false; // ثبت نام ناموفق به خاطر نام کاربری تکراری
            }
        }
        users.push_back(Account(id, username, password, name, role));
        return true; // ثبت نام موفق
    }

    bool loginUser(string username, string password) {
        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i].getUsername() == username && users[i].getPassword() == password) {
                currentUserIndex = i; // کاربر پیدا شد و لاگین کرد
                return true;
            }
        }
        return false; // نام کاربری یا رمز اشتباه بود
    }

    void logoutUser() {
        currentUserIndex = -1;
    }

    // متدهای مدیریت آهنگ‌ها
    void addSongToSystem(int id, string name, int year, string genre, string path, int artistId) {
        songs.push_back(Song(id, name, year, genre, path, artistId));
    }

    vector<Song> getAllSongs() {
        return songs;
    }
};

#endif // SPOTIFYMANAGER_H
