# Swarm
This module provides a shared backend for a Celty swarm using [memcached](http://memcached.org/) to share the status and information of all peers in the swarm rather than keeping the information locally.

This module requires `libmemcahced` to build.


## Example Usage
Swam can be used to set up a high-availability tracker solution as depicted below.

![tracker.academy.city](http://i.imgur.com/54LauIg.png)

In this example the load balancer is nginx using the [provided configuration](https://github.com/XAMPP/Celty/blob/master/etc/celty-proxy.conf)

