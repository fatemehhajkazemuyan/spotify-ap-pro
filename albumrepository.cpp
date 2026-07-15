#include "albumrepository.h"

int AlbumRepository::save(shared_ptr<Album> entity) {
    if (entity == nullptr) return -1;
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == entity->getId())
        {
            entities[i] = entity;
            return entity->getId();
        }
    }
    entities.push_back(entity);
    return entity->getId();
}

bool AlbumRepository::remove(int id) {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            entities.erase(entities.begin() + i);
            return true;
        }
    }
    return false;
}

shared_ptr<Album> AlbumRepository::search(int id) const {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            return entities[i];
        }
    }
    return nullptr;
}

vector<shared_ptr<Album>> AlbumRepository::albums(int artistId) const {
    vector<shared_ptr<Album>> result;
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getArtistId() == artistId) {
            result.push_back(entities[i]);
        }
    }
    return result;
}
