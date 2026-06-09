#include <iostream>
#include <vector>
using namespace std;

static constexpr size_t n=1024*1024;

//struct to represent a block of memory
struct Block{
    bool free;
    size_t size;
    Block* next;
};

int main(){
    return 0;
}