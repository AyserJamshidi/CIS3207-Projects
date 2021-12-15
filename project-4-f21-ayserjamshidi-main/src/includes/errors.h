//
// Created by ayserjamshidi on 11/19/21.
//

#ifndef PROJECT_4_F21_AYSERJAMSHIDI_ERRORS_H
#define PROJECT_4_F21_AYSERJAMSHIDI_ERRORS_H

#include <iostream>
#include <string>

class Errors {
public:
	static const int FORK_FAILURE = 1,
	SIGNAL_UNKNOWN = 2,
	PTHREAD_CREATE_FAILURE = 3;

	static void ExitWithErrorMessage(int error) {
		std::string err_msg;

		switch (error) {
			case FORK_FAILURE:
				err_msg = "Failure during initial child process fork.";
				break;
			case SIGNAL_UNKNOWN:
				err_msg = "Failure when a child was attempting to handle signals.";
				break;
			case PTHREAD_CREATE_FAILURE:
				err_msg = "Failure when a child was attempting to create a thread.";
				break;
			default:
				err_msg = "Unknown error: " + std::to_string(error);
				break;
		}

		std::cout << err_msg << std::endl;
		exit(error);
	}
};

#endif //PROJECT_4_F21_AYSERJAMSHIDI_ERRORS_H
