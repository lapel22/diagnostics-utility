# /*******************************************************************************
# Copyright Intel Corporation.
# This software and the related documents are Intel copyrighted materials, and your use of them
# is governed by the express license under which they were provided to you (License).
# Unless the License provides otherwise, you may not use, modify, copy, publish, distribute, disclose
# or transmit this software or the related documents without Intel's prior written permission.
# This software and the related documents are provided as is, with no express or implied warranties,
# other than those that are expressly stated in the License.
#
# *******************************************************************************/

import platform

from typing import Optional
from pathlib import Path
from modules.printing.printer import print_ex


def print_epilog(output_txt: Optional[Path], output_json: Optional[Path], version: str):
    print_ex("", output_txt)
    if output_txt:
        print_ex(f"Console output file: {output_txt}", output_txt)
    if output_json:
        print_ex(f"JSON output file: {output_json}", output_txt)
    print_ex("", output_txt)
    print_ex(f"This report for {platform.node()}", output_txt)
    print_ex(f"was generated by the Diagnostics Utility for Intel® oneAPI Toolkits {version}.", output_txt)
