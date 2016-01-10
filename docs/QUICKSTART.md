# Celty Quickstart
**NOTE:** Although most of the commands listed here can just be copy-pasted it is advisable to not just willy nilly do so.


So you want to get up an running as fast as possible? Well this is the guide for you. There are two paths to take, the first is a full install, modules and all, the second is a bare install, that won't build or install any modules.

The two share some steps up until you need to make the decision, so first, here is the common process:

```
$ git clone https://github.com/XAMPP/Celty.git celty
$ mkdir celty-build
$ cd !$
```
Also ensure that you have `pthreads` and `libev` installed, we need them.

This will clone the repository, make a build directory and move you into it. Now comes the part where you decide what to build.

## Full
The full build is really easy:

```
$ cmake ../celty
$ make && sudo make install
```

That said, if you are building *all* of the modules you'll need some dependencies first.

 * `libpqxx3`
 * `libmemcached`

Once you have them, then you can do the step above.

## Standalone

When building standalone, we need to pass cmake an option to disable building the modules. other than that it's identical to the full install.

```
$ cmake ../celty -DBUILD_MODULES=OFF
$ make && sudo make install
```
Other than that, you're done!

## Done~
by default, Celty expects there to be a `celty` user that has read permissions to `/usr/local/` and write permissions to `/usr/local/etc/celty`. Celty will attempt to run as that user whenever it is daemonized.

To change the user, either rebuild Celty with a new default or if you are using `systemd` change the `User` field.

Once you have it built and installed, the modules, documentation, example configurations are all installed, as well as if your platform has a start script like `systemd` or the FreeBSD `rc.d` system that will also be installed.

So for any `systemd` platform, after the install you can just do the following:
```
$ systemctl enable celty
$ systemctl start celty
```

And you're up and running!
