=====================================================
Vire library : architecture of the experiment manager
=====================================================

:author: J.Hommet, Y.Lemière, F.Mauger, J. Poincheval


The experiment manager
======================

 * Class name:  ``vire::experiment_manager``
 * Contains:

   * A 'bus' of components
   * A plugin system to dynamically/statically add more components

System components
=================

System components are:

 * The user manager (manage users, groups, authentication (delegated))
 * The resource manager :

    * dictionary of resources:

      * identification of resources (RID)
      * description of the resources (model)
      * sets of resources (as resources)


 * The session manager

      * manage login sessions

          * open/close
          * authentication
          * validation of resources request (set of resources)

            * access rights, availability, agenda...



The user manager
================

   * Class name: ``vire::user_manager``
   * The list of users
   * The list of user groups
   * File names for accessing user/group database files

User
====

* Class name:  ``vire::user``
* A user object is defined by :

  * UID : unique user identifier (int32_t)
  * Name (a.k.a. login) : unique string label (std::string)
  * Display name (std::string)
  * Description (std::string)
  * Some auxiliary properties (e-mail and other meta-data)

User group
==========

* Class name:  ``vire::user_group``
* A user group object is defined by :

  * GID : unique user group identifier (int32_t)
  * Name : unique string label (std::string)
  * Display name (std::string)
  * Description (std::string)
  * Some auxiliary properties (meta-data)
  * A specific user selector object (via a ``cuts::cut_handle_type`` from the Bayeux/cuts library)

A `user selector` is able to check the membership of any user with respect to the group.
This kind of object is inherited from the ``cuts::i_cut`` abstract class.
A simple case is implemented by the ``vire::enumerated_user_selector`` class
which describes explicitely the list of users in a given group: ::

  group "expert" = { "john", "jimmy", "jarvis" }
  group "user" = { "foo", "bar", "toto", "serge", "blah" }

Other user selector objects can be implemented, using specific boolean operations
on other user selectors: ::

  group "all" = "expert" OR "user"

which is equivalent to: ::

  group "all" = { "john", "jimmy", "jarvis", "foo", "bar", "toto", "serge", "blah" }

The ``Bayeux/cuts`` library already implements a large set of boolean operations:
``NOT``, ``AND``, ``OR``, ``NAND``, ``NOR``, ``XOR``, ``multi-OR``, ``multi-AND``...



The resource manager
====================

The resource manager knows everything about the full set of resources available from the experiment
manager.


Resource
=======

* Class name:  ``vire::resource``
* A resource object is defined by :

  * RID : unique resource identifier (int32_t)
  * Name (a.k.a. login) : unique string label (std::string)
  * Display name (std::string)
  * Description (std::string)
  * Some auxiliary properties (meta-data)
  * Path: unique string identifier following a hierarchical scheme (std::string)
  * Resource model:

  * Static traits

    * Allow
    * Deny
    * ...

  * Dynamic traits

    * List of subscribed sessions
    * (by-config) Lock





The session manager
===================



       ----> Session (uid/gids/roles
                      abonnement aux datapoints
		      list of resources (sets of datapoints)

Others:


 * ConfigControl:
    ----> ConfigManager
 * RunControl:
    ----> SingleRunManager
    ----> RunScheduler
 * HardwareControl (plugin component):
    ----> Mock_OPCUA
    ----> SuperNEMO_OPCUA
