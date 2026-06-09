#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_OBJECTS 100

char canvas[HEIGHT][WIDTH];

typedef enum
{
    LINE = 1,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct
{
    int id;
    ShapeType type;

    int x1, y1;
    int x2, y2;
    int x3, y3;

    int radius;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

/* Function Prototypes */
void clearCanvas();
void displayCanvas();
void plot(int x, int y);

void drawLine(int x1, int y1, int x2, int y2);
void drawRectangle(int x, int y, int width, int height);
void drawCircle(int xc, int yc, int r);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

void redrawPicture();
void renderObject(Object obj);

void addObject();
void deleteObject();
void modifyObject();

/* Clear Canvas */
void clearCanvas()
{
    int i, j;

    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

/* Display Canvas */
void displayCanvas()
{
    int i, j;

    printf("\n");

    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

/* Plot Point */
void plot(int x, int y)
{
    if(x >= 0 && x < WIDTH &&
       y >= 0 && y < HEIGHT)
    {
        canvas[y][x] = '*';
    }
}

/* Draw Line - Bresenham */
void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1)
    {
        plot(x1, y1);

        if(x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

/* Draw Rectangle */
void drawRectangle(int x, int y, int width, int height)
{
    drawLine(x, y, x + width, y);
    drawLine(x, y + height, x + width, y + height);

    drawLine(x, y, x, y + height);
    drawLine(x + width, y, x + width, y + height);
}

/* Draw Circle */
void drawCircle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while(y >= x)
    {
        plot(xc + x, yc + y);
        plot(xc - x, yc + y);

        plot(xc + x, yc - y);
        plot(xc - x, yc - y);

        plot(xc + y, yc + x);
        plot(xc - y, yc + x);

        plot(xc + y, yc - x);
        plot(xc - y, yc - x);

        x++;

        if(d > 0)
        {
            y--;
            d += 4 * (x - y) + 10;
        }
        else
        {
            d += 4 * x + 6;
        }
    }
}

/* Draw Triangle */
void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* Render One Object */
void renderObject(Object obj)
{
    switch(obj.type)
    {
        case LINE:
            drawLine(obj.x1, obj.y1,
                     obj.x2, obj.y2);
            break;

        case RECTANGLE:
            drawRectangle(obj.x1, obj.y1,
                          obj.x2, obj.y2);
            break;

        case CIRCLE:
            drawCircle(obj.x1, obj.y1,
                       obj.radius);
            break;

        case TRIANGLE:
            drawTriangle(obj.x1, obj.y1,
                         obj.x2, obj.y2,
                         obj.x3, obj.y3);
            break;
    }
}

/* Redraw All Objects */
void redrawPicture()
{
    int i;

    clearCanvas();

    for(i = 0; i < objectCount; i++)
    {
        renderObject(objects[i]);
    }
}

/* Add Object */
void addObject()
{
    Object obj;

    printf("\nObject ID: ");
    scanf("%d", &obj.id);

    printf("\n1. Line");
    printf("\n2. Rectangle");
    printf("\n3. Circle");
    printf("\n4. Triangle");

    printf("\nEnter Shape Type: ");
    scanf("%d", (int*)&obj.type);

    if(obj.type == LINE)
    {
        printf("Enter x1 y1 x2 y2: ");
        scanf("%d%d%d%d",
              &obj.x1,&obj.y1,
              &obj.x2,&obj.y2);
    }
    else if(obj.type == RECTANGLE)
    {
        printf("Enter x y width height: ");
        scanf("%d%d%d%d",
              &obj.x1,&obj.y1,
              &obj.x2,&obj.y2);
    }
    else if(obj.type == CIRCLE)
    {
        printf("Enter centerX centerY radius: ");
        scanf("%d%d%d",
              &obj.x1,&obj.y1,
              &obj.radius);
    }
    else if(obj.type == TRIANGLE)
    {
        printf("Enter x1 y1 x2 y2 x3 y3: ");
        scanf("%d%d%d%d%d%d",
              &obj.x1,&obj.y1,
              &obj.x2,&obj.y2,
              &obj.x3,&obj.y3);
    }
    else
    {
        printf("Invalid Shape!\n");
        return;
    }

    if(objectCount < MAX_OBJECTS)
    {
        objects[objectCount++] = obj;
        redrawPicture();
        printf("Object Added Successfully.\n");
    }
}

/* Delete Object */
void deleteObject()
{
    int id;
    int i, pos = -1;

    printf("Enter Object ID to Delete: ");
    scanf("%d", &id);

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].id == id)
        {
            pos = i;
            break;
        }
    }

    if(pos == -1)
    {
        printf("Object Not Found.\n");
        return;
    }

    for(i = pos; i < objectCount - 1; i++)
    {
        objects[i] = objects[i + 1];
    }

    objectCount--;

    redrawPicture();

    printf("Object Deleted Successfully.\n");
}

/* Modify Object */
void modifyObject()
{
    int id;
    int i;

    printf("Enter Object ID to Modify: ");
    scanf("%d", &id);

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].id == id)
        {
            printf("Enter New X1 Y1: ");
            scanf("%d%d",
                  &objects[i].x1,
                  &objects[i].y1);

            redrawPicture();

            printf("Object Modified Successfully.\n");
            return;
        }
    }

    printf("Object Not Found.\n");
}

/* Main */
int main()
{
    int choice;

    clearCanvas();

    while(1)
    {
        printf("\n============================");
        printf("\n 2D GRAPHICS EDITOR");
        printf("\n============================");

        printf("\n1. Add Object");
        printf("\n2. Delete Object");
        printf("\n3. Modify Object");
        printf("\n4. Display Picture");
        printf("\n5. Exit");

        printf("\n\nEnter Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addObject();
                break;

            case 2:
                deleteObject();
                break;

            case 3:
                modifyObject();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid Choice!\n");
        }
    }

    return 0;
}