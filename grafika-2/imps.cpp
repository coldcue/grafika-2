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
    
    Light() {
        
    }
    
    Light(Point const p0, Color const color) : p0(p0), color(color) {
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
    bool refractive;
    bool reflective;
    
    Surface(Color k, Color n, bool refractive, bool reflective)
    : k(k), n(n), refractive(refractive), reflective(reflective) {
        f0 = ((n - 1.0f) * (n - 1.0f) + k * k) / ((n + 1.0f) * (n + 1.0f) + k * k);
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
    
public:
    Surface surface;
    
    Object(Surface surface, float bvR, Point bvP0)
    : surface(surface), bvR(bvR), bvP0(bvP0) {
    };
    
    Vector reflectDir(Ray &ray, Vector &n) {
        return ray.v + n * (n.negate() * ray.v) * 2;
    }
    
    Vector refractDir(Ray &ray, Vector &n) {
        return Vector(); //TODO calc refract dir
    }
    
    virtual bool intersect(Ray &ray, float &t, Vector &n) = 0;
    
    virtual ~Object() {
    };
};

template<class T>
class DynamicArray {
    int capacity;
    T *array;
    
public:
    int size;
    
    DynamicArray(int capacity) : capacity(capacity) {
        array = new T[capacity];
    }
    
    void push(T o) {
        if (size < capacity)
            array[size++] = o;
    }
    
    T &operator[](int index) {
        return array[index];
    }
    
    ~DynamicArray() {
        delete array;
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
                if (t_temp > 0 && t_temp < t) {
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
    
    
    Color directLight(Point &p, Object *object, Vector &n) {
        Color color = object->surface.k * ambientLight;
        
        for (int i; i < lights.size; i++) {
            Vector lightDirection = lights[i].p0 - p;
            float lightDistance = lights[i].p0.distance(p);
            
            Ray shadowRay(p, lightDirection);
            
            float t;
            bool intersect = firstIntersect(shadowRay, t);
            if (!intersect || p.distance(shadowRay.getPoint(t)) > lightDistance) {
                Color diffuseLight = (lightDirection * n > 0.0f) ? object->surface.k : Color();
                Color blinnShine = Color();                //TODO add Blinn shine
                color = color + (diffuseLight + blinnShine) * lightIntensity(lightDistance);
            }
        }
        
        return color;
    }
    
    float lightIntensity(float dist) {
        return 1 / (dist * dist);
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
    
    Color trace(Ray &ray, int d = 0) {
        if (d > maxTrace)
            return ambientLight;
        
        Color color;
        float t;
        Vector n;
        Object *object;
        
        if (!firstIntersect(ray, t, object, n)) {
            if (d == 0)
                return background;
            else
                return ambientLight;
        }
        
        Point point = ray.getPoint(t);
        
        color = directLight(point, object, n);
        
        if (object->surface.reflective) {
            Ray reflectRay = Ray(point, object->reflectDir(ray, n));
            color = color - object->surface.k * trace(reflectRay, d + 1);
        }
        
        if (object->surface.refractive) {
            Ray refractRay = Ray(point, object->refractDir(ray, n));
            color = color - object->surface.k * trace(refractRay, d + 1);
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
    float r;
    Point p0;
    
    CylinderObject(Surface surface, float r, Point p0) : Object(surface, r, p0), r(r), p0(p0) {
        
    }
    
    bool intersect(Ray &ray, float &t, Vector &n) {
        if (!intersectBV(ray)) return false;
        
        
        return false;
    }
};

//--------------------------------------------------------
// GroundObject
//--------------------------------------------------------
class GroundObject : public Object {
    bool intersect(Ray &ray, float &t, Vector &n) {
        float temp = ray.v.x + ray.v.y;
        
        if (temp == 0.0f) return false;
        t = -(ray.p0.x + ray.p0.y) / temp;
        n = Vector(0.0f, 0.0f, 1.0f);
        
        return true;
    }
    
public:
    GroundObject(Surface surface) : Object(surface, 0.0f, Point()) {
        
    }
};