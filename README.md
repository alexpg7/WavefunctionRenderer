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

## 🛠️ Compilation

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
