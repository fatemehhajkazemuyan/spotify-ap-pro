#ifndef ABSTRACT_REPOSITORY_HPP
#define ABSTRACT_REPOSITORY_HPP

#include <vector>
#include <optional>
#include <memory>
#include <algorithm>

template <typename T>
class AbstractRepository {
protected:
    std::vector<std::shared_ptr<T>> entities; // لیستی برای نگهداری موقت داده‌ها در حافظه

public:
    virtual ~AbstractRepository() = default;

    // ذخیره یا به‌روزرسانی یک موجودیت
    virtual void save(std::shared_ptr<T> entity) {
        if (!entity) return;

        auto it = std::find(entities.begin(), entities.end(), entity);
        if (it == entities.end()) {
            entities.push_back(entity);
        }
    }

    // حذف موجودیت بر اساس شناسه
    virtual void remove(int id) {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [id](const std::shared_ptr<T>& entity) {
                    return entity->getId() == id;
                }),
            entities.end()
        );
    }

    // جستجوی موجودیت بر اساس شناسه با استفاده از std::optional
    virtual std::optional<std::shared_ptr<T>> search(int id) const {
        for (const auto& entity : entities) {
            if (entity->getId() == id) {
                return entity;
            }
        }
        return std::nullopt;
    }

    // گرفتن همه داده‌ها
    const std::vector<std::shared_ptr<T>>& getAll() const {
        return entities;
    }
};

#endif // ABSTRACT_REPOSITORY_HPP
