#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int l;
    cin >> l; cin.ignore();
    int h;
    cin >> h; cin.ignore();
    string t;
    getline(cin, t);
    vector<string> asciiArt(h);
    for (int i = 0; i < h; i++) {
        getline(cin, asciiArt[i]);
    }

    // Перевожу ввод в верхний регистр и заменяю все недопустимые символы на '?'
    for(char &c : t) {
        if (!isalpha(c)) {
            c = '?';
        } else {
            c = toupper(c);
        }
    }

    // Вывожу ASCII-арт построчно
    for (int i = 0; i < h; i++) {
        for (char c : t) {
            int index = c - 'A';
            if (index < 0 || index > 26) index = 26; // Я использую '?' как последний символ
            for (int j = 0; j < l; j++) {
                cout << asciiArt[i][index * l + j];
            }
        }
        cout << endl;
    }

    return 0;
}
