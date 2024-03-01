# PhysicsEngine
A small 2D physics engine with collision detection and rigid body simulation. The collision resolver is checking for the overlap of shape edges, thus they can be abitrarily rotated. Currently supports only rectangles and triangles, but with the appropriate function to keep the vertices updated, any convex shape could be utilized.

## Dependencies and External Libraries:
* **SDL 2:**
	- x64 SDL headers and libraries used for creating windows, handling events, and rendering primitives
* **Visual Studio:**
	- project is set up to be developed with Microsoft Visual Studio