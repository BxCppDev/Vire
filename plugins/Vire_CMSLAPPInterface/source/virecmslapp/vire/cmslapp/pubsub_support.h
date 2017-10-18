//! \file  vire/cmslapp/pubsub_support.h
//! \brief Pub/Sub support for the server by the Vire server
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_CMSLAPP_PUBSUB_SUPPORT_H
#define VIRE_CMSLAPP_PUBSUB_SUPPORT_H

#include<vire/cms/resource_pubsub.h>
#include<vire/cms/resource_exec.h>
#include<vire/cms/resource_exec_success.h>
#include<vire/cms/resource_exec_failure.h>

namespace vire {

  namespace cmslapp {

    // The name of the effective resource path corresponds to a method name on CMS/LAPP side:
    // Format is :
    //   <Vire resource path> + '/'+ "__dp_sub__"
    //
    // Argument: boolean
    //  0 == false : unsubscribe
    //  1 == true :  subscribe
    //
    // Example:
    //
    //   SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Current/__dp_sub__(0|1)
    //
    // Returned payload:
    //   vire::cms::resource_exec_success
    // or:
    //   vire::cms::resource_exec_failure
    //
    static const std::string & dp_pub_suffix();

    // Convert a "resource_pubsub" payload received from the Vire client to the Vire server
    // (PubSub manager) into a dedicated "resource_exec" payload for 
    int convert(const vire::cms::resource_pubsub & pubsub_,
                 vire::cms::resource_exec & exec_);

    /* void convert(const vire::cms::resource_exec_success & pubsub_, */
    /*           vire::cms::resource_exec & exec_); */

    /* void convert(const vire::cms::resource_exec_failure & pubsub_, */
    /*           vire::cms::resource_exec & exec_); */

  } // namespace cmslapp

} // namespace vire

#endif // VIRE_CMSLAPP_DISCONNECTION_REQUEST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
