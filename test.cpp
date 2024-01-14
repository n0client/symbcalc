#include <iostream>
#include "node.hpp"
#include <string>


int main() {
  //  3 * 4 * 5/(1 - 5)
  std::string in = "3 + 4 * 5/(1 - 5)";
  Node n(in);
  n.print();
  std::cout << n.eval() << '\n';
  return 0;
}

/*
  Token n = Token();
  Token y = Token();
  Token x = Token(); 
  Token z = Token();
 Token f = Token();

  TokenList list = TokenList();
  list.push_back(&n);
  list.push_back(&y);

  for (size_t i = 0; i < list.size(); ++i)
    std::cout << list[i] << '\n';
  std::cout << '\n';

  list.push_back(&x);
  list.push_back(&z);
  list.push_back(&f);

  for (size_t i = 0; i < list.size(); ++i)
    std::cout << list[i] << '\n';
  std::cout << '\n';

  std::cout << list.size() << '\n';
  std::cout << list.back() << '\n';
  list.pop_back();
  std::cout << list.size() << '\n';
*/
