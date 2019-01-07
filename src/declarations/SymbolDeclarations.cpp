#include "SymbolDeclarations.hpp"

std::shared_ptr<const logic::Symbol> locationSymbol(std::string location, unsigned numberOfLoops)
{
    auto enclosingIteratorTypes = std::vector<const logic::Sort*>();
    for(int i=0; i < numberOfLoops; ++i)
    {
        enclosingIteratorTypes.push_back(logic::Sorts::natSort());
    }
    return logic::Signature::fetchOrAdd(location, enclosingIteratorTypes, logic::Sorts::timeSort());

}

std::shared_ptr<const logic::Symbol> locationSymbolForStatement(const program::Statement* statement)
{
    if (statement->type() == program::Statement::Type::WhileStatement)
    {
        return locationSymbol(statement->location, statement->enclosingLoops->size() + 1);
    }
    else
    {
        return locationSymbol(statement->location, statement->enclosingLoops->size());
    }
}

std::shared_ptr<const logic::Symbol> locationSymbolLeftBranch(const program::IfElse* ifElse)
{
    return locationSymbol(ifElse->location + "_lEnd", ifElse->enclosingLoops->size());
}
std::shared_ptr<const logic::Symbol> locationSymbolRightBranch(const program::IfElse* ifElse)
{
    return locationSymbol(ifElse->location + "_rEnd", ifElse->enclosingLoops->size());
}

std::shared_ptr<const logic::Symbol> locationSymbolEndLocation(const program::Function* function)
{
    return locationSymbol(function->name + "_end", 0);
}

std::shared_ptr<const logic::Symbol> lastIterationSymbol(const program::WhileStatement* statement, bool twoTraces)
{
    std::vector<const logic::Sort*> argumentSorts;
    for (unsigned i=0; i < statement->enclosingLoops->size(); ++i)
    {
        argumentSorts.push_back(logic::Sorts::natSort());
    }
    if (twoTraces)
    {
        argumentSorts.push_back(logic::Sorts::traceSort());
    }
    return logic::Signature::fetchOrAdd("n" + statement->location, argumentSorts, logic::Sorts::natSort());
}

std::shared_ptr<const logic::Symbol> iteratorSymbol(const program::WhileStatement* whileStatement)
{
    return logic::Signature::varSymbol("It" + whileStatement->location, logic::Sorts::natSort());
}

std::shared_ptr<const logic::Symbol> trace1Symbol()
{
    return logic::Signature::fetchOrAdd("t1", {}, logic::Sorts::traceSort());
}
std::shared_ptr<const logic::Symbol> trace2Symbol()
{
    return logic::Signature::fetchOrAdd("t2", {}, logic::Sorts::traceSort());
}

void declareSymbolForProgramVar(const program::Variable* var)
{
    std::vector<const logic::Sort*> argSorts;
    if (!var->isConstant)
    {
        argSorts.push_back(logic::Sorts::timeSort());
    }
    if (var->isArray)
    {
        argSorts.push_back(logic::Sorts::intSort());
    }
    if(var->twoTraces)
    {
        argSorts.push_back(logic::Sorts::traceSort());
    }
    
    logic::Signature::add(var->name, argSorts, logic::Sorts::intSort());
}

void declareSymbolsForTraces()
{
    // declare trace symbols
    trace1Symbol();
    trace2Symbol();
}

// symbols get declared by constructing them once
void declareSymbolsForFunction(const program::Function* function, bool twoTraces)
{
    // recurse on statements
    for (const auto& statement : function->statements)
    {
        declareSymbolsForStatements(statement.get(), twoTraces);
    }
    
    // declare end-location of function
    locationSymbolEndLocation(function);
}

void declareSymbolsForStatements(const program::Statement* statement, bool twoTraces)
{
    // declare main location symbol
    locationSymbolForStatement(statement);
    
    if (statement->type() == program::Statement::Type::IfElse)
    {
        auto castedStatement = static_cast<const program::IfElse*>(statement);
        
        // declare end-location symbols for both branches
        locationSymbolLeftBranch(castedStatement);
        locationSymbolRightBranch(castedStatement);
        
        // recurse
        for (const auto& statementInBranch : castedStatement->ifStatements)
        {
            declareSymbolsForStatements(statementInBranch.get(), twoTraces);
        }
        for (const auto& statementInBranch : castedStatement->elseStatements)
        {
            declareSymbolsForStatements(statementInBranch.get(), twoTraces);
        }
    }
    else if (statement->type() == program::Statement::Type::WhileStatement)
    {
        auto castedStatement = static_cast<const program::WhileStatement*>(statement);
        
        // declare last iteration-symbol
        lastIterationSymbol(castedStatement, twoTraces);
        
        // recurse
        for (const auto& bodyStatement : castedStatement->bodyStatements)
        {
            declareSymbolsForStatements(bodyStatement.get(), twoTraces);
        }
    }
}

