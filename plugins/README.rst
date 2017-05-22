=================================
Vire plugins directory
=================================

:Authors: François Mauger
:Date:    2016-09-30

.. contents::
   :depth: 3
..


Presentation
============

This is  the top  level directory  of all plugins associated to
the Vire library.

Contents:

 * ``README.rst`` : this file
 * ``CMakeLists.txt`` : the CMake script for plugin build/install
 * ``_make_list_of_plugins_resource_files`` : a shell script that generated
   official list of published resource files for each module
 * Plugins:

    * ``Vire_MOS`` : Vire/MOS interface and tools
    * ``Vire_CMSLAPPInterface/`` : Vire-CMS/LAPP interface and tools


Management of resource files
============================

Any  plugin may  provide some  resource files  (configuration or  data
files,  examples...).  We  use a  convention to  allow each  plugin to
publish   such    files   within    the   Vire    framework.    The
``_make_list_of_plugins_resource_files``  script is  able to  scan the
``resources``  directory from  the base  source directory  of a  given
plugin. The list of all publishable resource files is built and stored
in the  ``resources/_resources.cmake`` file  in a format  suitable for
CMake  processing.  From  this file,  CMake is  able to  configure and
install all the resource files associated to the plugin.

Each time some  new resource files are added to  (or removed from) the
list of publishable resource files by  the developper of a plugin, the
``_make_list_of_plugins_resource_files`` script must be used to update
the *official* list: ::

  $ cd {Vire trunk base dir}/plugins
  $ ./_make_list_of_plugins_resource_files --only {PluginName}

where  ``{Vire  trunk  base  dir}``  is  the  Vire  source  base
directory and ``{PluginName}`` is the name of the target plugin.

Example for the Vire_MOS plugin: ::

  $ ./_make_list_of_plugins_resource_files --only Vire_MOS
  $ svn st Vire_MOS/resources/
  ...
  M       _resources.cmake
  ...

Note  that  all  files  or directories  starting  with  an  underscore
(examples:  ``_dummy.txt``, ``_backup.d``),  ``.svn`` directories  and
backup files with the ``~`` suffix are not publishable by convention.
