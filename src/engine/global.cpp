#include "global.hpp"


void setVec3(const vec3 origin, vec3 destination){
    for(int i=0;i<3;i++)
        destination[i] = origin[i];
}

void setVec4(const vec4 origin, vec4 destination){
    for(int i=0;i<4;i++)
        destination[i] = origin[i];
}