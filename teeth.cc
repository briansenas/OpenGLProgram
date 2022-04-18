#include "teeth.h"

_teeth::_teeth(float CSize,float TSize){
	DCHA =  _tooth(TSize);
	IZDA =  _tooth(TSize);
	this->CSize = CSize;
	this->TSize = TSize;
	this->Tend = DCHA.Tend + IZDA.Tend;
}

void _teeth::setSelected(int i, int j){
	int c = -1;
	if(i>=Tvalue and i<=DCHA.getT())
		c = 0;
	else if(i>DCHA.getT())
		c = 1;
	switch(c){
		case 0:
			DCHA.setSelected(i,j);
			break;
		case 1:
			IZDA.setSelected(i,j);
			break;
		default:
			DCHA.setSelected(-1,-1);
			IZDA.setSelected(-1,-1);
			break;
	}
}

_vertex3f _teeth::getSelected(int i, int j){
	_vertex3f res,base;
	int c = -1;
	if(i>=Tvalue and i<=DCHA.getT())
		c = 0;
	else if(i>DCHA.getT())
		c = 1;
	switch(c){
		case 0:
			base = DCHA.getSelected(i,j);
			res._0 = MV[0][0]*base._0 + MV[0][4]*base._1 + MV[0][8]*base._2 + MV[0][12];
			res._1 = MV[0][1]*base._0 + MV[0][5]*base._1 + MV[0][9]*base._2 + MV[0][13];
			res._2 = MV[0][2]*base._0 + MV[0][6]*base._1 + MV[0][10]*base._2 +MV[0][14];
			break;
		case 1:
			base = IZDA.getSelected(i,j);
			res._0 = MV[1][0]*base._0 + MV[1][4]*base._1 + MV[1][8]*base._2 + MV[1][12];
			res._1 = MV[1][1]*base._0 + MV[1][5]*base._1 + MV[1][9]*base._2 + MV[1][13];
			res._2 = MV[1][2]*base._0 + MV[1][6]*base._1 + MV[1][10]*base._2 +MV[1][14];
			break;
		default:break;
	}
	return res;
}

void _teeth::draw_something(unsigned int type, unsigned int mat, int index){
	if(index>=0)
		this->Tvalue = index;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glPushMatrix();
	glTranslatef(-CSize,0,0);
        switch(type){
            case 1:
                DCHA.draw_point();
                break;
            case 2:
                DCHA.draw_line();
                break;
            case 3:
                DCHA.draw_fill();
                break;
            case 4:
                DCHA.draw_chess();
                break;
            case 5:
                DCHA.draw_light_flat(mat);
                break;
            case 6:
                DCHA.draw_light_smooth(mat);
                break;
            case 7:
                DCHA.draw_fill(&index);
                break;

        }
		glLoadIdentity();
		glTranslatef(-CSize,0,0);
		glGetFloatv(GL_MODELVIEW_MATRIX,MV[0]);
    glPopMatrix();

    glPushMatrix();
	glTranslatef(CSize,0,0);
    glScalef(-1,1,1);
        switch(type){
            case 1:
                IZDA.draw_point();
                break;
            case 2:
                IZDA.draw_line();
                break;
            case 3:
                IZDA.draw_fill();
                break;
            case 4:
                IZDA.draw_chess();
                break;
            case 5:
                IZDA.draw_light_flat(mat);
                break;
            case 6:
                IZDA.draw_light_smooth(mat);
                break;
            case 7:
                IZDA.draw_fill(&index);
                break;

        }
	glLoadIdentity();
	glTranslatef(CSize,0,0);
    glScalef(-1,1,1);
	glGetFloatv(GL_MODELVIEW_MATRIX,MV[1]);
    glPopMatrix();

	glPopMatrix();
}

bool _teeth::move_lower(float step){
	DCHA.move_lower(step);
	return IZDA.move_lower(step);
}

bool _teeth::scale_lower(float step){
	DCHA.scale_lower(step);
	return IZDA.scale_lower(step);
}

void _teeth::draw_point(){
    this->draw_something(1);
}

void _teeth::draw_line(){
    this->draw_something(2);
}

void _teeth::draw_fill(){
    this->draw_something(3);
}

void _teeth::draw_fill(int* index){
    this->draw_something(7,-1,*index);
	*index += this->Tend;
}

void _teeth::draw_chess(){
    this->draw_something(4);
}
void _teeth::draw_light_flat(unsigned int mat){
    this->draw_something(5,mat);
}
void _teeth::draw_light_smooth(unsigned int mat){
    this->draw_something(6,mat);
}
void _teeth::change_light1(unsigned int val){
	DCHA.change_light1(val);
	IZDA.change_light1(val);
}

void _teeth::change_light2(unsigned int val){
	DCHA.change_light2(val);
	IZDA.change_light2(val);
}
void _teeth::move_light(int i, int j){
	DCHA.move_light(i,j);
	IZDA.move_light(i,j);
}
