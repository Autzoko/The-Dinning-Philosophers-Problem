#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

typedef struct chopstick
{
    char symbol;
    int is_available; // 0 for available, 1 for busy
    char user;
    
    // ab-1, bc-2, cd-3, de-4, ea-5
}CS;

typedef struct philosopher
{
    char name;
    int chop_num;
    int status; //1 for thinking, 0 for eating, -1 for waiting to eat, 2 for desire for eating
    int think_time;
    int eat_time;
    CS _x;
    CS _y;
}PLS;

void set();
int think(PLS person);
int pick_chop(PLS person);
int release_chop(PLS person);
int eat(PLS person);
int wait_to_eat(PLS person);
void *live(void *p);

CS _A, _B, _C, _D, _E;
PLS ph[5];

void set()
{
    ph[0].name = 'a';
    ph[1].name = 'b';
    ph[2].name = 'c';
    ph[3].name = 'd';
    ph[4].name = 'e';

    ph[0]._x = _A;
    ph[0]._y = _E;

    ph[1]._x = _A;
    ph[1]._y = _B;

    ph[2]._x = _B;
    ph[2]._y = _C;

    ph[3]._x = _C;
    ph[3]._y = _D;

    ph[4]._x = _D;
    ph[4]._y = _E;

    ph[0].think_time = 5;
    ph[0].eat_time = 5;

    ph[1].think_time = 7;
    ph[1].eat_time = 3;

    ph[2].think_time = 4;
    ph[2].eat_time = 6;

    ph[3].think_time = 0;
    ph[3].eat_time = 10;

    ph[4].think_time = 3;
    ph[4].eat_time = 7;

    _A.symbol = '1';
    _B.symbol = '2';
    _C.symbol = '3';
    _D.symbol = '4';
    _E.symbol = '5';

    _A.is_available = 0;
    _B.is_available = 0;
    _C.is_available = 0;
    _D.is_available = 0;
    _E.is_available = 0;
}

int think(PLS person)
{
    person.status = 1;
    printf("%c is thinking...\n", person.name);
    sleep(person.think_time);
    person.status = 2;
    return person.status;
}

int pick_chop(PLS person)
{
    if(person._x.is_available == 0)
    {
        person.chop_num++;
        person._x.is_available == 1;
        person._x.user = person.name;
        printf("%c pick chopstick %d\n", person.name, person._x.symbol);
    }
    if(person._y.is_available == 0)
    {
        person.chop_num++;
        person._y.is_available = 1;
        person._y.user = person.name;
        printf("%c pick chopstick %d\n", person.name, person._y.symbol);
    }
    return 0;
}

int release_chop(PLS person)
{
    person._x.is_available = 0;
    person._y.is_available = 0;
    person.chop_num = 0;
    return 0;
}

int eat(PLS person)
{
    if(person.status == 2)
    {
        if(person.chop_num == 2)
        {
            person.status = 0;
            printf("%c is eating...\n", person.name);
            sleep(person.eat_time);
            release_chop(person);
            printf("%c finished!\n", person.name);
            person.status = 1;
            return 1;
        }
        else
        {
            wait_to_eat(person);
        }
    }
    return 0;
}

int wait_to_eat(PLS person)
{
    if(person.status == 2)
    {
        if(person.chop_num == 1)
        {
            if(person._x.user != person.name)
            {
                person.status = -1;
                printf("%c is waiting to eat...\n", person.name);
                while(1)
                {
                    while(person._x.is_available != 0);
                    if(person._x.is_available == 0)
                        break;
                }
                person._x.user = person.name;
                person._x.is_available = 0;
                person.chop_num++;
                person.status = 2;
                eat(person);
            }
            if(person._y.user != person.name)
            {
                person.status = -1;
                printf("%c is waiting to eat...\n", person.name);
                while(1)
                {
                    while(person._y.is_available != 0);
                    if(person._y.is_available == 0)
                        break;
                }
                person._y.user = person.name;
                person._y.is_available = 0;
                person.chop_num++;
                person.status = 2;
                eat(person);
            }
        }
        if(person.chop_num == 0)
        {
            person.status = -1;
            printf("%c is waiting to eat...\n", person.name);
            while(1)
            {
                while(person._x.is_available != 0 && person._y.is_available != 0);
                if(person._x.is_available == 0 && person._y.is_available == 0)
                    break;
            }
            person._x.user = person.name;
            person._y.user = person.name;
            person.chop_num = 2;
            person.status = 2;
            eat(person);
        }
        return person.status;
    }
    else
    {
        return person.status;
    }
}

void *live(void *p)
{
    PLS *person = (PLS*)p;
    while(1)
    {
        think(*person);
        pick_chop(*person);
        eat(*person);
    }
}

int main()
{
    set();
    pthread_t p[NUM_THREADS];
    int ret;
    for(int i = 0; i < NUM_THREADS; i++)
    {
        printf("Creating thread %d\n", i);
        pthread_create(&p[i], NULL, live, (void*)&ph[i]);
    }
    pthread_exit(NULL);
    return 0;
}