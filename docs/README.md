# Yokai Engine

[Download Engine](https://github.com/RubokiReuchi/Yokai_Engine/releases/tag/v1.0) -
[View Github](https://github.com/RubokiReuchi/Yokai_Engine).

## About Yokai Engine
Yokai Engine is a 3D game development engine made as part of an university assignment. The engine allows the user to create a scene with a GameObject system and is able to execute it's code in runtime.
The engine was make to be the most easy to be user for the users, simple but with all neaded funtionalities.

## About Ruboki Reuchi
Hi, Ruboki Reuchi is my username, but my real name is Rubén Ayora. I'm the developer of Yokai Engine, a project in which I have worked for 4 months.
# ![face](/docs/myface.png)

## Core Sub-Systems

- 3D Rendering
Render was done using OpenGL, in order to have a better performance the engine uses instancited meshes to avoid loding the same mesh more than one time.

- Camera
Making use of Frustrum culling the camera only show the gameobject that are in camera vision, this process was done using AABB boxes and discarting the gameobject that are not in camera vision.

- GameObjects
Create new gameobject is easy and fast as well as adding components to them. The engine uses a hierarchy system to make the scene creation easy for the user.

- Enviroment Customization
The user is able to move and dock all windows and the engine store the position of them in case of close the engine.

## Scripting System
The engine uses a Blueprint system for create the code, allowing to work in the engine without knowing any coding language. The Blueprint can be exported and imported from the pc folders.
Blueprints allow to modify any game data of the attached gameobject, in order to do that they can make use of values of other gameobjects.

![firstgift](/docs/gift1.gif)

As is show in the previus gift, we can quickly modify values to make the game alive.

![secondgift](/docs/gift2.gif)

And saw the result inmediatly working in the game.

## Video

[![Alt text](https://img.youtube.com/vi/jrvIhYVSiKk/0.jpg)](https://www.youtube.com/watch?v=jrvIhYVSiKk)

## Licence
MIT License

Copyright (c) 2022 RubokiReuchi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

theme: jekyll-theme-cayman
