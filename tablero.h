#ifndef TABLERO_H
#define TABLERO_H

#include "object3d.h"
#include "operate.h"

class _tablero : public _object3D
{
private:
    float width;
    float height;
    float depth;

	void enable_textured_light(unsigned int var);
	void disable_textured_light();

public:
    _tablero(unsigned int steps=32,float width=2.0, float height=2.0,float depth=2.0);

    void draw_texture();
    void draw_texture_flat(unsigned int var=0);
    void draw_texture_smooth(unsigned int var=0);

    float par_x(float t, bool reverse = false);
    float par_y(float t, bool reverse = false);
    float par_z(float t, bool reverse = false);
};

#endif // TABLERO_H
