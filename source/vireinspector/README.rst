====================
   Vire Inspector
====================



Test
----

From the Vire build directory:

1. Prepare setup to address useful resource files:

.. code:: sh

   $ export VIRE_RESOURCE_FILES_DIR=$(pwd)/BuildProducts/share/resources
   $ export VIRE_SANDBOX_RESOURCES_DIR=${VIRE_RESOURCE_FILES_DIR}/sandbox
..

2. Run the inspector:


.. code:: sh

   $ BuildProducts/bin/vireinspector -c @vire:sandbox/snemo/config/1.0/devices/manager.conf
..
