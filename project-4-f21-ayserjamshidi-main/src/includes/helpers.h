//
// Created by ayserjamshidi on 11/18/21.
//

#ifndef PROJECT_4_F21_AYSERJAMSHIDI_HELPERS_H
#define PROJECT_4_F21_AYSERJAMSHIDI_HELPERS_H

class Helpers {
public:
	class Generics {
	public:
		static int rng(int min, int max);

		static unsigned long rng(unsigned long min, unsigned long max);

		/**
		 * @brief Makes the thread that calls this function sleep a random amount of milliseconds between [10, 100]
		 */
		static void sleep();

		static const char * SigusrText(int num);
	};
};

#endif //PROJECT_4_F21_AYSERJAMSHIDI_HELPERS_H
