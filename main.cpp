#include <iostream>
#include "LoopManager.h"

int main() {

    FileReader reader;
   vector<string> st = reader.readFile("test.txt");

    for (std::vector<string>::const_iterator i = st.begin(); i != st.end(); ++i)
        std::cout << *i << '\n';
}