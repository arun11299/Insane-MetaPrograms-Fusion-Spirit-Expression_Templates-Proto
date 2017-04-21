#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <iostream>

/*
 * As answered by jv_ in
 * http://stackoverflow.com/questions/39537675/strange-behaviour-of-parser-with-boost-spirit-x3
 */

namespace boost { namespace spirit { namespace x3
{
    template <typename Subject>
    struct hold_directive : unary_parser<Subject, hold_directive<Subject>>
    {
        using base_type = unary_parser<Subject, hold_directive<Subject>>;

        static bool const is_pass_through_unary = true;
        static bool const handles_container = Subject::handles_container;

        hold_directive(const Subject& subject)
          : base_type(subject) {}

        template <typename Iterator, typename Context,
                  typename RContext, typename Attribute>
        bool parse(Iterator& first, const Iterator& last,
                   const Context& context, RContext& rcontext, 
                   Attribute& attr) const
        {
            Attribute copy(attr);
            if (this->subject.parse(first, last, context, rcontext, copy))
            {
                traits::move_to(copy, attr);
                return true;
            }
            return false;
        }

    };

    struct hold_gen
    {
        template <typename Subject>
        hold_directive<typename extension::as_parser<Subject>::value_type>
        operator[](const Subject& subject) const
        {
            return { as_parser(subject) };
        }
    };

    auto const hold = hold_gen{};
}}}
