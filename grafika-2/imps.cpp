#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const float FLOAT_MAX = powf(10, 37);

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
    float x, y, z;

    Vector() {
        x = y = z = 0;
    }

    Vector(float x, float y, float z) : x(x), y(y), z(z) {
    }

    Vector operator*(float a) {
        return Vector(x * a, y * a, z * a);
    }

    Vector operator/(float a) {
        return Vector(x / a, y / a, z / a);
    }

    Vector operator+(const Vector &v) {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector &v) {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    float operator*(const Vector &v) {    // dot product
        return (x * v.x + y * v.y + z * v.z);
    }

    Vector operator%(const Vector &v) {    // cross product
        return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    float length() {
        return sqrtf(x * x + y * y + z * z);
    }

    Vector negate() {
        return Vector(-1.0f * x, -1.0f * y, -1.0f * z);
    }

    Vector normalize() {
        return (*this) / this->length();
    }
};

//--------------------------------------------------------
// Quadratic Matrix
//--------------------------------------------------------
struct QVector {
    float x, y, z, w;

    QVector() {
        x = y = z = w = 0.0f;
    }

    QVector(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
    }

};

struct QMatrix {
    float m[4][4] = {0.0f};

    QMatrix operator*(const QMatrix &right) {
        QMatrix result;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                float sum = 0;
                for (int i = 0; i < 4; i++) {
                    sum += m[i][y] * right.m[x][i];
                }
                result.m[x][y] = sum;
            }
        }
        return result;
    }

    QVector operator*(const QVector &right) {
        float x = m[0][0] * right.x + m[1][0] * right.y + m[2][0] * right.z + m[3][0] * right.w;
        float y = m[0][1] * right.x + m[1][1] * right.y + m[2][1] * right.z + m[3][1] * right.w;
        float z = m[0][2] * right.x + m[1][2] * right.y + m[2][2] * right.z + m[3][2] * right.w;
        float w = m[0][3] * right.x + m[1][3] * right.y + m[2][3] * right.z + m[3][3] * right.w;
        return QVector(x, y, z, w);
    }

    QMatrix operator*(float f) {
        QMatrix result;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                result.m[x][y] = m[x][y] * f;
            }
        }
        return result;
    }

    QMatrix transpose() {
        QMatrix result;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                result.m[x][y] = m[y][x];
            }
        }
        return result;
    }

};

QVector operator*(const QVector &left, const QMatrix &right) {
    float x = right.m[0][0] * left.x + right.m[0][1] * left.y + right.m[0][2] * left.z + right.m[0][3] * left.w;
    float y = right.m[1][0] * left.x + right.m[1][1] * left.y + right.m[1][2] * left.z + right.m[1][3] * left.w;
    float z = right.m[2][0] * left.x + right.m[2][1] * left.y + right.m[2][2] * left.z + right.m[2][3] * left.w;
    float w = right.m[3][0] * left.x + right.m[3][1] * left.y + right.m[3][2] * left.z + right.m[3][3] * left.w;
    return QVector(x, y, z, w);
}


//--------------------------------------------------------
// 3D Point
//--------------------------------------------------------
struct Point {
    float x, y, z;

    Point() {
        x = y = z = 0;
    }

    Point(float x, float y, float z) : x(x), y(y), z(z) {
    }

    Point operator+(const Vector &p) {
        return Point(x + p.x, y + p.y, z + p.z);
    }

    Vector operator-(const Point &p) {
        return Vector(x - p.x, y - p.y, z - p.z);
    }

    Point operator*(const Vector &v) {
        return Point(x * v.x, y * v.y, z * v.z);
    }

    float distance(const Point &p) {
        return sqrtf((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z));
    }

    Vector vectorFromOrigo() {
        return Vector(x, y, z);
    }
};


//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
    float r, g, b;

    Color() {
        r = g = b = 0;
    }

    Color(float r0, float g0, float b0) {
        r = r0;
        g = g0;
        b = b0;
    }

    Color operator*(float a) {
        return Color(r * a, g * a, b * a);
    }

    Color operator+(float a) {
        return Color(r + a, g + a, b + a);
    }

    Color operator-(float a) {
        return Color(r - a, g - a, b - a);
    }

    Color operator*(const Color &c) {
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color operator/(const Color &c) {
        return Color(r / c.r, g / c.g, b / c.b);
    }

    Color operator+(const Color &c) {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator-(const Color &c) {
        return Color(r - c.r, g - c.g, b - c.b);
    }

    Color inverse() {
        return Color(1.0f - r, 1.0f - g, 1.0f - b);
    }
};

//--------------------------------------------------------
// Ray
//--------------------------------------------------------
struct Ray {
    Point p0;
    Vector v;

    Ray(Point p0, Vector v) : p0(p0), v(v) {
    };

    Point getPoint(float t) {
        return p0 + (v * t);
    }
};


//--------------------------------------------------------
// Light
//--------------------------------------------------------
struct Light {
    Point p0;
    Color color;
    float intensity;

    Light() {

    }

    Light(Point p0, Color color, float intensity) : p0(p0), color(color), intensity(intensity) {
    }

    float getIntensity(float dist) {
        if (dist < 1.0f) return intensity;
        return intensity * (1.0f / (dist * dist));
    }
};

//--------------------------------------------------------
// Surface
//--------------------------------------------------------
struct Surface {
    Color k;
    Color n;
    Color f0;

    float shininess;
    float navg;
    bool refractive;
    bool reflective;

    Surface(Color k, Color n, float shininess, bool refractive, bool reflective)
            : k(k), n(n), shininess(shininess), refractive(refractive), reflective(reflective) {
        f0 = ((n - 1.0f) * (n - 1.0f) + k * k) / ((n + 1.0f) * (n + 1.0f) + k * k);
        navg = (n.r + n.g + n.b) / 3.0f;
    }

    Color fresnel(Vector &v, Vector &n) {
        return f0 + f0.inverse() * powf(1.0f - cosf(fabsf(n * v)), 5);
    }
};


//--------------------------------------------------------
// Object
//--------------------------------------------------------
class Object {
    float bvR;
    Point bvP0;

protected:
    bool intersectBV(Ray &ray) {
        float d = ray.v.length();
        float t = -1.0f * ((bvP0 - ray.p0) * ray.v / (d * d));

        return ray.getPoint(t).distance(bvP0) <= bvR;
    }

    bool closestRoot(float a, float b, float c, float &t) {
        float disc = b * b - 4.0f * a * c;

        if (disc < 0.0f) return false;

        float t1 = (-1.0f * b + sqrtf(disc)) / (2.0f * a);
        float t2 = (-1.0f * b - sqrtf(disc)) / (2.0f * a);

        t = (t1 < t2) ? t1 : t2;
        return true;
    }

public:
    Surface surface;

    Object(Surface surface, float bvR, Point bvP0)
            : surface(surface), bvR(bvR), bvP0(bvP0) {
    };

    Vector reflectDir(Ray &ray, Vector &n) {
        return ray.v + n * (n.negate() * ray.v) * 2;
    }

    bool refractDir(Ray &ray, Vector &n, Vector &dir, bool &out) {
        float cosa = ray.v.normalize() * n.negate();

        float cn = (out) ? 1.0f / surface.navg : surface.navg;
        float disc = 1 - (1 - cosa * cosa) / (cn * cn);

        if (disc < 0.0f)
            return false;

        dir = (ray.v / cn + n * (cosa / cn - sqrtf(disc)) + ray.v / cn).normalize();
        return true;
    }

    virtual bool intersect(Ray &ray, float &t, Vector &n) = 0;

    virtual ~Object() {
    };
};

//--------------------------------------------------------
// DynamicArray
//--------------------------------------------------------
template<class T>
class DynamicArray {
    int capacity;
    T *array;

public:
    int size;

    DynamicArray(int capacity) : capacity(capacity), size(0), array(NULL) {
        array = new T[capacity];
    }

    void push(const T &o) {
        if (size < capacity)
            array[size++] = o;
    }

    T &operator[](int index) {
        return array[index];
    }

    ~DynamicArray() {
        delete[] array;
    }
};

//--------------------------------------------------------
// World
//--------------------------------------------------------
class World {
    Color ambientLight;
    int maxTrace;

    bool firstIntersect(Ray &r, float &t, Object *&o, Vector &n) {
        bool intersected = false;
        for (int i = 0; i < objects.size; i++) {
            Vector n_temp;
            float t_temp;
            if (objects[i]->intersect(r, t_temp, n_temp)) {
                if (t_temp > 0.01f && t_temp < t) {
                    t = t_temp;
                    n = n_temp;
                    o = objects[i];
                    intersected = true;
                }
            }
        }
        return intersected;
    }

    bool firstIntersect(Ray &r, float &t) {
        Object *o;
        Vector n;
        return firstIntersect(r, t, o, n);
    }


    Color directLight(Point &p, Ray &ray, Vector &n, Object *object) {
        Color color = object->surface.k * ambientLight;

        for (int i = 0; i < lights.size; i++) {
            Ray shadowRay(p, (lights[i].p0 - p).normalize());
            float lightDistance = lights[i].p0.distance(p);

            float t = FLOAT_MAX;
            bool intersect = firstIntersect(shadowRay, t);

            if (!intersect || p.distance(shadowRay.getPoint(t)) > lightDistance) {
                float costheta = shadowRay.v * n;
                Color diffuseLight = (costheta > 0.0f) ? object->surface.k * costheta : Color();
                float cosphi = (shadowRay.v.negate() + ray.v).normalize() * n;
                Color blinnShine = (cosphi > 0.0f) ? object->surface.n * powf(cosphi, object->surface.shininess) : Color();
                color = color + (diffuseLight + blinnShine) * lights[i].color * lights[i].getIntensity(lightDistance);
            }
        }

        return color;
    }

public:
    DynamicArray<Object *> objects;
    DynamicArray<Light> lights;
    Color background;

    World(int maxObjects, int maxLights, Color background, Color ambientLight, int maxTrace)
            : objects(maxObjects),
              lights(maxLights),
              background(background),
              ambientLight(ambientLight),
              maxTrace(maxTrace) {

    }

    Color trace(Ray &ray, Color power = Color(), int d = 0, bool out = false) {
        if (d > maxTrace)
            return background * 0.5f;

        Color color;
        float t = FLOAT_MAX;
        Vector n;
        Object *object;

        if (!firstIntersect(ray, t, object, n)) {
            return background * 0.5f;
        }

        Point point = ray.getPoint(t);

        color = directLight(point, ray, n, object);

        Color fresnel = object->surface.fresnel(ray.v, n);
        if (object->surface.reflective) {
            Ray reflectRay = Ray(point, object->reflectDir(ray, n));
            color = color + fresnel * trace(reflectRay, fresnel, d + 1);
        }

        if (object->surface.refractive) {
            Vector dir;
            if (object->refractDir(ray, n, dir, out)) {
                Ray refractRay = Ray(point, dir);
                Color fresnel2 = fresnel * -1.0f + 1.0f;
                color = color + fresnel2 * trace(refractRay, fresnel2, d + 1, !out);
            }
        }

        if (!object->surface.refractive && !object->surface.reflective && d > 0) {
            color = color + power * 0.1f;
        }

        return color;
    }

    ~World() {
        for (int i = 0; i < objects.size; i++) {
            delete objects[i];
        }
    }
};


//--------------------------------------------------------
// CylinderObject
//--------------------------------------------------------
class CylinderObject : public Object {
    Vector v;
    Point p0;

    CylinderObject(Surface surface, Vector v, Point p0) : Object(surface, 0.0, p0), v(v), p0(p0) {

    }

    bool intersect(Ray &ray, float &t, Vector &n) {
        //if (!intersectBV(ray)) return false;

        // a^2 * vy^2 + b^2 * vx^2
        float a = (v.x * v.x) * (ray.v.y * ray.v.y) + (v.y * v.y) * (ray.v.x * ray.v.x);
        // 2 * a^2 * y0 * vy + 2 * b^2 * x0 * vx
        float b = (2 * (v.x * v.x) * ray.p0.y * ray.v.y) + (2 * (v.y * v.y) * ray.p0.x * ray.v.x);
        // a^2 * y0^2 + b^2 * x0^2
        float c = (v.x * v.x) * (ray.p0.y * ray.p0.y) + (v.y * v.y) * (ray.p0.x * ray.p0.x) - (v.x * v.x) * (v.y * v.y);


        if (!closestRoot(a, b, c, t)) {
            return false;
        }


        return true;
    }
};

//--------------------------------------------------------
// SphereObject
//--------------------------------------------------------
class SphereObject : public Object {
    float r;
    Point p0;

public:
    SphereObject(Surface surface, float r, Point p0) : Object(surface, r, p0), r(r), p0(p0) {

    }

    bool intersect(Ray &ray, float &t, Vector &n) {
        //if (!intersectBV(ray)) return false;

        Vector temp(ray.p0 - p0);

        float a = ray.v * ray.v;
        float b = temp * ray.v * 2.0f;
        float c = temp * temp - (r * r);

        if (!closestRoot(a, b, c, t)) {
            return false;
        }

        n = ((ray.getPoint(t) - p0) / r).normalize();

        return true;
    }
};

//--------------------------------------------------------
// GroundObject
//--------------------------------------------------------
class GroundObject : public Object {
    bool intersect(Ray &ray, float &t, Vector &n) {
        if (ray.v.z == 0)
            return false;
        t = (-1.0f * ray.p0.z) / ray.v.z;
        n = Vector(0.0f, 0.0f, 1.0f);
        return true;
    }

public:
    GroundObject(Surface surface) : Object(surface, 0.0f, Point()) {

    }
};