#ifndef RX_VIRTUALSHAREDFROMTHIS_H
#define RX_VIRTUALSHAREDFROMTHIS_H

/* A hack allowing multi-inheritance with std::shared_from_this<T> */
/* Credit goes to https://stackoverflow.com/questions/15549722/double-inheritance-of-enable-shared-from-this */
/* Good on ya! */

#include <memory>

namespace std {

struct virtual_enable_shared_from_this_base:
   std::enable_shared_from_this<virtual_enable_shared_from_this_base> {
   virtual ~virtual_enable_shared_from_this_base() {}
};
template<typename T>
struct virtual_enable_shared_from_this:
virtual virtual_enable_shared_from_this_base {
   std::shared_ptr<T> shared_from_this() {
      return std::dynamic_pointer_cast<T>(
         virtual_enable_shared_from_this_base::shared_from_this());
   }
};

} // END namespace std

#endif // RX_VIRTUALSHAREDFROMTHIS_H