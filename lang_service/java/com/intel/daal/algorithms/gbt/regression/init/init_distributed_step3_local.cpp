/* file: init_distributed_step3_local.cpp */
/*******************************************************************************
* Copyright 2014-2019 Intel Corporation.
*
* This software and the related documents are Intel copyrighted  materials,  and
* your use of  them is  governed by the  express license  under which  they were
* provided to you (License).  Unless the License provides otherwise, you may not
* use, modify, copy, publish, distribute,  disclose or transmit this software or
* the related documents without Intel's prior written permission.
*
* This software and the related documents  are provided as  is,  with no express
* or implied  warranties,  other  than those  that are  expressly stated  in the
* License.
*******************************************************************************/

#include <jni.h>
#include "daal.h"
#include "com_intel_daal_algorithms_gbt_regression_init_InitDistributedStep3Local.h"

#include "common_helpers.h"

USING_COMMON_NAMESPACES()
using namespace daal::algorithms::gbt::regression::init;

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_gbt_regression_init_InitDistributedStep3Local_cInit
(JNIEnv *, jobject, jint prec, jint method, jlong maxBins)
{
    return jniDistributed<step3Local, gbt::regression::init::Method, Distributed, defaultDense>::newObj(prec, method, maxBins);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_gbt_regression_init_InitDistributedStep3Local_cInitParameter
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniDistributed<step3Local, gbt::regression::init::Method, Distributed, defaultDense>::getBaseParameter(prec, method, algAddr);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_gbt_regression_init_InitDistributedStep3Local_cGetInput
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniDistributed<step3Local, gbt::regression::init::Method, Distributed, defaultDense>::getInput(prec, method, algAddr);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_gbt_regression_init_InitDistributedStep3Local_cGetPartialResult
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniDistributed<step3Local, gbt::regression::init::Method, Distributed, defaultDense>::getPartialResult(prec, method, algAddr);
}

JNIEXPORT void JNICALL Java_com_intel_daal_algorithms_gbt_regression_init_InitDistributedStep3Local_cSetPartialResult
(JNIEnv *, jobject, jlong algAddr, jint prec, jint method, jlong partialResultAddr)
{
    jniDistributed<step3Local, gbt::regression::init::Method, Distributed, defaultDense>::
        setPartialResult<gbt::regression::init::DistributedPartialResultStep3>(prec, method, algAddr, partialResultAddr);
}

JNIEXPORT jlong JNICALL Java_com_intel_daal_algorithms_gbt_regression_init_InitDistributedStep3Local_cClone
(JNIEnv *env, jobject thisObj, jlong algAddr, jint prec, jint method)
{
    return jniDistributed<step3Local, gbt::regression::init::Method, Distributed, defaultDense>::getClone(prec, method, algAddr);
}