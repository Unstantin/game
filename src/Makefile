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

TARGET_DIR = ..
TARGET_NAME_FULL := $(strip $(TARGET_DIR))/$(TARGET)

OBJ := $(SRC:.c=.o)
HDR := $(SRC:.c=.h)

GITHUB_DIR = $(strip $(TARGET_DIR))/github
GITHUB_SRC_DIR := $(strip $(GITHUB_DIR))/src
GITHUB_IMG_DIR := $(strip $(GITHUB_DIR))/img

################################

.PHONY: run debug ctags start clean github-src

run: $(TARGET)
	cd $(TARGET_DIR) && ./$<

$(TARGET): $(OBJ)
	gcc $^ $(LDLIBS) -o $(TARGET_NAME_FULL)

%.o: %.c 
	gcc -c $(CFLAGS) $<

$(OBJ): $(HDR) Makefile 

$(HDR): | start 
	touch $@
        
debug: $(TARGET)
	cd $(TARGET_DIR) && gdb -q $<

ctags:
	ctags -R

start: 
	@printf '\n\e[32;1m******************** Start ********************\e[0m\n\n'

clean:
	@find . \( -name "*.o" -o -name ".*.sw?" \) -print0 | xargs -0 rm -vf
        
github-src:
	@rm -rf $(GITHUB_SRC_DIR)
	@mkdir -p $(GITHUB_SRC_DIR) && cp -v $(SRC) $(HDR) Makefile $(GITHUB_SRC_DIR)
	
