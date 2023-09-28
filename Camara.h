//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_CAMARA_H
#define CG2023_CAMARA_H

#include "Rayo.h"
#include "Luz.h"
#include "vec3.h"
#include "CImg.h"
#include "Objeto.h"
#include <utility>
#include <vector>
#include <cmath>
using namespace std;
using namespace cimg_library;
typedef unsigned char BYTE;

class Camara {
    vec3 eye, xe, ye, ze;
    float f, a, b, w, h;
    std::vector<Objeto *> objetos;
    std::vector<Luz> luces;
    CImg<BYTE> *pImg;
    int max_depth = 4;

public:
    Camara() {}
    void configurar(float _near, float fov, int ancho, int alto,
                    vec3 pos_eye, vec3 center, vec3 up);
    void renderizar(int i);
    vec3 calcular_color(Rayo rayo, std::vector<Luz> luces, vec3 color, float k_ambiente, int depth);

    void fresnel(vec3 &I, vec3 &N, float &ior, float &kr);

    vec3 refract(vec3 &I, vec3 &N, float &ior);

    void mover_luciernagas(Esfera* luciernaga, int i, float rotation_speed, vec3 center);

    void set_objetos(std::vector<Objeto*> o){
        this->objetos = std::move(o);
    }

    void set_luces(std::vector<Luz> l){
        this->luces = std::move(l);
    }
};


#endif //CG2023_CAMARA_H
