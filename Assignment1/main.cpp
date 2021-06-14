#include "VM.h"
#include "VM.cpp"



using namespace std;

void test(string filename) {
    VM *vm = new VM();
    try {
        vm->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete vm;
}

int main(int argc, char **argv)
{
  clock_t tStart = clock();
    test("prog1.txt");
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;

  


  
  return 0;
}