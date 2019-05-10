#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <istream>
#include <set>
#include <unordered_map>
#include <unordered_set>

#define EPSILON "''"
#define END_OF_INPUT ".$."

enum class SymbKind { Terminal, NonTerminal };

class Grammar;
class GSymbol;
class GRule;

using GSymbolUPtr = std::unique_ptr<GSymbol>;
using GSymbolPtr = GSymbol*;
using GSymbolPtrVector = std::vector<GSymbolPtr>;
using GRuleUPtr = std::unique_ptr<GRule>;
using GRulePtr = GRule*;
using GRulePtrVector = std::vector<GRulePtr>;

class GSymbol {
public:
    GSymbol(const std::string& name, SymbKind kind): kind(kind), name(name) {}

    void setKind(SymbKind k) { kind = k; }
    std::string getName() const { return name; }
    bool isEpsilon() { return name == EPSILON; }
    bool isEndOfInput() { return name == END_OF_INPUT; }
    bool isTerminal() { return kind == SymbKind::Terminal; }
    bool isNonTerminal() { return kind == SymbKind::NonTerminal; }

    static GSymbolPtr epsilon();
    static GSymbolPtr endOfInput();

    bool operator==(const GSymbol& rhs) const {
        return (kind == rhs.kind) && (name == rhs.name);
    }

    bool operator!=(const GSymbol& rhs) const {
        return !operator==(rhs);
    }

    std::string toString() {
        std::string result;
        switch (kind) {
            case SymbKind::Terminal: result = "Terminal:"; break;
            case SymbKind::NonTerminal: result = "NonTerminal:"; break;
            default:
                result = "Unknown:";
        }
        result += name;

        return result;
    }

private:
    SymbKind kind;
    std::string name;
};

struct GSymbolFunc {
    struct HashPtr {
        size_t operator()(const GSymbolPtr sptr) const {
            std::hash<std::string> hash_str;

            return hash_str(sptr->getName());
        }
    };
    struct PtrEqualTo {
        bool operator()(const GSymbolPtr lhs, const GSymbolPtr rhs) const {
            return lhs->getName() == rhs->getName();
        }
    };
};

using GSymbolPtrSet = std::unordered_set<GSymbolPtr, GSymbolFunc::HashPtr, GSymbolFunc::PtrEqualTo>;

class GRule {
public:
    GRule(const GSymbolPtr lhs, GSymbolPtrVector&& rhs):
        lhs(lhs), rhs(std::move(rhs)) {}

    const GSymbolPtr getLHS() const { return lhs; }
    const GSymbolPtrVector& getRHS() const { return rhs; }
    std::string toString() const;

private:
    GSymbolPtr lhs;
    GSymbolPtrVector rhs;
};

class Grammar {
public:
    Grammar(): start_symb(nullptr) {}

    GSymbolPtr getStartSymbol() const { return start_symb; }
    const GSymbolPtrVector& getTerminals() const { return terminals; }
    const GSymbolPtrVector& getNonTerminals() const { return non_terminals; }
    const GRulePtrVector& getAllRules() const { return rulesv; }

    const GRulePtrVector& getRulesFor(const GSymbolPtr symb) const {
        auto it = rule_map.find(symb);
        if (it != rule_map.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Symbol '" + symb->getName() + "' has no rules\n");
        }
    }

    GSymbolPtr getSymbol(const std::string& sname) const {
        auto it = symb_map.find(sname);
        
        return (it == symb_map.end())? nullptr : it->second;
    }

    bool loadFromFile(std::istream& in) {
        Loader loader(in);

        return loader.loadGrammar(*this);
    }

private:
    class Loader {
    public:
        Loader(std::istream& in): in(in) {}
        bool loadGrammar(Grammar &gr);

    private:
        struct RuleParserData {
            std::string lhs;
            std::vector<std::string> rhs;
        };

        bool parseRule(const std::string &srule, RuleParserData& rule_data);

        GSymbolPtr getSymbol(const std::string& sname) const {
            auto it = symb_map.find(sname);

            return (it == symb_map.end())? nullptr : it->second;
        }

        GSymbolPtr createSymbol(const std::string& sname, Grammar &gr, SymbKind kind);

        GSymbolPtr getOrCreateSymbol(const std::string& sname, Grammar &gr, SymbKind kind) {
            GSymbolPtr symb_ptr = getSymbol(sname);

            if (symb_ptr == nullptr) {
                symb_ptr = createSymbol(sname, gr, kind);
            }
            return symb_ptr;
        }

    private:
        std::istream& in;
        std::unordered_map<std::string, GSymbolPtr> symb_map;
        GSymbolPtrVector all_symbs;
    };

private:
    GSymbolPtr start_symb;
    std::list<GSymbolUPtr> symbs;    // All grammar symbols
    std::unordered_map<std::string, GSymbolPtr> symb_map;
    GSymbolPtrVector non_terminals; // Non terminal symbols
    GSymbolPtrVector terminals;     // Terminal symbols
    std::list<GRuleUPtr> rules;      // Grammar rules
    GRulePtrVector rulesv;
    std::unordered_map<GSymbolPtr, GRulePtrVector> rule_map;
};

#endif
