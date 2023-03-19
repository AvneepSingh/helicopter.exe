#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

pthread_t d;
pthread_t c;

long int score = 0;

int px=30;
int py=6;

int grid[100][10];

void printgrid()
{
    system("/bin/stty cooked");
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<100;j++)
        {
            if(j==px && i==py)
            {
                printf(">");
                continue;
            }
            if(grid[j][i]==0)
            {
                printf(" ");
            }
            if(grid[j][i]==1)
            {
                printf("+");
            }
        }
        printf("\n");
    }
    printf("         Distance Traveled : %ld\n",score);
    system("/bin/stty raw");
}
void movegridbyone()
{
    score++;
    for(int i=1;i<100;i++)
    {
        for(int j=0;j<10;j++)
        {
            grid[i-1][j] = grid[i][j];
        }
    }
}
int pos=0;
void createobs(int flag)
{
    int len = rand()%9;
    for(int i=0;i<10;i++)
    {
        grid[99][i] = 0;
    }
    if(flag == 0)
    {
        if(pos == 0)
        {
            for(int i=0;i<len;i++)
            {
                grid[99][i] = 1;
            }
        }
        if(pos == 1)
        {
            for(int i=9;i>(9-len);i--)
            {
                grid[99][i] = 1;
            }
        }
        pos = (pos+1)%2;
    }
}

void *rungrid(void *prm)
{
    int timer=0;
    while(1)
    {
        system("clear");
        printgrid();
        if(grid[px][py] == 1)
        {
            break;
        }
        movegridbyone();
        createobs(timer);
        timer = (timer+1)%10;
        usleep(90000);
    }
    printf("\n\n\n<< GAME OVER >>\n\n\n");
    printf("press <q> and <shift> to quit\n\n");
    return 0;
}

void *controller(void* prm)
{
    system("/bin/stty raw");
    char c;
    while(1)
    {
        c = getchar();
        if(c == 'w')
        {
            if(py>0)
            {
                py = py-1;
            }
        }
        if(c == 's')
        {
            if(py<9)
            {
                py = py+1;
            }
        }
        if(c == 'Q')
        {
            pthread_cancel(d);
            break;
        }
    }
    system("clear");
    system("/bin/stty cooked");
    return 0;
}

int main()
{
    printgrid();
    
    pthread_create(&d,NULL,rungrid,(void*)0);
    pthread_create(&c,NULL,controller,(void*)0);
    
    pthread_join(c,NULL);
    pthread_join(d,NULL);
}
