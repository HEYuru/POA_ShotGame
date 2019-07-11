.SUFFIXES:
.SUFFIXES:      .cpp .o .h

FMOD		= fmod/4.3

O			= Labyrinthe.o Chasseur.o Gardien.o
OBJ			= Labyrinthe.obj Chasseur.obj Gardien.obj


LIBS		= $(FMOD)/libfmodex64.so -lglut -lGLU -lGL jpeg/libjpeg.a -lm


CXX			= g++ -std=c++11
CXXFLAGS	= -I $(FMOD) -O4 -Wall

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

labh:	$(O)
	$(CXX) -o $@ $(O) OpenGL64.o $(LIBS)

clean:
	@rm -f labh $(O) maclabh labh.exe $(OBJ)

Labyrinthe.o:	Labyrinthe.h Chasseur.h Gardien.h
Chasseur.o: Chasseur.cpp Chasseur.h
Gardien.o: Gardien.cpp Gardien.h
