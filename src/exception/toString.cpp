/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include <dci/exception/toString.hpp>
#include <dci/exception.hpp>
#include <stdexcept>

namespace dci::exception
{
    std::string API_DCI_EXCEPTION toString(std::exception_ptr ptr)
    {
        std::string res;
        while(ptr)
        {
            if(!res.empty())
                res += ", caused by ";

            try
            {
                try
                {
                    std::rethrow_exception(ptr);
                }
                catch(const std::logic_error& e)
                {
                    res = res + "logic_error{" + e.what() + "}";
                    std::rethrow_if_nested(e);
                }
                catch(const std::runtime_error& e)
                {
                    res = res + "runtime_error{" + e.what() + "}";
                    std::rethrow_if_nested(e);
                }
                catch(const dci::Exception& e)
                {
                    res = res + std::string{e.name()} + "{" + e.what() + "}";
                    std::rethrow_if_nested(e);
                }
                catch(const std::exception& e)
                {
                    res = res + "exception{" + e.what() + "}";
                    std::rethrow_if_nested(e);
                }
                catch(const std::nested_exception& e)
                {
                    res = res + "unknown exception";
                    std::rethrow_exception(e.nested_ptr());
                }
                catch(...)
                {
                    res = res + "unknown exception";
                }

                //no nesteds more
                ptr = std::exception_ptr();
            }
            catch(...)
            {
                //nested
                ptr = std::current_exception();
            }
        }

        return res;
    }

    std::string API_DCI_EXCEPTION currentToString()
    {
        return toString(std::current_exception());
    }
}
