==================================================
The Vire MOS (Multipurpose OPCUA Server) extension
==================================================

Introduction
============

The  Multipurpose OPCUA  Server  (MOS) is  a  software component  that
implements and runs a OPCUA server  to access hardware devices.  It is
developped by the software group at LAPP (IN2P3/CNRS).

This  Vire extension  implements tools  and interfaces  to communicate
with a MOS server and publish  its *datapoints* and *method* in a Vire
environment.

To do
=====

* Some XSD schema files in MOS 2.0.3 (``resources/data/mos/xml/2.0.3/``)
  should be fixed. The ``MOS_OPCUA.xsd`` dictionary uses malformed
  ``"Info"`` elements in the following sequence elements:

  * ``SimpleDevice``
  * ``CompoundDevice``
  * ``CompoundDatapoint``
  * ``SimpleDatapoint``
  * ``Variable``
  * ``Method``

  .. code:: xsd

     <xs:element minOccurs="0" maxOccurs="unbounded" ref="Info" />
  ..

  It should be:

  .. code:: xsd

     <xs:element minOccurs="0" ref="Info" />
  ..

  Such elements will be then mapped as optional attributes in Vire MOS
  associated classes.
