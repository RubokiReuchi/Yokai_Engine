# Yokai Engine
https://github.com/RubokiReuchi/Yokai_Engine

- A Engine 3D that allow create an eviroment using external meshes and textures. Continues in develop.

## Controls:
### Window Hierarchy
- Create new Game Object button --> Add new GameObject to the scene, only basic shapes and empty game object at the moment.
- Left Click in hierarchy tree --> chose the selected game object.
- Eye Icon --> hide the game object with all his children (only in editor, not in game).
- G --> open selected game object options: unparent, create empty child and destroy game object.
### Window Scene
- Right Click and Drag --> rotate camera.
- Right Click + W --> move camera to front.
- Right Click + A --> move camera to right.
- Right Click + S --> move camera to left.
- Right Click + D --> move camera to back.
- Right Click + Q --> move camera to down.
- Right Click + E--> move camera to up.
- Right Click + mouse wheel --> increase/decrease camera movement speed.
- Middle Click and Drag --> pan camera.
- Right Click and Drag + Left Alt --> orbit arround selected game object.
- Mouse wheel up and down --> zoom.
- F --> Go to selected game object.
- Mouse confine in screen when drag action happening.
- Q --> hide transform guizmo.
- W --> show translate guizmo.
- E --> in camera game object show only Y-rotation guizmo, in other game object show rotation guizmo.
- R --> in camera game object show only Z-rotation guizmo, in other game object show scale guizmo.
- Left Click in mesh --> select parent game object of clicked mesh.
- DOUBLE Left Click in mesh --> select clicked mesh.
### Window Inspector
- Change GameObject name.
- Set visibility of game objects and their children.
- Add component with filter (only c_camera yet).
- Edit and display component propieties.
- Delete component.
#### Camera Component
- display the game camera view.
- Allow to edit camera FOV.
- Allow to edit camera far plane distance.
#### Tranform Component
- Display and edit position, rotation and scale.
#### Mesh Renderer Component
- Display visibility of the gameObject.
- Allow to turn on/off AABB display.
- Allow to turn on/off OBB display.
#### Material Component
- Display texture path.
- Allow to change the texture from the default one, the texture checker one and the textures loaded.
- Display preview of selected texture, if is not default or checkers.
### Window Performance
- Set FPS limit.
- Set window size.
- Toggle Vsync.
### Window Project
- Back Arrow if inside folder --> leave the folder.
- Filter to find file in Assets.
#### Left Box
- DOUBLE Left Click in folder --> open folder in Right Box.
#### Right Box
- Left Click in folder --> open folder in Right Box.
- DOUBLE Left Click in file --> open file with desired application.
### Window Console
- Clean Logs
- Save Logs

## Drag and Drop
### External Drag
When a file is dropped in the engine window a copy is done in the current folder(of the project window).
### Internal Drag
To drag files in scene, drag it from project window.
- Mesh type --> the mesh is creted in scene origin with default texture.
- Texture type --> (first time)texture is loaded in memory, (after load texture)the texture is applied to the game object with the mouse above it.

## Serialization
- New Scene --> create a defult scene(only a camera) and close current scene without save it(careful).
- Load Scene --> select a scene previusly saved and loads it.
- Save Scene --> save current scene.
- Save Scene As --> save current scene as a new one or override one actualy saved.

## CHECK tab (only create for show motor funtions in second deliver)
- Frustrum culling --> enabled, makes the game camera to display only the items in scene camera frustrum. Move scene camera to watch the changes in game camera.
- Log loaded meshes --> log in console all loaded meshes, if someone is loaded twice it should log both. (motor never loadad more than once a meshs).
- Log loaded textures --> log in console all loaded textures, if someone is loaded twice it should log both. (motor never loadad more than once a texture).

## Extra information
- if a asset is removed or modified from Assets folder, the correspondent custom file an his metadata are deleted.
- custom files and metadata are generated the first time an asset is loaded.
- the metadata file are hidden in floders, make sure to make them visible in folder settings. They are created in the same folder of his correspondent custom file.

## Members:
- Rubén Ayora Alcañiz: https://github.com/RubokiReuchi



by Rubén Ayora

theme: cayman
