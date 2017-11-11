
rabbitmqctrl
============

Create an administrator user for the Vire CMS server:

.. code:: sh

   $ sudo rabbitmqctl add_user supernemo_adm {password}
   $ sudo rabbitmqctl change_password supernemo_adm sesame
   $ sudo rabbitmqctl set_user_tags supernemo_adm administrator
   $ sudo rabbitmqctl list_users

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
