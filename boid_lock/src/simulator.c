#include "../include/boids.h"
#define _NET_WM_STATE_REMOVE 0 // remove/unset property
#define _NET_WM_STATE_ADD 1    // add/set property
#define _NET_WM_STATE_TOGGLE 2 // toggle property
#define _NET_WM_USER_TIME 0
#define _NET_WM_DESKTOP 1
#define _NET_WM_FULLSCREEN_MONITORS 2
#define _NET_WM_WINDOW_TYPE 3

// Initialization:
void	init_boids(t_boid *boids, int screen_width, int screen_height)
{
	for (int i = 0; i < numBoids; i++)
	{
		boids[i].x = rand() % (screen_width + 1);
		boids[i].y = rand() % (screen_height + 1);
		boids[i].vx = 0;
		boids[i].vy = 0;
		boids[i].count = 2;
		if (rand() % 10 < 6)
			boids[i].species = 0;
		else if (rand() % 2)
			boids[i].species = 1;
		else
			boids[i].species = 2;
	}
}

// Window management:
void	MakeAlwaysOnTop(Display *display, Window root, Window mywin)
{
	Atom	wmStateAbove;
	Atom	wmNetWmState;
		XClientMessageEvent xclient;

	wmStateAbove = XInternAtom(display, "_NET_WM_STATE_ABOVE", 1);
	if (wmStateAbove != None)
	{
		printf("_NET_WM_STATE_ABOVE has atom of %ld\n", (long)wmStateAbove);
	}
	else
	{
		printf("ERROR: cannot find atom for _NET_WM_STATE_ABOVE !\n");
		return ;
	}
	wmNetWmState = XInternAtom(display, "_NET_WM_STATE", 1);
	if (wmNetWmState != None)
	{
		printf("_NET_WM_STATE has atom of %ld\n", (long)wmNetWmState);
	}
	else
	{
		printf("ERROR: cannot find atom for _NET_WM_STATE !\n");
		return ;
	}
	// set window always on top hint
	if (wmStateAbove != None)
	{
		memset(&xclient, 0, sizeof(xclient));
		xclient.type = ClientMessage;
		xclient.window = mywin;              // GDK_WINDOW_XID(window);
		xclient.message_type = wmNetWmState;
			//gdk_x11_get_xatom_by_name_for_display( display, "_NET_WM_STATE" );
		xclient.format = 32;
		xclient.data.l[0] = _NET_WM_STATE_ADD;
			// add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
		xclient.data.l[1] = wmStateAbove;     
			//gdk_x11_atom_to_xatom_for_display (display, state1);
		xclient.data.l[2] = 0;                
			//gdk_x11_atom_to_xatom_for_display (display, state2);
		xclient.data.l[3] = 0;
		xclient.data.l[4] = 0;
		XSendEvent(display,
					root,
					False,
					SubstructureRedirectMask | SubstructureNotifyMask,
					(XEvent *)&xclient);
		XFlush(display);
		return ;
	}
	return ;
}

void	window_hole_punch(Display *disp, Window win, int screen_height,
		int screen_width)
{
	Region		region;
	XRectangle	frame[4];

	region = XCreateRegion();
	frame[Top] = CLITERAL(XRectangle){0, 0, 540, 30};
	XUnionRectWithRegion(&frame[Top], region, region);
	frame[Left] = CLITERAL(XRectangle){0, 0, 180, 540};
	XUnionRectWithRegion(&frame[Left], region, region);
	frame[Right] = CLITERAL(XRectangle){540, 0, screen_width, screen_height};
	XUnionRectWithRegion(&frame[Right], region, region);
	frame[Down] = CLITERAL(XRectangle){0, 160, screen_width, screen_height};
	XUnionRectWithRegion(&frame[Down], region, region);
	XShapeCombineRegion(disp, win, ShapeBounding, 0, 0, region, ShapeSet);
	XMapWindow(disp, win);
	XClearArea(disp, win, 0, 0, 200, 200, True);
}

void	auto_pointer(int *initial_x, int *initial_y, double j, int screen_width,
		int screen_height)
{
	double	a;
	double	b;

	a = 600;
	b = 800;
	*initial_x = (screen_width / 2) + (a * sqrt(2) * cos(j)) / (1 + sin(j)
			* sin(j));
	*initial_y = (screen_height / 2) + (b * sqrt(2) * cos(j) * sin(j)) / (1
			+ sin(j) * sin(j));
}

// Movement:
void	update_boids(t_boid *boids, int screen_width, int screen_height)
{
	int	overshot;

	for (int i = 0; i < numBoids; i++)
	{
		if (boids[i].species == 1)
		{
			boids[i].vx *= 0.65;
			boids[i].vy *= 0.65;
		}
		else if (boids[i].species == 0)
		{
			boids[i].vx *= 0.8;
			boids[i].vy *= 0.8;
		}
		else
		{
			boids[i].vx *= 0.7;
			boids[i].vy *= 0.7;
		}
		boids[i].x += boids[i].vx;
		boids[i].y += boids[i].vy;
		//COLLISION WITH WALL TELEPORTS
		if (boids[i].x > screen_width)
		{
			overshot = boids[i].x - screen_width;
			boids[i].x = overshot;
			boids[i].y = screen_height - boids[i].y;
		}
		if (boids[i].y > screen_height)
		{
			overshot = boids[i].y - screen_height;
			boids[i].y = overshot;
			boids[i].x = screen_width - boids[i].x;
		}
		if (boids[i].x < 0)
		{
			overshot = abs(boids[i].x);
			boids[i].x = screen_width - overshot;
			boids[i].y = screen_height - boids[i].y;
		}
		if (boids[i].y < 0)
		{
			overshot = abs(boids[i].y);
			boids[i].y = screen_height - overshot;
			boids[i].x = screen_width - boids[i].x;
		}
	}
}

double	randomDouble(double min, double max)
{
	double	range;
	double	randomValue;

	range = max - min;
	randomValue = (double)rand() / RAND_MAX;
	return (min + (randomValue * range));
}

void	addRandomnessToMovement(t_boid *boids, double maxRandomVelocity)
{
	double	randomVelocityX;
	double	randomVelocityY;

	for (int i = 0; i < numBoids; i++)
	{
		randomVelocityX = randomDouble(-2, 2);
		randomVelocityY = randomDouble(-2, 2);
		boids[i].vx += randomVelocityX;
		boids[i].vy += randomVelocityY;
	}
}

void	moveTowardsFlockCenter(t_boid *boids, double maxSpeed)
{
	double	centerX;
	double	centerY;
	double	angle;
	double	vx;
	double	vy;
	double	speed;

	centerX = 0;
	centerY = 0;
	for (int i = 0; i < numBoids; i++)
	{
		centerX += boids[i].x;
		centerY += boids[i].y;
	}
	centerX /= numBoids;
	centerY /= numBoids;
	for (int i = 0; i < numBoids; i++)
	{
		angle = atan2(centerY - boids[i].y, centerX - boids[i].x);
		vx = maxSpeed * cos(angle);
		vy = maxSpeed * sin(angle);
		speed = sqrt(boids[i].vx * boids[i].vx + boids[i].vy * boids[i].vy);
		if (speed + maxSpeed < speed)
		{
			boids[i].vx += vx;
			boids[i].vy += vy;
		}
	}
}

void	followNearbyBoids(t_boid *boids, double perceptionRadius,
		double maxSpeed)
{
	double	avgVx;
	double	avgVy;
	int		nearbyBoids;
	double	distance;
	double	angle;
	double	vx;
	double	vy;

	for (int i = 0; i < numBoids; i++)
	{
		avgVx = 0;
		avgVy = 0;
		nearbyBoids = 0;
		for (int j = 0; j < numBoids; j++)
		{
			if (i != j)
			{
				distance = sqrt((boids[i].x - boids[j].x) * (boids[i].x
							- boids[j].x) + (boids[i].y - boids[j].y)
						* (boids[i].y - boids[j].y));
				if (distance <= perceptionRadius
					&& boids[j].species != boids[i].species)
				{
					avgVx += boids[j].vx;
					avgVy += boids[j].vy;
					nearbyBoids++;
				}
			}
		}
		if (nearbyBoids > 0)
		{
			avgVx /= nearbyBoids;
			avgVy /= nearbyBoids;
			angle = atan2(avgVy, avgVx);
			vx = maxSpeed * cos(angle);
			vy = maxSpeed * sin(angle);
			boids[i].vx += vx;
			boids[i].vy += vy;
		}
	}
}

void	followOtherBoids(t_boid *boids, double maxSpeed)
{
	int i, j, count;
	double centerX, centerY, angle, vx, vy, speed;
	for (i = 0; i < numBoids; i++)
	{
		count = 0;
		centerX = 0;
		centerY = 0;
		for (j = 0; j < numBoids; j++)
		{
			if (boids[j].species != 3)
			{
				centerX += boids[j].x;
				centerY += boids[j].y;
				count++;
			}
		}
		if (count)
		{
			centerX /= count;
			centerY /= count;
			angle = atan2(centerY - boids[i].y, centerX - boids[i].x);
			vx = maxSpeed * cos(angle);
			vy = maxSpeed * sin(angle);
			speed = sqrt(boids[i].vx * boids[i].vx + boids[i].vy * boids[i].vy);
			if (speed + maxSpeed < speed)
			{
				boids[i].vx += vx;
				boids[i].vy += vy;
			}
		}
	}
}

void	moveTowardsMouse(t_boid *boids, Vector2 mousepos, double maxSpeed)
{
	int		i;
	double	distance;

	i = 0;
	while (i < numBoids)
	{
		if (boids[i].species != 3)
		{
			double angle = atan2(mousepos.y - boids[i].y,
									mousepos.x - boids[i].x);
			distance = sqrt(pow(boids[i].x - mousepos.x, 2) + pow(boids[i].y
						- mousepos.y, 2));
			boids[i].vx += (distance / maxSpeed) * cos(angle);
			boids[i].vy += (distance / maxSpeed) * sin(angle);
		}
		i++;
	}
}

// Graphics:
unsigned int	hsv_to_rgb(int h, float s, float v)
{
	int		h_i;
	float	f;
	float	p;
	float	q;
	float	t;
	int		red;
	int		green;
	int		blue;

	h_i = h / 60;
	f = h / 60.0f - h_i;
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	float r, g, b;
	switch (h_i)
	{
	case 0:
		r = v;
		g = t;
		b = p;
		break ;
	case 1:
		r = q;
		g = v;
		b = p;
		break ;
	case 2:
		r = p;
		g = v;
		b = t;
		break ;
	case 3:
		r = p;
		g = q;
		b = v;
		break ;
	case 4:
		r = t;
		g = p;
		b = v;
		break ;
	case 5:
		r = v;
		g = p;
		b = q;
		break ;
	default:
		r = 0;
		g = 0;
		b = 0;
		break ;
	}
	red = (int)(r * 255);
	green = (int)(g * 255);
	blue = (int)(b * 255);
	return ((red << 16) + (green << 8) + blue);
}

void	update_color(t_boid boid, int *hex_color, int j)
{
	int	speed;
	int	hue;

	speed = (int)sqrt(boid.vx * boid.vx + boid.vy * boid.vy);
	double hue_speed = (speed * 1.4) + j;
		// adjust this value to control the hue change rate
	hue = (int)hue_speed % 360;
	int hex_res = hsv_to_rgb(hue, 1, 1); // convert hue value to RGB
	*hex_color = hex_res;
}

void	draw_boid(t_boid boid, Display *disp, Window win, GC gc, int screen,
		int j)
{
	int	radius;
	int	hex_color;

	radius = 5;
	hex_color = 0xffff00;
	update_color(boid, &hex_color, j);
	XSetForeground(disp, gc, hex_color);
	XFillArc(disp, win, gc, boid.x - radius, boid.y - radius, radius * 2, radius
			* 2, 0, 360 * 64);
}

void	draw_grav(Display *disp, Window win, GC gc, Vector2 grav)
{
	int	radius;

	radius = 20;
	XFillArc(disp, win, gc, grav.x - radius, grav.y - radius, radius * 2, radius
			* 2, 0, 360 * 64);
}

void	display_stuff(t_boid boid, Display *disp, Window win, GC gc, int screen,
		Vector2 tmp_mousepos, Vector2 initial, Vector2 scnd_grav, int j)
{
	draw_boid(boid, disp, win, gc, screen, j);
	if (tmp_mousepos.x && tmp_mousepos.y)
		draw_grav(disp, win, gc, initial);
	if (scnd_grav.x && scnd_grav.y)
		draw_grav(disp, win, gc, scnd_grav);
}

// Main loop:

int	main_loop_boid(Display *disp, Window win, int screen_width,
		int screen_height, int screen)
{
	t_boid boids[numBoids];
	init_boids(boids, screen_width, screen_height);
	GC gc = XCreateGC(disp, win, 0, NULL);
	GC gc_c = XCreateGC(disp, win, 0, NULL);
	XSetForeground(disp, gc_c, BlackPixel(disp, screen));
	int initial_x;
	int initial_y;
	XEvent ev;
	XSelectInput(disp, win,
			ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
	int pointer_x, pointer_y;
	unsigned int mask;
	XGrabPointer(disp, win, False, PointerMotionMask | ButtonPressMask,
			GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	int i = 0;
	double j = 0;
	Pixmap buffer;
	GC bufferGC;
	int tmp_mouse_x;
	int tmp_mouse_y;
	int scnd_grav_x;
	int scnd_grav_y;
	double gradient = 0;
	while (1)
	{
		srand(time(NULL));
		if (i == 60)
		{
			XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, XK_End), True, 0);
			XTestFakeKeyEvent(disp, XKeysymToKeycode(disp, XK_End), False, 0);
			i = 0;
		}
		buffer = XCreatePixmap(disp, win, screen_width, screen_height,
				DefaultDepth(disp, screen));
		bufferGC = XCreateGC(disp, buffer, 0, NULL);
		XSetForeground(disp, bufferGC, 0x0000112e);
		XFillRectangle(disp, buffer, bufferGC, 0, 0, screen_width,
				screen_height);
		XNextEvent(disp, &ev);
		if (XCheckTypedEvent(disp, ButtonPress, &ev)
			&& ev.xbutton.button == Button3)
			break ;
		if (XCheckTypedEvent(disp, MotionNotify, &ev))
		{
			if (ev.type == MotionNotify)
			{
				if (!XQueryPointer(disp, win, &ev.xbutton.root,
						&ev.xbutton.window, &pointer_x, &pointer_y,
						&tmp_mouse_x, &tmp_mouse_y, &mask))
				{
				}
				else
				{
					XUngrabPointer(disp, CurrentTime);
				}
			}
		}

		if (!tmp_mouse_x && !tmp_mouse_y)
		{
			auto_pointer(&initial_x, &initial_y, j, screen_width,
					screen_height);
			if (j == 360)
				j = 0;
		}
		else
		{
			initial_x = tmp_mouse_x;
			initial_y = tmp_mouse_y;
		}
		j += 0.01;
		if (XCheckTypedEvent(disp, ButtonPress | ButtonRelease, &ev)
			&& ev.xbutton.button == Button1)
		{
			scnd_grav_x = initial_x;
			scnd_grav_y = initial_y;
		}
		if (XCheckTypedEvent(disp, ButtonPress, &ev)
			&& ev.xbutton.button == Button5 || ev.xbutton.button == Button4)
		{
			scnd_grav_x = 0;
			scnd_grav_y = 0;
		}
		for (int i = 0; i < numBoids; i++)
		{
			XSetForeground(disp, bufferGC, WhitePixel(disp,
						DefaultScreen(disp)));
			display_stuff(boids[i], disp, buffer, bufferGC, screen,
					CLITERAL(Vector2){tmp_mouse_x, tmp_mouse_y},
					CLITERAL(Vector2){initial_x, initial_y},
					CLITERAL(Vector2){scnd_grav_x, scnd_grav_y}, gradient);
		}
		gradient += 2;
		if (gradient > 360)
			gradient = 0;
		int radius = 10;
		XFillArc(disp, win, gc, initial_x - radius, initial_y - radius, radius
				* 2, radius * 2, 0, 360 * 64);
		usleep(20000);
		moveTowardsMouse(boids, CLITERAL(Vector2){initial_x, initial_y}, 100);
		if (scnd_grav_x && scnd_grav_y)
			moveTowardsMouse(boids, CLITERAL(Vector2){scnd_grav_x, scnd_grav_y},
					50);
		followNearbyBoids(boids, 50, 10);
		moveTowardsFlockCenter(boids, 10);
		addRandomnessToMovement(boids, 10);
		followOtherBoids(boids, 100);
		update_boids(boids, screen_width, screen_height);
		i++;
		XCopyArea(disp, buffer, win, gc_c, 0, 0, screen_width, screen_height, 0,
				0);
		XFreePixmap(disp, buffer);
		XFreeGC(disp, bufferGC);
		XFlush(disp);
	}
	XUngrabPointer(disp, CurrentTime);
	XFlush(disp);
}