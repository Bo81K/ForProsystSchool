#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    int x, y;
};

int main()
{
    int checkpoints;
    cin >> checkpoints; cin.ignore();
    vector<Point> checkpointList(checkpoints);
    for (int i = 0; i < checkpoints; i++) {
        cin >> checkpointList[i].x >> checkpointList[i].y; cin.ignore();
    }

    while (1) {
        int checkpointIndex, x, y, vx, vy, angle;
        cin >> checkpointIndex >> x >> y >> vx >> vy >> angle; cin.ignore();

        Point target = checkpointList[checkpointIndex % checkpointList.size()];
        
        // Вычисляю угол к целевому чекпоинту
        int dx = target.x - x;
        int dy = target.y - y;
        double targetAngle = atan2(dy, dx) * 180.0 / M_PI;
        if (targetAngle < 0) targetAngle += 360;

        // Нормализую разницу углов с учётом максимального поворота 18 градусов
        double angleDiff = targetAngle - angle;
        while (angleDiff > 180) angleDiff -= 360;
        while (angleDiff <= -180) angleDiff += 360;
        angleDiff = max(min(angleDiff, 18.0), -18.0); // ограничиваю диапазоном [-18; 18]

        // Вычисляю новый угол поворота
        int newAngle = static_cast<int>((angle + angleDiff + 360)) % 360;

        // Вычисляю расстояние до чекпоинта
        double distance = sqrt(dx * dx + dy * dy);
        
        // Определяю силу тяги в зависимости от расстояния
        int thrust;
        if (distance <= 300) {
            thrust = 0;
        } else if (distance <= 600) {
            thrust = 30;
        } else if (distance <= 1200) {
            thrust = 75;
        } else {
            thrust = 100;
        }

        cout << target.x << " " << target.y << " " << thrust << " approaching checkpoint" << endl;
    }
}
