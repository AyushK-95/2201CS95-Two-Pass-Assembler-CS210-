/*
TITLE: Emulator                                                                                                                           
AUTHOR: Ayush Kumar
ROLL NUMBER: 2201CS95
Declaration of Authorship
This C++ file, asm.cpp, is part of the assignment of CS209 & CS210 at the department of Computer Science and Engineering, IIT Patna. 
*/

#include <bits/stdc++.h>
using namespace std;

#define len(x) x.size()
#define ll unsigned long
#define i32 int32_t
map<int, string> op;

i32 memory[10000];
i32 A = 0, B = 0, PC = 0, SP = 9999;

void print_err()
{
	cout << "Invalid format used.\n";
    cout << "Usage: emu [option] file_name.o\n";
    cout << "-trace\t:\tShow instruction trace.\n";
    cout << "-before\t:\tShow memory dump before execution.\n";
    cout << "-after\t:\tShow memory dump after execution.\n";
    cout << "-isa\t:\tDisplay ISA.";
}
string toHex(i32 num)
{
	std::ostringstream oss;
	int len = 8;
	oss << hex << setw(len) << setfill('0') << num;
	string ans = oss.str();
	if (len < len(ans))
	{
		ans=ans.substr(len(ans)-len, len);
	} 
	return ans;
}

void ISA() 
{
	cout << "Opcode Mnemonic Operand"<<endl;
	cout << "\t data\t value"<<endl;
	cout << "0 \t ldc\t  value"<<endl;
	cout << "1 \t adc\t value"<<endl;
	cout << "2 \t ldl\t value"<<endl;
	cout << "3 \t stl\t value"<<endl;
	cout << "4 \t ldnl\t value"<<endl;
	cout << "5 \t stnl\t value"<<endl;
	cout << "6 \t add"<<endl;
	cout << "7 \t sub"<<endl;
	cout << "8 \t shl"<<endl;
	cout << "9 \t shr"<<endl;
	cout << "10 \t adj \t value"<<endl;
	cout << "11 \t a2sp"<<endl;
	cout << "12 \t sp2a"<<endl;
	cout << "13 \t call \t offset"<<endl;
	cout << "14 \t return"<<endl;
	cout << "15 \t brz \t offset"<<endl;
	cout << "16 \t brlz \t offset"<<endl;
	cout << "17 \t br  \t offset"<<endl;
	cout << "18 \t HALT"<<endl;
	cout << "\t\t SET \t value"<<endl;
}

void dumping(int PC, ofstream& traceFile)
{
	cout<<endl;
	cout<<"\t\t---Dumping from memory---\t\t"<<endl;
	traceFile<<"\n\t\t---Dumping from memory---\t\t"<<endl;
	for (int i=0; i<PC; i++)
	{
		if (i % 4==0)
		{
			cout<<endl;
			cout<<toHex(i)<<"\t"<<toHex(memory[i])<<"\t ";
			traceFile<<endl<<toHex(i)<<"\t"<<toHex(memory[i])<<"\t ";
		} 
		else
		{
			cout<<toHex(memory[i])<<"\t ";
			traceFile<<toHex(memory[i])<<"\t ";
		}
	} 
	cout<<"\n";
}

// Function to trace individual instructions
void trace(int PC, ofstream& traceFile)
{
	cout<<endl;
	cout<<"\t\t---Tracing instructions---\t\t\n"<<endl;
	traceFile<<"\n\t\t---Tracing instruction---\t\t\n"<<endl;
	bool halt = false;
	set<int> PCoffset;

	PCoffset.insert(16); PCoffset.insert(17);
	PCoffset.insert(13); PCoffset.insert(15);
	int line = 0;

	// Loop till halt is true
	while(true)
	{
		i32 instr = memory[PC];

		// for calculation of negative numbers in hexadecimal
		ll hex_value = stoul("ffffffff", nullptr, 16);
		i32 all_max = static_cast<i32>(hex_value);

		int maxi_op = stoi("ff", nullptr, 16);

		i32 tmpCode = instr & 0xff;
		string tmp = toHex(tmpCode);
		tmp = tmp.substr(6, 2);

		i32 opCode = stoi(tmp, nullptr, 16);
		if (tmp[0] >= '8')
			opCode = -(maxi_op - opCode + 1);

		i32 opr = instr & 0xffffff00;
		if (opr & (1<<31))
			opr = -(all_max - opr + 1);
		opr >>= 8;

		if (op.count(opCode)!=0)
		{
			cout << "PC: " << toHex(PC) << "    SP: " << toHex(SP) << "    A: " << toHex(A) << "    B: " << toHex(B) << "    " << 
			op[opCode] << " " << opr << "\n\n";

			traceFile << "PC: " << toHex(PC) << "    SP: " << toHex(SP) << "    A: " << toHex(A) << "    B: " << toHex(B) << 
			"\t" << op[opCode] << " " << opr << "\n\n";
		} 
		else
			halt = true;

			if(opCode==0){
				B = A;
				A = opr;
			}
			else if(opCode==1){
				A += opr;
			}
			else if(opCode==2){
				B = A;
				A = memory[SP+opr];
			}
			else if(opCode==3){
				memory[SP+opr] = A;
				A = B;
			}
			else if(opCode==4){
				A = memory[A+opr];
			}
			else if(opCode==5){
				memory[A+opr] = B;
			}
			else if(opCode==6){
				A += B;
			}
			else if(opCode==7){
				A = B - A;
			}
			else if(opCode==8){
				A = B << A;
			}
			else if(opCode==9){
				A = B >> A;
			}
			else if(opCode==10){
				SP += opr;
			}
			else if(opCode==11){
				SP = A;
				A = B;
			}
			else if(opCode==12){
				B = A;
				A = SP;
			}
			else if(opCode==13){
				B = A;
				A = PC;
				PC += opr;
			}
			else if(opCode==14)
			{
				if (B == A and A == PC)
					halt = true;
				PC = A;
				A = B;
			}
			else if(opCode==15){
				if(A == 0){
					PC += opr;
				}
			}
			else if(opCode==16){
				if(A < 0){
					PC += opr;
				}
			}
			else if(opCode==17)
				PC += opr;
	
			else if(opCode==18)
				halt = true;

		if (SP>9999)
		{
			cout<<"SP exceeds the memory at PC: "<<PC<<"\n";
			halt=true;
		}
		if (opr==-1 and PCoffset.count(opCode))
		{
			cout<<"Infinite loop: \n";
			halt=true;
		} 
		if (halt) 
			break;
		PC=PC+1;
		line=line+1;
	}
	cout<<line<<" number of instructions executed!\n";
}

int main(int argc, char* argv[])
{
	if (argc <=2)
	{
		print_err();
		return 0;
	}
	op={{0,"ldc"},{1,"adc"},{2,"ldl"},{3,"stl"},{4,"ldnl"},{5,"stnl"},{6,"add"},{7,"sub"},
		{8,"shl"},{9,"shr"},{10,"adj"},{11,"a2sp"},{12,"sp2a"},{13,"call"},{14,"return"},
		{15,"brz"},{16,"brlz"},{17,"br"},{18,"HALT"},{-1,"data"},{-2,"SET"}};

	string inFile = string(argv[2]);
	int dot=inFile.find('.');
	string traceFile = inFile.substr(0, dot) + ".trace";

	std::ifstream input(inFile, ios::out | ios::binary);
	std::ofstream output(traceFile);

	string text;
	getline(input, text);
	if (len(text)==0)
		return 0;
	
	int zero=0;
	int line = zero;

	// for calculation of negative numbers in hexadecimal
	int maxi_opr = stoi("ffffff", nullptr, 16) , maxi_op = stoi("ff", nullptr, 16);

	for (int start = zero; start < len(text); start += 8)
	{
		string macCode = text.substr(start, 8);
		ll hex_value = stoul(macCode, nullptr, 16);
		i32 instr = static_cast<i32>(hex_value);
		string tmp = macCode.substr(6, len(macCode));
		i32 opCode = stoi(tmp, nullptr, 16);

		if (tmp[0] >= '8')
			opCode = -(maxi_op - opCode + 1);

		tmp = macCode.substr(0, 6);
		i32 opr = stoi(tmp, nullptr, 16);
		if (tmp[0] >= '8')
			opr = -(maxi_opr - opr + 1);

		if (opCode >= 0)
			memory[line] = instr;
		else 
			memory[line] = opr;

		line++;
	}

	// Dumping before execution
	if(string(argv[1]) == "-before")
		dumping(line, output);

	// Dumping after execution
	else if(string(argv[1]) == "-after")
	{
		trace(PC, output);
		dumping(line, output);
	}

	// Tracing each instruction
	else if(string(argv[1]) == "-trace")
		trace(PC, output);

	return 0;
}