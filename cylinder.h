#ifndef CYLINDER_H
#define CYLINDER_H

#include "object3d.h"
#include "operate.h"

class _cylinder : public _object3D
{
public:
    _cylinder(float Size=1.0,unsigned int N=128,bool upper=true,bool lower=true);
	void draw_texture();
	void draw_texture_flat(unsigned int var=0);
	void draw_texture_smooth(unsigned int var=0);
	void enable_textured_light(unsigned int var=0);
	void disable_textured_light();
};

#endif // CYLINDER_H
