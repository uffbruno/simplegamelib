#the compiler
CC=g++

#compiler flags, for extra error-and-warning intolerance
CPPFLAGS=-Wall -Wextra -pedantic -Weffc++ -Werror -g

#some shell commands
RM=rm
RMDIR=rmdir
MKDIR=mkdir
CP=cp

#directory where all source code is located
SRCDIR=src

#directory where all objects are located
#TODO: adapt this makefile to generate *.o -> build/*.o
OBJDIR=.

#directory where our final executable will be located
BINDIR=.

#directory for the distribution
DISTDIR=dist

#directory with the app's resources
RESOURCESDIR=resources

# 'make' looks for objects in the directories specified by VPATH
VPATH=$(SRCDIR);$(OBJDIR);$(BINDIR)
COMMON_OBJECTS=animation.o character.o map2d.o sprite.o bounding_box.o char_2d.o tile.o camera.o
LIBNAME=libsglib.a

all: $(LIBNAME)

$(LIBNAME): $(COMMON_OBJECTS)
	ar rvs $(LIBNAME) $(COMMON_OBJECTS)
    
#header file dependencies
animation.o: animation.hpp bounding_box.hpp
character.o: character.hpp game_object.hpp
map2d.o: map2d.hpp tile.hpp
sprite.o: sprite.hpp animation.hpp
bounding_box.o: bounding_box.hpp vector2d.hpp
char_2d.o: char_2d.hpp character.hpp
tile.o: tile.hpp game_object.hpp
camera.o: camera.hpp vector2d.hpp game_object.hpp

dist: $(LIBNAME)
	$(MKDIR) $(DISTDIR)
	$(CP) $(BINDIR)\$(LIBNAME) $(DISTDIR)
	strip $(DISTDIR)\$(EXECUTABLE)
    
dist-zip: dist
	7z a $(DISTDIR).zip $(DISTDIR)\* -r
    
clean:
	$(RM) $(COMMON_OBJECTS) $(LIBNAME)

.PHONY: clean-dist
clean-dist:
	$(RM) -rf $(DISTDIR)
	$(RM) $(DISTDIR).zip
