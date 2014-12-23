#!/usr/bin/env node
var G        = require('./server/global.js').g
  , peers    = require('./server/PeerLock.js').PeerManager(G.maxpeers) 
  , sockets  = require('./server/SocketHandler.js')
  , server   = require('./server/server.js').server
  , port     = G.favoritePort || G.backupPort;

console.log( '> starting server' )
console.log( '> open slots = ' + peers.AvailableCount() )
console.log( '> PORT::'+port )

server( sockets, peers, port )