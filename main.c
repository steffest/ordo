#include "proto.h"

BOOL playMusic = TRUE;

int main() {
    check_free_memory();

    if (!openScreen()) return 1;
    hideMouse();
    srand(1);

    show_loader();
    if (playMusic) play_mod_from_asset(0);
    intro();
    winter();
    waves();
    bears();
    greetings();
   
    //mute_mod();
    printf("\n");
    printf("------------------------\n");
    printf("          ORDO          \n");
    printf("------------------------\n\n");
    printf(" Released at RSYNC 2024 \n");
    printf("     06 January 2024    \n");
    printf("    Leuven - Belgium    \n\n");
    printf("  Thanks for watching!  \n\n");
    printf("Yes, I know this demo is heavy on pixels and light on effects\n");
    printf("But hey, it's open source and available on\n");
    printf("https://github.com/steffest/ordo\n");
    printf("Feel free to improve :-)\n");
    printf("------------------------\n");
    printf("        Steffest        \n");
    printf("------------------------\n");

    Delay(50);

    free_assets();
    closeScreen();

    if (playMusic){
        stop_mod();
        Delay(10);

        play_mod("data/end.mod");
        Delay(50);
        stop_mod();
    }
   
    Delay(10);
    return 0;
}

