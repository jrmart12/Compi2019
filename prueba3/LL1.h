#ifndef __LL1_H__
#define __LL1_H__

#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include "grammar.h"

using StringSet = std::set<std::string>;
using StringToStringSetMap = std::unordered_map<std::string, StringSet>;

class LL1 {
public:
    using TableRow = std::unordered_map<std::string, GRulePtr>;
    using Table = std::unordered_map<std::string, TableRow>;

    LL1(Grammar &gr, const StringToStringSetMap& _firsts, const StringToStringSetMap& _follows);

    const GSymbolPtrSet& getFirsts(const GSymbolPtr symb) const {
        auto it = firsts.find(symb->getName());

        if (it == firsts.end()) {
            throw std::logic_error("Symbol '" + symb->getName() + "' not found in 'first' map");
        }
        return it->second;
    }

    const GSymbolPtrSet& getFollows(const GSymbolPtr symb) const {
        auto it = follows.find(symb->getName());

        if (it == follows.end()) {
            throw std::logic_error("Symbol '" + symb->getName() + "' has no follows");
        }
        return it->second;
    }

    const GRulePtr getEntry(const std::string& non_term, const std::string& term) const {
        auto it = tbl.find(non_term);
        if (it == tbl.end()) {
            return nullptr;
        }
        const TableRow& tbl_row = it->second;
        auto itr = tbl_row.find(term);
        if (itr == tbl_row.end()) {
            return nullptr;
        }
        return itr->second;
    }
    
    size_t getRowCount() { return tbl.size(); }

    void computeTable();
    GSymbolPtrSet getFirsts(const GSymbolPtrVector& symbv);

private:
    Grammar &gr;
    std::unordered_map<std::string, GSymbolPtrSet> firsts;
    std::unordered_map<std::string, GSymbolPtrSet> follows;
    Table tbl;
};

#endif
