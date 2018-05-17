=================================================
How to use the RabbitMQ Server 
=================================================


System
======

* Ubuntu Linux 18.04
* rabbitmq-server, version 3.6.10-1



Install the server
==================

.. code:: sh

   $ sudo apt-get install rabbitmq-server
..


Configure the server
====================

TBD


Start the server
================

.. code:: sh

   $ sudo service rabbitmq-server start
..

Start the web interface
=======================

.. code:: sh

   $ sudo rabbitmq-plugins enable rabbitmq_management
..

Then browse URL: http://localhost:15672/


Administration account
======================

Create an administrator user for the Vire CMS server:

.. code:: sh

   $ sudo rabbitmqctl add_user supernemo_adm {password}
   $ sudo rabbitmqctl change_password supernemo_adm sesame
   $ sudo rabbitmqctl set_user_tags supernemo_adm administrator
   $ sudo rabbitmqctl set_permissions -p / supernemo_adm  ".*" ".*" ".*"
   $ sudo rabbitmqctl list_users

   
Normal account
==============

Create a normal user for the Vire CMS/LAPP subcontractor:

.. code:: sh

   $ sudo rabbitmqctl add_user cmslapp {password}
   $ sudo rabbitmqctl list_users


Vire CMS native Vhosts
======================

.. code:: sh

   $ sudo rabbitmqctl add_vhost /supernemo/demonstrator/cms/monitoring
   $ sudo rabbitmqctl add_vhost /supernemo/demonstrator/cms/control
   $ sudo rabbitmqctl add_vhost /supernemo/demonstrator/cms/subcontractors/cmslapp
   $ sudo rabbitmqctl add_vhost /supernemo/demonstrator/cms/clients/gate
   $ sudo rabbitmqctl list_vhosts

   $ sudo rabbitmqctl set_permissions -p /supernemo/demonstrator/cms/monitoring supernemo_adm  ".*" ".*" ".*"
   $ sudo rabbitmqctl set_permissions -p /supernemo/demonstrator/cms/control supernemo_adm  ".*" ".*" ".*"
   $ sudo rabbitmqctl set_permissions -p /supernemo/demonstrator/cms/subcontractors/cmslapp supernemo_adm  ".*" ".*" ".*"
   $ sudo rabbitmqctl set_permissions -p /supernemo/demonstrator/cms/clients/gate supernemo_adm  ".*" ".*" ".*"

..
   the end
