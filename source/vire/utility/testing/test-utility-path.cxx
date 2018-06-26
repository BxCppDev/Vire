// test-utility-path.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// This project:
#include <vire/vire.h>
#include <vire/utility/path.h>

void test_path_1();
void test_path_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::utility::path' class."
              << std::endl;

    test_path_1();
    test_path_2();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_path_1()
{
  std::clog << "\ntest_path_1: basics" << std::endl;

  std::clog << std::endl;
  std::clog << "Building path: " << std::endl;
  std::clog << std::endl;

  {
    std::string setup("SuperNEMO");
    std::vector<std::string> segments({
        "Demonstrator",
          "CMS",
          "Coil",
          "Control",
          "Voltage",
          });
    std::string leaf("__dp_read__");
    std::string path = vire::utility::path::build(setup, segments);
    std::clog << "Path 1 = '" << path << "'" << std::endl;
  }

  {
    std::string setup("SuperNEMO");
    std::string dirs("/Demonstrator/CMS/Coil/Control/Voltage");
    std::string leaf("__dp_read__");
    std::string path = vire::utility::path::build(setup, dirs, leaf);
    std::clog << "Path 2 = '" << path << "'" << std::endl;
  }

  try {
    std::string setup("SuperNEMO");
    std::string dirs("/Demonstrator/CMS/Coil/Control/Current/");
    std::string leaf("__dp_write__");
    std::string path = vire::utility::path::build(setup, dirs, leaf);
    std::clog << "Path 3 = '" << path << "'" << std::endl;
  } catch (std::exception & x) {
    std::cerr << "[error] As expected 3 : " << x.what() << std::endl;
  }


  try {
    std::string setup("SuperNEMO");
    std::string dirs("Demonstrator/CMS/Coil/Control/Current");
    std::string leaf("__dp_write__");
    std::string path = vire::utility::path::build(setup, dirs, leaf);
    std::clog << "Path 4 = '" << path << "'" << std::endl;
  } catch (std::exception & x) {
    std::cerr << "[error] As expected 4 : " << x.what() << std::endl;
  }

  try {
    std::string setup("SuperNEMO");
    std::string dirs("Demonstrator/CMS/Coil/Monitoring/Current");
    std::string leaf("__dp_read__");
    std::string path = vire::utility::path::build(setup, dirs, leaf);
    std::clog << "Path 5 = '" << path << "'" << std::endl;
  } catch (std::exception & x) {
    std::cerr << "[error] As expected 5 : " << x.what() << std::endl;
  }

  {
    std::string setup("SuperNEMO");
    std::string dirs("/");
    std::string leaf("pause");
    std::string path = vire::utility::path::build(setup, dirs, leaf);
    std::clog << "Path 6 = '" << path << "'" << std::endl;
  }

  try {
    std::string setup("SuperNEMO");
    std::string dirs("");
    std::string leaf("stop");
    std::string path = vire::utility::path::build(setup, dirs, leaf);
    std::clog << "Path 7 = '" << path << "'" << std::endl;
  } catch (std::exception & x) {
    std::cerr << "[error] As expected 7: " << x.what() << std::endl;
  }

  try {
    std::string setup("");
    std::string dirs("Demonstrator/CMS/Coil/Monitoring/Current");
    std::string leaf("__dp_read__");
    std::string path = vire::utility::path::build(setup, dirs, leaf);
    std::clog << "Path 8 = '" << path << "'" << std::endl;
  } catch (std::exception & x) {
    std::cerr << "[error] As expected: " << x.what() << std::endl;
  }

  std::clog << std::endl;
  std::clog << "Extracting path: " << std::endl;
  std::clog << std::endl;
  {
    std::vector<std::string> paths = {
      "",
      ":/a/b/c",
      "a/b/c/",
      "SuperNEMO/test",
      "SuperNEMO:resume",
      "SuperNEMO:",
      "SuperNEMO:/",
      "SuperNEMO:/resume",
      "SuperNEMO:/Demonstrator/",
      "SuperNEMO:/Demonstrator/CMS/calorimeter/HV/expert_action",
    };
    for (const auto & path : paths) {
      std::string setup;
      std::string dirs;
      std::string leaf;
      if (vire::utility::path::extract(path,
                                       setup,
                                       dirs,
                                       leaf)) {
        std::cerr << "Extracted from resource path '" << path << "'!" << std::endl;
        std::clog << " - Setup  = '" << setup << "'" << std::endl;
        std::clog << " - Dirs   = '" << dirs << "'" << std::endl;
        std::clog << " - Leaf   = '" << leaf << "'" << std::endl;
        std::clog << std::endl;
      } else {
        std::cerr << "[error] As expected: Cannot extract resource path from '" << path << "'!" << std::endl;
      }
    }
    std::clog << std::endl;
  }

  std::clog << std::endl;
  std::clog << "Path validation: " << std::endl;
  std::clog << std::endl;
  {
    std::vector<std::string> paths = {
      "",
      "://a/b/c",
      "a/b/c/",
      "SuperNEMO://resume",
      "SuperNEMO:/Demonstrator//Foo",
      "SuperNEMO://Demonstrator/CMS/calorimeter/HV/expert_action",
    };
    for (auto path : paths) {
      if (vire::utility::path::validate_path(path)) {
        std::clog << "Path '" << path << "' is valid." << std::endl;
      } else {
        std::clog << "Path '" << path << "' is not valid." << std::endl;
      }
    }
    std::clog << std::endl;
  }

  std::clog << std::endl;
  std::clog << "Path to address conversion: " << std::endl;
  std::clog << std::endl;
  {
    std::vector<std::string> paths = {
      "",
      "://a/b/c",
      "/a/b/c/",
      "SuperNEMO:/resume",
      "SuperNEMO:/Demonstrator/",
      "SuperNEMO:/Demonstrator/CMS/calorimeter/start",
      "SuperNEMO:/Demonstrator/CMS/calorimeter/Status/__dp_read__",
      "SuperNEMO:/Demonstrator/CMS/calorimeter/HV/expert_action",
    };
    for (auto path : paths) {
      try {
        std::string addr = vire::utility::path::to_address(path);
        std::clog << "Path '" << path << "' : \n  ==> Address = [" << addr << "]"  << std::endl;
      } catch (std::exception & x) {
        std::cerr << "[error] As expected: \n" << x.what() << std::endl;
      }
    }
    std::clog << std::endl;
  }

  std::clog << std::endl;
  std::clog << "Address validation and conversion: " << std::endl;
  std::clog << std::endl;
  {
    std::vector<std::string> addresses = {
      "",
      ".a.b.c",
      "a.b.c.",
      "SuperNEMO.resume",
      "SuperNEMO.Demonstrator.",
      "SuperNEMO.Demonstrator.CMS.calorimeter.HV.expert_action",
    };
    for (auto addr : addresses) {
      if (vire::utility::path::validate_address(addr)) {
        std::clog << "Address '" << addr << "' is valid." << std::endl;
        std::string path = vire::utility::path::from_address(addr);
        std::clog << "  with path '" << path << "'" << std::endl;
      } else {
        std::clog << "Address '" << addr << "' is not valid." << std::endl;
      }
    }
    std::clog << std::endl;
  }

  std::clog << std::endl;
  std::clog << "Parent path: " << std::endl;
  std::clog << std::endl;
  {
    std::vector<std::string> ppaths = {
      "SuperNEMO:",
      "SuperNEMO:/",
      //"SuperNEMO://",
      "SuperNEMO:/Test",
      "Atlas:/Phase1",
      "SuperNEMO:/Demonstrator",
      "SuperNEMO:/Demonstrator/CMS",
      //"SuperNEMO:/Demonstrator//CMS",
      "SuperNEMO:/Demonstrator/CMS/calorimeter",
      "SuperNEMO:/Demonstrator/CMS/calorimeter/HV",
      "SuperNEMO:/Demonstrator/Test",
      "SuperNEMO:/Demonstrator/CMS/calorimeter/HV/expert_action",
      "SuperNEMO:/Demonstrator/CMS/calorimeter/HV/expert_action/foo",
    };
    std::string cpath = "SuperNEMO:/Demonstrator/CMS/calorimeter/HV/expert_action";
    for (auto ppath : ppaths) {
      if (vire::utility::path::validate_path(ppath)) {
        std::clog << "Path '" << ppath << "' is valid." << std::endl;
      }
      if (vire::utility::path::is_root(ppath)) {
        std::clog << "Path '" << ppath << "' is root." << std::endl;
      }
    }
    std::clog << std::endl;
    for (auto ppath : ppaths) {
      if (vire::utility::path::validate_path(ppath) &&
          vire::utility::path::is_parent_of(ppath, cpath)) {
        std::clog << "[info] Path '" << ppath << "' is parent\n       of   '" << cpath << "'" << std::endl;
        if (vire::utility::path::is_parent_of(ppath, cpath, true)) {
          std::clog << "[info] Path '" << ppath << "' is direct parent\n       of   '" << cpath << "'" << std::endl;
        }
      } else {
        std::clog << "[error] Path '" << ppath << "' is NOT parent\n        of   '" << cpath << "'" << std::endl;
      }
      std::clog << std::endl;
    }
    std::clog << std::endl;
  }
  return;
}

void test_path_2()
{
  std::clog << "\ntest_path_2: more" << std::endl;

  vire::utility::path p1;
  p1.from_string("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV");
  std::clog << "Path p1 =  '" << p1 << "'" << std::endl;

  vire::utility::path p2;
  p2.from_string("SuperNEMO:/Demonstrator/CMS/Calorimeter/HV/Foo/Bar");
  std::clog << "Path p2 =  '" << p2 << "'" << std::endl;
                 
  bool direct = false;
  // direct = true;
  if (p1.is_parent_of(p2, direct)) {
    std::clog << "Path p1 is parent of p2" << std::endl;
  } else {
    std::clog << "Path p1 is not parent of p2" << std::endl;
  }

  if (p2.is_child_of(p1, direct)) {
    std::clog << "Path p2 is child of p1" << std::endl;
  } else {
    std::clog << "Path p2 is not child of p1" << std::endl;
  }

  vire::utility::relative_path p12 = p2.subtract(p1);
  std::clog << "Relative path p12 =  '" << p12 << "'" << std::endl;
  
  vire::utility::path p3;
  p3.from_string("SuperNEMO:/Demonstrator/CMS/Doo");
  std::clog << "Path p3 =  '" << p3 << "'" << std::endl;
  p3.append(p12);
  std::clog << "Path p3 =  '" << p3 << "' (append)" << std::endl;
  
  std::clog << std::endl;
  return;
}
