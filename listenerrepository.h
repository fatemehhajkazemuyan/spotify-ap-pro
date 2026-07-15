#ifndef LISTENER_REPOSITORY_H
#define LISTENER_REPOSITORY_H

#include "accountrepository.h"
#include <string>
#include <memory>

using namespace std;

class ListenerRepository : public AccountRepository {
public:
    int save(shared_ptr<Account> entity) override;
    bool remove(int id) override;
    shared_ptr<Account> search(int id) const override;
    shared_ptr<Account> searchByUserName(string username) const override;
    void updateLiked(int listenerId, int songId, bool isLiked);
    bool isLiked(int listenerId, int songId) const;
};

#endif // LISTENER_REPOSITORY_H
