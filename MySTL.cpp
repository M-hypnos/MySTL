﻿
#include <iostream>
#include <vld.h>
#include "test/vectorTest.h"
#include "test/listTest.h"
#include "test/dequeTest.h"
#include "test/slistTest.h"
#include "test/queueTest.h"
#include "test/heapTest.h"
#include "test/rbTreeTest.h"
#include "test/mapTest.h"

int main()
{
    std::cout << "Hello World!\n";
    mySTL::vectorTest::test();
    mySTL::listTest::test();
    mySTL::dequeTest::test();
    mySTL::slistTest::test();
    mySTL::queueTest::test();
    mySTL::heapTest::test();
    mySTL::rbTreeTest::test();
    mySTL::mapTest::test();
}
