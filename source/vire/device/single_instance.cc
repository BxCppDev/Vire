// vire/device/single_instance.cc
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
#include <vire/device/single_instance.h>

// Third party:
// - Boost:
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace qi = boost::spirit::qi;

namespace parsing_impl {
  // Parsing struct:
  struct si_tokens {
    std::string token;
  };
}

// Struct adapter:
BOOST_FUSION_ADAPT_STRUCT(parsing_impl::si_tokens, (std::string, token))

namespace vire {

  namespace device {

    VIRE_DEVICE_INSTANCE_REGISTRATION_IMPLEMENT(single_instance, "vire::device::single_instance");

    bool single_instance::has_slot() const
    {
      return _slot_.is_valid();
    }

    void single_instance::set_slot(const slot & slot_)
    {
      _slot_ = slot_;
      return;
    }

    void single_instance::reset_slot()
    {
      _slot_.reset();
      return;
    }

    const slot & single_instance::get_slot() const
    {
      return _slot_;
    }

    slot & single_instance::grab_slot()
    {
      return _slot_;
    }

    single_instance::single_instance()
    {
      return;
    }

    single_instance::single_instance(const slot & slot_)
    {
      set_slot(slot_);
      return;
    }

    single_instance::~single_instance()
    {
      return;
    }

    bool single_instance::is_valid() const
    {
      return has_slot();
    }

    void single_instance::reset()
    {
      reset_slot();
      return;
    }

    space::dimension_type single_instance::get_dimension() const
    {
      return _slot_.get_dimension();
    }

    size_t single_instance::get_number_of_items() const
    {
      return 1;
    }

    // size_t single_instance::compute_index_map(std::vector<uint32_t> & map_, int item_) const
    // {
    //   DT_THROW_IF(item_ != 0, std::domain_error, "Invalid item index '" << item_ << "' !");
    //   map_.clear();
    //   map_.push_back(_slot_);
    //   return map_.size();
    // }

    void single_instance::fetch_slot(int item_, slot & s_) const
    {
      DT_THROW_IF(item_ != 0, std::logic_error, "Invalid item index '" << item_ << "' !");
      s_ = _slot_;
      return;
    }

    void single_instance::tree_dump(std::ostream & out_,
                                    const std::string & title_,
                                    const std::string & indent_,
                                    bool inherit_) const
    {
      this->i_instance::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Slot = ";
      if (has_slot()) {
        out_ << _slot_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_,
                              const single_instance & si_)
    {
      std::string str;
      single_instance::to_string(str, si_);
      out_ << str;
      return out_;
    }

    // static
    bool single_instance::from_string(const std::string & from_, single_instance & si_)
    {
      return si_.from_string(from_);
    }

    template <typename Iterator>
    struct si_parser : qi::grammar<Iterator, parsing_impl::si_tokens(), qi::ascii::space_type>
    {
      // Syntax: [slot=token]
      si_parser() : si_parser::base_type(start)
      {
        slot_tok  %= +~qi::char_("]");
        start     %= '[' >> qi::lit("slot") >> '=' >> slot_tok >> ']';
      }
      // Parsing rules:
      qi::rule<Iterator, std::string(), qi::ascii::space_type>             slot_tok;
      qi::rule<Iterator, parsing_impl::si_tokens(), qi::ascii::space_type> start;
    };

    // virtual
    bool single_instance::from_string(const std::string & from_)
    {
      namespace qi = boost::spirit::qi;
      this->reset();
      std::string slot_token;
      std::string::const_iterator str_iter = from_.begin();
      std::string::const_iterator end_iter = from_.end();
      typedef std::string::const_iterator iterator_type;
      typedef si_parser<iterator_type> parser_type;
      parsing_impl::si_tokens sit;
      parser_type g;
      bool res = qi::phrase_parse(str_iter, end_iter, g, qi::ascii::space, sit);
      if (!res || str_iter != end_iter) {
        DT_LOG_ERROR(get_logging(), "Spirit/Qi parser failed to extract a slot token!");
        return false;
      }
      slot s;
      if (!s.from_string(sit.token)) {
        DT_LOG_ERROR(get_logging(), "Invalid slot token '" << slot_token << "'!");
        return false;
      }
      DT_LOG_DEBUG(get_logging(), "Parsed slot='" << s << "'");
      this->set_slot(s);
      return true;
    }

    // static
    void single_instance::to_string(std::string & to_, const single_instance & si_)
    {
      si_.to_string(to_);
      return;
    }

    void single_instance::to_string(std::string & to_) const
    {
      namespace karma = boost::spirit::karma;
      const slot & s = this->get_slot();
      std::string slot_token;
      s.to_string(slot_token);
      to_.clear();
      std::back_insert_iterator<std::string> out(to_);
      boost::spirit::karma::generate(out,
                                     '[' << karma::lit("slot") << '=' << karma::lit(slot_token) << ']');
      return;
    }

  } // namespace device

} // namespace vire
