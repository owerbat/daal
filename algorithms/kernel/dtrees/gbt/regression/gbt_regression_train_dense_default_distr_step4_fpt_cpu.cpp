/* file: gbt_regression_train_dense_default_distr_step4_fpt_cpu.cpp */
/*******************************************************************************
* Copyright 2014-2019 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/*
//++
//  Implementation of gradient boosted trees regression training functions for the default method
//--
*/

#include "gbt_regression_train_kernel.h"
#include "gbt_regression_train_container.h"
#include "gbt_regression_train_dense_default_distr_impl.i"

namespace daal
{
namespace algorithms
{
namespace gbt
{
namespace regression
{
namespace training
{
namespace interface1
{
template class DistributedContainer<step4Local, DAAL_FPTYPE, defaultDense, DAAL_CPU>;
}
namespace internal
{
template class RegressionTrainDistrStep4Kernel<DAAL_FPTYPE, defaultDense, DAAL_CPU>;
}

}
}
}
}
}
