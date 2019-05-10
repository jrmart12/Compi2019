#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include "doctest.h"
#include "grammar.h"
#include "LL1.h"

using LL1TableRow = std::unordered_map<std::string, std::string>;
using LL1Table = std::unordered_map<std::string, LL1TableRow>;

static const char *gr1[] = {
    "E -> T E'",    // 0
    "E' -> + T E'", // 1
    "E' -> ''",     // 2
    "T -> F T'",    // 3
    "T' -> * F T'", // 4
    "T' -> ''",     // 5
    "F -> ( E )",   // 6
    "F -> id"       // 7
};

static StringSet gr1_non_terminals = {"E", "T", "E'", "T'", "F"};
static StringSet gr1_terminals = {"+", "*", "(", ")", "id", EPSILON, END_OF_INPUT};
static const int gr1_size = sizeof(gr1) / sizeof(gr1[0]);

static const char *gr3[] = {
    "I -> T",       // 0
    "T -> A B C",   // 1
    "A -> a",       // 2
    "A -> ''",      // 3
    "B -> b",       // 4
    "B -> ''",      // 5
    "C -> c",       // 6
    "C -> ''",      // 7
};

static StringSet gr3_non_terminals = {"I", "T", "A", "B", "C"};
static StringSet gr3_terminals = {"a", "b", "c", EPSILON, END_OF_INPUT};
static const int gr3_size = sizeof(gr3) / sizeof(gr3[0]);

namespace doctest {
    template<> struct StringMaker<StringSet> {
        static String convert(const StringSet& symbv) {
            bool first = true;
            std::ostringstream out;

            out << "{";
            for (auto s : symbv) {
                if (first) {
                    first = false;
                } else {
                    out << ", ";
                }
                out << s;
            }
            out << "}";
            return doctest::String(out.str().c_str());
        }
    };
}

template <typename SymbCtr>
StringSet convertToStringSet(const SymbCtr& sctr) {
    StringSet sset;

    std::transform(sctr.begin(),
                   sctr.end(),
                   std::inserter(sset, sset.end()),
                   [](const GSymbolPtr s) { return s->getName(); }
    );

    return sset;
}

std::string makeString(const char *str_arr[], int size) {
    std::ostringstream out;

    for (int i = 0; i < size; i++) {
        out << str_arr[i] << '\n';
    }
    return out.str();
}

static const StringToStringSetMap gr1_firsts = {
    {"E", {"(", "id"}},
    {"T", {"(", "id"}},
    {"E'", {"+", EPSILON}},
    {"T'", {"*", EPSILON}},
    {"F", {"(", "id"}},
};

static const StringToStringSetMap gr1_follows = {
    { "E", {")", END_OF_INPUT} },
    { "E'", {")", END_OF_INPUT} },
    { "T", {"+", ")", END_OF_INPUT} },
    { "T'", {"+", ")", END_OF_INPUT} },
    { "F", {"*", "+",  ")", END_OF_INPUT} },
};

static const LL1Table gr1_ll1_table = {
    { "E", { {"(", "E -> T E'"}, {"id", "E -> T E'"} } },
    { "T", { {"(", "T -> F T'"}, {"id", "T -> F T'"} } },
    { "E'", { {"+", "E' -> + T E'"}, {")", "E' -> ''"}, {END_OF_INPUT, "E' -> ''"} } },
    { "T'", { {"*", "T' -> * F T'"}, {"+", "T' -> ''"}, {")", "T' -> ''"}, {END_OF_INPUT, "T' -> ''"} } },
    { "F", { {"(", "F -> ( E )"}, {"id", "F -> id"} } },
};

static const StringToStringSetMap gr3_firsts = {
    { "I", {"a", EPSILON, "b", "c"} },
    { "T", {"a", EPSILON, "b", "c"} },
    { "A", {"a", EPSILON} },
    { "B", {"b", "''"} },
    { "C", {"c", "''"} },
};

static const StringToStringSetMap gr3_follows = {
    { "I", {END_OF_INPUT} },
    { "T", {END_OF_INPUT} },
    { "A", {"b", "c", END_OF_INPUT} },
    { "B", {"c", END_OF_INPUT} },
    { "C", {END_OF_INPUT} },
};

static const LL1Table gr3_ll1_table = {
    { "I", { {"a", "I -> T"}, { "b", "I -> T"},	{ "c", "I -> T"}, { END_OF_INPUT, "I -> T"} } },
    { "T", { {"a", "T -> A B C"}, { "b", "T -> A B C"},	{ "c", "T -> A B C"}, { END_OF_INPUT, "T -> A B C"} } },
    { "A", { {"a", "A -> a"}, { "b", "A -> ''"}, { "c", "A -> ''"},	{ END_OF_INPUT, "A -> ''"} } },
    { "B", { { "b", "B -> b"},	{ "c", "B -> ''"}, { END_OF_INPUT, "B -> ''"} } },
    { "C", { { "c", "C -> c"}, { END_OF_INPUT, "C -> ''"} } },
};

StringSet getMapEntry(const StringToStringSetMap& map, const std::string& sname) {
    auto it = map.find(sname);
    if (it == map.end()) {
        return StringSet();
    }
    return it->second;
}

std::string getLL1Entry(const LL1Table& tbl, const std::string& nt, const std::string& t) {
    auto it = tbl.find(nt);
    if (it == tbl.end()) {
        return "";
    }
    const LL1TableRow& row = it->second;
    auto itr = row.find(t);
    if (itr == row.end()) {
        return "";
    }
    return itr->second;
}

void testLL1(const char *glines[], unsigned size,
                 const StringToStringSetMap& efirsts,
                 const StringToStringSetMap& efollows,
                 const LL1Table& etable)
{
    std::istringstream in(makeString(glines, size));
    Grammar g;
    g.loadFromFile(in);

    LL1 ll1(g, efirsts, efollows);

    ll1.computeTable();   
    for (auto nt : g.getNonTerminals()) {
        for (auto t : g.getTerminals()) {
            const GRulePtr tentryp = ll1.getEntry(nt->getName(), t->getName());
            std::string msg = "[" + nt->getName() + ", " + t->getName() + "]";
            INFO(msg);
            std::string tentry = tentryp? tentryp->toString() : "";
            std::string eentry = getLL1Entry(etable, nt->getName(), t->getName());
            CHECK( tentry == eentry );
        }
    }
}

TEST_CASE("LL1 table") {
    testLL1(gr1, gr1_size, gr1_firsts, gr1_follows, gr1_ll1_table);
    testLL1(gr3, gr3_size, gr3_firsts, gr3_follows, gr3_ll1_table);
}