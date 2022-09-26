
#include <iostream>
#include "raylib.h"

using namespace std;

struct Line {
    Vector2 point1;
    Vector2 point2;
};

struct Intersection {
    Vector2 pos; // La posicion de la interseccion.
    int lines[2]; // Cuales lineas conforman esta interseccion.
};

// -- Variables --

// Ventana
const int screenWidth = 800;
const int screenHeight = 450;

// Vectores
const int maxVectors = 4;
Line vectors[maxVectors];

int currentVector;
// Si es par, para el imput;
bool par;

// Intersecciones
const int maxIntersects = 6;
Intersection intersections[maxIntersects];
int currectIntersection;

// Formas
int shapeCode[maxVectors];

// Pi
const float m_pi = 3.14159265358979323846;

// Display
const char* shapeName;
float shapeArea = 0;

// -- Funciones --

// Paso 4; Calculo de la forma.
// Paso 3; Identificacion de forma.

namespace Shapes {
    // Hace lo mismo que CalculateTriangle, pero devuelve el valor.
    float CalculateTriangleArea(float a, float b, float c);
    // Identifica si el cuadrilatero es convexo o concavo, define por donde cortarlo en 2 triangulos.
    void FindQuadrilateral(int intersects[]);
    // Calcula el area de un cuadrilatero.
    void CalculateQuadrilateral();
    // Encuentra las intersecciones que componen al triangulo.
    void FindTriangle(int intersects[]);
    // Calcula el area del traingulo.
    void CalculateTriangle();
    // Devuelve si la forma es cuadrilateral.
    bool IsQuadrilateral(int code[]);
    // Devuelve si la forma es un triangulo.
    bool IsTriangle(int code[]);
    // Checkea si dos codigos son iguales.
    bool CompareCodes(int c1[], int c2[]);
    // Reinicia el codigo de la forma.
    void ResetShapeCode();
    // Se ocupa de averiguar que forma es y hacer los calculos necesarios.
    void Manage();
}

// Paso 2; busqueda de intersecciones.

namespace Intersections {
    // Añade una interseccion al array.
    void Add(Vector2 intersection, int l1, int l2);
    // Devuelve si la interseccion en cuestion ya se encuentra en el array.
    bool IsRepeated(Vector2 intersection);
    // Devuelve si la interseccion de una linea es invalida o si se encuentra fuera de rango.
    bool IsInvalid(Line l, Vector2 intersection);
    // Pasandole los datos de 2 lineas, encuentra la interseccion entre estas.
    Vector2 LineLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
    // Encuentra todas las posibles intersecciones, añade las que son validas.
    void FindIntersections();
}

// Paso 1; Programa base y Input del usuario.

namespace Program {
    void ManageInput();
    void Close();
    void Draw();
    void Update();
    void Init();
}

// Funciones Utiles

#pragma region Vectors
// Recibe 2 vectores, y un vector centro, devuelve el angulo entre los 3 puntos.
float GetAngle(Vector2 v1, Vector2 v2, Vector2 c);
// Devuelve la distancia entre 2 vectores.
float GetDistance(Vector2 v1, Vector2 v2);
void InitVectors();

#pragma endregion

#pragma region Tools
// Cambia el valor de grados a radianes.
float DegreesToRadians(float deg);
// Cambia el valor de radianes a grados.
float RadiansToDegrees(float rad);
// Dibuja texto centrado.
void DrawCenteredText(const char* text, Vector2 pos, int fontSize, Color color);
// Devuelve si el valor se encuentra en el array.
bool IsInArray(int arr[], int size, int value);
// Copia un array viejo en uno nuevo.
void CopyArray(int oldArr[], int newArr[], int size);
// Ordena los valores del codigo de la forma de mayor a menor.
void ArrangeArray(int arr[], int size);
// Imprime el codigo de la forma en la pantalla.
void PrintArray(int arr[], int size);

#pragma endregion

// -------------------------------------------
// Source

int main() {
    Program::Init();

    Program::Update();

    Program::Close();

    return 0;
}

namespace Intersections {

    void Add(Vector2 intersection, int l1, int l2) {
        intersections[currectIntersection].pos = intersection;
        intersections[currectIntersection].lines[0] = l1;
        intersections[currectIntersection].lines[1] = l2;
        currectIntersection++;
    }

    bool IsRepeated(Vector2 intersection) {
        if (currectIntersection > 0) {
            for (int i = 0; i < currectIntersection; i++) {
                if (intersections[i].pos.x == intersection.x && intersections[i].pos.y == intersection.y) {
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
                    Add(intersect, i, j);

                    shapeCode[i]++;
                    shapeCode[j]++;
                    cout << "intersection found: (" << intersect.x << ";" << intersect.y << ") Lines: [" << i << " & " << j << "]\n";
                }
            }
        }
    }
}

namespace Shapes {

    float CalculateTriangleArea(float a, float b, float c) {
        /*
        Resuelto con la formula de Heron
        https://en.wikipedia.org/wiki/Heron%27s_formula
        */
        float s = ((a + b + c) / 2);
        return sqrt(s * ((s - a) * (s - b) * (s - c)));
    }

    void FindQuadrilateral(int intersects[]) {
        /*
        Esto podria estar harcodeado, ya que si el main es 0, no importa como el usuario haga el cuadrilatero,
        sus parientes siempre van a ser 1 y 2, y su opuesto 3.
        Pero igualmente me rompí el coco haciendo este algoritmo, por que no?
        */
        int main = 0; // Agarramos una insterseccion cualquiera, en este caso la 0.
        int parents[2] = { 0, 0 }; // Esa interseccion va a tener si o si 2 intersecciones parientes.
        int currentParent = 0;
        // Buscamos quienes son esos parientes y los asignamos a parents.
        for (int i = 1; i < currectIntersection; i++) {
            if (intersections[i].lines[0] == intersections[main].lines[0] ||
                intersections[i].lines[0] == intersections[main].lines[1] ||
                intersections[i].lines[1] == intersections[main].lines[0] ||
                intersections[i].lines[1] == intersections[main].lines[1]) {
                parents[currentParent] = i;
                currentParent++;
            }
        }
        int opposite = 0; // Ya que hay un main y 2 parientes, tambien tiene que haber un opuesto.
        for (int i = 0; i < currectIntersection; i++) {
            if (i != main && i != parents[0] && i != parents[1])
                opposite = i;
        }
        cout << "Main Intersect: " << main <<
            "\nParent A: " << parents[0] <<
            "\nParent B: " << parents[1] <<
            "\nOpposite: " << opposite << "\n";
        // Ahora que ya sabemos el main, sus parientes y el opuesto, calculamos los 4 angulos que componen al cuadrilatero.
        float mainAng = RadiansToDegrees(GetAngle(intersections[parents[0]].pos, intersections[parents[1]].pos, intersections[main].pos));
        float parent1Ang = RadiansToDegrees(GetAngle(intersections[opposite].pos, intersections[main].pos, intersections[parents[0]].pos));
        float parent2Ang = RadiansToDegrees(GetAngle(intersections[opposite].pos, intersections[main].pos, intersections[parents[1]].pos));
        float oppositeAng = RadiansToDegrees(GetAngle(intersections[parents[0]].pos, intersections[parents[1]].pos, intersections[opposite].pos));
        cout << "Main Angle: " << mainAng <<
            "\nParent A Angle: " << parent1Ang <<
            "\nParent B Angle: " << parent2Ang <<
            "\nOpposite Angle: " << oppositeAng << "\n";
        // Los angulos siempre se van a calular de su lado concavo. Si la suma de todos los angulos da 360 aprox, el cuadrilatero no es concavo.
        if ((mainAng + parent1Ang + parent2Ang + oppositeAng) > 358) {
            cout << "It's a Convex Quadrilateral.\n";
            shapeName = "Convex Quadrilateral";
            intersects[0] = main;
            intersects[1] = opposite;
        }
        else {
            // En caso de ser concavo, la suma de opuestos que sea mas alta define por donde cortar el cuadrilatero.
            cout << "It's a Concave Quadrilateral.\n";
            shapeName = "Concave Quadrilateral";
            if (mainAng + oppositeAng > parent1Ang + parent2Ang) {
                cout << "Slicing triangles from main to opposite.\n";
                intersects[0] = main;
                intersects[1] = opposite;
            }
            else {
                cout << "Slicing triangles from parent A to parent B.\n";
                intersects[0] = parents[0];
                intersects[1] = parents[1];
            }
        }
    }

    void CalculateQuadrilateral() {
        // Para calcular el area del cuadrilatero, lo vamos a dividir en 2 triangulos.
        int mainIntersects[2]; // Para saber por donde cortarlo, primero necesitamos elegir intersecciones opuestas, estas se van a llamar main.
        int parentIntersects[2] = {0, 0}; // Las otras dos intersecciones se van a llamar parents.
        int parentCount = 0;
        FindQuadrilateral(mainIntersects); // Antes de cortar el cuadrilatero en 2 triangulos, necesitamos saber si es concavo o convexo y setear main acordemente.
        for (int i = 0; i < currectIntersection; i++) {
            if (i != mainIntersects[0] && i != mainIntersects[1]) {
                parentIntersects[parentCount] = i;
                parentCount++;
            }
        }
        // Hipotenusa, la comparten ambos triangulos.
        float a = GetDistance(intersections[mainIntersects[0]].pos, intersections[mainIntersects[1]].pos);

        float b = GetDistance(intersections[mainIntersects[0]].pos, intersections[parentIntersects[0]].pos);
        float c = GetDistance(intersections[mainIntersects[1]].pos, intersections[parentIntersects[0]].pos);
        float triangleA = CalculateTriangleArea(a, b, c);
        cout << "a: " << b <<
            "\nb: " << c << "\n";

        b = GetDistance(intersections[mainIntersects[0]].pos, intersections[parentIntersects[1]].pos);
        c = GetDistance(intersections[mainIntersects[1]].pos, intersections[parentIntersects[1]].pos);
        float triangleB = CalculateTriangleArea(a, b, c);
        cout << "c: " << b <<
            "\nd: " << c << "\n";

        cout << "The area of the quadrilateral is: " << (triangleA + triangleB) << ".\n";
        shapeArea = (triangleA + triangleB);
    }

    void FindTriangle(int intersects[]) {
        int cursor = 0;
        for (int i = 0; i < currectIntersection; i++) {
            if (shapeCode[intersections[i].lines[0]] > 1 && shapeCode[intersections[i].lines[1]] > 1) {
                intersects[cursor] = i;
                cursor++;
            }
        }
    }

    void CalculateTriangle() {
        int intersects[3];
        FindTriangle(intersects);

        float a = GetDistance(intersections[intersects[0]].pos, intersections[intersects[1]].pos);
        cout << "a: " << a << "\n";
        float b = GetDistance(intersections[intersects[1]].pos, intersections[intersects[2]].pos);
        cout << "b: " << b << "\n";
        float c = GetDistance(intersections[intersects[2]].pos, intersections[intersects[0]].pos);
        cout << "c: " << c << "\n";
        float s = ((a + b + c) / 2);
        float area = sqrt(s * ((s - a) * (s - b) * (s - c)));
        cout << "The area of the triangle is: " << area << "\n";
        shapeName = "Triangle";
        shapeArea = area;
    }

    bool IsQuadrilateral(int code[]) {
        int code1[] = { 2, 2, 2, 2 };
        return (CompareCodes(code, code1));
    }

    bool IsTriangle(int code[]) {
        int code1[] = { 2, 2, 2, 0 };
        int code2[] = { 3, 2, 2, 1 };
        return (CompareCodes(code, code1) || CompareCodes(code, code2));
    }

    bool CompareCodes(int c1[], int c2[]) {
        for (int i = 0; i < maxVectors; i++) {
            if (c1[i] != c2[i])
                return false;
        }
        return true;
    }

    void ResetShapeCode() {
        for (int i = 0; i < maxVectors; i++) {
            shapeCode[i] = 0;
        }
    }

    void Manage() {
        int arrangedCode[maxVectors];
        CopyArray(shapeCode, arrangedCode, maxVectors);
        ArrangeArray(arrangedCode, maxVectors);
        cout << "Shape code: ";
        PrintArray(arrangedCode, maxVectors);
        cout << "\n";
        
        if (IsTriangle(arrangedCode)) {
            cout << "The shape is a triangle!\n";
            CalculateTriangle();
        }
        if (IsQuadrilateral(arrangedCode)) {
            cout << "The shape is a quadrilateral!\n";
            CalculateQuadrilateral();
        }
    }
}

namespace Program {
    void ManageInput() {
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
            shapeArea = 0;
            currentVector = 0;
            par = true;
            currectIntersection = 0;
            Shapes::ResetShapeCode();
            InitVectors();
            cout << "Vectores reiniciados..." << endl;
        }
    }

    void Close() {
        CloseWindow();
    }

    void Draw() {
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
            DrawCircle(intersections[i].pos.x, intersections[i].pos.y, 5.0f, BLUE);
            DrawCenteredText(TextFormat("%i", i), intersections[i].pos, 10.0f, WHITE);
        }

        if (shapeArea > 0) {
            DrawText(shapeName, GetScreenWidth() * .02, GetScreenHeight() * .05, GetScreenHeight() * .08, ORANGE);
            DrawText(TextFormat("Area: %f", shapeArea), GetScreenWidth() * .02, GetScreenHeight() * .15, GetScreenHeight() * .04, RAYWHITE);
        }

        DrawText("[Left Click] anywhere in the screen to create vectors.", GetScreenWidth() * .02, GetScreenHeight() * .88, GetScreenHeight() * .04, RAYWHITE);
        DrawText("[Right Click] to erase the vectors.", GetScreenWidth() * .02, GetScreenHeight() * .94, GetScreenHeight() * .04, RAYWHITE);

        EndDrawing();
    }

    void Update() {
        while (!WindowShouldClose()) {
            ManageInput();
            Draw();
        }
    }

    void Init() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(screenWidth, screenHeight, "Algebra");
        SetTargetFPS(30);

        currentVector = 0;
        par = true;
        currectIntersection = 0;
        InitVectors();
    }
}

#pragma region Vectors
float GetAngle(Vector2 v1, Vector2 v2, Vector2 center) {
    /*
    Encontre esta formula aca.
    http://phrogz.net/angle-between-three-points
    */
    float a = powf(center.x - v1.x, 2) + powf(center.y - v1.y, 2);
    float b = powf(center.x - v2.x, 2) + powf(center.y - v2.y, 2);
    float c = powf(v2.x - v1.x, 2) + powf(v2.y - v1.y, 2);
    return acosf((a + b - c) / sqrtf(4 * a * b));
}

float GetDistance(Vector2 v1, Vector2 v2)
{
    /*
    Teorema de pitagoras
    https://en.wikipedia.org/wiki/Euclidean_distance
    */
    float distX = v1.x - v2.x;
    float distY = v1.y - v2.y;
    return sqrt((distX * distX) + (distY * distY));
}

void InitVectors() {
    for (int i = 0; i < maxVectors; i++) {
        vectors[i].point1 = { 0, 0 };
        vectors[i].point2 = { 0, 0 };
    }
}

#pragma endregion

#pragma region Tools
float DegreesToRadians(float deg) {
    return (deg * (m_pi / 180.0));
}

float RadiansToDegrees(float rad) {
    return (rad * (180.0 / m_pi));
}

void DrawCenteredText(const char* text, Vector2 pos, int fontSize, Color color) {
    int textWide = MeasureText(text, fontSize);
    DrawText(text, (pos.x) - (textWide * .5), (pos.y) - (fontSize * .5), fontSize, color);
}

bool IsInArray(int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return true;
        }
    }
    return false;
}

void ArrangeArray(int arr[], int size) {
    int aux;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j + 1 < size - i; j++) {
            if (arr[j] < arr[j + 1]) {
                aux = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = aux;
            }
        }
    }
}

void CopyArray(int oldArr[], int newArr[], int size) {
    for (int i = 0; i < size; i++) {
        newArr[i] = oldArr[i];
    }
}

void PrintArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
}

#pragma endregion