==================================================
The Vire CMS/LAPP interface extension
==================================================


Introduction
============

The  Multipurpose OPCUA  Server  (MOS) is  a  software component  that
implements and runs a OPCUA server  to access hardware devices.  It is
developped by the software group at LAPP (IN2P3/CNRS).

Within Vire, the MOS servers are hidden behind a dedicated server, the
CMS/LAPP  server, which  is the  unique single  window that  allows to
perform  command  and monitoring  operations  related  to the  devices
managed by MOS Servers.

This Vire extension  implements an API to  implement the communication
between a  Vire Server instance and  a CMS/LAPP server. It  is used to
implement the  Control and  Monitoring System  (CMS) of  the SuperNEMO
Demonstrator Experiment.
