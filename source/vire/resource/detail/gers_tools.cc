//! \file vire/resource/detail/gers_tool.cc
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
#include <vire/resource/detail/gers_tools.h>

// - Bayeux/datatools:
#include <datatools/detail/command_macros.h>
// - Bayeux/cuts:
#include <cuts/i_cut.h>
#include <cuts/i_binary_cut.h>
#include <cuts/not_cut.h>
#include <cuts/or_cut.h>
#include <cuts/and_cut.h>
#include <cuts/exclude_cut.h>

// This project:
#include <vire/resource/manager.h>
#include <vire/resource/role.h>
#include <vire/resource/regex_resource_selector.h>
#include <vire/resource/ranged_resource_selector.h>
#include <vire/resource/enumerated_resource_selector.h>
#include <vire/resource/by_access_resource_selector.h>

namespace vire {

  namespace resource {

    namespace detail {

      // static
      boost::phoenix::function<general_expression_parsing::negate_expr>
      general_expression_parsing::neg;

      general_expression_parsing::expression_ast &
      general_expression_parsing::expression_ast::operator+=(const expression_ast & rhs_)
      {
        expr = binary_op('+', expr, rhs_);
        return *this;
      }

      general_expression_parsing::expression_ast &
      general_expression_parsing::expression_ast::operator-=(const expression_ast & rhs_)
      {
        expr = binary_op('-', expr, rhs_);
        return *this;
      }

      general_expression_parsing::expression_ast &
      general_expression_parsing::expression_ast::operator*=(const expression_ast & rhs_)
      {
        expr = binary_op('*', expr, rhs_);
        return *this;
      }

      datatools::command::returned_info
      general_expression_parsing::parse(const std::string & expr_,
                                        expression_ast & ast_)
      {
        datatools::command::returned_info rinfo;
        // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::parse: "
        //           << "Entering..."
        //           << std::endl;
        using boost::spirit::ascii::space;

        typedef std::string::const_iterator iterator_type;
        typedef calculator<iterator_type> calculator;

        calculator calc; // Our grammar
        std::string::const_iterator iter = expr_.begin();
        std::string::const_iterator end = expr_.end();
        ast_print printer;
        bool r = phrase_parse(iter, end, calc, space, ast_);
        if (r && iter == end) {
          printer(ast_);
          std::clog << std::endl;
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::parse: "
          //           << "Exiting: SUCCESS!"
          //           << std::endl;
          return rinfo;
        }
        // Error:
        std::string rest(iter, end);
        // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::parse: "
        //           << "Exiting: FAILURE!"
        //           << std::endl;
        DT_COMMAND_RETURNED_ERROR(rinfo, datatools::command::CEC_COMMAND_INVALID_SYNTAX,
                                  "Parsing failed at '" << rest << "'!");
        return rinfo;
      }

      // static
      datatools::command::returned_info
      general_expression_parsing::build_cut(const expression_ast & ast_,
                                            const manager & mgr_,
                                            cuts::cut_handle_type & hcut_)
      {
        datatools::command::returned_info rinfo;
        // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::build_cut: "
        //           << "Entering..."
        //           << std::endl;

        hcut_.reset();
        rinfo = _build_cut_from_ast_node(ast_, mgr_, hcut_);


        // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::build_cut: "
        //           << "Exiting: FAILURE!"
        //           << std::endl;
        return rinfo;
      }

      // struct cut_node_builder_visitor : public boost::static_visitor<> {};

      // static
      datatools::command::returned_info
      general_expression_parsing::_build_cut_from_ast_node(const expression_ast & ast_,
                                                           const manager & mgr_,
                                                           cuts::cut_handle_type & hcut_)
      {
        datatools::command::returned_info rinfo;

        if (const identifier_node * id_node = boost::get<identifier_node>(&ast_.expr)) {

          // Fetch a selector by name from the manager:
          const std::string & role_path = id_node->path;
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found identifier node with path '" << role_path << "'"
          //           << std::endl;
          if (!mgr_.has_role_by_path(role_path)) {
            DT_COMMAND_RETURNED_ERROR(rinfo, datatools::command::CEC_PARAMETER_INVALID_VALUE,
                                      "Resource manager has no role with path '" << role_path << "'!");
            return rinfo;
          }
          role & r = const_cast<role &>(mgr_.get_role_by_path(role_path));
          if (!r.has_functional_resource_selector()) {
            DT_COMMAND_RETURNED_ERROR(rinfo, datatools::command::CEC_COMMAND_INVALID_CONTEXT,
                                      "Role '" << role_path << "' has no functional resource selector!");
            return rinfo;
          }
          hcut_ = r.grab_functional_resource_selector_handle();

        } else if (const regex_node * re_node = boost::get<regex_node>(&ast_.expr)) {

          // Create a regex selector:
          const std::string & regex = re_node->re;
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found regex node with regex '" << regex << "'..."
          //           << std::endl;
          regex_resource_selector * rrs = new regex_resource_selector;
          hcut_.reset(rrs);
          rrs->set_resource_mgr(mgr_);
          rrs->set_regex_paths(regex);
          rrs->initialize_simple();

        } else if (const access_node * ac_node = boost::get<access_node>(&ast_.expr)) {

          // Create a by access selector:
          const std::string & access_label = ac_node->label;
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found access node with label '" << access_label << "'..."
          //           << std::endl;
          by_access_resource_selector * bars = new by_access_resource_selector;
          hcut_.reset(bars);
          bars->set_resource_mgr(mgr_);
          bars->set_access_by_label(access_label);
          bars->initialize_simple();

        } else if (const range_node * ra_node = boost::get<range_node>(&ast_.expr)) {

          // Create a ranged selector:
          const range_node::range_pair_type & rp = ra_node->range_pair;
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found range node with range '[" << rp.first << ';' << rp.second << "]'..."
          //           << std::endl;
          ranged_resource_selector * rrs = new ranged_resource_selector;
          hcut_.reset(rrs);
          rrs->set_resource_mgr(mgr_);
          rrs->set_range(rp.first, rp.second);
          rrs->initialize_simple();

        } else if (const enum_node * en_node = boost::get<enum_node>(&ast_.expr)) {

          // Create a enumerated selector:
          const enum_node::path_coll_type & pc = en_node->paths;
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found enumeration node with [" << pc.size() << "] paths..."
          //           << std::endl;
          enumerated_resource_selector * ers = new enumerated_resource_selector;
          hcut_.reset(ers);
          ers->set_resource_mgr(mgr_);
          for (auto path : pc) {
            ers->add_path(path);
          }
          ers->initialize_simple();

        } else if (const expression_ast * expr_node = boost::get<expression_ast>(&ast_.expr)) {

          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found expression AST node..."
          //           << std::endl;
          rinfo = _build_cut_from_ast_node(expr_node->expr, mgr_, hcut_);
          if (rinfo.is_failure()) {
            return rinfo;
          }

        } else if (const binary_op * binop_node = boost::get<binary_op>(&ast_.expr)) {

          // Build a binary operator selector:
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found binary_op node..."
          //           << std::endl;
          char op = binop_node->op;
          cuts::i_binary_cut * bincut = 0;
          if (op == '+') {
            bincut = new cuts::or_cut;
            // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
            //           << "Found OR binary cut..."
            //           << std::endl;
          } else if (op == '*') {
            bincut = new cuts::and_cut;
            // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
            //           << "Found AND binary cut..."
            //           << std::endl;
          } else if (op == '-') {
            bincut = new cuts::exclude_cut;
            // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
            //           << "Found EXCLUDE binary cut..."
            //           << std::endl;
          } else {
            DT_COMMAND_RETURNED_ERROR(rinfo, datatools::command::CEC_PARAMETER_INVALID_KEY,
                                      "Invalid binary operator '" << op << "'!");
            return rinfo;
          }
          cuts::cut_handle_type cut1;
          cuts::cut_handle_type cut2;
          rinfo = _build_cut_from_ast_node(binop_node->left, mgr_, cut1);
          if (rinfo.is_failure()) {
            return rinfo;
          }
          rinfo = _build_cut_from_ast_node(binop_node->right, mgr_, cut2);
          if (rinfo.is_failure()) {
            return rinfo;
          }
          bincut->set_cuts(cut1, cut2);
          hcut_.reset(bincut);
          bincut->initialize_simple();

        } else if (const unary_op * unop_node = boost::get<unary_op>(&ast_.expr)) {

          // Build an unary operator selector:
          // std::cerr << "DEVEL: vire::resource::detail::general_expression_parsing::_build_cut_from_ast_node: "
          //           << "Found unary_op node..."
          //           << std::endl;
          char op = unop_node->op;
          if (op == '!') {
            cuts::not_cut * negcut = new cuts::not_cut;
            cuts::cut_handle_type cut0;
            rinfo = _build_cut_from_ast_node(unop_node->subject, mgr_, cut0);
            if (rinfo.is_failure()) {
              return rinfo;
            }
            negcut->set_cut(cut0);
            hcut_.reset(negcut);
            negcut->initialize_simple();
          }

        } else {

          DT_COMMAND_RETURNED_ERROR(rinfo, datatools::command::CEC_FAILURE,
                                    "Invalid AST node!");
          return rinfo;
        }
        return rinfo;
      }

    } // namespace detail

  } // namespace resource

} // namespace vire
