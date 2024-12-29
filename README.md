# MMO PoC

### Auth Flow

In terms of auth flows, MMOs typically follow an authoritative gateway architecture.

1- The client connects to the gateway and performs a login.
2- The gateway generates a session token that is persisted temporarely in a distributed persistence layer
3- The gateway returns the token to the client
4- The client connects to the game server using the token provided
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
    "host": "127.0.0.1",
    "port": 6379
}
```


![alt text](doc/auth-flow.png)