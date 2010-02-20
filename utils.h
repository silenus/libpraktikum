#include <TF1.h>
#include <TMath.h>

using namespace std;

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

	/** \brief Transforms an array with a given mathematical expression
 	 *
	 * This function transforms a given array in-place with a given mathematical expression. The expression needs to be in root syntax, because root is used for parsing the expression by creating a TF1 object from the expression.
	 * \param array The double array, which will be transformed. This parameter is both input and output, since it will be transformed in-place.
	 * \param[in] length The length of the array.
	 * \param[in] expression The mathematical expression in root syntax as used by the first constructor of TF1.
	 */
	void transform(double *array, const unsigned int length, const string &expression);

	/** \brief Transforms an array with a given TF1 object.
 	 *
	 * This function transforms a given array in-place with a given root TF1 object. This object can be created by a mathematical expression or with a pointer to a function.
	 * \param array The double array, which will be transformed. This parameter is both input and output, since it will be transformed in-place.
	 * \param[in] length The length of the array.
	 * \param[in] function The mathematical function, which will be used to transform the array
	 */
	void transform(double *array, const unsigned int length, TF1 *function);
}
