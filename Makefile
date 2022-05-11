BUILDDIR = .
SOURCEDIR = .
OBJS = $(SOURCEDIR)/Main.cpp $(SOURCEDIR)/Game.cpp $(SOURCEDIR)/StateStack.cpp $(SOURCEDIR)/State.cpp $(SOURCEDIR)/MenuState.cpp $(SOURCEDIR)/GameState.cpp $(SOURCEDIR)/GameOverState.cpp $(SOURCEDIR)/PauseState.cpp $(SOURCEDIR)/Playfield.cpp $(SOURCEDIR)/Tetromino.cpp $(SOURCEDIR)/TetrominoPreview.cpp $(SOURCEDIR)/Animation.cpp $(SOURCEDIR)/GameOverAnimation.cpp $(SOURCEDIR)/ClearLinesAnimation.cpp $(SOURCEDIR)/SoundPlayer.cpp $(SOURCEDIR)/MusicPlayer.cpp

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system

OBJ_NAME = $(BUILDDIR)/main

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
