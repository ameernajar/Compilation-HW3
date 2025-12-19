#include "myVisitor.hpp"
//
// Created by amirn on 12/19/2025.
//

void MyVisitor::getFuncs()
{

    auto &funcsMap = currentScope->funcsMap;

    // add built-in functions
    funcsMap["print"] = {ast::BuiltInType::VOID, {ast::BuiltInType::STRING}};
    funcsMap["printi"] = {ast::BuiltInType::VOID, {ast::BuiltInType::INT}};

    printer.emitFunc("print", ast::BuiltInType::VOID, {ast::BuiltInType::STRING});
    printer.emitFunc("printi", ast::BuiltInType::VOID, {ast::BuiltInType::INT});

    for (const auto &func : dynamic_cast<ast::Funcs *>(program.get())->funcs)
    {
        const string &name = func->id->value;
        // given that a function must not be defined more than once (even with different parameters)
        if (funcsMap.find(name) != funcsMap.end())
        {
            output::errorDef(func->line, name);
        }
        vector<ast::BuiltInType> params = {};
        if (func->formals)
        {
            for (const auto &formal : func->formals->formals)
            {
                params.push_back(formal->type->type);
            }
        }

        funcsMap[name] = {func->return_type->type, params};
        printer.emitFunc(name, func->return_type->type, params);
    }
}

void MyVisitor::checkForMain()
{
    const auto &it = currentScope->funcsMap.find("main");
    if (it == currentScope->funcsMap.end() || it->second.ret != ast::BuiltInType::INT || !it->second.params.empty())
    {
        output::errorMainMissing();
    }
}

void MyVisitor::visit(ast::Num &node)
{
    lastType = ast::BuiltInType::INT;
    return;
}

void MyVisitor::visit(ast::NumB &node)
{
    // maybe check if byte is negative
    if (node.value > 255)
    {
        output::errorByteTooLarge(node.line, node.value);
    }
    lastType = ast::BuiltInType::BYTE;
    return;
}

void MyVisitor::visit(ast::String &node)
{
    lastType = ast::BuiltInType::STRING;
}

void MyVisitor::visit(ast::Bool &node)
{
    lastType = ast::BuiltInType::BOOL;
}

void MyVisitor::visit(ast::ID &node)
{
    auto result = currentScope->findType(node.value);
    if (!result.found)
        output::errorUndef(node.line, node.value);

    lastType = result.type;
}

void MyVisitor::visit(ast::BinOp &node)
{
    visit(*(node.left));

    visit(*(node.right));
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
    // add new id to varsMap before visiting ID
    //  emitVar(const std::string &id, const ast::BuiltInType &type, int offset);
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