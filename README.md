# ParkGL
> Project for the course **Computer Graphics** for 3rd year students on the  
> Computer Science studies at the Faculty of Mathematics, University of Belgrade.  
> Made with materials from [Learn OpenGL](https://learnopengl.com/)

---

# Build
> - ## Linux
>     - ### Prerequisites:  
>       ```
>       $ sudo apt-get install g++ cmake git build-essential libgl1-mesa-dev libsoil-dev libglm-dev libassimp-dev libglew-dev libglfw3-dev libxinerama-dev libxcursor-dev libxi-dev mesa-common-dev mesa-utils libxxf86vm-dev libfreetype6-dev
>       ```
>     - ### Clone:
>       ```
>       $ git clone https://github.com/VeljkoDeljanin/Computer-Graphics-Project.git
>       ```
>     - ### CLion:
>       - CLion -> Open -> path/to/my/project
>       - ALT+SHIFT+F10 -> ParkGL -> Run
>     - ### Manually:  
>       Position your terminal to path/to/my/project
>       ```
>       $ mkdir build && cd build
>       $ cmake ..
>       $ make
>       $ cd .. && ./ParkGL
>       ```

---

# Implemented topics
> - ## Mandatory
>     - From 1st to 7th week
>     - [Blending](https://learnopengl.com/Advanced-OpenGL/Blending)
>     - [Face culling](https://learnopengl.com/Advanced-OpenGL/Face-culling)
>     - [Advanced lighting](https://learnopengl.com/Advanced-Lighting/Advanced-Lighting)
> - ## Optional
>     - ### [Model](https://learnopengl.com/Model-Loading/Model)
>     - ### Group A
>         - [Framebuffers](https://learnopengl.com/Advanced-OpenGL/Framebuffers)
>         - [Cubemaps](https://learnopengl.com/Advanced-OpenGL/Cubemaps)
>         - [Instancing](https://learnopengl.com/Advanced-OpenGL/Instancing)
>         - [Anti Aliasing](https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing)
>     - ### Group B
>         - [Normal Mapping](https://learnopengl.com/Advanced-Lighting/Normal-Mapping), [Parallax Mapping](https://learnopengl.com/Advanced-Lighting/Parallax-Mapping)
>         - [HDR](https://learnopengl.com/Advanced-Lighting/HDR), [Bloom](https://learnopengl.com/Advanced-Lighting/Bloom)

---

# Key Bindings
> - ## Controls
>     - `WASD` - Camera movement
>     - `Mouse` - Look around
>     - `Mouse scroll` - Zoom
> - ## General
>     - `V` - VSync
>     - `←`/`→` - Decrease/Increase movement speed
>     - `C` - Mouse cursor
>     - `F1` - Settings
>     - `Esc` - Exit
> - ## Camera
>     - `N` - Fly camera
> - ## Lights
>     - `F` - Flashlight
>     - `L` - Lamp lights
>     - `K` - Billboard lights
> - ## Post-Processing
>     - `F2` - Anti-Aliasing
>     - `F3` - Grayscale
>     - `F4 - F8` - Kernel effects (Identity, Sharpen, Blur, Edge detection and Emboss)
>     - `H` - HDR
>     - `B` - Bloom
>     - `Q`/`E` - Decrease/Increase exposure
> - ## Normal and Parallax Mapping
>     - `O` - Normal Mapping
>     - `P` - Parallax Mapping
>     - `↓`/`↑` - Decrease/Increase height scale

---

# Assets
> Some models have modified materials and paths,  
> so it works directly with the way model loading is set up.
> - ## Models
>     - [Chess Set](https://polyhaven.com/a/chess_set)
>     - [Flashlight](https://sketchfab.com/3d-models/retro-style-flashlight-1bca09efb5154e75b6f6f17d8f71deeb)
>     - [Fountain](https://sketchfab.com/3d-models/fountain-1e2234e51a46400bbd3a71afd9c2b750)
>     - [Bench](https://sketchfab.com/3d-models/classic-park-bench-low-poly-01a5b64427984632bb44242da3813bb1)
>     - [Lamp](https://www.turbosquid.com/3d-models/standart-street-lamp-3d-model-1945402)
>     - [Table Chair Set](https://polyhaven.com/a/outdoor_table_chair_set_01)
>     - [Billboard](https://www.cgtrader.com/free-3d-models/exterior/exterior-public/a-billboard)
>     - [Telescope](https://sketchfab.com/3d-models/telescope-b03b8fc3a5e343bab43f3fa81c024ce5)
>     - [Tree](https://sketchfab.com/3d-models/pine-tree-e52769d653cd4e52a4acff3041961e65)
> - ## Textures
>     - [Cobblestone](https://polyhaven.com/a/cobblestone_floor_03)
>     - [Grass Path](https://polyhaven.com/a/grass_path_2)
>     - [Skybox](https://drive.google.com/file/d/1-Cw7GqwP9GBVznwGJq5tYne5GEmyeA_X/view)
>     - [Billboard](https://uramljivanjeslika.rs/digitalna-stampa-na-platnu/slike-beograda/)

---

# Course Details
> - Academic Year 2023/24
> - Assistant: [Marko Spasić](https://matf-racunarska-grafika.github.io/)
> - Professor: [Dr. Vesna Marinković](http://poincare.matf.bg.ac.rs/~vesna.marinkovic/grafika.html)

---


