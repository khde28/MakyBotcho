#include "StateManager.h"

void StateManager::push(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void StateManager::pop() {
    if (!states.empty()) states.pop();
}

void StateManager::change(std::unique_ptr<State> state) {
    if (!states.empty()) states.pop();
    states.push(std::move(state));
}

State* StateManager::current() {
    if (states.empty()) return nullptr;
    return states.top().get();
}
