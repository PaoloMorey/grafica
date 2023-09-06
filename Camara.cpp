//
// Created by hgallegos on 30/03/2023.
//

#include "Camara.h"
#include <iostream>
#include <ctime>

using namespace std;

void Camara::configurar(float _near, float fov, int ancho, int alto,
                        vec3 pos_eye, vec3 center, vec3 up) {
    f = _near;
    w = ancho;
    h = alto;
    a = 2 * f * tan(fov * M_PI / 360);
    b = w / h * a;
    eye = pos_eye;
    ze = eye - center;
    ze.normalize();
    xe = up.cruz(ze);
    xe.normalize();
    ye = ze.cruz(xe);
}

void Camara::renderizar() {
    Rayo rayo;
    rayo.ori = eye;
    vec3 dir;

    pImg = new CImg<BYTE>(w, h, 1, 3);
    CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camara Pinhole");

    // Esfera esf(vec3(2, 0, 0), 8, vec3(0, 0, 1), 0.9, 0.6);
    std::vector<Objeto*> esferas;
    for(int i = 0; i < 50; ++i){
        Esfera* esf = Esfera::esfera_random();
        esferas.push_back(esf);
    }
    Luz luz(vec3(10, 10, 10), vec3(1, 1, 1));
    float k_ambiente = 0.1;
    vec3 color;
    float t;
    vec3 n, aux;
    int n_exp = 8;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;
            color.set(0, 0, 0);
            float min_d = FLT_MAX;
            int num_esf = 0;
            for(int i = 0; i < esferas.size(); ++i){
                if(esferas[i]->intersectar(rayo, t, n, aux)){
                    if(t < min_d){
                        t = min_d;
                        num_esf = i;
                    }
                }
            }
            if (esferas[num_esf]->intersectar(rayo, t, n, aux)) {
                auto vec_aux = luz.pos - aux;
                vec_aux.normalize();
                vec3 I_D;
                auto r = 2 * (vec_aux.punto(n)) * n - vec_aux;
                vec3 I_E;
                if (r.punto(-dir) > 0) I_E = luz.color * pow((r.punto(-dir) * esferas[num_esf]->kd), n_exp);
                if (n.punto(vec_aux) > 0) I_D = luz.color *     esferas[num_esf]->kd * (n.punto(vec_aux));
                color = esferas[num_esf]->color * (I_D + luz.color * k_ambiente + I_E);
                color.max_to_one();
            }
            (*pImg)(x, h - 1 - y, 0) = (BYTE) (color.x * 255);
            (*pImg)(x, h - 1 - y, 1) = (BYTE) (color.y * 255);
            (*pImg)(x, h - 1 - y, 2) = (BYTE) (color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    string nombre_archivo = "imagen" + to_string(1) + ".bmp";
    pImg->save(nombre_archivo.c_str());
    while (!dis_img.is_closed()) {
        dis_img.wait();
    }

}

