// test-resource-resource.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <map>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>

void test_resource_1();
void test_resource_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::resource' class."
              << std::endl;

    test_resource_1();
    // test_resource_2();

    std::clog << "The end." << std::endl;
  }  catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_resource_1()
{
  std::clog << "\ntest_resource_1: basics" << std::endl;

  typedef std::map<std::string, vire::resource::resource> res_dict_type;

  res_dict_type rDict;
  rDict["A1"] = vire::resource::resource();
  rDict["A2"] = vire::resource::resource();
  rDict["B"] = vire::resource::resource();
  rDict["Z"] = vire::resource::resource();
  rDict["C"] = vire::resource::resource();
  rDict["D"] = vire::resource::resource();

  vire::resource::resource & rA1 = rDict["A1"];
  rA1.set_name("A1");
  rA1.set_id(1230);
  rA1.set_path("Test:/A/B/C/D/_x1");
  rA1.set_access(vire::utility::RW_WRITABLE);
  rA1.set_max_number_of_tokens(1);
  rA1.set_display_name("_x1 action from Test:/A/B/C/D object");
  rA1.set_terse_description("This is a mock Vire resource");
  rA1.grab_auxiliaries().store_flag("mock");
  rA1.initialize_simple();
  rA1.tree_dump(std::clog, rA1.get_name());
  std::clog << std::endl;

  vire::resource::resource & rA2 = rDict["A2"];
  rA2.set_name("A2");
  rA2.set_id(1231);
  rA2.set_path("Test:/A/B/C/D/_x2");
  rA2.set_access(vire::utility::RW_WRITABLE);
  rA2.set_max_number_of_tokens(1);
  rA2.set_display_name("_x2 action from Test:/A/B/C object");
  rA2.set_terse_description("This is a mock Vire resource");
  rA2.grab_auxiliaries().store_flag("mock");
  rA2.initialize_simple();
  rA2.tree_dump(std::clog, rA2.get_name());
  std::clog << std::endl;

  vire::resource::resource & rB = rDict["B"];
  rB.set_name("B");
  rB.set_id(1234);
  rB.set_path("Test:/A/B/C/D");
  rB.set_access(vire::utility::RW_WRITABLE);
  rB.set_max_number_of_tokens(1);
  rB.set_display_name("D action from Test:/A/B/C object");
  rB.set_terse_description("This is a mock Vire resource");
  rB.grab_auxiliaries().store_flag("mock");
  rB.add_dependee(rA1);
  rB.add_dependee(rA2);
  rB.initialize_simple();

  vire::resource::resource & rZ = rDict["Z"];
  rZ.set_name("Z");
  rZ.set_id(1236);
  rZ.set_path("Test:/A/Z/C/D");
  rZ.set_access(vire::utility::RW_READABLE);
  rZ.set_max_number_of_tokens(3);
  rZ.set_display_name("D action from Test:/A/Z/C object");
  rZ.set_terse_description("This is a mock Vire resource");
  rZ.grab_auxiliaries().store_flag("mock");
  rZ.initialize_simple();

  vire::resource::resource & rC = rDict["C"];
  rC.set_name("C");
  rC.set_id(1238);
  rC.set_path("Test:/A/B/C/x");
  rC.set_access(vire::utility::RW_READABLE);
  rC.set_max_number_of_tokens(3);
  rC.set_display_name("x action from Test:/A/B/C object");
  rC.set_terse_description("This is a mock Vire resource");
  rC.grab_auxiliaries().store_flag("mock");
  rC.add_dependee(rB);
  rC.initialize_simple();

  vire::resource::resource & rD = rDict["D"];
  rD.set_name("D");
  rD.set_id(1239);
  rD.set_path("Test:/A/B/C/y");
  rD.set_access(vire::utility::RW_READABLE);
  rD.set_max_number_of_tokens(3);
  rD.set_display_name("y action from Test:/A/B/C object");
  rD.set_terse_description("This is a mock Vire resource");
  rD.grab_auxiliaries().store_flag("mock");
  rD.add_dependee(rB);
  rD.add_dependee(rZ);
  rD.initialize_simple();

  std::clog << "\nBuild dependency cache: " << std::endl;
  for (res_dict_type::iterator iRes = rDict.begin();
       iRes != rDict.end();
       iRes++) {
    iRes->second.build_cached_dependee_resource_ids();
    iRes->second.build_cached_depender_resource_ids();
  }

  std::clog << "\nList of resources: " << std::endl;
  for (res_dict_type::const_iterator iRes = rDict.begin();
       iRes != rDict.end();
       iRes++) {
    iRes->second.tree_dump(std::clog, iRes->second.get_name());
    std::clog << std::endl;
  }

  rDict.erase("D");

  std::clog << "\nList of resources (without D): " << std::endl;
  for (res_dict_type::const_iterator iRes = rDict.begin();
       iRes != rDict.end();
       iRes++) {
    iRes->second.tree_dump(std::clog, iRes->second.get_name());
    std::clog << std::endl;
  }

  rDict.erase("B");

  std::clog << "\nList of resources (without B): " << std::endl;
  for (res_dict_type::const_iterator iRes = rDict.begin();
       iRes != rDict.end();
       iRes++) {
    iRes->second.tree_dump(std::clog, iRes->second.get_name());
    std::clog << std::endl;
  }

  return;
}

void test_resource_2()
{

  std::clog << "\ntest_resource_2: basics" << std::endl;
  std::map<int32_t, vire::resource::resource> resources;

  for (int i = 0; i < 10; i++) {
    int32_t id = 1234 + i;
    {
      vire::resource::resource new_res;
      resources[id] = new_res;
    }
    vire::resource::resource & r = resources.find(id)->second;
    r.set_id(id);
    std::ostringstream path_oss;
    path_oss << "/A/B/C/D" << i;
    std::ostringstream name_oss;
    name_oss << "A.B.C.D" << i;
    r.set_path(path_oss.str());
    if (i < 3) {
      r.set_max_number_of_tokens(vire::resource::resource::MAX_TOKENS_SINGLETON);
    } else if (i < 6) {
      r.set_max_number_of_tokens(3 + i);
    } else {
      r.set_max_number_of_tokens(vire::resource::resource::MAX_TOKENS_UNLIMITED);
    }
    r.set_access(vire::utility::RW_WRITABLE);
    r.set_name(name_oss.str());
    std::ostringstream display_name_oss;
    display_name_oss << "D"  << i << " action from A/B/C object";
    r.set_display_name(display_name_oss.str());
    r.set_terse_description("This is a mock Vire resource");
    r.grab_auxiliaries().store_flag("mock");
    r.initialize_simple();
    r.tree_dump(std::clog, r.get_display_name());
    std::clog << std::endl;
  }
  std::clog << std::endl;
  return;
}
