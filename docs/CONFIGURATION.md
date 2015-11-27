# Celty Configuration
The Celty configuration uses a simple key

## List of Options
The following is a list of all of the configuration options.

##### AnnounceTime
	default: 25m
The amount of time that clients should wait before asking for a new peer list.
##### AnnounceTimeMin
	default: 25m
The absolute minimum amount of time before a client is allowed to ask for a new peer list.
##### APIEndpoint
	default: yes
Enables the Celty API endpoint.
##### APIListen
	default: 0.0.0.0
The address to listen on for API requests.
##### APIPort
	default: 2600
The port to listen on for API requests.
##### BlacklistEnabled
	default: no
Set if the blacklist is enabled or not.
##### Blacklist
	default: /etc/celty/blacklist.db
The path to the blacklist.
##### BlacklistConnector
	default: file
The connector to use for blacklist management.
##### DefaultPeerNum
	default: 100
The number of peers to return if a client never explicitly asks for a number.
##### ForgiveLeach
	default: no
Forgive peers that only leach and never seed.
##### HTTPEndpoint
	default: yes
Enables the HTTP endpoint.
##### HTTPListen
	default: 0.0.0.0
The address to listen for HTTP requests on.
##### HTTPMaxConnections
	default: 0
The maximum number of HTTP requests to service per worker, 0 for unlimited. 
##### HTTPPort
	default: 6881
The port to listen on for HTTP requests
##### HTTPReadTimeout
	default: 4s
The time that must pass on a read in order for it to timeout.
##### HTTPReqTimeout
	default: 4s
The time that must pass on a request in order for it to timeout.
##### HTTPWriteTimeout
	default: 4s
The time that must pass on a write in order for it to timeout.
##### InactiveRatio
	default: 2.0
The ratio that must be reached in order for a client to not be marked as Inactive.
##### InactiveRecycle
	default: no
Enables recycling inactive torrents, or torrents that have no seeds or leaches.
##### InactiveSeachInterval
	default: 1m
The interval in which we look for inactive clients.
##### Private
	default: yes
Set weather or not this tracker is private or not.
##### StatisticsEnable
	default: yes
Enables collecting statistics on torrents and clients.
##### StatisticsDeep
	default: yes
Enables more advanced statistics collection.
##### UDPEndpoint
	default: yes
Enables the UDP endpoint.
##### UDPListen
	default: 0.0.0.0
The address to listen on for UDP requests.
##### UDPPort
	default: 6881
The port to listen on for UDP requests
##### WhitelistEnabled
	default: no
Enables the whitelist.
##### Whitelist
	default: /etc/celty/whitelist.db
The path to the whitelist.
##### WhitelistConnector
	default: file
The connector used to manage the whitelist.
##### Workers
	default: 0
The number or worker threads to use for listening. 0 for one worker per CPU core.
