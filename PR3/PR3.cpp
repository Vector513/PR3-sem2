#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

map <string, int> weight = { {"+", 1 },
                             {"-", 1 },
                             {"*", 2 },
                             {"/", 2 },
                             {"(", 0 },
                             {")", 0 } };

struct Stack 
{
    int value;
    Stack* next = nullptr;
};

void showStack(Stack* stack) 
{
    while (stack != nullptr) 
    {
        cout << stack->value << ' ';
        stack = stack->next;
    }
}

void push(Stack*& stack, int value) 
{
    Stack* new_elem = new Stack;
    new_elem->value = value;
    new_elem->next = stack;
    stack = new_elem;
}

void pop(Stack*& stack) 
{
    Stack* tmp = stack;
    stack = stack->next;
    delete tmp;
}

vector<string> tokenizer (const string& text) 
{
    vector<string> tokens{};
	istringstream input(text);

	for (string token; getline(input, token, ' '); /*nothing to do*/) {
		tokens.push_back(token);
	}

	return tokens;
}

int main() 
{
    Stack* stack = new Stack;
    stack->value = 1;
    
    push(stack, 6);
    push(stack, 7);
    push(stack, 8);
    pop(stack);
    showStack(stack);
    cout << '\n';

    string in;
    getline(cin, in);

    vector<string> tokens = tokenizer(in);

    for (int i = 0; i < tokens.size(); i++) {
        cout << tokens[i] << ' ';
    }
    cout << tokens.size();

    return 0;
}