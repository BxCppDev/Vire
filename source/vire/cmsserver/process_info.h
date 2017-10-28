//! \file  vire/cmsserver/process_info.h
//! \brief The CMS server process info
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_PROCESS_INFO_H
#define VIRE_CMSSERVER_PROCESS_INFO_H

// Standard Library:
#include <string>
#include <memory>

// Third party:
// - Bayeux:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/properties.h>

namespace vire {

  namespace cmsserver {

    /// \brief Process information
    /**
     *
     *  [name="TrackerHVDataSurvey" type="snemo::cms::survey::device_histogramming"]
     *
     *  #@description Survey histogramming of tracker HV Power supply devices
     *
     *  histograms : string[2] = "h1" "h2"
     *  histograms.h1.resource    : string = "SuperNEMO:/Demonstrator/CMS/Tracker/PS3/Channel1/Current/__dp_read__"
     *  histograms.h1.update_rate : real as time = 10 second
     *  histograms.h2.resource    : string = "SuperNEMO:/Demonstrator/CMS/Tracker/PS3/Channel2/Current/__dp_read__"
     *  histograms.h2.update_rate : real as time = 16 second
     *  output : string as path = "${SNEMO_RT_SURVEY}/save1.hist"
     *  process.start_macro.urn : string = "snemo:cms:macros:histtrackerstart-1"
     *  process.start_macro     : string = "devhisttracker-start.mac"
     *  process.stop_macro.urn :  string = "snemo:cms:macros:histtrackerstop-1"
     *  process.stop_macro      : string = "devhisttracker-stop.mac"
     *
     *  [name="CaloHVDataSurvey" type="snemo::cms::survey::correlation_study"]
     *
     *  #@description Survey histogramming of calorimeter HV Power supply devices
     *
     *  devices : string[2] = "Dev1" "Dev2"
     *  devices.Dev1.path : string = "SuperNEMO:/Demonstrator/CMS/Calorimeter/PS1"
     *  devices.Dev1.relative_resources : string[3] = \
     *       "Channel1/Current/__dp_read__" "Channel1/Status/__dp_read__"  "Channel1/Voltage/__dp_read__"
     *  devices.Dev2.path : string = "SuperNEMO:/Demonstrator/CMS/Calorimeter/PS2"
     *  devices.Dev2.relative_resources : string[2] = \
     *       "Channel1/Current/__dp_read__" "Channel1/Status/__dp_read__"
     *  update_rate : real as time = 60 second
     *  output : string as path = "${SNEMO_RT_SURVEY}/save2.data"
     *
     */
    class process_info
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {
    public:

      //! Default constructor
      process_info();

      //! Destructor
      virtual ~process_info();

      bool is_valid() const;

      void reset();

      bool has_name() const;

      void set_name(const std::string &);

      const std::string & get_name() const;

      bool has_description() const;

      void set_description(const std::string &);

      const std::string & get_description() const;

      bool has_model_id() const;

      void set_model_id(const std::string &);

      const std::string & get_model_id() const;

      bool has_config_urn() const;

      void set_config_urn(const std::string &);

      const std::string & get_config_urn() const;

      bool has_config_path() const;

      void set_config_path(const std::string &);

      const std::string & get_config_path() const;

      const datatools::properties & get_config() const;

      datatools::properties & grab_config();

      void set_config(const datatools::properties &);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      // Configuration:
      std::string _name_;             ///< Unique process identifier
      std::string _description_;      ///< Short description of the process
      std::string _model_id_;         ///< Type of the process (class identifier)
      std::string _config_urn_;       ///< The tag of the set of configuration parameters
      std::string _config_path_;      ///< The path of the configuration file
      datatools::properties _config_; ///< Set of configuration parameters

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_PROCESS_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
