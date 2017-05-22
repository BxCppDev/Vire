====================================================================
XSD dictionaries for MOS servers and devices  description files
====================================================================

List of XML schema dictionary files (version 2.0.2) used in the ``Vire_MOS`` plugin:

 * ``MOS_Alarm.xsd`` : Definitions of alarm/event related structures
 * ``MOS_FSM.xsd`` : Finite state machine support
 * ``MOS_HardwareInterface.xsd`` : Not really used for now
 * ``MOS_OPCUA.xsd`` : Definitions of the top-level OPCUA structures: OPCUA servers, devices and datapoints
 * ``MOS_SimpleDefinition.xsd`` : Definitions of simple types
 * ``MOS_Structure.xsd`` : Various structures
 * ``MOS_Unit.xsd`` : Physical unit support

History
-------

2.0.2
~~~~~

Release 2.0.2  (J.-L.  Panazol) consists in  reordering the dictionary
files to  order the tags like  in version 2.0.0 because  of historical
constraints in the XML validation process.

2.0.1
~~~~~~~

Some  modifications  have  been  applied to  take  into  account  last
discussions between the Vire and MOS groups (preliminary 2.0.1):

1. the ``MetaTag``  element has  been renamed  in ``UserInfo``  in the
   ``MOS_SimpleDefinition.xsd`` file.
2. the ``UserInfo``  element originaly present as  ``MetaTag`` is used
   in the following elements (from the ``MOS_OPCUA.xsd`` file):

   * ``OPCUA``
   * ``CompoundDevice``
   * ``SimpleDevice``
   * ``CompoundDatapoint``
   * ``SimpleDatapoint``
   * ``Variable``
   * ``Method``
   * ``Argument``

3. XML style (indentation, *untabification*) and has been revisited to
   ease readability if XSD files.
4. Some comments were fixed.

2.0.0
~~~~~

The original files have been provided by J.-L.  Panazol as the version
2.0.0 of the MOS schema (2015-11-19).
