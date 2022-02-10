TARGET = game
SRC = $(TARGET:=.c) polygon.c snowflake.c axes.c window.c \
    board.c color.c cell.c shape.c piece.c keyboard.c render.c \
    wall.c board-piece.c board-cell.c board-wall.c node.c mouse.c \
    scene.c board-wall-float.c board-wall-borderwall.c \
    board-wall-rand.c point.c board-path.c path.c board-piece-move.c \
    dir.c timer.c lodepng.c texture.c board-node.c help.c slide.c \
    help-texture.c

LDLIBS = -lm -lglut -lGL -lGLEW
CFLAGS = -g3 -fdiagnostics-color=always

OBJ := $(SRC:.c=.o)
HDR := $(SRC:.c=.h)

GITHUB_DIR = github
GITHUB_SRC_DIR := $(strip $(GITHUB_DIR))/src
GITHUB_IMG_DIR := $(strip $(GITHUB_DIR))/img

################################

.PHONY: all debug ctags start github

all: $(TARGET)
	./$<

$(TARGET): $(OBJ)
	gcc $^ $(LDLIBS) -o $@

%.o: %.c 
	gcc -c $(CFLAGS) $<

$(OBJ): $(HDR) Makefile 

$(HDR): | start 
	touch $@
        
debug: $(TARGET)
	gdb -q $(TARGET)

ctags:
	ctags -R

start: 
	@printf '\n\e[32;1m******************** Start ********************\e[0m\n\n'
        
github:
	@rm -rf $(GITHUB_SRC_DIR) $(GITHUB_IMG_DIR) 
	@mkdir -p $(GITHUB_SRC_DIR) && cp -v $(SRC) $(HDR) Makefile $(GITHUB_SRC_DIR)
	@cp -vr img $(GITHUB_DIR)
	
