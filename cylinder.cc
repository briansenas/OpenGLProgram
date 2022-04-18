#include "cylinder.h"

_cylinder::_cylinder(float Size,unsigned int N,bool upper_, bool lower_)
{
	this->upper = upper_;
	this->lower = lower_;
	Vertices.clear();
	Vertices.push_back(_vertex3f(Size,-Size/2.0,0));
	/*Vertices.push_back(_vertex3f(Size,-Size/4.0,0));
	Vertices.push_back(_vertex3f(Size,0,0));
	Vertices.push_back(_vertex3f(Size,Size/4.0,0));*/
	Vertices.push_back(_vertex3f(Size,Size/2.0,0));
	Vertices.push_back(_vertex3f(0,Size/2.0,0));
	Vertices.push_back(_vertex3f(0,-Size/2.0,0));

	this->stepV = Vertices.size()-2;
	_operate::_rotateY(Vertices,Triangles,N,360,upper,lower);
	this->stepH = N;
	this->steps = N*(stepV-1);
}
void _cylinder::draw_texture(){

	this->repeat_vertex();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT,GL_FILL);
	glBegin(GL_TRIANGLES);

	unsigned int N_cols = this->stepV;
	float vert = 1.0/((N_cols+1)*1.0);
	float diff = 1.0/((stepH)*1.0);
	float last = diff;
	float coord = vert;
	unsigned int cont = 0;
	for(unsigned int i=0;i<steps-stepV+1;i++){
		glTexCoord2f(last-diff,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
		glTexCoord2f(last-diff,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
		cont++;
		glTexCoord2f(last,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
		glTexCoord2f(last-diff,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
		cont++;
		if(i % (stepV-1) < stepV-2)
			coord += vert;
		else {
			coord = vert;
			last += diff;
		}
	}
	unsigned int offset=this->ax*stepH+stepV;
	for(unsigned int i=0;i<stepV-1;i++){
		glTexCoord2f(last-diff,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
		glTexCoord2f(last-diff,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
		cont++;
		glTexCoord2f(last,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i+1]);
		glTexCoord2f(last-diff,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
		//last += diff;
		coord+=vert;
	}
	cont = (stepV+(stepV-2))*stepH;
	if(lower){
		coord = 0;
		last = diff;
		for(unsigned int i=0;i<stepH;i++){
			glTexCoord2f(last-diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
			glTexCoord2f(last,coord+vert);
			if(i==stepH-1)
				glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset]);
			else
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			last+=diff;
			cont++;
		}
		cont = (stepV+1+(stepV-2))*stepH;
	}
	if(upper){
		coord = 1;
		last = 1-diff;
		for(unsigned int i=0;i<stepH;i++){
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
			glTexCoord2f(last-diff,coord-vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last,coord-vert);
			if(i==stepH-1)
				glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+stepV-1]);
			else
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			last-=diff;
			if(i==stepH-2)
				last = 0;
			cont++;
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void _cylinder::enable_textured_light(unsigned int var){
	this->enable_lights(var);
	glShadeModel(GL_FLAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Multiply texture colors with light values
	//DECAL only draws the texture, and REPLACE gives priority to the light

	glEnable(GL_TEXTURE_2D);
}

void _cylinder::draw_texture_flat(unsigned int var){
	if(TNormals.empty())
		this->compute_normal_triangles();

	if(TNormals.empty())
		cout << "[ERROR]Empty Normal Vector...." << endl;
	else{

		this->enable_textured_light(var);
		glPolygonMode(GL_FRONT,GL_FILL);
		glBegin(GL_TRIANGLES);

		this->repeat_vertex();

		unsigned int N_cols = this->stepV;
		float vert = 1.0/((N_cols+1)*1.0);
		float diff = 1.0/((stepH)*1.0);
		float last = diff;
		float coord = vert;
		unsigned int cont = 0;
		for(unsigned int i=0;i<steps-stepV+1;i++){
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glTexCoord2f(last-diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			cont++;
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			cont++;
			if(i % (stepV-1) < stepV-2)
				coord += vert;
			else {
				coord = vert;
				last += diff;
			}
		}
		unsigned int offset=this->ax*stepH+stepV;
		for(unsigned int i=0;i<stepV-1;i++){
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glTexCoord2f(last-diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			cont++;
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i+1]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			cont++;
			//last += diff;
			coord+=vert;
		}
		cont = (stepV+(stepV-2))*stepH;
		if(lower){
			coord = 0;
			last = diff;
			for(unsigned int i=0;i<stepH;i++){
				glNormal3fv((const GLfloat *) &TNormals[cont]);
				glTexCoord2f(last-diff,coord);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
				glTexCoord2f(last,coord+vert);
				if(i==stepH-1)
					glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset]);
				else
					glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
				glTexCoord2f(last-diff,coord+vert);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
				last+=diff;
				cont++;
			}
			cont = (stepV+1+(stepV-2))*stepH;
		}
		if(upper){
			coord = 1;
			last = 1-diff;
			for(unsigned int i=0;i<stepH;i++){
				glNormal3fv((const GLfloat *) &TNormals[cont]);
				glTexCoord2f(last,coord);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
				glTexCoord2f(last-diff,coord-vert);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
				glTexCoord2f(last,coord-vert);
				if(i==stepH-1)
					glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+stepV-1]);
				else
					glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
				last-=diff;
				if(i==stepH-2)
					last = 0;
				cont++;
			}
		}
	}
	glEnd();
	this->disable_textured_light();
}

void _cylinder::disable_textured_light(){
	glDisable(GL_TEXTURE_2D);
	this->disable_lights();
}

void _cylinder::draw_texture_smooth(unsigned int var){
	if(PNormals.empty())
		this->compute_normal_vertex();

	if(PNormals.empty())
		cout << "[ERROR]Empty Normal Vector...." << endl;
	else{

		this->enable_textured_light(var);
		glPolygonMode(GL_FRONT,GL_FILL);
		glBegin(GL_TRIANGLES);

		this->repeat_vertex();

		unsigned int N_cols = this->stepV;
		float vert = 1.0/((N_cols+1)*1.0);
		float diff = 1.0/((stepH)*1.0);
		float last = diff;
		float coord = vert;
		unsigned int cont = 0;
		for(unsigned int i=0;i<steps-stepV+1;i++){
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last-diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			cont++;
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			cont++;
			if(i % (stepV-1) < stepV-2)
				coord += vert;
			else {
				coord = vert;
				last += diff;
			}
		}
		unsigned int offset=this->ax*stepH+stepV;
		for(unsigned int i=0;i<stepV-1;i++){
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last-diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			cont++;
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i+1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last-diff,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			cont++;
			coord+=vert;
		}
		cont = (stepV+(stepV-2))*stepH;
		if(lower){
			coord = 0;
			last = diff;
			for(unsigned int i=0;i<stepH;i++){
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
				glTexCoord2f(last-diff,coord);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
				glTexCoord2f(last,coord+vert);
				if(i==stepH-1)
					glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset]);
				else
					glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
				glTexCoord2f(last-diff,coord+vert);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
				last+=diff;
				cont++;
			}
		cont = (stepV+1+(stepV-2))*stepH;
		}
		if(upper){
			coord = 1;
			last = 1-diff;
			for(unsigned int i=0;i<stepH;i++){
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
				glTexCoord2f(last,coord);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
				glTexCoord2f(last-diff,coord-vert);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
				glTexCoord2f(last,coord-vert);
				if(i==steps-1)
					glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+stepV-1]);
				else
					glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
				last-=diff;
				if(i==stepH-2)
					last = 0;
				cont++;
			}
		}
	}
	glEnd();
	this->disable_textured_light();
}
