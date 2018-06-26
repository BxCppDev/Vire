//! \file    vire/resource/datapoint_resource_instance.h
//! \brief   Representation of a datapoint resource
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_RESOURCE_DATAPOINT_RESOURCE_INSTANCE_H
#define VIRE_RESOURCE_DATAPOINT_RESOURCE_INSTANCE_H

// This project:
#include <vire/resource/base_resource_instance.h>
#include <vire/device/base_datapoint_model.h>

namespace vire {

  namespace resource {

    //! \brief Datapoint resource instance
    class datapoint_resource_instance
      : public base_resource_instance
    {
    public:

      //! Default constructor
      datapoint_resource_instance();

      //! Destructor
      virtual ~datapoint_resource_instance();

      //! Check if the datapoint model is set
      bool has_model() const;

      //! Set the datapoint model
      void set_model(const vire::device::base_datapoint_model & model_);

      //! Reset the datapoint model
      void reset_model();

      //! Return the const reference to the  datapoint model
      const vire::device::base_datapoint_model & get_model() const;

      //! Check initialization status
      virtual bool is_initialized() const;

      //! Initialize
      virtual void initialize(const datatools::properties & config_,
                              datatools::service_manager &);

      //! Reset
      virtual void reset();

      //! Smart print
     void print_tree(std::ostream & out_ = std::clog,
                     const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    private:

      // Management:
      bool _initialized_; //!< Initialization flag

      // Data:
      const vire::device::base_datapoint_model * _model_; //!< Datapoint model

    };

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_DATAPOINT_RESOURCE_INSTANCE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
