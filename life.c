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

int countNeighbor(int rows, int columns, int x, int y, char* simulation)
{
    int count = 0;
    int pivot = y*columns+x; //home cell
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
    RULES
    1.Any live cell with fewer than two live neighbours dies, as if by underpopulation.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overpopulation.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

char* step(int rows, int col, char* prevSimulation)
{
    char* stepSimulation = calloc(rows*col, sizeof(char));
    if(stepSimulation == NULL) return NULL;

    for(int y = 1; y < rows-1; y++) {
        for(int x = 1; x < col-1; x ++){
            int live = countNeighbor(rows, col, x, y, prevSimulation);
            char cell = *(prevSimulation + y*col+x);
            if(cell == '#') live--;
            // 1st Rule - underpopulation
            if(live < 2 && cell == '#')
            {
                *(stepSimulation+y*col+x) = '.'; //dead
            }
            // 2nd Rule - next generation
            else if((live == 2 || live == 3) && cell == '#')
            {
                *(stepSimulation+y*col+x) = '#'; //live
            }
            // 3rd Rule - overpopulation
            else if(live > 3 && cell == '#')
            {
                *(stepSimulation+y*col+x) = '.'; //dead
            }
            // 4th Rule - reproduction
            else if(cell == '.' && live == 3)
            {
                *(stepSimulation+y*col+x) = '#'; //live
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
        free(newSim);
        sim = newSim;
        display(rows, columns, sim);
        sleep(3);
    }
}

double randomGenerator(double min, double max) {
    return ((double)rand()/RAND_MAX)*(max-min)+min;
}

void display(int rows, int col, char* mtrx) {
    printf("\n\n\n\n\n");
    for(int y = 1; y < rows-1; y++) {
        for(int x = 1; x < col-1; x ++){
            printf("%c ", *(mtrx + y*col + x));
        }
        printf("\n");
    }
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


