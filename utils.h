/** \namespace utils \brief A collection of utility functions
 *
 * This namespace is a collection of utility functions, which don't belong in any class and which should be useful.
 */

namespace utils {

	/** Allocates memory for an array and initializes its elements with a given value.
	 * \param[in] length The length of the array.
	 * \param[in] value The value with which the array will be initialized
	 * \returns A pointer to the array
	 */
	double *createArray(const int length, const double value);
}
