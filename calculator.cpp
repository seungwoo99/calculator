//calculator.cpp
//Simon Choi
//CS480

//header files
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <cmath>
using namespace std;

//global variables
// ANSI escape codes for text color
const string RED_COLOR = "\x1b[31m";
const string RESET_COLOR = "\x1b[0m";
vector<string> tokenizedExp;

/* splitExpression method
input: expression(string)
output: tokens(vector<string>)
*/
vector<string> splitExpression(const string& expression){
    vector<string> tokens;

    stringstream ss(expression);
    char token;
    while(ss >> token){
        if(isdigit(token)){ //digits
            string digit = "";
            bool dotted = false;
            digit += token;
            while(ss >> token){
                if(isdigit(token)){
                    digit += token;
                }else if(token == '.'){
                    if(dotted){
                        ss.putback(token);
                        break;
                    }
                    digit += token;
                    dotted = true;
                }else{
                    ss.putback(token);
                    break;
                }
            }
            if(digit[digit.length()-1] == '.'){
                digit.pop_back();
                ss.putback('.');
            }

            tokens.push_back(digit);
        }else if(token == '+' || token == '-' || token == '*' || token == '/' || token == '^' 
        || token == '(' || token == ')' || token == '{' || token == '}'){ //arithmetic operations
            string arithmetic = "";
            arithmetic += token;
            tokens.push_back(arithmetic);
        }else if(isalpha(token)){ //letters
            string letter;
            letter += token;
            while(ss >> token){
                if(isalpha(token)){
                    letter += token;
                }else{
                    ss.putback(token);
                    break;
                }
            }
            tokens.push_back(letter);
        }else{
            string ch = "";
            ch += token;
            tokens.push_back(ch);
        }
    }

    return tokens;
}

/* isArithmetic method
input: string
output: bool
*/
bool isArithmetic(const string& input){
    return input == "+" || input == "-" || input == "*" || input == "/" || input == "^";
}

/* isTrigLog method
input: string
output: bool
*/
bool isTrigLog(const string& input){
    return input == "sin" || input == "cos" || input == "tan" || input == "cot" || input == "ln" || input == "log";
}

/* isOpenBracket method
input: string
output: bool
*/
bool isOpenBracket(const string& input){
    return input == "(" || input == "{";
}

/* isCloseBracket method
input: string
output: bool
*/
bool isCloseBracket(const string& input){
    return input == ")" || input == "}";
}


/* checkTokens method
input: tokens(vector<string>)
output: validation(vector<bool>)
*/
vector<bool> checkTokens(vector<string>& tokens){
    vector<bool> validations(tokens.size());
    vector<int> parenthesis;
    vector<int> curly;
    
    //first token
    if(isdigit(tokens[0][0]) && tokens.size() > 1){
        validations[0] = true;
    }else if(isOpenBracket(tokens[0]) && tokens.size() > 1){
        if(tokens[1] == "-" || isdigit(tokens[1][0]) || isTrigLog(tokens[1]) || isOpenBracket(tokens[1])){
            validations[0] = true;
        }
    }else if(tokens[0] == "-" && tokens.size() > 1){
        validations[0] = true;
    }else if(tokens[0] == "log" && tokens.size() > 2){
        if(stoi(tokens[1]) == 10 && tokens[2] == "("){
            validations[0] = true;
        }
    }else if((tokens[0] == "sin" || tokens[0] == "cos" || tokens[0] == "tan" || tokens[0] == "cot" || tokens[0] == "ln") && tokens.size() >1){
        if(isOpenBracket(tokens[1])){
            validations[0] = true;
        }
    }


    if(tokens[0] == "("){
        parenthesis.push_back(0);
    }else if(tokens[0] == "{"){
        curly.push_back(0);
    }

    //check all the other tokens
    for(int index=1; index<tokens.size()-1; index++){
        if(tokens[index] == "+" || tokens[index] == "*" || tokens[index] == "/" || tokens[index] == "^"){ //arithmetic operations
            if((isdigit(tokens[index-1][0]) || isCloseBracket(tokens[index-1]))&&(isdigit(tokens[index+1][0]) || isOpenBracket(tokens[index+1]) || isTrigLog(tokens[index+1]) || tokens[index+1] == "-")){
                validations[index] = true;
            }
        }else if(tokens[index] == "-"){ //minus operator
            if((isdigit(tokens[index-1][0]) || isArithmetic(tokens[index-1]) || isOpenBracket(tokens[index-1]) || isCloseBracket(tokens[index-1]) || tokens[index-1] == "-")
            &&(isdigit(tokens[index+1][0]) || isOpenBracket(tokens[index+1]) || tokens[index+1] == "-" || isTrigLog(tokens[index+1]))){
                validations[index] = true;
            }
        }else if(isTrigLog(tokens[index])){ //trigonometric and logarithmic operations
            if(tokens[index] == "log"){
                if((isArithmetic(tokens[index-1]) || isOpenBracket(tokens[index-1]))&&(isdigit(tokens[index+1][0])) &&(index+2 < tokens.size())){
                    if(tokens[index+2] == "(" && stoi(tokens[index+1]) == 10){
                        validations[index] = true;
                    }
                }
            }else{
                if((isOpenBracket(tokens[index-1]) || isArithmetic(tokens[index-1]))&&(tokens[index+1] == "(")){
                    validations[index] = true;
                }
            }
        }else if(isdigit(tokens[index][0])){ //digit
            if((isOpenBracket(tokens[index-1]) || isArithmetic(tokens[index-1]) || tokens[index-1] == "-") && (isCloseBracket(tokens[index+1]) || isArithmetic(tokens[index+1]))){
                validations[index] = true;
            }
            if(stoi(tokens[index]) == 10 && tokens[index-1] == "log"){
                validations[index] = true;
            }
        }else if(tokens[index] == "("){ //opening parenthesis
            if((isArithmetic(tokens[index-1]) || isOpenBracket(tokens[index-1]) || isTrigLog(tokens[index-1])) && (isdigit(tokens[index+1][0]) || tokens[index+1]== "-" || isTrigLog(tokens[index+1]) || isOpenBracket(tokens[index+1]))){
                validations[index] = true;
            }else if(index-2 >= 0){
                if(isdigit(tokens[index-1][0]) && tokens[index-2] == "log"){
                    if(stoi(tokens[index-1]) == 10){
                        validations[index] = true;
                    }
                }
            }
            if(tokens[index-1] == "log"){
                validations[index] = false;
            }
            parenthesis.push_back(index);
        }else if(tokens[index] == "{"){ //opeing curly brackets
            if((isArithmetic(tokens[index-1]) || isOpenBracket(tokens[index-1])) && (isdigit(tokens[index+1][0]) || tokens[index+1] == "-" || isTrigLog(tokens[index+1]) || isOpenBracket(tokens[index+1]))){
                validations[index] = true;
            }
            curly.push_back(index);
        }else if(isCloseBracket(tokens[index])){ //closing parenthesis and curly brackets
            if((isdigit(tokens[index-1][0]) || isCloseBracket(tokens[index-1])) && (isArithmetic(tokens[index+1]) || isCloseBracket(tokens[index+1]))){
                validations[index] = true;
            }
            if(tokens[index] == ")"){
                validations[index] = true;
                if(parenthesis.empty()){
                    validations[index] = false;
                }else{
                    int openIndex = parenthesis.back();
                    bool remove = false;
                    if(!curly.empty()){
                        for(int cur=0; cur<curly.size(); cur++){
                            int curIndex = curly.front();
                            if(curIndex > openIndex && curIndex < index){
                                validations[curIndex] = false;
                                remove = true;
                            }
                            curly.erase(curly.begin());
                            curly.push_back(curIndex);
                        }
                        parenthesis.pop_back();
                    }else{
                        parenthesis.pop_back();
                    }
                }
            }else if(tokens[index] == "}"){
                validations[index] = true;
                if(curly.empty()){
                    validations[index] = false;
                }else{
                    int openIndex = curly.back();
                    bool remove = false;
                    if(!parenthesis.empty()){
                        for(int par=0; par<parenthesis.size(); par++){
                            int parIndex = parenthesis.front();
                            if(parIndex > openIndex && parIndex < index){
                                validations[parIndex] = false;
                                remove = true;
                            }
                            parenthesis.erase(parenthesis.begin());
                            parenthesis.push_back(parIndex);
                        }
                        curly.pop_back();
                    }else{
                        curly.pop_back();
                    }
                }
            }
        }else{
            validations[index] = false;
        }

    }

    //check last index   
    int lastIndex = tokens.size()-1;
    if(isdigit(tokens[lastIndex][0])){
        validations[lastIndex] = true;
    }else if(lastIndex != 0 && isCloseBracket(tokens[lastIndex])){
        if((isdigit(tokens[lastIndex-1][0]) || isCloseBracket(tokens[lastIndex-1]))){
            validations[lastIndex] = true;
        }
        if(tokens[lastIndex] == ")"){
            validations[lastIndex] = true;
            if(parenthesis.empty()){
                validations[lastIndex] = false;
            }else{
                int openIndex = parenthesis.back();
                bool remove = false;
                if(!curly.empty()){
                    for(int cur=0; cur<curly.size(); cur++){
                        int curIndex = curly.front();
                        if(curIndex > openIndex && curIndex < lastIndex){
                            validations[curIndex] = false;
                            remove = true;
                        }
                        curly.erase(curly.begin());
                        curly.push_back(curIndex);
                    }
                    parenthesis.pop_back();
                }else{
                    parenthesis.pop_back();
                }
            }
        }else if(tokens[lastIndex] == "}"){
            validations[lastIndex] = true;
            if(curly.empty()){
                validations[lastIndex] = false;
            }else{
                int openIndex = curly.back();
                bool remove = false;
                if(!parenthesis.empty()){
                    for(int par=0; par<parenthesis.size(); par++){
                        int parIndex = parenthesis.front();
                        if(parIndex > openIndex && parIndex < lastIndex){
                            validations[parIndex] = false;
                            remove = true;
                        }
                        parenthesis.erase(parenthesis.begin());
                        parenthesis.push_back(parIndex);
                    }
                    curly.pop_back();
                }else{
                    curly.pop_back();
                }
            }
        }
        if((!isdigit(tokens[lastIndex-1][0]) && !isCloseBracket(tokens[lastIndex-1])) || isalpha(tokens[lastIndex][0])){
            validations[lastIndex] = false;
        }
    }
    if(!parenthesis.empty()){
        for(int p=0; p<parenthesis.size(); p++){
            validations[parenthesis.back()] = false;
            parenthesis.pop_back();
        }
    }
    if(!curly.empty()){
        for(int c=0; c<curly.size(); c++){
            validations[curly.back()] = false;
            curly.pop_back();
        }
    }

    return validations;
}

/* processTokens method
input: validation(vector<string>)
output: valid(bool)
*/
bool processTokens(vector<string>& tokens, vector<bool>& validations){
    bool valid = true;
    for(bool validation:validations){
        if(validation == false){
            valid = false;
        }
    }
    if(valid == false){
        cout << RED_COLOR << "ERRORS: ";
        for(int index=0; index<validations.size(); index++) {
            if(validations[index] == true){
                cout << RESET_COLOR << tokens[index];
            }else {
                cout << RED_COLOR << tokens[index];
            }
        }
        cout << RESET_COLOR << endl;
    }else{ //distinguish unary and bianry operator
        int limit = tokens.size();
        for(int i=0; i <limit; i++){
            if(tokens[i] == "-"){
                if(isArithmetic(tokens[i-1]) || isOpenBracket(tokens[i-1])){
                    int end = i+1;
                    while(tokens[end] == "-"){
                        end++;
                    }
                    int count = end - i;
                    if(count % 2 == 0){
                        while(count > 0){
                            tokens.erase(tokens.begin()+i);
                            count--;
                        }
                    }else{
                        while(count > 1){
                            tokens.erase(tokens.begin()+i);
                            count--;
                        }
                        tokens[i] = "-1";
                        tokens.insert(tokens.begin()+i+1, "*");
                    }
                    limit = tokens.size();
                }
            }
        }
    }
    tokenizedExp = tokens;
    return valid;

}

/* compute2nums method
input: num2(double), operation(string), num1(double)
output: calculation result(double)
*/
double compute2nums(double num2, string operation, double num1){
    if(operation == "+"){
        return num2 + num1;
    }else if(operation == "-"){
        return num2 - num1;
    }else if(operation == "*"){
        return num2 * num1;
    }else if(operation == "/"){
        return num2 / num1;
    }else if(operation == "^"){
        return pow(num2, num1);
    }else{
        return 0;
    }
}

/* calculate method
input: equations(vector<string>)
output: result(double)
*/
double calculate(const vector<string>& equations){
    double result = 0;
    stack<double> numStack; //stack for numbers
    stack<string> operStack; //stack for operators

    for(int index=0; index<equations.size(); index++){
        vector<string> instance;
        if(isdigit(equations[index][equations[index].length()-1])){ //number
            double num = stod(equations[index]);
            if(!operStack.empty()){
                if(operStack.top() == "-"){
                    num = -num;
                    operStack.pop();
                    operStack.push("+");
                }
            }
            numStack.push(num);
        }else if(equations[index] == "+" || equations[index] == "-"){ //arithmetic operators
            bool check = true;
            while(check){
                if(!operStack.empty()){
                    if(operStack.top() != "(" && (operStack.top() == "*" || operStack.top() == "/" || operStack.top() == "^")){
                        string operation = operStack.top();
                        operStack.pop();
                        double num1 = numStack.top();
                        numStack.pop();
                        double num2 = numStack.top();
                        numStack.pop();
                        numStack.push(compute2nums(num2, operation, num1));
                    }else{
                        break;
                    }
                }else{
                    break;
                }
            }
            if(equations[index] == "-"){
                if(numStack.empty() && !operStack.empty()){
                    if((operStack.top() == "(" || operStack.top() == "{")){
                        numStack.push(-1);
                        operStack.push("*");
                    }
                }else if(numStack.empty() && operStack.empty()){
                    numStack.push(-1);
                    operStack.push("*");
                }else {
                    operStack.push(equations[index]);
                }
            }else{
                operStack.push(equations[index]);
            }
        }else if(equations[index] == "*" || equations[index] == "/"){
            bool check = true;
            while(check){
                if(!operStack.empty()){
                    if(operStack.top() != "(" && operStack.top() == "^"){
                        string operation = operStack.top();
                        operStack.pop();
                        double num1 = numStack.top();
                        numStack.pop();
                        double num2 = numStack.top();
                        numStack.pop();
                        numStack.push(compute2nums(num2, operation, num1));
                    }else{
                        break;
                    }
                }else {
                    break;
                }
            }
            operStack.push(equations[index]);
        }else if(equations[index] == "^"){
            operStack.push(equations[index]);
        }else if(equations[index] == "("){ //parenthesis
            operStack.push(equations[index]);
        }else if(equations[index] == ")"){
            while(operStack.top() != "("){
                string operation = operStack.top();
                operStack.pop();
                double num1 = numStack.top();
                numStack.pop();
                double num2 = numStack.top();
                numStack.pop();
                numStack.push(compute2nums(num2, operation, num1));
            }
            operStack.pop();
        }else if(equations[index] == "{"){ //curly brackets
            operStack.push(equations[index]);
        }else if(equations[index] == "}"){
            while(operStack.top() != "{"){
                string operation = operStack.top();
                operStack.pop();
                double num1 = numStack.top();
                numStack.pop();
                double num2 = numStack.top();
                numStack.pop();
                numStack.push(compute2nums(num2, operation, num1));
            }
            operStack.pop();
        }else if(equations[index] == "sin"){ //trigonometic operations
            int openParen = 0;
            int closeParen = 0;
            do{
                index++;
                if(equations[index] == "("){
                    openParen++;
                }else if(equations[index] == ")"){
                    closeParen++;
                }
                instance.push_back(equations[index]);
            }while(openParen != closeParen);
            double parenVal = calculate(instance);
            numStack.push(sin(parenVal));
        }else if(equations[index] == "cos"){
            int openParen = 0;
            int closeParen = 0;
            do{
                index++;
                if(equations[index] == "("){
                    openParen++;
                }else if(equations[index] == ")"){
                    closeParen++;
                }
                instance.push_back(equations[index]);
            }while(openParen != closeParen);
            double parenVal = calculate(instance);
            numStack.push(cos(parenVal));
        }else if(equations[index] == "tan"){
            int openParen = 0;
            int closeParen = 0;
            do{
                index++;
                if(equations[index] == "("){
                    openParen++;
                }else if(equations[index] == ")"){
                    closeParen++;
                }
                instance.push_back(equations[index]);
            }while(openParen != closeParen);
            double parenVal = calculate(instance);
            numStack.push(tan(parenVal));
        }else if(equations[index] == "cot"){
            int openParen = 0;
            int closeParen = 0;
            do{
                index++;
                if(equations[index] == "("){
                    openParen++;
                }else if(equations[index] == ")"){
                    closeParen++;
                }
                instance.push_back(equations[index]);
            }while(openParen != closeParen);
            double parenVal = calculate(instance);
            numStack.push(compute2nums(cos(parenVal), "/", sin(parenVal)));
        }else if(equations[index] == "ln"){ //logarithmic operations
            int openParen = 0;
            int closeParen = 0;
            do{
                index++;
                if(equations[index] == "("){
                    openParen++;
                }else if(equations[index] == ")"){
                    closeParen++;
                }
                instance.push_back(equations[index]);
            }while(openParen != closeParen);
            double parenVal = calculate(instance);
            numStack.push(log(parenVal));
        }else if(equations[index] == "log"){
            int openParen = 0;
            int closeParen = 0;
            index += 1;
            do{
                index++;
                if(equations[index] == "("){
                    openParen++;
                }else if(equations[index] == ")"){
                    closeParen++;
                }
                instance.push_back(equations[index]);
            }while(openParen != closeParen);
            double parenVal = calculate(instance);
            numStack.push(log10(parenVal));
        }
    }
    //calculate rest of the operations in the operStack
    while(!operStack.empty()){
        string operation = operStack.top();
        operStack.pop();
        double num1 = numStack.top();
        numStack.pop();
        double num2 = numStack.top();
        numStack.pop();
        if(!operStack.empty()){
            if(operStack.top() == "-"){
                num2 = -num2;
                operStack.pop();
                operStack.push("+");
            }
        }
        numStack.push(compute2nums(num2, operation, num1));
    }

    return numStack.top();
}

//main function
int main(){
    //get arithmetic expression
    while(true){
        string input;
        cout << "Enter an arithmetic expression(Enter exit to finish): ";
        getline(cin, input);

        //convert to lowercase alphabets
        string expression;
        for(auto ch:input){
            expression += tolower(ch);
        }
        //exit
        if(expression == "exit"){
            break;
        }

        //split the expression into tokens
        vector<string> tokens;
        tokens = splitExpression(expression);
        if(tokens.size() == 0){
            continue;
        }

        //check if the tokens are correct
        vector<bool> validations;
        validations = checkTokens(tokens);

        //if validation has more than one false, inform incorrect characters
        //if all values are true, move on to the calculation
        bool valid = processTokens(tokens, validations);

        if(valid == false){
            continue;
        }

        //do the calculation
        try{
            double result = calculate(tokenizedExp);
            cout << "Result: " << result << endl;
        }catch(const exception& e) { //throws an error
            //cerr << "Error: " << e.what() << endl;
        }
    }
    return 0;
}