#include "songrepository.h"

// ذخیره آهنگ
int SongRepository::save(shared_ptr<Song> entity) {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == entity->getId()) {
            entities[i] = entity;
            return entity->getId();
        }
    }
    entities.push_back(entity);
    return entity->getId();
}

// حذف آهنگ
bool SongRepository::remove(int id) {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            entities.erase(entities.begin() + i);
            return true;
        }
    }
    return false;
}

// جستجوی آهنگ
shared_ptr<Song> SongRepository::search(int id) const {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            return entities[i];
        }
    }
    return nullptr;
}

// آهنگ‌های تکی هنرمند
vector<shared_ptr<Song>> SongRepository::singleSongs(int artistId) const {
    vector<shared_ptr<Song>> result;
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getArtistId() == artistId && entities[i]->getAlbumId() <= 0) {
            result.push_back(entities[i]);
        }
    }
    return result;
}

// آهنگ‌های یک آلبوم
vector<shared_ptr<Song>> SongRepository::getByAlbum(int albumId) const {
    vector<shared_ptr<Song>> result;
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getAlbumId() == albumId) {
            result.push_back(entities[i]);
        }
    }
    return result;
}

// آهنگ‌های یک هنرمند
vector<shared_ptr<Song>> SongRepository::getByArtist(int artistId) const {
    vector<shared_ptr<Song>> result;
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getArtistId() == artistId) {
            result.push_back(entities[i]);
        }
    }
    return result;
}

// آهنگ‌های یک پلی‌لیست
vector<shared_ptr<Song>> SongRepository::getByPlaylist(int playlistId, shared_ptr<PlaylistRepository> playlistRepo) const {
    vector<shared_ptr<Song>> result;
    if (playlistRepo == nullptr) return result;

    shared_ptr<Playlist> playlist = playlistRepo->search(playlistId);
    if (playlist != nullptr) {
        vector<int> songIds = playlist->getSongIds();
        for (size_t i = 0; i < songIds.size(); i++) {
            shared_ptr<Song> song = this->search(songIds[i]);
            if (song != nullptr) {
                result.push_back(song);
            }
        }
    }
    return result;
}

// آهنگ‌های لایک شده شنونده
vector<shared_ptr<Song>> SongRepository::getByLikedSongs(int listenerId, shared_ptr<PlaylistRepository> playlistRepo) const {
    vector<shared_ptr<Song>> result;
    if (playlistRepo == nullptr) return result;

    vector<shared_ptr<Playlist>> userPlaylists = playlistRepo->playlists(listenerId);
    for (size_t i = 0; i < userPlaylists.size(); i++) {
        if (userPlaylists[i]->getName() == "Favorites") {
            return getByPlaylist(userPlaylists[i]->getId(), playlistRepo);
        }
    }
    return result;
}
