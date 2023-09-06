#include <iostream>
#include "Camara.h"
int main() {
    srand(time(NULL));
    Camara cam;
    cam.configurar(3, 60, 800, 600,
                       vec3(0,5,200),
                       vec3(75,75,0),
                       vec3(0,1,0));
    cam.renderizar();

    return 0;
}
