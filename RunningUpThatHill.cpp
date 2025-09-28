#include <bits/stdc++.h>
using namespace std;

const string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

// Нахожу индекс символа в алфавите
int idx(char c){ auto p = alphabet.find(c); return p==string::npos ? -1 : (int)p; }

// Вычисляю НОД двух чисел (классический алгоритм Евклида)
int gcd_int(int a,int b){ if(a<0) a=-a; if(b<0) b=-b; while(b){ int t=a%b; a=b; b=t; } return a; }

// Расширенный алгоритм Евклида для поиска коэффициентов x, y: ax + by = gcd(a, b)
long long extgcd(long long a,long long b,long long &x,long long &y){
    if(b==0){ x=1; y=0; return a; }
    long long x1,y1; long long g = extgcd(b, a%b, x1, y1);
    x = y1;
    y = x1 - (a/b)*y1;
    return g;
}

// Нахожу обратный элемент по модулю m (если он существует)
int modinv_int(int a,int m){
    long long x,y;
    long long g = extgcd((a%m + m)%m, m, x, y);
    if(g != 1) return -1;
    long long res = (x % m + m) % m;
    return (int)res;
}

// Умножение матриц по модулю mod
vector<vector<int>> matMul(const vector<vector<int>>& A, const vector<vector<int>>& B, int mod){
    int n = A.size(), k = A[0].size(), m = B[0].size();
    vector<vector<int>> C(n, vector<int>(m,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            long long sum = 0;
            for(int t=0;t<k;t++) sum += 1LL * A[i][t] * B[t][j];
            C[i][j] = (int)((sum % mod + mod) % mod);
        }
    }
    return C;
}

// Умножение матрицы на вектор по модулю mod
vector<int> matMulVec(const vector<vector<int>>& A, const vector<int>& v, int mod){
    int n = A.size(), m = A[0].size();
    vector<int> res(n,0);
    for(int i=0;i<n;i++){
        long long sum = 0;
        for(int j=0;j<m;j++) sum += 1LL * A[i][j] * v[j];
        res[i] = (int)((sum % mod + mod) % mod);
    }
    return res;
}

// Нахожу обратную матрицу по модулю mod методом Гаусса
vector<vector<int>> matInv(vector<vector<int>> A, int mod){
    int n = A.size();
    vector<vector<int>> I(n, vector<int>(n,0));
    for(int i=0;i<n;i++) I[i][i]=1;
    for(int col=0; col<n; col++){
        int pivot = -1;
        // Ищу строку с обратимым элементом в текущем столбце
        for(int row=col; row<n; row++){
            if(gcd_int(A[row][col], mod) == 1){ pivot = row; break; }
        }
        if(pivot == -1) return {};
        if(pivot != col){ swap(A[pivot], A[col]); swap(I[pivot], I[col]); }
        int inv = modinv_int((A[col][col]%mod+mod)%mod, mod);
        if(inv == -1) return {};
        // Нормирую текущую строку
        for(int j=0;j<n;j++){
            A[col][j] = (int)((1LL * A[col][j] * inv) % mod);
            I[col][j] = (int)((1LL * I[col][j] * inv) % mod);
        }
        // Обнуляю остальные строки в этом столбце
        for(int row=0; row<n; row++){
            if(row == col) continue;
            int factor = A[row][col];
            if(factor == 0) continue;
            for(int j=0;j<n;j++){
                A[row][j] = (A[row][j] - 1LL * factor * A[col][j]) % mod;
                if(A[row][j] < 0) A[row][j] += mod;
                I[row][j] = (I[row][j] - 1LL * factor * I[col][j]) % mod;
                if(I[row][j] < 0) I[row][j] += mod;
            }
        }
    }
    return I;
}

// Перевожу строку в последовательность чисел по алфавиту
vector<int> toNums(const string &s){
    vector<int> res;
    for(char c: s) res.push_back(idx(c));
    return res;
}

// Перевожу последовательность чисел обратно в строку
string fromNums(const vector<int> &v){
    string s;
    for(int x: v) s.push_back(alphabet[x]);
    return s;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Входные строки: зашифрованный текст, открытый текст, шифр для расшифровки и текст для шифрования
    string cipher, clear, cipher_to_dec, clear_to_enc;
    getline(cin, cipher);
    getline(cin, clear);
    getline(cin, cipher_to_dec);
    getline(cin, clear_to_enc);

    // Преобразую строки в числовые векторы
    vector<int> plain = toNums(clear);
    vector<int> cipherv = toNums(cipher);
    int L = plain.size();

    // Подбираю возможные размеры блоков (делители длины текста)
    vector<int> divisors;
    for(int d=2; d<=L; ++d) if(L % d == 0) divisors.push_back(d);

    const int MOD = 45;
    bool found = false;
    vector<vector<int>> M; // матрица шифрования

    // Перебираю возможные размеры блоков и пытаюсь восстановить матрицу M
    for(int n : divisors){
        int blocks = L / n;
        if(blocks < n) continue;

        // Формирую блоки открытого текста и шифротекста
        vector<vector<int>> pblocks(blocks, vector<int>(n));
        vector<vector<int>> cblocks(blocks, vector<int>(n));
        for(int b=0;b<blocks;b++){
            for(int j=0;j<n;j++){
                pblocks[b][j] = plain[b*n + j];
                cblocks[b][j] = cipherv[b*n + j];
            }
        }

        // Строю матрицы из последовательных блоков
        for(int start=0; start + n <= blocks && !found; start++){
            vector<vector<int>> P(n, vector<int>(n));
            vector<vector<int>> C(n, vector<int>(n));
            for(int col=0; col<n; col++){
                for(int row=0; row<n; row++){
                    P[row][col] = pblocks[start + col][row];
                    C[row][col] = cblocks[start + col][row];
                }
            }
            // Пытаюсь найти обратную матрицу к P и вычислить кандидата для M
            auto Pinv = matInv(P, MOD);
            if(Pinv.empty()) continue;
            auto Mcand = matMul(C, Pinv, MOD);

            // Проверяю, подходит ли кандидат для всех блоков
            bool ok = true;
            for(int b=0;b<blocks && ok;b++){
                vector<int> pb = pblocks[b];
                vector<int> cb_expect = matMulVec(Mcand, pb, MOD);
                for(int i=0;i<n;i++){
                    if(cb_expect[i] != cblocks[b][i]) { ok = false; break; }
                }
            }
            if(ok){
                M = Mcand;
                found = true;
                break;
            }
        }
        if(found) break;
    }

    // Если матрица не найдена — вывожу пустой результат
    if(!found){
        cout << endl << endl;
        return 0;
    }

    // Нахожу обратную матрицу для расшифровки
    auto Minv = matInv(M, MOD);
    int n = M.size();

    // Расшифровываю переданный шифртекст
    vector<int> cdec_nums = toNums(cipher_to_dec);
    string decoded="";
    for(int i=0;i<(int)cdec_nums.size(); i+=n){
        vector<int> blk(n);
        for(int j=0;j<n;j++) blk[j] = cdec_nums[i+j];
        auto plainblk = matMulVec(Minv, blk, MOD);
        decoded += fromNums(plainblk);
    }

    // Шифрую переданный открытый текст
    vector<int> cle_to_enc_nums = toNums(clear_to_enc);
    string encoded="";
    for(int i=0;i<(int)cle_to_enc_nums.size(); i+=n){
        vector<int> blk(n);
        for(int j=0;j<n;j++) blk[j] = cle_to_enc_nums[i+j];
        auto cblk = matMulVec(M, blk, MOD);
        encoded += fromNums(cblk);
    }

    cout << decoded << "\n" << encoded << "\n";
    return 0;
}
