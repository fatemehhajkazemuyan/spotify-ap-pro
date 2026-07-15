#include "playlistrepository.h"

int PlaylistRepository::save(shared_ptr<Playlist> entity) {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == entity->getId()) {
            entities[i] = entity;
            return entity->getId();
        }
    }
    entities.push_back(entity);
    return entity->getId();
}

bool PlaylistRepository::remove(int id) {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            entities.erase(entities.begin() + i);
            return true;
        }
    }
    return false;
}

shared_ptr<Playlist> PlaylistRepository::search(int id) const {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            return entities[i];
        }
    }
    return nullptr;
}

void PlaylistRepository::insertSong(int playlistId, int songId) {
    shared_ptr<Playlist> playlist = this->search(playlistId);
    if (playlist != nullptr) {
        playlist->addSong(songId);
    }
}

void PlaylistRepository::removeSong(int playlistId, int songId) {
    shared_ptr<Playlist> playlist = this->search(playlistId);
    if (playlist != nullptr) {
        playlist->removeSong(songId);
    }
}

vector<shared_ptr<Playlist>> PlaylistRepository::playlists(int listenerId) const {
    vector<shared_ptr<Playlist>> result;
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getCreatorId() == listenerId) {
            result.push_back(entities[i]);
        }
    }
    return result;
}
