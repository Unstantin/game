#include "scene.h"
#include "mouse.h"
#include "keyboard.h"
#include "board.h"
#include "board-piece.h"
#include "board-wall.h"
#include "board-wall-rand.h"
#include "board-wall-float.h"
#include "help.h"
#include "render.h"

void scenePieceSelectClose();

scene scenes[SCENE_N] = {
    [SCENE_PIECE_MOVE] = {
        .mouseReceiver = mouseReceiverScenePieceMove,
        .keyboardReceiver = keyboardReceiverScenePieceMove,
        .render = drawBoard,
        .open = NULL,
        .close = NULL
    }, 
    [SCENE_WALL_FLOAT] = {
        .mouseReceiver = mouseReceiverSceneWallFloat,
        .keyboardReceiver = keyboardReceiverSceneWallFloat,
        .render = drawBoard,
        .open = boardWallFloatSceneOpen,
        .close = boardWallFloatSceneClose
    },
    [SCENE_PIECE_SELECT] = {
        .mouseReceiver = mouseReceiverScenePieceSelect,
        .keyboardReceiver = keyboardReceiverScenePieceSelect,
        .render = drawBoard,
        .open = NULL,
        .close = scenePieceSelectClose
    },
    [SCENE_HELP] = {
        .mouseReceiver = mouseReceiverSceneHelp,   
        .keyboardReceiver = keyboardReceiverSceneHelp,
        .render = drawHelp,
        .open = NULL,
        .close = NULL
    }
};

scene *sceneActive = NULL;

void sceneInit() {
//    sceneChange(SCENE_PIECE_MOVE); 
//    sceneChange(SCENE_PIECE_SELECT); 
    sceneChange(SCENE_HELP);
}

void sceneChange(unsigned int scenename) {
    if(scenename >= SCENE_N) return;
    scene *scn = &scenes[scenename];
    scene *scnPrev = sceneActive;

    if(scnPrev && scnPrev->close) scnPrev->close();
    if(scn->open) scn->open();

    mouseSwitch(scn->mouseReceiver);
    keyboardSwitch(scn->keyboardReceiver);
    renderSwitch(scn->render);

    sceneActive = scn;
}

void scenePieceSelectClose() {
    boardWallRandInit();

    boardPieceSelectedToEnabled();
    boardPieceActiveFirstEnabled();
}
