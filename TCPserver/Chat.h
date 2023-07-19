#pragma once
#include <vector>
#include <exception>
#include <memory>

#include "Message.h"
#include "User.h"

using namespace std;

class Chat {
public:
    shared_ptr<User> getCurrentUser() const { return _currentUser; }
    string messageProcessing(string clientMessage);

private:
    vector<User> _users;
    vector<Message> _messages;
    shared_ptr<User> _currentUser;
    vector<string> _vectorFromMessage;

    string loginUser();
    string registerUser();
    string showChat();
    string showUsers();
    string addMessage();

    vector<User>& getAllUsers() { return _users; }
    vector<Message>& getAllMessages() { return _messages; }
    shared_ptr<User> getUserByLogin(const string& login) const;
    shared_ptr<User> getUserByName(const string& name) const;

    vector<string> messageToVector(string message, string delimiter);
};