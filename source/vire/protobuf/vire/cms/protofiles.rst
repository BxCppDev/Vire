===========================
Vire cms protobuf files
===========================

:author: F.Mauger
:email: mauger@lpccaen.in2p3.fr
:date: 2017-10-17
:version: 1.0.0
:status: validated

The package ``vire.cms`` contains several *protobuf* descriptions:

* Inherited from ``vire.utility.BaseError`` :

  * ``ArgumentError`` : C++ class ``vire::cms::argument_error``
  * ``InvalidCredentialsError`` : C++ class ``vire::cms::invalid_credentials_error``
  * ``InvalidResourceError`` : C++ class ``vire::cms::invalid_resource_error``
  * ``InvalidStatusError`` : C++ class ``vire::cms::invalid_status_error``
  * ``InvalidUserError`` : C++ class ``vire::cms::invalid_user_error``
  * ``NoPubsubResourceError`` : C++ class ``vire::cms::no_pubsub_resource_error``
  * ``ResourceExecError`` : C++ class ``vire::cms::resource_exec_error``
  * ``TimeoutError`` : C++ class ``vire::cms::timeout_error``
  * ``UnknownResourcesError`` : C++ class ``vire::cms::unknown_resources_error``

* Inherited from ``vire.utility.BasePayload`` :

  * ``MethodArgument`` : C++ class ``vire::cms::method_argument``
  * ``ResourceStatusRecord`` : C++ class ``vire::cms::resource_status_record``
  * ``ResourceExec`` : C++ class ``vire::cms::resource_exec``
  * ``ResourceExecSuccess`` : C++ class ``vire::cms::resource_exec_success``
  * ``ResourceExecFailure`` : C++ class ``vire::cms::resource_exec_failure``
  * ``ResourceFetchStatus`` : C++ class ``vire::cms::resource_fetch_status``
  * ``ResourceFetchStatusSuccess`` : C++ class ``vire::cms::resource_fetch_status_success``
  * ``ResourceFetchStatusFailure`` : C++ class ``vire::cms::resource_fetch_status_failure``
  * ``ResourcePubsub.PubsubAction`` : C++ enum ``vire::cms::resource_pubsub::pubsub_action``
  * ``ResourcePubsub`` : C++ class ``vire::cms::resource_pubsub``
  * ``ResourcePubsubSuccess`` : C++ class ``vire::cms::resource_pubsub_success``
  * ``ResourcePubsubFailure`` : C++ class ``vire::cms::resource_pubsub_failure``
  * ``ResourceStatusChange`` :
  * ``DeviceStatusChange`` :


.. end
