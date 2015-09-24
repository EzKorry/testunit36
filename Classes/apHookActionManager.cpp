/*
 * apHookActionManager.cpp
 *
 *  Created on: 2015. 9. 20.
 *      Author: ±Ë≈¬»∆
 */

#include "apHookActionManager.h"

namespace arphomod {

template<class TString>
void apHookActionManager::addHook(TString&& hook) {

	// if hook not found,
	if(_actions.find(hook) == _actions.end()) {
		_actions.emplace(std::forward<TString>(hook), std::set<string>());
	}
}

template<class TFunc>
void apHookActionManager::addAction(const std::string& hook, const std::string& tag, TFunc&& action) {


	addHook(hook);

	// if tag not found,
	_actions[hook].emplace(tag);
	_tagToFunc.emplace(tag, std::forward<TFunc>(action));


}

// run hook. then all that function will be invoked.
void apHookActionManager::runHook(const std::string& hook) {

	// if hook found,
	if(_actions.find(hook) != _actions.end()) {

		for(auto& item : _actions[hook]) {

			_tagToFunc[item]();
		}
	}
}

// remove hook.
void apHookActionManager::removeHook(const std::string& hook) {

	// if hook found,
	if(_actions.find(hook) != _actions.end()) {

		for(auto& item: _actions[hook]) {

			_tagToFunc.erase(item);

		}
		_actions.erase(hook);
	}

}

// remove Action.
void apHookActionManager::removeAction(const std::string& hook, const std::string& tag) {
	auto actionIt = _actions.find(hook);
	if(actionIt != _actions.end()) {
		auto& set = (*actionIt).second;
		auto funcIt = set.find(tag);
		if(funcIt != set.end()) {
			_tagToFunc.erase(tag);
			set.erase(funcIt);
		}

	}

}



apHookActionManager::apHookActionManager()  = default;

apHookActionManager::~apHookActionManager() = default;





} /* namespace arphomod */
