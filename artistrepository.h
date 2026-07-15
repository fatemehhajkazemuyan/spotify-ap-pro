#ifndef ARTIST_REPOSITORY_H
#define ARTIST_REPOSITORY_H

#include "accountrepository.h"
#include <string>
#include <memory>

using namespace std;

class ArtistRepository : public AccountRepository {
public:
    int save(shared_ptr<Account> entity) override;
    bool remove(int id) override;
    shared_ptr<Account> search(int id) const override;
    shared_ptr<Account> searchByUserName(string username) const override;
};

#endif // ARTIST_REPOSITORY_H
