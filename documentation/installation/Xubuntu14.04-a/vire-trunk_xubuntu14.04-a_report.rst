.. -*- coding: utf-8 -*-

=================================================================
Vire/trunk installation report on (X)ubuntu 14.04 LTS (64bits)
=================================================================

:Author:  François Mauger, LPC Caen
:Contact: mauger@lpccaen.in2p3.fr
:Date:    2016-03-22
:Version: 0.1

In  this  document  we  propose  an  installation  procedure  for  the
Vire_/trunk  library on  top  of the Bayeux_/trunk and Cadfaelbrew_  (2016.01) on  Xubuntu
14.04 LTS (Trusty Tahr) for a system (64-bits).  By default, the build
is done using the C++11 standard.

.. _Vire: https://nemo.lpc-caen.in2p3.fr/wiki/Software/Vire

.. contents::

.. raw:: latex

   \pagebreak

The target system
==================

* Architecture:

.. code:: sh

   $ uname -a
   Linux bayeux-laptop 3.13.0-74-generic #118-Ubuntu SMP ... x86_64 GNU/Linux
..

* Processors:

.. code:: sh

   $ cat /proc/cpuinfo | grep "model name"
   model name   : Intel(R) Core(TM) i7-3540M CPU @ 3.00GHz
   model name   : Intel(R) Core(TM) i7-3540M CPU @ 3.00GHz
   model name   : Intel(R) Core(TM) i7-3540M CPU @ 3.00GHz
   model name   : Intel(R) Core(TM) i7-3540M CPU @ 3.00GHz
..

* Linux version:

.. code:: sh

   $ cat /etc/lsb-release
   DISTRIB_ID=Ubuntu
   DISTRIB_RELEASE=14.04
   DISTRIB_CODENAME=trusty
   DISTRIB_DESCRIPTION="Ubuntu 14.04.3 LTS"
..


* Dependencies:

Vire_ depends on Bayeux_ (and  Cadfaelbrew_). To install both packages,
please follow the instructions from:

 * Bayeux_ (SuperNEMO Wiki)
 * Cadfael_ (SuperNEMO Wiki)
 * Cadfaelbrew_ repository (GitHub)

.. _Bayeux: https://nemo.lpc-caen.in2p3.fr/wiki/Software/Bayeux
.. _Cadfael: https://nemo.lpc-caen.in2p3.fr/wiki/Software/Cadfael
.. _Cadfaelbrew: https://github.com/SuperNEMO-DBD/cadfaelbrew

Setup of Bayeux
===============================


Please follow the instructions on the installation report at https://nemo.lpc-caen.in2p3.fr/browser/Bayeux/trunk/doc/InstallationReports/Bayeux/Xubuntu14.04-a/tagged/bayeux-trunk_xubuntu14.04-a_report-0.1.pdf


Configuration and build of Bayeux/trunk
=================================================

Set the software base directory where there is enough storage capacity
to host  Vire (100 MB). Here we  use a simple  environment variable
``SW_WORK_DIR``  which   points  to   a  specific  directory   on  the
filesystem:

Set the software base directory where there is enough storage capacity to host Vire.
We choose here to use a simple environment variable ``SW_WORK_DIR``:

.. code:: sh

   $ export SW_WORK_DIR=/data/sw
..

You should adapt this base directory to your own system, for example:

.. code:: sh

   $ export SW_WORK_DIR=${HOME}/Software
..

Then create a few working directories:

.. code:: sh

   $ mkdir -p ${SW_WORK_DIR}
   $ mkdir ${SW_WORK_DIR}/Vire        # base working directory for Vire
   $ mkdir ${SW_WORK_DIR}/Vire/Source # hosts the source code
   $ mkdir ${SW_WORK_DIR}/Vire/Binary # hosts the build/installation directories
..

Download Vire
---------------------

Download Vire/trunk source files:

.. code:: sh

   $ cd ${SW_WORK_DIR}/Vire/Source
   $ svn co https://nemo.lpc-caen.in2p3.fr/svn/Vire/trunk Vire-trunk
   $ cd Vire-trunk
   $ LANG=C svn info
   ...
..

.. raw:: latex

   \pagebreak

Configure Vire
--------------------------

  1. Make sure Cadfaelbrew_ and Bayeux_ are setup on  your system. If you follow the
     Cadfaelbrew_ installation  report available from the  Cadfael wiki
     page, you just have to invoke:

.. code:: sh

   $ brewsh
..

     To enable Bayeux_, please use the dedicated function as explained in the documentation:

.. code:: sh

   $ do_bayeux_dev11_setup
..

     or use any proper technique to setup Bayeux_.


  2. Create a build directory and cd in it:

.. code:: sh

   $ VIRE_DEV_BIN_DIR="${SW_WORK_DIR}/Vire/Binary/Vire-trunk"
   $ mkdir -p ${VIRE_DEV_BIN_DIR}/Build-gcc-cxx11-ninja-Linux-x86_64
   $ cd ${VIRE_DEV_BIN_DIR}/Build-gcc-cxx11-ninja-Linux-x86_64
..

  3. Configure the Vire build with CMake and using Ninja and GCC :

   $ VIRE_DEV_INSTALL_DIR="${VIRE_DEV_BIN_DIR}/Install-gcc-cxx11-Linux-x86_64"
   $ cmake \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_INSTALL_PREFIX:PATH="${VIRE_DEV_INSTALL_DIR}" \
    -DCMAKE_FIND_ROOT_PATH:PATH="$(bxquery --prefix);$(gsl-config --prefix)/../.." \
    -DVIRE_FORCE_CXX_ALL_WARNINGS=OFF \
    -DVIRE_ENABLE_TESTING=ON \
    -DVIRE_WITH_DOCS=ON \
    -DVIRE_WITH_PLUGINS=OFF \
    -DVIRE_WITH_SANDBOX=OFF \
    -GNinja \
    ${SW_WORK_DIR}/Vire/Source/Vire-trunk
..

	 ..
	    You may not want to build the sandbox projects. In such case,
	    use  the ``-DVIRE_WITH_SANDBOX=OFF``  switch  in the  command
	    above.

Build
-----------------

Using 4 processors to go faster (depends on your machine):

.. code:: sh

   $ time ninja -j4


.. raw:: latex

   \pagebreak

Test programs
---------------------------------

Before to do the final installation, we run the test programs:

.. code:: sh

   $ ninja test
   ...
..

.. raw:: latex

   \pagebreak

Installation
============

Run:

.. code:: sh

   $ ninja install
   ...
..


Setup your environment for Vire
=======================================

Here we explicitely *load/setup* the Vire environment from a Bash shell
with a dedicated function defined in my ``~/.bashrc`` startup file:

.. code:: sh

   # The base directory of all the software (convenient path variable):
   export SW_WORK_DIR=/data/sw
   export VIRE_DEV_BIN_DIR="${SW_WORK_DIR}/Vire/Binary/Vire-trunk"

   # The Vire/trunk setup function:
   function do_vire_trunk_cxx11_setup()
   {
    do_bayeux_dev11_setup # Automatically setup the Bayeux dependency
    if [ -n "${VIRE_DEV_INSTALL_DIR}" ]; then
      echo "ERROR: Vire/trunk is already setup !" >&2
      return 1
    fi
    export VIRE_DEV_INSTALL_DIR=${VIRE_DEV_BIN_DIR}/Install-gcc-cxx11-Linux-x86_64
    export PATH=${VIRE_DEV_INSTALL_DIR}/bin:${PATH}
    echo "NOTICE: Vire/trunk is now setup !" >&2
    return;
   }
   export -f do_vire_trunk_cxx11_setup

   # Special alias:
   alias do_vire_dev11_setup="do_vire_trunk_cxx11_setup"
   alias do_vire_dev_setup="do_vire_trunk_cxx11_setup"
..

When I want to use pieces of software from Vire, I run:

.. code:: sh

   $ brewsh
   $ do_vire_dev_setup
   $ vire-query --help
..

.. raw:: latex

   \pagebreak

Appendices
===================================================

Rebuild Vire
------------------

In case of problem, you can discard the build directory and retry:

.. code:: sh

   $ rm -fr ${SW_WORK_DIR}/Vire/Binary/Vire-trunk/Build-gcc-cxx11-ninja-Linux-x86_64
   $ mkdir ${SW_WORK_DIR}/Vire/Binary/Vire-trunk/Build-gcc-cxx11-ninja-Linux-x86_64
   $ cd ${SW_WORK_DIR}/Vire/Binary/Vire-trunk/Build-gcc-cxx11-ninja-Linux-x86_64
..

then re-configure and build.

.. raw:: latex

   \pagebreak
