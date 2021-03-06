/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_BOUNDED_ARRAY_H
#define OUTPOST_BOUNDED_ARRAY_H

#include <stddef.h>

#include "meta.h"

namespace outpost
{

/**
 * Wrapper for C style arrays with additional length information.
 *
 * \author  Fabian Greif
 */
template <typename T>
class BoundedArray
{
public:
    typedef typename outpost::remove_const<T>::type NonConstType;
    typedef const NonConstType ConstType;

    friend class BoundedArray<NonConstType>;
    friend class BoundedArray<ConstType>;

    /**
     * Initialize directly from a C style array.
     *
     * The array needs to still have all the type information attached and
     * must not have degraded to a pointer type.
     *
     * Example:
     * \code
     * uint8_t array[7];
     * BoundedArray<uint8_t> wrappedArray(array);
     * \endcode
     *
     * \param array
     *      Array with should be wrapped.
     */
    template <size_t N>
    explicit inline
    BoundedArray(T (&array)[N]) :
        mData(array),
        mNumberOfElements(N)
    {
    }

    inline
    BoundedArray(const BoundedArray& rhs) :
        mData(rhs.mData),
        mNumberOfElements(rhs.mNumberOfElements)
    {
    }

    inline BoundedArray&
    operator=(const BoundedArray& rhs)
    {
        mData = rhs.mData;
        mNumberOfElements = rhs.mNumberOfElements;
        return *this;
    }

    /**
     * Generate an empty array.
     */
    static inline BoundedArray
    empty()
    {
        BoundedArray array(0, 0);
        return array;
    }


    /**
     * Initialize from a pointer to an array.
     *
     * Example:
     * \code
     * uint8_t array[7];
     * BoundedArray<uint8_t> wrappedArray(&array[0], 7);
     * \endcode
     *
     * \param array
     *      Pointer to the first element of the array.
     * \param numberOfElements
     *      Number of elements in the array.
     */
    inline
    BoundedArray(T* array, size_t numberOfElements) :
        mData(array),
        mNumberOfElements(numberOfElements)
    {
    }

    /**
     * Get number of elements in the array.
     */
    inline size_t
    getNumberOfElements() const
    {
        return mNumberOfElements;
    }

    /**
     * Access elements of the array.
     *
     * \code
     * BoundedArray<uint8_t> array(...);
     *
     * for (size_t i = 0; i < array.getNumberOfElements(); ++i)
     * {
     *     array[i] = i;
     * }
     * \endcode
     *
     * \warning
     *      No out-of-bound error checking is performed.
     */
    inline T&
    operator[](size_t index) const
    {
        return mData[index];
    }

    inline T*
    begin() const
    {
        return &mData[0];
    }

    inline T*
    end() const
    {
        return &mData[mNumberOfElements];
    }

    inline
    operator BoundedArray<const T>() const
    {
        return BoundedArray<const T>(mData, mNumberOfElements);
    }

private:
    /// Pointer to the array
    T* mData;

    /// Size of array
    size_t mNumberOfElements;
};

template <typename T, size_t N>
static inline outpost::BoundedArray<T>
toArray(T (&array)[N])
{
    outpost::BoundedArray<T> a(array);
    return a;
}

}

#endif
