#include "myVisitor.hpp"
//
// Created by amirn on 12/19/2025.
//

bool isNumericType(ast::BuiltInType type)
{
    return type == ast::BuiltInType::INT || type == ast::BuiltInType::BYTE;
}

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
    const ast::BuiltInType leftType = lastType;

    if (!isNumericType(leftType))
        output::errorMismatch(node.line);

    visit(*(node.right));
    const ast::BuiltInType rightType = lastType;

    if (!isNumericType(rightType))
        output::errorMismatch(node.line);

    if (leftType == ast::BuiltInType::INT || rightType == ast::BuiltInType::INT)
        lastType = ast::BuiltInType::INT;
    else
        lastType = ast::BuiltInType::BYTE;
}

void MyVisitor::visit(ast::RelOp &node)
{
    visit(*(node.left));
    const ast::BuiltInType leftType = lastType;

    if (!isNumericType(leftType))
        output::errorMismatch(node.line);

    visit(*(node.right));
    const ast::BuiltInType rightType = lastType;

    if (!isNumericType(rightType))
        output::errorMismatch(node.line);

    lastType = ast::BuiltInType::BOOL;
}

void MyVisitor::visit(ast::Not &node)
{
    visit(*(node.exp));
    const ast::BuiltInType expType = lastType;

    if (expType != ast::BuiltInType::BOOL)
        output::errorMismatch(node.line);

    lastType = ast::BuiltInType::BOOL;
}

void booleanOp(ast::Node &node)
{
    visit(*(node.left));
    const ast::BuiltInType leftType = lastType;

    if (leftType != ast::BuiltInType::BOOL)
        output::errorMismatch(node.line);

    visit(*(node.right));
    const ast::BuiltInType rightType = lastType;

    if (rightType != ast::BuiltInType::BOOL)
        output::errorMismatch(node.line);
}

void MyVisitor::visit(ast::And &node)
{
    booleanOp(node);
    lastType = ast::BuiltInType::BOOL;
}

void MyVisitor::visit(ast::Or &node)
{
    booleanOp(node);
    lastType = ast::BuiltInType::BOOL;
}

void MyVisitor::visit(ast::Type &node)
{
    lastType = node.type;
}

void MyVisitor::visit(ast::Cast &node)
{
    visit(*(node.exp));
    const ast::BuiltInType expType = lastType;

    if (expType == node.target_type->type)
        return;

    if (expType == ast::BuiltInType::INT && node.target_type->type == ast::BuiltInType::BYTE)
    {
        lastType = ast::BuiltInType::BYTE;
        return;
    }

    if (expType == ast::BuiltInType::BYTE && node.target_type->type == ast::BuiltInType::INT)
    {
        lastType = ast::BuiltInType::INT;
        return;
    }

    output::errorMismatch(node.line);
}

void MyVisitor::visit(ast::ExpList &node)
{
    for (const auto &exp : node.exps)
    {
        visit(*exp);
    }
}

void MyVisitor::visit(ast::Call &node)
{
    const auto findResult = currentScope->findType(node.func_id->value);
    if (!findResult.found)
    {
        output::errorUndefFunc(node.line, node.func_id->value);
    }
    if (findResult.idType != Scope::IdType::FUNC)
    {
        output::errorDefAsVar(node.line, node.func_id->value);
    }
    const auto &funcInfo = findResult.funcInfo;
    vector<ast::BuiltInType> argTypes;
    if (node.args)
    {
        for (const auto &arg : node.args->exps)
        {
            visit(*arg);
            argTypes.push_back(lastType);
        }
    }

    // cast param types to string for error message
    vector<string> paramTypes;
    for (const auto &type : funcInfo.params)
        paramTypes.push_back(output::typeToString(type));

    // check parameter count and types
    if (argTypes.size() != funcInfo.params.size())
        output::errorPrototypeMismatch(node.line, node.func_id->value, paramTypes);
    for (size_t i = 0; i < argTypes.size(); ++i)
    {
        if (argTypes[i] != funcInfo.params[i])
            output::errorPrototypeMismatch(node.line, node.func_id->value, paramTypes);
    }
}

void MyVisitor::visit(ast::Statements &node)
{
    // TODO: check scopes
    // TODO: STATEMENT NODES DO NOT HAVE VISIT!
    for (const auto &statement : node.statements)
    {
        visit(*statement);
    }
}

void MyVisitor::visit(ast::Break &node)
{
    if (!currentScope->isLoopScope)
        output::errorUnexpectedBreak(node.line);
}

void MyVisitor::visit(ast::Continue &node)
{
    if (!currentScope->isLoopScope)
        output::errorUnexpectedContinue(node.line);
}

void MyVisitor::visit(ast::Return &node)
{
    if (node.exp)
    {
        visit(*(node.exp));
    }
}

void MyVisitor::visit(ast::If &node)
{
    visit(*(node.condition));
    const ast::BuiltInType condType = lastType;

    if (condType != ast::BuiltInType::BOOL)
        output::errorMismatch(node.line);

    currentScope = make_shared<Scope>(currentScope);
    printer.beginScope();
    visit(*(node.then));
    printer.endScope();
    currentScope = currentScope->parentScope;
    if (node.otherwise)
    {
        currentScope = make_shared<Scope>(currentScope);
        printer.beginScope();
        visit(*(node.otherwise));
        printer.endScope();
        currentScope = currentScope->parentScope;
    }
}

void MyVisitor::visit(ast::While &node)
{
    visit(*(node.condition));
    const ast::BuiltInType condType = lastType;

    if (condType != ast::BuiltInType::BOOL)
        output::errorMismatch(node.line);

    currentScope = make_shared<Scope>(currentScope);
    currentScope->isLoopScope = true;
    printer.beginScope();
    visit(*(node.body));
    printer.endScope();
    currentScope = currentScope->parentScope;
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