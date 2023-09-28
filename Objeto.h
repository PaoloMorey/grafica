//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_OBJETO_H
#define CG2023_OBJETO_H
#include "vec3.h"
#include "Rayo.h"
#include <iostream>

class Objeto {
public:
    vec3 color;
    float kd;
    float ke;
    float km;
    bool transparente;
    float ior;
    bool es_luz;
    Objeto(vec3 col, float kd, float ke, float km, bool transp, float ior, bool es_luz): color{col}, kd{kd}, ke{ke}, km{km}, transparente{transp}, ior{ior}, es_luz(es_luz) {}

    virtual bool intersectar(Rayo ray, float &t, vec3 &n)=0;
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

    static Esfera* esfera_random(int max_x = 200, int max_y = 200, int max_z = 200, int min_x = 0, int min_y = 0, int min_z = 0){
        vec3 cen(rand()%(max_x-min_x + 1) + min_x, rand()%(max_y-min_y + 1) + min_y, rand()%(max_z-min_z + 1) + min_z);
        float random_r = RandomFloat(12, 20);
        vec3 random_color(RandomFloat(0.1,1), RandomFloat(0.1,1), RandomFloat(0.1,1));
        float random_kd = RandomFloat(0.8,0.9);
        float random_ke = RandomFloat(0.8,0.9);
        auto x = new Esfera(cen, random_r, random_color, random_kd, random_ke, 0, false, 0, false);
        return x;
    }

    Esfera(vec3 cen, float r, vec3 col, float kd, float ke, float km, bool transp, float ior, bool es_luz):
    centro{cen}, radio{r}, Objeto(col, kd, ke, km, transp, ior, es_luz) {}
    bool intersectar(Rayo ray, float &t, vec3 &n) {
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
        vec3 pi = o+d*t;
        n = (pi-centro)*(1/(pi-centro).modulo());
        if (t <= 0) { return false; }
        return true;

    }
};

class Triangulo : public Objeto {
public:
    vec3 p1;
    vec3 p2;
    vec3 p3;

    Triangulo(vec3 p1, vec3 p2, vec3 p3, vec3 col, float kd, float ke, float km, bool transp, float ior, bool es_luz):
    p1{p1}, p2{p2}, p3{p3}, Objeto(col, kd, ke, km, transp, ior, es_luz) {}

    bool intersectar(Rayo ray, float &t, vec3 &n){
        vec3 d = ray.dir;
        vec3 o = ray.ori;
        n = (p2-p1).cruz(p3-p2)/((p2-p1).cruz(p3-p2)).modulo();
        t = ((p1-o).punto(n))/((d.punto(n)));
        vec3 pi = o + d*t;
        if(t<=0) {
            return false;}
        else{
            // n se puede calcular antes
            if(n.punto((p2-p1).cruz(pi - p1)) > 0 && n.punto((p3-p2).cruz(pi - p2)) > 0 && n.punto((p1-p3).cruz(pi - p3)) > 0) return true;
            return false;
        }
    }

};

class Cilindro: public Objeto {
public:
    float radio;
    vec3 a;
    vec3 b;

    Cilindro(vec3 a_, vec3 b_, float rad, vec3 col, float kd, float ke, float km, bool transp, float ior, bool es_luz):
    a{a_}, b{b_}, radio{rad}, Objeto(col, kd, ke, km, transp, ior, es_luz) {}

    float sign(float y) {
        if(y < 0) return -1;
        else return 1;
    }

    bool intersectar(Rayo ray, float &t, vec3 &n){
        vec3 ro = ray.ori;
        vec3 rd = ray.dir;
        vec3 ba = b - a;
        vec3 oc = ro - a;

        float baba = ba.punto(ba);
        float bard = ba.punto(rd);
        float baoc = ba.punto(oc);

        float k2 = baba - bard * bard;
        float k1 = baba * oc.punto(rd) - baoc * bard;
        float k0 = baba * oc.punto(oc) - baoc * baoc - radio * radio * baba;

        float h = k1 * k1 - k2 * k0;
        if (h < 0.0) return false;
        //body
        h = sqrt(h);
        t = (-k1 - h) / k2;
        if (t <= 0) return false;
        float y = baoc + t * bard;
        if (y > 0.0 && y < baba) {
            n = (oc + t * rd - ba * y / baba) / radio;
            n.normalize();
            return true;
        }
        // caps
        t = (((y < 0.0) ? 0.0 : baba) - baoc) / bard;
        if (abs(k1 + k2 * t) < h) {
            n = ba * sign(y) / baba;
            n.normalize();
            return true;
        }
        return false;
    }
};
#endif //CG2023_OBJETO_H
