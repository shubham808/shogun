/*
* This software is distributed under BSD 3-clause license (see LICENSE file).
*
* Authors: Shubham Shukla
*/

#include <shogun/base/progress.h>
#include <shogun/lib/common.h>
#include <shogun/lib/config.h>
#include <shogun/machine/IterativeLinearMachine.h>

using namespace shogun;

CIterativeLinearMachine::CIterativeLinearMachine() : CLinearMachine()
{
	m_current_iteration = 0;
	m_complete = false;

	SG_ADD(
	    &m_current_iteration, "current_iteration",
	    "Current Iteration of training", MS_NOT_AVAILABLE);
	SG_ADD(
	    &m_max_iterations, "max_iterations", "Maximum number of Iterations",
	    MS_AVAILABLE);
	SG_ADD(&m_complete, "complete", "Convergence status", MS_NOT_AVAILABLE);
}

CIterativeLinearMachine::~CIterativeLinearMachine()
{
	SG_UNREF(m_continue_features);
}

bool CIterativeLinearMachine::train_machine(CFeatures* data)
{
	m_current_iteration = 0;
	m_complete = false;
	init_model(data);
	return continue_train();
}

bool CIterativeLinearMachine::continue_train()
{
	reset_computation_variables();
	auto pb = SG_PROGRESS(range(m_max_iterations));
	while (m_current_iteration < m_max_iterations && !m_complete)
	{
		COMPUTATION_CONTROLLERS
		iteration();
		m_current_iteration++;
		pb.print_progress();
	}
	pb.complete();
	return m_complete;
}
