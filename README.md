# boid_lock
A program that allows you to lock screen 42 using a boid simulation. Written in C.

![Screencast_from_03-28-2023_03_35_31_PM_AdobeExpress](https://user-images.githubusercontent.com/101118957/228265623-bf24f377-48bb-4221-a5c5-bba0f557692a.gif)

Installation
To use it, clone the repository and change the path in lock.sh to be where the lock is located:
```
git clone https://github.com/DaadfroGG/boid_lock
```
Usage
To run boid_lock, simply execute the lock.sh script:
```
./lock.sh
```
This will start the boid simulation and lock your screen.

If you want to control the boids with the mouse, you can make and execute the resulting a.out file:
```
make
./a.out
```
The control schemes are as such:
```
Left click: adds a point
Mouse wheel up: deletes a point
Right click: quits the program
```
boid_lock was inspired by pimp_my_lock (https://github.com/jerem-ma/pimp_my_lock).
