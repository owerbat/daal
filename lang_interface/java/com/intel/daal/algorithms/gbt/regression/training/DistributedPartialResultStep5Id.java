/* file: DistributedPartialResultStep5Id.java */
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

/**
 * @ingroup gbt_distributed
 * @{
 */
package com.intel.daal.algorithms.gbt.regression.training;

import com.intel.daal.utils.*;
/**
 * <a name="DAAL-CLASS-ALGORITHMS__GBT__REGRESSION__TRAINING__DISTRIBUTEDPARTIALRESULTSTEP5ID"></a>
 * @brief Available identifiers of partial results of the model-based training in the fifth step
 *        of the distributed processing mode
 */
public final class DistributedPartialResultStep5Id {
    private int _value;

    static {
        LibUtils.loadLibrary();
    }

    /**
     * Constructs the partial result object identifier using the provided value
     * @param value     Value corresponding to the partial result object identifier
     */
    public DistributedPartialResultStep5Id(int value) {
        _value = value;
    }

    /**
     * Returns the value corresponding to the partial result object identifier
     * @return Value corresponding to the partial result object identifier
     */
    public int getValue() {
        return _value;
    }

    private static final int step5TreeStructureValue = 0;
    private static final int step5TreeOrderValue = 1;

    public static final DistributedPartialResultStep5Id step5TreeStructure = new DistributedPartialResultStep5Id(step5TreeStructureValue);
        /*!<  */
    public static final DistributedPartialResultStep5Id step5TreeOrder = new DistributedPartialResultStep5Id(step5TreeOrderValue);
        /*!<  */
}
/** @} */