# Bugs to fix

# Currently working on
- getting the overlay to be pixel not size dependant

# Next on the list
- Map class becomes basically a renderer composed of canvases
	- need to decide if real time layer rendering happens in own window or just in the map area
		- leaning towards own window right now 
- Nagivation using mouse not just keyboard
	- Preferably using imgui/io not glfw callbacks
	- Done with imgui io, actually much easier this way

# Further roadmap and feature ideas
- Encapsulate view and model objects to Application class