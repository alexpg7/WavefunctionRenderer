# ⚛️ WavefunctionRenderer ![Static Badge](https://img.shields.io/badge/Original-blue?style=for-the-badge)

![Static Badge](https://img.shields.io/badge/C++-blue?style=flat&logo=cplusplus)
![Static Badge](https://img.shields.io/badge/Makefile-green?style=flat&logo=AnkerMake)
![Static Badge](https://img.shields.io/badge/status-completed-green?style=flat)

A simple library to visualize atomic wavefunctions in 3D.

# 📦 Installation

Firstly, you will need the SFML library for C++, you can install it from your terminal (Debian-based systems).

```bash
sudo apt update
sudo apt install libsfml-dev
```

This should install the ``2.6.1`` version (at least it did when i created the library, 28-05-2026).

> [!WARNING]
> Since ``apt`` installs the latest stable version, you should check it with:
> ```bash
> dpkg -l | grep sfml
> ```
> If you dont have the ``2.6.1`` version, you can try installing it from the original repository:
> ```bash
> git clone https://github.com/SFML/SFML.git
> cd SFML
> git checkout 2.6.1
> cmake .
> make
> sudo make install
> ```

Once you ensure that you have the ``2.6.1`` version, we can now start to install the ``WavefunctionRenderer`` lib (``libwfr``). Just clone this repository and execute the make command.

```bash
git clone https://github.com/alexpg7/WavefunctionRenderer.git WavefunctionRenderer && cd WavefunctionRenderer && make
```

It should output the ``libwfr.a`` file, which is a static library.

# 🛠️ Compilation

To use the library, you have to include the ``./inc/WavefuncrionRenderer.hpp`` header into your project and the ``libwfr.a`` in the location you prefer. To compile it, use the following command:

```bash
c++ yourmain.cpp -L. path/to/your/libwfr.a -lsfml-graphics -lsfml-window -lsfml-system -o yourmain.out
```

> [!TIP]
> You can place your ``libwfr.a`` in you project directory and use the linker version of the file:
> ```bash
> c++ yourmain.cpp -L. -lwfr -lsfml-graphics -lsfml-window -lsfml-system -o yourmain.out
> ```
>
> Even better, you can copy the library and header into your system directories and use it wherever you want:
> ```bash
> sudo cp libwfr.a /usr/local/lib/
> sudo cp -r inc /usr/local/include/
> ```

# 📔 Usage

## :atom: The ``WavefunctionRenderer`` class

This whole library centers on the ``WavefunctionRenderer`` class. To start visualizing wavefunctions, you must first instantiate a ``WavefucntionRenderer`` object:

```c++
WavefunctionRenderer  wave
```

Then, you configure your object by setting the different variables. At the end, use the ``.show()`` method to open a window with the visualization.

> [!WARNING]
> At least, you **must** set a function with the ``.setWaveFunction(const std::function<std::complex<float>(float, float, float)>& psi)`` method in order to start visualizing functions.

## 📘 The orbital template

If you want to use the library for the purpose it was built for, you may use the [atomic orbital](https://en.wikipedia.org/wiki/Atomic_orbital) functions. These come defined in the template  ``template<int n, int l, int m> std::complex<float> orbital(float x, float y, float z)`` from ``./inc/orbital.tpp`` and need to be prepared before compilation by defining the quantum numbers $n$, $l$ and $m$. As an example, for $n=2$, $l=1$ and $m=0$,

```c++
auto psi = orbital<2,1,0>;
```

Then, set the function with the ``.setWafeFunction()`` method:


```c++
auto psi = orbital<2,1,0>;
wave.setWaveFunction(psi);
```

Then, calling the ``.show()`` method should open a tiny window with the following look:

<img src="https://github.com/alexpg7/Images/blob/main/WavefunctionRenderer/wfrPreset.png" width="100">

For the moment, it does not look quite good, but we can improve it by modifying some settings with the following methods.

## ⚙️ Methods for settings

As we said, all the library centers on the ``WavefunctionRenderer`` class, so we may have to change the settings from there. Here is the list of all the settings you can control:

* ``void	setResolution(int W, int H);``
* ``void	setXResolution(int W);``
* ``void	setYResolution(int H);``

Set the resolution of the window (in px)

* ``void	setTitle(std::string title);``

Sets the title of the window (and the name of the visualization files that can be saved)

* ``void	setWaveFunction(const std::function<std::complex<float>(float, float, float)>& psi);``

Change the wavefunction that will be shown

* ``void	setGrid(unsigned int x);``

The visualization is calculated by evaluating the square modulus of the function ($|\psi(x,y,z)|^2$) in each point of the space and allocating it in a ``x``$\times $``x`` $\times $``x`` ``float`` array. Increasing the grid will enhance the quality of the visualization but will also increase (a lot) the memory used when showing the visualization. A recommended grid size is ``x = 256``.

<div><img src="https://github.com/alexpg7/Images/blob/main/WavefunctionRenderer/wfrGrid20.png" width="400"> <img src="https://github.com/alexpg7/Images/blob/main/WavefunctionRenderer/wfrGrid256.png" width="400"></div>

* ``void	setScale(float scale);``

This one changes the *zoom* of the visualization. Luckily, it can be changed in real time, as it will be explained in the 🎮 Controls section.

* ``void	setMode(Mode mode);``

Set the mode of visualization, changing it to ``{Mode::Density, Mode::Scattering, Mode::Surface}``, depending on your preference.
<div><img src="https://github.com/alexpg7/Images/blob/main/WavefunctionRenderer/wfrDensity.png" width="250"> <img src="https://github.com/alexpg7/Images/blob/main/WavefunctionRenderer/wfrScattering.png" width="250"><img src="https://github.com/alexpg7/Images/blob/main/WavefunctionRenderer/wfrSurface.png" width="250"></div>

* ``void	setIsosurface(float iso);``

Set the surface that will be shown in the ``Mode::Surface`` mode. It can be also changed in real time

* ``void	setIsosurfaceRate(float rate);``

Set the rate (in %) of change of the isosurface when we modify it with controls

* ``void	setColors(Color color1, Color color2);``
* ``void	setColor1(Color color1);``
* ``void	setColor2(Color color2);``

Set the 2 colors of the visualization. Color 1 is for high probability and Color 2 is for low. If you only want one color, just set both to the same one.

* ``void	setAngles(float theta, float phi);``
* ``void	setTheta(float theta);``
* ``void	setPhi(float phi);``

Set the angles of the camera in spherical coordinates. Thes can be also changed in real time

## 🎮 Controls

While the visualization window is open, you can press Ctrl+I to display the instructions in your terminal (it can also be done with the ``.instructions()`` method).

```output
WavefunctionRenderer instructions:
        -Mouse drag: Rotate perspective
        -Mouse scroll: Zoom in/out
        -Ctrl + mouse scroll: change isosurface by the fixed rate
        -Ctrl + S: Save stats in a .wfr file
        -Ctrl + P: Print instructions
        -Ctrl + I: Print stats
```

These do not need much further explanation, except for the saving one.

## 💾 Saving the visualization

As we said, you can save the state of your visualization (all the variables you customized) in a ``.wfr`` file. By pressing Ctrl+S, you save the current state with a timestamp name(in case you find an amazing perspective you want to keep). You can also save it with a custom name by calling ``.save(std::string filename)``.

Why do we want to save it? Because we can then load it with the ``.load(std::string filename)`` method, recovering the state of all the parameters saved (but not he wavefunction!!)
