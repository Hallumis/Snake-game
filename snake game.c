#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <unistd.h>

#define cols 25
#define rows 25
#define foods 50
#define SNAKE_MAX_LEN 256

char board[cols * rows];

int isGameOver = 0;

void fill_board() {
    int x,y;
    
    for(y=0; y<rows; y++) {
        for(x=0; x<cols; x++) {
            if(x==0||y==0||x==cols-1||y==rows-1) {
                board[y*cols + x] = '#';
            }else{
                board[y*cols + x] = ' ';
            }
        }
    }
}

void print_board() {
    int x,y;
        
    for(y=0; y<rows; y++) {
        for(x=0; x<cols; x++) {
            putch(board[y*cols + x]);
        }
        putch('\n');
    }    
}


struct SnakePart {
    int x, y;
};
struct Snake {
    int length;
    struct SnakePart part[SNAKE_MAX_LEN];
};
struct Snake snake;

struct Food {
    int x, y;
    int consumed;
};
struct Food food[foods];

void draw_snake() {
    int i;
    
    for(i=snake.length-1; i>0; i--) {
        board[snake.part[i].y*cols + snake.part[i].x] = '*';
    }
    board[snake.part[0].y*cols + snake.part[0].x] = '@';
}

void setup_snake() {
    snake.length = 1;
    snake.part[0].x = 1 + rand() % (cols-2);
    snake.part[0].y = 1 + rand() % (rows-2);
}



void move_snake(int deltaX, int deltaY) {
    int i;

    for(i=snake.length-1; i>0; i--) {
        snake.part[i] = snake.part[i-1];
    }
    
    snake.part[0].x += deltaX;
    snake.part[0].y += deltaY;
}

void read_keyboard(int *dx, int *dy) {
    static char lastChar = 'd';  // Default to moving right

    if (_kbhit()) {  
        lastChar = _getch();  // Get key press and update lastChar
    } 

    // Use last pressed key to determine movement
    switch (lastChar) {
        case 'w': *dx =  0; *dy = -1; break;
        case 's': *dx =  0; *dy =  1; break;
        case 'a': *dx = -1; *dy =  0; break;
        case 'd': *dx =  1; *dy =  0; break;
    }
    Sleep(1000);  // Short delay to control speed 
    
}

void draw_food() {
    int i;
    
    for(i=0; i<foods; i++) {
        if( !food[i].consumed ) {
            board[food[i].y*cols + food[i].x] = '+';
        }
    }
}

void setup_food() {
    int i;
    
    for(i=0; i<foods; i++) {
        food[i].x = 1 + rand() % (cols-2);
        food[i].y = 1 + rand() % (rows-2);
        food[i].consumed = 0;
    }
}

void game_rules() {
    int i;
    
    for(i=0; i<foods; i++) {
        if( !food[i].consumed ) {
            if( food[i].x == snake.part[0].x &&
                food[i].y == snake.part[0].y ) {
                    food[i].consumed = 1;
                    snake.length++;
            }
        }
    }
    
    if( snake.part[0].x == 0 || snake.part[0].x == cols-1 ||
        snake.part[0].y == 0 || snake.part[0].y == rows-1 ) {
            isGameOver = 1;
    }
    
    for(i=1; i<snake.length; i++) {
        if( snake.part[0].x == snake.part[i].x &&
            snake.part[0].y == snake.part[i].y ) {
                isGameOver = 1;
        }
    }
}


int main(int argc, char **argv)
{   
    srand(time(0));
    
    setup_snake();
    setup_food();
    
    while(!isGameOver) {
        fill_board();
        draw_food();
        draw_snake();
        game_rules();
        //clear_screen();
        printf("Snake Game, Score: %d\n", snake.length * 100);
        print_board();
        if( !isGameOver ) {
            int dx = 0, dy = 0;
            read_keyboard(&dx, &dy);
            move_snake(dx, dy);
        Sleep(1000);  // Delay for 5 seconds (5000 ms)
        
        }
    }
    printf("Game Over, Final score: %d\n", snake.length * 100);
    
    while(1) getch();
    
	return 0;
    
}