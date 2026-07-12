#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

using namespace std;

class Account {
private:
    int id;
    string username;
    string password;
    string name;
    string role;

public:
    Account(int _id, string _username, string _password, string _name, string _role) {
        id = _id;
        username = _username;
        password = _password;
        name = _name;
        role = _role;
    }

    int getId() { return id; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    string getName() { return name; }
    string getRole() { return role; }

    void setPassword(string newPassword) { password = newPassword; }
    void setName(string newName) { name = newName; }
};

#endif // ACCOUNT_H
