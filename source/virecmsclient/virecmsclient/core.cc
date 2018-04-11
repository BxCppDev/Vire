// Ourselves
#include "virecmsclient/core.h"

// Third Party:
// - Bayeux:
#include <bayeux/datatools/kernel.h>

namespace virecmsclient {

  uint32_t app_kernel_init_flags()
  {
    uint32_t kernel_init_flags = 0;
    kernel_init_flags |= datatools::kernel::init_no_help;
    kernel_init_flags |= datatools::kernel::init_no_splash;
    kernel_init_flags |= datatools::kernel::init_no_inhibit_libinfo;
    kernel_init_flags |= datatools::kernel::init_no_libinfo_logging;
    kernel_init_flags |= datatools::kernel::init_no_variant;
    kernel_init_flags |= datatools::kernel::init_no_inhibit_variant;
    kernel_init_flags |= datatools::kernel::init_no_locale_category;
    kernel_init_flags |= datatools::kernel::init_no_inhibit_qt_gui;
    return kernel_init_flags;
  }

}  // namespace virecmsclient
