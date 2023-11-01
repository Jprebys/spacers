#include <cstdio>

#include "window.h"
#include "entity.h"


int main(void)
{
    printf("Hello, world!\n");

    spacers::Window window{"Spacers", 800, 600};
    Cube cube;

    printf("cube %f\n", cube.vertices[0].x);


    return 0;
}