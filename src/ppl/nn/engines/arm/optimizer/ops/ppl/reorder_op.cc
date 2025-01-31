// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "ppl/nn/engines/arm/optimizer/ops/ppl/reorder_op.h"
#include "ppl/nn/engines/arm/kernels/ppl/reorder_kernel.h"
using namespace std;
using namespace ppl::common;

namespace ppl { namespace nn { namespace arm {

RetCode ReorderOp::Init(const OptKernelOptions& options) {
    infer_dims_func_ = [](InputOutputInfo* info) -> RetCode {
        auto& input = *info->GetInput<TensorImpl>(0)->GetShape();
        auto& output = *info->GetOutput<TensorImpl>(0)->GetShape();
        if (output.GetDataFormat() == DATAFORMAT_N8CX && input.GetDimCount() < 3) {
            auto padded_output_shape = PadShapeTo3Dims(input);
            output.Reshape(padded_output_shape.GetDims(), padded_output_shape.GetDimCount());
        } else {
            if (input.IsScalar()) {
                output.ReshapeAsScalar();
            } else {
                output.Reshape(input.GetDims(), input.GetDimCount());
            }
        }
        return RC_SUCCESS;
    };

    infer_type_func_ = PassiveInferType;
    return RC_SUCCESS;
}

KernelImpl* ReorderOp::CreateKernelImpl() const {
    return CreateKernelImplWithoutParam<ReorderKernel>();
}

}}} // namespace ppl::nn::ARM
