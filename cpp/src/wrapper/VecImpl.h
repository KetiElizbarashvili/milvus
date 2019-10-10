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


#pragma once

#include "knowhere/index/vector_index/VectorIndex.h"
#include "VecIndex.h"

#include <utility>
#include <memory>

namespace zilliz {
namespace milvus {
namespace engine {

class VecIndexImpl : public VecIndex {
 public:
    explicit VecIndexImpl(std::shared_ptr<zilliz::knowhere::VectorIndex> index, const IndexType &type)
        : index_(std::move(index)), type(type) {
    }

    Status
    BuildAll(const int64_t &nb,
             const float *xb,
             const int64_t *ids,
             const Config &cfg,
             const int64_t &nt,
             const float *xt) override;

    VecIndexPtr
    CopyToGpu(const int64_t &device_id, const Config &cfg) override;

    VecIndexPtr
    CopyToCpu(const Config &cfg) override;

    IndexType
    GetType() override;

    int64_t
    Dimension() override;

    int64_t
    Count() override;

    Status
    Add(const int64_t &nb, const float *xb, const int64_t *ids, const Config &cfg) override;

    zilliz::knowhere::BinarySet
    Serialize() override;

    Status
    Load(const zilliz::knowhere::BinarySet &index_binary) override;

    VecIndexPtr
    Clone() override;

    int64_t
    GetDeviceId() override;

    Status
    Search(const int64_t &nq, const float *xq, float *dist, int64_t *ids, const Config &cfg) override;

 protected:
    int64_t dim = 0;

    IndexType type = IndexType::INVALID;

    std::shared_ptr<zilliz::knowhere::VectorIndex> index_ = nullptr;
};

class IVFMixIndex : public VecIndexImpl {
 public:
    explicit IVFMixIndex(std::shared_ptr<zilliz::knowhere::VectorIndex> index, const IndexType &type)
        : VecIndexImpl(std::move(index), type) {
    }

    Status
    BuildAll(const int64_t &nb,
             const float *xb,
             const int64_t *ids,
             const Config &cfg,
             const int64_t &nt,
             const float *xt) override;

    Status
    Load(const zilliz::knowhere::BinarySet &index_binary) override;
};

class IVFHybridIndex : public IVFMixIndex {
 public:
    knowhere::QuantizerPtr
    LoadQuantizer(const Config& conf) override;

    Status
    SetQuantizer(const knowhere::QuantizerPtr& q) override;

    Status
    UnsetQuantizer() override;

    Status
    LoadData(const knowhere::QuantizerPtr &q, const Config &conf) override;
};

class BFIndex : public VecIndexImpl {
 public:
    explicit BFIndex(std::shared_ptr<zilliz::knowhere::VectorIndex> index) : VecIndexImpl(std::move(index),
                                                                                          IndexType::FAISS_IDMAP) {
    }

    ErrorCode
    Build(const Config &cfg);

    float *
    GetRawVectors();

    Status
    BuildAll(const int64_t &nb,
             const float *xb,
             const int64_t *ids,
             const Config &cfg,
             const int64_t &nt,
             const float *xt) override;

    int64_t *
    GetRawIds();
};

} // namespace engine
} // namespace milvus
} // namespace zilliz
