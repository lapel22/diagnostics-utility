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

import sqlite3
import json
import re
from typing import List, Dict


CHECK_VERSION = 1
CHECK_NAME = "driver_compatibility_check"
PACKAGE_DATABASE = Path(__file__).parent.parent.resolve() / "databases" / "compatibility_map.db"
PACKAGE_DATABASE_METADATA = Path(__file__).parent.parent.resolve() / "databases" / "metadata.json"
DOWNLOADED_DATABASE = Path.home() / "intel" / "diagnostics" / "databases" / "compatibility_map.db"
DOWNLOADED_DATABASE_METADATA = Path.home() / "intel" / "diagnostics" / "databases" / "metadata.json"


def _get_compatibilities_for_product(product_name: str, product_version: str, cursor) -> Dict:
    rows = []
    cursor.execute(
        """
        SELECT cons.Name, consV.Version FROM Compatibility INNER JOIN
        Version AS consV ON Compatibility.ConsumerId = consV.Id INNER JOIN
        Version AS servV ON Compatibility.ServiceId = servV.Id INNER JOIN
        Component AS cons ON consV.ComponentId = cons.Id INNER JOIN
        Component AS serv ON servV.ComponentId = serv.Id
        WHERE (serv.Name = '%s' AND servV.Version = '%s');
        """ % (product_name, product_version)
    )
    rows.extend(cursor.fetchall())
    cursor.execute(
        """
        SELECT serv.Name, servV.Version FROM Compatibility INNER JOIN
        Version AS consV ON Compatibility.ConsumerId = consV.Id INNER JOIN
        Version AS servV ON Compatibility.ServiceId = servV.Id INNER JOIN
        Component AS cons ON consV.ComponentId = cons.Id INNER JOIN
        Component AS serv ON servV.ComponentId = serv.Id
        WHERE (cons.Name = '%s' AND consV.Version = '%s');
        """ % (product_name, product_version)
    )
    rows.extend(cursor.fetchall())
    return {row[0]: row[1] for row in rows}


def _is_regression(driver_name: str, driver_version: str, cursor) -> bool:
    cursor.execute(
        """
        SELECT reg.Version FROM Regression AS reg INNER JOIN Component AS com
        ON reg.ComponentId = com.Id INNER JOIN OS
        ON reg.OSId = OS.Id
        WHERE com.Name = '%s' AND OS.Name = 'Linux' AND reg.Version = '%s';
        """ % (driver_name, driver_version)
    )
    row = cursor.fetchone()
    return True if row else False


def _is_latest_version(driver_name: str, driver_version: str, cursor) -> bool:
    cursor.execute(
        """
        SELECT lv.Version FROM LatestVersion AS lv INNER JOIN Component AS com
        ON lv.ComponentId = com.Id INNER JOIN OS ON
        lv.OSId = OS.Id
        WHERE com.Name = '%s' and OS.Name = 'Linux';
        """ % (driver_name,)
    )
    row = cursor.fetchone()
    return True if row == driver_version else False


def get_gpu_driver_version(data: Dict) -> Dict:
    gpu_drivers = {}
    try:
        level_zero_version = data["gpu_backend_check"]["Value"]["GPU"]["Value"]["Intel® oneAPI Level Zero Driver"]["Value"]["Driver information"]["Value"]["Driver # 0"]["Value"]["Driver version"]["Value"]  # noqa: E501
    except Exception:
        pass
    else:
        gpu_drivers["Intel® oneAPI Level Zero"] = level_zero_version

    try:
        opencl_version = data["gpu_backend_check"]["Value"]["GPU"]["Value"]["OpenCL™ Driver"]["Value"]["Driver information"]["Value"]["Platform # 0"]["Value"]["Devices"]["Value"]["Device # 0"]["Value"]["Driver version"]["Value"]  # noqa: E501
    except Exception:
        pass
    else:
        gpu_drivers["OpenCL™"] = opencl_version
    return gpu_drivers


def get_product_versions_install(data: Dict) -> Dict:
    product_versions = {}
    try:
        products = data["oneapi_app_check"]["Value"]["APP"]["Value"]["oneAPI products"]["Value"]
    except Exception:
        pass
    else:
        if isinstance(products, dict):
            product_versions = {
                product_name.strip(): value["Value"]["Version"]["Value"]
                for product_name, value in products.items()
            }
    return product_versions


def get_product_versions_env(data: Dict) -> Dict:
    product_versions = {}
    try:
        products = data["oneapi_env_check"]["Value"]["oneAPI products installed in the environment"]["Value"]
    except Exception:
        pass
    else:
        if isinstance(products, dict):
            product_versions = {
                product_name.strip(): value["Value"]["Version"]["Value"]
                for product_name, value in products.items()
            }
    return product_versions


def _check_regression(json_node: Dict, driver_name: str, driver_version: str,  cursor):
    value = {"Value": "Undefined", "RetVal": "PASS"}
    try:
        if _is_regression(driver_name, driver_version, cursor):
            value["Value"] = "Yes"
            value["RetVal"] = "FAIL"
            value["Message"] = f"Installed version of {driver_name} is regression."
        else:
            value["Value"] = "No"
    except Exception as error:
        value["RetVal"] = "ERROR"
        value["Message"] = str(error)
        value["HowToFix"] = "This error is unexpected. Please report the issue to " \
                            "Diagnostics Utility for Intel® oneAPI Toolkits repository: " \
                            "https://github.com/intel/diagnostics-utility."

    json_node.update({"Regression": value})


def _check_drivers(json_node: Dict, gpu_driver_versions: Dict, cursor):
    value = {"Value": {}, "RetVal": "INFO"}
    if not gpu_driver_versions:
        value["RetVal"] = "WARNING"
        value["Message"] = "There is no information about GPU driver(s)."
    else:
        try:
            for driver_name, driver_version in gpu_driver_versions.items():
                driver_value = {
                    "Value": {
                        "Version": {
                            "Value": driver_version,
                            "RetVal": "INFO"
                        }
                    },
                    "RetVal": "INFO"
                }
                if not _is_latest_version(driver_name, driver_version, cursor):
                    level_zero_link = "https://dgpu-docs.intel.com/technologies/level-zero.html"
                    opecl_link = "https://www.intel.com/content/www/us/en/developer/tools/opencl-sdk/overview.html"  # noqa: E501
                    link = level_zero_link if driver_name == "Intel® oneAPI Level Zero" else \
                        opecl_link if driver_name == "OpenCL™" else ""
                    driver_value["Value"]["Version"]["RetVal"] = "WARNING"
                    driver_value["Value"]["Version"]["Message"] = \
                        f"{driver_version} is not the latest available version of {driver_name} driver. " \
                        f"To get the latest version, visit {link}."
                _check_regression(driver_value["Value"], driver_name, driver_version, cursor)
                value["Value"].update({driver_name: driver_value})
        except Exception as error:
            value["RetVal"] = "ERROR"
            value["Message"] = str(error)
            value["HowToFix"] = "This error is unexpected. Please report the issue to " \
                                "Diagnostics Utility for Intel® oneAPI Toolkits repository: " \
                                "https://github.com/intel/diagnostics-utility."

    json_node.update({"GPU drivers information": value})


def _check_compatibilities(json_node: Dict, product_versions: Dict, gpu_driver_versions: Dict, install: bool, cursor):  # noqa: E501
    value = {"Value": {}, "RetVal": "PASS"}
    if not product_versions:
        value["RetVal"] = "WARNING"
        value["Message"] = "There are no products detected."
    elif not gpu_driver_versions:
        value["RetVal"] = "WARNING"
        value["Message"] = "There is no information about GPU driver(s)."
    else:
        try:
            for product, product_version in product_versions.items():
                product_value = {"Value": "Undefined", "RetVal": "PASS"}
                product_version_pattern = re.compile(r"([0-9.]+)-.+")
                product_version_match = re.search(product_version_pattern, product_version)
                product_version = product_version_match.group(1) if product_version_match else product_version
                product_compatibilities = _get_compatibilities_for_product(product, product_version, cursor)
                if not product_compatibilities:
                    product_value["RetVal"] = "WARNING"
                    product_value["Message"] = \
                        f"There is no information about {product} compatibilities. " \
                        "To get the latest version of oneAPI products, visit https://www.intel.com/content/www/us/en/develop/documentation/installation-guide-for-intel-oneapi-toolkits-linux/top/installation.html."  # noqa: E501
                for comp, comp_version in product_compatibilities.items():
                    if comp in gpu_driver_versions.keys() and gpu_driver_versions[comp] != comp_version:
                        product_value["Value"] = "No"
                        product_value["RetVal"] = "FAIL"
                        product_value["Message"] = \
                            f"Installed version of {comp} not compatible with the version of the {product}."
                        break
                    elif comp in gpu_driver_versions.keys() and gpu_driver_versions[comp] == comp_version:
                        product_value["Value"] = "Yes"
                value["Value"].update({f"{product}-{product_version}": product_value})
        except Exception as error:
            value["RetVal"] = "ERROR"
            value["Message"] = str(error)
            value["HowToFix"] = "This error is unexpected. Please report the issue to " \
                                "Diagnostics Utility for Intel® oneAPI Toolkits repository: " \
                                "https://github.com/intel/diagnostics-utility."

    node_name = "Compatibility of the installed products" if install else \
                "Compatibility of the products in the environment"
    json_node.update({node_name: value})


def check_compatibilities(json_node: Dict, data: Dict) -> None:
    value = {"Value": {}, "RetVal": "PASS"}
    try:
        database_file = DOWNLOADED_DATABASE if DOWNLOADED_DATABASE.exists() else PACKAGE_DATABASE
        database_metadata_file = DOWNLOADED_DATABASE_METADATA if DOWNLOADED_DATABASE.exists() else \
            PACKAGE_DATABASE_METADATA
        with open(database_metadata_file) as json_file:
            database_metadata = json.load(json_file)
            if CHECK_VERSION not in database_metadata["databases"]["compatibility"][0]["compatibility"][CHECK_NAME]:  # noqa: E501
                raise ValueError(f"{database_file} doesn't compatible with {CHECK_NAME} of {CHECK_VERSION} version.")  # noqa: E501
        connection = sqlite3.connect(str(database_file))
        cursor = connection.cursor()
        gpu_driver_versions = get_gpu_driver_version(data)
        product_versions_install = get_product_versions_install(data)
        product_versions_env = get_product_versions_env(data)
        _check_drivers(value["Value"], gpu_driver_versions, cursor)
        _check_compatibilities(value["Value"], product_versions_install, gpu_driver_versions, True, cursor)
        _check_compatibilities(value["Value"], product_versions_env, gpu_driver_versions, False, cursor)
        connection.commit()
        connection.close()
    except Exception as error:
        value["RetVal"] = "ERROR"
        value["Message"] = str(error)
        value["HowToFix"] = "This error is unexpected. Please report the issue to " \
                            "Diagnostics Utility for Intel® oneAPI Toolkits repository: " \
                            "https://github.com/intel/diagnostics-utility."

    json_node.update({"oneAPI products compatibilities with drivers": value})


def run_driver_compatibility_check(data: Dict) -> CheckSummary:
    result_json = {"Value": {}, "RetVal": "PASS"}

    check_compatibilities(result_json["Value"], data)

    check_summary = CheckSummary(
        result=json.dumps(result_json, indent=4)
    )

    return check_summary


def get_api_version() -> str:
    return "0.1"


def get_check_list() -> List[CheckMetadataPy]:
    someCheck = CheckMetadataPy(
        name=CHECK_NAME,
        type="Data",
        tags="default,sysinfo,compile,runtime,host,target",
        descr="This check verifies compatibility of oneAPI products versions and GPU drivers versions.",
        dataReq="{\"gpu_backend_check\": 1, \"oneapi_env_check\": 1 , \"oneapi_app_check\": 1}",
        merit=60,
        timeout=5,
        version=CHECK_VERSION,
        run="run_driver_compatibility_check"
    )
    return [someCheck]
