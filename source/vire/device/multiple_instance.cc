// vire/device/multiple_instance.cc
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
#include <vire/device/multiple_instance.h>

// Standard library:
#include <algorithm>
#include <string>

// Third party:
// - Boost:
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/single_instance.h>

namespace qi = boost::spirit::qi;

namespace parsing_impl {
  // Parsing struct:
  struct mi_tokens {
    std::vector<std::string> tokens;
  };
}

// Struct adapter:
BOOST_FUSION_ADAPT_STRUCT(parsing_impl::mi_tokens, (std::vector<std::string>, tokens))

namespace vire {

  namespace device {

    // DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(multiple_instance, "vire::device::multiple_instance")

    VIRE_DEVICE_INSTANCE_REGISTRATION_IMPLEMENT(multiple_instance, "vire::device::multiple_instance");

    bool multiple_instance::has_slots() const
    {
      return _slots_.size() > 0;
    }

    bool multiple_instance::has_slot(const slot & slot_) const
    {
      return std::find(_slots_.begin(), _slots_.end(), slot_) != _slots_.end();
    }

    void multiple_instance::append_slot(const slot & slot_)
    {
      DT_THROW_IF(!slot_.is_valid(), std::logic_error, "Invalid slot!");
      DT_THROW_IF(slot_.get_dimension() == space::DIM_0, std::logic_error, "Multiple instance does not accept dimension 0 slots!");
      DT_THROW_IF(has_slot(slot_), std::logic_error, "Slot " << slot_ << " already belongs to the set of slots!");
      if (_slots_.size() > 0) {
        DT_THROW_IF(_slots_[0].get_dimension() != slot_.get_dimension(),
                    std::logic_error,
                    "Cannot add slot with dimension [" << slot_.get_dimension() << "]! Dimension ["
                    << _slots_[0].get_dimension() << "] is required!");
      }
      _slots_.push_back(slot_);
      return;
    }

    void multiple_instance::remove_slot(const slot & slot_)
    {
      std::vector<slot>::iterator found = std::find(_slots_.begin(), _slots_.end(), slot_);
      if (found != _slots_.end()) {
        _slots_.erase(found);
      }
      return;
    }

    uint32_t multiple_instance::get_number_of_slots() const
    {
      return _slots_.size();
    }

    const std::vector<slot> & multiple_instance::get_slots() const
    {
      return _slots_;
    }

    multiple_instance::multiple_instance()
    {
      return;
    }

    multiple_instance::~multiple_instance()
    {
      return;
    }

    bool multiple_instance::is_valid() const
    {
      return has_slots();
    }

    void multiple_instance::reset()
    {
      _slots_.clear();
      return;
    }

    space::dimension_type multiple_instance::get_dimension() const
    {
      if (_slots_.size() == 0) {
        return space::DIM_INVALID;
      }
      return _slots_[0].get_dimension();
    }

    size_t multiple_instance::get_number_of_items() const
    {
      return _slots_.size();
    }

    void multiple_instance::fetch_slot(int item_, slot & s_) const
    {
      DT_THROW_IF((item_ < 0) || (item_ >= (int) get_number_of_items()), std::domain_error,
                  "Invalid item index '" << item_ << "' !");
      s_ = _slots_[item_];
      return;
    }

    void multiple_instance::tree_dump(std::ostream & out_,
                                      const std::string & title_,
                                      const std::string & indent_,
                                      bool inherit_) const
    {
      this->i_instance::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Slots = ";
      if (!has_slots()) {
        out_ << "<none>";
      } else {
        out_ << _slots_.size();
      }
      out_ << std::endl;
      for (int i = 0; i < (int) _slots_.size(); i++) {
        out_ << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
        if (i == (int) _slots_.size() - 1) {
          out_ << datatools::i_tree_dumpable::last_tag;
        } else {
          out_ << datatools::i_tree_dumpable::tag;
        }
        out_ << "Slot[" << i << "] = " << _slots_[i] << std::endl;
      }
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_,
                              const multiple_instance & si_)
    {
      std::string str;
      multiple_instance::to_string(str, si_);
      out_ << str;
      return out_;
    }

    template <typename Iterator>
    struct mi_parser : qi::grammar<Iterator, parsing_impl::mi_tokens(), qi::ascii::space_type>
    {
      // Syntax: [slots={token0 ;token1; token2...; tokenN}]
      mi_parser() : mi_parser::base_type(start)
      {
        slot_tok      %= +~qi::char_("{;}\r\n");
        slot_tok_list %= '{' >> (slot_tok % ';') >> '}';
        start         %= '[' >> qi::lit("slots") >> '=' >> slot_tok_list >> ']';
      }
      // Parsing rules:
      qi::rule<Iterator, std::string(), qi::ascii::space_type>              slot_tok;
      qi::rule<Iterator, std::vector<std::string>(), qi::ascii::space_type> slot_tok_list;
      qi::rule<Iterator, parsing_impl::mi_tokens(), qi::ascii::space_type>  start;
    };

    // static
    bool multiple_instance::from_string(const std::string & from_, multiple_instance & mi_)
    {
      return mi_.from_string(from_);
    }


    // virtual
    bool multiple_instance::from_string(const std::string & from_)
    {
      this->reset();
      std::string::const_iterator str_iter = from_.begin();
      std::string::const_iterator end_iter = from_.end();
      typedef std::string::const_iterator iterator_type;
      typedef mi_parser<iterator_type> parser_type;
      parsing_impl::mi_tokens mit;
      parser_type g;
      bool res = qi::phrase_parse(str_iter, end_iter, g, qi::ascii::space, mit);
      if (!res || str_iter != end_iter) {
        DT_LOG_ERROR(get_logging(), "Spirit/Qi parser failed to extract a vector of strings!");
        return false;
      }
      if (mit.tokens.size()) {
        try {
          for (int i = 0; i < (int) mit.tokens.size(); i++) {
            slot s;
            if (!s.from_string(mit.tokens[i])) {
              this->reset();
              return false;
            }
            this->append_slot(s);
          }
        } catch (std::exception & error) {
          DT_LOG_ERROR(get_logging(), "Multiple instance has rejected the slot: " << error.what());
          this->reset();
          return false;
        }
      } else {
        return false;
      }
      return true;
    }

    // static
    void multiple_instance::to_string(std::string & to_, const multiple_instance & mi_)
    {
      mi_.to_string(to_);
      return;
    }

    void multiple_instance::to_string(std::string & to_) const
    {
      namespace karma = boost::spirit::karma;
      to_.clear();
      std::vector<std::string> slot_tokens;
      slot_tokens.reserve(this->get_slots().size());
      for (int i = 0; i < (int) this->get_slots().size(); i++) {
        {
          std::string dummy;
          slot_tokens.push_back(dummy);
        }
        std::string & sltok = slot_tokens.back();
        this->get_slots()[i].to_string(sltok);
      }
      std::back_insert_iterator<std::string> out(to_);
      boost::spirit::karma::generate(out,
                                     '[' << karma::lit("slots") << '='
                                     << '{'
                                     << (karma::eps(slot_tokens.size() > 0) << (karma::string % karma::lit(";")) | "")
                                     << '}'
                                     << ']',
                                     slot_tokens);
      return;
    }

  } // namespace device

} // namespace vire
