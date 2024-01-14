#include <cctype> // isdigit, isalpha, tolower
#include <cassert> // assert();
#include <iostream>
#include "tokenizer.hpp"

// TOKEN
Token::Token() :  type(t_num), num(1.0) {}
Token::Token(const std::string& a_fun) 
    : type(a_fun == "max" || a_fun == "min" ? Token::t_fun_bin : Token::t_fun_un), prec(2) {
  new (&fun) std::string(a_fun);
}
Token::Token(const double& a_num) : type(Token::t_num), num(a_num), prec(0) {}
Token::Token(const char& a_op) : op(a_op) {
  switch (op) {
  case '*':
  case '/':
  case '+':
  case '-':
  case '^':
    prec = (op == '+' || op == '-') ? 1 : 2;
    type = Token::t_op_bin;
    break;
  case '(':
    prec = 0;
    type = Token::t_l_paren;
    break;
  case ')':
    prec = 0;
    type = Token::t_r_paren;
    break;
  }
}
Token::~Token() {
  if (type == Token::t_fun_bin || type == Token::t_fun_un)
    fun.~basic_string();
}
Token::Token(const Token& other) {
  prec = other.prec;
  type = other.type;
  switch (type) {
  case Token::t_num:
    num = other.num;
    break;
  case Token::t_op_bin:
  case Token::t_l_paren:
  case Token::t_r_paren:
    op = other.op;
    break;
  case Token::t_fun_bin:
  case Token::t_fun_un:
    new (&fun) std::string(other.fun);
    break;
  }
}
void Token::print() {
  switch (type) {
  case t_num:
    std::cout << "Number " << num << "\n";
    break;
  case t_op_bin:
    std::cout << "Operator " << op << "\n";
    break;
  case t_l_paren:
  case t_r_paren:
    std::cout << "Parenthesis " << op << "\n";
    break;
  case t_fun_bin:
  case t_fun_un:
    std::cout << "Function " << fun << "\n";
    break;
  }
}

// TOKENLIST
TokenList::TokenList() : max_size(4), at(0), p_toks(new Token*[4]) {}
TokenList::TokenList(std::string& s) : max_size(4), at(0), p_toks(new Token*[4]) {
  parse(s);
}
TokenList::~TokenList() {
  clean();
}
TokenList::TokenList(const TokenList& other) {
  clean();
  init(other);
}
TokenList& TokenList::operator=(const TokenList& other) noexcept {
  if (this != &other) {
    clean();
    init(other);
  }
  return *this;
}
void TokenList::init(const TokenList& other) {
  max_size = other.max_size;
  at = other.at;
  p_toks = new Token*[max_size];
  for (size_t i = 0; i < other.at; ++i)
    p_toks[i] = new Token(*other[i]);
}
void TokenList::clean() {
  for (size_t i = 0; i < at; ++i)
    delete p_toks[i];
  delete[] p_toks;
}
Token* TokenList::operator[](const size_t& i) const {
  if (i >= 0 && i < at)
    return p_toks[i];
  return nullptr;
}
size_t TokenList::size() const { return at; }
Token* TokenList::back() const {
  if (at == 0)
    return nullptr;
  return p_toks[at-1]; 
}
void TokenList::push_back(Token* tok) {
  if (at == max_size)
    resize();
  p_toks[at++] = new Token(*tok);
}
template <typename T>
void TokenList::push_back(const T& in) {
  if (at == max_size)
    resize();
  p_toks[at++] = new Token(in);
}
void TokenList::pop_back() {
  if (at == 0) {
    std::cout << "Popback empty\n";
    return;  
  }
  delete p_toks[--at];
}
void TokenList::print() {
  for (size_t i = 0; i < at; ++i)
    p_toks[i]->print();
}
void TokenList::resize() {
  max_size *= 2;
  Token** temp = new Token*[max_size];
  for (size_t i = 0; i < at; ++i) {
    temp[i] = new Token(*p_toks[i]);
    delete p_toks[i];
  }
  delete[] p_toks;
  p_toks = temp;
}
void TokenList::parse(std::string& s) {
  // add bogus character to simplify lookahead checks
  s += '\0';
  char op {'\0'};
  std::string fun {};
  double num {}, frac { 0.1 };
  bool collecting_num {}, collecting_dec {};
  auto isfun = [&](const std::string& inp) {
    return inp == "max" || inp == "min" || inp == "sin" || inp == "cos" || inp == "tan" || inp == "sqrt";
  };
  for (size_t i = 0; i < s.size()-1; ++i) {
    const char c = s[i];

    if (c == '.') {
      collecting_dec = true;
      collecting_num = true;
    }
    else if (collecting_dec && std::isdigit(c)) {
      num = num + frac * (c - '0');
      frac *= 0.1;
    }
    else if (std::isdigit(c)) {
      num = num * 10 + (c - '0');
      collecting_num = true;
    }
    else if (c == '*' || c == '/' || c == '+' || c == '-' || c == '^' ||  c== '(' || c == ')') {
      op = c;
    }
    else if (std::isalpha(c)) {
      fun += std::tolower(c);
    }

    if ((c == ' ') || (collecting_num && !std::isdigit(s[i+1]) && s[i+1] != '.') || (op != '\0') || (isfun(fun))) {
      if (collecting_num) {
        this->push_back(num);
        //std::cout << "It is a Num " << num << "\n";
      }
      else if (op != '\0') { 
        this->push_back(op);
        //std::cout << "It is a OP " << op << "\n";
      }
      else if (isfun(fun)) {
        this->push_back(fun);
        //std::cout << "It is a FUN " << fun << "\n";
      }
      else if (c == ' ')
        continue;
        //std::cout << "fear not, tis a space\n";
      else {
        assert(false);
        //std::cout << "Uknown condition??\n";
        //std::cout << "Frac " << frac << "\nNum " << num << "\nFun " << fun << "\nop " << op << "\n";
      }
      collecting_num = collecting_dec = false;
      frac = 0.1;
      num = 0.0;
      fun = "";
      op = '\0';
    }
  }
  s.pop_back();
}
void TokenList::rpn() {
  TokenList* out_queue = new TokenList();
  TokenList* op_stack = new TokenList();
  for (size_t i = 0; i < size(); ++i) {
    Token* t = p_toks[i];
    switch (t->type) {
    case Token::t_num:
      out_queue->push_back(t);
      break;
    case Token::t_fun_bin:
    case Token::t_fun_un:
      op_stack->push_back(t);
      break;
    case Token::t_op_bin:
      while (op_stack->size() > 0 && op_stack->back()->op != '(' &&
          (op_stack->back()->prec > t->prec || (op_stack->back()->prec == t->prec && t->op != '^'))) {
        out_queue->push_back(op_stack->back());
        op_stack->pop_back();
      }
      op_stack->push_back(t);
      break;
    case Token::t_l_paren:
      op_stack->push_back(t);
      break;
    case Token::t_r_paren:
      while (op_stack->back()->op != '(') {
        assert(op_stack->size() != 0); // if fails them mismatched parentheses
        out_queue->push_back(op_stack->back());
        op_stack->pop_back();
      }
      assert(op_stack->back()->op == '(');
      op_stack->pop_back();
      if (op_stack->back()->type == Token::t_fun_bin || op_stack->back()->type == Token::t_fun_un) {
        out_queue->push_back(op_stack->back());
        op_stack->pop_back();
      }
      break;
    }
  }
  while (op_stack->size() > 0) {
    assert(op_stack->back()->op != '('); // if fails then mismatched parentheses
    out_queue->push_back(op_stack->back());
    op_stack->pop_back();
  }
  delete op_stack;
  *this = *out_queue;
  delete out_queue;
}
