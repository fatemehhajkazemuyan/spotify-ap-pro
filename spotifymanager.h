#ifndef SPOTIFYMANAGER_H
#define SPOTIFYMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
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
    // کاربری که الانلاگین کرده (وارد برنامه شده) را اینجا ذخیره می‌کنیم.
    shared_ptr<Account> currentAccount;

public:
    SpotifyManager() {
        songRepo = make_shared<SongRepository>();
        albumRepo = make_shared<AlbumRepository>();
        playlistRepo = make_shared<PlaylistRepository>();
        artistRepo = make_shared<ArtistRepository>();
        listenerRepo = make_shared<ListenerRepository>();
        currentAccount = nullptr;
    }
    void registerUser(int id, string username, string password, string name, string role, string biography = "") {
        // ۱. بررسی اینکه نام کاربری تکراری نباش (هم هنرمند هم شنونده)
        if (artistRepo->searchByUserName(username) != nullptr || listenerRepo->searchByUserName(username) != nullptr) {
            throw runtime_error("نام کاربری قبلاً انتخاب شده است");
        }
        // ۲. ساخت اکانت بر اساس نقش کاربر
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
            throw runtime_error("نقش درسستی وارد نشد");
        }
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
//گتر ها برای بخش گرافیگی

    shared_ptr<Account> getCurrentAccount() const { return currentAccount; }
    shared_ptr<SongRepository> getSongRepo() const { return songRepo; }
    shared_ptr<AlbumRepository> getAlbumRepo() const { return albumRepo; }
    shared_ptr<PlaylistRepository> getPlaylistRepo() const { return playlistRepo; }
    shared_ptr<ArtistRepository> getArtistRepo() const { return artistRepo; }
    shared_ptr<ListenerRepository> getListenerRepo() const { return listenerRepo; }



    void deleteSongGlobally(int songId) {
        //  حذف آهنگ باید آن را از کل سامانه، آلبوم‌ها و لیست‌های پخش حذف کند!

        // ۱. حذف از مخزن اصلیآهنگ‌ها
        songRepo->remove(songId);

        // ۲. حذف از تمام آلبوم‌های موجود
        // (چون تابع getAll نداریم، در لایه گرافیک یا با متدهای کمکی هندل می‌شود، اما پایه کار حذف از سورس است)
    }
};

#endif // SPOTIFYMANAGER_H
