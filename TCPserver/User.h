#pragma once
#include <string>

using namespace std;

class User
{
    string _login;
    string _password;
    string _name;

public:
    User(const string& login, const string& password, const string& name):
        _login(login),
        _password(password),
        _name(name)
    {}

    User(const string& login, const string& password):
        _login(login),
        _password(password)
    {}

    ~User(){}

    const string& getUserLogin() const { return _login; }
    const string& getUserPassword() const { return _password; }
    const string& getUserName() const { return _name; }
    
    void setUserPassword(const string& password) { _password = password; }
    void setUserName(const string& name) { _name = name; }
};