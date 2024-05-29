#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <format>
#include <string>
#include <memory>
#include "yaml.h"

enum NodeTypes
{
};

template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
  if (size_s <= 0)
    throw std::runtime_error("Error during formatting.");
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1);
}

std::string tokenToString(TOKENS type)
{
  switch (type)
  {
  case VALUE:
    return "VALUE";
  case STRING:
    return "STRING";
  case COLON:
    return "COLON";
  case DASH:
    return "DASH";
  case END:
    return "END";
  }
}

Token::Token(TOKENS type, std::string value, std::string content)
{
  this->type = type;
  this->value = value;
  this->content = content;
}

void Lexer::tokenize(std::string yaml)
{
  this->current = 0;
  this->yaml = yaml;
  while (peek() != '\0')
    scanToken();
  newToken(END, "\0", "\0");
}

void Lexer::newToken(TOKENS type, std::string value, std::string content)
{
  tokens.push_back(Token(type, value, content));
}

char Lexer::peek()
{
  if (current >= yaml.length())
    return '\0';
  return yaml[current];
}

char Lexer::peekNext()
{
  if (current + 1 >= yaml.length())
    return '\0';
  return yaml[current + 1];
}

char Lexer::advance(char match)
{
  if (current >= yaml.length())
    return '\0';
  else if (peek() == match)
    return yaml[current++];
  return '\0';
}

bool Lexer::identifier(char chr)
{
  if (isalpha(chr) || chr == '_' || chr == ' ')
    return true;
  return false;
}

void Lexer::scanToken()
{
  char curr = advance(peek());
  switch (curr)
  {
  case ':':
    newToken(COLON, std::string(1, curr), NULL);
    break;
  case '-':
    newToken(DASH, std::string("-"), std::string("-"));
    break;
  case '#':
    while (peek() != '\n')
      current++;
    break;
  case ' ':
  case '\n':
    break;
  default:
    if (identifier(curr))
    {
      // Generic value
      std::string value(1, curr);
      while (identifier(peek()))
        value.push_back(advance(peek()));
      newToken(VALUE, value, value);
    }
  }
}

Token Parser::peek()
{
  return tokens.at(current);
}

Token Parser::peekNext()
{
  return tokens.at(current + 1);
}

Token Parser::eat(TOKENS type)
{
  if (peek().type == type)
    return tokens[current++];
  throw std::invalid_argument("Expected TODO");
}

std::string Node::toString()
{
  // Convert to string
  std::string nodes;
  for (auto i = 0; i < values.size(); i++)
  {
    Node value = values.at(i);
    nodes += value.toString();
    if (i != values.size() - 1)
      nodes += ", ";
  }
  if (key.size())
  {
    // TODO: Determine key type
    return '"' + key + '"' + nodes;
  }
  return "[" + nodes + "]";
}

Node Parser::scalar()
{
  switch (peek().type)
  {
  case VALUE:
  default:
    Token value = eat(VALUE);
    return Node(value.value);
  }
}

Node Parser::sequence()
{
  // Work out way down, start by parsing a sequence of scalars
  switch (peek().type)
  {
  case DASH:
  {
    // Start by parsing a sequence of scalars
    Node sequence;
    while (peek().type == DASH)
    {
      eat(DASH);
      sequence.values.push_back(scalar());
    }
    return sequence;
  }
  default:
    if (peekNext().type == COLON)
    {
      // key: value pair
    }
  }
}

void Parser::parse(std::vector<Token> tokens)
{
  this->tokens = tokens;
  while (peek().type != END)
  {
    // Parse until we reach END token
    ast.push_back(sequence());
  }
}

YAML::YAML(const char *path)
{
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    // Open file
    file.open(path);
    std::stringstream fileStream;

    fileStream << file.rdbuf();

    file.close();

    yaml = fileStream.str();
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "Error: Unable to open file at " << path << std::endl;
  }
}

void YAML::process()
{
  // Start by running through lexer
  lexer.tokenize(yaml);
  // Final step: take tokens and turn into tree
  parser.parse(lexer.tokens);
}