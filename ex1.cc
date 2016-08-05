#include <string>
#include <map>
#include <boost/proto/proto.hpp>

struct map_ {};
boost::proto::terminal<map_>::type map = {};
 
template< class Expr >
void format( std::string fmt, Expr const & expr )
{}


typedef std::map<std::string, std::string> string_map;
 
// Recursive function used to fill the map
template< class Expr >
void fill_map( Expr const & expr, string_map & subs )
{
    using boost::proto::value;      // read a value from a terminal
    using boost::proto::child_c;    // get the Nth child of a non-terminal
    std::cout << value(child_c<1>(expr)) << " :: " << value(child_c<2>(expr)) << std::endl;
    subs[ value(child_c<1>(expr)) ] = value(child_c<2>(expr));
    fill_map( child_c<0>(expr), subs );
}
 
// The 'map' terminal ends the recursion
void fill_map( boost::proto::terminal<map_>::type const &, string_map & )
{}
 
int main()
{
    format( "There are...", map("place", "heaven and earth")
                               ("thing", "philosophy") );
 
    format( "There are...", (map["place"] = "heaven and earth",
                             map["thing"] = "philosophy") );

    boost::proto::display_expr( (map["place"]="heaven and earth", map["thing"]="philosophy") );
    boost::proto::display_expr( map("place", "heaven and earth")("thing", "philosophy") );
    string_map m;
    fill_map(map("place", "heaven and earth")("thing", "philosophy"), m);

    return 0;
}
