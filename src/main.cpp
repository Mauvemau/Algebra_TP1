
#include <iostream>
#include "raylib.h"

using namespace std;

struct Line {
    Vector2 point1;
    Vector2 point2;
};

// -- Variables --

// Ventana
const int screenWidth = 800;
const int screenHeight = 450;

// Vectores
const int maxVectors = 4;
Line vectors[maxVectors];

// Intersecciones
const int maxIntersects = 6;
Vector2 intersections[maxIntersects];
int currectIntersection;

// -- Funciones --

namespace Intersections {
    // Añade una interseccion al array.
    void Add(Vector2 intersection);
    // Devuelve si la interseccion en cuestion ya se encuentra en el array.
    bool IsRepeated(Vector2 intersection);
    // Devuelve si la interseccion de una linea es invalida o si se encuentra fuera de rango.
    bool IsInvalid(Line l, Vector2 intersection);
    // Pasandole los datos de 2 lineas, encuentra la interseccion entre estas.
    Vector2 LineLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    // Encuentra todas las posibles intersecciones, añade las que son validas.
    void FindIntersections();
}

namespace Shapes {
    void CalculateTriangle();
    // Devuelve si la forma es un triangulo.
    bool IsTriangle();
    // Se ocupa de averiguar que forma es y hacer los calculos necesarios.
    void Manage();
}

#pragma region Vectors
// Devuelve la distancia entre 2 vectores.
float GetDistance(Vector2 v1, Vector2 v2);
// Devuelve el punto intermedio entre 2 vectores.
Vector2 PointBetweenTwoVectors(Vector2 v1, Vector2 v2);

void InitVectors();

#pragma endregion

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Algebra");
    SetTargetFPS(30);

    int currentVector = 0;
    bool par = true;
    currectIntersection = 0;
    InitVectors();

    while (!WindowShouldClose()) {
        // Input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && currentVector < maxVectors) {
            if (par) {
                vectors[currentVector].point1 = GetMousePosition();
                cout << "[" << currentVector + 1 << "] Punto 1: (" << vectors[currentVector].point1.x << ";" << vectors[currentVector].point1.y << ")" << endl;
                par = !par;
            }
            else {
                vectors[currentVector].point2 = GetMousePosition();
                cout << "[" << currentVector + 1 << "] Punto 2: (" << vectors[currentVector].point2.x << ";" << vectors[currentVector].point2.y << ")" << endl;
                par = !par;
                currentVector++;
            }
            if (currentVector == maxVectors) {
                Intersections::FindIntersections();
                Shapes::Manage();
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            currentVector = 0;
            par = true;
            currectIntersection = 0;
            InitVectors();
            cout << "Vectores reiniciados..." << endl;
        }
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < maxVectors; i++) {
            if (i < currentVector) {
                DrawLine(vectors[i].point1.x, vectors[i].point1.y, vectors[i].point2.x, vectors[i].point2.y, RED);
            }
            if (!par && currentVector < maxVectors) {
                DrawLine(vectors[currentVector].point1.x, vectors[currentVector].point1.y, GetMousePosition().x, GetMousePosition().y, RED);
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

namespace Intersections {

    void Add(Vector2 intersection) {
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

    bool IsInvalid(Line l, Vector2 intersection) {
        return (intersection.x < l.point1.x&& intersection.x < l.point2.x&& intersection.x
            || intersection.x > l.point1.x&& intersection.x > l.point2.x && intersection.x
            || intersection.y < l.point1.y&& intersection.y < l.point2.y&& intersection.y
            || intersection.y > l.point1.y&& intersection.y > l.point2.y && intersection.y);
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

    void FindIntersections() {
        for (int i = 0; i < maxVectors; i++) {
            for (int j = 0; j < maxVectors; j++) {
                Vector2 intersect = LineLineIntersection(vectors[i].point1.x, vectors[i].point1.y, vectors[i].point2.x, vectors[i].point2.y, vectors[j].point1.x, vectors[j].point1.y, vectors[j].point2.x, vectors[j].point2.y);
                if (!isnan(intersect.x) && !IsInvalid(vectors[i], intersect) && !IsInvalid(vectors[j], intersect) && !IsRepeated(intersect)) {
                    Add(intersect);
                    cout << "intersection found: (" << intersect.x << ";" << intersect.y << ")\n";
                }
            }
        }
    }

}

namespace Shapes {
    void CalculateTriangle() {
        float a = GetDistance(intersections[0], intersections[1]);
        cout << "a: " << a << "\n";
        float b = GetDistance(intersections[1], intersections[2]);
        cout << "b: " << b << "\n";
        float c = GetDistance(intersections[2], intersections[0]);
        cout << "c: " << c << "\n";
        float s = ((a + b + c) / 2);
        float area = sqrt(s * ((s - a) * (s - b) * (s - c)));
        cout << "The area of the triangle is: " << area << "\n";
    }

    bool IsTriangle() {
        return (currectIntersection == 3);
    }

    void Manage() {
        if (IsTriangle()) {
            cout << "The shape is a triangle!\n";
            CalculateTriangle();
        }
    }
}

#pragma region Vectors

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

void InitVectors() {
    for (int i = 0; i < maxVectors; i++) {
        vectors[i].point1 = { 0, 0 };
        vectors[i].point2 = { 0, 0 };
    }
}

#pragma endregion