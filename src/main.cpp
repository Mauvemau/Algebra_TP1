
#include <iostream>
#include "raylib.h"

using namespace std;

const int screenWidth = 800;
const int screenHeight = 450;

const int cantidadVectores = 4;

struct Linea {
    Vector2 punto1;
    Vector2 punto2;
};

Linea vectores[cantidadVectores];
float longitudes[cantidadVectores];

const int maxIntersects = 6;
Vector2 intersections[maxIntersects];
int currectIntersection;

float GetDistance(Vector2 v1, Vector2 v2)
{
    float distX = v1.x - v2.x;
    float distY = v1.y - v2.y;
    return sqrt((distX * distX) + (distY * distY));
}

Vector2 PointBetweenTwoVectors(Vector2 v1, Vector2 v2) {
    Vector2 point;

    point.x = (v1.x + v2.x) / 2;
    point.y = (v1.y + v2.y) / 2;

    return point;
}

void CalculateTriangle() {
    Vector2 point = PointBetweenTwoVectors(intersections[1], intersections[2]);
    float triangleHeight = GetDistance(intersections[0], point);
    cout << "The distance between (" << intersections[0].x << ";" << intersections[0].y << ") and (" << point.x << ";" << point.y << ") is: " << triangleHeight << endl;

}

bool IsTriangle() {
    return (currectIntersection == 3);
}

void ShapeManager() {
    if (IsTriangle)
        CalculateTriangle();
}


#pragma region intersection

void AddIntersection(Vector2 intersection) {
    intersections[currectIntersection] = intersection;
    currectIntersection++;
}

bool IsRepeated(Vector2 intersection) {
    if (currectIntersection > 0) {
        for (int i = 0; i < currectIntersection; i++) {
            if (intersections[i].x == intersection.x && intersections[i].y == intersection.y) {
                return true;
            }
        }
    }
    else {
        return false;
    }
    return false;
}

bool OobIntersection(Linea l, Vector2 intersect) {
    return (intersect.x < l.punto1.x && intersect.x < l.punto2.x && intersect.x
        || intersect.x > l.punto1.x&& intersect.x > l.punto2.x&& intersect.x
        || intersect.y < l.punto1.y&& intersect.y < l.punto2.y&& intersect.y
        || intersect.y > l.punto1.y&& intersect.y > l.punto2.y&& intersect.y);
}

Vector2 LineLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    Vector2 intersection;

    intersection.x =
        (((x1 * y2 - y1 * x2) * (x3 - x4)) - ((x1 - x2) * (x3 * y4 - y3 * x4))) /
        (((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)));

    intersection.y =
        (((x1 * y2 - y1 * x2) * (y3 - y4)) - ((y1 - y2) * (x3 * y4 - y3 * x4))) /
        (((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)));

    return intersection;
}

void CompareAllLines() {
    for (int i = 0; i < cantidadVectores; i++) {
        for (int j = 0; j < cantidadVectores; j++) {
            Vector2 intersect = LineLineIntersection(vectores[i].punto1.x, vectores[i].punto1.y, vectores[i].punto2.x, vectores[i].punto2.y, vectores[j].punto1.x, vectores[j].punto1.y, vectores[j].punto2.x, vectores[j].punto2.y);
            if (!isnan(intersect.x) && !OobIntersection(vectores[i], intersect) && !OobIntersection(vectores[j], intersect) && !IsRepeated(intersect)) {
                AddIntersection(intersect);
                cout << "intersection found: (" << intersect.x << ";" << intersect.y << ")\n";
            }
        }
    }
}

#pragma endregion

void InitVectores() {
    for (int i = 0; i < cantidadVectores; i++) {
        vectores[i].punto1 = { 0, 0 };
        vectores[i].punto2 = { 0, 0 };
    }
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Algebra");
    SetTargetFPS(30);

    int vectorActual = 0;
    bool par = true;
    currectIntersection = 0;
    InitVectores();

    while (!WindowShouldClose()) {
        // Input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && vectorActual < cantidadVectores) {
            if (par) {
                vectores[vectorActual].punto1 = GetMousePosition();
                cout << "[" << vectorActual + 1 << "] Punto 1: (" << vectores[vectorActual].punto1.x << ";" << vectores[vectorActual].punto1.y << ")" << endl;
                par = !par;
            }
            else {
                vectores[vectorActual].punto2 = GetMousePosition();
                cout << "[" << vectorActual + 1 << "] Punto 2: (" << vectores[vectorActual].punto2.x << ";" << vectores[vectorActual].punto2.y << ")" << endl;
                par = !par;
                vectorActual++;
            }
            if (vectorActual == cantidadVectores) {
                CompareAllLines();
                ShapeManager();
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            vectorActual = 0;
            currectIntersection = 0;
            InitVectores();
            cout << "Vectores reiniciados..." << endl;
        }
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < cantidadVectores; i++) {
            if (i < vectorActual) {
                DrawLine(vectores[i].punto1.x, vectores[i].punto1.y, vectores[i].punto2.x, vectores[i].punto2.y, RED);
            }
            if (!par && vectorActual < cantidadVectores) {
                DrawLine(vectores[vectorActual].punto1.x, vectores[vectorActual].punto1.y, GetMousePosition().x, GetMousePosition().y, RED);
            }
        }
        for (int i = 0; i < currectIntersection; i++) {
            DrawCircle(intersections[i].x, intersections[i].y, 5.0f, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}