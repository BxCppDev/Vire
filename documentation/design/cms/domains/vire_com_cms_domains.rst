==========================
Vire Communication Domains
==========================

:Author: J.Hommet, F.Mauger
:Date: 2018-08-30

Description of the Vire Communication Domains.

.. contents::

Definitions
===========

A  Vire communication  domain is  an abstract  space of  communication
between actors participating to the  Vire control and monitoring (CMS)
system.   A domain  hosts  a  given set  of  *mailboxes* with  special
permissions granted to different categories of actors.

A domain  uses a communication  technology implemented by  a dedicated
*transport  system*  (example:   *RabbitMQ*).   Messages  are  encoded
through   a   dedicated   *encoding  system*   (example:   *protobuf*,
*JSON*...).  All mailboxes hosted by a domain share the same messaging
technology.    However,   different    domains   may   use   different
communication technologies (RabbitMQ vs HTTP, protobuf vs JSON...).

When implemented through the RabbitMQ  system, a domain corresponds to
a RabbitMQ *virtual  host* and its mailboxes  correspond to *exchange*
objects.


Categories of actors
--------------------

Several *categories*  of actors are  supported in Vire and  defined in
different Vire applications:

* Server application (symbol: ``s``):
  
  * ``server-gate``: Vire Server actor  responsible of the client gate
    management   (one    unique   instance    within   a    Vire   CMS
    framework). There should be only one  user of this category in the
    server application.
  * ``server-cms``: Vire Server actor responsible of the CMS operation
    (access to  *Control* and  *Monitoring* domains). There  should be
    only one user of this category in the server appliation.
  * ``server-client-system``: Vire Server  actor dedicated to *system*
    communications with a given client.  There should be as many users
    of this category  in the server application as  there are connected
    Vire clients.
  * ``server-subcontractor-system``:  Vire Server  actor dedicated  to
    *system* communications  with a given subcontractor.  There should
    be as  many users of  this category  in the server  application as
    there are connected Vire subcontractors.

* Subcontractor application (symbol: ``b``):
    
  * ``subcontractor``  : Subcontractor  actors to  which the  Vire CMS
    delegates the  access to  specific resources,  this actor  is used
    both for  *system* and  CMS operations. There  should be  only one
    user of this category in the subcontractor application.

* Client application (symbol: ``c``):

  * ``client-gate``  :  Vire  Client   actor  used  to  established  a
    connected session with the Vire  server through the *Gate* device.
    There  should   be  only  one   user  of  this  category   in  the
    subcontractor application.
  * ``client-system``  :  Vire  Client  actor  dedicated  to  *system*
    communications with the Vire Server, once the connected session is
    established. There should be only one user of this category in the
    subcontractor application.
  * ``client-cms``  :  Vire  Client   actor  responsible  of  the  CMS
    operation    (access   to    the   *Monitoring*    and   *Control*
    domains). There  should be only one  user of this category  in the
    subcontractor application.
    
Each  actors may  simultaneously benefit  of connections  to different
domains and access to different mailboxes within domains, depending on
granted permissions.

Mailboxes
---------

A mailbox  is an  entity to  which actors  can publish  messages. Each
mailbox has  an unique  name within  the domain it  belongs to  and an
*address* with respect to the communication system.

A mailbox has one of the following *privacy* types:

* ``public`` : A  public |publicmailbox| mailbox can  be sent messages
  from many producer  actors.  Some consumer actors  must subscribe to
  the  mailbox to  pickup  posted messages.   This  corresponds to  an
  *exchange* in RabbitMQ.
* ``private``  :  A  private  |privatemailbox|  mailbox  can  be  sent
  messages typically from one producer actor.  Only one consumer actor
  can pickup posted messages.  This  corresponds to a direct exclusive
  *queue*  in RabbitMQ.  In principle,  Vire does  not expose  private
  mailboxes.

.. |publicmailbox| image::  images/vire_domain_public_mailbox.png
		   :width: 72px

.. |privatemailbox| image::  images/vire_domain_private_mailbox.png
		   :width: 72px

A mailbox uses one of the following working modes:

* ``service``  :  Actors can  publish  some  request messages  to  the
  mailbox with response messages  expected (within a reasonable delay)
  from the target actor (typically used for RPC transactions).
* ``event``  : Actor  can publish  some asynchronous  messages to  the
  mailbox, typically used for logging, alarm, Pub/Sub and other signals.


When  implemented through  the RabbitMQ  system, a  mailbox in  a Vire
domain corresponds to:

- a RabbitMQ *exchange* if it is *public* and identified through
  a public conventional name.
- a  RabbitMQ *exclusive  queue*  if it  is  *private* and  identified
  internally from a private name (not visible in the Vire API).



Vire CMS domains
----------------

The Vire CMS system typically  implements 5 usecases. Domain names are
formatted  following the  naming  scheme of  the  Vire library.   Each
domain name is prefixed with  the *experiment identifier*.  Client and
subcontractor *system* domain names have the clients or subcontrators'
identifiers as  a suffix.   In the cases  below, structural  names for
public  mailboxes are  colored in  blue. Domain  names are  colored in
magenta  and here  they  correspond  to a  specific  example with  the
"/supernemo/demonstrator"    experiment   ID    and   the    "cmslapp"
subcontractor ID.


**Vire clients gate domain:**

  This is  the domain used by  Vire clients to request  connections to
  the  Vire   CMS  from  the   Vire  server  *before*  a   session  is
  started. Only (not  connected) Vire clients and the  Vire server can
  communicate through this domain.

  .. image:: images/vire_domain_clients_gate_general.png
     :width: 80%
     :align: center
     :alt: The Vire clients *Gate* domain

**Vire client system domain:**

  Each client which benefits of a  *running* session with the Vire CMS
  is  able to  communicate with  the Vire  server through  a dedicated
  private domain of this type. There  is one Vire client system domain
  per connected client.  Only one Vire client and the  Vire server can
  communicate through this domain.

  .. image:: images/vire_domain_client_system.png
     :width: 80%
     :align: center
     :alt: The Vire client *System* private domain

**Vire control domain:**

  This  is  the domain  dedicated  to  the communication  of  resource
  *control* request  messages. Only Vire  clients and the  Vire server
  can communicate through this domain.

  .. image:: images/vire_domain_control.png
     :width: 80%
     :align: center
     :alt: The Vire *Control* domain

**Vire monitoring domain:**

  This  is  the domain  dedicated  to  the communication  of  resource
  *monitoring*  request  messages.  This   domain  is  also  used  for
  transfering logging  messages, alarm messages and  Pub/Sub messages.
  All categories of actors can use this domain.

  .. image:: images/vire_domain_monitoring.png
     :width: 100%
     :align: center
     :alt: The Vire *Monitoring* domain

**Subcontractor system domain:**

  Each subcontractor to which the Vire Server collaborate with is able
  to  communicate with  the Vire  server through  a dedicated  private
  domain of  this type. There  is one subcontractor system  domain per
  connected subcontractor. Only one  subcontractor and the Vire server
  can communicate through this domain.

  .. image:: images/vire_domain_subcontractor_system_pub.png
     :width: 80%
     :align: center
     :alt: The Vire Subcontractor *System* private domain

.. The end.
