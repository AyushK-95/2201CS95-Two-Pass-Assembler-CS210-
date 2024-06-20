/*
TITLE: Assembler                                                                                                                             
AUTHOR: Ayush Kumar
ROLL NUMBER: 2201CS95
Declaration of Authorship
This C++ file, asm.cpp, is part of the assignment of CS209 & CS210 at the department of Computer Science and Engineering, IIT Patna. 
*/

#include <bits/stdc++.h>
using namespace std;

#define pa pair<int,int>
#define pas pair<string,string>
#define len(x) x.size()
#define pb push_back
#define mp make_pair
#define F first
#define S second
#define all(x) x.begin(),x.end()

vector<pas> instructions;
map<int, vector<string> > code;
vector<pa> encodedLines; // format is opCode operand
vector<pair<int, string> > errList;
map<int, int> ind;
map<string, int> labels;

//Make a map with format => Name Number of operands OpCode
map<string,pa> op;
string trim(string label)
{
	int n=len(label);
	int start=0;
	for (int i=0; i<len(label);i++)
    {
		if (label[i] == ' ' || label[i] == '\t')
			continue;
		
		start = i;
		break;
	}
	int end = n-1;
	for (int i=len(label)-1; i>=0; i--)
    {
		if (label[i] == ' ' || label[i] == '\t')
		{
			continue;
		}
		end=i;
		break;
	}
	string ans="";
	for (int i=start; i<end+1; i++)
    {
		ans=ans+label[i];
	}
	return ans;
}
string toHex(int num, int len)
{
	std::ostringstream oss;
	oss << hex << setw(len) << setfill('0') << num;
	string ans = oss.str();
	if (len<len(ans))
		ans = ans.substr(len(ans)-len, len);
	
	return ans;
}

bool isAlphabet(char c)
{
	char cc = tolower(c);
	return cc >= 'a' and cc <= 'z';
}

bool isDigit(char c)
{
	return c >= '0' and c <= '9';
}

bool validLabel(string label)
{
	if (!(isAlphabet(label[0])))
		return false;
	
	for (int i=1; i<len(label); i++)
	{
		if (!isAlphabet(label[i]) && !isDigit(label[i]))
			return false;
	}
	return true;
}
void insertLabel(string label, int line, int PC)
{
	if (validLabel(label)==0)
	{
		errList.pb(mp(line, "Invalid Label Name: " + label));
		return;
	} 

	if (labels.count(label)==0) //if there or not already
		labels.insert({label,PC});	//labels[label] = PC;
	else
		errList.pb(mp(line, "Repeated label definition: " + label));
}

bool InvalidNumber(string s)
{
	for (char c:s)
	{
		if (!isDigit(c))
			return true;		
	}
	return false;
}

int decode(string opr, int line)
{
	if (validLabel(opr)==true)
	{
		if (labels.count(opr)==0)
			errList.pb(mp(line, "Label not present: " + opr));
		
		return labels[opr];
	}

	const char* start_ptr = opr.c_str();
	char* end_ptr;
	int num;
	bool issue = false;

	if (len(opr)>=2 && opr[0] == '0')
	{
        // Might be a hex / octal / binary
		if(opr[1]=='b')
		{	//Base 2
			issue = InvalidNumber(opr.substr(2, len(opr)-2));
			num = strtol(start_ptr + 2, &end_ptr, 2);
		}
		else if(opr[1]=='x')
		{ // Base 16
			issue = InvalidNumber(opr.substr(2, len(opr)-2));
			num = strtol(start_ptr + 2, &end_ptr, 16);
		}
		else // Base 8
		{		
			issue = InvalidNumber(opr);
			num = strtol(start_ptr, &end_ptr, 8);
		}
    } 
	else
	{ // Base 10
    	int slice = 0;
    	if (opr[0] == '-' || opr[0] == '+') 
			slice=slice+1;
    	issue = InvalidNumber(opr.substr(slice, len(opr)-slice));
    	num=strtol(start_ptr, &end_ptr, 10);
    } 
    if (issue==true)
	{
    	errList.pb(mp(line, "Invalid expression: " + opr));
    	return 0;
    }
    return num;
}

void firstPass(string inFile)
{
	std::ifstream input(inFile);
	string text;
	int PC = 0,line = 0;
	pair<int, string> lLabel;
	while (getline(input, text))
	{
		line=line+1;

		text = trim(text);
		code[PC].pb(text);
		int semicolon=text.find(';');
		text = text.substr(0, semicolon);
		text = trim(text);
		string label = "";
		string instr = "";
		string opr = "";

		if (text.find(':') != string::npos)
		{
			label = text.substr(0, text.find(':'));
			label = trim(label);
			if (len(label)!=0)
			{
				insertLabel(label, line, PC);
				lLabel = mp(PC, label);
			}
			int colon = text.find(':');
			text = text.substr(colon+1, len(text)-1-colon);
			text = trim(text);
		}
		if (text.find(' ') != string::npos)
		{
			instr = text.substr(0,  text.find(' '));
			instr = trim(instr);
			int space = text.find(' ');
			opr = text.substr(space+1, len(text)-1-space); 
			opr = trim(opr);
		} 
		else
			instr = trim(text);
		
		
		if (len(text)==0) 
			continue;
		ind[PC] = line;
		if (op .count(instr)==0)
			errList.pb(mp(line, "Ivalid Mnemonic: " + instr));
		
		if (instr == "SET")
		{
			if (lLabel.first == PC)
				labels[label] = decode(opr, line);
			else
				errList.pb(mp(line, "Label doesn't exist: " + instr));
		}
		
		if (len(opr)==0 && op[instr].first)
		{
			errList.pb(mp(line, "Missing operand: " + instr));
		} 
		else if (len(opr) && op[instr].first==0)
		{
			errList.pb(mp(line, "Unexpected operand: " + instr + " " + opr));
		}
		instructions.pb(mp(instr, opr));
		PC=PC+1;
	}
}

void secondPass(ofstream &logFile, ofstream &outFile, ofstream &objFile)
{
	for (int i=0; i<len(instructions); i++)
	{
		string instr = instructions[i].F , opr = instructions[i].S;
		int line = ind[i] , opCode = op[instr].S;
        // if coder haven't provided operand, take it as zero and continue on the function to add in errors in case of error the code stops later
		int operand = (op[instr].F) ? decode(opr, line) : 0;
		if(instr == "data")
		{
            // Since data doesn't have any opcode in the pdf they tell to use data as operand entirely
			opCode = operand & 0xff;
			operand >>= 8;
		}
		encodedLines.pb(mp(opCode, operand));
	}
	if (len(errList)!=0)
	{
		cout<<"Code contains errors\n";
		sort(all(errList));
		for (pair<int, string> it : errList)
			logFile<<"Line: "<<it.F<<" "<<it.S<<endl;
		
		return;
	} 
	else
		cout<<"Compiled successfully\n";

	int PC = 0;
	// Those who take offsets, here have to find the distance
	set<int> PCoffset;
	PCoffset.insert(16); 
	PCoffset.insert(17);
	PCoffset.insert(13); 
	PCoffset.insert(15);
	
	for (int PC=0; PC<len(encodedLines); PC++)
	{
		outFile << toHex(PC, 8) << " ";
		for (int i=0; i<len(code[PC])-1; i++)
		{
			string s = code[PC][i];
			outFile << "\t     "<< s <<endl;
			outFile << toHex(PC, 8) << " ";
		}

		int opCode = encodedLines[PC].F , opr = encodedLines[PC].S;

		if (PCoffset.count(opCode))
		{
			opr=opr-(PC+1);
		} 
		outFile << toHex(opr, 6) << toHex(opCode, 2) << " " << code[PC][code[PC].size()-1] << endl;
		objFile << toHex(opr, 6) << toHex(opCode, 2);
	}
}

int main(int argc, char* argv[])
{
	if(argc<=1)
	{
        cout<<"Usage: ./asm file.asm \n Where file.asm is the file to be compiled. \n";
        return 0;
    }
	op={{"data", {1, -1}}, {"ldc", {1, 0}}, {"adc", {1, 1}}, {"ldl", {1, 2}},
		{"stl", {1, 3}}, {"ldnl", {1, 4}}, {"stnl", {1, 5}}, {"add", {0, 6}},
		{"sub", {0, 7}}, {"shl", {0, 8}}, {"shr", {0, 9}}, {"adj", {1, 10}},
		{"a2sp", {0, 11}}, {"sp2a", {0, 12}}, {"call", {1, 13}}, {"return", {0, 14}},
		{"brz", {1, 15}}, {"brlz", {1, 16}}, {"br", {1, 17}}, {"HALT", {0, 18}},{"SET", {1, -2}}};

	string inFile = string(argv[1]);
	firstPass(inFile);

    //Naming output files
	int dot=inFile.find('.');
	inFile = inFile.substr(0, dot);
	std::ofstream outFile(inFile + ".l"); 
	std::ofstream logFile(inFile + ".log");
	std::ofstream objFile(inFile + ".o", ios::out|ios::binary);

	secondPass(logFile, outFile, objFile);

	return 0;
}