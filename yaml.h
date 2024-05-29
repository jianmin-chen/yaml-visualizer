#ifndef YAML_H
#define YAML_H

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

enum TOKENS
{
  VALUE,
  STRING,
  COLON,
  DASH,
  END
};

std::string tokenToString(TOKENS type);

class Token
{
public:
  TOKENS type;
  std::string value;
  std::string content;

  Token(TOKENS type, std::string value, std::string content);

  friend std::ostream &operator<<(std::ostream &os, const Token &token)
  {
    os << "{ \"type\": \"" << tokenToString(token.type) << "\", \"value\": \"" << token.value << "\" }";
    return os;
  }
};

class Lexer
{
public:
  std::vector<Token> tokens;
  std::string yaml;

  void tokenize(std::string yaml);

private:
  int current = 0;

  void newToken(TOKENS type, std::string value, std::string content);

  char peek();
  char peekNext();

  char advance(char match);

  bool identifier(char chr);

  void scanToken();
};

class Node
{
public:
  std::string key;
  std::vector<Node> values;

  Node() {}

  Node(std::string key)
  {
    this->key = key;
  }

  std::string toString();
};

class Parser
{
public:
  std::vector<Token> tokens;
  std::vector<Node> ast;

  void parse(std::vector<Token> tokens);

private:
  int current = 0;

  Token eat(TOKENS type);

  Node scalar();

  Node sequence();

  Token peek();

  Token peekNext();
};

class YAML
{
public:
  std::string yaml;
  Lexer lexer;
  Parser parser;

  YAML(const char *path);

  void process();
};

#endif