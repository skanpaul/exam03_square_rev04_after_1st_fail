#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERROR 1 
#define ERR_END_OF_FILE 0 
#define ERR_ARG         1          
#define ERR_FILE_CRPT   2

#define OUT_OF_SHAPE    0
#define IN_SHAPE        1
#define ON_EDGE         2

#define TXT_ERR_ARG         "Error: argument\n"
#define TXT_ERR_FILE_CRPT   "Error: Operation file corrupted\n"
// ----------------------------------------------------------------
typedef struct s_square
{
    char t;
    float x;
    float y;
    float w;
    float h;
    char c;
} t_square;

// ----------------------------------------------------------------
typedef struct s_zone
{
    int w;
    int h;
    char bg;
    int total;
} t_zone;

// ----------------------------------------------------------------
t_zone z;
t_square s;
char *map;

// ----------------------------------------------------------------
int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
        i++;    
    return i;
}

// ----------------------------------------------------------------
int message_error(FILE *ptr_file, int err_type)
{   
    if (err_type == ERR_FILE_CRPT)
    {
        write(1, TXT_ERR_FILE_CRPT, ft_strlen(TXT_ERR_FILE_CRPT));  
        if  (ptr_file)
            fclose(ptr_file);      
        return (1);
    }
    else if (err_type == ERR_ARG)
	{
        write(1, TXT_ERR_ARG, ft_strlen(TXT_ERR_ARG));
	}
    else
    {
        for (int y = 0; y < z.h; y++)
        {
            write(1, &map[y * z.w], z.w);
            write(1, "\n", 1);
        }
    }
    if (ptr_file)
        fclose(ptr_file);
    return err_type;
}

// ----------------------------------------------------------------
int pixel_status(float x, float y, t_square *s)
{
    if (x < s->x || x > (s->x + s->w) || y < s->y || y > (s->y + s->h))
        return OUT_OF_SHAPE;
    else if (x - s->x < 1 || (s->x + s->w) - x < 1 || y - s->y < 1 || (s->y + s->h) - y < 1)
        return ON_EDGE;
    return IN_SHAPE;
}

// ----------------------------------------------------------------
int main(int argc, char **argv)
{
    FILE *ptr_file;
    int res;
	int in_out_on;


    ptr_file = NULL;

    if(argc != 2)
        return(message_error(ptr_file, ERR_ARG));

    if ((ptr_file = fopen(argv[1], "r")) == NULL)
        return (message_error(ptr_file, ERR_FILE_CRPT));

    if (fscanf(ptr_file, "%d %d %c", &z.w, &z.h, &z.bg) != 3)
        return (message_error(ptr_file, ERR_FILE_CRPT));

    if (z.w <= 0 || z.w > 300 || z.h <= 0 || z.h > 300)    
        return (message_error(ptr_file, ERR_FILE_CRPT));

    z.total = z.w * z.h;
    map = malloc(z.total * sizeof(char));
    memset(map, z.bg, z.total);

    while (1)
    {
        res = fscanf(ptr_file, "\n%c %f %f %f %f %c", &s.t, &s.x, &s.y, &s.w, &s.h, &s.c);
		if (res == -1)
			return (message_error(ptr_file, ERR_END_OF_FILE));
		else if (res != 6 || s.w <= 0 || s.h <= 0 || (s.t != 'r' && s.t != 'R'))
			return (message_error(ptr_file, ERR_FILE_CRPT));			
		
		for (int y = 0; y < z.h; y++)
		{
			for (int x = 0; x < z.w; x++)
			{
				in_out_on = pixel_status(x, y, &s);
				if (s.t == 'r' && in_out_on == ON_EDGE)
					map[x + y * z.w] = s.c;
				if (s.t == 'R' && in_out_on)
					map[x + y * z.w] = s.c;
			}
		}
    }    
}