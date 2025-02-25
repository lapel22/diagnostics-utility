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

from pathlib import Path
from modules.check import CheckSummary, CheckMetadataPy

import os
import re
import json
from typing import List, Dict
from modules.files_helper import get_json_content_from_file


def get_oneapi_env_versions(json_node: Dict):
    value = {"Value": {}, "RetVal": "INFO"}
    try:
        oneapi_product_names_map = get_json_content_from_file(Path(__file__).parent.resolve() / "data" / "oneapi_names_map.json")  # noqa: E501
        if not oneapi_product_names_map:
            raise Exception("oneAPI product names map is empty.")
        LD_LIBRARY_PATH_ENV = os.getenv('LD_LIBRARY_PATH')
        LIBRARY_PATH_ENV = os.getenv('LIBRARY_PATH')
        PATH_ENV = os.getenv('PATH')
        env_paths = []
        env_paths += LD_LIBRARY_PATH_ENV.split(sep=":") if LD_LIBRARY_PATH_ENV else []
        env_paths += LIBRARY_PATH_ENV.split(sep=":") if LIBRARY_PATH_ENV else []
        env_paths += PATH_ENV.split(sep=":") if PATH_ENV else []
        for long_name, short_name in oneapi_product_names_map.items():
            product_versions_pattern = re.compile(fr".+/{short_name}/([0-9.]+)/.+")
            product_versions = set()
            for path in env_paths:
                product_version_match = re.search(product_versions_pattern, path)
                if product_version_match:
                    product_versions.add(product_version_match.group(1))
            product_value = {"Value": {"Version": {"Value": "", "RetVal": "INFO"}}, "RetVal": "INFO"}
            if len(product_versions) == 0:
                continue
            elif len(product_versions) == 1:
                product_value["Value"]["Version"]["Value"] = list(product_versions)[0]
            else:
                product_value["Value"]["Version"]["Value"] = ",".join(sorted(product_versions))
                product_value["Value"]["Version"]["RetVal"] = "WARNING"
                product_value["Value"]["Version"]["Message"] = f"Several versions of {long_name} was found in the current environment."  # noqa: E501
            value["Value"].update({long_name: product_value})
        if not value["Value"]:
            value["RetVal"] = "WARNING"
            value["Message"] = "There are no oneAPI products found in the current environment."
    except Exception as error:
        value["RetVal"] = "ERROR"
        value["Message"] = str(error)
        value["HowToFix"] = "This error is unexpected. Please report the issue to " \
                            "Diagnostics Utility for Intel® oneAPI Toolkits repository: " \
                            "https://github.com/intel/diagnostics-utility."

    json_node.update({"oneAPI products installed in the environment": value})


def run_oneapi_env_check(data: dict) -> CheckSummary:
    result_json = {"Value": {}, "RetVal": "INFO"}

    get_oneapi_env_versions(result_json["Value"])

    check_summary = CheckSummary(
        result=json.dumps(result_json, indent=4)
    )

    return check_summary


def get_api_version() -> str:
    return "0.1"


def get_check_list() -> List[CheckMetadataPy]:
    someCheck = CheckMetadataPy(
        name="oneapi_env_check",
        type="Data",
        tags="default,sysinfo,compile,runtime,host,target",
        descr="This check shows the version information of the oneAPI products installed in the environment.",
        dataReq="{}",
        merit=20,
        timeout=5,
        version=1,
        run="run_oneapi_env_check"
    )
    return [someCheck]
