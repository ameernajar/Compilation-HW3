#include "visitor.hpp"
#include "output.hpp"
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <iostream>

using output::ScopePrinter;
using std::cout;
using std::make_shared;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;
extern std::shared_ptr<ast::Node> program;

enum IdType
{
    VAR,
    FUNC
};

class Scope
{

public:
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
    struct Info
    {
        FuncInfo *func;
        VarInfo *var;
    };
    struct resultFound
    {
        ast::BuiltInType type;
        bool found;
        IdType idType;
        Info info;
    };

    // ------------------fields declaration------------------------
    std::map<std::string, FuncInfo> funcsMap;
    std::map<std::string, VarInfo> varsMap;
    size_t offset;
    shared_ptr<Scope> parentScope;
    bool isLoopScope = false;
    Scope(shared_ptr<Scope> parentScope) : funcsMap(), varsMap(),
                                           offset(parentScope ? parentScope->offset : 0),
                                           parentScope(parentScope),
                                           isLoopScope(parentScope ? parentScope->isLoopScope : false) {}

    // ---------------------------methods---------------------------------
    resultFound find(const string &id)
    {
        if (varsMap.find(id) != varsMap.end())
            return resultFound{varsMap[id].type, true, IdType::VAR, Info{nullptr, &varsMap[id]}};
        if (funcsMap.find(id) != funcsMap.end())
            return resultFound{funcsMap[id].ret, true, IdType::FUNC, Info{&funcsMap[id], nullptr}};
        if (parentScope)
            return parentScope->find(id);
        return resultFound{ast::BuiltInType::VOID, false, IdType::VAR, Info{nullptr, nullptr}};
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
    void visitStatement(ast::Statement &node);

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