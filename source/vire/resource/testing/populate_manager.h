// populate_manager.h

#ifndef VIRE_RESOURCE_TESTING_POPULATE_MANAGER_H
#define VIRE_RESOURCE_TESTING_POPULATE_MANAGER_H

// Standard Library
#include <string>
#include <vector>
#include <sstream>

// This project:
#include <vire/resource/manager.h>
#include <vire/resource/resource.h>
#include <vire/resource/role.h>
#include <vire/resource/enumerated_resource_selector.h>

namespace vire {

  namespace resource {

    namespace testing {

      void populate_manager(vire::resource::manager & mgr_,
                            uint32_t flags_ = 0);

      void populate_manager(vire::resource::manager & mgr_,
                            uint32_t /* flags_ */)
      {
        // Create some mock resources:
        int resource_id = 1000;
        std::string explabel = "SuperNEMO";
        std::vector<std::string> parts;
        parts.push_back("calorimeter");
        parts.push_back("tracker");
        for (std::vector<std::string>::const_iterator ipart = parts.begin();
             ipart != parts.end();
             ipart++) {
          for (int icrate = 0; icrate < 2; icrate++) {
            for (int iboard = 0; iboard < 4; iboard++) {
              for (int ichannel = 0; ichannel < 4; ichannel++) {

                {
                  std::ostringstream path_oss;
                  path_oss << explabel << ':'
                           << "/Demonstrator/CMS/"
                           << *ipart
                           << '/' << "HV" << '/'
                           << "crate_" << icrate << '/'
                           << "board_" << iboard << '/'
                           << "ch_" << ichannel << '/'
                           << "current";
                  vire::resource::resource r;
                  r.set_id(resource_id++);
                  r.set_path(path_oss.str());
                  std::ostringstream name_oss;
                  name_oss << explabel << "__"
                           << '.' << *ipart
                           << '.' << "HV" << '.'
                           << "crate_" << icrate << '.'
                           << "board_" << iboard << '.'
                           << "ch_" << ichannel << '.'
                           << "current";
                  r.set_name(name_oss.str());
                  r.set_access(vire::utility::RW_READABLE);
                  if (ichannel == 2) {
                    r.set_max_number_of_tokens(3);
                  }
                  r.set_display_name("A dummy resource");
                  r.set_terse_description("This is a mock Vire resource");
                  //  ??
                  if (iboard == 2) {
                     r.grab_auxiliaries().store("aux.snemo.role_tag.exclusive", "perso_jh");
                  }
                  //  ??
                  r.initialize_simple();
                  mgr_.add_resource(r);
                }

                {
                  std::ostringstream path_oss;
                  path_oss << explabel << ':'
                           << "/Demonstrator/CMS/"
                           << *ipart
                           << '/' << "HV" << '/'
                           << "crate_" << icrate << '/'
                           << "board_" << iboard << '/'
                           << "ch_" << ichannel << '/'
                           << "setpoint_voltage";
                  vire::resource::resource r;
                  r.set_id(resource_id++);
                  r.set_path(path_oss.str());
                  std::ostringstream name_oss;
                  name_oss << explabel << "__"
                           << '.' << *ipart
                           << '.' << "HV" << '.'
                           << "crate_" << icrate << '.'
                           << "board_" << iboard << '.'
                           << "ch_" << ichannel << '.'
                           << "setpoint_voltage";
                  r.set_name(name_oss.str());
                  r.set_access(vire::utility::RW_WRITABLE);
                  // r.set_max_number_of_tokens(1);
                  r.set_display_name("A dummy resource");
                  r.set_terse_description("This is a mock Vire resource");
                  r.initialize_simple();
                  mgr_.add_resource(r);
                }
              }
            }
          }
        }

        // Create some mock roles:
        {
          vire::resource::role tracko_hv1;
          tracko_hv1.set_name("tracko_hv1");
          tracko_hv1.set_resource_manager(mgr_);
          tracko_hv1.set_logging_priority(mgr_.get_logging_priority());
          tracko_hv1.set_path("SuperNEMO:/Demonstrator/CMS/tracker/HV1");
          tracko_hv1.add_allowed_user(1034);
          tracko_hv1.add_allowed_user(1035);
          {
            auto ers = new vire::resource::enumerated_resource_selector;
            ers->set_resource_mgr(mgr_);
            ers->add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_0/board_0/ch_0/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_0/board_0/ch_2/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_0/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_2/ch_0/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_2/ch_1/current");
            ers->initialize_simple();
            cuts::cut_handle_type hselector(ers);
            tracko_hv1.set_functional_resource_selector_handle(hselector);
          }
          // std::clog << "DEVEL: vire::resource::testing::populate_manager: path='"
          //           << tracko_hv1.get_path() << "'" << std::endl;
          tracko_hv1.initialize_simple();
          tracko_hv1.build_cached_functional_resource_ids();
          tracko_hv1.build_cached_distributable_resource_ids();
          tracko_hv1.build_cached_scope_resource_ids();
          tracko_hv1.tree_dump(std::clog, "Role 'tracko_hv1' : ");
          mgr_.add_role(tracko_hv1);
        }

        {
          vire::resource::role calo_hv1;
          calo_hv1.set_name("calo_hv1");
          calo_hv1.set_resource_manager(mgr_);
          calo_hv1.set_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV1");
          calo_hv1.add_allowed_user(1034);
          calo_hv1.add_allowed_user(1035);
          calo_hv1.add_allowed_group(1234);
          {
            auto ers = new vire::resource::enumerated_resource_selector;
            ers->set_resource_mgr(mgr_);
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_0/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_2/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_1/ch_0/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_1/ch_1/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_1/ch_2/current");
            ers->initialize_simple();
            cuts::cut_handle_type hselector(ers);
            calo_hv1.set_functional_resource_selector_handle(hselector);
          }
          {
            auto ers = new vire::resource::enumerated_resource_selector;
            ers->set_resource_mgr(mgr_);
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_3/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_1/ch_3/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_3/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_1/ch_3/current");
            ers->initialize_simple();
            cuts::cut_handle_type hselector(ers);
            calo_hv1.set_distributable_resource_selector_handle(hselector);
          }
          calo_hv1.initialize_simple();
          calo_hv1.tree_dump(std::clog, "Role 'calo_hv1' : ");
          std::clog << "DEVEL: path='" << calo_hv1.get_path() << "'" << std::endl;
          mgr_.add_role(calo_hv1);
        }

        {
          vire::resource::role calo_hv2;
          calo_hv2.set_name("calo_hv2");
          calo_hv2.set_resource_manager(mgr_);
          calo_hv2.set_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV2");
          calo_hv2.add_allowed_user(1234);
          calo_hv2.add_allowed_user(1235);
          calo_hv2.add_allowed_group(1109);
          calo_hv2.add_allowed_group(1113);
          {
            auto ers = new vire::resource::enumerated_resource_selector;
            ers->set_resource_mgr(mgr_);
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_0/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_1/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_2/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_3/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_0/setpoint_voltage");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_1/setpoint_voltage");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_2/setpoint_voltage");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_3/setpoint_voltage");
            ers->initialize_simple();
            cuts::cut_handle_type hselector(ers);
            calo_hv2.set_functional_resource_selector_handle(hselector);
          }
          {
            auto ers = new vire::resource::enumerated_resource_selector;
            ers->set_resource_mgr(mgr_);
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_0/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_1/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_2/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_3/current");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_0/setpoint_voltage");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_1/setpoint_voltage");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_2/setpoint_voltage");
            ers->add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_3/setpoint_voltage");
            ers->initialize_simple();
            cuts::cut_handle_type hselector(ers);
            calo_hv2.set_distributable_resource_selector_handle(hselector);
          }
          calo_hv2.initialize_simple();
          calo_hv2.tree_dump(std::clog, "Role 'calo_hv2' : ");
          std::clog << "DEVEL: path='" << calo_hv2.get_path() << "'" << std::endl;
          mgr_.add_role(calo_hv2);
        }

        return;
      }

    } // namespace testing

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_TESTING_POPULATE_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
