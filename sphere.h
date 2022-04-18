#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.h"
#include "operate.h"

class _sphere : public _object3D
{
public:
    _sphere(float Size=1.0,unsigned int N=32,unsigned int N2=32,bool upper=true,bool lower=true);
	void draw_texture();
	void draw_texture_flat(unsigned int var=0);
	void draw_texture_smooth(unsigned int var=0);
	void enable_textured_light(unsigned int var=0);
	void disable_textured_light();
};

#endif // SPHERE_H
