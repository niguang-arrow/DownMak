#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <map>


#ifndef TOKEN_H
#define TOKEN_H

namespace ssql {

using std::string;
using std::ostream;
using std::map;

// keywords and operators
enum Type {
    ID, NUM, CREATE, TABLE, INT, DEFAULT, PRIMARY, KEY, INSERT,
    INTO, VALUES, DELETE, FROM, WHERE, SELECT, ASSIGN, LT, GT, NEQ, EQ,
    GEQ, LEQ, PLUS, MINUS, MUL, DIV, AND, OR, NOT, L_PAREN, R_PAREN,
    COMMA, SEMICOLON, END, NONE
};

class TokenError : std::exception {
public:
    TokenError(string _msg) : msg(_msg) {}
    ~TokenError() throw () {}
    const char* what() const throw() { return msg.c_str(); }
private:
    string msg;
};

class Token {
public:
    Token(Type _type=NONE, const void *raw=NULL, const int size=0);
    Token(const Token &other);

    // number or identifier
    bool isValue(Type t) const {
        return t == NUM || t == ID;
    }

    Type getType() const {
        return type;
    }

    string getId() const;
    Type getKeyword() const;
    int getNumber() const;

    bool operator==(Type rhs) const {
        return type == rhs;
    }
    bool operator!=(Type rhs) const {
        return !(*this == rhs);
    }

    Token &operator=(const Token &rhs);
    friend ostream &operator<<(ostream &s, const Token &token);

    ~Token();

    static map<Type, string> name;


private:
    static void initNameMap(); // statically init names

    Type type; // type of the token
    char *data; // raw data
    int real_size; // size of data[]
};

}

#endif
