
=============================================
Vire Communication Domain Mailbox permissions
=============================================

:Author: J.Hommet, F.Mauger
:Date: 2017-10-25

Description of the permissions system associated to mailboxes
in Vire communication domains.

.. contents::

Definitions
===========

Supported  permissions granted  to  an actor  of  given category  with
respect to a given mailbox:

* ``put`` (``p``) : actor of  a given category can submit/put messages
  to the mailbox,
* ``private_pickup`` (``k``)  : actor of  a given category  can pickup
  messages from a private mailbox,
* ``public_subscribe``  (``s``)  :  actor  of  a  given  category  can
  subscribe to a public mailbox (this implies *public pickup*).

For  each  mailbox  within  a   domain,  the  permissions  granted  to
categories of actors are described through a 9 bit words:

+----------------+-----------------------+-----------------------+-----------------------+
| Actor category |  Server               | Subcontractor         |  Client               |
|                +-----------------------+-----------------------+-----------------------+
|                |         ``s``         |         ``b``         |         ``c``         |
+----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
|Access bits     | ``p`` | ``k`` | ``s`` | ``p`` | ``k`` | ``s`` | ``p`` | ``k`` | ``s`` |
+----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

Examples:

* ``--s------`` : Server can subscribe to a public mailbox (and pickup
  messages from it),
* ``p-----p--`` : Server and clients  can publish message to a mailbox
  (public or private),
* ``----k----``  : Subcontractor  can pickup  messages from  a private
  mailbox.


Vire clients gate domain
========================

+-----------------------------------------------------------+---------------+
| Mailbox                                                   | Permissions   |
+----------------------------+------------------------------+               |
| Name                       | Address                      |               |
+============================+==============================+===============+
| ``Gate``                   | ``gate.service``             | ``--s---p--`` |
+----------------------------+------------------------------+---------------+


Vire client system domain
=========================

+-----------------------------------------------------------+---------------+
| Mailbox                                                   | Permissions   |
+----------------------------+------------------------------+               |
| Name                       | Address                      |               |
+============================+==============================+===============+
| ``RequestToServer``        | ``vireserver.service``       | ``--s---p--`` |
+----------------------------+------------------------------+---------------+
| ``SignalFromServer``       | ``vireserver.event``         | ``p-------s`` |
+----------------------------+------------------------------+---------------+


Vire control domain
===================

+-----------------------------------------------------------+---------------+
| Mailbox                                                   | Permissions   |
+----------------------------+------------------------------+               |
| Name                       | Address                      |               |
+============================+==============================+===============+
| ``ResourceRequest``        | ``resource_request.service`` | ``--s---p--`` |
+----------------------------+------------------------------+---------------+


Vire monitoring domain
======================

+-----------------------------------------------------------+---------------+
| Mailbox                                                   |  Permissions  |
+----------------------------+------------------------------+               |
| Name                       | Address                      |               |
+============================+==============================+===============+
| ``ResourceRequest``        | ``resource_request.service`` | ``p-s--sp--`` |
+----------------------------+------------------------------+---------------+
| ``Alarm``                  | ``alarm.event``              | ``p-sp----s`` |
+----------------------------+------------------------------+---------------+
| ``Log``                    | ``log.event``                | ``p-sp----s`` |
+----------------------------+------------------------------+---------------+
| ``PubSub``                 | ``pubsub.event``             | ``p-sp----s`` |
+----------------------------+------------------------------+---------------+


Subcontractor system domain
===========================

+------------------------------------------------------------+---------------+
| Mailbox                                                    | Permissions   |
+-----------------------------+------------------------------+               |
| Name                        | Address                      |               |
+=============================+==============================+===============+
| ``SignalFromSubcontractor`` | ``subcontractor.event``      | ``--sp-----`` |
+-----------------------------+------------------------------+---------------+
| ``RequestToServer``         | ``vireserver.service``       | ``--sp-----`` |
+-----------------------------+------------------------------+---------------+
| ``SignalFromServer``        | ``vireserver.event``         | ``p----s---`` |
+-----------------------------+------------------------------+---------------+
| ``RequestToSubcontractor``  | ``subcontractor.service``    | ``p----s---`` |
+-----------------------------+------------------------------+---------------+


.. the end
