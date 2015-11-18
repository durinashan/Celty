# Celty
Dead simple, extensible, unix BitTorrent tracker.

## Why Celty when there is `X`?
From what I have experience, all the other trackers, like [Chihaya](https://github.com/chihaya/chihaya) or [Ocelot](https://github.com/WhatCD/Ocelot) all have a plethora of dependencies, strange configuration, or just general lack of useful documentation. The attempt that Celty makes is to make this process dead simple. Celty is mostly self contained, and will work right out of the box.

## Dependencies
The only thing you need is a fairly modern C++ compiler, CMake for building, and libev for the event loop.

## Building
To build Celty, create the build directory, and run cmake from that. E.G:

```
$ git clone https://github.com/XAMPP/Celty.git celty
$ mkdir celty-build
$ cd !$
cd celty-build
$ cmake ../celty
$ make
```

## Running
To run Celty, just run the build executable, it should fork of to a headless process as a daemon, or you can prevent it from daemonizing by passing the `--keep-head` option

Once Celty has forked off, you still might want to get stats or reload the configuration, this can be done using the `--sig` flag. E.G
```
$ ./celty
[@] Celty daemonized to pid 12345
$ ./celty --sig status
[@] Running
$ ./celty --sig reload
[@] Reloading Configuration
$ ./celty --sig halt
[@] Stopping Celty....
```

## Configuring
TODO



![Celty](http://i.imgur.com/oKwH6Ho.gif)


# License
This project is licensed under the MIT Open Source License, see [LICENSE.md](LICENSE.md) for details.
