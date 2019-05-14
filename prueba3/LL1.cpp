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
    GSymbolPtrSet  fset;
    auto it =symbv.begin();
    while(it != symbv.end()){
        GSymbolPtr s = *it;
        if(s->isTerminal()){
            fset.insert(s);
            break;
        }
        const GSymbolPtrSet &other_fset=firsts[s->getName()];
        bool epsilon_in_firsts=other_fset.count(GSymbol::epsilon())>0;
        fset.insert(other_fset.begin(),other_fset.end());
        if(!epsilon_in_firsts){
            break;
        }
        else{
            fset.erase(GSymbol::epsilon());
        }
        it++;
    }
    if(it==symbv.end()){
        fset.insert(GSymbol::epsilon());
    }
    return fset;
}

void LL1::computeTable()
{
    const GRulePtrVector &rules=gr.getAllRules();
    for(auto rule:rules){
        const GSymbolPtrSet &first_of_rule=getFirsts(rule->getRHS());
        std::string lhs_name=rule->getLHS()->getName();
        
        for(auto s:first_of_rule){
            if(!s->isEpsilon()){
                tbl[lhs_name][s->getName()]=rule;
            }
        }
    }


}

