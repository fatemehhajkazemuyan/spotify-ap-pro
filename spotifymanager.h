#ifndef SPOTIFYMANAGER_H
#define SPOTIFYMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include "song.h"
#include "songrepository.h"
#include "albumrepository.h"
#include "playlistrepository.h"
#include "artistrepository.h"
#include "listenerrepository.h"

using namespace std;

class SpotifyManager {
private:
    shared_ptr<SongRepository> songRepo;
    shared_ptr<AlbumRepository> albumRepo;
    shared_ptr<PlaylistRepository> playlistRepo;
    shared_ptr<ArtistRepository> artistRepo;
    shared_ptr<ListenerRepository> listenerRepo;
    shared_ptr<Account> currentAccount;

    // متد پاکسازی کاراکترهای مخفی ویندوز
    void cleanString(string &s) {
        s.erase(s.find_last_not_of(" \n\r\t") + 1);
    }

public:
    SpotifyManager() {
        songRepo = make_shared<SongRepository>();
        albumRepo = make_shared<AlbumRepository>();
        playlistRepo = make_shared<PlaylistRepository>();
        artistRepo = make_shared<ArtistRepository>();
        listenerRepo = make_shared<ListenerRepository>();
        currentAccount = nullptr;

        // لود کردن کاربران با شروع برنامه
        loadUsersFromFile();
    }

    void registerUser(int id, string username, string password, string name, string role, string biography = "") {
        if (artistRepo->searchByUserName(username) != nullptr || listenerRepo->searchByUserName(username) != nullptr) {
            throw runtime_error("نام کاربری قبلاً انتخاب شده است");
        }

        if (role == "Artist") {
            auto newArtist = make_shared<Account>(id, username, password, name, role, biography);
            artistRepo->save(newArtist);
        }
        else if (role == "Listener") {
            auto newListener = make_shared<Account>(id, username, password, name, role, biography);
            listenerRepo->save(newListener);
            auto favPlaylist = make_shared<Playlist>(id + 100000, "Favorites", id);
            playlistRepo->save(favPlaylist);
        }
        else {
            throw runtime_error("نقش درستی وارد نشد");
        }

        ofstream file("users_spotify.txt", ios::app);
        if (file.is_open()) {
            file << id << ","
                 << username << ","
                 << password << ","
                 << name << ","
                 << role << ","
                 << (biography.empty() ? "none" : biography) << "\n";
            file.close();
        }
    }

    void loadUsersFromFile() {
        ifstream file("D:/users_spotify.txt");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, username, password, name, role, biography;

            getline(ss, idStr, ',');
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, name, ',');
            getline(ss, role, ',');
            getline(ss, biography);

            if (!idStr.empty()) cleanString(idStr);
            if (!username.empty()) cleanString(username);
            if (!password.empty()) cleanString(password);
            if (!name.empty()) cleanString(name);
            if (!role.empty()) cleanString(role);
            if (!biography.empty()) cleanString(biography);

            if (!username.empty() && !role.empty()) {
                int id = stoi(idStr);
                if (biography == "none") biography = "";

                if (role == "Artist") {
                    auto newArtist = make_shared<Account>(id, username, password, name, role, biography);
                    artistRepo->save(newArtist);
                }
                else if (role == "Listener") {
                    auto newListener = make_shared<Account>(id, username, password, name, role, biography);
                    listenerRepo->save(newListener);

                    auto favPlaylist = make_shared<Playlist>(id + 100000, "Favorites", id);
                    playlistRepo->save(favPlaylist);
                }
            }}file.close();
    }

    void loginUser(string username, string password) {
        shared_ptr<Account> account = nullptr;
        account = artistRepo->searchByUserName(username);
        if (account == nullptr) {
            account = listenerRepo->searchByUserName(username);
        }
        if (account == nullptr) {
            throw runtime_error("کاربری با این نام کاربری یافت نشد!");
        }
        if (account->getPassword() != password) {
            throw runtime_error("رمز عبور اشتباه است!");
        }
        currentAccount = account;
    }

    void logoutUser() {
        currentAccount = nullptr;
    }

    // متد جدید برای افزودن آهنگ توسط هنرمند (داخل کلاس)
    bool addSongByArtist(int currentArtistId, string name, int releaseYear, string genre, string filePath, int albumId = 0) {
        // ۱. تولید آیدی یکتا برای آهنگ جدید
        int newSongId = songRepo->getAll().size() + 1;

        // ۲. ساخت آهنگ با ورودی‌های دقیق کلاس Song
        auto newSong = make_shared<Song>(newSongId, name, releaseYear, genre, filePath, currentArtistId, albumId);

        // ۳. ذخیره در ریپازیتوری آهنگ‌ها
        songRepo->save(newSong);

        // ۴. اگر آلبوم انتخاب شده بود، اضافه کردن آهنگ به آلبوم
        if (albumId != 0) {
            auto albums = albumRepo->getAll();
            for (auto& album : albums) {
                if (album->getId() == albumId) {
                    album->addSong(newSongId);
                    break;
                }
            }
        }

        return true;
    }
    void createPlaylistByListener(int listenerId, string playlistName) {
        int newPlaylistId = playlistRepo->getAll().size() + 1001;
        auto newPlaylist = make_shared<Playlist>(newPlaylistId, playlistName, listenerId);
        playlistRepo->save(newPlaylist);
    }
    void createAlbumByArtist(int artistId, string albumName) {
        // ۱. تولید خودکار شناسه آلبوم (تعداد آلبوم‌ها + ۱ یا آیدی آخرین آلبوم + ۱)
        int newAlbumId = 1;
        if (!albumRepo->getAll().empty()) {
            newAlbumId = albumRepo->getAll().back()->getId() + 1;
        }

        // ۲. ساخت شیء آلبوم جدید با شناسه خودکار و شناسه هنرمند (بدون سال/اختیاری)
        auto newAlbum = make_shared<Album>(newAlbumId, albumName, artistId, 2026);

        // ۳. ذخیره در ریپازیتوری آلبوم‌ها
        albumRepo->save(newAlbum);
    }
    void updateSongNameGlobally(int songId, string newName) {
        auto songs = songRepo->getAll();
        for (auto& song : songs) {
            if (song->getId() == songId) {
                song->setName(newName);
                break;
            }
        }
    }

    shared_ptr<Account> getCurrentAccount() const { return currentAccount; }
    shared_ptr<SongRepository> getSongRepo() const { return songRepo; }
    shared_ptr<AlbumRepository> getAlbumRepo() const { return albumRepo; }
    shared_ptr<PlaylistRepository> getPlaylistRepo() const { return playlistRepo; }
    shared_ptr<ArtistRepository> getArtistRepo() const { return artistRepo; }
    shared_ptr<ListenerRepository> getListenerRepo() const { return listenerRepo; }

    void deleteSongGlobally(int songId) {
        songRepo->remove(songId);
    }
};

#endif // SPOTIFYMANAGER_H
