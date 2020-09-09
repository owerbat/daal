/* file: bf_knn_classification_predict_kernel_impl.i */
/*******************************************************************************
* Copyright 2014-2020 Intel Corporation
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

#ifndef __BF_KNN_CLASSIFICATION_PREDICT_KERNEL_IMPL_I__
#define __BF_KNN_CLASSIFICATION_PREDICT_KERNEL_IMPL_I__

#include "algorithms/engines/engine.h"
#include "services/daal_defines.h"

#include "algorithms/k_nearest_neighbors/bf_knn_classification_model.h"
#include "src/algorithms/k_nearest_neighbors/bf_knn_classification_predict_kernel.h"
#include "src/algorithms/k_nearest_neighbors/oneapi/bf_knn_classification_model_ucapi_impl.h"
#include "src/algorithms/k_nearest_neighbors/bf_knn_impl.i"
#include "src/services/service_data_utils.h"
#include "src/data_management/service_numeric_table.h"

namespace daal
{
namespace algorithms
{
namespace bf_knn_classification
{
namespace prediction
{
namespace internal
{
template <typename algorithmFPType, CpuType cpu>
services::Status KNNClassificationPredictKernel<algorithmFPType, cpu>::compute(const NumericTable * data, const classifier::Model * m,
                                                                               NumericTable * label, NumericTable * indices, NumericTable * distances,
                                                                               const daal::algorithms::Parameter * par)
{
    const Model * convModel              = static_cast<const Model *>(m);
    NumericTableConstPtr trainDataTable  = convModel->impl()->getData();
    NumericTableConstPtr trainLabelTable = convModel->impl()->getLabels();

    const Parameter * const parameter   = static_cast<const Parameter *>(par);
    const uint32_t k                    = parameter->k;
    const uint32_t nClasses             = parameter->nClasses;
    const VoteWeights voteWeights       = parameter->voteWeights;
    const DAAL_UINT64 resultsToEvaluate = parameter->resultsToEvaluate;

    const size_t nTest = data->getNumberOfRows();
    daal::internal::WriteRows<algorithmFPType, cpu> distancesRows(distances, 0, nTest);
    daal::internal::WriteRows<int, cpu> indicesRows(indices, 0, nTest);
    algorithmFPType * neighborsDistances = distancesRows.get();
    int * neighborsIndices               = indicesRows.get();
    DAAL_CHECK_MALLOC(neighborsDistances);
    DAAL_CHECK_MALLOC(neighborsIndices);

    daal::algorithms::bf_knn_classification::internal::BruteForceNearestNeighbors<algorithmFPType, cpu> bfnn;
    bfnn.kNeighbors(k, nClasses, voteWeights, 0, resultsToEvaluate, trainDataTable.get(), data, trainLabelTable.get(), label, neighborsIndices,
                    neighborsDistances);
    return services::Status();
}

} // namespace internal
} // namespace prediction
} // namespace bf_knn_classification
} // namespace algorithms
} // namespace daal

#endif
