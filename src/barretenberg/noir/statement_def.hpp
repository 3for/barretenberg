#pragma once
#include "ast.hpp"
#include "ast_adapted.hpp"
#include "common.hpp"
#include "error_handler.hpp"
#include "expression.hpp"
#include "statement.hpp"
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace noir {
namespace parser {

using x3::lexeme;
using x3::raw;
using x3::string;
using x3::uint_;
using namespace x3::ascii;

typedef x3::rule<struct statement_list_class, ast::statement_list> statement_list_type;
typedef x3::rule<struct int_type_class, ast::int_type> int_type_type;
typedef x3::rule<struct type_id_class, ast::type_id> type_id_type;
typedef x3::rule<struct variable_declaration_class, ast::variable_declaration> variable_declaration_type;
typedef x3::rule<struct function_argument_class, ast::function_argument> function_argument_type;
typedef x3::rule<struct function_declaration_class, ast::function_declaration> function_declaration_type;
typedef x3::rule<struct assignment_class, ast::assignment> assignment_type;
typedef x3::rule<struct variable_class, ast::variable> variable_type;

statement_type const statement("statement");
statement_list_type const statement_list("statement_list");
int_type_type const int_type("int_type");
type_id_type const type_id("type_id");
variable_declaration_type const variable_declaration("variable_declaration");
function_argument_type const function_argument("function_argument");
function_declaration_type const function_declaration("function_declaration");
assignment_type const assignment("assignment");
variable_type const variable("variable");

// Import the expression rule
namespace {
auto const& expression = noir::expression();
}

// clang-format off
auto const statement_list_def =
        *(function_declaration | variable_declaration | assignment)
    ;

struct ensure_bit_range
{
    template <typename Context>
    void operator()(Context const& ctx) const
    {
        _pass(ctx) = !(_attr(ctx) < 2 || _attr(ctx) > 64);
    }
};

auto const int_type_def =
        lexeme[string("uint") > uint_[ensure_bit_range()]]
    |   lexeme[string("int") > uint_[ensure_bit_range()]]
    ;

auto const type_id_def =
        lexeme[(string("bool") | int_type) >> !(alnum | '_')] > -("[" > uint_ > "]")
    ;

auto const variable_declaration_def =
        type_id
    >   assignment
    ;

auto const function_argument_def =
        type_id > identifier
    ;

auto const function_declaration_def =
        (type_id > identifier)
    >>  ("(" > -(function_argument % ',') > ")")
    >>  ("{" > statement_list > "}")
    ;

auto const assignment_def =
        variable
    >   '='
    >   expression
    >   ';'
    ;

auto const variable_def = identifier;
auto const statement_def = statement_list;
// clang-format on

BOOST_SPIRIT_DEFINE(statement,
                    statement_list,
                    int_type,
                    type_id,
                    variable_declaration,
                    function_argument,
                    function_declaration,
                    assignment,
                    variable);

struct statement_class : error_handler_base, x3::annotate_on_success {};
struct assignment_class : x3::annotate_on_success {};
struct variable_class : x3::annotate_on_success {};
} // namespace parser

parser::statement_type const& statement()
{
    return parser::statement;
}
} // namespace noir
