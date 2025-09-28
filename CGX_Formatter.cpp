#include <iostream>
#include <string>
using namespace std;

int main() {
    int n; cin >> n; cin.ignore();

    // Считываю все строки входа в одну строку
    string input, line;
    for (int i = 0; i < n; ++i) {
        getline(cin, line);
        input += line;
    }

    // Убираю пробелы вне строковых литералов
    string clean;
    bool inString = false;
    for (char c : input) {
        if (c == '\'') {
            inString = !inString;
            clean.push_back(c);
        } else if (inString) {
            clean.push_back(c);
        } else {
            // игнорирую пробелы, табы и переносы строк вне строки
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') clean.push_back(c);
        }
    }

    int indent = 0;
    bool lineEnded = true;
    bool afterEq = false;   // флаг после знака '='

    // Лямбда для вывода отступов
    auto printIndent = [&](int cnt) { for (int k = 0; k < cnt; ++k) cout << ' '; };

    size_t i = 0;
    while (i < clean.size()) {
        char c = clean[i];

        // Обработка строкового литерала
        if (c == '\'') {
            size_t start = i; ++i;
            while (i < clean.size() && clean[i] != '\'') ++i;
            if (i < clean.size()) ++i; // включаю закрывающую кавычку
            string tok = clean.substr(start, i - start);
            if (!afterEq) {
                if (!lineEnded) cout << '\n';
                printIndent(indent);
            }
            cout << tok;
            lineEnded = false;
            afterEq = false;

        } else if (c == '(') {
            if (!lineEnded) cout << '\n';
            printIndent(indent);
            cout << '(' << '\n';
            lineEnded = true;
            indent += 4;
            afterEq = false;
            ++i;

        } else if (c == ')') {
            if (!lineEnded) cout << '\n';
            indent -= 4;
            printIndent(indent);
            cout << ')';
            lineEnded = false;
            afterEq = false;
            ++i;

        } else if (c == ';') {
            cout << ';';
            lineEnded = false;
            afterEq = false;
            ++i;

        } else if (c == '=') {
            cout << '=';
            afterEq = true;
            lineEnded = false;
            ++i;

        // Обработка идентификаторов и других токенов
        } else {
            size_t start = i;
            while (i < clean.size() && clean[i] != '(' && clean[i] != ')' && clean[i] != ';' && clean[i] != '=' && clean[i] != '\'') ++i;
            string tok = clean.substr(start, i - start);
            if (!afterEq) {
                if (!lineEnded) cout << '\n';
                printIndent(indent);
            }
            cout << tok;
            lineEnded = false;
            afterEq = false;
        }
    }

    cout << '\n';
    return 0;
}
