/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Fernando Iglesias, Shashwat Lal Das, Thoralf Klein, 
 *          Soeren Sonnenburg
 */

#include <shogun/loss/SquaredHingeLoss.h>
#include <shogun/io/SGIO.h>

using namespace shogun;

float64_t CSquaredHingeLoss::loss(float64_t z)
{
	return (z < 1) ? 0.5 * (1-z) * (1-z) : 0;
}

float64_t CSquaredHingeLoss::first_derivative(float64_t z)
{
	return (z < 1) ? z-1 : 0;
}

float64_t CSquaredHingeLoss::second_derivative(float64_t z)
{
	return (z < 1) ? 1 : 0;
}

float64_t CSquaredHingeLoss::get_update(float64_t prediction, float64_t label, float64_t eta_t, float64_t norm)
{
	SG_NOTIMPLEMENTED
	return -1;
}

float64_t CSquaredHingeLoss::get_square_grad(float64_t prediction, float64_t label)
{
	SG_NOTIMPLEMENTED
	return -1;
}
