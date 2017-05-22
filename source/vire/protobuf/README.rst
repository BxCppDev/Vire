=============
Vire Protobuf
=============

List of protobuf files supported by the Vire library.

Basic types conventions used by Vire
------------------------------------

- C/C++ types: ``int8_t``, ``int16_t`` and ``int32_t`` are mapped
  to Protobuf type ``sint32``,
- C/C++ types: ``uint8_t``, ``uint16_t`` and ``uint32_t`` are mapped
  to Protobuf type ``uint32``,
- C/C++ types: ``int64_t`` is mapped to Protobuf type ``sint64``,
- C/C++ types: ``uint64_t`` is mapped to Protobuf type ``uint64``.


Field index conventions
-----------------------

- The index ``1`` is  reserved for a  field representing
  the base class.   Such a field, if any, is conventionally named ``_base``.
- Indexes between ``2`` and ``99`` are reserved fro future usage.
- Fields in protobuf message use index starting from ``100``.

Other conventions
-----------------

- An  enumeration  should start  at  ``0``  (representing the  invalid
  default value).

JAR file
--------

The Java archive containing Java stub classes built from the
``proto`` files is conventionnaly named: ``VireProtobuf.jar``.

.. end
