/*
  libpuTools - Basic types/algorithms/containers

  Copyright (C) 2006-2016 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef PUTOOLS_STL_CONTAINER_UTILITY_H
#define PUTOOLS_STL_CONTAINER_UTILITY_H

namespace miutil {

/**
 * @addtogroup miutil
 * @{
 */


/**
 * eraseElement, erase an element in the container at the position
 * given with the iterator it. \a It must be an valid iterator for the
 * container.
 *
 * On return an iterator for the next element in the container is returned.
 *
 * @param container The container to erase an element from.
 * @param it The element to erase from the container.
 * @return An iterator pointing to the next element in the container.
 */
template <typename T>
typename T::iterator
eraseElement(T &container, typename T::iterator it)
{
  if (it == container.end())
    return it;
  else
    return container.erase(it);
}


/**
 * eraseElementIf, erase an element in the container at the position
 * given with the iterator it, if the condition is true.
 *
 * \a It must be an valid iterator for the container.
 *
 * On return an iterator for the next element in the container is returned.
 * ie. The iterator returned is for the next element in the container
 * regardless of the element was erased or not.
 *
 * @param container The container to erase an element from.
 * @param it The element to erase from the container.
 * @param condition erase the element at the iterator \a it if
 *                  true. If false just advance to the next element.
 * @return An iterator pointing to the next element in the container.
 */
template <typename T>
typename T::iterator
eraseElementIf(T &container, typename T::iterator it, bool condition)
{
  if (it == container.end())
    return it;
  else if (condition)
    return container.erase(it);
  else
    return ++it;
}

/** @} */

}

#endif // PUTOOLS_STL_CONTAINER_UTILITY_H
