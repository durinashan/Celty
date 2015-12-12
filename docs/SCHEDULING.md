# Celty Scheduling Modes
This document describes the general idea and motivation behind each of the scheduling algorithms.

## First-Come-First-Serve (FCFS)
The `FirstCome` setting for the scheduler is simple, requests are queued up and serviced in the order they are received. 

For example:

Client `A` sends a request, followed by client `B` and `C`, however `B` sends 2 requests before `C` can send a request, client `B` will then be serviced twice, prior to `C`.

## Favor Seeder
The `FavorSeed` setting is slightly more complicated, it is calculated thusly.
![Favor](http://i.imgur.com/1MjK0re.png)
Where `S` is the clients seed amount in megabytes, `n` is the number of clients and `i` is the index in the client list.

For Example:

There is a 1MB file, client `A` seeded 2MB, client `B` seeded 6MB, and client `C` seeded 1MB. The following favor is calculated for each client.

![Af](http://i.imgur.com/43Xd4t1.png)
![Bf](http://i.imgur.com/aVETGJs.png)
![Cf](http://i.imgur.com/3B0vbLG.png)

Thus, client `B` is favored when making requests over `A` and `C` due to the higher seed amount.

## Favor Leacher
The `FavorLeach` setting is the inverse of the Favor Seeder method, it uses the leach amount rather than the seed amount to calculate the favor of a client. 
![Favor](http://i.imgur.com/34ehC3C.png)
In this case `L` is the leached amount in megabytes'

## Round Robbin
The `RoundRobbin` setting keeps track of each of the clients that connect to the tracker for a set amount and then iterates through the list to see if the client as a request to service. This ensures that all clients get the same amount of time but is substantially slower and is outperformed by One of the Favor methods.

For Example:

There are 3 clients `A`, `B`, and `C`, once Celty has been made aware of them, each client will be serviced right after the other, even if there is no request from the client, so first `A`, then `B`, and `C`, followed by `A` again.

## Best Ratio
This is in the guise of `FavorSeed`, as it uses the calculated client ratio from the total combined UP and DOWN stats for each client, it then ranks the ratios in order of priority.

## Bogo Schedule
This scheduler is a random walk through a pooled client list, it is the least optimal and worst performing, it is included for parody. 
