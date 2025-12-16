

#ifndef OOP_GAME_EXCEPTIONS_H
#define OOP_GAME_EXCEPTIONS_H


#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message);
};

// Eroare fisier inexistent
class AssetMissingException : public GameException {
public:
    explicit AssetMissingException(const std::string& filename);
};

class JsonMissingAttributeException : public GameException {
public:
    explicit JsonMissingAttributeException(const std::string& filename, const std::string& AttributeName);
};

// Erori de logică în timpul jocului
// Exemplu: Dacă un inamic atacă fără armă (Weapon = Null)
class InvalidActionException : public GameException {
public:
    explicit InvalidActionException(const std::string& action);
};

// Erori de date
// Exemplu: Eroare dacă viteza inamicului este -50.
class InvalidDataException : public GameException {
public:
    explicit InvalidDataException(const std::string& varName, float value);
};


#endif //OOP_GAME_EXCEPTIONS_H