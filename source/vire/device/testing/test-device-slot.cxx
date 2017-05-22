//! \file device/testing/test-device-slot.cxx
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

// Ourselves:
#include <vire/device/slot.h>

// Standard library:
#include <iostream>

// Third party:
// - Camp:
#include <camp/enum.hpp>
#include <camp/class.hpp>
#include <camp/args.hpp>
#include <camp/value.hpp>
#include <camp/userobject.hpp>
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/exception.h>
// - Bayeux/geomtools:
#include <geomtools/utils.h>

// This project:
#include <vire/device/space.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    bool interactive = false;

    {
      // Invalid slot:
      vire::device::slot s;
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // Slot without dimension:
      vire::device::slot s;
      s.make_dimension_0(true);
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // Invalid slot without dimension:
      vire::device::slot s;
      s.make_dimension_0(false);
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // 1D Slot:
      vire::device::slot s;
      s.make_dimension_1(3);
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // 1D Slot:
      vire::device::slot s;
      s.make_dimension_1(vire::device::space::COORDINATE_ANY);
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // 1D Slot:
      vire::device::slot s;
      s.make_dimension_1(vire::device::space::COORDINATE_NA);
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // 2D Slot:
      vire::device::slot s;
      s.make_dimension_2(3, 7);
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // 3D Slot:
      vire::device::slot s;
      s.make_dimension_3(3, vire::device::space::COORDINATE_ANY, 2);
      s.tree_dump(std::clog, "Slot: ");
      std::clog << "Slot = " << s << std::endl;
      std::clog << std::endl;
    }

    {
      // Parse slot:
      vire::device::slot s;
      std::string line;
      if (interactive) {
        std::clog << "Input slot : ";
        std::getline(std::cin, line);
      } else {
        line = "(2:2,3)";
      }
      if (s.from_string(line)) {
        s.tree_dump(std::clog, "Slot: ");
        std::clog << "Parsed slot = " << s << std::endl;
      } else {
        std::cerr << "ERROR: Invalid slot format '" << line << "'!" << std::endl;
      }
      std::clog << std::endl;
    }

    {
      std::clog << "NOTICE: Reflection..." << std::endl;
      const camp::Class & spaceMetaClass = camp::classByName("vire::device::space");
      camp::detail::EnumManager & enumMgr = camp::detail::EnumManager::instance();
      std::clog << "Enumeration manager has [" << enumMgr.count() << "] enumeration(s)" << std::endl;
      for (int i = 0; i < (int) enumMgr.count(); i++) {
        std::clog << " - Enumeration #" << i << " : " << "'" << enumMgr.getByIndex(i).name() << "'" << std::endl;
      }
      std::clog << "Type = '" << camp::detail::StaticTypeId<datatools::logger::priority>::get() << "'" << std::endl;
      std::clog << "Type = '" << camp::detail::StaticTypeId<geomtools::direction_type>::get() << "'" << std::endl;
      std::clog << "Type = '" << camp::detail::StaticTypeId<vire::device::space::dimension_type>::get() << "'" << std::endl;

      if (! enumMgr.enumExists("::vire::device::space::dimension_type")) {
        std::cerr << "ERROR: No enumeration named '" << "vire::device::space::dimension_type" << "'!" << std::endl;
      } else {
        const camp::Enum & spaceDimMetaEnum = camp::enumByName("vire::device::space::dimension_type");
        for (int i = 0; i < (int) spaceDimMetaEnum.size(); i++) {
          std::clog << " - Key '" << spaceDimMetaEnum.pair(i).name
                    << "' has value = " << spaceDimMetaEnum.pair(i).value << std::endl;
        }

        const camp::Class & slotMetaClass = camp::classByName("vire::device::slot");
        std::clog << "Slot meta class: " << "'" << slotMetaClass.name() << "'" << std::endl;
        std::clog << "  Methods: " << "[" << slotMetaClass.functionCount() << "]" << std::endl;
        for (int i = 0; i < (int) slotMetaClass.functionCount(); i++) {
          std::clog << "  - Method #" << i << " : " << "'" << slotMetaClass.function(i).name() << "'" << std::endl;
        }

        {
          long dim = spaceDimMetaEnum.value("vire::device::space::DIM_2");
          camp::EnumObject spaceDimObj(vire::device::space::DIM_2);
          boost::scoped_ptr<vire::device::slot> slotHandle;
          camp::UserObject slotObj = slotMetaClass.construct(camp::Args(vire::device::space::DIM_2,
                                                                        2,
                                                                        3,
                                                                        vire::device::space::COORDINATE_INVALID));
          slotObj = slotMetaClass.construct(camp::Args(spaceDimObj,
                                                       camp::Value(2),
                                                       camp::Value(3),
                                                       camp::Value(vire::device::space::COORDINATE_INVALID)));
          slotObj.call("tree_print", camp::Args(1, "Slot: "));
          slotObj.set("dimension",
                      camp::Value(spaceDimMetaEnum.value("vire::device::space::DIM_3")));
          slotObj.call("set_coordinate", camp::Args(0, 8));
          slotObj.call("set_coordinate", camp::Args(1, 5));
          slotObj.call("set_coordinate", camp::Args(2, 1));
          slotObj.call("tree_print", camp::Args(1, "Slot (2): "));
          slotObj.call("reset_coordinates");
          slotObj.call("tree_print", camp::Args(1, "Slot (3): "));
          slotObj.call("make_dimension_1", camp::Args(camp::Value(0)));
          slotObj.call("tree_print", camp::Args(1, "Slot (4): "));
          slotObj.call("reset");
          slotObj.call("tree_print", camp::Args(1, "Slot (5): "));

        }
      }
      std::clog << std::endl;
    }

  }
  catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
