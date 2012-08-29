#
# This template file is defined for standalone scripts/modules
# that do not live in the askap namespace.  It is expected that
# these packages would only exist in Code/Systems/<system>
#
# These are simple scripts and modules NOT python packages.
# For the distinction, the "Distributing Python Modules" manual
# under Section 1.3 "General Python terminology" (pp.3 in Release 2.5.3).
# This is part of the normal Python documentation.

from askapdev.rbuild.setup import setup
from askapdev.rbuild.dependencies import Dependency

dep = Dependency()

setup(name             = 'dummy',
      dependency       = dep,
     )
