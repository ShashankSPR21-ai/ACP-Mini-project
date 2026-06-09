#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 40
#define COLS 80
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef enum
{
    LINE,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct
{
    int id;
    ShapeType type;

    union
    {
        struct
        {
            int x1, y1, x2, y2;
        } line;

        struct
        {
            int x, y, width, height;
        } rect;

        struct
        {
            int xc, yc, radius;
        } circle;

        struct
        {
            int x1, y1, x2, y2, x3, y3;
        } triangle;
    };

} Shape;

Shape objects[MAX_OBJECTS];
int objectCount = 0;
int nextID = 1;

/* ---------------- Canvas Functions ---------------- */

void clearCanvas()
{
    int i, j;

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

void setPixel(int x, int y)
{
    if(x >= 0 && x < COLS && y >= 0 && y < ROWS)
    {
        canvas[y][x] = '*';
    }
}

void displayCanvas()
{
    int i, j;

    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

/* ---------------- Drawing Functions ---------------- */

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1)
    {
        setPixel(x1, y1);

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

void drawRectangle(int x, int y, int width, int height)
{
    drawLine(x, y, x + width, y);
    drawLine(x, y, x, y + height);
    drawLine(x + width, y, x + width, y + height);
    drawLine(x, y + height, x + width, y + height);
}

void drawCircle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 1 - r;

    while(x <= y)
    {
        setPixel(xc + x, yc + y);
        setPixel(xc - x, yc + y);
        setPixel(xc + x, yc - y);
        setPixel(xc - x, yc - y);

        setPixel(xc + y, yc + x);
        setPixel(xc - y, yc + x);
        setPixel(xc + y, yc - x);
        setPixel(xc - y, yc - x);

        if(d < 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }

        x++;
    }
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* ---------------- Object Management ---------------- */

void redrawCanvas()
{
    int i;

    clearCanvas();

    for(i = 0; i < objectCount; i++)
    {
        Shape *s = &objects[i];

        switch(s->type)
        {
            case LINE:
                drawLine(
                    s->line.x1,
                    s->line.y1,
                    s->line.x2,
                    s->line.y2
                );
                break;

            case RECTANGLE:
                drawRectangle(
                    s->rect.x,
                    s->rect.y,
                    s->rect.width,
                    s->rect.height
                );
                break;

            case CIRCLE:
                drawCircle(
                    s->circle.xc,
                    s->circle.yc,
                    s->circle.radius
                );
                break;

            case TRIANGLE:
                drawTriangle(
                    s->triangle.x1,
                    s->triangle.y1,
                    s->triangle.x2,
                    s->triangle.y2,
                    s->triangle.x3,
                    s->triangle.y3
                );
                break;
        }
    }
}

void addShape()
{
    Shape s;
    int choice;

    printf("\n1.Line\n2.Rectangle\n3.Circle\n4.Triangle\n");
    printf("Choose Shape: ");
    scanf("%d", &choice);

    s.id = nextID++;

    switch(choice)
    {
        case 1:
            s.type = LINE;

            printf("Enter x1 y1 x2 y2: ");
            scanf("%d%d%d%d",
                  &s.line.x1,
                  &s.line.y1,
                  &s.line.x2,
                  &s.line.y2);
            break;

        case 2:
            s.type = RECTANGLE;

            printf("Enter x y width height: ");
            scanf("%d%d%d%d",
                  &s.rect.x,
                  &s.rect.y,
                  &s.rect.width,
                  &s.rect.height);
            break;

        case 3:
            s.type = CIRCLE;

            printf("Enter centerX centerY radius: ");
            scanf("%d%d%d",
                  &s.circle.xc,
                  &s.circle.yc,
                  &s.circle.radius);
            break;

        case 4:
            s.type = TRIANGLE;

            printf("Enter x1 y1 x2 y2 x3 y3: ");
            scanf("%d%d%d%d%d%d",
                  &s.triangle.x1,
                  &s.triangle.y1,
                  &s.triangle.x2,
                  &s.triangle.y2,
                  &s.triangle.x3,
                  &s.triangle.y3);
            break;

        default:
            printf("Invalid Choice!\n");
            return;
    }

    objects[objectCount++] = s;

    redrawCanvas();

    printf("Shape Added! ID = %d\n", s.id);
}

void deleteShape()
{
    int id;
    int i, j;

    printf("Enter Shape ID to delete: ");
    scanf("%d", &id);

    for(i = 0; i < objectCount; i++)
    {
        if(objects[i].id == id)
        {
            for(j = i; j < objectCount - 1; j++)
            {
                objects[j] = objects[j + 1];
            }

            objectCount--;

            redrawCanvas();

            printf("Deleted Successfully!\n");
            return;
        }
    }

    printf("ID Not Found!\n");
}

void listShapes()
{
    int i;

    printf("\nStored Shapes:\n");

    for(i = 0; i < objectCount; i++)
    {
        printf("ID = %d  Type = ", objects[i].id);

        switch(objects[i].type)
        {
            case LINE:
                printf("LINE");
                break;

            case RECTANGLE:
                printf("RECTANGLE");
                break;

            case CIRCLE:
                printf("CIRCLE");
                break;

            case TRIANGLE:
                printf("TRIANGLE");
                break;
        }

        printf("\n");
    }
}

/* ---------------- Main ---------------- */

int main()
{
    int choice;

    clearCanvas();

    while(1)
    {
        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Shape\n");
        printf("2. Delete Shape\n");
        printf("3. Display Picture\n");
        printf("4. List Shapes\n");
        printf("5. Exit\n");

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                addShape();
                break;

            case 2:
                deleteShape();
                break;

            case 3:
                displayCanvas();
                break;

            case 4:
                listShapes();
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