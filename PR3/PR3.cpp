#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

const char separator[] = "------------------------------------------------------------------------------------------------------------------------";

map <string, int> weight = { {"+", 1},
                             {"-", 1},
                             {"*", 2},
                             {"/", 2},
                             {"(", 0},
                             {")", 0}, };

class StackVar
{
private:
    int size = 0;
    struct Stack {
        int var = 0;
        Stack* next = nullptr;
    };

    Stack* stack = new Stack;

public:
    int getSize()
    {
        return size;
    }

    void show()
    {
        Stack* curr = stack;
        while (curr) {
            cout << curr->var << ' ';
            curr = curr->next;
        }
    }

    void push(int var)
    {
        Stack* new_elem = new Stack;
        new_elem->var = var;
        new_elem->next = stack;
        stack = new_elem;
        size++;
    }

    void pop()
    {
        if (stack) {
            Stack* tmp = stack;
            stack = stack->next;
            delete tmp;
            size--;
        }
    }

    int top() 
    {
        return stack->var;
    }
};

class StackOp
{
private:
    int size = 0;
    struct Stack {
        string op = "";
        Stack* next = nullptr;
    };

    Stack* stack = new Stack;

public:
    int getSize()
    {
        return size;
    }

    void show()
    {
        Stack* curr = stack;
        while (curr) {
            cout << curr->op << ' ';
            curr = curr->next;
        }
    }

    void push(string op)
    {
        Stack* new_elem = new Stack;
        new_elem->op = op;
        new_elem->next = stack;
        stack = new_elem;
        size++;
    }

    void pop()
    {
        if (stack) {
            Stack* tmp = stack;
            stack = stack->next;
            delete tmp;
            size--;
        }
    }

    string top() 
    {
        return stack->op;
    }
};

bool isDigit(char letter) 
{
    return (letter >= '0' && letter <= '9');
}

bool isLetter(char letter) 
{
    return ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'));
}

bool isString(string& str) 
{
    for (int i = 0; i < str.length(); i++) {
        if (!isalpha(str[i])) {
            return false;
        }
    }

    return true;
}

bool isNumber(string& str) 
{
    if ((str[0] == '-' && str.length() > 1) || (isdigit(str[0]))) {
        for (int i = 1; i < str.length(); i++) {
            if (!isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }
    
    return false;
}

int countParams(vector<string>& tokens) 
{
    int res = 0;
    for (int i = 0; i < tokens.size(); i++) {
        if (isNumber(tokens[i]) || isString(tokens[i])) res++;
    }

    return res;
}

int countOperators(vector<string>& tokens)
{
    int res = 0;
    for (int i = 0; i < tokens.size(); i++) {
        if (weight.count(tokens[i]) && tokens[i] != "(" && tokens[i] != ")") res++;
    }

    return res;
}

void showTokens(vector<string>& tokens)
{
    if (!tokens.empty()) {
        for (int i = 0; i < tokens.size(); i++) {
            cout << tokens[i] << ' ';
        }
    }
    else {
        cout << "Запись ещё не была введена или вычислена";
    }
    cout << '\n';
}

vector<string> tokenizer(const string& text)
{
    vector<string> tokens{};

    string tmp = "";
    string elem = "";
    for (int i = 0; i < text.length(); i++) {
        tmp += text[i];
        if (isDigit(text[i])) {
            elem += text[i];
        }
        else if (isLetter(text[i])) {
            elem += text[i];
        }
        else if (text[i] == ' ') {
            if (elem != "") {
                tokens.push_back(elem);
                elem = "";
            }
        }
        else if (weight.count(tmp)) {
            if (!(text[i] == '-' && ((i + 1) < text.length()) && (isDigit(text[i + 1]) || isLetter(text[i + 1])))) {
                if (elem != "") {
                    tokens.push_back(elem);
                    elem = "";
                }
                elem += text[i];
                tokens.push_back(elem);
                elem = "";
            }
            else {
                elem += text[i];
            }
        }
        if ((i + 1) == text.length() && elem != "") {
            tokens.push_back(elem);
        }

        tmp = "";
    }

    return tokens;
}

vector<string> reverseTokens(vector<string>& tokens)
{
    vector<string> reversedTokens;

    for (int i = tokens.size() - 1; i >= 0; i--) {
        if (tokens[i] == "(") {
            reversedTokens.push_back(")");
        }
        else if (tokens[i] == ")") {
            reversedTokens.push_back("(");
        }
        else {
            reversedTokens.push_back(tokens[i]);
        }
    }

    return reversedTokens;
}

string infPost(vector<string>& tokens) 
{
    string post = "";
    StackOp stackOp;

    int j = 1;
    for (int i = 0; i < tokens.size(); i++) {
        if (weight.count(tokens[i])) {
            if (stackOp.top() == "" || weight[tokens[i]] > weight[stackOp.top()] || tokens[i] == "(") {
                cout << i+j << ". " << tokens[i] << " Помещаем в стек\n";
                stackOp.push(tokens[i]);
            }
            else if (tokens[i] == ")") {
                while (stackOp.top() != "(") {
                    cout << i+j << ". " << stackOp.top() << " Помещаем в строку из стека" << "\n";
                    post += stackOp.top() + ' ';
                    stackOp.pop();
                    j++;
                }
                stackOp.pop();
            }
            else {
                while (stackOp.top() != "" && weight[stackOp.top()] >= weight[tokens[i]]) {
                    cout << i+j << ". " << stackOp.top() << " Помещаем в строку из стека\n";
                    post += stackOp.top() + ' ';
                    stackOp.pop();
                    j++;
                }
                cout << i+j << ". " << tokens[i] << " Помещаем в стек\n";
                stackOp.push(tokens[i]);
            }
        }

        else if (isString(tokens[i]) || isNumber(tokens[i])) {
            cout << i+j << ". " << tokens[i] << " Помещаем в строку\n";
            post += tokens[i] + ' ';
        }
    }

    j += tokens.size();

    int end = stackOp.getSize();
    for (int i = 0; i <= end; i++) {
        cout << i+j << ". " << stackOp.top() << " Помещаем в строку из стека\n";
        post += stackOp.top() + ' ';
        stackOp.pop();
    }

    return post;
}

string infPref(vector<string>& tokens) 
{
    string pref = "";
    vector<string> reversedTokens;
    vector<string> prefTokens;
    reversedTokens = reverseTokens(tokens);

    cout << "Разворачиваем строку\n";
    cout << "Алгоритм из инфиксной в постфиксную\n";

    pref = infPost(reversedTokens);
    
    prefTokens = tokenizer(pref);
    prefTokens = reverseTokens(prefTokens);
    
    cout << "Разворачиваем строку\n";

    pref = "";
    for (int i = 0; i < prefTokens.size(); i++) {
        pref += prefTokens[i] + ' ';
    }
    
    return pref;
}

bool verifyExpr(vector<string>& expr) {
    int count = 0;
    for (int i = 0; i < expr.size(); i++) {
        if (expr[i] == "(") count++;
        else if (expr[i] == ")") count--;
    }
    if (expr.empty())  return false;
    if (count != 0) {
        return false;
    }
    if (countParams(expr) - countOperators(expr) != 1) {
        return false;
    }
    
    return true;
}

int calc(int a, int b, string& op) 
{
    if (op == "+") {
        b += a;
    }
    else if (op == "-") {
        b -= a;
    }
    else if (op == "*") {
        b *= a;
    }
    else if (op == "/") {
        b /= a;
    }

    return b;
}

int calcPost(vector<string>& tokens, vector<pair<string, int>>& dict) 
{
    StackVar stackVar;
    int j = 0, a, b;
    for (int i = 0; i < tokens.size(); i++) {
        if (isString(tokens[i])) {
            if (dict[j].first == tokens[i]) {
                stackVar.push(dict[j].second);
                cout << i+1 << ". Кладём " << dict[j].second << " в стек\n";
                j++;
            }
        }
        else if (weight.count(tokens[i])) {
            a = stackVar.top();
            stackVar.pop();
            b = stackVar.top();
            stackVar.pop();
            cout << i + 1 << ". Достаём два верхних элемента стека " << a << " " << b << " и проделываем операцию " << tokens[i] << ". Результат " << calc(a, b, tokens[i]) << " кладём в стек\n";
            stackVar.push(calc(a, b, tokens[i]));
        }
        else if (isNumber(tokens[i])) {
            stackVar.push(stoi(tokens[i]));
            cout << i + 1 << ". Кладём число " << stoi(tokens[i]) << " в стек\n";
        }
    }

    return stackVar.top();
}

int calcPref(vector<string>& tokens, vector<pair<string, int>>& dict) 
{
    StackVar stackVar;
    int j = dict.size()-1, a, b;
    for (int i = tokens.size()-1; i >= 0; i--) {
        if (isString(tokens[i])) {
            if (dict[j].first == tokens[i]) {
                stackVar.push(dict[j].second);
                j--;
            }
        }
        else if (weight.count(tokens[i])) {
            b = stackVar.top();
            stackVar.pop();
            a = stackVar.top();
            stackVar.pop();
            stackVar.push(calc(a, b, tokens[i]));
        }
        else if (isNumber(tokens[i])) {
            stackVar.push(stoi(tokens[i]));
        }
    }

    return stackVar.top();
}

// (3 + 8) * (2 - 5) - ((125 + 53) / (613 * 623) - 35) / 2 + (16 + 16) * 3

void launch() 
{
    vector<string> tokens;
    vector<string> tokensPost;
    vector<string> tokensPref;
    vector<pair<string, int>> dict;

    string inf = "";
    string post = "";
    string pref = "";

    char commands[] = "1) Ввод простого выражения\n2) Преобразование введённого выражения в обратную польскую нотацию\n3) Преобразование введённого выражения в прямую польскую нотацию\n4) Проверка на корректность простого выражения\n5) Проверка на корректность обратной польской нотации\n6) Проверка на корректность прямой польской нотации\n7) Вычисление простого выражения\n8) Вычисление выражения в обратной польской нотации\n9) Вычисление выражения в прямой польской нотации\ns) Вывести текущие выражения\nc) Вывод списка комманд\ne) Выход из программы\n";

    char command = 'c';

    do {
        if (command == 'e') {
            cout << "Программа была завершена по воле пользователя";
            break;
        }

        else if (command == 'c') {
            cout << commands;
        }

        else if (command == 's') {
            if (inf != "") {
                cout << "Простое выражение: " << inf << '\n';
            }
            else {
                cout << "Простое выражение не было введено пользователем\n";
            }
            if (post != "") {
                cout << "Выражение в обратной польской записи: " << post << '\n';
            }
            else {
                cout << "Выражение в обратной польской записи ещё не было вычислено\n";
            }
            if (pref != "") {
                cout << "Выражение в прямой польской записи: " << pref << '\n';
            }
            else {
                cout << "Выражение в прямой польской записи ещё не было вычислено\n";
            }
        }

        else if (command == '1') {
            dict.clear();
            tokensPost.clear();
            tokensPref.clear();
            post = "";
            pref = "";
            cout << "Введите выражение: ";
            getline(cin, inf);
            getline(cin, inf);

            tokens = tokenizer(inf);

            for (int i = 0; i < tokens.size(); i++) {
                if (isString(tokens[i])) {
                    cout << tokens[i] << " = ";
                    int tmp;
                    cin >> tmp;
                    if (cin.fail()) {
                        i--;
                    }
                    else {
                        dict.push_back(make_pair(tokens[i], tmp));
                    }
                }
            }

            cout << "Переменные были введены\n";
        } 

        else if (command == '2') {
            post = infPost(tokens);
            tokensPost = tokenizer(post);
            cout << post << '\n';
        }
        
        else if (command == '3') {
            pref = infPref(tokens);
            tokensPref = tokenizer(pref);
            cout << pref << '\n';
            
        }

        else if (command == '4') {
            if (!tokens.empty()) {
                showTokens(tokens);

                if (!verifyExpr(tokens)) {
                    cout << "Запись простого выражения некорректна\n" << "Введите выражение заново\n";
                }

                else {
                    cout << "Запись простого выражения корректна\n";
                }
            }
            else {
                cout << "Запись ещё не была введена\n";
            }
        }

        else if (command == '5') {
            if (!tokensPost.empty()) {
                showTokens(tokensPost);

                if (!verifyExpr(tokensPost)) {
                    cout << "Запись выражения в обратной нотации некорректна\n" << "Введите выражение заново\n";
                }

                else {
                    cout << "Запись выражения в обратной нотации корректна\n";
                }
            }
            else {
                cout << "Запись ещё не была вычислена\n";
            }
        }

        else if (command == '6') {
            if (!tokensPref.empty()) {
                showTokens(tokensPref);

                if (!verifyExpr(tokensPost)) {
                    cout << "Запись выражения в прямой нотации некорректна\n" << "Введите выражение заново\n";
                }

                else {
                    cout << "Запись выражения в обратной нотации корректна\n";
                }
            }
            else {
                cout << "Запись ещё не была вычислена\n";
            }
        }

        else if (command == '7') {
            int res = 0;
            if (!tokens.empty()) {
                if (tokensPost.empty()) {
                    post = infPost(tokens);
                    tokensPost = tokenizer(post);
                }
                res = calcPost(tokensPost, dict);
                cout << "\nРезультат: " << res << '\n';
            }
            else {
                cout << "Запись ещё не была вычислена\n";
            }
        }

        else if (command == '8') {
            int res = 0;
            if (!tokensPost.empty()) {
                res = calcPost(tokensPost, dict);
                cout << "\nРезультат: " << res << '\n';
            }
            else {
                cout << "Запись ещё не была вычислена\n";
            }
        }

        else if (command == '9') {
            int res = 0;
            if (!tokensPref.empty()) {
                res = calcPost(tokensPost, dict);
                cout << "\nРезультат: " << res << '\n';
            }
            else {
                cout << "Запись ещё не была вычислена\n";
            }
        }

        cout << separator << '\n';
        cout << "Введите комманду: ";
        cin >> command;
        cout << '\n';

    } while (true);
}

int main() 
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    
    launch();
    

    return 0;
}