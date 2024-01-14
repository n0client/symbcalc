#ifndef NODE_H
#define NODE_H

#include "tokenizer.hpp"
#include <string>

struct Node {
  enum {t_num, t_op_bin, t_fun_bin, t_fun_un} type;
  union {
    char op_bin;
    double num;
    std::string fun_bin;
    std::string fun_un; // consider removing
  };
  Node* left;
  Node* right;
  Node* root;
  Node** container;
  bool visited;

  Node();
  Node(std::string& inp); // creates ast
  Node(const Token* token, Node* lhs, Node* rhs); // token constructor
  Node(const Node* other); // shallow copy useless
  Node(const Node& other); // deep copy useless
  Node& operator=(const Node& other); // useless
  ~Node(); 
  void clean(Node* n);
  void clean_iter();
  double eval();
  double eval_iter();

  void print();
};

#endif /* NODE_H */
