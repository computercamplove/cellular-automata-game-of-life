#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void simulation(int rows, int columns);
char* create(int rows, int columns);
double randomGenerator(double min, double max);
void display(int rows, int col, char* mtrx);
int countNeighbor(int rows, int columns, int x, int y, char* simulation);
char* step(int rows, int col, char* prevSimulation);
   

int main(int argc, char* argv[])
{
    int rows = atoi(argv[1]);
    if(rows <= 0) {
        printf("Rows must be greater than 0 here is %d\n", rows);
        return -1;
    }
    rows+=2;

    int col = atoi(argv[2]);
    if(col <= 0) {
        printf("Rows must be greater than 0 here is %d\n", col);
        return -1;
    }
    col+=2;

    simulation(rows, col);
}

void display(int rows, int col, char* mtrx) {
    printf("\n\n\n\n\n");
    for(int y = 1; y < rows - 1; y++) {
        for(int x = 1; x < col - 1; x++){
            printf("%c ", *(mtrx + y*col + x));
        }
        printf("\n");
    }
}

/* function findNeighbor() count how many neighbors around current cell*/
int findNeighbor(int rows, int columns, int x, int y, char* simulation)
{
    int count = 0;

    int pivot = y*columns+x;
    /*
    . . . . . .
    . 1 2 3 4 .
    . 1 2 3 4 .
    . 1 2 3 4 . 
    . 1 2 3 4 . 
    . . . . . .
    */
    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++)
        {
            char c = *(simulation+pivot+(i*columns)+j);

            if(c == '#') count++;
        }
    }
    return count;
}


/*
    function step() to generate new simulation from previous simulation
    using funciotn findNeighbor() to check how many neighbors around cell to apply Rules

    RULES (from wikipedia)
    1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    2. Any live cell with two or three live neighbours lives on to the next generation.
    3. Any live cell with more than three live neighbours dies, as if by overpopulation.
    4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

char* step(int rows, int columns, char* prevSimulation)
{
    char* stepSimulation = calloc(rows*columns, sizeof(int));
    if(stepSimulation == NULL) return NULL;

    for(int y = 1; y < rows - 1; y++) {
        for(int x = 1; x < columns - 1; x++){
            int live = findNeighbor(rows, columns, x, y, prevSimulation);
            char cell = *(prevSimulation+y*columns+x);
            if(cell == '#') live--;
            *(stepSimulation+y*columns+x) = cell;
            // 1st Rule - underpopulation
            if(live < 2)
            {
                *(stepSimulation+y*columns+x) = '.'; //dead
            }
            // 2nd Rule - next generation
            else if((live == 2 || live == 3) && cell == '#')
            {
                *(stepSimulation+y*columns+x) = '#'; //live
            }
            // 3rd Rule - overpopulation
            else if(live > 3 && cell == '#')
            {
                *(stepSimulation+y*columns+x) = '.'; //dead
            }
            // 4th Rule - reproduction
            else if(live == 3 && cell == '.') 
            {
                *(stepSimulation+y*columns+x) = '#'; //live
            }

        }
    }
    return stepSimulation;
}


void simulation(int rows, int columns) {
    puts("Begining");
    char* sim = create(rows, columns);
    if(sim == NULL) return;
    display(rows, columns, sim);

    while(1)
    {
        char* newSim = step(rows, columns, sim);
        if(newSim == NULL) return;
        free(sim);
        sim = newSim;
        display(rows, columns, sim);
        usleep(1000000);
    }
}

double randomGenerator(double min, double max) {
    return ((double)rand()/RAND_MAX)*(max-min)+min;
}



char* create(int rows, int columns){
    char* mtrx = (char*)calloc(rows*columns, sizeof(char));
    if(mtrx == NULL)
    {
        return NULL;
    }
    for(int y = 1; y < rows-1; y++){
        for(int x = 1; x < columns-1; x++){
            if(randomGenerator(0.0, 1.0) <= 0.35) //if at least 35% that cell is alive
            {
                *(mtrx + y*columns + x) = '#';
            }
            else 
            {
                *(mtrx + y*columns + x) = '.';
            }
        }
    }
    return mtrx;
}


