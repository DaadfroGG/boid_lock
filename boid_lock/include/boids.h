#ifndef BOIDS_H
# define BOIDS_H
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# define Font XFont
# include "../include/raylib.h"
# undef Font
# include <GLFW/glfw3.h>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
# include <X11/extensions/XTest.h>
# include <X11/extensions/shape.h>
# include <math.h>
# include <unistd.h>

# define numBoids 1000

typedef struct s_boid
{
	int			x;
	int			y;
	double		vx;
	double		vy;
	int			count;
	int			species;
}				t_boid;

typedef struct s_control_vars
{
	double		mouseAttraction;
	int			wheel;
	Vector2		mousepos;
	Color		pointerColor;
	double		random_move;
}				t_control_vars;

typedef struct s_grav_point
{
	int			speed;
	int			type;
	Color		color;
}				t_grav;

// Initialization:
void			init_boids(t_boid *boids, int screen_width, int screen_height);
// Movement:
void			update_boids(t_boid *boids, int screen_width,
					int screen_height);
double			randomDouble(double min, double max);
void			addRandomnessToMovement(t_boid *boids,
					double maxRandomVelocity);
void			moveTowardsFlockCenter(t_boid *boids, double maxSpeed);
void			followNearbyBoids(t_boid *boids, double perceptionRadius,
					double maxSpeed);
void			followOtherBoids(t_boid *boids, double maxSpeed);
void			moveTowardsMouse(t_boid *boids, Vector2 mousepos,
					double maxSpeed);
// Graphics:
unsigned int	hsv_to_rgb(int h, float s, float v);
void			update_color(t_boid boid, int *hex_color, int j);
void			draw_boid(t_boid boid, Display *disp, Window win, GC gc,
					int screen, int j);
void			draw_grav(Display *disp, Window win, GC gc, Vector2 grav);
void			display_stuff(t_boid boid, Display *disp, Window win, GC gc,
					int screen, Vector2 tmp_mousepos, Vector2 initial,
					Vector2 scnd_grav, int j);
// Window management:
enum			e_frame
{
	Top,
	Left,
	Right,
	Down
};
void			MakeAlwaysOnTop(Display *display, Window root, Window mywin);
void			window_hole_punch(Display *disp, Window win, int screen_height,
					int screen_width);
void			auto_pointer(int *initial_x, int *initial_y, double j,
					int screen_width, int screen_height);
// Main loop:
int				main_loop_boid(Display *disp, Window win, int screen_width,
					int screen_height, int screen);
#endif