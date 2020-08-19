#include "test.h"

#include <iostream>
int main()
{
    while(true)
    {
        printf("> ");
        std::string s;
        std::cin >> s;
        sayhi();
    }    
}