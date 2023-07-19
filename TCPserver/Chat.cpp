#include <iostream>
#include <ranges>
#include <string_view>

#include "Chat.h"

/* Основной метод, осуществляющий процессинг команд
 * Здесь происходит обработка сообщения, приходящего от клиента.
 * А также вызов соответствующего метода для формирование ответа для клиента.
 * 
 * Сообщение от клиента приходит в виде:
 * <Название метода, отправившего сообщение>:<данные с разделителем через двоеточие>
 * Аналогичным образом формируется ответ клиенту.
 * Мы всегда знаем, какой метод клиента и что отправил на сервер, и наоборот.
 */
string Chat::messageProcessing(string clientMessage)
{
    string outMessage;

    _vectorFromMessage = messageToVector(clientMessage, ":");

    if (_vectorFromMessage[0] == "loginUser") {
        outMessage = loginUser();
    } else if (_vectorFromMessage[0] == "registerUser") {
        outMessage = registerUser();
    } else if (_vectorFromMessage[0] == "showChat") {
        outMessage = showChat();
    } else if (_vectorFromMessage[0] == "addMessage") {
        outMessage = addMessage();
    } else if (_vectorFromMessage[0] == "showUsers") {
        outMessage = showUsers();
    }

    return outMessage;
}

/**
 * Парсинг входящего сообщения.
 * Метод разбивает строку по разделителю на отдельные элементы
 * и записывает их в вектор.
 */
vector<string> Chat::messageToVector(string message, string delimiter)
{
    vector<string> resultVector;

    if (!message.empty()) {
        int start = 0;
        do {
            int idx = message.find(delimiter, start);
            if (idx == string::npos) {
                break;
            }
 
            int length = idx - start;
            resultVector.push_back(message.substr(start, length));
            start += (length + delimiter.size());
        } while (true);

        resultVector.push_back(message.substr(start));
    }

    return resultVector;
}

/**
 * Метод получения объекта User по логину.
*/
shared_ptr<User> Chat::getUserByLogin(const string &login) const
{
    for (auto &user : _users)
    {
        if (login == user.getUserLogin())
        return make_shared<User>(user);
    }

    return nullptr;
}

/**
 * Метод получения объекта User по имени.
*/
shared_ptr<User> Chat::getUserByName(const string &name) const
{
    for (auto &user : _users)
    {
        if (name == user.getUserName())
        return make_shared<User>(user);
    }

    return nullptr;
}

/**
 * Метод, формирующий ответ клиенту по попытке входа.
 * Если логин или пароль введены некорректно,
 * то клиент получает сообщение "error",
 * в случае верного ввода, клиент получает сообщение "ok".
*/
string Chat::loginUser()
{
    string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];
    char c;

    if (_currentUser == nullptr || (password != _currentUser->getUserPassword()))
    {
        _currentUser = nullptr;
        outMessage = "loginUser:error";
    }

    if (_currentUser != nullptr && (password == _currentUser->getUserPassword())) {
        name = _currentUser->getUserName();
        outMessage = "loginUser:ok:" + name;
    }

    return outMessage;
}

/**
 * Метод, формирующий ответ клиенту по попытке регистрации.
 * Если логин или имя введены некорректно,
 * то клиент получает сообщение "error",
 * в случае, если зарегистрирован уникальный пользователь,
 * то клиент получает сообщение "ok".
 * 
 * Если пользователь уникальный, то его логин, пароль и имя
 * записываются в вектор _users класса User
*/
string Chat::registerUser()
{
    string login, password, name, outMessage;
    login = _vectorFromMessage[1];
    password = _vectorFromMessage[2];
    name = _vectorFromMessage[3];

    if (getUserByLogin(login) || login == "all") {
        outMessage = "registerUser:error:login";
    }

    if (getUserByName(name) || name == "all") {
        outMessage = "registerUser:error:name";
    }

    if ((!getUserByName(name) || name != "all") && (!getUserByLogin(login) || login != "all")) {
        outMessage = "registerUser:ok:" + name;
    }

    User user = User(login, password, name);
    _users.push_back(user);
    _currentUser = make_shared<User>(user);

    return outMessage;
}

/**
 * Метод, формирующий ответ клиенту, получилось отправить сообщение в чат или нет.
 * Если сообщение отправлено в чат, то оно записывается в вектор _messages класса Message.
*/
string Chat::addMessage()
{
    string to, text, outMessage;
    to = _vectorFromMessage[1];
    text = _vectorFromMessage[2];

    if (!(to == "all" || getUserByName(to))) {
        outMessage = "addMessage:error:" + to;
    } else {
        outMessage = "addMessage:ok";

        if (to == "all")
            _messages.push_back(Message{_currentUser->getUserLogin(), "all", text});
        else
            _messages.push_back(Message{_currentUser->getUserLogin(), getUserByName(to)->getUserLogin(), text});
    }

    return outMessage;
}

/**
 * Метод, формирующий список всех сообщений отправленных в чат всеми пользователями.
 * Если чат не пустой то формируется сообщение клиенту вида:
 * <название метода: в данном случае showChat>:
 * <от кого:пользователь 1>:<кому>:<текст>:
 * <от кого:пользователь 2>:<кому>:<текст>:
 * .....
 * <от кого:пользователь N>:<кому>:<текст>:
 * <название метода: в данном случае showChat>
 * (посленее нужно чтобы при парсинге не получить отсутствующий символ после разделителя)
*/
string Chat::showChat()
{
	string from, to, text, outMessage;

    outMessage = "showChat:";

    if (_messages.empty()) {
        outMessage += "empty:";
    } else {
        for (auto& mess : _messages)
        {
            if (_currentUser->getUserLogin() == mess.getFrom() || _currentUser->getUserLogin() == mess.getTo() || mess.getTo() == "all") {
                from = (_currentUser->getUserLogin() == mess.getFrom()) ? "(меня)" : getUserByLogin(mess.getFrom())->getUserName();

                if (mess.getTo() == "all") {
                    to = "всех";
                } else {
                    to = (_currentUser->getUserLogin() == mess.getTo()) ? "(меня)" : getUserByLogin(mess.getTo())->getUserName();
                }

                text = mess.getText();
            }

            outMessage += from + ":" + to + ":" + text + ":";
    	}
    }

    outMessage += "showChat";

    return outMessage;
}

/**
 * Метод, формирующий список всех зарегистрированных пользователей.
 * Если чат не пустой то формируется сообщение клиенту вида:
 * <название метода: в данном случае showUsers>:
 * <имя пользователя 1>:
 * <имя пользователя 2>:
 * .....
 * <имя пользователя N>:
 * <название метода: в данном случае showUsers>
 * (посленее нужно чтобы при парсинге не получить отсутствующий символ после разделителя)
*/
string Chat::showUsers()
{
    string name, outMessage;

    outMessage = "showUsers:";

    for (auto& user : _users) {
        if (_currentUser->getUserLogin() == user.getUserLogin()) {
            name = "(я)";
        } else {
            name = user.getUserName();
        }

        outMessage += name + ":";
    }

    outMessage += "showUsers";

    return outMessage;
}

