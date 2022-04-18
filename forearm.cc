#include "forearm.h"

_forearm::_forearm(float CSize,float OSize,float TSize){
    right =  _teeth(CSize,TSize);
    left  =  _teeth(CSize,TSize);
    upper =  _cube(CSize);
	this->OSize = OSize;
	this->CSize = CSize;
	this->TSize = TSize;
	this->Tend = 1 + right.Tend + 1 + left.Tend + 1;
}

void _forearm::setSelected(int i, int j){
	int c = -1;
	if(i==Tvalue)
		c = 0;
	else if(i>=Tvalue+1 and i<=right.getT())
		c = 1;
	else if(i>right.getT())
		c = 2;
	switch(c){
		case 0:
			upper.setSelected(j);
			break;
		case 1:
			right.setSelected(i,j);
			break;
		case 2:
			left.setSelected(i,j);
			break;
		default:
			upper.setSelected(-1);
			right.setSelected(-1,-1);
			left.setSelected(-1,-1);
			break;
	}
}

_vertex3f _forearm::getSelected(int i, int j){
	_vertex3f res,base;
	int c = -1;
	if(i==Tvalue)
		c = 0;
	else if(i>=Tvalue+1 and i<=right.getT())
		c = 1;
	else if(i>right.getT())
		c = 2;
	switch(c){
		case 0:
			base = upper.getSelected(j);
			res._0 = MV[0][0]*base._0 + MV[0][4]*base._1 + MV[0][8]*base._2 + MV[0][12];
			res._1 = MV[0][1]*base._0 + MV[0][5]*base._1 + MV[0][9]*base._2 + MV[0][13];
			res._2 = MV[0][2]*base._0 + MV[0][6]*base._1 + MV[0][10]*base._2 +MV[0][14];
			break;
		case 1:
			base = right.getSelected(i,j);
			res._0 = MV[1][0]*base._0 + MV[1][4]*base._1 + MV[1][8]*base._2 + MV[1][12];
			res._1 = MV[1][1]*base._0 + MV[1][5]*base._1 + MV[1][9]*base._2 + MV[1][13];
			res._2 = MV[1][2]*base._0 + MV[1][6]*base._1 + MV[1][10]*base._2 +MV[1][14];
			break;
		case 2:
			base = left.getSelected(i,j);
			res._0 = MV[2][0]*base._0 + MV[2][4]*base._1 + MV[2][8]*base._2 + MV[2][12];
			res._1 = MV[2][1]*base._0 + MV[2][5]*base._1 + MV[2][9]*base._2 + MV[2][13];
			res._2 = MV[2][2]*base._0 + MV[2][6]*base._1 + MV[2][10]*base._2 +MV[2][14];
			break;
		default:break;
	}
	return res;
}

void _forearm::draw_something(unsigned int type, unsigned int mat,int index){
	if(index>=0)
		this->Tvalue = index;
    glPushMatrix();
        glTranslatef(0,(-delta*CSize)/2.0,0);
            glScalef(1,OSize+delta,1);
            switch(type){
                case 1:
                    upper.draw_point();
                    break;
                case 2:
                    upper.draw_line();
                    break;
                case 3:
                    upper.draw_fill();
                    break;
                case 4:
                    upper.draw_chess();
                    break;
                case 5:
                    upper.draw_light_flat(mat);
                    break;
                case 6:
                    upper.draw_light_smooth(mat);
                    break;
                case 7:
                    upper.draw_fill(&index);
                    break;

                }
			glLoadIdentity();
			glTranslatef(0,(-delta*CSize)/2.0,0);
			glScalef(1,OSize+delta,1);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[0]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,(-CSize*OSize)/2.0-delta*CSize,0);
            switch(type){
                case 1:
                    right.draw_point();
                    break;
                case 2:
                    right.draw_line();
                    break;
                case 3:
                    right.draw_fill();
                    break;
                case 4:
                    right.draw_chess();
                    break;
                case 5:
                    right.draw_light_flat(mat);
                    break;
                case 6:
                    right.draw_light_smooth(mat);
                    break;
                case 7:
                    right.draw_fill(&index);
                    break;
                }
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0,(-CSize*OSize)/2.0-delta*CSize,0);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[1]);
			glPopMatrix();

        glRotatef(90,0,1,0);
            switch(type){
                case 1:
                    left.draw_point();
                    break;
                case 2:
                    left.draw_line();
                    break;
                case 3:
                    left.draw_fill();
                    break;
                case 4:
                    left.draw_chess();
                    break;
                case 5:
                    left.draw_light_flat(mat);
                    break;
                case 6:
                    left.draw_light_smooth(mat);
                    break;
                case 7:
                    left.draw_fill(&index);
                    break;
                }

			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0,(-CSize*OSize)/2.0-delta*CSize,0);
			glRotatef(90,0,1,0);
			glGetFloatv(GL_MODELVIEW_MATRIX,MV[2]);
			glPopMatrix();

        glPopMatrix();
}

bool _forearm::move_claw(float step){
    this->right.move_lower(step);
    bool moved = this->left.move_lower(step);
    if(!moved){
        //cout << "MAX/MIN MOVEMENT" << endl;
        return false;
    }
    return true;
}
bool _forearm::expand_claw(float step){
    this->right.scale_lower(step);
    bool moved = this->left.scale_lower(step);
    if(!moved){
        return false;
    }
    return true;
}

bool _forearm::expand(float step){
    if(delta+step/10.0>0 and delta+step/10.0<=5){
        delta+=step/10.0;
        return true;
    }
    else{
        return false;
    }
}


void _forearm::draw_point(){
    this->draw_something(1);
}
void _forearm::draw_line(){
    this->draw_something(2);
}
void _forearm::draw_fill(){
    this->draw_something(3);
}
void _forearm::draw_fill(int* index){
    this->draw_something(7,-1,*index);
	*index += this->Tend;
}
void _forearm::draw_chess(){
    this->draw_something(4);
}
void _forearm::draw_light_flat(unsigned int mat){
    this->draw_something(5,mat);
}
void _forearm::draw_light_smooth(unsigned int mat){
    this->draw_something(6,mat);
}
void _forearm::change_light1(unsigned int val){
    right.change_light1(val);
    left.change_light1(val);
    upper.change_light1(val);
}

void _forearm::change_light2(unsigned int val){
    right.change_light2(val);
    left.change_light2(val);
    upper.change_light2(val);
}
void _forearm::move_light(int i, int j){
    right.move_light(i,j);
    left.move_light(i,j);
    upper.move_light(i,j);
}
