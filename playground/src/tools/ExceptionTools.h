#pragma once
#include <exception>
#include <string>
#include <stdexcept>

namespace ExceptionTools {
    inline std::string handle_eptr(std::exception_ptr eptr)
    {
      try {
        if (eptr) {
          std::rethrow_exception(eptr);
        }
      } catch(const std::out_of_range& e) {
        return "Caught std::out_of_range \""s + e.what() + "\"\n";
      } catch(const std::runtime_error& e) {
        return "Caught runtime_error \""s + e.what() + "\"\n";
      } catch(const std::exception& e) {
        return "Caught exception \""s +  e.what()  + "\"\n";
      } catch(...) {
        return "Unknown Exception!";
      }
      return "";
    }
}