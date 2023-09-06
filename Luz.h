//
// Created by jrpao on 8/23/2023.
//

#ifndef INC_2023_2_RAYTRACING_LUZ_H
#define INC_2023_2_RAYTRACING_LUZ_H

#include "vec3.h"

class Luz {
public:
    vec3 pos;
    vec3 color;
    Luz(vec3 pos, vec3 color): pos{pos}, color{color} {}

};


#endif //INC_2023_2_RAYTRACING_LUZ_H
