



# First Person Game demo/engine


This project uses the following as a base [project]: https://github.com/SOFT356/Samuel-Collis-SOFT356 . The base project was an open-gl model loader that allowed for the parsing and rendering of .obj and .dae file types.

The initial aims of this project was to create an engine that would allow for a basic explorer game with basic controls.  This branched into creating a custom image/ui element renderer. This allowed for the creation of a stamina bar to be created and rendered on screen which is tied to the players actions. To stem on this, a custom text renderer has also been wrote which is able to take the font outputs from Hiero @ https://github.com/libgdx/libgdx/wiki/Hiero  and use them to create render-able strings of text for within the game.
 
 A custom plane generator was also created to create the floor for the game. The rest of the game world was created through object files that have a personal use license, these can be found for the rocks here: https://free3d.com/3d-model/free-low-poly-rock-model-pack-131559.html and for the trees here: https://free3d.com/3d-model/christmas-tree-891764.html](https://free3d.com/3d-model/christmas-tree-891764.html .

Within the project there is a demo that utilises these features to create a basic explorer game.  When running the game you should see a window like so: 


![Game screenshot](https://imgur.com/aWgp8ae.jpg "In game window")




### Controls

Within the main method there are several controls that can be used to manipulate the loaded objects. They are:

- W,A,S,D for movement in the x and z dimensions
- Mouse movement for changing the direction that the player is facing
- Left CTRL - crouching
- Space - JUMP

## Using/Running the project

To run the project locally, you will need to clone the repo. After this, you will need to obtain a copy of the [glm library]([https://glm.g-truc.net/0.9.9/index.html](https://glm.g-truc.net/0.9.9/index.html)). You will then need to edit the configuration of the project. If you're using visual studio, this is done as so:
- right click the project
- Go to the 'VC++ directories' tab
- Add an additional entry under the include directories
- Make sure this new addition is pointing to the glm library

You will also need to make sure that the nuget packages for nupengl.core and nupengl.core.redist are downloaded.  

If everything is in place, you should then be able to build and run the project. If you are running this directly from the .exe file, a media folder must be placed inside the same directory with the same contents as found @ `\SOFT356 Project 2\media\`

# Modifying the demo

To modify the demo, there are quite a few ways this can be done. 

### Changing used font and custom text

This can be done using the the hiero tool as mentioned above to generate a new set of font files. For best results, it is best to use native rendering and generate a glyph cache. Then you can go to the media folder and rename the files to be 'font.fnt' and 'font.png', or you can go into the UIRenderer.cpp, at line 11 and change the files used to create the text.
Here are a few examples: 

Using impact font
![game screenshot](https://i.imgur.com/n3pC4O3.png "impact font")

Comic Sans MS: 
![game screenshot](https://i.imgur.com/McLkhVe.png "comic sans font")
As you can see in the comic sans screenshot, some fonts have better results. This is a limitation to the text renderer that I have created. It doesn't deal with text overhangs very well. For best results, a more square font should be used.

We are also able to change the text be changing the parameter used within the createText method found on line 12 of the same file.

![Custom text](https://i.imgur.com/pjMRtpW.png "Custom text")

### Custom images rendered

Within the UIRenderer.cpp file, you should be able to find a method called "createGameElements", within here we will be able to create new ui elements that will be rendered on the screen. We can create a custom elements to be shown. As the stamina bar shows a simple rectangle being created, we'll render some images in-game as an example.

``` c++
	UIelement element;
	//parameters as: postion, width, height, colour, texture location
	element.createRectangle(glm::vec2(0.6f, 1.0f), 0.4f, 0.4f, glm::vec4(0.29f, 0.0f, 1.0f, 0.5f), "media/test.png");

	this->uiElements.push_back(element);
```
This will create an element of size 0.4x0.4 with the texture of 'test.png'. 

After this modification we should be able to see our new item in game like so:
![game screenshot](https://i.imgur.com/yOdLMoS.png "custom image rendered")

It might not be useful to show full images, but we are also able to show transparent images. The next example would be implementing a "health bar" consisting of pictures of hearts.

``` c++
	UIelement heart;
	heart.setTransparent(true);
	for (int i = 0; i < 5; i++) {
		heart.createRectangle(glm::vec2(-1.0f + (i * 0.1f), -0.9f), 0.1f, 0.1f, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), "media/heart.png");
		this->uiElements.push_back(heart);
	}

``` 
This will create 5 heart ui elements to be shown on screen, this could easily be tied to a player health system. We have enabled transparency for these elements, meaning that the png files can be shown without a white background. 

![game screenshot](https://i.imgur.com/KiKQ3pO.png "custom image rendered")

### Custom ingame objects
Within the project there is a WorldGeneration.h file, this is responsible for generating the floor plane and loading the objects to be shown in the game world.

To change the texture or the size of the generated plane, then all we need to do is change the parameters used in the method found on line 11: 

``` 	Model floor = createTerrain(size, 2, "media/sand.png"); ```

So by changing the texture, I am able to change the floor from grass as seen in all the screenshots before to a floor of sand instead.

![game screenshot](https://i.imgur.com/8NQC9bP.png "custom floor rendered")

Below this, we also setup the object loader to load an object into the scene. For the demo, we load in a tree object and a rock object.  To set up and load a custom item, the following code is needed: 

```
Model custom = loadFromFile("media/custom.obj");
custom.location = glm::vec3(x, y, z);
models.push_back(custom);
```
This should then place the object in game, you may need to play with scaling and positioning depending on the object file.


A video for this project can be found below which outlines some areas of the code more specifically: 

[![Youtube video](http://img.youtube.com/vi/_QhinMTyjbI/0.jpg)](https://www.youtube.com/watch?v=_QhinMTyjbI "Video Title")


