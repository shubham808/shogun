/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Shubham Shukla
 */

#ifndef _ITERATIVELINEARMACHINE_H__
#define _ITERATIVELINEARMACHINE_H__

#include <shogun/lib/config.h>

#include <shogun/lib/SGVector.h>
#include <shogun/lib/common.h>
#include <shogun/machine/LinearMachine.h>

namespace shogun
{
	class CFeatures;
	class CLabels;

	class CIterativeLinearMachine : public CLinearMachine
	{
	public:
		/* default constructor */
		CIterativeLinearMachine();
		virtual ~CIterativeLinearMachine();

		/** train classifier
		  *
		  * @param data training data (parameter can be avoided if distance or
		  * kernel-based classifiers are used and distance/kernels are
		  * initialized with train data)
		  *
		  * @return whether training was successful
		  */
		virtual bool train_machine(CFeatures* data = NULL);

		/** Continue Training
		  *
		  * Continue to train after prematurely stopping
		* Implements the loop of iterations
		  *
		  * @return whether continue training was successful
		  */
		virtual bool continue_train();

	protected:
		/* Single Iteration implemented in an iterative model
		 * @params f features
		 * @params l labels
		 */
		virtual void iteration() = 0;

		/* init the model
		 * @params f features
		 * @params l labels
		 */
		virtual void init_model(CFeatures* data = NULL) = 0;

		/* Maximum Iterations */
		index_t m_max_iterations;
		/* Current iteration of training loop */
		index_t m_current_iteration;
		/* Completion status */
		bool m_complete;
	};
}
#endif
