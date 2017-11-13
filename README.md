# Tetris Game
An implementation of the [Tetris](https://en.wikipedia.org/wiki/Tetris) game in C++ with [CImage](http://cimg.eu/) graphics library. Use arrow keys to move the game pieces, space bar to rotate them, and P to pause the game!

I implemented this game more than 10 years ago when I was in the second semester of my undergraduate studies! Time flies! :)

### How to compile?
#### Windows
Microsoft Visual Studio.NET and Visual Express Edition: Use the project files and solution files provided in the CImg Library package (directory 'compilation/') to see how it works (see CImg library documentation).

####Linux
```
sudo apt-get install libx11-dev
g++ -o tetris.o main.cpp tetris.cpp -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
```

####Mac OS
```
g++ -o tetris.o main.cpp tetris.cpp -O2 -lm -lpthread -I/usr/X11R6/include -L/usr/X11R6/lib -lm -lpthread -lX11
```

### Enjoy!
You can also use the pre-built Windows and Linux binaries found in the `bin` folder.

![alt text](/img/screenshot.png?raw=true "Tetris screenshot")
