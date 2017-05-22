//! \file dom_utils.h
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

#ifndef VIRE_MOS_DOM_UTILS_H
#define VIRE_MOS_DOM_UTILS_H

// Standard library:
#include <cstdlib>

// Third Party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
// - Xerces-C++:
#include <xercesc/dom/DOMNode.hpp>
// - Camp:
#include <camp/enum.hpp>
#include <camp/detail/enummanager.hpp>
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/exception.h>

namespace vire {

  namespace mos {

    //! \brief Base builder
    class base_builder
    {
    public:

      //! Constructor
      base_builder(datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~base_builder();

      //! Build method
      virtual void operator()(const xercesc::DOMNode * node_) = 0;

      //! Set the logging priority threshold
      void set_logging(datatools::logger::priority prio_);

      //! Return the logging priority threshold
      datatools::logger::priority get_logging() const;

    protected:

      datatools::logger::priority _logging; //!< Logging priority threshold

    };

    //! \brief Abstract generic builder with embedded object with templatized type
    template<class ValueType>
    class base_generic_builder : public base_builder
    {
    public:

      //! Constructor
      base_generic_builder(ValueType & target_,
                           datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL)
        : base_builder(prio_), _target(&target_)
      {
        return;
      }

      //! Destructor
      virtual ~base_generic_builder()
      {
        return;
      }

      //! Access to the mutable embedded object
      ValueType & grab()
      {
        return *_target;
      }

      //! Access to the non mutable embedded object
      const ValueType & get() const
      {
        return *_target;
      }

    protected:

      ValueType * _target; //!< Target object

    };

    //! \brief Boolean builder
    class boolean_builder : public base_generic_builder<bool>
    {
    public:

      //! Constructor
      boolean_builder(bool & value_,
                      datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~boolean_builder();

      //! Build method for a boolean
      virtual void operator()(const xercesc::DOMNode * node_);

    };

    //! \brief Integer builder
    class integer_builder : public base_generic_builder<int>
    {
    public:

      //! Constructor
      integer_builder(int & value_,
                      datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~integer_builder();

      //! Build method for a integer
      virtual void operator()(const xercesc::DOMNode * node_);

    };

    //! \brief Float builder
    class float_builder : public base_generic_builder<float>
    {
    public:

      //! Constructor
      float_builder(float & value_,
                    datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~float_builder();

      //! Build method for a float
      virtual void operator()(const xercesc::DOMNode * node_);

    };

    //! \brief Double builder
    class double_builder : public base_generic_builder<double>
    {
    public:

      //! Constructor
      double_builder(double & value_,
                     datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~double_builder();

      //! Build method for a double
      virtual void operator()(const xercesc::DOMNode * node_);

    };

    //! \brief String builder
    class string_builder : public base_generic_builder<std::string>
    {
    public:

      //! Constructor
      string_builder(std::string & str_,
                     datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~string_builder();

      //! Build method for a string
      virtual void operator()(const xercesc::DOMNode * node_);

    };

    //! \brief Array builder
    template<class ValueType, class Builder>
    class vector_builder : public base_generic_builder<std::vector<ValueType> >
    {
    public:

      //! Constructor
      vector_builder(std::vector<ValueType> & vec_,
                     datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL)
        : base_generic_builder<std::vector<ValueType> >(vec_, prio_)
      {
        return;
      }

      //! Destructor
      virtual ~vector_builder()
      {
        return;
      }

      //! Build method for an element of a vector
      virtual void operator()(const xercesc::DOMNode * node_)
      {
        std::vector<ValueType> & vec = base_generic_builder<std::vector<ValueType> >::grab();
        {
          ValueType dummy;
          vec.push_back(dummy);
        }
        ValueType & item = vec.back();
        Builder builder(item, dynamic_cast<base_builder*>(this)->get_logging());
        builder(node_);
        return;
      }

    };

    //! \brief Optional builder
    template<class ValueType, class Builder>
    class optional_builder : public base_generic_builder<boost::optional<ValueType> >
    {
    public:

      //! Constructor
      optional_builder(boost::optional<ValueType> & opt_,
                       datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL)
        : base_generic_builder<boost::optional<ValueType> >(opt_, prio_)
      {
        return;
      }

      //! Destructor
      virtual ~optional_builder()
      {
        return;
      }

      //! Build method for an optional object
      virtual void operator()(const xercesc::DOMNode * node_)
      {
        datatools::logger::priority logging = dynamic_cast<base_builder*>(this)->get_logging();
        DT_LOG_DEBUG(logging, "Entering...")
        {
          ValueType dummy;
          base_generic_builder<boost::optional<ValueType> >::grab() = dummy;
          DT_LOG_DEBUG(logging, "dummy type = '" << typeid(dummy).name() << "'")
        }
        ValueType & t = base_generic_builder<boost::optional<ValueType> >::grab().get();
        Builder builder(t, logging);
        builder(node_);
        return;
      }

    };

    //! \brief Generic builder with embedded object with templatized enumeration type
    template<class EnumType>
    class enum_generic_builder : public base_generic_builder<EnumType>
    {
    public:

      //! Constructor
      enum_generic_builder(EnumType & target_,
                           datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL)
        : base_generic_builder<EnumType>(target_, prio_)
      {
        return;
      }

      //! Destructor
      virtual ~enum_generic_builder()
      {
        return;
      }

      //! Build method for an enumeration
      virtual void operator()(const xercesc::DOMNode * node_)
      {
        datatools::logger::priority logging = dynamic_cast<base_builder*>(this)->get_logging();
        DT_LOG_DEBUG(logging, "Entering...")
        std::string s;
        string_builder sbuilder(s);
        sbuilder(node_);
        DT_THROW_IF(s.empty(), std::logic_error, "Cannot parse a string for enumeration!");
        DT_LOG_DEBUG(logging, "string = '" << s << "'");
        for (int i = 0; i < (int) camp::detail::EnumManager::instance().count(); i++) {
          const camp::Enum & metaEnum = camp::detail::EnumManager::instance().getByIndex(i);
          DT_LOG_DEBUG(logging, "enum='" << metaEnum.name() << "'");
        }
        try {
          const camp::Enum & metaEnum = camp::enumByType<EnumType>();
          DT_THROW_IF(! metaEnum.hasName(s), std::logic_error,
                      "Enumeration '" << metaEnum.name() << "' has no value labelled '" << s << "'!");
          long lval = metaEnum.value(s);
          DT_LOG_DEBUG(logging, "lval=[" <<lval << "]");
          base_generic_builder<EnumType>::grab() = static_cast<EnumType>(lval);
          DT_LOG_DEBUG(logging, "Done.");
        } catch (std::exception & error) {
          DT_LOG_FATAL(logging, error.what());
          throw;
        }
        return;
      }

    };

  } // namespace mos

} // namespace vire

#define VIRE_MOS_DOM_BUILDER_ALIAS(BuilderTypeId, PrimitiveBuilderType) \
  typedef PrimitiveBuilderType BuilderTypeId;                           \
  /**/

#define VIRE_MOS_DOM_BUILDER_ENUM_ALIAS(EnumBuilderTypeId, EnumType)    \
  typedef ::vire::mos::enum_generic_builder<EnumType> EnumBuilderTypeId ; \
  /**/

#define VIRE_MOS_DOM_BUILDER_DECLARE(BuilderTypeId, ObjectType)         \
  class BuilderTypeId : public ::vire::mos::base_generic_builder<ObjectType> \
  {                                                                     \
  public:                                                               \
    BuilderTypeId(ObjectType &, datatools::logger::priority prio_ = datatools::logger::PRIO_FATAL); \
    virtual ~BuilderTypeId() {}                                         \
    virtual void operator()(const xercesc::DOMNode *);                  \
  };                                                                    \
  /**/

#include <vire/mos/dom_utils-inl.h>

#endif // VIRE_MOS_DOM_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
