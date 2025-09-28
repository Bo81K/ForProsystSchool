#include <bits/stdc++.h>
using namespace std;

// Структура для хранения координат точки
struct Point {
    long long x, y;
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

// Вычисляю векторное произведение OA × OB
// Используется для определения поворота (левый, правый или на одной линии)
long long cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Считаю расстояние между двумя точками
double dist(const Point& A, const Point& B) {
    long long dx = A.x - B.x;
    long long dy = A.y - B.y;
    return sqrt((double)dx*dx + (double)dy*dy);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; i++) cin >> pts[i].x >> pts[i].y;


    if (n == 1) {
        double perimeter = 2 * M_PI * 3; // только окружность радиуса 3
        long long rolls = (long long)ceil(perimeter / 5.0);
        cout << rolls << "\n";
        return 0;
    }

    if (n == 2) {
        double perimeter = 2 * dist(pts[0], pts[1]) + 2 * M_PI * 3;
        long long rolls = (long long)ceil(perimeter / 5.0);
        cout << rolls << "\n";
        return 0;
    }

    // Строю выпуклую оболочку методом монотонной цепи (алгоритм Эндрю)
    sort(pts.begin(), pts.end());
    vector<Point> hull;

    // Нижняя часть оболочки
    for (int i = 0; i < n; i++) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull.back(), pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    // Верхняя часть оболочки
    for (int i = n-2, t = hull.size()+1; i >= 0; i--) {
        while (hull.size() >= t && cross(hull[hull.size()-2], hull.back(), pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    hull.pop_back(); // последняя точка дублируется, удаляю её

    // Считаю периметр многоугольника
    double perimeter = 0.0;
    for (int i = 0; i < (int)hull.size(); i++) {
        perimeter += dist(hull[i], hull[(i+1)%hull.size()]);
    }
    perimeter += 2 * M_PI * 3;

    long long rolls = (long long)ceil(perimeter / 5.0);
    cout << rolls << "\n";
    return 0;
}
