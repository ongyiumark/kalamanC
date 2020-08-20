#include <iostream>
#include "Objects/object.h"

using namespace Objects;
int main(int argc, char ** argv)
{
    Object* val = new Integer(new int(10));
    std::cout << type_to_string(val->get_type()) << std::endl;
    std::cout << *((Integer*)val)->get_value() << std::endl;
    system("PAUSE");
    return 0;
}