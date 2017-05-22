==============================
Communication service
==============================

I/O plug
============

The ``Com`` service consists in a  collection of I/O plugs.  Each plug
has an unique  name/identifier within the service it  belongs to.  The
service  acts as  a  factory of  plugs.  Any  client  may request  the
creation,  usage or  destruction  of an  arbitrary  plug through  this
service..

There are four different types of plugs:

* ``event_emitter_plug``  :  It  contains one  dedicated  output  port
  connected to some  networked target where to  publish event messages
  to the outside world.
* ``event_listener_plug``  :  It  contains one  dedicated  input  port
  connected  to  some  message  queue  and  possibly  binded  to  some
  networked source of event messages from the outside world.
* ``server_blocking_plug`` :  It contains a  pair of ports  (one input
  port and one output port) and  is typically used for synchronous RPC
  calls (request/response) initiated by remote client.
* ``client_blocking_plug`` :  It contains a  pair of ports  (one output
  port and one input port) and  is typically used for synchronous RPC
  calls (request/response) emitted to remote server(s).
