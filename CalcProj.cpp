//============================================================================
// Name        : CalcProj.cpp
// Author      : David Machin
//============================================================================

#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <stack>
#include <cmath>
#include <time.h>
using namespace std;

void mathExpression(string input);
void evaluateExpression(string var);
double getAnswer(string s, double operan1, double operan2);
bool checkAssignment(string s);


bool divByZero = false;
bool undeclaredVar = false;
bool variableDec = false;
bool quitProgram = false;
bool variableExist = false;
unordered_map<string,double> variableStorage; // stores any variables created
stack<char> operatorStorage; // stores the operators
queue<string> storage; // stores the entire expression

int main() {

	//std::ios_base::sync_with_stdio(false);
	//clock_t t, f;
	//t = clock();

	string userInput;
	string expression;

	variableStorage["Pi"] = 3.14169; //constants used in program
	variableStorage["e"] = 2.718;

	for (int i = 0; i < 10000; i++){
		variableExist = false;
		undeclaredVar = false;
		divByZero = false;
		variableDec = false;
		getline(cin, userInput); //gets the user input
		mathExpression(userInput); //passes in user input to the function
	}
	//f = clock();
	//t = t - f;
	//cout<<((float)t)/CLOCKS_PER_SEC << endl;
	return 0;
}

bool checkAssignment(string s){
	bool assignment = false;

	if(storage.front() == "let" && storage.front() == s){ //if the front of queue equals "let" then pop it off and return a true value
		assignment = true;
		storage.pop();
	}
	return assignment;
}

int operatorPrecedence(char character){ //returns a precedence value based on operator passed in
	 if (character == '-' || character == '+'){
	 	return 1;
	 }
  	else if (character == '/' || character == '*'){
  		 return 2;
  	}
  	else if(character == '^'){
  		return 3;
  	}
  	else if(character == '@' || character == '$' || character == '#'){ //for trig and log functions
  		return 4;
  	}
  	else{
  		return 0;
  	}
  }

void mathExpression(string input){ //shuntz algorithm
	string variable;
	bool check = false;
	bool check2 = false;
	bool equalSign = false;

	for(unsigned int i =0; i<input.length(); i++){
		string currentString = ""; //initializes string
		if(input[i] == ' '){ //if empty space continue
			continue;
		}
		else if(input[i] == '='){ // set a boolean to true if an equal sign is encountered.
			equalSign = true;
			continue;
		}
		else if(isalpha(input[i])){ //if character is alphabetical enter
			while(isalpha(input[i]) && i < input.length()){ //Will append a string with characters until something that is not a character is encountered
				currentString.push_back(input[i]);
				i++;
				if(!isalpha(input[i]) || i >= input.length()){
					i--;
					break;
				}
			}
			if(currentString == "quit"){  //break off loop if user inputs quit
				quitProgram = true;
				break;
			}
			else if(currentString == "let" && storage.empty() && check == false && equalSign == false){ //if user is declaring a variable, set some booleans to true
				storage.push(currentString);
				check2 = checkAssignment(currentString);
				check = true;
				continue;
			}
			else if(check2 == true && equalSign == false){ // if user is declaring a variable then the variable should be before an equal sign and after "let"
				storage.push(currentString);
				variable = storage.front();
				storage.pop();
				variableDec = true;
				continue;
			}
			else if(currentString == "sin"){ //if string is a trig or log push in their respective symbols
				operatorStorage.push('@');
			}
			else if(currentString == "cos"){
				operatorStorage.push('$');
			}
			else if(currentString == "log"){
				operatorStorage.push('#');
			}
			else{
				if(variableStorage.count(currentString) == 0){ //if variable does not exist in hash map then print statement and break
					cout<<"Undeclared-Variable\n";
					undeclaredVar = true;
					break;
				}
				else{
					storage.push(currentString); //if variable exists, push value into queue
					variableExist = true;
				}
				continue;
			}
			currentString = "";
		}

		if(isdigit(input[i])){ //if character is a digit then enter statement
			while((isdigit(input[i]) || input[i] == '.') && i < input.length()){ //checks for consecutive digits and appends them to string
				currentString.push_back(input[i]);
				i++;
				if((!isdigit(input[i]) && input[i] != '.' ) || i >= input.length()){
					i--;
					break;
				}
			}
			storage.push(currentString);
			currentString = "";
			continue;
		}
		else if((operatorStorage.empty() || operatorStorage.top() == '(') && operatorPrecedence(input[i]) > 0){ //if stack is empty then input any operator that comes through
			operatorStorage.push(input[i]);
			continue;
		}
		else if(input[i] == '('){ // push left parenthesis into stack
			operatorStorage.push(input[i]);
			continue;
		}
		else if(input[i] == ')'){ // if character is a right parenthesis, continue pushing all the operators in the stack until a left parenthesis is encountered
			bool paren = false;
			while(paren == false && !operatorStorage.empty()){
				if(!operatorStorage.empty() && operatorStorage.top() != '('){
					currentString.push_back(operatorStorage.top());
					storage.push(currentString);
					operatorStorage.pop();
					currentString = "";
				}
				else{
					operatorStorage.pop(); //pops "("
					paren = true;
					if(!operatorStorage.empty() && (operatorStorage.top() == '@' || operatorStorage.top() == '#' || operatorStorage.top() == '$')){ //if a trig or log function is at the top of stack then push it to queue
						currentString.push_back(operatorStorage.top());
						storage.push(currentString);
						operatorStorage.pop();
						currentString = "";
					}
				}
			}
			continue;
		}
			int precedence = operatorPrecedence(input[i]); //gets precedence of character
			if((!operatorStorage.empty()) && (precedence > operatorPrecedence(operatorStorage.top()) || precedence == 3) && operatorStorage.top() != '@' && operatorStorage.top() != '#' && operatorStorage.top() != '$') { //push exponents into stack unless their is a trig or log function at the top
				operatorStorage.push(input[i]);
				continue;
			}
			else if(precedence <= 2){
				bool push = false;
				while(!operatorStorage.empty() && precedence <= 2 && precedence <= operatorPrecedence(operatorStorage.top()) && operatorStorage.top() != '@' && operatorStorage.top() != '#' && operatorStorage.top() != '$'){
					currentString.push_back(operatorStorage.top());
					storage.push(currentString);
					operatorStorage.pop();
					currentString = "";
					push = true;
				}
				if(push == true){
					operatorStorage.push(input[i]);
					continue;
				}
			}
			else{
				operatorStorage.push(input[i]);
			}
	}
	string currentString2 = "";
	while(!operatorStorage.empty()){// Push remaining elements on stack onto queue
		currentString2.push_back(operatorStorage.top());
		storage.push(currentString2);
		operatorStorage.pop();
		currentString2 = "";
	}
	evaluateExpression(variable);  //pass in expression as well as variable name (if one exists) to evaluate

}

double getAnswer(string s, double operan1, double operan2) { // conducts operations based on the operators and operands

	 if (s == "-"){
	 	return operan2 - operan1;
	 }
	 else if (s == "+"){
	 	return operan2 + operan1;
	 }
	 else if (s == "*"){
  		 return operan2 * operan1;
  	}
  	else if (s == "/"){
  		 return operan2/operan1;
  	}
  	else if(s == "^"){
  		return pow(operan2, operan1);
  	}
  	else if(s == "@"){
  		return sin(operan1);
  	}
  	else if(s == "#"){
  		return log(operan1);
  	}
  	else if(s == "$"){
  		return cos(operan1);
  	}
  	else{
  		return 0;
  	}
}
void evaluateExpression(string var){
	stack<double> expression; // new stack created to store the results
	string string1;
	double operand1;
	double operand2;
	double answer;

	while(!storage.empty() && undeclaredVar == false){
		string1 = storage.front();

		if(isdigit(string1[0])){
			expression.push(atof(string1.c_str()));
		}
		else if(string1 == "+" || string1 == "-" || string1 == "/" || string1 == "*" || string1 == "^" ){ //if a basic operator pop off corresponding operands and compute
			operand1 = expression.top();
			expression.pop();
			operand2 = expression.top();
			expression.pop();
			if(operand1 == 0 && string1 == "/"){ //checks if something is divided by 0
				cout<<"Division-By-Zero\n";
				divByZero = true;
				break;
			}
			else{
				answer = getAnswer(string1, operand1, operand2); // gets result of computation
				expression.push(answer);
			}
		}
		else if(string1 == "@" || string1 == "#" || string1 == "$" ){ //if trig or log then compute the corresponding operation
			operand1 = expression.top();
			expression.pop();
			answer = getAnswer(string1, operand1, operand2);
			expression.push(answer);
		}
		else if(isalpha(string1[0]) && variableExist == true && variableStorage.count(string1) > 0){
			expression.push(variableStorage[string1]);
		}
		storage.pop(); //pop element off queue

	}
	if(var != "" && variableDec ==true && undeclaredVar == false){ //if variable has been declared, it will be stored into hashmap here along with its value
		variableStorage[var] = expression.top();
	}
	if(!expression.empty() && divByZero != true && variableDec != true && undeclaredVar != true && quitProgram != true){ // Only if boolean values are false and stack is not empty will value be printed
		cout<<expression.top()<<"\n";
	}

	while(!expression.empty()){ //pops any remaining elements on stack
		expression.pop();
	}
	while(!storage.empty()){ //pops any remaining elements on queue
		storage.pop();
	}

	var = "";
}



