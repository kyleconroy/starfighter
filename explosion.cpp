#include "explosion.h"
#include "log.h"
#include "main.h"

GLuint Explosion::texture_ids[NUM_IMAGES];

Explosion::Explosion() : Geometry("explosion") {
    current_texture_index = 0;
    frame_counter = 0;
    update();
    playAudio("boom");
};

void Explosion::draw() {
    if (dead) {
        return;
    }

	frame_counter++;
	if (current_texture_index == NUM_IMAGES - 1) {
        debug("explosion ended");
        dead = true;
        return;
	} else if (frame_counter % FPP == 0) {
		current_texture_index++;
	}

    glPushMatrix();
        
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Explosion::texture_ids[current_texture_index]);
    glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // transparency

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
	
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Explosion::dump() {
    LOG("\tType:\tExplosion",1);
}

void Explosion::update(){
    
}

void Explosion::loadAnimationFrames() {
	int i = 0;
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion1.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion2.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion3.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion4.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion5.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion6.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion7.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion8.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion9.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion10.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion11.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion12.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion13.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion14.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion15.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion16.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion17.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion18.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion19.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion20.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion21.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion22.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion23.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion24.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion25.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion26.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion27.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion28.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion29.png");
	Explosion::texture_ids[i++] = loadImage((char *) "animations/explosion30.png");
	Explosion::texture_ids[i] = loadImage((char *) "animations/blank.png");
}

GLuint Explosion::loadImage(char * filename) {
	GLuint texture_id = SOIL_load_OGL_texture
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
