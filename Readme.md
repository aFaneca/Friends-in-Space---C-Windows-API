A Phoenix-like multiplayer game built upon C using the Windows API to its full extent.
Even though, because of time constraints, the game isn't fully developed, this build provides a very funcional - and basic - gameplay experience. Enough to allow anyone to play it from start to finish.

The game is composed of 3 different programs and multiple processes:
  - 1 Server - responsible for all the  logic of the game (communication server <-> gateway recurring to *Memory Mapped Files*)
  - 1 Gateway - the bridge between the gateway and the client 
  - N Clients - responsible for the user interface of the game (communication gateway <-> client using *Named Pipes*)
  
 
 #Screenshots
![Novo Jogo](http://afaneca.com/imagens/1.png)
