#include <iostream>
#include <string>
#include <cmath>
using namespace std;
const int MAX = 100;
class CharStack {
    char arr[MAX];
    int top;
public:
    CharStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    void push(char c) { arr[++top] = c; }
    char pop() { return arr[top--]; }
    char peek() { return arr[top]; }
};
class StringStack {
    string arr[MAX];
    int top;
public:
    StringStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    void push(string s) { arr[++top] = s; }
    string pop() { return arr[top--]; }
};
class DoubleStack {
    double arr[MAX];
    int top;
public:
    DoubleStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    void push(double d) { arr[++top] = d; }
    double pop() { return arr[top--]; }
};
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}
string reverseString(string s) {
    string r = "";
    for (int i = s.length() - 1; i >= 0; i--)
        r += s[i];
    return r;
}

string convert(string expr, bool prefixMode) {
    CharStack st;
    string result = "";

    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (c == ' ') continue;

        if (isalnum(c)) {
            result += c;
        }
        else if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            while (!st.isEmpty() && st.peek() != '(')
                result += st.pop();
            if (!st.isEmpty()) st.pop();
        }
        else if (isOperator(c)) {
            while (!st.isEmpty() && st.peek() != '(') {
                int topP = precedence(st.peek());
                int curP = precedence(c);
                bool shouldPop;
                if (!prefixMode) {
                    shouldPop = (topP > curP) || (topP == curP && c != '^');
                } else {
                    shouldPop = (topP > curP) || (topP == curP && c == '^');
                }
                if (!shouldPop) break;
                result += st.pop();
            }
            st.push(c);
        }
    }
    while (!st.isEmpty())
        result += st.pop();

    return result;
}

string infix_to_postfix(string expr) {
    return convert(expr, false);
}

string infix_to_prefix(string expr) {
    string rev = reverseString(expr);
    for (int i = 0; i < rev.length(); i++) {
        if (rev[i] == '(') rev[i] = ')';
        else if (rev[i] == ')') rev[i] = '(';
    }
    return reverseString(convert(rev, true));
}

string postfix_to_infix(string expr) {
    StringStack st;
    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (c == ' ') continue;
        if (isOperator(c)) {
            string b = st.pop();
            string a = st.pop();
            st.push("(" + a + c + b + ")");
        } else {
            st.push(string(1, c));
        }
    }
    return st.pop();
}

double calculate(double a, double b, char op) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    return pow(a, b);
}

double evaluate_postfix(string expr) {
    DoubleStack st;
    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];
        if (c == ' ') continue;
        if (isdigit(c)) {
            st.push(c - '0');
        } else {
            double b = st.pop();
            double a = st.pop();
            st.push(calculate(a, b, c));
        }
    }
    return st.pop();
}

double evaluate_prefix(string expr) {
    DoubleStack st;
    for (int i = expr.length() - 1; i >= 0; i--) {
        char c = expr[i];
        if (c == ' ') continue;
        if (isdigit(c)) {
            st.push(c - '0');
        } else {
            double a = st.pop();
            double b = st.pop();
            st.push(calculate(a, b, c));
        }
    }
    return st.pop();
}

int main() {
    string tests[] = { "A+B*C", "(A+B)*C", "A+B*C-D/E", "A^B^C", "(A+B)*(C-D)/E" };

    cout << "--- Infix to Postfix and Prefix ---" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Infix   : " << tests[i] << endl;
        cout << "Postfix : " << infix_to_postfix(tests[i]) << endl;
        cout << "Prefix  : " << infix_to_prefix(tests[i]) << endl << endl;
    }

    cout << "--- Postfix to Infix ---" << endl;
    cout << "ABC*+   =>  " << postfix_to_infix("ABC*+") << endl;
    cout << "AB+CD-* =>  " << postfix_to_infix("AB+CD-*") << endl << endl;

    cout << "--- Evaluation ---" << endl;
    string exp = "(3+4)*2-6/3";
    string post = infix_to_postfix(exp);
    string pre = infix_to_prefix(exp);
    cout << "Infix   : " << exp << endl;
    cout << "Postfix : " << post << "  = " << evaluate_postfix(post) << endl;
    cout << "Prefix  : " << pre << "  = " << evaluate_prefix(pre) << endl;
    cout << "evaluate_postfix(\"512+4*+3-\") = " << evaluate_postfix("512+4*+3-") << endl;
    cout << "evaluate_prefix(\"-+5*+1243\")  = " << evaluate_prefix("-+5*+1243") << endl;

    return 0;
}