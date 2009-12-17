#include "crosshair.h"
#include "log.h"

Crosshair::Crosshair() : Geometry("crosshair") {
	loadImage((char *) "textures/crosshair.png");
    update();
};

void Crosshair::draw(){
    glScalef(0.07, 0.07, 0.07);
    
	glEnable(GL_TEXTURE_2D); // enables texture mapping

	glBindTexture(GL_TEXTURE_2D, texture_id); // set the active texture 

    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // transparency
	glEnable(GL_BLEND);

	//glPushMatrix();

	// 1x1 square at position 0,0,0
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
	glEnd();
	
	//glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
    
}

void Crosshair::update(){
}

void Crosshair::dump(){
    LOG("\tType:\tCrosshair",1);
}

GLuint Crosshair::loadImage(char * filename) {
	
	texture_id = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if( 0 == texture_id )
	{
		debug("SOIL loading error: '%s'", SOIL_last_result());
	} else {
		debug("texture %s loaded successfully", filename);
	}

	return texture_id;
}