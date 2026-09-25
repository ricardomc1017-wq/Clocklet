#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "numbers.h"

#define GAP 2

// Definir el tamaño de los numeros

static int glyph_width(int g){
    return (int) strlen(glyphs[g][0]);
}

// Funcion para dibujar los numeros

static int draw_glyph(int y, int x, int g){
    for (int row = 0; row < GLYPH_HEIGHT; row++){
        mvaddstr(y + row, x, glyphs[g][row]);
    }
    return x + glyph_width(g) + GAP;
}

// Funcion principal

int main(int argc, char *argv[]){
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(200);

    if (has_colors()){

        start_color();
        use_default_colors();

        short fg = COLOR_WHITE;

        if (argc == 3 && strcmp(argv[1],"-c") == 0){

            if (strcmp(argv[2],"red") == 0){
                fg = COLOR_RED;
            } else if (strcmp(argv[2], "green") == 0){
                fg = COLOR_GREEN;
            } else if (strcmp(argv[2], "yellow") == 0){
                fg = COLOR_YELLOW;
            } else if (strcmp(argv[2], "blue") == 0){
                fg = COLOR_BLUE;
            } else if (strcmp(argv[2], "magenta") == 0){
                fg = COLOR_MAGENTA;
            } else if (strcmp(argv[2],"cyan") == 0){
                fg = COLOR_CYAN;
            } else if (strcmp(argv[2], "black") == 0){
                fg = COLOR_BLACK;
            }

        }

        init_pair(1, fg, -1);
    }

    int active = 1;

    // Bucle Principal

    while (active){
        time_t now = time(NULL);
        struct tm tm_now;
        localtime_r(&now, &tm_now);

        // Cambio de los numeros en tiempo real

        int cht = tm_now.tm_hour / 10;
        int cho = tm_now.tm_hour % 10;
        int cmt = tm_now.tm_min  / 10;
        int cmo = tm_now.tm_min  % 10;
        int colon_on = (tm_now.tm_sec % 2 == 0);

        // Definir el orden de los numeros a imprimir

        int seq[5] = { cht, cho, GLYPH_COLON, cmt, cmo };
        int total = 0;
        for (int i = 0; i < 5; i++) total += glyph_width(seq[i]) + GAP;
        total -= GAP;

        // Definir el tamaño de la ventana

        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        int y = (rows - GLYPH_HEIGHT) / 2;
        int x = (cols - total) / 2;
        if (y < 0) y = 0;
        if (x < 0) x = 0;

        // Dibujar los numeros

        erase();
        attron(COLOR_PAIR(1));
        for (int i = 0; i < 5; i++){
            if (seq[i] == GLYPH_COLON && !colon_on){
                x += glyph_width(GLYPH_COLON) + GAP;
            } else {
                x = draw_glyph(y, x, seq[i]);
            }
        }
        attroff(COLOR_PAIR(1));
        refresh();

        // Cerrar la ventana con 'q'

        int ch = getch();
        if (ch == 'q' || ch == 'Q') active = 0;
    }

    endwin();
    return 0;
}
