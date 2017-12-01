//! \file vire/mos/opcua_export_vire.h
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

#ifndef VIRE_MOS_OPCUA_EXPORT_VIRE_H
#define VIRE_MOS_OPCUA_EXPORT_VIRE_H

// Standard library:
#include <string>
#include <list>

// Third Party:
// - Boost++:
#include <boost/filesystem.hpp>
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/properties.h>
#include <datatools/introspection/data_description.h>

// This project:
#include <vire/mos/opcua_ast.h>
#include <vire/utility/rw_access.h>
#include <vire/mos/export_utils.h>

namespace vire {

  namespace mos {

    //! \brief Export an OPCUA mos structure to a setup of Vire devices
    class opcua_export_vire
    {
    public:

      //! Return the standard filename for device and port models definitions
      static const std::string & model_definition_filename();

      //! Return the standard filename for merged device and port models definitions
      static const std::string & merged_models_definition_filename();

      //! Return the standard filename for list of definition files
      static const std::string & model_list_of_definition_filenames();

      //! Return the standard suffix for model name
      static const std::string & model_name_suffix();

      //! Return the name of the standard programs informations device
      static const std::string & programs_informations_name();

      //! Default constructor
      opcua_export_vire();

      //! Destructor
      ~opcua_export_vire();

      //! Set the logging priority
      void set_logging(datatools::logger::priority);

      //! Return the logging priority
      datatools::logger::priority get_logging() const;

      //! Check if only public components are exported
      bool is_export_only_public_components() const;

      //! Set the flag to export only public components
      void set_export_only_public_components(bool);

      // //! Check if datapoint should be equipped with their default inteface
      // bool is_datapoint_default_interface() const;

      // //! Set the flag to equip datapoint with their default inteface
      // void set_datapoint_default_interface(bool);

      //! Set the flag to not export "private informations" compound datapoints
      void set_export_no_private_informations(bool);

      //! Set the flag to not export "get" and "set" MOS methods
      void set_export_no_getset_methods(bool);

      //! Set the flag to not export the MOS server model
      void set_export_no_mos_server(bool);

      //! Set the export path
      void set_export_path(const std::string &);

      //! Return the export path
      const std::string & get_export_path() const;

      //! Return the suffix for model name
      std::string get_model_name_suffix();

      //! Check if OPCUA server model name is set
      bool has_model_name() const;

      //! Return the OPCUA server model name
      const std::string & get_model_name() const;

      //! Set the OPCUA server model name
      void set_model_name(const std::string &);

      //! Set the path prefix for listing the model definition files
      void set_model_def_filename_path_prefix(const std::string &);

      //! Check if the path prefix for listing the model definition files is set
      bool has_model_def_filename_path_prefix() const;

      //! Set the flag to not export the MOS server model
      void set_model_merge_def_files(bool);

      //! Main export
      void process(const vire::mos::OPCUA & server_);

      //! Translate MOS type to datatools introspection type
      static datatools::introspection::data_type translate_type(Type type_);

      //! Translate MOS info to RW access type
      static vire::utility::rw_access_type translate_rw_access(const has_infos_interface & with_infos_,
                                                               bool strict_ = false);

      //! Translate a typed object to a datatools introspection data description
      static void translate_data_description(const SimpleDatapoint & sdatapoint_,
                                             datatools::introspection::data_description & dd_);

      //! Translate an argument to a datatools introspection data description
      static void translate_data_description(const Argument & argument_,
                                             datatools::introspection::data_description & dd_);

    protected:

      //! Set default attributes
      void _set_defaults();

      void _process_server(const std::string & base_name_,
                           const boost::filesystem::path & base_dir_,
                           const OPCUA & server_);

      void _process_simple_datapoint(const std::string & base_name_,
                                     const boost::filesystem::path & base_dir_,
                                     const SimpleDatapoint & sdatapoint_);

      void _process_compound_datapoint(const std::string & base_name_,
                                       const boost::filesystem::path & base_dir_,
                                       const CompoundDatapoint & cdatapoint_);

      void _process_simple_device(const std::string & base_name_,
                                  const boost::filesystem::path & base_dir_,
                                  const SimpleDevice & sdevice_);

      void _process_compound_device(const std::string & base_name_,
                                    const boost::filesystem::path & base_dir_,
                                    const CompoundDevice & cdevice_);

      void _scan_datapoints(const has_datapoints_interface & device_with_datapoints_,
                            const std::string & base_name_,
                            const boost::filesystem::path & base_dir_,
                            datatools::properties & device_config_);

      void _scan_devices(const has_devices_interface & with_devices_,
                         const std::string & base_name_,
                         const boost::filesystem::path & base_dir_,
                         datatools::properties & with_devices_config_);

      void _scan_methods(const has_methods_interface & device_with_methods_,
                         const std::string & base_name_,
                         const boost::filesystem::path & base_dir_,
                         datatools::properties & device_config_,
                         std::vector<device_entry_type> & method_port_model_descriptions_);

      void _populate_method_port(const Method & method_,
                                 datatools::properties & method_port_config_);

      void _scan_device_attributes(const has_attributes_interface & common_,
                                   device_entry_type & common_model_desc_);

      void _scan_device_userinfos(const has_userinfos_interface & common_,
                                  device_entry_type & common_model_desc_);

      // bool is_published(const has_info_interface & with_info_, const std::string & name_) const;
      bool is_published(const has_infos_interface & with_infos_, const std::string & name_) const;

      bool is_special_published(const has_infos_interface & with_infos_, const std::string & name_) const;

    private:

      datatools::logger::priority _logging_;        //!< The logging priority threshold
      std::string _model_name_;                     //!< OPCUA server model name
      std::string _export_path_;                    //!< The base directory for the generation of device model definition files
      bool _export_only_public_components_  = true; //!< Flag to export only public components (methods/devices/datapoints)
      bool _export_no_private_informations_ = true; //!< Don't export private informations compound datapoints
      bool _export_no_getset_methods_       = true; //!< Don't export get/set methods
      bool _export_no_mos_server_           = true; //!< Don't export the MOS server model
      // bool _datapoint_default_interface_;           //!< Flag to setup the default interface for datapoint (not used)
      std::list<std::string> _model_def_filenames_list_;
      std::string            _model_def_filename_path_prefix_;
      bool                   _model_merge_def_files_ = true;

    };

  } // namespace mos

} // namespace vire

#endif // VIRE_MOS_OPCUA_EXPORT_VIRE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
