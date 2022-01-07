<strong>3D Simple Bowling Game by using C++ OpenGL (including glfw libraries).</strong>

With the help of:
* http://www.songho.ca/opengl/gl_cylinder.html
* http://www.songho.ca/opengl/gl_sphere.html 

I could drew 10 cylinderical pins with specific height and radius in order to have the shape of a bowling pin.

Also, I could drew a bowling ball (a sphere).

When the user presses a mouse button, a different action happens:

* If the user puts the mouse cursor over the ball and the left mouse button is pressed, the bowling ball will be "launched" on the camera’s viewing direction (translation) and will have a rotation movement around its own center. When the ball intersects (collides with) at least one pin, all of them will be displayed "laying" on the ground (rotated).

![Alt Text](https://media.giphy.com/media/bAqUP4h3wC1mtssOUp/giphy.gif)


When the right mouse button is pressed, the ball is brought back to its initial position

![Alt Text](https://media.giphy.com/media/GCNSP7GrsRnCsGVcht/giphy.gif)

View the pins from an appropriate angle and translate the camera on all 3 axis.

![Alt Text](https://media.giphy.com/media/wGj3rB2J5ShOwLYlfb/giphy.gif)

