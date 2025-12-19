#include "myVisitor.hpp"
//
// Created by amirn on 12/19/2025.
//

void MyVisitor::getFuncs(std::shared_ptr<ast::Funcs> program)
{
    std::shared_ptr<ast::FuncDecl> printFunc = std::make_shared<ast::FuncDecl>(
        std::make_shared<ast::ID>("print"),
        std::make_shared<ast::Type>(ast::VOID),
        std::make_shared<ast::Formals>(ast::FormalDecl(std::make_shared<ast::ID>("toPrint"),
                                                       std::make_shared<ast::Type>(ast::STRING))),
        std::make_shared<ast::Statements>());
    std::shared_ptr<ast::FuncDecl> printiFunc = std::make_shared<ast::FuncDecl>(
        std::make_shared<ast::ID>("printi"),
        std::make_shared<ast::Type>(ast::VOID),
        std::make_shared<ast::Formals>(ast::FormalDecl(std::make_shared<ast::ID>("toPrint"),
                                                       std::make_shared<ast::Type>(ast::INT))),
        std::make_shared<ast::Statements>());
    funcsMap["print"] = printFunc;   // built-in function
    funcsMap["printi"] = printiFunc; // built-in function

    for (const auto &func : program->funcs)
    {
        // given that a function must not be defined more than once (even with different parameters)
        if (funcsMap.find(func->id->value) != funcsMap.end())
        {
            output::errorDef(func->line, func->id->value);
        }
        funcsMap[func->id->value] = func;
    }
}

void MyVisitor::visit(ast::Num &node)
{
}

void MyVisitor::visit(ast::NumB &node)
{
}

void MyVisitor::visit(ast::String &node)
{
}

void MyVisitor::visit(ast::Bool &node)
{
}

void MyVisitor::visit(ast::ID &node)
{
}

void MyVisitor::visit(ast::BinOp &node)
{
}

void MyVisitor::visit(ast::RelOp &node)
{
}

void MyVisitor::visit(ast::Not &node)
{
}

void MyVisitor::visit(ast::And &node)
{
}

void MyVisitor::visit(ast::Or &node)
{
}

void MyVisitor::visit(ast::Type &node)
{
}

void MyVisitor::visit(ast::Cast &node)
{
}

void MyVisitor::visit(ast::ExpList &node)
{
}

void MyVisitor::visit(ast::Call &node)
{
}

void MyVisitor::visit(ast::Statements &node)
{
}

void MyVisitor::visit(ast::Break &node)
{
}

void MyVisitor::visit(ast::Continue &node)
{
}

void MyVisitor::visit(ast::Return &node)
{
}

void MyVisitor::visit(ast::If &node)
{
}

void MyVisitor::visit(ast::While &node)
{
}

void MyVisitor::visit(ast::VarDecl &node)
{
}

void MyVisitor::visit(ast::Assign &node)
{
}

void MyVisitor::visit(ast::Formal &node)
{
}

void MyVisitor::visit(ast::Formals &node)
{
}

void MyVisitor::visit(ast::FuncDecl &node)
{
}

void MyVisitor::visit(ast::Funcs &node)
{
}
