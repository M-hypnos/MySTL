
#include <iostream>
#include <vld.h>
#include "test/vectorTest.h"
#include "test/listTest.h"
#include "test/dequeTest.h"

int main()
{
    std::cout << "Hello World!\n";
    mySTL::vectorTest::test();
    mySTL::listTest::test();
    mySTL::dequeTest::test();
}
