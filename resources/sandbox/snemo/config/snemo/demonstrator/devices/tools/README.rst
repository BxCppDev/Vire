====================
 Device tools
====================

Convert MOS device definition XML files to Vire device definition files:

.. code:: sh

   $ which virequery
..

.. code:: sh

   $ cd {...}/config/snemo/demonstrator/devices/tools
   $ ./export_mos_devices.bash --list-of-devices
   $ ./export_mos_devices.bash --build-device-model SNEMO/COIL/COIL_PS_1.xml
   $ ./export_mos_devices.bash --build-all-device-models --install

..

Tools
=====

Parse the ``devices_launch.conf`` startup file and extract informations
for building Vire device models definition files:

.. code:: sh

   $ cd {...}/config/snemo/demonstrator/devices/0.1/tools
   $ python3 mos_device_mgr.py \
	  -L ../../cms/mos_xml/0.9.2/SNEMO/devices_launch.conf \
	  -o snemo_mos_devices.lis

..
   end
