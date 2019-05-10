#include <iostream>
#include <algorithm>
#include "LL1.h"

LL1::LL1(Grammar &gr,
         const StringToStringSetMap& _firsts,
         const StringToStringSetMap& _follows): gr(gr)
{
    for (const auto& f : _firsts) {
        std::string nt_name = f.first;
        const StringSet& sset = f.second;
        GSymbolPtrSet symb_set;

        std::transform(sset.begin(), sset.end(), std::inserter(symb_set, symb_set.end()),
                        [&gr](const std::string& s) { return gr.getSymbol(s); });

        firsts[nt_name] = std::move(symb_set);
    }

    for (const auto& f : _follows) {
        std::string nt_name = f.first;
        const StringSet& sset = f.second;
        GSymbolPtrSet symb_set;

        std::transform(sset.begin(), sset.end(), std::inserter(symb_set, symb_set.end()),
                        [&gr](const std::string& s) { return gr.getSymbol(s); });

        follows[nt_name] = std::move(symb_set);
    }
}

GSymbolPtrSet LL1::getFirsts(const GSymbolPtrVector& symbv) {
    GSymbolPtr firt = gr.getStartSymbol();
    GSymbolPtrSet getfirt = getFirsts(firt);

    return getfirt;
}

void LL1::computeTable()
{
    GSymbolPtr start_symb;
    GSymbolPtrVector non_terminals; // Non terminal symbols
    GSymbolPtrVector terminals;     // Terminal symbols
    GRulePtrVector rulesv;

    start_symb=gr.getStartSymbol();
    non_terminals=gr.getNonTerminals();
    terminals=gr.getTerminals();
    rulesv = gr.getAllRules();


}

