//! \file  vire/protobuf/testing/test-protobuf-ModelIdentifier.cxx
//! \brief Protobuf support Vire library
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <fstream>

// Third party:
#include <google/protobuf/stubs/common.h>
#include <datatools/version_id.h>

// This project:
#include <vire/vire.h>
#include <vire/version.h>
#include <vire/protobuf_utils.h>
#include <vire/utility/model_identifier.h>

#include <vire/utility/BaseIdentifier.pb.h>
#include <vire/utility/ModelIdentifier.pb.h>

void test_1();
void test_2();

namespace vire {

  template<typename Type, typename PbMessage>
  class converter;

  template<>
  class converter<vire::utility::model_identifier,
                  vire::utility::ModelIdentifier>
  {
  public:

    static void to_protobuf(const vire::utility::model_identifier & obj_,
                            vire::utility::ModelIdentifier & msg_)
    {
      VIRE_PROTOBUF_BASE_REF(msg_).set_name(obj_.get_name());
      if (obj_.has_version()) {
        VIRE_PROTOBUF_BASE_REF(msg_).set_version(obj_.get_version().to_string());
      }
      return;
    }

    static void from_protobuf(vire::utility::model_identifier & obj_,
                              const vire::utility::ModelIdentifier & msg_)
    {
      obj_.reset();
      obj_.set_name(VIRE_PROTOBUF_BASE_REF_CONST(msg_).name());
      const std::string & versionRepr = VIRE_PROTOBUF_BASE_REF_CONST(msg_).version();
      if (!versionRepr.empty()) {
        datatools::version_id verId;
        verId.from_string(versionRepr);
        obj_.set_version(verId);
      }
      return;
    }

  };

} // end of vire

template<class Obj, class Msg>
void operator>>(const Obj & obj_, Msg & msg_)
{
  vire::converter<Obj, Msg>::to_protobuf(obj_, msg_);
  return;
}

template<class Obj, class Msg>
void operator<<(Obj & obj_, const  Msg & msg_)
{
  vire::converter<Obj, Msg>::from_protobuf(obj_, msg_);
  return;
}

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the Protocol Buffer library."
              << std::endl;

    test_1();
    test_2();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  google::protobuf::ShutdownProtobufLibrary();
  vire::terminate();
  return (error_code);
}

void test_1()
{
  std::clog << "\ntest_1: " << std::endl;
  std::clog << "GOOGLE_PROTOBUF_VERSION = " << GOOGLE_PROTOBUF_VERSION << std::endl;
  {
    vire::utility::ModelIdentifier typeId;
    // vire::utility::BaseIdentifier * baseId = typeId.mutable__base();
    // baseId->set_name("vire::utility::ModelIdentifier");
    // baseId->set_version("1.0");
    VIRE_PROTOBUF_BASE_REF(typeId).set_name("vire::utility::ModelIdentifier");
    VIRE_PROTOBUF_BASE_REF(typeId).set_version("1.0");
    std::clog << "Type ID: \n" << typeId.DebugString() << std::endl;
    std::ofstream fout("test-protobuf-ModelIdentifier_1.pb");
    typeId.SerializeToOstream(&fout);
  }

  {
    vire::utility::ModelIdentifier typeId;
    std::ifstream fin("test-protobuf-ModelIdentifier_1.pb");
    typeId.ParseFromIstream(&fin);
    std::clog << "Deserialized type ID: \n" << typeId.DebugString() << std::endl;
    std::clog << "Base: \n" << VIRE_PROTOBUF_BASE_REF_CONST(typeId).DebugString() << std::endl;
  }

  return;
}

void test_2()
{
  std::clog << "\ntest_2: " << std::endl;
  vire::utility::model_identifier typeId;
  typeId.set_name("foo::bar");
  typeId.set_version(datatools::version_id(3, 1415, 0, "alpha"));
  std::clog << "Type ID: " << typeId.to_string() << std::endl;
  {
    vire::utility::ModelIdentifier typeId_Pb;
    vire::converter<vire::utility::model_identifier,
                    vire::utility::ModelIdentifier>::to_protobuf(typeId, typeId_Pb);
    std::clog << "1) Protobuf type ID : \n" << typeId_Pb.DebugString() << std::endl;
  }

  {
    vire::utility::ModelIdentifier typeId_Pb;
    typeId >> typeId_Pb;
    std::clog << "2) Protobuf type ID : \n" << typeId_Pb.DebugString() << std::endl;

    vire::utility::model_identifier typeId2;
    typeId2 << typeId_Pb;
    std::clog << "Type ID 2: " << typeId2.to_string() << std::endl;
  }

  return;
}
