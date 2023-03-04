/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "api.hpp"
#include "../eid.hpp"
#include <exception>
#include <string>

namespace dci::exception
{
    std::exception_ptr API_DCI_EXCEPTION buildInstance(const Eid& eid, const std::exception_ptr& cause = std::exception_ptr());

    template <class E, class... Args>
    std::exception_ptr buildInstance(Args&&... args) requires std::is_constructible_v<E, Args&&...>;

    template <class E, class... Args>
    std::exception_ptr buildInstance(const std::exception_ptr& cause, Args&&... args) requires std::is_constructible_v<E, Args&&...>;
}

namespace dci::exception
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... Args>
    std::exception_ptr buildInstance(Args&&... args) requires std::is_constructible_v<E, Args&&...>
    {
        return std::make_exception_ptr(E{std::forward<Args>(args)...});
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    template <class E, class... Args>
    std::exception_ptr buildInstance(const std::exception_ptr& cause, Args&&... args) requires std::is_constructible_v<E, Args&&...>
    {
        if(cause)
        {
            try
            {
                try { std::rethrow_exception(cause); }
                catch (...)
                {
                    std::throw_with_nested(E{std::forward<Args>(args)...});
                }
            }
            catch(...)
            {
                return std::current_exception();
            }
        }

        return buildInstance<E>(std::forward<Args>(args)...);
    }

}
