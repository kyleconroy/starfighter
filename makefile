#!gmake 

CC = g++ -Wall -g
cc = gcc -Wall -g

TARGETS = starfighter

OBJECTS = main.o game.o level.o parser.o \
passivemodel.o activemodel.o ship.o \
objgeometry.o box.o log.o teapot.o plane.o wall.o ball.o explosion.o crosshair.o \
shipgeometry.o \
kdtree.o boundingbox.o boundingsphere.o force.o \
envmodel.o \
projectile.o asteroid.o kamikaze.o turret.o \
SOIL.o image_DXT.o image_helper.o stb_image_aug.o \
utilities.o

FRAMEWORK = -framework GLUT -framework OpenGL -framework CoreFoundation -framework AudioToolbox
MACLIBS = -lGL -lGLU -lm -lstdc++
MACINCS = -L"/System/Library/Frameworks/OpenGL.framework/Libraries"
EIGEN = -I eigen2
FIMAGE = -L./freeimage -lfreeimage
CCOPTS = $(OPTMAC) $(MACINCS) $(EIGEN) -DOSX -fast
LDOPTS = $(CCOPTS) $(MACLIBS)

default: $(TARGETS)

clean:
	/bin/rm -f *.o $(TARGETS)

################################################################################

starfighter: $(OBJECTS)
	$(CC) $(OBJECTS) $(FRAMEWORK) $(LDOPTS) -o starfighter

main.o: main.cpp main.h
	$(CC) main.cpp -c $(CCOPTS)
game.o: game.cpp game.h
	$(CC) game.cpp -c $(CCOPTS)
level.o: level.cpp level.h main.h
	$(CC) level.cpp -c $(CCOPTS)
parser.o: parser.cpp parser.h
	$(CC) parser.cpp -c $(CCOPTS)

passivemodel.o: passivemodel.cpp passivemodel.h model.h
	$(CC) passivemodel.cpp -c $(CCOPTS)
activemodel.o: activemodel.cpp activemodel.h model.h force.h
	$(CC) activemodel.cpp -c $(CCOPTS)
objgeometry.o: objgeometry.cpp objgeometry.h geometry.h
	$(CC) objgeometry.cpp -c $(CCOPTS)

ship.o: ship.cpp ship.h model.h
	$(CC) ship.cpp -c $(CCOPTS)
box.o: box.cpp box.h geometry.h
	$(CC) box.cpp -c $(CCOPTS)
plane.o: plane.cpp plane.h geometry.h
	$(CC) plane.cpp -c $(CCOPTS)
wall.o: wall.cpp wall.h
	$(CC) wall.cpp -c $(CCOPTS)
ball.o: ball.cpp ball.h
	$(CC) ball.cpp -c $(CCOPTS)
teapot.o: teapot.cpp teapot.h geometry.h
	$(CC) teapot.cpp -c $(CCOPTS)
explosion.o: explosion.cpp explosion.h geometry.h soil/SOIL.h
	$(CC) explosion.cpp -c $(CCOPTS)
crosshair.o: crosshair.cpp crosshair.h geometry.h main.h soil/SOIL.h
	$(CC) crosshair.cpp -c $(CCOPTS)
	
shipgeometry.o: shipgeometry.cpp shipgeometry.h objgeometry.h model.h
	$(CC) shipgeometry.cpp -c $(CCOPTS)

kdtree.o: kdtree/kdtree.c kdtree/kdtree.h
	$(cc) kdtree/kdtree.c -c $(CCOPTS)
boundingbox.o: boundingbox.cpp boundingbox.h boundingshape.h
	$(CC) boundingbox.cpp -c $(CCOPTS)
boundingsphere.o: boundingsphere.cpp boundingsphere.h boundingshape.h
	$(CC) boundingsphere.cpp -c $(CCOPTS)
force.o: force.cpp force.h activemodel.h
	$(CC) force.cpp -c $(CCOPTS)

envmodel.o: envmodel.cpp envmodel.h
	$(CC) envmodel.cpp -c $(CCOPTS)

projectile.o: projectile.cpp projectile.h
	$(CC) projectile.cpp -c $(CCOPTS)	
asteroid.o: asteroid.cpp asteroid.h enemy.h
	$(CC) asteroid.cpp -c $(CCOPTS)
kamikaze.o: kamikaze.cpp kamikaze.h enemy.h
	$(CC) kamikaze.cpp -c $(CCOPTS)
turret.o: turret.cpp turret.h enemy.h
	$(CC) turret.cpp -c $(CCOPTS)

log.o: log.cpp log.h
	$(CC) log.cpp -c $(CCOPTS)

SOIL.o: soil/SOIL.c soil/SOIL.h
	$(cc) soil/SOIL.c -c $(CCOPTS)
image_DXT.o: soil/image_DXT.c soil/image_DXT.h
	$(cc) soil/image_DXT.c -c $(CCOPTS)
image_helper.o: soil/image_helper.c soil/image_helper.h
	$(cc) soil/image_helper.c -c $(CCOPTS)
stb_image_aug.o: soil/stb_image_aug.c soil/stb_image_aug.h soil/stbi_DDS_aug.h soil/stbi_DDS_aug_c.h
	$(cc) soil/stb_image_aug.c -c $(CCOPTS)

utilities.o: utilities.cpp
	$(CC) utilities.cpp -c $(CCOPTS)
