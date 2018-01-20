/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Jacob Walker, Sergey Lisitsyn, Heiko Strathmann, Roman Votyakov, 
 *          Soeren Sonnenburg, Wu Lin, Fernando Iglesias
 */

#ifndef _GAUSSIANPROCESSREGRESSION_H_
#define _GAUSSIANPROCESSREGRESSION_H_


#include <shogun/lib/config.h>
#include <shogun/machine/GaussianProcessMachine.h>
#include <shogun/machine/gp/Inference.h>
#include <shogun/features/Features.h>
#include <shogun/labels/Labels.h>

namespace shogun
{

class CInference;
class CFeatures;
class CLabels;

/** @brief Class GaussianProcessRegression implements regression based on
 * Gaussian Processes.
 */
class CGaussianProcessRegression : public CGaussianProcessMachine
{
public:
	/** problem type */
	MACHINE_PROBLEM_TYPE(PT_REGRESSION);

	/** default constructor */
	CGaussianProcessRegression();

	/** constructor
	 *
	 * @param method chosen inference method
	 */
	CGaussianProcessRegression(CInference* method);

	virtual ~CGaussianProcessRegression();

	/** apply regression to data
	 *
	 * @param data (test)data to be classified
	 * @return classified labels
	 */
	virtual CRegressionLabels* apply_regression(CFeatures* data=NULL);

	/** get predicted mean vector
	 *
	 * @return predicted mean vector
	 */
	SGVector<float64_t> get_mean_vector(CFeatures* data);

	/** get variance vector
	 *
	 * @return variance vector
	 */
	SGVector<float64_t> get_variance_vector(CFeatures* data);

	/** get classifier type
	 *
	 * @return classifier type GaussianProcessRegression
	 */
	virtual EMachineType get_classifier_type()
	{
		return CT_GAUSSIANPROCESSREGRESSION;
	}

	/** return name of the regression object
	 *
	 * @return name GaussianProcessRegression
	 */
	virtual const char* get_name() const { return "GaussianProcessRegression"; }

protected:
	/** train regression
	 *
	 * @param data training data
	 *
	 * @return whether training was successful
	 */
	virtual bool train_machine(CFeatures* data=NULL);

	/** check whether training labels are valid for regression
	 *
	 * @param lab training labels
	 *
	 * @return whether training labels are valid for regression
	 */
	virtual bool is_label_valid(CLabels *lab) const
	{
		return lab->get_label_type()==LT_REGRESSION;
	}
};
}
#endif /* _GAUSSIANPROCESSREGRESSION_H_ */
