# Celty Module API
To build a module for Celty is quite simple, first off, if you don't have a copy of the Celty sources, clone them to a directory where you will be working on the module.

## Initial Setup

```
$ git clone https://github.com/XAMPP/Celty.git celty
$ cd celty/modules
```

At this point, you should create a new directory with the name of your module and copy the `CMakeLists.txt` file from [modules/dummy](../modules/dummy) into your module directory and replace the name dummy with the name of your module.

```
$ mkdir mynewmodule
$ cp ./dummy/CMakeLists.txt ./mynewmodule/
$ cd mynewmodule
$ sed -i s/dummy/mynewmodule/g CMakeLists.txt
```

## Writing a Module
Now that you have the basic layout for creating a module, you can start writing code, for a starter, you can just copy the dummy template over.

```
$ cp ../dummy/dummy.cc mynewmodule.cc
```

Here is a listing of the dummy module code

```c++
/*! \file dummy.cc
	A test module
*/
#include <iostream>
#include <Module.hh>
using Celty::Module;

MODULE("DummyMod")

Module::Module(void) {
	std::cout << "[!] Dummy Module Loaded" << std::endl;
}

Module::~Module(void) {
	std::cout << "[!] Dummy Module Unloaded" << std::endl;
}
```

For the most part it is self explanatory, it prints out a message when it is loaded and unloaded, and it defines itself as a module with the `MODULE()` deceleration
