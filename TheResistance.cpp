#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int main()
{
    string morse;
    cin >> morse; cin.ignore();
    int n;
    cin >> n; cin.ignore();

    // Храню азбуку Морзе для каждой буквы
    unordered_map<char, string> morseCodes = {
        {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
        {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
        {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
        {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
        {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
        {'Z', "--.."}
    };

    // Формирую словарь представлений слов в виде азбуки Морзе
    vector<string> words(n);
    for (int i = 0; i < n; i++) {
        cin >> words[i]; cin.ignore();
        string morseWord;
        for (char c : words[i]) {
            morseWord += morseCodes[toupper(c)];
        }
        words[i] = morseWord;
    }

    // Динамически рассчитываю количество допустимых разбиений
    vector<long long> dp(morse.length() + 1, 0);
    dp[0] = 1;

    for (int i = 0; i < morse.length(); i++) {
        if (dp[i] == 0) continue;
        for (const string& word : words) {
            if (i + word.length() <= morse.length() && morse.substr(i, word.length()) == word) {
                dp[i + word.length()] += dp[i];
            }
        }
    }

    cout << dp[morse.length()] << endl;
    return 0;
}
