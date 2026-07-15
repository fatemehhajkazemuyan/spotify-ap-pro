#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H

#include "abstractrepository.h"
#include "Account.h"
#include <string>
#include <memory>

using namespace std;

class AccountRepository : public AbstractRepository<Account> {
public:
    virtual ~AccountRepository() = default;
    virtual shared_ptr<Account> searchByUserName(string username) const = 0;
};

#endif // ACCOUNT_REPOSITORY_H
