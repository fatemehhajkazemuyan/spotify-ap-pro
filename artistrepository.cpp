#include "artistrepository.h"

// ذخیره کردن هنرمند
int ArtistRepository::save(shared_ptr<Account> entity) {
    if (entity == nullptr || entity->getRole() != "Artist") {
        return -1;
    }
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == entity->getId()) {
            entities[i] = entity;
            return entity->getId();
        }
    }
    entities.push_back(entity);
    return entity->getId();
}

// حذف کردن هنرمند
bool ArtistRepository::remove(int id) {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            entities.erase(entities.begin() + i);
            return true;
        }
    }
    return false;
}

// پیدا کردن با آی‌دی
shared_ptr<Account> ArtistRepository::search(int id) const {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            return entities[i];
        }
    }
    return nullptr;
}

// پیدا کردن با نام کاربری
shared_ptr<Account> ArtistRepository::searchByUserName(string username) const {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getUsername() == username) {
            return entities[i];
        }
    }
    return nullptr;
}
