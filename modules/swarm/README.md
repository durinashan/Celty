# Swarm
This module provides a shared backend for a Celty swarm using [memcached](http://memcached.org/) to share the status and information of all peers in the swarm rather than keeping the information locally.

This module requires `libmemcahced` to build.

## Configuration Options
The swarm module expects the following values to be set:

##### SwarmMemcachedHost
	default: none
This sets the host of the memcached instance to sync to.

##### SwarmMemcachedPort
	default: 11211
The port that memcached is running on.

##### SwarmSyncInterval
	default: 4s
The interval in which the swarm module will sync with the cache.

## Example Usage
Swam can be used to set up a high-availability tracker solution as depicted below.

![tracker.academy.city](http://i.imgur.com/54LauIg.png)

In this example the load balancer is nginx using the [provided configuration](https://github.com/XAMPP/Celty/blob/master/etc/celty-proxy.conf)

The above diagram illustrates a fictional tracker, `tracker.academy.city`. It is a fairly large tracker, and as such it has some needs that have to be addressed, the first of which is high-availability, seeing as no single system can handle the throughput that their users demand, they have a load balancer set up to keep thins spread evenly across multiple instances of Celty. The issue comes to be is how do all of the instances share data? If they have no way to do that then what's the point? Using the `swarm` module it is possible to pool multiple Celty instances running on different machines on the same network, or even across the Internet to be synchronized to act as one massive tracker.
