#include "envmodel.h"
#include "passivemodel.h"
#include "boundingsphere.h"
#include "log.h"

GLuint EnvironmentModel::texture_id;

EnvironmentModel::EnvironmentModel() : PassiveModel() {
	damage = 0;
	score = 0;
	health = -1; // invincible
}

EnvironmentModel::EnvironmentModel(Eigen::Vector3f pos) : PassiveModel(pos) {
	damage = 0;
	score = 0;
	health = -1; // invincible
}

void EnvironmentModel::hit(Model *otherModel) {
	// do nothing! it's invincible
}

void EnvironmentModel::draw() {
    //PassiveModel::draw();
    
	glPushMatrix();
             
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, EnvironmentModel::texture_id);
     glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glDepthMask(GL_FALSE);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // transparency
    
    glTranslatef(position.x(), position.y(), position.z());
    glRotatef(rotate.x(), 1, 0, 0);
    glRotatef(rotate.y(), 0, 1, 0);
    glRotatef(rotate.z(), 0, 0, 1);
    glScalef(scale.x(), scale.y(), scale.z());

	 // land?
	 glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f(0.0f, 0.0f, 12.070650f);
      glTexCoord2f(0.0f, 0.0f);
      glVertex3f(5.0, 0.0f, 12.070650f);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f(5.0f, 0.0f, -12.070650f);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f(0.0f, 0.0f, -12.070650f);
     glEnd();
     
     glDisable(GL_BLEND);
     glDepthMask(GL_TRUE);
     glDisable(GL_TEXTURE_2D);
     glPopMatrix();
}

GLuint EnvironmentModel::loadImage(char * filename) {
	EnvironmentModel::texture_id = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if (0 == texture_id) {
        debug("SOIL loading error: '%s'", SOIL_last_result());
	} else {
        debug("texture %s loaded successfully", filename);
	}

	return texture_id;
}