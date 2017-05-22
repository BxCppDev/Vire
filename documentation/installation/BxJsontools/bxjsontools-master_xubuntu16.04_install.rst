===============================================
Installation of BxJsontools (devel)
===============================================

:Author: François Mauger, LPC Caen <mauger@lpccaen.in2p3.fr>
:Date:   2016-09-22
:Version: 0.1

In  this  document  we  propose  an  installation  procedure  for  the
BxJsontools C++ library, master branch, pre version 0.1.0, on top of  Xubuntu
16.04 LTS (Xenial Xerus) for a system (64-bits).

BxJsontools is used by the Vire_ library for the SuperNEMO experiment.

.. contents::

Links
------------

 * Cadfael_ (SuperNEMO Wiki)
 * Cadfaelbrew_ repository (GitHub)
 * BxJsontools_ repository (GitHub)
 * Vire_ repository (SuperNEMO Wiki)

.. _Bayeux: https://nemo.lpc-caen.in2p3.fr/wiki/Software/Bayeux
.. _Cadfael: https://nemo.lpc-caen.in2p3.fr/wiki/Software/Cadfael
.. _Cadfaelbrew: https://github.com/SuperNEMO-DBD/brew
.. _BxJsontools: https://github.com/BxCppDev/bxjsontools.git
.. _Vire: https://nemo.lpc-caen.in2p3.fr/wiki/Software/Vire


Download
-------------
.. code:: sh

   $ mkdir -p /opt/sw/BxJsontools/Source
   $ mkdir -p /opt/sw/BxJsontools/Binary
   $ cd /opt/sw/BxJsontools/Source
   $ git clone https://github.com/fmauger/bxjsontools.git
..



Installation
--------------------

1. Create a build directory and start a brew (Cadfaelbrew_) session:

.. code:: sh

   $ cd /opt/sw/BxJsontools/Binary
   $ mkdir -p /opt/sw/BxJsontools/Binary/build.d
   $ cd /opt/sw/BxJsontools/Binary/build.d
   $ brewsh
..


2. Build/install:

.. code:: sh

   $ cmake \
   -DCMAKE_INSTALL_PREFIX="/opt/sw/BxJsontools/Binary/install-0.1.0" \
   -DBXJSONTOOLS_WITH_BOOST=ON \
   -DBoost_DIR="/opt/sw/SuperNEMO-DBD/Cadfaelbrew" \
   /opt/sw/BxJsontools/Source/bxjsontools
   $ make -j4
   $ make test
   $ make install
   ...
..

3. Check:

.. code:: sh

   $ LANG=C tree -L 1 /opt/sw/BxJsontools/Binary/install-0.1.0
   /opt/sw/BxJsontools/Binary/install-0.1.0
   |-- bin
   |-- include
   |-- lib
   `-- share

.. code:: sh

   $ /opt/sw/BxJsontools/Binary/install-0.1.0/bin/bxjsontools-query --version
   0.1.0
..


Setup BxJsontools
------------------------------

In your ``.bashrc`` script, add:

.. code:: sh

   export BXJSONTOOLS_BASE_DIR=/opt/sw/BxJsontools

   function do_bxjsontools_010_setup()
   {
     if [ -n "${BXJSONTOOLS_INSTALL_DIR}" ]; then
	  echo "WARNING: BxJsontools is already setup !" >&2
	  return 1
     fi
     export BXJSONTOOLS_INSTALL_DIR="${BXJSONTOOLS_BASE_DIR}/Binary/install-0.1.0"
     export PATH="${BXJSONTOOLS_INSTALL_DIR}/bin:${PATH}"
     echo "NOTICE: BxJsontools is now setup !" >&2
     return 0;
   }
   export -f do_bxjsontools_010_setup

   # Special aliases:
   alias bxjsontools_setup="do_bxjsontools_010_setup"
..

To setup the BxJsontools library from a terminal, run :

.. code:: sh

   $ brewsh
   $ bxjsontools_setup
..

Check the ``bxjsontools-query`` utility:

.. code:: sh

   $ which bxjsontools-query
   /opt/sw/BxJsontools/Binary/install-0.1.0/bin/bxjsontools-query
..
