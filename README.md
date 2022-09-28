## The Dining Philosophers Problem

### Basic methods
Create struct "philosopher" and "chopstick" to manage critical resources. Using keys to figure out whether those resources are available.

### To run program
Run file by using instruction:

```
bash ./run.sh
```

or could type the instructions by your own.
```
gcc main.c -lpthread -o main.o
./main.o
```

### Problems to fix
- Have problems about using multiple threads. 
- Always pick up "chopstick 0" when running function "pick_chop()".
- Have problems in entering function "eat()".
- Conflicts in critical area.