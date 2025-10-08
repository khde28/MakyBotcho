#pragma once
#include <memory>
#include <stack>
#include "State.h"

// Gestiona una pila de estados (puedes apilar menú, pausa, etc.)
class StateManager
{
private:
    std::stack<std::unique_ptr<State>> states;

public:
    void push(std::unique_ptr<State> state);
    void pop();
    void change(std::unique_ptr<State> state);
    State *current();

    bool empty() const { return states.empty(); }
};
