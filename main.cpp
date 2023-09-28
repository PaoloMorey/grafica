#include <iostream>
#include "Camara.h"
#include <chrono>

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    srand(time(NULL));
    Camara cam;
    int frames = 300;
    float rotation_speed = 1.0f;
    vec3 pos_eye(150,170,550);
    vec3 center(150,70,200);
    std::vector<Objeto*> objetos;
    auto *cil = new Cilindro(vec3(150, 10, 210), vec3(150,100,210), 50, vec3(1,1,1), 0.7, 0.3, 0, true, 2, false);
    objetos.push_back(cil);
    auto *cil2 = new Cilindro(vec3(150, 100, 210), vec3(150,120,210), 20, vec3(1,1,1), 0.7, 0.3, 0, false, 0, false);
    auto *cil3 = new Cilindro(vec3(260, 40, 150), vec3(290,60,150), 10, vec3(1,1,0), 0.9, 0.9, 0, false, 1, false);
    objetos.push_back(cil3);
    auto *tri = new Triangulo(vec3(-200, 10, 300),vec3(800, 10, 300),vec3(100,10,0),vec3(0.394, 0.255, 0.522), 0.9, 0.1, 1, false, 2, false);
    objetos.push_back(tri);
    std::vector<Luz> luces;
    auto *esf = new Esfera(vec3(280, 80, 150), 12, vec3(1,0,0), 0.9, 0.9, 0, false, 0, false);
    auto *esf2 = new Esfera(vec3(0, 80, 150), 12, vec3(0,1,0), 0.9, 0.9, 0, false, 0, false);
    auto *esf3 = new Esfera(vec3(40, 10, 150), 22, vec3(0,0,1), 0.9, 0.9, 0, false, 0, false);
    objetos.push_back(esf);
    objetos.push_back(esf2);
    objetos.push_back(esf3);
    auto *esf4 = new Esfera(vec3(230, 10, 220), 17, vec3(0,1,1), 0.9, 0.9, 0, false, 0, false);
    objetos.push_back(esf4);
    auto *esf5 = new Esfera(vec3(80, 60, 50), 22, vec3(1,0,1), 0.9, 0.9, 0, false, 0, false);
    objetos.push_back(esf5);
    // Luciernagas
    auto* luci1 = new Esfera(vec3(150, 50, 210), 2, vec3(1,1,0), 0, 0, 0, false, 0, true);
    Luz luci_luz_1(vec3(150, 50, 210), vec3(1,1,1));
    auto* luci2 = new Esfera(vec3(160, 50, 210), 2, vec3(1,1,0), 0, 0, 0, false, 0, true);
    Luz luci_luz_2(vec3(160, 50, 210), vec3(1,1,1));
    auto* luci3 = new Esfera(vec3(140, 50, 210), 2, vec3(1,1,0), 0, 0, 0, false, 0, true);
    Luz luci_luz_3(vec3(140, 50, 210), vec3(1,1,1));
    auto* luci4 = new Esfera(vec3(170, 50, 210), 2, vec3(1,1,0), 0, 0, 0, false, 0, true);
    Luz luci_luz_4(vec3(170, 50, 210), vec3(1,1,1));
    auto* luci5 = new Esfera(vec3(130, 50, 210), 2, vec3(1,1,0), 0, 0, 0, false, 0, true);
    Luz luci_luz_5(vec3(130, 50, 210), vec3(1,1,1));
    objetos.push_back(luci1);
    luces.push_back(luci_luz_1);
    objetos.push_back(luci2);
    luces.push_back(luci_luz_2);
    objetos.push_back(luci3);
    luces.push_back(luci_luz_3);
    objetos.push_back(luci4);
    luces.push_back(luci_luz_4);
    objetos.push_back(luci5);
    luces.push_back(luci_luz_5);
    objetos.push_back(cil2);
    cam.set_objetos(objetos);
    cam.set_luces(luces);
    for(int i = 0; i < frames; ++i){
        float angleY = rotation_speed;
        float angleYRad = angleY * M_PI/180.0f;
        float relativeX = pos_eye.x - center.x;
        float relativeZ = pos_eye.z - center.z;
        pos_eye.x = center.x + relativeX * cos(angleYRad) - relativeZ * sin(angleYRad);
        pos_eye.z = center.z + relativeX * sin(angleYRad) + relativeZ * cos(angleYRad);
        cam.configurar(3, 60, 800, 600,pos_eye,
                       center,
                       vec3(0,1,0));
        cam.renderizar(i);
        std::cout<<i<<endl;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    // Print the total time in seconds
    std::cout << "Total time taken: " << duration.count() << " seconds" << std::endl;
    return 0;
}
