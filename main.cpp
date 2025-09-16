
#include <iostream>
#include <string>

#include "node.hpp"


int main() {
  std::string in;

  while (1)
  {
start:
    std::cout << ">> ";
    std::getline(std::cin, in);

    if (in.size() == 0) goto start;

    for (int i = 0; i < in.size(); ++i)
    {
      char c = in[i];
      if (!(c == ' ' || std::isdigit(c) || c == '/' || c == '*' || c == '+' || c == '-' || c == '^' || c == '(' || c == ')'))
      {
        std::cout << "Unknown token at position " << i + 1 << "\n";
        goto start;
      }
    }

    Node n(in);
    //n.print();
    std::cout << " = " << n.eval() << '\n';
  }
  return 0;
}

