#ifndef VM_H
#define VM_H

#include "main.h"

class myData{
  public:
    int dataInt{0};
    double dataFloat{0};
    bool dataBool{0};
    int indexAddress{0};
    int dataType{-1};
  void operator=(const myData& t);
  myData operator+(const myData& t);
  myData operator-(const myData& t);
  myData operator*(const myData& t);
  myData operator/(const myData& t);
  bool operator==(const myData& t);
  bool operator<(const myData& t);
  bool operator<=(const myData& t);
  void clear();
};
class VM
{
  private:
  string insMem[66536]; //Declare Instruction Memory
  int insMemlength{0};
  myData Registor[16];
  int ipReg{0};
  myData staticMem[66536];

  public:
  VM()= default;
  void run(string filename);              //main function
  void readFile(string filename);         //read the file
  bool checkValidIns(string s);            //check the validity of Instruction
  bool checkValidOp(string s);            //check the validity of Operand
  void doInstruction();                 
  string seperateIns(string s);           //convert the instruction from string
  string seperateReg(string s);           //convert the registor from string
  int getRegPos(string s);                //convert the "Rx" in to number
  myData convertLast(string s);           //convert the second operand
  void loadStoreIns(string ins, int dest, myData src, int ip);
  void ArithmeticIns(string ins, int dest, myData src, int ip);
  void ComparisonIns(string ins, int dest, myData src, int ip);
  void LogicIns(string ins, int dest, myData src, int ip);
  void InOutIns(string ins, string temp, int ip);
  int getInsNum(string s);
};
#endif
