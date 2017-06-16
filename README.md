# glotter
`glotter` is a tool meant for visualizing graphs and algorithms operating on them for the purposes of debugging. Traditional debuggers such as `gdb` are usually not of much use when it comes to graphs. All they can do is display raw edge lists, which are not clear and intuitive. It gets only worse when we introduce more complicated algorithms such as network flow algorithms. `glotter` changes that by adding a GUI which displays the graph visually. You can also mark various things on the graph for your attention.

![Screenshot of example BFS debugging session](help/screen1.jpg "Screen of example BFS debugging session")

## Usage

### Basic example

#### Writing code

Full code is available in [test/bfs/main.cpp](test/bfs/main.cpp) file.

Since these algorithms are complicated and the result is supposed to look nice, `glotter` needs more information about code than basic debuggers want. However, the API is designed so that work and LoC required are as small as possible, allowing for quick debugging.

What you need to do is:
```cpp
// Include the header
#include <glotter.hpp>

// Create an object
Glotter glotter;

	// Tell glotter how many vertices the graph will have
	glotter.resize(vertexCount);

	// When adding edges, also tell glotter about them
	glotter.addEdge2(firstVertex, secondVertex);

	// Then, you can change color of edges/vertices as you like
	glotter.setVertexColor(verterxNumber, "#ADD8E6");
	glotter.setEdge2Color(firstVertex, secondVertex, "#888888");

	// Glotter will pause the execution after any change automatically
```

#### Compiling

You will need the following flags:
- `-std=c++14` I like C++14.

Also, the following libraries:
- `-pthread` Web server needs this for threading. Doesn't need installation.
- `-lssl` Web server needs this for encryption stuff. Available in Ubuntu package `libssl-dev`.
- `-luWS` A web server. Needs to be cloned and compiled from [Github](https://github.com/uNetworking/uWebSockets).
- `-lz` Web server needs this for theading stuff. Available in Ubuntu package `zlib1g-dev`.

See [Makefile](Makefile) for an example.

#### Running

First, run your compiled app just as you would normally run it. If you are following the BFS test, example input is provided in [test/bfs/input.txt](test/bfs/input.txt) file. Run your favourite browser and load `localhost:57077`.

#### Using

In right top corner, you will see a "Next" button which is used just like `next` in `gdb`. `glotter` automatically pauses program execution after every visual change. In order to resume in, click the button.

Under the control bar, your graph should be displayed. You can drag nodes around, zoom with mouse scroll, and move the view via dragging the background.

In left top corner, you can see the status bar. It will tell you if everything is working correctly(it should be green with a "Ready" text).

## ~~FA~~Q

**Can this be used simultaneously with a normal debugger?**
Yes, just run it in `gdb`. This is a graph visualization tool, not a complete debugger.

**Can it be used without Internet connection?**
Yes. Browser is required, but all resources are available locally.