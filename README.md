# Yokai Engine
https://github.com/RubokiReuchi/Yokai_Engine

- A Engine 3D that allow to display meshes with textures. Continues in develop.

## Controls:
### Window Hierarchy
- Create new Game Object button --> Add new GameObject to the scene, only basic shapes at the moment.
- Left Click in hierarchy tree --> chose the selected game object.
- Eye Icon --> hide the game object with all his children (only in editor, not in game).
### Window Scene
- Right Click and Drag --> rotate camera.
- Right Click + W --> move camera to front.
- Right Click + A --> move camera to right.
- Right Click + S --> move camera to left.
- Right Click + D --> move camera to back.
- Right Click + Q --> move camera to down.
- Right Click + E--> move camera to up.
- Left Shift --> Toggle camera movement speed.
- Middle Click and Drag --> pan camera.
- Right Click and Drag + Left Alt --> orbit arround selected game object.
- Mouse wheel up and down --> zoom.
- F --> Go to selected game object.
- Muse confine in screen when drag action happening.
### Window Inspector
- Change GameObject name.
- Set visibility of game objects and their children.
- Add component with filter (only c_camera yet).
- Edit and display component propieties.
- Delete component.
#### Camera Component
- display the game camera view.
#### Tranform Component
- Display and edit position, rotation and scale.
#### Mesh Renderer Component
- Display texture path.
- Allow to change the texture from the default one, the texture checker one and the textures loaded.
- Display visibility of the gameObject.
### Window Performance
- Set FPS limit.
- Set window size.
- Toggle Vsync.
### Window Project
- Filter to find file in Assets.
- Open folders in tree.
- In tree folders Doble Right Click --> open folder.
- Open flders in square format.
### Window Console
- Clean Logs

## Drag and Drop
### External Drag
When a file is dropped in the engine window a copy is done in the current folder(of the project window).
### Internal Drag
To drag files in scene, drag it from project window.
- Mesh type --> the mesh is creted in scene origin with default texture.
- Texture type --> the texture is loaded and applied to the selected gameObject(no to the children).

## Members:
- Rubén Ayora Alcañiz: https://github.com/RubokiReuchi



by Rubén Ayora
