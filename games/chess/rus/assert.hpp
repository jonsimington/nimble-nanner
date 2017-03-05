#include <iostream>

#define RUS_ASSERT_EQ(a, b) { \
    auto myA = (a); \
    auto myB = (b); \
    if(myA != myB ) { \
        std::cerr << "ASSERTION FAILED: Not Equal" << std::endl; \
        std::cerr << "    " << #a << ": " << myA << std::endl; \
        std::cerr << "    " << #b << ": " << myB << std::endl; \
    } \
}

