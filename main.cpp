#include <iostream>
#include <stdexcept>
#include "forw_list.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[]) {
	try {
		ForwList::FL<int> fl1;
		default_random_engine d;
		uniform_int_distribution<> u(0, 10000);
		for (int i = 0; i != 5; ++i) {
			fl1.insert_after(fl1.before_begin(), i);
		}
		fl1.insert_after(++++++++fl1.cbegin(), 111);
		cout << "fl1\t"; for (int i : fl1) cout << i << '\t'; cout << endl;
		ForwList::FL<int> fl2;
		cout << "fl1\t"; for (int i : fl1) cout << i << '\t'; cout << endl;
		cout << "fl2\t"; for (int i : fl2) cout << i << '\t'; cout << endl;
		fl2 = fl1;
		cout << "fl1\t"; for (int i : fl1) cout << i << '\t'; cout << endl;
		cout << "fl2\t"; for (int i : fl2) cout << i << '\t'; cout << endl;
		fl2.erase_after(fl2.begin());
		cout << "fl1\t"; for (int i : fl1) cout << i << '\t'; cout << endl;
		cout << "fl2\t"; for (int i : fl2) cout << i << '\t'; cout << endl;
		fl2 = fl1;
		fl2.erase_after(fl2.cbegin());
		fl2.erase_after(++++++fl2.begin());
		cout << "fl1\t"; for (int i : fl1) cout << i << '\t'; cout << endl;
		cout << "fl2\t"; for (int i : fl2) cout << i << '\t'; cout << endl;
		fl2.insert_after(fl2.cbefore_begin(), 999);
		fl2.insert_after(++fl2.before_begin(), 888);
		cout << "fl1\t"; for (int i : fl1) cout << i << '\t'; cout << endl;
		cout << "fl2\t"; for (int i : fl2) cout << i << '\t'; cout << endl;
		swap(fl1, fl2);
		cout << "fl1\t"; for (int i : fl1) cout << i << '\t'; cout << endl;
		cout << "fl2\t"; for (int i : fl2) cout << i << '\t'; cout << endl;
		for (ForwList::FL<int>::iterator i = fl2.begin(); i != fl2.end(); ++i) cout << *i << "_\t"; cout << endl;
	}
	catch (exception _e) {
		cout << _e.what() << endl;
	}
	

	char z1;
	cin >> z1;
	return 0;
}
