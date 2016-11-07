# cautious-robot

Getting GUI's to work with current versions of OpenGL and GLFW we're taking way too much time, so I just created a bunch of callbacks in order to change values without recompiling. 

`space`: toggle pause

If paused, you can left-click and drag the bead to move its position on the wire. This resets it's (angular) velocity to 0. If right clicked and dragged, you'll see a circle come up that shows the magnititude of the velocity you'll set; up means positive and down means negative. 

If you hold down `g` and scroll, you can increase and decrease gravitational force. 

If you hold down `m` and scroll, you can increase and decrease the bead's mass. 

Pressing `o` should output in the console a brief summary of the bead's current state.
