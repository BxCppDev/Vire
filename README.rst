====
Vire
====

C++ library for the description and management of an experimental setup

**This README file is in progress**

Introduction
------------

This is the Vire C++ software suite.

Vire ([vir]) stands for (Vir)tual  (E)xperiment.  It consists in a C++
library and  some helper applications to  model the components/devices
(mechanics,   electronic   devices,   cables...)   that   compose   an
experimental  setup.    Vire  is  inspired  by   some  other  software
architectures  like  the  ones  used  in  SystemC_,  Geant4_/GDML  and
Bayeux_.

.. _Bayeux:   https://github.com/BxCppDev/Bayeux
.. _SystemC:  http://www.systemc.org/
.. _Geant4:   http://geant4.org/

The purpose of Vire is to provide basic tools to:

 - Describe an experimental setup, its architecture and hierarchy, components and
   connections (cable, pipe, abstract links) between components,
 - Define an addressing scheme for all (or parts of) the components in the system:
   devices, channels, cables, registers, configuration parameters and measurement
   data points, control and monitoring actions...
 - Define/generate cabling tables,
 - Build helper tools for setup management, simulation, data analysis...
 - Implement a real time control and monitoring system with:

   - Support for users, groups of users, roles and resources management,
   - Device configuration and system parametrization,
   - Build online control and monitoring applications (*Command and control*),

Vire team
---------

The current design and development team is:

 - Jean Hommet <hommet@lpccaen.in2p3.fr>
 - François Mauger <mauger@lpccaen.in2p3.fr>
 - Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>

License
-------

Vire is free software, released under the GNU GENERAL PUBLIC LICENSE 3.0.
See the LICENSE.txt file distributed together with the source code.


Prerequisites and dependencies
------------------------------

Vire supports Ubuntu Linux 16.04 system (x86_64), which is our main development system.
Support for more flavors of Linux will come later (i.e. Scientific Linux 7.X).

Vire is built on top of the  Bayeux_ library which assumes itself you'll build and run
the software from a Linuxbrew_ system.

* Linuxbrew_ is a package manager for Linux and macosX (visit https://github.com/SuperNEMO-DBD/brew).
* Bayeux_ provides its Bayeux/datatools and Bayeux/geomtools modules which are used to build core
  parts of Vire.

  .. _Linuxbrew:   http://linuxbrew.sh/

More, Vire depends on other third party software packages:

* protobuf_ : Google Protocol Buffers (3.0)
* librabbitmq-c_ : RabbitMQ C AMQP client library (version 0.8.0 with support for the AMQP protocol version 0.9.1)
* BxProtobuftools_ : A C++ library which eases the use of the Google Protobuf Buffers serialization system
  for C++ classes (depends on protobuf_).
* BxJsontools_ : A C++ library which eases the serialization of C++ classes using JSON.
* BxRabbitMQ_ : A C++ library which wraps parts of the librabbitmq C library (depends on librabbitmq-c_ and BxJsontools_).
* Java : OpenJDK Runtime Environment (version 1.8.0) (``sudo apt-get install openjdk-8-jdk``)

.. _protobuf: https://github.com/google/protobuf
.. _librabbitmq-c:    https://github.com/alanxz/rabbitmq-c
.. _BxProtobuftools: https://github.com/BxCppDev/bxprotobuftools
.. _BxJsontools:     https://github.com/BxCppDev/bxjsontools
.. _BxRabbitMQ:      https://github.com/BxCppDev/bxrabbitmq

We recommend to use Linuxbrew_ and the `BxCppDev Linuxbrew tap`_ to install Vire library's  dependencies.

.. _`BxCppDev Linuxbrew tap`: https://github.com/BxCppDev/homebrew-bxtap


Getting Vire
------------

Vire is hosted at https://github.com/BxCppDev/Vire.

1. From a directory of your choice, clone Vire source code and cd in the Vire source directory:

.. code:: sh

   $ mkdir -p ${HOME}/Vire
   $ cd ${HOME}/Vire # Choice any suitable place to host the Vire repository
   $ git clone https://github.com/BxCppDev/Vire Vire.git
   $ cd Vire.git
..


Installation
------------


1. Create a build directory for Vire

.. code:: sh

   $ mkdir -m ${HOME}/Vire/_build.d
   $ cd ${HOME}/Vire/_build.d
..

3. Setup Linuxbrew or enter a brew shell (assuming the ``brew`` command is in your ``PATH``):

.. code:: sh

   $ linuxbrew_setup
..


4. Setup the Bayeux library with dedicated scripts:

.. code:: sh

   $ bayeux_pro_setup  # your own setup function to 'activate' Bayeux (export PATH=...)
   $ which bxquery     # check the bxquery is now in your PATH
   ...
   $ bxquery --prefix  # check the Bayeux's installation path
   ...
..

   You should also be able to guess the Linuxbrew installation directory through:

.. code:: sh

      $ which clhep-config     # check the clhep-config is now in your PATH
      ...
      $ clhep-config --prefix | tr -d '"'  # check the CLHEP library installation path (should be brew!)
      ...
..

5. Setup third party software:

.. code:: sh

      $ protobuf_setup                 # your own setup function to 'activate' protobuf
      $ which protoc                   # check installation path
      $ bxprotobuftools_setup          # your own setup function to 'activate' bxprotobuftools
      $ bxprotobuftools-query --prefix # check installation path
      ...
      $ bxrabbitmq_setup               # your own setup function to 'activate' bxrabbitmq
      $ bxrabbitmq-query --prefix      # check installation path
      ...
..


6. Configure:

.. code:: sh

      $ cmake \
	     -DCMAKE_BUILD_TYPE:STRING=Release \
	     -DCMAKE_INSTALL_PREFIX:PATH=${HOME}/Vire/install \
	     -DCMAKE_FIND_ROOT_PATH:PATH="$(clhep-config --prefix | tr -d '"')" \
	     -DBayeux_DIR:PATH="$(bxquery --prefix)" \
	     -DVIRE_COMPILER_ERROR_ON_WARNING=ON \
	     -DVIRE_CXX_STANDARD="11" \
	     -DVIRE_ENABLE_TESTING=ON \
	     -DVIRE_WITH_DOCS=ON \
	     -DVIRE_WITH_DEVELOPER_TOOLS=ON \
	     -DVIRE_WITH_PLUGINS=ON \
	     -DVIRE_WITH_SANDBOX=OFF \
	     -DVIRE_WITH_JAVA=ON \
	     -DVIRE_WITH_PROTOBUF_JAVA=ON \
	     -DPROTOBUF_ROOT:PATH="/opt/sw/GoogleProtocolBuffers/install-3.0.0" \
	     -DBxRabbitMQ_DIR:PATH="$(bxrabbitmq-query --cmakedir)" \
	     -DBxProtobuftools_DIR:PATH="$(bxprotobuftools-query --cmakedir)" \
	     ${HOME}/Vire/Vire.git
..

7. Build:

.. code:: sh

      $ make -j4
..

8. Run tests:

.. code:: sh

      $ make test
..

9. Install:

.. code:: sh

      $ make install
..


Setup Vire in your environment
-------------------------------

If you use a Bash environment, we recommend to create a function from
your startup file ``~/.bashrc`` :

.. code:: sh

     function do_vire_develop_setup()
     {
       protobuf_setup
       bxprotobuftools_setup
       bxrabbitmq_setup
       bayeux_setup
       if [ -n "${VIRE_INSTALL_DIR}" ]; then
         echo "ERROR: Vire/develop is already setup ! Ignore!" >&2
         return 1
       fi
       export VIRE_INSTALL_DIR={Vire installation directory}
       export PATH=${VIRE_INSTALL_DIR}/bin:${PATH}
       echo "NOTICE: Vire/develop is now setup !" >&2
       return;
     }
     export -f do_vire_develop_setup
     alias vire_dev_setup="do_vire_develop_setup"
..

   where ``protobuf_setup``, ``bxprotobuftools_setup``, ``bxrabbitmq_setup`` and ``bayeux_setup``
   are shell function/aliases which setup the third party software not managed through brew.

   Then each time you want to use this version of Vire from a shell, type:

.. code:: sh

   shell$ vire_dev_setup
..
