//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_OBJETO_H
#define CG2023_OBJETO_H
#include "vec3.h"
#include "Rayo.h"

class Objeto {
public:
    vec3 color;
    float kd;
    float ke;
    Objeto(vec3 col, float kd, float ke):color{col}, kd{kd}, ke{ke}{}

    virtual bool intersectar(Rayo ray, float &t, vec3 &n, vec3&aux)=0;
};

class Esfera : public Objeto {
public:
    vec3 centro;
    float radio;

    static float RandomFloat(float a, float b){
        float random = ((float) rand()) / (float) RAND_MAX;
        float diff = b - a;
        float r = random * diff;
        return a + r;
    }

    // Constructor vacio
    static Esfera* esfera_random(int max_x = 100, int max_y = 100, int max_z = 100, int min_x = 0, int min_y = 0, int min_z = 0){
        vec3 cen(rand()%(max_x-min_x + 1) + min_x, rand()%(max_y-min_y + 1) + min_y, rand()%(max_z-min_z + 1) + min_z);
        float random_r = RandomFloat(4, 12);
        vec3 random_color(rand()%(5-0 + 1) + 0, rand()%(5-0 + 1) + 0, rand()%(1-0 + 1) + 0);
        float random_kd = RandomFloat(0.1,0.9);
        float random_ke = RandomFloat(0.1,0.9);
        auto x = new Esfera(cen, random_r, random_color, random_kd, random_ke);
        return x;
    }

    Esfera(vec3 cen, float r, vec3 col, float kd, float ke):
    centro{cen}, radio{r}, Objeto(col, kd, ke) {}
    bool intersectar(Rayo ray, float &t, vec3 &n, vec3 &aux) {
        vec3 d = ray.dir;
        vec3 o = ray.ori;
        float a = d.punto(d);
        float b = 2 * d.punto(o - centro);
        float c = (o - centro).punto(o - centro) - radio*radio;
        float D = b*b - 4*a*c;
        if (D <= 0) { return false; }
        float t1 = (-b + sqrt(D)) / (2*a);
        float t2 = (-b - sqrt(D)) / (2*a);
        t = std::min(t1, t2);
        aux = o+d*t - centro;
        n = aux*(1/(aux).modulo());
        if (t <= 0) { return false; }
        return true;

    }
};

class Plano : public Objeto {
public:
    vec3 p1;
    vec3 p2;
    vec3 p3;


};
#endif //CG2023_OBJETO_H
