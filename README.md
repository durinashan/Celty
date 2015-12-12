# Celty [![Build Status](https://travis-ci.org/XAMPP/Celty.svg?branch=master)](https://travis-ci.org/XAMPP/Celty)
Dead simple, extensible, unix BitTorrent tracker.

**NOTICE:** This project is still in development, it's not quite working yet.

## Why Celty when there is `X`?
From what I have experienced, all the other trackers, like [Chihaya](https://github.com/chihaya/chihaya) or [Ocelot](https://github.com/WhatCD/Ocelot) all have a plethora of dependencies, strange configuration, or just general lack of useful documentation. The attempt that Celty makes is to make this process dead simple. Celty is mostly self contained, and will work right out of the box.

## Dependencies
The only thing you need is a fairly modern C++ compiler (clang >= 3.6 or GCC >= 5.1), CMake for building, and libev for the event loop.

## Building
**ATTN:** Read [Quick Start](https://github.com/XAMPP/Celty/blob/master/docs/QUICKSTART.md) to get up and running ASAP.

To build Celty, create the build directory, and run cmake from that. e.g:

```
$ git clone https://github.com/XAMPP/Celty.git celty
$ mkdir celty-build
$ cd !$
cd celty-build
$ cmake ../celty
$ make && sudo make install
```

That will build and install celty to your system, if you want to remove it, then you can do the following:
```
$ cd celty-build
$ sudo make uninstall
```

For the most part the default options will be fine, but if you want to change anything, here is a list of available build options.

##### `CMAKE_INSTALL_PREFIX`
	type: path
	default "/usr/local"

##### `DAEMON_NAME`
	type: string
	default: "celty"

The name of the daemon, it will be used on all the logs and other messages

##### `DEFAULT_LOCKDIR`
	type: path
	default: "/var/lock/subsys"

The location of the lock file for the daemonized process

##### `DEFAULT_LOCKFILE`
	type: string
	default: "celty.lwk"

The name of the lock file to use

##### `DEFAULT_MODULEDIR`
	type: string
	default: "$(CMAKE_INSTALL_PREFIX)/lib/celty"

The location where celty looks for modules.

##### `DEFAULT_MODULEEXT`
	type: string
	default: ".moe"

The extension of the module to load.

**NOTE:** The `.` in the extension name is optional, in that case Celty will look for any filed that is suffixed with a matching string.

**NOTE:** The any module following the standard build method in the `[modules](./modules)` directory will automatically be built with the given extension.

##### `BUILD_MODULES`
	type: boolean
	default: yes

Builds the modules along with Celty.

**NOTE:** Each module should have a `BUILD_*NAME*_MODULE` that allows you to control each module being built.

**NOTE:** Some modules might have explicit dependencies and will fail to build, check each modules readme for a list of dependencies.

##### `DEFAULT_RUNAS`
	type: string
	default "nobody"

The user to run the daemon as.

##### `DEFAULT_WORKINGDIR`
	type: string
	default: "/"

The working directory to use when running as a daemon.

##### `CONFIG_DIRECTORY`
	type: path
	default: "$(CMAKE_INSTALL_PREFIX)/etc"

The default directory that Celty will look for configuration files in.

##### `CONFIG_FILE`
	type: string
	default: "celty.cfg"

The default name of the configuration file that Celty will look for.

##### `PID_FILE`
	type: string
	default: "/tmp/celty.pid"

The location the Celty daemon writes a PID file for signaling.

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
The configuration is stored in a simple key-value list that allows the simple setting of options and values. For more information on this as well as documentation on all of the settings, see [CONFIGURATION.md](https://github.com/XAMPP/Celty/blob/master/docs/CONFIGURATION.md)

## Modules
Celty for the most part is all self contained, but if you want to extend this functionality then modules are what you want.

### For users
Modules are libraries that allow to Celty to do more than what comes out of the box.

If you want to install a module all you need to do is drop the module in the configured module directory and restart Celty. Or alternatively issue the `--sig reload` command.

### For module developers
The module API is documented under [MODULES.md](https://github.com/XAMPP/Celty/blob/master/docs/MODULES.md)


![Celty](http://i.imgur.com/oKwH6Ho.gif)

# License
This project is licensed under the MIT Open Source License, see [LICENSE.md](https://github.com/XAMPP/Celty/blob/master/LICENSE.md) for details.

