#include "sphere.h"

_sphere::_sphere(float Size,unsigned int N,unsigned int N2,bool upper_, bool lower_)
{
	this->upper = upper_;
	this->lower = lower_;
    Vertices.clear();
    Vertices.push_back(_vertex3f(0,-Size,0));
    _operate::_rotateZ(Vertices,Triangles,N,180);
    _operate::_rotateY(Vertices,Triangles,N2,360,upper,lower);
	this->stepV = N;
	this->stepH = N2;
	this->steps = (N-2)*N2;
}
void _sphere::draw_texture(){

	if(!repeated)
		this->repeat_vertex();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT,GL_FILL);
	glBegin(GL_TRIANGLES);

	float vert = 1.0/((stepV)*1.0);
	float diff = 1.0/((stepH)*1.0);
	float last = 0;
	float coord = vert;
	unsigned int cont = 0;
	for(unsigned int i=0;i<steps-stepV+2;i++){
		glTexCoord2f(last,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last+diff,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
		cont++;
		coord += vert;
		glTexCoord2f(last+diff,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last+diff,coord-vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
		if(abs(1-vert-coord)<=1e-9) {
			coord = vert;
			last += diff;
		}
		cont++;
	}
	unsigned int offset = this->ax*(stepH)+(stepV); //remove upper/lower and get first last
	for(unsigned int i=0;i<stepV-2;i++){
		glTexCoord2f(last,coord+vert);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last+diff,coord);
		glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
		cont++;
		coord += vert;
		glTexCoord2f(last+diff,coord);
		glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i+1]);
		glTexCoord2f(last,coord);
		glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
		glTexCoord2f(last+diff,coord-vert);
		glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
		if(abs(1-vert-coord)<=1e-9) {
			coord = vert;
			last += diff;
		}
		cont++;
	}
	cont = 2*steps;
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
		cont = 2*steps+stepH;
	}
	if(upper){
		coord = (stepV-1)*vert;
		last = (stepH-1)*diff;
		for(unsigned int i=0;i<stepH;i++){
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
			glTexCoord2f(last-diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			if(i==stepH-1)
				glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-this->ax*stepH-2]);
			else
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			last-=diff;
			cont++;
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void _sphere::enable_textured_light(unsigned int var){
	this->enable_lights(var);
	glShadeModel(GL_FLAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //Multiply texture colors with light values
	//DECAL only draws the texture, and REPLACE gives priority to the light

	glEnable(GL_TEXTURE_2D);
}

void _sphere::draw_texture_flat(unsigned int var){
	if(TNormals.empty())
		this->compute_normal_triangles();

	if(TNormals.empty())
		cout << "[ERROR]Empty Normal Vector...." << endl;
	else{
		this->enable_textured_light(var);
		if(!repeated)
			this->repeat_vertex();
		glBegin(GL_TRIANGLES);

		float vert = 1.0/((stepV)*1.0);
		float diff = 1.0/((stepH)*1.0);
		float last = 0;
		float coord = vert;
		unsigned int cont = 0;
		for(unsigned int i=0;i<steps-stepV+2;i++){
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last+diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			cont++;
			coord += vert;
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last+diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last+diff,coord-vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			if(abs(1-vert-coord)<=1e-9) {
				coord = vert;
				last += diff;
			}
			cont++;
		}
		unsigned int offset = this->ax*stepH+stepV; //remove upper/lower and get first last
		for(unsigned int i=0;i<stepV-2;i++){
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last+diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			cont++;
			coord += vert;
			glNormal3fv((const GLfloat *) &TNormals[cont]);
			glTexCoord2f(last+diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i+1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glTexCoord2f(last+diff,coord-vert);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			if(abs(1-vert-coord)<=1e-9) {
				coord = vert;
				last += diff;
			}
			cont++;
		}
		cont = 2*steps;
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
			cont = 2*steps+stepH;
		}
		if(upper){
			coord = (stepV-1)*vert;
			last = (stepH-1)*diff;
			for(unsigned int i=0;i<stepH;i++){
				glNormal3fv((const GLfloat *) &TNormals[cont]);
				glTexCoord2f(last,coord+vert);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
				glTexCoord2f(last-diff,coord);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
				glTexCoord2f(last,coord);
				if(i==stepH-1)
					glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-this->ax*stepH-2]);
				else
					glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
				last-=diff;
				cont++;
			}
		}
	}
	glEnd();
	this->disable_textured_light();
}

void _sphere::disable_textured_light(){
	glDisable(GL_TEXTURE_2D);
	this->disable_lights();
}

void _sphere::draw_texture_smooth(unsigned int mat){
	if(PNormals.empty())
		this->compute_normal_vertex();

	if(PNormals.empty())
		cout << "[ERROR]Empty Normal Vector...." << endl;
	else{

		this->enable_textured_light(mat);
		if(!repeated)
			this->repeat_vertex();
		glBegin(GL_TRIANGLES);

		float vert = 1.0/((stepV)*1.0);
		float diff = 1.0/((stepH)*1.0);
		float last = 0;
		float coord = vert;
		unsigned int cont = 0;
		for(unsigned int i=0;i<steps-stepV+2;i++){
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last+diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			cont++;
			coord += vert;
			glTexCoord2f(last+diff,coord);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last+diff,coord-vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
			if(abs(1-vert-coord)<=1e-9) {
				coord = vert;
				last += diff;
			}
			cont++;
		}
		unsigned int offset = this->ax*stepH+stepV; //remove upper/lower and get first last
		for(unsigned int i=0;i<stepV-2;i++){
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glTexCoord2f(last,coord+vert);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last+diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			cont++;
			coord += vert;
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
			glTexCoord2f(last+diff,coord);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i+1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
			glTexCoord2f(last,coord);
			glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
			glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
			glTexCoord2f(last+diff,coord-vert);
			glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-offset+i]);
			if(abs(1-vert-coord)<=1e-9) {
				coord = vert;
				last += diff;
			}
			cont++;
		}
		cont = 2*steps;
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
			cont = 2*steps+stepH;
		}
		if(upper){
			coord = (stepV-1)*vert;
			last = (stepH-1)*diff;
			for(unsigned int i=0;i<stepH;i++){
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._0]);
				glTexCoord2f(last,coord+vert);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._0+i]);
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._1]);
				glTexCoord2f(last-diff,coord);
				glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._1]);
				glTexCoord2f(last,coord);
				glNormal3fv((const GLfloat *) &PNormals[Triangles[cont]._2]);
				if(i==stepH-1)
					glVertex3fv((const GLfloat *) &Vertices[Vertices.size()-this->ax*stepH-2]);
				else
					glVertex3fv((const GLfloat *) &Vertices[Triangles[cont]._2]);
				last-=diff;
				cont++;
			}
		}
	}

	glEnd();
	this->disable_textured_light();
}
