#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
    double x, y;
};

struct Segment {
    Point start, end;
};

struct Polygon {
    vector<Point> vertices;
};

struct Triangle {
    Point vertexA, vertexB, vertexC;
};

// Проверка на то, лежит ли точка справа от вектора
bool isPointClockwise(const Point& A, const Point& B, const Point& P) {
    return ((B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x)) < 0;
}

// Нахождение пересечения двух отрезков
Point computeSegmentIntersection(const Segment& segment1, const Segment& segment2) {
    double a1 = segment1.end.y - segment1.start.y;
    double b1 = segment1.start.x - segment1.end.x;
    double c1 = a1 * segment1.start.x + b1 * segment1.start.y;

    double a2 = segment2.end.y - segment2.start.y;
    double b2 = segment2.start.x - segment2.end.x;
    double c2 = a2 * segment2.start.x + b2 * segment2.start.y;

    double det = a1 * b2 - a2 * b1;

    double x = (b2 * c1 - b1 * c2) / det;
    double y = (a1 * c2 - a2 * c1) / det;

    return { x, y };
}

// Нахождение площади многоугольника
double calculatePolygonArea(const vector<Point>& polygon) {
    double area = 0.0;
    int n = polygon.size();

    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += polygon[i].x * polygon[j].y;
        area -= polygon[j].x * polygon[i].y;
    }

    return abs(area) / 2.0;
}

// Пересечение многоугольников
vector<Point> findPolygonIntersection(const Polygon& subjectPolygon, const Polygon& clipPolygon) {
    vector<Point> outputList = subjectPolygon.vertices;

    for (int i = 0; i < clipPolygon.vertices.size(); ++i) {
        int j = (i + 1) % clipPolygon.vertices.size();
        vector<Point> inputList = outputList;
        outputList.clear();
        Point S = inputList.back();

        for (const Point& E : inputList) {
            if (isPointClockwise(clipPolygon.vertices[i], clipPolygon.vertices[j], E)) {
                if (!isPointClockwise(clipPolygon.vertices[i], clipPolygon.vertices[j], S)) {
                    Segment segment1 = { S, E };
                    Segment segment2 = { clipPolygon.vertices[i], clipPolygon.vertices[j] };
                    outputList.push_back(computeSegmentIntersection(segment1, segment2));
                }
                outputList.push_back(E);
            }
            else if (isPointClockwise(clipPolygon.vertices[i], clipPolygon.vertices[j], S)) {
                Segment segment1 = { S, E };
                Segment segment2 = { clipPolygon.vertices[i], clipPolygon.vertices[j] };
                outputList.push_back(computeSegmentIntersection(segment1, segment2));
            }
            S = E;
        }
    }

    return outputList;
}

int main() {

    setlocale(LC_ALL, "rus");

    Point point1, point2, point3, point4, point5, point6;
    Triangle triangle1, triangle2;

    // Считываем координаты вершин для двух треугольников
    cin >> point1.x >> point1.y >> point2.x >> point2.y >> point3.x >> point3.y >> point4.x >> point4.y >> point5.x >> point5.y >> point6.x >> point6.y;
    triangle1 = { point1, point2, point3 };
    triangle2 = { point4, point5, point6 };

    // Преобразуем треугольники в многоугольники
    Polygon polygon1 = { {triangle1.vertexA, triangle1.vertexB, triangle1.vertexC} };
    Polygon polygon2 = { {triangle2.vertexA, triangle2.vertexB, triangle2.vertexC} };

    // Пересекаем многоугольники и вычисляем площадь пересечения
    vector<Point> intersectionPolygon = findPolygonIntersection(polygon1, polygon2);
    double intersectionArea = calculatePolygonArea(intersectionPolygon);

    // Выводим площадь пересечения
    cout << "Площадь пересечения: " << intersectionArea << endl;

    return 0;
}