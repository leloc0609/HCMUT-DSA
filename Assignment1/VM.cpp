#include "VM.h"
//definition
#define Arithmetic 	1
#define	Comparison 	2
#define	Logical		3
#define	LoadStore	4
#define	SeqControl	5
#define	IO			6

#define boolT 	1
#define floatT 	2
#define intT 	3
#define addressT 4
//-------------FILE READING-----------------------------------
void VM::readFile(string filename) //correct
{ifstream ifs;
	ifs.open(filename);
	int n = 0;
	while (!ifs.eof()) {
		char tmp[256];
		ifs.getline(tmp, 256, '\n');
		if (checkValidIns(tmp))
		{
			if (checkValidOp(tmp)){
					insMem[n]= tmp;
			}
			else
			{
				throw InvalidOperand(n);
			}
		}	 	
		else throw InvalidInstruction(n);
		insMem[n]=tmp;
		n++;
	}
	insMemlength=n;
	ifs.close();
}
//-------------VALID OPERAND CHECK----------------------------
bool VM::checkValidOp(string s){
	int insType = checkValidIns(s);
	string ins= seperateIns(s);
	//Halt and Return exception-------------------------------------------
	if (ins=="Halt"){
		if (s.length()!=4)
			return false;
        else    
            return true;
	}
	if (ins=="Return"){
		if (s.length()!=6)
			return false;
        else 
            return true;
	}
	//----------------------------------------------------------------------
	regex floatType("\\d+\\.\\d+");
	regex boolType("(true|false)");
	regex intType("[^0]\\d+");
	regex addType("\\d+A");
	regex regType("R1|R2|R3|R4|R5|R6|R7|R8|R9|R10|R11|R12|R13|R14|R15");
	string reg = seperateReg(s);
    //missing jump and call 
	if (ins=="Jump"||ins=="Call"||ins=="Output"){
		if (regex_match(reg, floatType)||	regex_match(reg, boolType)|| regex_match(reg, intType)|| regex_match(reg, addType)|| regex_match(reg, regType))
			return true;
		else	
			return false;
	}
	if (ins=="Input")
	{
		if(regex_match(reg, regType))
			return true;
		else 	
			return false;
	}
	string temp2;
	if(s.find(",")!= std::string::npos){	//check if instruction have second operand.
		size_t colonPos= s.find(",");	
    	temp2= s.substr(colonPos+2); // take the rest operand to convert it.
	}
	if (regex_match(reg, regType))
    {
        if (ins=="Not") return true;
		if (insType==Arithmetic || insType==Comparison || ins=="And" || ins=="Or" || insType==LoadStore || ins=="JumpIf")
	    {	if (regex_match(temp2, floatType)||	regex_match(temp2, boolType)|| regex_match(temp2, intType)|| regex_match(temp2, addType)|| regex_match(temp2, regType))
				return true;
			else 
                return false;
        }
		else 
            return false;	
    }
	else return false;
}
//-------------VALID INSTRUCTION CHECK------------------------
bool VM:: checkValidIns(string s){ 
	regex arith			("(Add|Minus|Mul|Div)\\s[^\\s]+,\\s[^\\s]+");
	regex compar		("(CmpEQ|CmpNE|CmpLT|CmpLE|CmpGT|CmpGE)\\s[^\\s]+,\\s[^\\s]+");
	regex logic			("(Not\\s[^\\s]+)|((And|Or)\\s[^\\s]+,\\s[^\\s]+)");
	regex loadandstore	("(Move|Load|Store)\\s[^\\s]+,\\s[^\\s]+");
	regex control		("((Jump|Call)\\s[^\\s]+)|(JumpIf\\s[^\\s]+,\\s[^\\s]+)|Return|Halt");
	regex inout 		("(Input|Output)\\s[^\\s]+");

	string temp =seperateIns(s);
	if (temp=="Add" || temp=="Minus" ||temp=="Mul"||temp=="Div"){
		if(regex_match(s, arith))
		{
			return true;
		}
	}
	if (temp=="CmpEQ"|| temp=="CmpNE" || temp=="CmpLT"|| temp=="CmpLE" || temp=="CmpGT"|| temp=="CmpGE")
	{
		if (regex_match(s,compar)){
		return true;
		}
	}
	if(temp=="Not" || temp=="And" || temp=="Or"){
		if (regex_match(s,logic)){
		return true;
		}
	}
	if (temp=="Jump"|| temp=="Call" || temp=="JumpIf" || temp=="Return" || temp=="Halt"){
		if (regex_match(s, control)){
		return true;
		}	
	}
	if (temp=="Move"|| temp=="Load" || temp=="Store"){
		if (regex_match(s,loadandstore)){
		return true;
		}
	}
	if (temp=="Input"||temp=="Output"){
		if(regex_match(s,inout)){
		return true;
		}
	}
	return false;
}
//--------------INSTRUCTION GET-------------------------------
string VM::seperateIns(string s)// correct
{
	size_t spacePos= s.find(" ");
	string temp= s.substr(0,spacePos);
	return temp;
}
//--------------REGISTOR GET----------------------------------
string VM ::seperateReg(string s){
	size_t spacePos= s.find(" ");
	string temp{};
	if (s.find(",")!=std::string::npos){
		size_t colonPos= s.find(",");
        temp= s.substr(spacePos+1,colonPos-spacePos-1);
	}
	else	
	{
		temp= s.substr(spacePos+1);
	}
	return temp;
}
//--------------TURN REG INDEX INTO INT-----------------------
int VM:: getRegPos(string temp)// correct
{
	if (temp == "R1")
		return 1;
	if (temp == "R2")
		return 2;
	if (temp == "R3")
		return 3;
	if (temp == "R4")
		return 4;
	if (temp == "R5")
		return 5;
	if (temp == "R6")
		return 6;
	if (temp == "R7")
		return 7;
	if (temp == "R8")
		return 8;
	if (temp == "R9")
		return 9;
	if (temp == "R10")
		return 10;
	if (temp == "R11")
		return 11;
	if (temp == "R12")
		return 12;
	if (temp == "R13")
		return 13;
	if (temp == "R14")
		return 14;
	if (temp == "R15")
		return 15;
	return -1;
}
//--------------GET 2ND OPERAND-------------------------------
myData VM:: convertLast(string temp){
	myData temp2;
	if(temp.find("R")==std::string::npos){
		if(temp.find("A")==std::string::npos){
			if (temp=="true"||temp=="false")
			{
				temp2.dataType= boolT;
				if (temp=="true") temp2.dataBool= true;
				else temp2.dataBool= false;
			}
			else if (temp.find("."))
			{
				temp2.dataType= floatT;
				temp2.dataFloat= stod(temp);
			}
			else if (temp.find(".")==std::string::npos){
				temp2.dataInt= stoi(temp);
				temp2.dataType= intT;
			}
		}
		else{
				temp.pop_back();
				temp2.dataType= addressT;
				temp2.indexAddress= stoi(temp);
			}
	}
	else {
		int pos= getRegPos(temp);
		temp2= Registor[pos];
	}
	return temp2;
}
//Load & Store Instructions only done the move
void VM:: loadStoreIns(string ins, int dest, myData src, int ip){
	if(ins=="Move")
	{
		Registor[dest]= src;
		return;
	}
	if(ins=="Store"){
		if(Registor[dest].dataType!=addressT) {throw TypeMismatch(ip);};
		staticMem[Registor[dest].indexAddress]=src;
		return;
	}
	if(ins=="Load"){
		if(src.dataType!=addressT) {throw TypeMismatch(ip);};
		Registor[dest]=staticMem[src.indexAddress];
	}
}

void VM:: ArithmeticIns(string ins, int dest, myData src, int ip){
	if (src.dataType==addressT || src.dataType==boolT|| Registor[dest].dataType==addressT || Registor[dest].dataType==boolT){
		throw TypeMismatch(ip);
	}
	else
	{
		if (ins=="Add")
			Registor[dest]= Registor[dest]+src;
		else if (ins=="Minus")
			Registor[dest]= Registor[dest]-src;
		else if (ins=="Mul")
			Registor[dest]= Registor[dest]*src;
		else if (ins=="Div")
		{
			if ((src.dataType==floatT && src.dataFloat==0)||(src.dataInt==0 && src.dataType==intT))
				throw DivideByZero(ip);
			else
				Registor[dest]= Registor[dest]/src;
		}
			
	}
}

void VM:: ComparisonIns(string ins, int dest, myData src, int ip){
	if((Registor[dest].dataType==boolT && src.dataType!=boolT) || (Registor[dest].dataType!=boolT && src.dataType==boolT) || Registor[dest].dataType==addressT || src.dataType==addressT)
		throw TypeMismatch(ip);
	
	if (ins=="CmpEQ"){
		if(Registor[dest]==src)
		{
			Registor[dest].clear();
			Registor[dest].dataBool=1;
			Registor[dest].dataType=boolT;
		}
		else
		{
			Registor[dest].clear();
			Registor[dest].dataBool=0;
			Registor[dest].dataType=boolT;
		}
		return;
			
	}
	if (ins=="CmpNE"){
		if(Registor[dest]==src)
		{
			Registor[dest].clear();
			Registor[dest].dataBool=0;
			Registor[dest].dataType=boolT;
		}
		else
		{
			Registor[dest].clear();
			Registor[dest].dataBool=1;
			Registor[dest].dataType=boolT;
		}
		return;
	}
	if (Registor[dest].dataType==boolT && src.dataType==boolT){
		throw TypeMismatch(ip);
	}
	if (ins=="CmpLT"){
		if(Registor[dest]<src)
		{
			Registor[dest].clear();
			Registor[dest].dataBool=1;
			Registor[dest].dataType=boolT;
		}
		else
		{
			Registor[dest].clear();
			Registor[dest].dataBool=0;
			Registor[dest].dataType=boolT;
		}
		return;
	}
	if (ins=="CmpGE"){
		if(Registor[dest]<src)
		{
			Registor[dest].clear();
			Registor[dest].dataBool=0;
			Registor[dest].dataType=boolT;
		}
		else
		{
			Registor[dest].clear();
			Registor[dest].dataBool=1;
			Registor[dest].dataType=boolT;
		}
		return;
	}
	if (ins=="CmpLE"){
		if(Registor[dest]<=src)
		{
			Registor[dest].clear();
			Registor[dest].dataBool=1;
			Registor[dest].dataType=boolT;
		}
		else
		{
			Registor[dest].clear();
			Registor[dest].dataBool=0;
			Registor[dest].dataType=boolT;
		}
		return;
	}
	if (ins=="CmpGT"){
		if(Registor[dest]<=src)
		{
			Registor[dest].clear();
			Registor[dest].dataBool=0;
			Registor[dest].dataType=boolT;
		}
		else
		{
			Registor[dest].clear();
			Registor[dest].dataBool=1;
			Registor[dest].dataType=boolT;
		}
		return;
	}
}

void VM:: LogicIns(string ins, int dest, myData src, int ip){
	if(Registor[dest].dataType!=boolT)
		throw TypeMismatch(ip);
	if(ins=="Not"){
		Registor[dest].dataBool=!Registor[dest].dataBool;
		return;
	}
	if (ins=="And"){
		Registor[dest].dataBool=Registor[dest].dataBool && src.dataBool;
		return;
	}
	if (ins=="Or"){
		Registor[dest].dataBool=Registor[dest].dataBool || src.dataBool;
		return;
	}
}

void VM:: InOutIns(string ins, string temp, int ip){
	regex regis("R\\d+");
	if (ins=="Output"){
		if (regex_match(temp, regis))
		{
			int pos= getRegPos(temp);
			if (Registor[pos].dataType==boolT){
				if (Registor[pos].dataBool)
					cout << "true";
				else
					cout <<"false";
			}
			else if (Registor[pos].dataType==floatT){
				cout << Registor[pos].dataFloat;
			}
			else if (Registor[pos].dataType==intT){
				cout << Registor[pos].dataInt;
			}
			else if (Registor[pos].dataType==addressT){
				cout <<Registor[pos].indexAddress<<"A";
			}
		}
		else
		{
				cout <<temp;
		}
	}
	if(ins=="Input"){
		int pos= getRegPos(temp);
		regex floatType("\\d+\\.\\d+");
		regex boolType("(true|false)");
		regex intType("\\d+");
		regex addType("\\d+A");
		string in;
		getline(cin, in);
		if (regex_match(in, floatType)|| regex_match(in, boolType)|| regex_match(in, addType)|| regex_match(in, intType)){
			myData temp2=convertLast(in);
			Registor[pos]= temp2;
		}
		else 
			throw InvalidInput(ip);
	}
}

int VM:: getInsNum(string s){
	string temp= seperateIns(s);
	if (temp=="Add" || temp=="Minus" ||temp=="Mul"||temp=="Div"){
			return Arithmetic;
	}
	if (temp=="CmpEQ"|| temp=="CmpNE" || temp=="CmpLT"|| temp=="CmpLE" || temp=="CmpGT"|| temp=="CmpGE")
	{
		return Comparison;
	}
	if(temp=="Not" || temp=="And" || temp=="Or"){
		return Logical;
	}
	if (temp=="Jump"|| temp=="Call" || temp=="JumpIf" || temp=="Return" || temp=="Halt"){
		return SeqControl;
	}
	if (temp=="Move"|| temp=="Load" || temp=="Store"){
		return LoadStore;
	}
	if (temp=="Input"||temp=="Output"){
		return IO;
	}
	return 0;
}

void VM::doInstruction(){
	string temp;
	vector<int> myStack;
	myData scr;
	int i=0;
	while (i< insMemlength){
		if (insMem[i].find(",")!=std::string::npos)
		{
			size_t colonPos= insMem[i].find(',');
			temp= insMem[i].substr(colonPos+2);
			scr= convertLast(temp);
		}
		int dest= getRegPos(seperateReg(insMem[i]));
		string ins= seperateIns(insMem[i]);
		int insType=-1;
		insType= getInsNum(insMem[i]);
		string temp=seperateReg(insMem[i]);
		if (ins=="Halt") break;
		switch (insType)
		{
		case Arithmetic:
			ArithmeticIns(ins, dest, scr, i);
			break;
		case Comparison:
			ComparisonIns(ins, dest, scr, i);
			break;
		case Logical:
			LogicIns(ins, dest, scr, i);
			break;
		case LoadStore:
			loadStoreIns(ins, dest, scr, i);
			break;
		case IO:
			InOutIns(ins, temp, i);
			break;	
		case SeqControl:
			if (ins== "Jump")
			{
				myData pos= convertLast(temp);
				if (pos.dataType!=addressT)
				{
					throw TypeMismatch(i);
				}
				if(pos.indexAddress>insMemlength)
				{
					throw InvalidDestination(i);
				}
				i=pos.indexAddress-1;
				
			}
			if (ins== "JumpIf")
			{
				if (Registor[dest].dataType== boolT){
					if (Registor[dest].dataBool)
					{
						if (scr.dataType!=addressT || Registor[dest].dataType!=boolT)
						{
							throw TypeMismatch(i);
						}
						if(scr.indexAddress>insMemlength)
						{
							throw InvalidDestination(i);
						}
						i= scr.indexAddress-1;
					}
				}
				
				
			}	
			if (ins=="Call")
			{
				myData pos= convertLast(temp);
				if (pos.dataType!=addressT)
				{
					throw TypeMismatch(i);
				}
				if(pos.indexAddress>insMemlength)
				{
					throw InvalidDestination(i);
				}
				myStack.push_back(i);
				if (myStack.size()>1000)
					throw StackFull(i);
				i= pos.indexAddress-1;
			}
			if (ins=="Return")
			{
				i=myStack.back();
				myStack.pop_back();
			}
			break;
		default:
			break;
		}
		++i;
	}
	myStack.clear();	
}

void VM::run(string filename)
{
   readFile(filename);
   doInstruction();

}

void myData:: operator=(const myData& t){
	this->dataBool=t.dataBool;
	this->dataFloat=t.dataFloat;
	this->dataInt=t.dataInt;
	this->indexAddress=t.indexAddress;
	this->dataType=t.dataType;
}

myData myData:: operator+(const myData& t){
	myData temp;
	double temp1{0};
	if (this->dataType==intT && t.dataType==intT){
		temp.dataInt= this->dataInt+t.dataInt;
		temp.dataType=intT;
	} 
	else if (this->dataType==floatT && t.dataType==floatT){
		temp.dataFloat=this->dataFloat+t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==intT && t.dataType==floatT)
	{
		temp1=this->dataInt;
		temp.dataFloat=temp1+ t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==floatT && t.dataType==intT)
	{
		temp1=t.dataInt;
		temp.dataFloat=this->dataFloat + temp1;
		temp.dataType=floatT;
	}
	return temp;
	
}

myData myData:: operator-(const myData& t){
	myData temp;
	if (this->dataType==intT && t.dataType==intT){
		temp.dataInt= this->dataInt-t.dataInt;
		temp.dataType=intT;
	} 
	else if (this->dataType==floatT && t.dataType==floatT){
		temp.dataFloat= this->dataFloat-t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==intT && t.dataType==floatT)
	{
		temp.dataFloat=this->dataInt-t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==floatT && t.dataType==intT)
	{
		temp.dataFloat=this->dataFloat-t.dataInt;
		temp.dataType=floatT;
	}
	return temp;
	
}

myData myData:: operator*(const myData& t){
	myData temp;
	if (this->dataType==intT && t.dataType==intT){
		temp.dataInt= this->dataInt*t.dataInt;
		temp.dataType=intT;
	} 
	else if (this->dataType==floatT && t.dataType==floatT){
		temp.dataFloat= this->dataFloat*t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==intT && t.dataType==floatT)
	{
		temp.dataFloat=this->dataInt*t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==floatT && t.dataType==intT)
	{
		temp.dataFloat=this->dataFloat*t.dataInt;
		temp.dataType=floatT;
	}
	return temp;
}

myData myData:: operator/(const myData& t){
	myData temp;
	if (this->dataType==intT && t.dataType==intT){
		temp.dataInt= this->dataInt/t.dataInt;
		temp.dataType=intT;
	} 
	else if (this->dataType==floatT && t.dataType==floatT){
		temp.dataFloat= this->dataFloat/t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==intT && t.dataType==floatT)
	{
		temp.dataFloat=this->dataInt/t.dataFloat;
		temp.dataType=floatT;
	}
	else if (this->dataType==floatT && t.dataType==intT)
	{
		temp.dataFloat=this->dataFloat/t.dataInt;
		temp.dataType=floatT;
	}
	return temp;
	
}

bool myData:: operator==(const myData& t){
	double temp1{0};
	double temp2{0};
	if(this->dataType==boolT && t.dataType==boolT){
		if(this->dataBool==t.dataBool)
			return true;
		else	
			return false;
	}
	if (this->dataType==intT){
		temp1= this->dataInt;
	}
	else	
		temp1= this->dataFloat;
	
	if (t.dataType==intT){
		temp2= t.dataInt;
	}
	else	
		temp2= t.dataFloat;
	
	if (temp1==temp2)
		return true;
	else	
		return false;
	return false;
}

bool myData:: operator<(const myData& t){
	double temp1{0};
	double temp2{0};
	if(this->dataType==boolT && t.dataType==boolT){
		if(this->dataBool<t.dataBool)
			return true;
		else	
			return false;
	}
	if (this->dataType==intT){
		temp1= this->dataInt;
	}
	else	
		temp1= this->dataFloat;
	
	if (t.dataType==intT){
		temp2= t.dataInt;
	}
	else	
		temp2= t.dataFloat;
	
	if (temp1<temp2)
		return true;
	else	
		return false;
	return false;	
}

bool myData:: operator<=(const myData& t){
	double temp1{0};
	double temp2{0};
	if(this->dataType==boolT && t.dataType==boolT)
	{
		if(this->dataBool<=t.dataBool)
			return true;
		else	
			return false;
	}
	
	if (this->dataType==intT){
		temp1= this->dataInt;
	}
	else	
		temp1= this->dataFloat;
	
	if (t.dataType==intT){
		temp2= t.dataInt;
	}
	else	
		temp2= t.dataFloat;
	
	if (temp1<=temp2)
		return true;
	else	
		return false;
	return false;	
}
void myData::clear(){
	dataInt=0;
    dataFloat=0;
    dataBool=0;
	indexAddress=0;
	dataType=-1;
}