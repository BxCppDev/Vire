// populate_manager.h

#ifndef VIRE_USER_TESTING_POPULATE_MANAGER_H
#define VIRE_USER_TESTING_POPULATE_MANAGER_H

// Standard Library
#include <string>
#include <vector>
#include <sstream>

// This project:
#include <vire/user/manager.h>
// #include <vire/user/user.h>
// #include <vire/user/group.h>

namespace vire {

  namespace user {

    namespace testing {

      void populate_manager(vire::user::manager & mgr_,
                            uint32_t flags_ = 0);

      void populate_manager(vire::user::manager & mgr_,
                            uint32_t /* flags_ */)
      {
        // Create some mock users:
        mgr_.create_new_user_by_name("admin", 0);

        mgr_.create_new_user_by_name("nemo", 1000);
        mgr_.create_new_user_by_name("nemoadmin", 1001);
        mgr_.create_new_user_by_name("nemoprod", 1002);
        mgr_.create_new_user_by_name("expert", 1010);
        mgr_.create_new_user_by_name("fifi", 1034);
        mgr_.create_new_user_by_name("loulou", 1035);
        mgr_.create_new_user_by_name("riri", 1234);
        mgr_.create_new_user_by_name("johndoe", 1235);

        // Create some mock groups:
        mgr_.create_new_group_by_name("admin", 0);
        mgr_.create_new_group_by_name("gnemo", 1000);
        mgr_.create_new_group_by_name("gnemoadmin", 1001);
        mgr_.create_new_group_by_name("gnemoprod", 1002);
        mgr_.create_new_group_by_name("foo", 1234);
        mgr_.create_new_group_by_name("bar", 1109);
        mgr_.create_new_group_by_name("staff", 1113);
        mgr_.create_new_group_by_name("visitor", 1200);

        // Add users in groups:
        mgr_.add_user_in_group(0, 0);

        mgr_.add_user_in_group(1000, 1000);
        mgr_.add_user_in_group(1001, 1000);
        mgr_.add_user_in_group(1002, 1000);
        mgr_.add_user_in_group(1010, 1000);
        mgr_.add_user_in_group(1034, 1000);
        mgr_.add_user_in_group(1035, 1000);
        mgr_.add_user_in_group(1234, 1000);

        mgr_.add_user_in_group(1001, 1001);
        mgr_.add_user_in_group(1010, 1001);

        mgr_.add_user_in_group(1002, 1002);
        mgr_.add_user_in_group(1010, 1002);
        mgr_.add_user_in_group(1234, 1002);

        mgr_.add_user_in_group(1034, 1113);
        mgr_.add_user_in_group(1035, 1113);
        mgr_.add_user_in_group(1234, 1113);

        mgr_.add_user_in_group(1034, 1234);
        mgr_.add_user_in_group(1035, 1234);

        mgr_.add_user_in_group(1035, 1109);

        mgr_.add_user_in_group(1235, 1200);

        return;
      }

    } // namespace testing

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_TESTING_POPULATE_MANAGER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
