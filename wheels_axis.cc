#include "wheels_axis.h"
#include <GL/gl.h>

_wheels_axis::_wheels_axis(float Size,unsigned int N,float wheel_size,float width){
    this->Wheel =  _wheel(wheel_size,N);
    this->Wheel2 =  _wheel(wheel_size,N);
    this->Cylinder =  _cylinder(Size,N);
    this->wheel_size = wheel_size;
    this->width = width;
	this->ASize = Size;
	this->WSize = wheel_size;
	this->Tend = 3;
}


void _wheels_axis::setSelected(int i, int j){
	int c = -1;
	if(i==Tvalue)
		c = 0;
	else if(i==Tvalue+1)
		c = 1;
	else if(i==Tvalue+2)
		c = 2;
	switch(c){
		case 0:
			Cylinder.setSelected(j);
			break;
		case 1:
			Wheel.setSelected(j);
			break;
		case 2:
			Wheel2.setSelected(j);
			break;
		default:
			Cylinder.setSelected(-1);
			Wheel.setSelected(-1);
			Wheel2.setSelected(-1);
			break;
	}
}

_vertex3f _wheels_axis::getSelected(int i, int j){
	_vertex3f res,base;
	int c = -1;
	if(i==Tvalue)
		c = 0;
	else if(i==Tvalue+1)
		c = 1;
	else if(i==Tvalue+2)
		c = 2;
	switch(c){
		case 0:
			base = Cylinder.getSelected(j);
			res._0 = MV[0][0]*base._0 + MV[0][4]*base._1 + MV[0][8]*base._2 + MV[0][12];
			res._1 = MV[0][1]*base._0 + MV[0][5]*base._1 + MV[0][9]*base._2 + MV[0][13];
			res._2 = MV[0][2]*base._0 + MV[0][6]*base._1 + MV[0][10]*base._2 +MV[0][14];
			break;
		case 1:
			base = Wheel.getSelected(j);
			res._0 = MV[1][0]*base._0 + MV[1][4]*base._1 + MV[1][8]*base._2 + MV[1][12];
			res._1 = MV[1][1]*base._0 + MV[1][5]*base._1 + MV[1][9]*base._2 + MV[1][13];
			res._2 = MV[1][2]*base._0 + MV[1][6]*base._1 + MV[1][10]*base._2 +MV[1][14];
			break;
		case 2:
			base = Wheel2.getSelected(j);
			res._0 = MV[2][0]*base._0 + MV[2][4]*base._1 + MV[2][8]*base._2 + MV[2][12];
			res._1 = MV[2][1]*base._0 + MV[2][5]*base._1 + MV[2][9]*base._2 + MV[2][13];
			res._2 = MV[2][2]*base._0 + MV[2][6]*base._1 + MV[2][10]*base._2 +MV[2][14];
			break;
	}
	return res;
}

void _wheels_axis::clear(){
    alpha = 0;
    delta = 0;
    theta = 0;
}

void _wheels_axis::rotate(float step){
    alpha += step;
    delta += step;
    theta += step;
}

void _wheels_axis::draw_something(unsigned int type,unsigned int mat, int index){
	if(index>=0)
		this->Tvalue = index;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(alpha,1,0,0);
    glRotatef(90,0,0,1);
    glScalef(0.4,(width/ASize),0.4);
    switch(type){
        case 1:
            Cylinder.draw_point();
            break;
        case 2:
            Cylinder.draw_line();
            break;
        case 3:
            Cylinder.draw_fill();
            break;
        case 4:
            Cylinder.draw_chess();
            break;
        case 5:
            Cylinder.draw_light_flat(mat);
            break;
        case 6:
            Cylinder.draw_light_smooth(mat);
            break;
        case 7:
            Cylinder.draw_fill(&index);
            break;
    }
	glLoadIdentity();
    glRotatef(alpha,1,0,0);
    glRotatef(90,0,0,1);
    glScalef(0.4,(width/ASize),0.4);
	glGetFloatv(GL_MODELVIEW_MATRIX,MV[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2.0),0,0);
    glRotatef(delta,1,0,0);
    switch(type){
        case 1:
            Wheel.draw_point();
            break;
        case 2:
            Wheel.draw_line();
            break;
        case 3:
            Wheel.draw_fill();
            break;
        case 4:
            Wheel.draw_chess();
            break;
        case 5:
            Wheel.draw_light_flat(mat);
            break;
        case 6:
            Wheel.draw_light_smooth(mat);
            break;
        case 7:
            Wheel.draw_fill(&index);
            break;

    }
	glLoadIdentity();
    glTranslatef(-(width/2.0),0,0);
    glRotatef(delta,1,0,0);
	glGetFloatv(GL_MODELVIEW_MATRIX,MV[1]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(width/2.0,0,0);
    glRotatef(theta,1,0,0);
    switch(type){
        case 1:
            Wheel2.draw_point();
            break;
        case 2:
            Wheel2.draw_line();
            break;
        case 3:
            Wheel2.draw_fill();
            break;
        case 4:
            Wheel2.draw_chess();
            break;
        case 5:
            Wheel2.draw_light_flat(mat);
            break;
        case 6:
            Wheel2.draw_light_smooth(mat);
            break;
        case 7:
            Wheel2.draw_fill(&index);
            break;

    }
	glLoadIdentity();
    glTranslatef(width/2.0,0,0);
    glRotatef(theta,1,0,0);
	glGetFloatv(GL_MODELVIEW_MATRIX,MV[2]);
    glPopMatrix();

}

void _wheels_axis::draw_point(){
    this->draw_something(1);
}

void _wheels_axis::draw_line(){
    this->draw_something(2);
}

void _wheels_axis::draw_fill(){
    this->draw_something(3);
}
void _wheels_axis::draw_fill(int* index){
	this->draw_something(7,-1,*index);
	*index += this->Tend;
}

void _wheels_axis::draw_chess(){
    this->draw_something(4);
}

void _wheels_axis::draw_light_flat(unsigned int mat){
    this->draw_something(5,mat);
}
void _wheels_axis::draw_light_smooth(unsigned int mat){
    this->draw_something(6,mat);
}
void _wheels_axis::change_light1(unsigned int val){
    this->Wheel.change_light1(val);
    this->Cylinder.change_light1(val);
}

void _wheels_axis::change_light2(unsigned int val){
    this->Wheel.change_light2(val);
    this->Cylinder.change_light2(val);
}

void _wheels_axis::move_light(int i, int j){
    this->Wheel.move_light(i,j);
    this->Cylinder.move_light(i,j);
}
