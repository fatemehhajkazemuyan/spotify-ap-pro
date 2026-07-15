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
    string biography;
public:
    Account(int _id, string _username, string _password, string _name, string _role, string _biography = "") {
        id = _id;
        username = _username;
        password = _password;
        name = _name;
        role = _role;
        biography = _biography;
    }


    int getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getName() const { return name; }
    string getRole() const { return role; }
    string getBiography() const { return biography; }

    void setPassword(string newPassword) { password = newPassword; }
    void setName(string newName) { name = newName; }
    void setBiography(string newBio) { biography = newBio; }
};

#endif // ACCOUNT_H
