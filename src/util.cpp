#include "util.h"

/**
 * Check whether the state is all invalid or not
 */
bool checkAllI(const string &state){
	for (int i = 0; i < state.length(); i++)
		if (state[i] != 'I')
			return false;
	return true;
}

/**
 * Getting the protocol type(type), current state(oldState)
 * and actionNum, output next state
 * actionNum = 4 * core index + operation index
 */
string action(string type, const string &oldState, int actionNum) {
	bool allI = checkAllI(oldState);
	char op;
	char state;
	string states_(oldState);
	int num = states_.length();

	for (int i = 0; i < num; i++) {
		state = states_[i];

		//Check whether the action is my action or not
		if (actionNum / 4 == i) {
			op = table_[actionNum % 4];
		} else {
			op = table_[actionNum % 4 + 3];
		}

		// MESI
		if (!type.compare("MESI") || !type.compare("PMESI")) {
			if (state == 'M') {
				if (op == 'L') {
					state = 'S';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			} else if (state == 'I') {
				if (op == 'e') {
					return "Invalid";
				} else if (op == 's') {
					state = 'M';
				} else if (op == 'l' && allI == true) {
					state = 'E';
				} else if (op == 'l') {
					state = 'S';
				}
			} else if (state == 'S') {
				if (op == 's') {
					state = 'M';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			} else {
				if (op == 's') {
					state = 'M';
				} else if (op == 'L') {
					state = 'S';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			}

		// MOSI
		} else if (!type.compare("MOSI")) {
			if (state == 'M') {
				if (op == 'e' || op == 'S') {
					state = 'I';
				} else if (op == 'L') {
					state = 'O';
				}
			} else if(state == 'O') {
				if(op == 'e' || op == 'S')
					state = 'I';
				else if(op == 's')
					state = 'M';
			}
			else if (state == 'I') {
				if (op == 'e') {
					return "Invalid";
				} else if (op == 's') {
					state = 'M';
				} else if (op == 'l') {
					state = 'S';
				}
			} else if (state == 'S') {
				if (op == 's') {
					state = 'M';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			} else {
				if (op == 's') {
					state = 'M';
				} else if (op == 'L') {
					state = 'S';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			}

		// PMOSI
		} else if (!type.compare("PMOSI")) {
			if (state == 'M') {
				if (op == 'e' || op == 'S') {
					state = 'I';
				} else if (op == 'L') {
					state = 'O';
				}
			} else if(state == 'O') {
				if(op == 'e' || op == 'S')
					state = 'I';
				else if(op == 's')
					state = 'M';
				// different from MOSI
				else if (op == 'q')
					state = 'S';
			}
			else if (state == 'I') {
				if (op == 'e') {
					return "Invalid";
				} else if (op == 's') {
					state = 'M';
				} else if (op == 'l') {
					state = 'S';
				}
			} else if (state == 'S') {
				if (op == 's') {
					state = 'M';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			} else {
				if (op == 's') {
					state = 'M';
				} else if (op == 'L') {
					state = 'S';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			}

		// MSI
		} else if (!type.compare("MSI")) {
			if (state == 'M') {
				if (op == 'L') {
					state = 'S';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			} else if (state == 'I') {
				if (op == 'e') {
					return "Invalid";
				} else if (op == 's') {
					state = 'M';
				}  else if (op == 'l') {
					state = 'S';
				}
			} else if (state == 'S') {
				if (op == 's') {
					state = 'M';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			} else {
				if (op == 's') {
					state = 'M';
				} else if (op == 'L') {
					state = 'S';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			}

		// MOESI
		} else if (!type.compare("MOESI")) {

			if (state == 'M') {
				if (op == 'e' || op == 'S') {
					state = 'I';
				} else if (op == 'L') {
					state = 'O';
				}
			} else if(state == 'O')	{
				if(op == 'e' || op == 'S')
					state = 'I';
				else if(op == 's')
					state = 'M';
				// different from MOSI
				else if (op == 'q')
					state = 'S';
			}
			else if (state == 'I') {
				if (op == 'e') {
					return "Invalid";
				} else if (op == 's') {
					state = 'M';
				} else if (op == 'l' && allI == true) {
					state = 'E';
				} else if (op == 'l') {
					state = 'S';
				}
			} else if (state == 'S') {
				if (op == 's') {
					state = 'M';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			} else {
				if (op == 's') {
					state = 'M';
				} else if (op == 'L') {
					state = 'S';
				} else if (op == 'e' || op == 'S') {
					state = 'I';
				}
			}
		}

		states_[i] = state;
	}
	return states_;	

}
