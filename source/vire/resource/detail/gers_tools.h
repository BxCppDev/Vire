//! \file  vire/resource/detail/gers_tools.h
//! \brief Tools for Vire general expression resource selector
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

#ifndef VIRE_RESOURCE_DETAIL_GERS_TOOLS_H
#define VIRE_RESOURCE_DETAIL_GERS_TOOLS_H

// Standard library:
#include <iostream>
#include <exception>
#include <string>
#include <vector>

// Third party:
// - Boost:
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix.hpp>
// - Bayeux/datatools:
#include <datatools/command_utils.h>
// - Bayeux/cuts:
#include <cuts/cut_tools.h>

// This project:
#include <vire/resource/utils.h>

namespace vire {

  namespace resource {

    class manager;

    namespace detail {

      namespace qi    = boost::spirit::qi;
      namespace phoenix = boost::phoenix;
      namespace ascii = boost::spirit::ascii;

      /// \brief General expression Spirit-based parsing and abstract syntax tree (AST) generator
      struct general_expression_parsing {

        struct binary_op;
        struct unary_op;
        struct nil {};

        struct identifier_node
        {
          identifier_node() {}

          identifier_node(const std::string & token_) : path(token_)
          {
          }

          identifier_node & operator=(const std::string & token_)
          {
            path = token_;
            return *this;
          }

          std::string path; //!< Path of the role
        };

        struct access_node
        {
          access_node() {}

          access_node(const std::string & token_) : label(token_)
          {
          }

          access_node & operator=(const std::string & token_) {
            label = token_;
            return *this;
          }

          std::string label; //!< Access mode label
        };

        struct regex_node
        {
          regex_node() {}

          regex_node(const std::string & token_) : re(token_)
          {
          }

          regex_node & operator=(const std::string & token_) {
            re = token_;
            return *this;
          }

          std::string re; //!< Regular expression on resource paths
        };

        struct range_node
        {
          typedef std::pair<std::string, std::string> range_pair_type;

          range_node() {}

          range_node(const range_pair_type & pair_)
            : range_pair(pair_)
          {
          }

          range_node(const std::string & first_, const std::string & second_)
            : range_pair(first_, second_) {}

          range_node(const std::vector<std::string> & limits_)
            : range_pair(limits_[0], limits_[1])
          {
          }

          range_node & operator=(const range_pair_type & pair_)
          {
            range_pair = pair_;
            return *this;
          }

          range_node & operator=(const std::vector<std::string> & limits_)
          {
            range_pair.first  = limits_[0];
            range_pair.second = limits_[1];
            return *this;
          }

          range_pair_type range_pair; //!< Min/max resource path (sorted using alphabetical ordering)
        };

        struct enum_node
        {
          typedef std::vector<std::string> path_coll_type;
          enum_node() {}

          enum_node(const path_coll_type & coll_) : paths(coll_)
          {
          }

          enum_node & operator=(const path_coll_type & coll_)
          {
            paths = coll_;
            return *this;
          }

          path_coll_type paths; //!< List of resource paths
        };

        struct expression_ast
        {
          typedef
          boost::variant<
            nil // can't happen!
            , identifier_node
            , regex_node
            , access_node
            , range_node
            , enum_node
            , boost::recursive_wrapper<expression_ast>
            , boost::recursive_wrapper<binary_op>
            , boost::recursive_wrapper<unary_op>
            >
          type;

          // Default constructor
          expression_ast() : expr(nil())
          {
            return;
          }

          // Templatized constructor from an expression
          template <typename Expr>
          expression_ast(const Expr & expr) : expr(expr)
          {
            return;
          }

          expression_ast & operator+=(const expression_ast & rhs);
          expression_ast & operator-=(const expression_ast & rhs);
          expression_ast & operator*=(const expression_ast & rhs);

          type expr;
        };

        struct binary_op
        {
          binary_op(char op, const expression_ast & left, const expression_ast & right)
            : op(op), left(left), right(right)
          {
            return;
          }

          char op;
          expression_ast left;
          expression_ast right;
        };

        struct unary_op
        {
          unary_op(char op, const expression_ast & subject)
            : op(op), subject(subject)
          {
            return;
          }

          char op;
          expression_ast subject;
        };

        struct negate_expr
        {
          template <typename T>
          struct result { typedef T type; };

          expression_ast operator()(const expression_ast & expr_) const
          {
            return expression_ast(unary_op('!', expr_));
          }
        };

        static boost::phoenix::function<negate_expr> neg;

        struct ast_print
        {
          typedef void result_type;

          ast_print(std::ostream & out_ = std::clog) : _out_(&out_)
          {
            return;
          }

          // void operator()(qi::info::nil) const
          void operator()(nil) const
          {
            return;
          }

          void operator()(const expression_ast & ast) const
          {
            boost::apply_visitor(*this, ast.expr);
            return;
          }

          void operator()(const binary_op & expr) const
          {
            *_out_ << "op:" << expr.op << "(";
            boost::apply_visitor(*this, expr.left.expr);
            *_out_ << ", ";
            boost::apply_visitor(*this, expr.right.expr);
            *_out_ << ')';
            return;
          }

          void operator()(unary_op const& expr) const
          {
            *_out_ << "op:" << expr.op << "(";
            boost::apply_visitor(*this, expr.subject.expr);
            *_out_ << ')';
            return;
          }

          void operator()(const std::string & token_) const
          {
            *_out_ << '"' << token_ << '"';
            return;
          }

          void operator()(const identifier_node & identifier_) const
          {
            *_out_ << "id:[" << identifier_.path << ']';
            return;
          }

          void operator()(const range_node & range_) const
          {
            *_out_ << "ra:[" << range_.range_pair.first << ';' << range_.range_pair.second << ']';
            return;
          }

          void operator()(const regex_node & regex_) const
          {
            *_out_ << "re:[" << regex_.re << ']';
            return;
          }

          void operator()(const access_node & access_) const
          {
            *_out_ << "ac:[" << access_.label << ']';
            return;
          }

          void operator()(const enum_node & enum_) const
          {
            *_out_ << "en:[size=" << enum_.paths.size() << ']';
            return;
          }

          std::ostream * _out_;

        };

        template <typename Iterator>
        struct resource_path_grammar : qi::grammar<Iterator, std::string(), ascii::space_type>
        {
          resource_path_grammar() : resource_path_grammar::base_type(start)
          {
            using qi::_val;
            using qi::_1;
            using qi::lit;
            using qi::char_;
            using qi::string;
            using ascii::alnum;
            using ascii::alpha;

            identifier_name = ((alpha | char_('_')) >> *(alnum | char_('_') | char_('.')));

            identifier_namespace = identifier_name >> char_(":") >> -string("//");

            identifier_path %= -identifier_namespace >> (identifier_name |  char_("/") >> identifier_name )
                                                    >> *(char_("/") >> identifier_name);

            start = identifier_path [_val = _1];
            return;
          }

          qi::rule<Iterator, std::string(), ascii::space_type> identifier_name;
          qi::rule<Iterator, std::string(), ascii::space_type> identifier_namespace;
          qi::rule<Iterator, std::string(), ascii::space_type> identifier_path;
          qi::rule<Iterator, std::string(), ascii::space_type> start;

        };

        /*
        "path/to/a/leaf"
        "/path/to/a/leaf"
        "namespace:path/to/a/leaf"
        "namespace:/path/to/a/leaf"
        "namespace://path/to/a/leaf"
        "namespace:///path/to/a/leaf"
        */
        template <typename Iterator>
        struct role_path_grammar : qi::grammar<Iterator, identifier_node(), ascii::space_type>
        {
          role_path_grammar() : role_path_grammar::base_type(start)
          {
            using qi::_val;
            using qi::_1;
            using qi::lit;
            using qi::char_;
            using qi::string;
            using ascii::alnum;
            using ascii::alpha;

            identifier_name = ((alpha | char_('_')) >> *(alnum | char_('_') | char_('.')));

            identifier_namespace = identifier_name >> char_(":") >> -string("//");

            identifier_path %= -identifier_namespace >> (identifier_name |  char_("/") >> identifier_name )
                                                    >> *(char_("/") >> identifier_name);

            start = identifier_path [_val = _1];
            return;
          }

          qi::rule<Iterator, std::string(),     ascii::space_type> identifier_name;
          qi::rule<Iterator, std::string(),     ascii::space_type> identifier_namespace;
          qi::rule<Iterator, std::string(),     ascii::space_type> identifier_path;
          qi::rule<Iterator, identifier_node(), ascii::space_type> start;

        };

        template <typename Iterator>
        struct role_name_grammar : qi::grammar<Iterator, identifier_node(), ascii::space_type>
        {
          role_name_grammar() : role_name_grammar::base_type(start)
          {
            using qi::_val;
            using qi::_1;
            using qi::char_;
            using ascii::alnum;
            using ascii::alpha;

            identifier_name %= ((alpha | char_('_')) >> *(alnum | char_('_') | char_('.')));

            start = identifier_name [_val = _1];
            return;
          }

          qi::rule<Iterator, std::string(),     ascii::space_type> identifier_name;
          qi::rule<Iterator, identifier_node(), ascii::space_type> start;

        };

        template <typename Iterator>
        struct role_re_grammar : qi::grammar<Iterator, regex_node(), ascii::space_type>
        {
          role_re_grammar() : role_re_grammar::base_type(start)
          {
            using qi::_val;
            using qi::_1;
            using qi::lit;
            using qi::lexeme;
            using ascii::char_;
            using ascii::alnum;
            using ascii::alpha;

            quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];

            regex_re %= '%' >> quoted_string;

            start = regex_re [_val = _1];
            return;
          }

          qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
          qi::rule<Iterator, std::string(), ascii::space_type> regex_re;
          qi::rule<Iterator, regex_node(),  ascii::space_type> start;

        };

        template <typename Iterator>
        struct role_ac_grammar : qi::grammar<Iterator, access_node(), ascii::space_type>
        {
          role_ac_grammar() : role_ac_grammar::base_type(start)
          {
            using qi::_val;
            using qi::_1;
            using qi::lit;
            using qi::lexeme;
            using ascii::string;

            access_re %= lit("@") >> (string(access_to_label(::vire::resource::ACCESS_READ_ONLY))
                                      | string(access_to_label(::vire::resource::ACCESS_WRITE_ONLY))
                                      | string(access_to_label(::vire::resource::ACCESS_READ_WRITE))
                                      );

            start = access_re[_val = _1];
            return;
          }

          qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
          qi::rule<Iterator, std::string(), ascii::space_type> access_re;
          qi::rule<Iterator, access_node(),  ascii::space_type> start;

        };

        template <typename Iterator>
        struct role_rg_grammar : qi::grammar<Iterator, range_node(), ascii::space_type>
        {
          role_rg_grammar() : role_rg_grammar::base_type(start)
          {
            using qi::_val;
            using qi::_1;

            range_g %= '[' >> resource_path_g
                           >> ';'
                           >> resource_path_g
                           >> ']';

            start = range_g [_val = _1];
            return;
          }
          resource_path_grammar<Iterator>                                   resource_path_g;
          qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> range_g;
          qi::rule<Iterator, range_node(), ascii::space_type>               start;

        };

        template <typename Iterator>
        struct role_enum_grammar : qi::grammar<Iterator, enum_node(), ascii::space_type>
        {
          role_enum_grammar() : role_enum_grammar::base_type(start)
          {
            using qi::_val;
            using qi::_1;

            enum_g %= '{' >> (resource_path_g % ';')  >> '}';

            start = enum_g [_val = _1];
            return;
          }
          resource_path_grammar<Iterator>                                   resource_path_g;
          qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> enum_g;
          qi::rule<Iterator, enum_node(), ascii::space_type>                start;

        };

        template <typename Iterator>
        struct calculator : qi::grammar<Iterator, expression_ast(), ascii::space_type>
        {
          calculator() : calculator::base_type(expression)
          {
            using qi::_val;
            using qi::_1;

            expression =
              term                            [_val = _1]
              >> *(   ('+' >> term            [_val += _1])
                      |   ('-' >> term        [_val -= _1])
                      )
              ;

            term =
              factor                          [_val = _1]
              >> *(   ('*' >> factor          [_val *= _1])
                      )
              ;

            factor =
              role_path_g                     [_val = _1]
              | role_rg_g                     [_val = _1]
              | role_enum_g                   [_val = _1]
              | role_re_g                     [_val = _1]
              | role_ac_g                     [_val = _1]
              |   '(' >> expression           [_val = _1] >> ')'
              |   ('!' >> factor              [_val = neg(_1)])
              ;

            return;
          }

          qi::rule<Iterator, expression_ast(), ascii::space_type> expression;
          qi::rule<Iterator, expression_ast(), ascii::space_type> term;
          qi::rule<Iterator, expression_ast(), ascii::space_type> factor;
          role_path_grammar<Iterator>                             role_path_g; ///< Role by path grammar
          role_re_grammar<Iterator>                               role_re_g;   ///< Role from regex grammar
          role_rg_grammar<Iterator>                               role_rg_g;   ///< Role from range grammar
          role_enum_grammar<Iterator>                             role_enum_g; ///< Role from enum grammar
          role_ac_grammar<Iterator>                               role_ac_g;   ///< Role from access grammar

        };

        static datatools::command::returned_info
        parse(const std::string & expr_,
              expression_ast & ast_);

        static datatools::command::returned_info
        build_cut(const expression_ast & ast_,
                  const manager & mgr_,
                  cuts::cut_handle_type & hcut_);

      protected:

        static datatools::command::returned_info
        _build_cut_from_ast_node(const expression_ast & ast_,
                                 const manager & mgr_,
                                 cuts::cut_handle_type & hcut_);

      };

    } // namespace detail

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_DETAIL_GERS_TOOLS_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
