# MMO PoC

This project attempts to be a PoC/research tool to fiddle and understand MMO architecture and implementation and to be a good excuse to try C++20 (If I ever configure the f** g++ settings for Github Codespaces...).

### THE FUTURE...

What I expect from this project? absolutely nothing...

I just want to learn more and apply some hardcore c++ shennanigans for fun and hopefully to impress girls.

Any relevant stuff I learn might or might not end being a post in my blog... Who knows https://prdeving.wordpress.com

### Functionalities

NONE...

for now (29th Dec 2024) I'm mostly focused in idiomatic abstractions, multithreading and networking in C++.
In terms of functionallities, the project implements a simple handshake mechanism that allows clients to login in game servers, the first step towards WoW2.

### Pieces

- redis (docker-compose.yml)

Redis is used as a distributed session persistence layer so game servers can validate users.

- game-server (C++)

It's the main game piece, gonna handle the game flow and it's the connection point for the clients.
In the future I plan to introduce world-servers to implement seamless zone swapping but that gonna have to wait.

- gateway-server (NodeJS, dockerized)

For now, a simple implementation to mock the session auth service, it's going to handle logins and game-server balancing until world servers are implemented

- client (C++)

Simple game client that connects to the game server

### Running the project

There's no simple way to run everything at once yet.

**Build the project**

```
mkdir build
cd build
cmake ..
make
```

**Install NPM dependencies for the gateway sevice**

```
cd gateway-server
npm i
```

Run everything, bear in mind the order!!

**Run redis**

```
docker compose up redis
```

**Run the gateway**

```
docker compose up gateway-server
```

**Run the other stuff**
```
./build/bin/game-server
./build/bin/client
```

you should have an output like this, showing the login and handshake procedure between client and game server:

```
/workspaces/MMO/build (main) $ ./bin/game-server 
ENet: Initialized.
Hiredis: Connected to Redis server
ENet: Host created.
Client 3493870355 connected.
Hiredis: GET k: 9e86cace6928ebc3c7517e42a311f8d08a09a54baee9e5b45b5e4d324459759f, v: 04f8996da763b7a969b1028ee3007569eaf3a635486ddab211d512c85b9df8fb
Client 3493870355 logged in as 04f8996da763b7a969b1028ee3007569eaf3a635486ddab211d512c85b9df8fb.
ENet: Deinitialized.
Hiredis: Disconnected from Redis server
```

```
/workspaces/MMO/build (main) $ ./bin/client 
cUrl: Initialized libcurl.
cUrl: Created cUrl instance.
ENet: Initialized.
ENet: Connecting to peer.
Received token: 9e86cace6928ebc3c7517e42a311f8d08a09a54baee9e5b45b5e4d324459759f
 from gateway
Connected.
Synced with user 04f8996da763b7a969b1028ee3007569eaf3a635486ddab211d512c85b9df8fb
Disconnected.
ENet: Deinitialized.
cUrl: Cleaned up cUrl instance.
cUrl: Cleaned up libcurl.
```

### Directories

**Misc**
```
+ doc (documentation and diagrams gonna be here)
```

**Tech stuff**
```
+ external (third party libs like sqlite or hiredis)
+ include (comon hpp for other packages)
    + infra (holds infra abstractions for different services)
+ libs (custom libs like networking or persistence abstractions)
```

**Applications**
```
+ game-server (game server app)
+ gateway-server (gateway app)
+ client (client app)
```

### Auth Flow

In terms of auth flow, this project follow what is a typicall authoritative gateway architecture, pretty standard in MMOs.

1- The client connects to the gateway and performs a login with credentials.

2- The gateway generates a session token that is persisted temporarely in a distributed persistence layer

3- The gateway returns the token to the client along the designated game server information

4- The client connects to the game server using the token and information provided

5- The game server checks against the persistence layer if the token is valid


In this project the gateway is written as a simple nodeJS HTTP server with a single `/login` endpoint and a single hardcoded user with the following credentials:

```
{
    "username": "user",
    "password": "password"
}
```

The login endpoint stores a session in redis for the game server to auth the connection and returns the session token and the game server address and port to the client so it can connect:

```
{
    "session": "fb7af8663063f55cc6f37643fdbc35980595ebe4383695a085645f7631396eaf",
    "host": "localhost",
    "port": 1234
}
```


![alt text](doc/auth-flow.png)

### Changelog

- initial prototype for game input handling
- game logic library and instantiation
- message handlers
- event-based net abstraction
- lockless queue implementations
- threaded networking
- add gateway to docker-compose (why not...)
- repository pattern to inter-service connections
- get rid of unneded Socket abstraction
- fixed some memory leaks
- implemented login call from client to gateway so auth flow is complete
- changed std:cout to printf for easy formatting
- implemented cUrl implementation for HTTP ops
- implemented HTTP wrapper