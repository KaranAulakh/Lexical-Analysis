#include "pa3.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

// node constructor
stack::node::node(string kw):keyword(kw), next(NULL) {}

// stack constructor
stack::stack(int s):head(NULL) {} 

// adds an element to the stack
void stack::push(string kw) {
	node* newNode = new node(kw);
	if (head == NULL)
		head = newNode;
	else {
		newNode->next = head;
		head = newNode;
	}
}

// takes away the last added element from the stack 
void stack::pop() {
	head = head->next;
}

int stack::stackSize() {
	int size = 0;
	node* transversalPointer = head;

	while(transversalPointer != NULL) {
		size++;
		transversalPointer = transversalPointer->next;
	}
	return size;
	cout << "\n\n\n" << size << "\n\n\n";
}

// returns the last added element into the stack 
string stack::get_topKeyword() {
	return head->keyword;
}

//constructor
wordSorter::wordSorter(int s):bracketCount(0) {}

//determins what class the token is and stores it accordingly
void wordSorter::add(string token) {
	bool alreadyTaken = false;
	bool isNumber = true;
	// Checks to see if token is an integer
	for (int i = 0; i < token.size(); i++) {
		if (!isdigit(token[i]))
			isNumber = false;
	}
	// if token is an integer it will place it in the constants vector, unless the vector already contains it
	if (isNumber) {
		for(int i = 0; i < constants.size(); i++) {
	   		if (constants[i].compare(token) == 0)
	   			alreadyTaken = true;
	   	}
	   	if (!alreadyTaken)
	   		constants.push_back(token);
	}
	// checks to see if token is a delimiter and stores it in the delimiter vector if so 
	else if (token.compare(",") == 0 || token.compare(";") == 0) {
		for(int i = 0; i < delimiters.size(); i++) {
	   		if (delimiters[i].compare(token) == 0)
	   			alreadyTaken = true;
	   	}
	   	if (!alreadyTaken)
	   		delimiters.push_back(token);
	} 
	// checks to see if the token is a operator and store it in the operators vector if so 
	else if (token.compare("+") == 0 || token.compare("++") == 0 || token.compare("-") == 0 || token.compare("*") == 0 || token.compare("/") == 0 || token.compare("=") == 0) {
		for(int i = 0; i < operators.size(); i++) {
	   		if (operators[i].compare(token) == 0)
	   			alreadyTaken = true;
	   	}
	   	if (!alreadyTaken)
	   		operators.push_back(token);
	}
	// checks to see if the token is a bracket, and uses the bracketCounter to represent a mismatch in brackets, if
	// bracket counter is not zero, there is a mismatch, a negative int means there are more left brackets
	else if (token.compare("(") == 0) 
		bracketCount--;
	else if (token.compare(")") == 0) 
		bracketCount++;
	// if the token isn't anything else, it is a identifier
	else {
		for(int i = 0; i < identifiers.size(); i++) {
	   		if (identifiers[i].compare(token) == 0)
	   			alreadyTaken = true;
	   	}
		if (!alreadyTaken)
			identifiers.push_back(token);
	}
}
// simply prints each classification of tokens and then all token in that given vector
void wordSorter::printTokens() {
	cout << "Identifiers: ";
	for(int i = 0; i < identifiers.size(); i++) {
   		cout << identifiers[i] << " ";
   	}
   	cout << "\nConstants: ";
   	for (int i = 0; i < constants.size(); i++) {
   		cout << constants[i] << " ";
   	}
   	cout << "\nOperators: ";
	for(int i = 0; i < operators.size(); i++) {
   		cout << operators[i] << " ";
   	}
   	cout << "\nDelimiters: ";
   	for(int i = 0; i < delimiters.size(); i++) {
   		cout << delimiters[i] << " ";
   	}
   	cout << "\n\nSyntax Error(s): ";
   	if (errors.size() < 1 && bracketCount == 0)
   		cout << "NA";
   	else if (bracketCount != 0) {
   		if (bracketCount < 0) {
   			while (bracketCount < 0) {
   				cout << "( ";
   				bracketCount++;
   			}
   		}
   		else {
   			while (bracketCount > 0) {
   				cout << ") ";
   				bracketCount--;
   			}
   		}
   	}
   	for(int i = 0; i < errors.size(); i++) {
   		cout << errors[i] << " ";
   	}   	
   	cout << endl;
}

void wordSorter::addTo_error(string er) {
		errors.push_back(er);
}

int main() {
	int loopDepth = 0; // keeps track of the current depth of the loop
	int maxLoopDepth = 0; // keeps track of the max depth the loop has ever been 
	bool FOR = false; // determines whether or not the FOR keyword was used
	bool BEGIN = false; // determines whether the BEGIN keyword was used
	bool END = false; // determines whether the END keyword was used
	string input; // holds the entire input file in a string format
	string temp; // used as needed to open file, get the contents of the file and parse it 
	ifstream myFile;
	stack myStack(-1);
	wordSorter my_wordSorter(-1);

	cout << "Please enter the name of the input file:\n";
	cin >> temp;
	myFile.open(temp.c_str());

	// if the file cannot be opened it just closes the program 
	if (!myFile.is_open()) {
		cout << "error could not open file\n";
		return 0;
	}
	// places the entire file into the input string by getting it line by line using temp
	while (getline(myFile, temp)) {
		temp = temp + "\n";
		input = input + temp;
	}

	// extracts and operators, delimiters and brackets from the string and adds in a space for easier parsing later on
	for (int i = 0; i < input.size(); i++) {
		if (input.at(i) == '-' || input.at(i) == '=' || input.at(i) == '*' || input.at(i) == '/' || input.at(i) == '(' || input.at(i) == ')' || input.at(i) == ',' || input.at(i) == ';') {
			my_wordSorter.add(input.substr(i, 1));
			input[i] = ' ';
		}
		// since there is a "+" and "++" that operator, whenever the program comes across a "+" it checks to see if
		// it's a part of a "++" operator, and it adds the proper one to my_wordSorter
		else if (input.at(i) == '+') {
			if (input.at(i+1) == '+') {
				my_wordSorter.add("++");
				input[i] = ' ';
				input[i+1] = ' ';
			}
			else {
				my_wordSorter.add("+");
				input[i] = ' ';
			}
		}
	}
	// parses the rest of the string by getting each token surrounded by white space. The program keeps creating
	// substrings until the input string is empty. Which is why the i index is reset every time it finds a token
	for (int i = 0; i < input.size(); i++) {
		if (input.at(i) == 9 || input.at(i) == 10 || input.at(i) == 11 || input.at(i) == 13 || input.at(i) == 32 || input.at(i) == 40 || input.at(i) == 41) {
			temp = input.substr(0, i);
			input = input.substr(i+1, input.size());	
			if (temp != "") {
				// if the token is a "FOR" or "BEGIN" it will add it to my stack
				if (temp.size() > 2 && isupper(temp[0])) {
					if (temp.compare("FOR") == 0) {
						FOR = true;
						myStack.push(temp);
					}
					else if (temp.compare("BEGIN") == 0) {
						BEGIN = true;
						myStack.push(temp);
					}
					// if the token is "END" it will first check the size of the stack to determine the current loop
					// depth and then pop each eliminate until the FOR is found that created the given loop
					else if (temp.compare("END") == 0) {
						END = true;
						loopDepth = floor(myStack.stackSize()/2);
						if (loopDepth >= maxLoopDepth)
							maxLoopDepth = loopDepth;
						while (myStack.get_topKeyword() != "FOR") {
							myStack.pop();
						}
						myStack.pop();
						loopDepth--;
					}
					// if the word was meant to be a keyword but didn't match the three acceptable ones, it will end
					// up in the error vector.
					else {
						my_wordSorter.addTo_error(temp);
					}
				}
				// for every other token besides keywords, they will be sorted by the class that is meant to sort 
				// and keep track of all tokens (wordSorter).
				else
					my_wordSorter.add(temp);
			}
			i = -1;
		}
	}
	// if the stack still contains elements, that must mean there is a missing "END" keyword.
	if (myStack.stackSize() > 0)
		my_wordSorter.addTo_error("END");
	// prints all the statements.
	cout << "\nThe depth of nested loop(s) is " << maxLoopDepth << "\n\n";
	cout << "Keywords: ";
	if (FOR)
		cout << "FOR ";
	if (BEGIN)
		cout << "BEGIN ";
	if (END)
		cout << "END ";
	cout << endl;
	my_wordSorter.printTokens();	
}