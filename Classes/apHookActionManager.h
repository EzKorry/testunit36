/*
 * apHookActionManager.h
 *
 *  Created on: 2015. 9. 20.
 *      Author: ±Ë≈¬»∆
 */

#ifndef APHOOKACTIONMANAGER_H_
#define APHOOKACTIONMANAGER_H_
#include <string>

namespace arphomod {
using namespace std;

class apHookActionManager {
public:
	apHookActionManager();

	void addHook(string hook);
	void removeHook(string hook);
	void runHook(string hook);

	void addAction(string hook, function<void()> action, string tag);

	void removeAction(string tag);


	static shared_ptr<apHookActionManager> getInstance() {

		if(_sp == nullptr) {
			_sp = make_shared<apHookActionManager>();
		}
		return _sp;
	}

	virtual ~apHookActionManager();

private:
	string _defaultTag {""};
	unordered_map<string,set<string>> _actions;
	unordered_map<string, function<>
	static shared_ptr<apHookActionManager> _sp;
};

} /* namespace arphomod */

#endif /* APHOOKACTIONMANAGER_H_ */
