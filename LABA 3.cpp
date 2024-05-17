#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <cctype>

using namespace std;

// Функция для получения приоритета оператора
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Функция для проверки является ли символ оператором
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Преобразование инфиксного выражения в обратную польскую нотацию (ОПН)  
string infixToPostfix(const string& expression) {
    stack<char> operators;
    string postfix;
    int step = 1;
    cout << "\nПреобразование инфиксного выражения в обратную польскую нотацию:\n";
    cout << "-------------------------------------------------------------\n";
    string currentNumber; // Добавляем текущее число
    for (char token : expression) {
        if (isdigit(token) || isalpha(token)) {
            currentNumber += token;
        }
        else if (isspace(token)) { // Пробелы разделяют числа
            if (!currentNumber.empty()) {
                postfix += currentNumber + " "; // Добавляем число в выходную строку с пробелом
                cout << step++ << ". Добавляем операнд '" << currentNumber << "' к выходной строке: " << postfix << "\n";
                currentNumber.clear(); // Очищаем текущее число
            }
            continue;
        }
        else {
            if (!currentNumber.empty()) {
                postfix += currentNumber + " ";
                cout << step++ << ". Добавляем операнд '" << currentNumber << "' к выходной строке: " << postfix << "\n";
                currentNumber.clear();
            }
        }
        if (token == '(') {
            operators.push(token);
            cout << step++ << ". Добавляем '(' в стек операторов\n";
        }
        else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                cout << step++ << ". Перемещаем оператор '" << operators.top() << "' из стека в выходную строку: " << postfix << "\n";
                operators.pop();
            }
            operators.pop();
            cout << step++ << ". Удаляем '(' из стека\n";
        }
        else if (isOperator(token)) {
            while (!operators.empty() && getPriority(operators.top()) >= getPriority(token)) {
                postfix += operators.top();
                cout << step++ << ". Перемещаем оператор '" << operators.top() << "' из стека в выходную строку: " << postfix << "\n";
                operators.pop();
            }
            operators.push(token);
            cout << step++ << ". Добавляем оператор '" << token << "' в стек операторов\n";
        }
    }
    if (!currentNumber.empty()) {
        postfix += currentNumber + " ";
        cout << step++ << ". Добавляем операнд '" << currentNumber << "' к выходной строке: " << postfix << "\n";
    }
    while (!operators.empty()) {
        postfix += operators.top();
        cout << step++ << ". Перемещаем оставшийся оператор '" << operators.top() << "' из стека в выходную строку: " << postfix << "\n";
        operators.pop();
    }
    cout << "-------------------------------------------------------------\n";
    return postfix;
}


// Преобразование обратной польской нотации в инфиксное выражение    
string postfixToInfix(const string& postfix) {
    stack<string> s;
    int step = 1;
    cout << "\nПреобразование обратной польской нотации в инфиксное выражение:\n";
    cout << "----------------------------------------------------------------\n";
    string currentNumber;
    for (char token : postfix) {
        if (isdigit(token) || isalpha(token)) {
            currentNumber += token;
        }
        else if (isspace(token)) {
            if (!currentNumber.empty()) {
                s.push(currentNumber);
                cout << step++ << ". Добавляем операнд '" << currentNumber << "' в стек: " << currentNumber << "\n";
                currentNumber.clear();
            }
            continue;
        }
        else {
            if (!currentNumber.empty()) {
                s.push(currentNumber);
                cout << step++ << ". Добавляем операнд '" << currentNumber << "' в стек: " << currentNumber << "\n";
                currentNumber.clear();
            }
        }
        if (isOperator(token)) {
            string b = s.top(); s.pop();
            string a = s.top(); s.pop();
            string exp = "(" + a + token + b + ")";
            s.push(exp);
            cout << step++ << ". Преобразуем '" << a << " " << token << " " << b << "' в '" << exp << "' и добавляем в стек\n";
        }
    }
    if (!currentNumber.empty()) {
        s.push(currentNumber);
        cout << step++ << ". Добавляем операнд '" << currentNumber << "' в стек: " << currentNumber << "\n";
    }
    cout << "----------------------------------------------------------------\n";
    return s.top();
}

// Проверка корректности инфиксного выражения
bool isValidInfixExpression(const string& expression) {
    stack<char> brackets;
    for (char token : expression) {
        if (token == '(') {
            brackets.push(token);
        }
        else if (token == ')') {
            if (brackets.empty()) return false;
            brackets.pop();
        }
    }
    return brackets.empty();
}

// Проверка корректности выражения в обратной польской нотации
bool isValidPostfixExpression(const string& expression) {
    int operandCount = 0;
    bool prevIsSpace = true; // Флаг для обработки пробелов между операндами
    for (char token : expression) {
        if (isdigit(token) || isalpha(token)) {
            if (prevIsSpace) { // Если предыдущий символ - пробел, увеличиваем счетчик операндов
                operandCount++;
                prevIsSpace = false;
            }
        }
        else if (isspace(token)) { // Если текущий символ - пробел, устанавливаем флаг в true
            prevIsSpace = true;
        }
        else if (isOperator(token)) {
            if (operandCount < 2) return false;
            operandCount--;
        }
    }
    return operandCount == 1;
}


// Вычисление значения выражения в обратной польской нотации
double evaluatePostfix(const string& postfix, const unordered_map<char, double>& variables) {
    stack<double> operands;
    int step = 1;
    cout << "\nВычисление выражения в обратной польской нотации:\n";
    cout << "------------------------------------------------\n";
    string operandStr; // Строка для формирования числового операнда
    for (char token : postfix) {
        if (isdigit(token) || token == '.') {
            operandStr += token; // Добавляем символ к строке операнда
        }
        else if (token == ' ' && !operandStr.empty()) {
            // Если встречаем пробел и строка операнда не пуста, это конец числа
            double operand = stod(operandStr); // Преобразуем строку в вещественное число
            operands.push(operand); // Добавляем число в стек операндов
            cout << step++ << ". Добавляем число '" << operand << "' в стек операндов\n";
            operandStr = ""; // Очищаем строку операнда для следующего числа
        }
        else if (isalpha(token)) {
            operands.push(variables.at(token));
            cout << step++ << ". Добавляем значение переменной '" << token << "' (" << variables.at(token) << ") в стек операндов\n";
        }
        else if (isOperator(token)) {
            double b = operands.top(); operands.pop();
            double a = operands.top(); operands.pop();
            double result;
            switch (token) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': result = a / b; break;
            }
            operands.push(result);
            cout << step++ << ". Выполняем операцию '" << a << " " << token << " " << b << "' и добавляем результат '" << result << "' в стек операндов\n";
        }
    }
    cout << "------------------------------------------------\n";
    return operands.top();
}

int main() {
    setlocale(LC_ALL, "Rus");
    string expression;
    unordered_map<char, double> variables;
    char choice;

    cout << "Выберите нотацию для ввода выражения (i - инфиксная, p - обратная польская): ";
    cin >> choice;
    cin.ignore(); // игнорировать оставшийся символ новой строки

    if (choice == 'i') {
        cout << "\nВведите инфиксное выражение: ";
        getline(cin, expression);

        if (!isValidInfixExpression(expression)) {
            cout << "Выражение некорректно.\n";
            return 1;
        }

        // Инициализация неизвестных переменных в введенном выражении
        for (char token : expression) {
            if (isalpha(token) && variables.find(token) == variables.end()) {
                int value;
                cout << "Введите значение для переменной " << token << ": ";
                cin >> value;
                variables[token] = value;
            }
        }

        // Преобразуем обратную польскую нотацию в инфиксное выражение для вывода
        string postfix = infixToPostfix(expression);
        cout << "\nВыражение в обратной польской нотации: " << postfix << "\n";

        // Вычисляем значение инфиксного выражения
        double result = evaluatePostfix(postfix, variables);
        cout << "\nРезультат вычисления: " << result << "\n";
    }
    else if (choice == 'p') {
        cout << "\nВведите выражение в обратной польской нотации: ";
        getline(cin, expression);

        if (!isValidPostfixExpression(expression)) {
            cout << "Выражение некорректно.\n";
            return 1;
        }

        for (char token : expression) {
            if (isalpha(token) && variables.find(token) == variables.end()) {
                int value;
                cout << "Введите значение для переменной " << token << ": ";
                cin >> value;
                variables[token] = value;
            }
        }

        // Преобразуем обратную польскую нотацию в инфиксное выражение для вывода
        string infix = postfixToInfix(expression);
        cout << "\nВыражение в инфиксной нотации: " << infix << "\n";


        // Вычисляем значение выражения в обратной польской нотации
        double result = evaluatePostfix(expression, variables);
        cout << "\nРезультат вычисления: " << result << "\n";
    }
    else {
        cout << "Некорректный выбор нотации.\n";
    }

    return 0;
}