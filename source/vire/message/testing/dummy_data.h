#ifndef VIRE_MESSAGE_TESTING_DUMMY_DATA_H
#define VIRE_MESSAGE_TESTING_DUMMY_DATA_H

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/archives_instantiation.h>
// - BxJsontools:
// #include <jsontools/std_type_converters.h>

// This project:
#include <vire/utility/base_payload.h>

namespace vire {

  namespace message {

    namespace testing {

      // A dummy data
      class dummy_data
        : public vire::utility::base_payload
      {
      public:

        dummy_data()
          : vire::utility::base_payload()
        {
          return;
        }

        explicit dummy_data(const double value_)
          : vire::utility::base_payload()
        {
          set_value(value_);
          return;
        }

        virtual ~dummy_data()
        {
          return;
        }

        void set_value(const double & value_)
        {
          _value_ = value_;
        }

        double get_value() const
        {
          return _value_;
        }

        virtual void tree_dump(std::ostream & out_ = std::clog,
                               const std::string & title_  = "",
                               const std::string & indent_ = "",
                               bool inherit_ = false) const
        {
          this->base_event::tree_dump(out_, title_, indent_, true);

          out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
               << "Value : '" << _value_ << "'" << std::endl;

          return;
        }

        /// Main JSON (de-)serialization method
        virtual void jsonize(jsontools::node & node_,
                               unsigned long int version_ = 0);

        // /// Main Protobuf (de-)serialization method
        // virtual void protobufize(protobuftools::message_node & node_,
        //                          const unsigned long int version_ = 0);

      private:

        double _value_;

        VIRE_UTILITY_PAYLOAD_INTERFACE(dummy_data)

      };

      VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(dummy_data,"vire::message::testing::dummy_data")

      void dummy_data::jsonize(jsontools::node & node_,
                               unsigned long int /* version_ */)
      {
        this->base_event::jsonize(node_);
        node_["value"] % _value_;
        return;
      }

      // void dummy_data::protobufize(protobuftools::message_node & node_,
      //                              const unsigned long int /* version_ */)
      // {
      //   VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload, node_);
      //   node_["value"] % _value_;
      //   return;
      // }

      template<class Archive>
      void dummy_data::serialize(Archive & archive_, const unsigned int /* version_*/)
      {
        archive_ & boost::serialization::make_nvp("vire__utility__base_payload",
                                                  boost::serialization::base_object<vire::utility::base_payload>(*this));
        archive_ & boost::serialization::make_nvp("value", _value_);
        return;
      }

    }
  }
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(vire::message::testing::dummy_data,"vire::message::testing::dummy_data")
DATATOOLS_SERIALIZATION_CLASS_SERIALIZE_INSTANTIATE_ALL(vire::message::testing::dummy_data)
BOOST_CLASS_EXPORT_IMPLEMENT(vire::message::testing::dummy_data)

// // Bind the C++ class to a specific protobuf message class
// #include <bayeux/protobuftools/protobuf_utils.h>
// BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::message::testing::dummy_data,
//                                                   "vire::message::testing::dummy_data")

#endif // VIRE_MESSAGE_TESTING_DUMMY_DATA_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
