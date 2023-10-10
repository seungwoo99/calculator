//calculator.cpp
//CS480
//Simon Choi

//header files
#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <cmath> //for pow()
using namespace std;

/* compute2nums method
input: num2(double), operation(char), num1(double)
output: calculation result(double)
*/
double compute2nums(double num2, char operation, double num1){
    if(operation == '+'){
        return num2 + num1;
    }else if(operation == '-'){
        return num2 - num1;
    }else if(operation == '*'){
        return num2 * num1;
    }else if(operation == '/'){
        return num2 / num1;
    }else if(operation == '^'){
        return pow(num2, num1);
    }else {
        cerr << "Invalid operation: " << operation << endl;
        exit(1);
    }
}

/* getParenExp method
: returns expression in the parenthesis
input: ss(stringstream object)
output: expression in parenthesis(string)
*/
string getParenExp(stringstream &ss){
    char parenToken;
    ss >> parenToken;
    string logExpression;
    if(parenToken == '('){
        while(parenToken != ')'){
            logExpression += parenToken;
            ss >> parenToken;
        }
    }
    return logExpression+parenToken;
}

/* calculate method
input: aritmetic expression(string)
output: calculation result(double)
*/
double calculate(const string& expression){
    stack<double> numStack; //stack for numbers
    stack<char> operStack; //stack for operators

    stringstream ss(expression);
    char token;
    while(ss >> token){
        //number
        if(isdigit(token)){
            ss.putback(token);
            double num;
            ss >> num; //reads a double value in stream
            numStack.push(num);
        }else if(token == '+' || token == '-'){ //arithmetic operations
            while(!operStack.empty() && operStack.top() != '(' && (operStack.top() == '*' || operStack.top() == '/' || operStack.top() == '^')){
                char operation = operStack.top();
                operStack.pop();
                double num1 = numStack.top();
                numStack.pop();
                double num2 = numStack.top();
                numStack.pop();
                numStack.push(compute2nums(num2, operation, num1));
            }
            operStack.push(token);
        }else if(token == '*' || token == '/'){
            while(!operStack.empty() && operStack.top() != '(' && operStack.top() == '^'){
                char operation = operStack.top();
                operStack.pop();
                double num1 = numStack.top();
                numStack.pop();
                double num2 = numStack.top();
                numStack.pop();
                numStack.push(compute2nums(num2, operation, num1));
            }
            operStack.push(token);
        }else if(token == '^'){
            operStack.push(token);
        }else if(token == '('){ //parenthesis
            operStack.push(token);
        }else if(token == ')'){
            while(operStack.top() != '('){
                char operation = operStack.top();
                operStack.pop();
                double num1 = numStack.top();
                numStack.pop();
                double num2 = numStack.top();
                numStack.pop();
                numStack.push(compute2nums(num2, operation, num1));
            }
            operStack.pop();
        }else if(isalpha(token)){ //trigonometric operations
            string logarith;
            do {
                logarith += token;
                ss >> token;
                if(logarith == "ln"){
                    break;
                }
            }while(logarith.length() < 3);
            ss.putback(token);
            
            if(logarith == "sin"){
                string parenExpression = getParenExp(ss);
                double parenVal = calculate(parenExpression);
                numStack.push(sin(parenVal));
            }else if(logarith == "cos"){
                string parenExpression = getParenExp(ss);
                double parenVal = calculate(parenExpression);
                numStack.push(cos(parenVal));
            }else if(logarith == "tan"){
                string parenExpression = getParenExp(ss);
                double parenVal = calculate(parenExpression);
                numStack.push(tan(parenVal));
            }else if(logarith == "cot"){
                string parenExpression = getParenExp(ss);
                double parenVal = calculate(parenExpression);
                numStack.push(compute2nums(cos(parenVal), '/', sin(parenVal)));
            }else if(logarith == "ln"){
                string parenExpression = getParenExp(ss);
                double parenVal = calculate(parenExpression);
                numStack.push(log(parenVal));
            }else if(logarith == "log"){

            }
        }else {
            cerr << "Invalid character in the expression: " << token << endl;
            exit(1);
        }
    }
    //calculate rest of the operations in the operStack
    while(!operStack.empty()){
        char operation = operStack.top();
        operStack.pop();
        double num1 = numStack.top();
        numStack.pop();
        double num2 = numStack.top();
        numStack.pop();
        numStack.push(compute2nums(num2, operation, num1));
    }

    return numStack.top();
}


//main function
int main(){
    //get arithmetic expression
    string expression;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);

    //do the calculation
    try{
        double result = calculate(expression);
        cout << "Result: " << result << endl;
    }catch(const exception& e) { //throws an error
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}