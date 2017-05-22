====
Vire
====

C++ library for the description and management of an experimental setup

**This README file is in progress**


*Warning*: Do not even  dare to dream to hope to use  Vire now!  It is
still in intensive development phase  and not ready for production and
even testing for fun.

Introduction
------------

This is the Vire_ C++ software suite.

*Vire* ([vir])  stands for (Vir)tual  (E)xperiment.  It consists  in a
C++   library   and   some    helper   applications   to   model   the
components/devices  (mechanics, electronic  devices, cables...)   that
compose an experimental setup, typically for physics experiment.  Vire
is inspired by some other software architectures like the ones used in
SystemC_, Geant4_/GDML_ and Bayeux_.

.. _Vire:   https://github.com/BxCppDev/Vire
.. _Bayeux:   https://github.com/BxCppDev/Bayeux
.. _SystemC:  http://www.systemc.org/
.. _Geant4:   http://geant4.org/
.. _GDML: http://gdml.web.cern.ch/GDML/

The purpose of Vire is to provide basic tools to:

  * Describe an experimental setup, its architecture and hierarchy, components and
    connections (cable, pipe, abstract links) between components,
  * Define an addressing scheme for all (or parts of) the components in the system:
    devices, channels, cables, registers, configuration parameters and measurement
    data points, control and monitoring actions...
  * Define/generate cabling tables,
  * Build helper tools for setup management, simulation, data analysis...
  * Implement a real time control and monitoring system with:

    * Support for users, groups of users, roles and resources management,
    * Device configuration and system parametrization,
    * Build online control and monitoring applications (*Command and control*),

Vire team
---------

The Vire design  and development team is hosted at  the *Laboratoire de
Physique Corpusculaire de Caen* (`LPC Caen`_, Normandie Univ, ENSICAEN,
UNICAEN, CNRS/IN2P3):

  * Jean Hommet <hommet@lpccaen.in2p3.fr>
  * François Mauger <mauger@lpccaen.in2p3.fr>
  * Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>

The *Vire/MOS*  and *Vire/CMSLAPPInterface* plugins are  developped in
collaboration  with the  authors  of the  *Multipurpose OPCUA  Server*
control and  monitoring software at  *Laboratoire d'Annecy le  Vieux de
Physique  des Particules*  (LAPP_, CNRS/IN2P3,  Université Savoie  Mont
Blanc) in the framework of the SuperNEMO double beta decay experiment.

.. _`LPC Caen`: http://www.lpc-caen.in2p3.fr/
.. _LAPP: https://lapp.in2p3.fr/


License
-------

Vire is free  software, released under the GNU  GENERAL PUBLIC LICENSE
3.0.  See  the LICENSE.txt file  distributed together with  the source
code.


Prerequisites and dependencies
------------------------------

Vire supports  Ubuntu Linux 16.04  system (x86_64), which is  our main
development system.  Support for more flavors of Linux will come later
(i.e. Scientific Linux 7.X).

Vire  is built  on top  of the  Bayeux_ library  which assumes  itself
you'll build and run the software from a Linuxbrew_ system.

 * Linuxbrew_  is  a  package  manager  for  Linux  and  macosX  (visit
   https://github.com/BxCppDev/homebrew-bxtap for more informations).
 * Bayeux_ provides  its Bayeux/datatools and  Bayeux/geomtools modules
   which are used to build core parts of Vire.


More, Vire depends on other third party software packages:

 * protobuf_ : Google Protocol Buffers (3.0)
 * librabbitmq-c_ : RabbitMQ C AMQP client library (version 0.8.0 with support for the AMQP protocol version 0.9.1)
 * BxProtobuftools_ : A C++ library which eases the use of the Google Protobuf Buffers serialization system
   for C++ classes (depends on protobuf_).
 * BxJsontools_ : A C++ library which eases the serialization of C++ classes using JSON.
 * BxRabbitMQ_ : A C++ library which wraps parts of the librabbitmq C library (depends on librabbitmq-c_ and BxJsontools_).
 * Java : OpenJDK Runtime Environment (version 1.8.0)
 * Bayeux_ : The Bayeux library (version >= 3.0.0)

.. _Linuxbrew:   http://linuxbrew.sh/
.. _protobuf: https://github.com/google/protobuf
.. _librabbitmq-c:    https://github.com/alanxz/rabbitmq-c
.. _BxProtobuftools: https://github.com/BxCppDev/bxprotobuftools
.. _BxJsontools:     https://github.com/BxCppDev/bxjsontools
.. _BxRabbitMQ:      https://github.com/BxCppDev/bxrabbitmq

We warmly recommend to use Linuxbrew_ and the `BxCppDev Linuxbrew tap`_
to install Vire library's  core dependencies.

.. _`BxCppDev Linuxbrew tap`: https://github.com/BxCppDev/homebrew-bxtap


Getting Vire
------------

Vire is hosted at  https://github.com/BxCppDev/Vire.  From a directory
of  your choice,  clone Vire  source code  and cd  in the  Vire source
directory:

.. code:: sh

   $ mkdir -p ${HOME}/Vire
   $ cd ${HOME}/Vire # You can choose any suitable place to host the Vire repository
   $ git clone https://github.com/BxCppDev/Vire Vire.git
   $ cd Vire.git
..

The default branch is named ``develop``. You way want to switch to the
``master`` stable branch before to build the Vire software:

.. code:: sh

   $ git checkout master
..


Dependencies
------------

WIP

List of required libraries and tools:

* Build tools:

  - Linuxbrew (https://github.col/BxCppDev/homebrew-bxtap)
  - cmake >=3.8.1 (automatically installed from Linuxbrew)
  - pkg-config >=0.29.2 (automatically installed from Linuxbrew)

* Third party libraries:

  - curlpp >= 0.8.1 (automatically installed from Linuxbrew)
  - rabbitmq-c >= 3.3.0 (automatically installed from Linuxbrew)
  - Java JDK (installed from the system)

* BxCppDev software (installed from the https://github.col/BxCppDev/homebrew-bxtap Linuxbrew tap):

  - BxJsontools 0.1.0 (automatically installed from the Linuxbrew BxCppDev tap)
  - Protobuf 3.3.0 (automatically installed from the Linuxbrew BxCppDev tap)
  - BxProtobuftools 0.2.0 (installed from the Linuxbrew BxCppDev tap)
  - BxRabbitMQ 0.3.0 (installed from the Linuxbrew BxCppDev tap)
  - Bayeux 3.0.0 (installed from the Linuxbrew BxCppDev tap)

Example of dependency installation for Ubuntu Linux 16.04:

.. code::sh

   $ sudo apt-get install openjdk-8-jdk
   $ brew tap bxcppdev/homebrew-bxtap
   $ brew install bxcppdev/bxtap/bxrabbitmq --with-manager
   $ brew install bxcppdev/bxtap/bxprotobuftools
   $ brew install bxcppdev/bxtap/bayeux@3.0.0
..

Vire on Linuxbrew
------------------

WIP

No brew formula is provided for Vire yet.


Installation
------------

1. Setup Linuxbrew (visit https://github.com/BxCppDev/homebrew-bxtap/blob/master/README.md)

.. code:: sh

   $ linuxbrew_setup
..

   Your system PATH should be prefixed with the brew binary path, example:

.. code:: sh

   $ echo $PATH
   /path/to/Linuxbrew/installation/directory/bin:/some/other/directories/in/your/path...
..

   You should also be able to guess the Linuxbrew installation directory through:

.. code:: sh

   $ which clhep-config                # check the clhep-config is now in your PATH
   ...
   $ clhep-config --prefix | tr -d '"' # check the CLHEP library installation path,
                                       # it should be the Linuxbrew prefix.
   ...
..


2. Create a build directory for Vire and cd in it:

.. code:: sh

   $ mkdir -m ${HOME}/Vire/_build.d
   $ cd ${HOME}/Vire/_build.d
..

3. Setup the Bayeux library with dedicated scripts:

.. code:: sh

   $ bayeux_pro_setup  # your own setup function to 'activate' Bayeux (export PATH=...)
   $ which bxquery     # check the bxquery is now in your PATH
   ...
   $ bxquery --prefix  # check the Bayeux's installation path
   ...
..

4. Setup third party software:

.. code:: sh

      $ bxprotobuftools_setup          # your own setup function to 'activate' bxprotobuftools
      $ bxprotobuftools-query --prefix # check installation path
      ...
      $ bxrabbitmq_setup               # your own setup function to 'activate' bxrabbitmq
      $ bxrabbitmq-query --prefix      # check installation path
      ...
..


5. Configure:

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

6. Build:

.. code:: sh

      $ make -j4
..

7. Run tests:

.. code:: sh

      $ make test
..

8. Install:

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
