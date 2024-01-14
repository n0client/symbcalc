#include "node.hpp"
#include <iostream>
#include <string>
#include "tokenizer.hpp"

Node::Node() : type(t_num), num(0.0), left(nullptr), right(nullptr), visited(false) {}
Node::Node(const Token* token, Node* lhs, Node* rhs) : left(lhs), right(rhs), visited(false) {
  switch (token->type) {
  case Token::t_num:
    num = token->num;
    type = Node::t_num;
    break;
  case Token::t_op_bin:
    op_bin = token->op;
    type = Node::t_op_bin;
    break;
  case Token::t_fun_bin:
    new (&fun_bin) std::string(token->fun);
    type = Node::t_fun_bin;
    break;
  case Token::t_fun_un:
    new (&fun_un) std::string(token->fun);
    type = Node::t_fun_un;
    break;
  case Token::t_l_paren:
  case Token::t_r_paren:
    break;
  }
}
Node::Node(const Node& other) : type(other.type), left(nullptr), right(nullptr), visited(other.visited) {
  std::cout << "Copy constructor\n";
  switch (type) {
  case Node::t_num:
    num = other.num;
    break;
  case Node::t_op_bin:
    op_bin = other.op_bin;
    break;
  case Node::t_fun_bin:
    new (&fun_bin) std::string(other.fun_bin);
    break;
  case Node::t_fun_un:
    new (&fun_un) std::string(other.fun_un);
    break;
  }
  if (other.left)
    left = new Node(*other.left);
  if (other.right)
    right = new Node(*other.right);
}
void Node::clean_iter() {
/*
function deleteBinaryTreeIterative(root):
    if root is null:
        return  // Tree is already empty

    stack = new empty stack
    current = root

    while current is not null or stack is not empty:
        while current is not null:
            // Traverse to the leftmost node
            push current onto stack
            current = current.left

        // Process the current node
        current = pop from stack
        temp = current
        current = current.right  // Move to the right subtree

        // Delete the current node
        delete temp

    // The tree is now empty

*/
}
void Node::clean(Node* node) {
  if (!node || node->visited) {
    return;
  }
  if (node->type == Node::t_fun_bin)
    fun_bin.~basic_string();
  else if (node->type == Node::t_fun_un)
    fun_un.~basic_string();
  node->visited = true;
  clean(node->left);
  clean(node->right);
  delete node;
}
Node::~Node() {
  clean(root);
  delete[] container; 
}
Node::Node(std::string& inp) {
  TokenList tokens = TokenList(inp);
  tokens.rpn();
  Node** node_stack = new Node*[tokens.size()/2+1];
  size_t pos {};
  for (size_t i = 0; i < tokens.size(); ++i) {
    Token* token = tokens[i];
    switch (token->type) {
    case Token::t_num: {
      Node* num_node = new Node(token, nullptr, nullptr);
      node_stack[pos++] = num_node;
     break;
    }
    case Token::t_op_bin:
    case Token::t_fun_bin: {
      Node* bin_node = new Node(token, node_stack[pos-2], node_stack[pos-1]);
      pos -= 2;
      node_stack[pos++] = bin_node;
      break;
    }
    case Token::t_fun_un: {
      Node* un_node = new Node(token, nullptr, node_stack[--pos]);
      node_stack[pos++] = un_node;
      break;
    }
    case Token::t_l_paren:
    case Token::t_r_paren:
      break;
    }
  }
  type = node_stack[0]->type;
  if (type == Node::t_op_bin)
    op_bin = node_stack[0]->op_bin;
  else if (type == Node::t_fun_bin)
    fun_bin = node_stack[0]->fun_bin;
  else
    fun_un = node_stack[0]->fun_un; // could be a problem point b/c of else 
  left = node_stack[0]->left;
  right = node_stack[0]->right;
  container = node_stack;
  root = node_stack[0];
}
double Node::eval() {
  if (type == Node::t_num)
    return num;
  else if (type == Node::t_op_bin) {
    double lhs = left->eval();
    double rhs = right->eval();
    switch (op_bin) {
    case '+':
      return lhs + rhs;
    case '-':
      return lhs - rhs;
    case '*':
      return lhs * rhs;
    case '/':
      return lhs / rhs;
    }
  }
/*
function evaluate(node):
    if node is a number node:
        return the numeric value associated with the node
    else if node is an operator node:
        left_result = evaluate(node's left subtree)
        right_result = evaluate(node's right subtree)
        combine left_result and right_result based on the operator
        return the combined result
*/ 
}
double Node::eval_iter() {
/*
function evaluateIterative(root):
    stack = new empty stack
    push root onto stack
    
    while stack is not empty:
        current = pop from stack
        
        if current is a number node:
            push the numeric value associated with the current node onto stack
        else if current is an operator node:
            left_result = pop from stack
            right_result = pop from stack
            
            combine left_result and right_result based on the operator
            push the combined result onto stack
            
    result = pop from stack
    return result
*/
}
void Node::print() {
  switch (type) {
    case Node::t_num:
      std::cout << "Number " << num << "\n";
      return;
    case Node::t_op_bin:
      std::cout << "Bin op " << op_bin << "\n";
      break;
    case Node::t_fun_bin:
      std::cout << "Bin fun " << fun_bin << "\n";  // Corrected: n->fun_bin
      break;
    case Node::t_fun_un:
      std::cout << "Un fun " << fun_un << "\n";
      break;
  }
  left->print();
  right->print();
}

/*
Node::Node(const Node* other) : type(other->type) {
  switch (type) {
  case Node::t_num:
    num = other->num;
    break;
  case Node::t_op_bin:
    op_bin = other->op_bin;
    break;
  case Node::t_fun_bin:
    new (&fun_bin) std::string(other->fun_bin);
    break;
  case Node::t_fun_un:
    new (&fun_un) std::string(other->fun_un);
    break;
  }
  left = other->left;
  right = other->right;
}
Node& Node::operator=(const Node& other) {
  if (this != &other) {
    delete left;
    delete right;
    type = other.type;
    switch (type) {
    case Node::t_num:
      num = other.num;
      break;
    case Node::t_op_bin:
      op_bin = other.op_bin;
      break;
    case Node::t_fun_bin:
      fun_bin.~basic_string();
      new (&fun_bin) std::string(other.fun_bin);
      break;
    case Node::t_fun_un:
      fun_un.~basic_string();
      new (&fun_un) std::string(other.fun_un);
      break;
    }
  }
  return *this;
}
*/
