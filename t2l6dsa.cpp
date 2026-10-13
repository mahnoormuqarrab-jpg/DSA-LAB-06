#include <iostream>
#include <string>
#include <vector>
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
};
class TagStack {
    string names[MAX];
    int positions[MAX];
    int top;
public:
    TagStack() { top = -1; }
    bool isEmpty() { return top == -1; }
    void push(string name, int pos) {
        top++;
        names[top] = name;
        positions[top] = pos;
    }
    void pop() { top--; }
    string topName() { return names[top]; }
    int topPos() { return positions[top]; }
};
bool isValidBrackets(string s) {
    CharStack st;
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        }
        else if (c == ')' || c == '}' || c == ']') {
            if (st.isEmpty()) return false;
            char open = st.pop();
            if (c == ')' && open != '(') return false;
            if (c == '}' && open != '{') return false;
            if (c == ']' && open != '[') return false;
        }
    }
    return st.isEmpty();
}
bool validate_markup(string s, vector<string>& errors) {
    TagStack st;
    int i = 0;
    while (i < s.length()) {
        if (s[i] != '<') { i++; continue; }
        int end = s.find('>', i);
        if (end == string::npos) {
            errors.push_back("Missing '>' for tag at position " + to_string(i));
            break;
        }
        string inside = s.substr(i + 1, end - i - 1);
        int pos = i;
        i = end + 1;

        if (inside.length() == 0) {
            errors.push_back("Empty tag at position " + to_string(pos));
        }
        else if (inside[0] == '/') {
            string name = inside.substr(1);
            if (st.isEmpty()) {
                errors.push_back("Extra closing tag </" + name + "> at position " + to_string(pos));
            }
            else if (st.topName() != name) {
                errors.push_back("Expected </" + st.topName() + "> but found </" + name + "> at position " + to_string(pos));
            }
            else {
                st.pop();
            }
        }
        else if (inside[inside.length() - 1] == '/') {
        }
        else {
            string name = "";
            for (int k = 0; k < inside.length() && inside[k] != ' '; k++)
                name += inside[k];
            st.push(name, pos);
        }
    }

    while (!st.isEmpty()) {
        errors.push_back("Tag <" + st.topName() + "> at position " + to_string(st.topPos()) + " was never closed");
        st.pop();
    }

    return errors.size() == 0;
}
int main() {
    cout << "--- Part A: Brackets ---" << endl;
    string a[] = { "{[()]}", "([{}])", "{[(])}", "((()", "a(b)[c]{d}", ")(", "" };
    for (int i = 0; i < 7; i++) {
        cout << "\"" << a[i] << "\"  ->  ";
        if (isValidBrackets(a[i])) cout << "True" << endl;
        else cout << "False" << endl;
    }
    cout << endl << "--- Part B: HTML/XML tags ---" << endl;
    string b[] = {
        "<html><body><h1>Hi</h1><br/></body></html>",
        "<div><p>Hello</div></p>",
        "<ul><li>One</li><li>Two</ul>",
        "<a href=\"x\">link</a></b>",
        "<div><span>text</span>"
    };
    for (int i = 0; i < 5; i++) {
        vector<string> errors;
        bool ok = validate_markup(b[i], errors);
        cout << b[i] << endl;
        cout << "  -> " << (ok ? "True" : "False") << endl;
        for (int k = 0; k < errors.size(); k++)
            cout << "     * " << errors[k] << endl;
    }
    return 0;
}