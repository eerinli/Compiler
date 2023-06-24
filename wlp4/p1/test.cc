#include <iostream>
#include <cstdint>

int64_t *mymalloc(int64_t size);
void myfree(int64_t *address);

int main() {
    std::cout << "Some useless stuff" << std::endl;
    myfree(mymalloc(1));
    myfree(mymalloc(2));
    myfree(mymalloc(3));

    std::cout << "Small blocks" << std::endl;
    int64_t* one   = mymalloc(1);
    int64_t* two   = mymalloc(2);
    int64_t* three = mymalloc(3);

    one[0]   = 100;
    two[0]   = 200;
    three[0] = 300;

    std::cout << "More useless stuff" << std::endl;
    myfree(mymalloc(99));
    myfree(mymalloc(199));
    myfree(mymalloc(299));

    std::cout << "Free the small" << std::endl;
    myfree(three);
    myfree(two);
    myfree(one);

    std::cout << "Very useless" << std::endl;
    myfree(mymalloc(1));
    myfree(mymalloc(2));
    myfree(mymalloc(3));

    std::cout << "We did it" << std::endl;
    return 0;
}