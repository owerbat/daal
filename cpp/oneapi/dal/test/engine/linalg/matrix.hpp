/*******************************************************************************
* Copyright 2021 Intel Corporation
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

#pragma once

#include "oneapi/dal/array.hpp"
#include "oneapi/dal/table/common.hpp"
#include "oneapi/dal/table/row_accessor.hpp"

namespace oneapi::dal::test::engine::linalg {

enum class layout {
    row_major,
    column_major,
};

inline constexpr layout transpose_layout(layout l) {
    return (l == layout::row_major) ? layout::column_major : layout::row_major;
}

class shape {
public:
    shape(std::int64_t row_count = 0, std::int64_t column_count = 0) {
        ONEDAL_ASSERT(row_count >= 0, "Row count must be non-negative");
        ONEDAL_ASSERT(column_count >= 0, "Column count must be non-negative");
        if (row_count == 0 || column_count == 0) {
            ONEDAL_ASSERT(row_count == 0,
                          "Both row count and column count must be zeros, "
                          "but got non-zero row count");
            ONEDAL_ASSERT(column_count == 0,
                          "Both row count and column count must be zeros, "
                          "but got non-zero column count");
        }
        ONEDAL_ASSERT_MUL_OVERFLOW(std::int64_t, row_count, column_count);
        shape_[0] = row_count;
        shape_[1] = column_count;
    }

    std::int64_t operator[](std::int64_t i) const {
        ONEDAL_ASSERT(i == 0 || i == 1, "Index can be only 0 or 1");
        return shape_[i];
    }

    std::int64_t get_row_count() const {
        return shape_[0];
    }

    std::int64_t get_column_count() const {
        return shape_[1];
    }

    std::int64_t get_count() const {
        return get_row_count() * get_column_count();
    }

    shape t() const {
        return shape{ shape_[1], shape_[0] };
    }

    bool operator==(const shape& other) const {
        return shape_[0] == other.shape_[0] && shape_[1] == other.shape_[1];
    }

    bool operator!=(const shape& other) const {
        return !(*this == other);
    }

private:
    std::int64_t shape_[2];
};

template <layout lyt>
class matrix_base {
public:
    constexpr layout get_layout() const {
        return lyt;
    }

    std::int64_t get_row_count() const {
        return shape_.get_row_count();
    }

    std::int64_t get_column_count() const {
        return shape_.get_column_count();
    }

    std::int64_t get_count() const {
        return shape_.get_count();
    }

    const shape& get_shape() const {
        return shape_;
    }

    std::int64_t get_stride() const {
        return stride_;
    }

    std::int64_t get_linear_index(std::int64_t i, std::int64_t j) const {
        ONEDAL_ASSERT(i >= 0 && i < get_row_count(), "Row index is out of range");
        ONEDAL_ASSERT(j >= 0 && j < get_column_count(), "Column index is out of range");
        if constexpr (lyt == layout::row_major) {
            return i * get_stride() + j;
        }
        else {
            return j * get_stride() + i;
        }
    }

protected:
    explicit matrix_base(const shape& s, std::int64_t stride) : shape_(s), stride_(stride) {
        if constexpr (lyt == layout::row_major) {
            ONEDAL_ASSERT(stride >= s.get_column_count(),
                          "Stride must be greater than "
                          "column count in row-major layout");
        }
        else if constexpr (lyt == layout::column_major) {
            ONEDAL_ASSERT(stride >= s.get_row_count(),
                          "Stride must be greater than "
                          "row count in column-major layout");
        }
    }

    explicit matrix_base(const shape& s) : matrix_base(s, get_default_stride(s)) {}

protected:
    static std::int64_t get_default_stride(const shape& s) {
        if constexpr (lyt == layout::row_major) {
            return s.get_column_count();
        }
        else {
            return s.get_row_count();
        }
        return 0;
    }

private:
    shape shape_;
    std::int64_t stride_;
};

template <typename Float, layout lyt = layout::row_major>
class matrix : public matrix_base<lyt> {
public:
    template <typename Float_, layout lyt_>
    friend class matrix;

    using base = matrix_base<lyt>;
    using base::get_row_count;
    using base::get_column_count;
    using base::get_count;
    using base::get_shape;
    using base::get_stride;
    using base::get_linear_index;

    static matrix wrap(const Float* data, const shape& s) {
        return matrix{ array<Float>::wrap(data, s.get_count()), s };
    }

    static matrix wrap(const array<Float>& x) {
        return matrix{ x, { 1, x.get_count() } };
    }

    static matrix wrap(const array<Float>& x, const shape& s) {
        return matrix{ x, s };
    }

    static matrix wrap(const table& t) {
        if constexpr (lyt != layout::row_major) {
            // TODO: Figure out how to use column-major layout
            throw unimplemented{ dal::detail::error_messages::unsupported_data_layout() };
        }
        const auto t_flat = row_accessor<const Float>{ t }.pull();
        return wrap(t_flat, { t.get_row_count(), t.get_column_count() });
    }

    static matrix wrap(const matrix<Float>& x) {
        return matrix{ x.get_array(), { x.get_row_count(), x.get_column_count() } };
    }

    template <typename NdArrayLike>
    static matrix wrap_nd(const NdArrayLike& x) {
        static_assert(NdArrayLike::axis_count_v == 1 || NdArrayLike::axis_count_v == 2);

        if constexpr (NdArrayLike::axis_count_v == 1) {
            return wrap(x.get_data(), { 1, x.get_dimension(0) });
        }

        return wrap(x.get_data(), { x.get_dimension(0), x.get_dimension(1) });
    }

    static matrix empty(const shape& s) {
        return wrap(array<Float>::empty(s.get_count()), s);
    }

    template <typename Filler>
    static matrix full(const shape& s, Filler&& filler) {
        return empty(s).fill(std::forward<Filler>(filler));
    }

    static matrix ones(const shape& s) {
        return full(s, Float(1));
    }

    static matrix zeros(const shape& s) {
        return full(s, Float(0));
    }

    static matrix eye(std::int64_t dim) {
        auto m = zeros({ dim, dim });
        Float* data = m.get_mutable_data();
        for (std::int64_t i = 0; i < dim; i++) {
            data[i * dim + i] = Float(1);
        }
        return m;
    }

    matrix() : base({ 0, 0 }, 0) {}

    const array<Float>& get_array() const {
        return x_;
    }

    array<Float>& get_array() {
        return x_;
    }

    const Float* get_data() const {
        return x_.get_data();
    }

    Float* get_mutable_data() const {
        return x_.get_mutable_data();
    }

    matrix& need_mutable_data() {
        x_.need_mutable_data();
        return *this;
    }

    bool has_data() const {
        return get_count() > 0;
    }

    bool has_mutable_data() const {
        return has_data() && x_.has_mutable_data();
    }

    auto t() const {
        return matrix<Float, transpose_layout(lyt)>{ x_, get_shape().t(), get_stride() };
    }

    Float get(std::int64_t linear_i) const {
        return get_data()[linear_i];
    }

    Float get(std::int64_t i, std::int64_t j) const {
        return get_data()[get_linear_index(i, j)];
    }

    Float& set(std::int64_t linear_i) const {
        return get_mutable_data()[linear_i];
    }

    Float& set(std::int64_t i, std::int64_t j) {
        return get_mutable_data()[get_linear_index(i, j)];
    }

    auto& fill(Float filler) {
        Float* ptr = get_mutable_data();
        for (std::int64_t i = 0; i < get_count(); i++) {
            ptr[i] = filler;
        }
        return *this;
    }

    template <typename T = Float, typename = std::enable_if_t<std::is_same_v<T, bool>>>
    bool all() const {
        bool result = true;
        for (std::int64_t i = 0; i < get_count(); i++) {
            result = result && get(i);
        }
        return result;
    }

    template <typename T = Float, typename = std::enable_if_t<std::is_same_v<T, bool>>>
    bool any() const {
        bool result = false;
        for (std::int64_t i = 0; i < get_count(); i++) {
            result = result || get(i);
        }
        return result;
    }

private:
    explicit matrix(const array<Float>& x, const shape& s, std::int64_t stride)
            : base(s, stride),
              x_(x) {
        ONEDAL_ASSERT(s.get_count() <= x.get_count(),
                      "Element count in matrix does not match "
                      "element count in the provided array");
    }

    explicit matrix(const array<Float>& x, const shape& s)
            : matrix(x, s, base::get_default_stride(s)) {}

    array<Float> x_;
};

} // namespace oneapi::dal::test::engine::linalg
