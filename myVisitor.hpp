#include "visitor.hpp"
#include "output.hpp"
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <pair>

using output::ScopePrinter;
using std::make_shared;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;
extern std::shared_ptr<ast::Node> program;

class Scope
{
    struct FuncInfo
    {
        ast::BuiltInType ret;
        vector<ast::BuiltInType> params;
    };

    struct VarInfo
    {
        ast::BuiltInType type;
        int offset;
    };

    struct resultFound
    {
        enum IdType; // forward declaration
        ast::BuiltInType type;
        bool found;
        IdType idType; // true for var, false for func
    };

public:
    enum IdType
    {
        VAR,
        FUNC
    };

    std::map<std::string, FuncInfo>
        funcsMap;
    std::map<std::string, VarInfo> varsMap;
    size_t offset;
    shared_ptr<Scope> parentScope;
    bool isLoopScope = false;
    Scope(shared_ptr<Scope> parentScope) : offset(0), parentScope(parentScope) {}

    resultFound findType(const string &id)
    {
        if (varsMap.find(id) != varsMap.end())
            return resultFound(varsMap[id].type, true, IdType::VAR);
        if (funcsMap.find(id) != funcsMap.end())
            return resultFound(funcsMap[id].ret, true, IdType::FUNC);
        if (parentScope)
            return parentScope->findType(id);
        return resultFound(ast::BuiltInType::VOID, false, IdType::VAR);
    }
};

class MyVisitor : public Visitor
{
    ScopePrinter printer;
    shared_ptr<Scope> currentScope = make_shared<Scope>(nullptr);
    ast::BuiltInType lastType;

    void getFuncs();

    void checkForMain();

public:
    MyVisitor() = default;

    void analyze()
    {
        getFuncs();
        checkForMain();
    }

    void visit(ast::Num &node) override;

    void visit(ast::NumB &node) override;

    void visit(ast::String &node) override;

    void visit(ast::Bool &node) override;

    void visit(ast::ID &node) override;

    void visit(ast::BinOp &node) override;

    void visit(ast::RelOp &node) override;

    void visit(ast::Not &node) override;

    void visit(ast::And &node) override;

    void visit(ast::Or &node) override;

    void visit(ast::Type &node) override;

    void visit(ast::Cast &node) override;

    void visit(ast::ExpList &node) override;

    void visit(ast::Call &node) override;

    void visit(ast::Statements &node) override;

    void visit(ast::Break &node) override;

    void visit(ast::Continue &node) override;

    void visit(ast::Return &node) override;

    void visit(ast::If &node) override;

    void visit(ast::While &node) override;

    void visit(ast::VarDecl &node) override;

    void visit(ast::Assign &node) override;

    void visit(ast::Formal &node) override;

    void visit(ast::Formals &node) override;

    void visit(ast::FuncDecl &node) override;

    void visit(ast::Funcs &node) override;
};