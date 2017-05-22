//! \file utility/testing/test-utility-base_payload.cxx
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
#include <iostream>
#include <vector>
#include <memory>

// Third party:
// - Boost
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
// - Bayeux/datatools
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/io_factory.h>
#include <bayeux/datatools/archives_instantiation.h>

// This project:
#include <vire/utility/base_payload.h>
#include <vire/utility/base_event.h>
#include <vire/utility/base_alarm.h>
#include <vire/time/utils.h>

namespace vire {
  namespace utility {
    namespace testing {
      // Declare a new concrete event class inherited from the base_event abstract class:
      struct dummy_event : public vire::utility::base_event
      {
      public:
        dummy_event() {}
        explicit dummy_event(int foo_) : vire::utility::base_event(vire::time::now()) { set_foo(foo_); }
        dummy_event(const boost::posix_time::ptime & timestamp_, int foo_) : vire::utility::base_event(timestamp_) { set_foo(foo_); }
        virtual ~dummy_event() {}
        void set_foo(int foo_) { _foo_ = foo_;}
        int get_foo() const { return _foo_; }
        //! Smart print
        virtual void tree_dump(std::ostream & out_ = std::clog,
                               const std::string & title_  = "",
                               const std::string & indent_ = "",
                               bool inherit_ = false) const {
          this->base_event::tree_dump(out_, title_, indent_, true);
          out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
               << "Foo : [" << _foo_ << "]" << std::endl;
          return;
        }
      private:
        int32_t _foo_;

        //! Support for Boost-based serialization
        DATATOOLS_SERIALIZATION_DECLARATION()

        //! Support for cloneable interface
        DATATOOLS_CLONEABLE_DECLARATION(dummy_event)
      };

      DATATOOLS_SERIALIZATION_IMPLEMENTATION(dummy_event,"vire::utility::testing::dummy_event")
      DATATOOLS_CLONEABLE_IMPLEMENTATION(vire::utility::testing::dummy_event)

      template<class Archive>
      void dummy_event::serialize (Archive & archive_, const unsigned int /* version_*/)
      {
        archive_ & boost::serialization::make_nvp("vire__utility__base_event",
                                                  boost::serialization::base_object<vire::utility::base_event>(*this));
        archive_ & boost::serialization::make_nvp("foo", _foo_);
        return;
      }
    }
  }
}
#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(vire::utility::testing::dummy_event,"vire::utility::testing::dummy_event")
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::utility::testing::dummy_event)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::utility::testing::dummy_event)

void test_base_payload_1();
void test_base_payload_2();

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    test_base_payload_1();
    test_base_payload_2();

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void test_base_payload_1()
{
  std::clog << "\ntest_base_payload_1: basics..." << std::endl;

  {
    // Dummy event:
    vire::utility::testing::dummy_event bar;
    bar.set_timestamp(vire::time::now());
    bar.set_foo(42);
    std::clog << "Event 'bar': foo=" << bar.get_foo() << std::endl;
    std::clog << "Category = " << bar.get_category() << std::endl;
    std::clog << "Category = '" << bar.get_category_label() << "'" << std::endl;
    bar.tree_dump(std::clog, "Bar: ");
    std::clog << std::endl;
    {
      std::clog << "test_base_payload_1: Serializing the payload..." << std::endl;
      datatools::data_writer writer("test-utility-base_payload.xml");
      writer.store(bar);
    }
  }

  {
    vire::utility::testing::dummy_event bar;
    {
      std::clog << "test_base_payload_1: Deserializing the payload..." << std::endl;
      datatools::data_reader reader("test-utility-base_payload.xml");
      reader.load(bar);
    }
    bar.tree_dump(std::clog, "Loaded Bar: ");
    std::clog << std::endl;
  }

  return;
}

void test_base_payload_2()
{
  std::clog << "\ntest_base_payload_2: advanced..." << std::endl;

  typedef std::shared_ptr<vire::utility::base_payload> payload_ptr;
  {
    std::vector<payload_ptr> payloads;
    payloads.push_back(std::make_shared<vire::utility::testing::dummy_event>(12));
    payloads.push_back(std::make_shared<vire::utility::testing::dummy_event>(42));
    payloads.push_back(std::make_shared<vire::utility::base_alarm>("warning", "A generic alarm"));
    payloads.push_back(std::make_shared<vire::utility::testing::dummy_event>(666));
    payloads.push_back(std::make_shared<vire::utility::base_alarm>("error", "Another generic alarm"));

    for (const auto pl : payloads) {
      pl->tree_dump(std::clog, "Payload: ");
    }
    {
      std::clog << "test_base_payload_2: Serializing the payloads..." << std::endl;
      datatools::data_writer writer("test-utility-base_payload_2.xml");
      writer.store("std::vector<std::shared_ptr<vire::utility::base_payload>>>", payloads);
    }
    std::clog << std::endl;
  }

  {
    std::vector<payload_ptr> payloads;
    {
      std::clog << "test_base_payload_2: Deserializing the payloads..." << std::endl;
      datatools::data_reader reader("test-utility-base_payload_2.xml");
      reader.load("std::vector<std::shared_ptr<vire::utility::base_payload>>>", payloads);
    }
    for (const auto pl : payloads) {
      pl->tree_dump(std::clog, "Payload: ");
    }
    std::clog << std::endl;
  }

  return;
}
