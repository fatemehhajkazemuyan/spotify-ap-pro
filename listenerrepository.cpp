#include "listenerrepository.h"

// ذخیره کردن شنونده
int ListenerRepository::save(shared_ptr<Account> entity) {
    if (entity == nullptr || entity->getRole() != "Listener") {
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

// حذف کردن شنونده
bool ListenerRepository::remove(int id) {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            entities.erase(entities.begin() + i);
            return true;
        }
    }
    return false;
}

// پیدا کردن با آی‌دی
shared_ptr<Account> ListenerRepository::search(int id) const {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getId() == id) {
            return entities[i];
        }
    }
    return nullptr;
}

// پیدا کردن با نام کاربری
shared_ptr<Account> ListenerRepository::searchByUserName(string username) const {
    for (size_t i = 0; i < entities.size(); i++) {
        if (entities[i]->getUsername() == username) {
            return entities[i];
        }
    }
    return nullptr;
}

// تغییر وضعیت لایک (گرافیک باید کامل شه)
void ListenerRepository::updateLiked(int listenerId, int songId, bool isLiked) {
    shared_ptr<Account> listener = search(listenerId);
    if (listener != nullptr) {
        // اینجا بعدا متد لایک ساده کلاس اکانت را صدا می‌زنیم
    }
}

// لایک شده؟
bool ListenerRepository::isLiked(int listenerId, int songId) const {
    shared_ptr<Account> listener = search(listenerId);
    if (listener != nullptr) {
        return false;
    }
    return false;
}
