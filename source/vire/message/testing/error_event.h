#ifndef VIRE_MESSAGE_TESTING_ERROR_EVENT_H
#define VIRE_MESSAGE_TESTING_ERROR_EVENT_H

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/archives_instantiation.h>
// - BxJsontools:
// #include <jsontools/std_type_converters.h>

// This project:
#include <vire/utility/base_event.h>

namespace vire {
  namespace message {
    namespace testing {
      // An event that embeds base error object
      class error_event : public vire::utility::base_event
      {
      public:
        error_event() : vire::utility::base_event(vire::time::now())
        {
          return;
        }
        explicit error_event(const vire::utility::base_error & err_) : vire::utility::base_event(vire::time::now())
        {
          set_err(err_);
          return;
        }
        virtual ~error_event()
        {
          return;
        }
        void set_err(const vire::utility::base_error & err_)
        {
          _err_ = err_;
        }
        const vire::utility::base_error & get_err() const
        {
          return _err_;
        }

        virtual void tree_dump(std::ostream & out_ = std::clog,
                               const std::string & title_  = "",
                               const std::string & indent_ = "",
                               bool inherit_ = false) const
        {
          this->base_event::tree_dump(out_, title_, indent_, true);

          out_ << indent_ << ::datatools::i_tree_dumpable::tag
               << "Error code : " << _err_.get_code() << std::endl;

          out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
               << "Error message : '" << _err_.get_message() << "'" << std::endl;
          return;
        }

        /// Main JSON (de-)serialization method
        virtual void jsonize(jsontools::node & node_,
                               unsigned long int version_ = 0);

      private:
        vire::utility::base_error _err_;

        VIRE_UTILITY_PAYLOAD_INTERFACE(error_event)

      };

      VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(error_event,"vire::message::testing::error_event")

      void error_event::jsonize(jsontools::node & node_,
                                  unsigned long int version_)
      {
        this->base_event::jsonize(node_);
        node_["err"] % _err_;
        return;
      }

      template<class Archive>
      void error_event::serialize (Archive & archive_, const unsigned int /* version_*/)
      {
        archive_ & boost::serialization::make_nvp("vire__utility__base_event",
                                                  boost::serialization::base_object<vire::utility::base_event>(*this));
        archive_ & boost::serialization::make_nvp("err", _err_);
        return;
      }
    }
  }
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(vire::message::testing::error_event,"vire::message::testing::error_event")
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::testing::error_event)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::message::testing::error_event)

#endif // VIRE_MESSAGE_TESTING_ERROR_EVENT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
