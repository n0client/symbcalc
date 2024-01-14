#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <cstddef>
#include <string>

struct TokenList;

struct Token {
private:
  enum {t_num, t_op_bin, t_fun_bin, t_fun_un, t_l_paren, t_r_paren} type;
  union { 
    std::string fun;// can use wrapper class here (another struct) non-trivial
    double num; // trivial
    char op; // trivial
  };
  int prec;
  friend struct TokenList;
  friend struct Node;

public:
  Token();
  Token(const std::string& a_fun);
  Token(const double& a_num);
  Token(const char& a_op);
  ~Token();
  Token(const Token& other);
  void print();
};

struct TokenList {
  TokenList();
  TokenList(std::string& s);
  ~TokenList();
  TokenList(const TokenList& other);
  TokenList& operator=(const TokenList& other) noexcept;
  Token* operator[](const size_t& i) const;
  size_t size() const;
  Token* back() const;
  void push_back(Token* tok);
  template <typename T> void push_back(const T& inp);
  void pop_back();
  void print();
  void rpn();

private:
  size_t max_size;
  size_t at;
  Token** p_toks;
  void init(const TokenList& other);
  void clean();
  void resize();
  void parse(std::string& s);
};

#endif /* TOKENIZER_H */
