#include <algorithm>
#include <iostream>
#include <sstream>
#include <set>
#include "str_util.h"
#include "grammar.h"

GSymbolPtr GSymbol::epsilon() {
    static GSymbol eps("''", SymbKind::Terminal);
    return &eps;
}

GSymbolPtr GSymbol::endOfInput() {
    static GSymbol eoi(".$.", SymbKind::Terminal);
    return &eoi;
}

std::string GRule::toString() const {
    std::ostringstream out;

    out << lhs->getName() << " -> ";

    bool first = true;
    for (const auto& s : rhs) {
        if (first) {
            first = false;
        } else {
            out << " ";
        }
        out << s->getName();
    }

    return out.str();
}

bool Grammar::Loader::loadGrammar(Grammar& gr) {
    std::string line;

    GSymbolPtr eoi = createSymbol(END_OF_INPUT, gr, SymbKind::Terminal);
    all_symbs.push_back(eoi);

    while (!in.eof()) {
        std::getline(in, line);
        trim(line);

        if (line.empty()) {
            continue;
        }

        RuleParserData rule_data;

        if (!parseRule(line, rule_data)) {
            std::cerr << "Invalid Rule '" << line << "'" << std::endl;
            return false;
        }

        GSymbolPtr lhs_symb = getOrCreateSymbol(rule_data.lhs, gr, SymbKind::NonTerminal);
        GSymbolPtrVector rhsv;

        lhs_symb->setKind(SymbKind::NonTerminal);
        for (const auto& s : rule_data.rhs) {
            GSymbolPtr symb_ptr = getOrCreateSymbol(s, gr, SymbKind::Terminal);
            rhsv.push_back(symb_ptr);
        }

        GRuleUPtr rule_uptr = std::make_unique<GRule>(lhs_symb, std::move(rhsv));
        GRulePtr rule_ptr = rule_uptr.get();

        gr.rule_map[lhs_symb].push_back(rule_ptr);
        gr.rulesv.push_back(rule_ptr);

        if (gr.start_symb == nullptr) {
            gr.start_symb = lhs_symb;
        }

        gr.rules.push_back(std::move(rule_uptr));
    }

    for (auto symb : all_symbs) {
        if (symb->isTerminal()) {
            gr.terminals.push_back(symb);
        } else if (symb->isNonTerminal()) {
            gr.non_terminals.push_back(symb);
        } else {
            throw std::logic_error("Invalid symbol kind for '" + symb->toString() + "'");
        }
    }

    gr.symb_map = std::move(symb_map);

    return true;
}

GSymbolPtr Grammar::Loader::createSymbol(const std::string& sname, Grammar& gr, SymbKind kind) {
    GSymbolUPtr symb_uptr = std::make_unique<GSymbol>(sname, kind);
    GSymbolPtr symb_ptr = symb_uptr.get();
    symb_map.emplace(sname, symb_ptr);
    all_symbs.push_back(symb_ptr);

    gr.symbs.push_back(std::move(symb_uptr));

    return symb_ptr;
}

bool Grammar::Loader::parseRule(const std::string &srule, RuleParserData& rule_data) {
    int pos = srule.find("->");

    if (pos == std::string::npos) {
        return false;
    }

    std::string str_lhs = srule.substr(0, pos);
    std::string str_rhs = srule.substr(pos + 2);
    trim(str_lhs);
    trim(str_rhs);

    rule_data.lhs = str_lhs;
    strsplit(str_rhs, " ", std::back_inserter(rule_data.rhs));

    return true;
}
