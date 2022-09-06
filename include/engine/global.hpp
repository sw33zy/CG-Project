#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>



typedef float vec3[3];
typedef float vec4[4];


void setVec3(const vec3 origin, vec3 destination);
void setVec4(const vec4 origin, vec4 destination);

#endif