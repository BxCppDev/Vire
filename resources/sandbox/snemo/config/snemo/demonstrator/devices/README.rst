==============================
Device description
==============================

Vire provides a system to describe the static structure and architecture of the
experimental  setup, in  terms of  deployed hardware  and software
devices. A given description is associated to its unique identifier.

This description of the structure of the experimental setup should not
be mixed up with its *dynamic configuration* for a given purpose (data
collection, commissioning runs...), i.e. the way settings are performed
by the user before a run.

This system aims to :

- describe the list of hardware/firmware/software devices used in the setup,
- organize these devices in a hierarchy (parent/child relationship),
- provide a naming scheme for all devices,
- describe the model associated to each device,
- provide a mapping scheme for addresses associated to some devices.

Experimental setup identifier
=============================

The experimental setup is identified through:

* an unique  name: ``"SuperNEMO"`` has  been chosen for  the SuperNEMO
  demonstrator experiment.
* a version number: a sequence of 1 to 3 integer numbers

  * the major number,
  * the minor number,
  * the revision number.

Examples:

- "SuperNEMO-0.1" identifies a description of the setup used only for testing purpose.
- "SuperNEMO-1.3.12" identifies a description of the setup for production use.

Versioning rules
=================

Version number bumping rules:

* The major number is bumped only  when the number and type of devices
  deployed in the architecture is  changed with respect to an existing
  version.
* The minor  number is bumped  only when  an existing device  is added
  with a new functionality: datapoint or model.
* The  revision number  is bumped  only when  a fix  is applied  to an
  existing device (error in the device model).
* Major  number 0  (*zero*) is  reserved for  testing and  development
  purpose.  It may  be reused  for  building a  new architecture  from
  scratch or from an existing one that must be significantly modified.
