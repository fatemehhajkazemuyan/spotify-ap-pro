#ifndef ABSTRACT_REPOSITORY_H
#define ABSTRACT_REPOSITORY_H

#include <vector>
#include <memory>

using namespace std;

template <typename T>
class AbstractRepository {
protected:
    vector<shared_ptr<T>> entities;

public:
    virtual ~AbstractRepository() = default;

    // توابع مجازی خالص دانشجویی و ساده
    virtual int save(shared_ptr<T> entity) = 0;
    virtual bool remove(int id) = 0;
    virtual shared_ptr<T> search(int id) const = 0; // خروجی ساده شد!

    const vector<shared_ptr<T>>& getAll() const {
        return entities;
    }
};

#endif // ABSTRACT_REPOSITORY_H
