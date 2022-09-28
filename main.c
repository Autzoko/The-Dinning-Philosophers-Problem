#include <stdio.h>
#include <stdlib.h>

typedef struct philosopher
{
    char name;
    int chop_num;
    int status; //1 for thinking, 0 for eating, -1 for waiting to eat, 2 for desire for eating
    CS _x;
    CS _y;
}PLS;

typedef struct chopstick
{
    char symbol;
    int is_available; // 0 for available, 1 for busy
    char user;
    
    // ab-1, bc-2, cd-3, de-4, ea-5
}CS;

CS _A, _B, _C, _D, _E;
PLS a, b, c, d, e;

void set()
{
    a.name = 'a';
    b.name = 'b';
    c.name = 'c';
    d.name = 'd';
    e.name = 'e';

    a._x = _A;
    a._y = _E;

    b._x = _A;
    b._y = _B;

    c._x = _B;
    c._y = _C;

    d._x = _C;
    d._y = _D;

    e._x = _D;
    e._y = _E;

    _A.symbol = '1';
    _B.symbol = '2';
    _C.symbol = '3';
    _D.symbol = '4';
    _E.symbol = '5';

    return;
}

static int all_chop = 5;
int count;

int think(PLS person)
{
    person.status = 1;
    printf("%c is thinking...\n", person.name);
    wait(10000);
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
    }
    if(person._y.is_available == 0)
    {
        person.chop_num++;
        person._y.is_available = 1;
        person._y.user = person.name;
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
            wait(10000);
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
        }
    }
}