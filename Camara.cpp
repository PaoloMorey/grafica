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

void Camara::fresnel(vec3 &I, vec3 &N, float &ior, float &kr) {
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
}

vec3 Camara::refract(vec3 &I, vec3 &N, float &ior){
    float cosi = clamp(-1, 1, I.punto(N));
    float etai = 1, etat = ior;
    vec3 n = N;
    if (cosi < 0) { cosi = -cosi; }
    else {
        std::swap(etai, etat);
        n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3(0, 0, 0) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

vec3 Camara::calcular_color(Rayo rayo, std::vector<Luz> luces, vec3 color, float k_ambiente, int depth) {
    float min_d = FLT_MAX, t;
    Objeto* obj;
    vec3 n, min_n;
    int n_exp = 8;
    bool obj_int = false;
    for (auto & objeto : objetos) {
        if (objeto->intersectar(rayo, t, n)) {
            obj_int = true;
            if (t < min_d) {
                min_d = t;
                min_n = n;
                obj = objeto;
            }
        }
    }
    if (obj_int) {
        if(obj->es_luz){
            color = obj->color;
        }
        else {
            // aqui
            auto pi = rayo.ori + min_d * rayo.dir;
            vec3 I_D;
            vec3 I_E;
            for(auto &luz: luces){
                auto L = luz.pos - pi;
                auto L_mod = L.modulo();
                L.normalize();
                auto r = 2 * (L.punto(min_n)) * min_n - L;
                bool sombra = false;
                vec3 rs_pi = pi + 0.0005 * min_n;
                Rayo rs(rs_pi, L);
                for (auto &objeto: objetos) {
                    if (objeto->intersectar(rs, min_d, n) && !objeto->es_luz && !objeto->transparente) {
                        if (min_d <= L_mod) {
                            sombra = true;
                            break;
                        }
                    }
                }
                if (!sombra && !obj->transparente){
                    if (r.punto(-rayo.dir) > 0) I_E = I_E + luz.color * obj->ke * pow(r.punto(-rayo.dir), n_exp);
                    if (min_n.punto(L) > 0) I_D = I_D + luz.color * obj->kd * (min_n.punto(L));
                }
            }
            color = obj->color * (I_D/luces.size() + vec3(1,1,1) * k_ambiente + I_E/luces.size());
            if (obj->transparente && depth < max_depth) {
                vec3 color_refrac, color_reflec;
                float kr;
                fresnel(rayo.dir, min_n, obj->ior, kr);
                bool outside = rayo.dir.punto(min_n) < 0;
                vec3 bias = 0.0005 * min_n;
                 if (kr < 1) {
                     vec3 refrac_dir = refract(rayo.dir, min_n, obj->ior);
                     refrac_dir.normalize();
                     vec3 refrac_ori = outside ? pi - bias : pi + bias;
                     Rayo refrac_ray(refrac_ori, refrac_dir);
                     color_refrac = calcular_color(refrac_ray, luces, color, k_ambiente, depth + 1);
                 }
                vec3 reflec_dir = 2 * ((-rayo.dir).punto(min_n)) * min_n - (-rayo.dir);
                reflec_dir.normalize();
                vec3 reflec_ori = outside ? pi + bias : pi - bias;
                Rayo reflec_ray(reflec_ori, reflec_dir);
                color_reflec = calcular_color(reflec_ray, luces, color, k_ambiente, depth + 1);
                color = color + color_reflec * kr + color_refrac * (1 - kr);
            }
            if (obj->km > 0) {
                vec3 reflect = 2 * ((-rayo.dir).punto(min_n) * min_n) - (-rayo.dir);
                reflect.normalize();
                if (depth < max_depth) {
                    vec3 color_ref = calcular_color(Rayo(pi + 0.0005 * min_n, reflect), luces, color, k_ambiente,
                                                    depth + 1);
                    color = color + obj->km * color_ref;
                }
            }
        }
        color.max_to_one();
    }
    return color;
}

void Camara::mover_luciernagas(Esfera *luciernaga, int i, float rotation_speed, vec3 center) {
    float angleY = rotation_speed;
    float angleYRad = angleY * M_PI/180.0f;
    float relativeX = luciernaga->centro.x - center.x;
    float relativeZ = luciernaga->centro.z - center.z;
    luciernaga->centro.x = center.x + relativeX * cos(angleYRad) - relativeZ * sin(angleYRad);
    luciernaga->centro.z = center.z + relativeX * sin(angleYRad) + relativeZ * cos(angleYRad);
    this->luces[i].pos.x = luciernaga->centro.x;
    this->luces[i].pos.z = luciernaga->centro.z;
}

void Camara::renderizar(int i) {
    Rayo rayo;
    rayo.ori = eye;
    vec3 dir;

    pImg = new CImg<BYTE>(w, h, 1, 3);
    CImgDisplay dis_img((*pImg), "Imagen RayTracing en Perspectiva desde una Camara Pinhole");
    float k_ambiente = 0.2;
    vec3 color;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
            dir.normalize();
            rayo.dir = dir;
            color.set(0, 0, 0);
            color = calcular_color(rayo, luces, color, k_ambiente, 1);
            (*pImg)(x, h - 1 - y, 0) = (BYTE) (color.x * 255);
            (*pImg)(x, h - 1 - y, 1) = (BYTE) (color.y * 255);
            (*pImg)(x, h - 1 - y, 2) = (BYTE) (color.z * 255);
        }
    }
    mover_luciernagas(dynamic_cast<Esfera *>(objetos[9]), 1, 10, vec3(150, 50, 210));
    mover_luciernagas(dynamic_cast<Esfera *>(objetos[10]), 2, 12, vec3(150, 50, 210));
    mover_luciernagas(dynamic_cast<Esfera *>(objetos[11]), 3, 16, vec3(150, 50, 210));
    mover_luciernagas(dynamic_cast<Esfera *>(objetos[12]), 4, 20, vec3(150, 50, 210));
    /*dis_img.render((*pImg));
    dis_img.paint();*/
    string nombre_archivo = "imagen" + to_string(i) + ".bmp";
    pImg->save(nombre_archivo.c_str());
    /*while (!dis_img.is_closed()) {
        dis_img.wait();
    }*/
}


