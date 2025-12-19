#include "visitor.hpp"
#include "output.hpp"
#include <memory>
#include <map>
#include <string>
#include <vector>

using output::ScopePrinter;
using std::string;
using std::vector;

extern std::shared_ptr<ast::Node> program;

class MyVisitor : public Visitor
{
    struct FuncInfo
    {
        ast::BuiltInType ret;
        vector<ast::BuiltInType> params;
    };

    ScopePrinter scopePrinter;
    std::map<std::string, FuncInfo> funcsMap;

public:
    MyVisitor() = default;

    void getFuncs();

    void analyze()
    {
        getFuncs();
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