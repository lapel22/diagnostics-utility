/*******************************************************************************
Copyright Intel Corporation.
This software and the related documents are Intel copyrighted materials, and your use of them
is governed by the express license under which they were provided to you (License).
Unless the License provides otherwise, you may not use, modify, copy, publish, distribute, disclose
or transmit this software or the related documents without Intel's prior written permission.
This software and the related documents are provided as is, with no express or implied warranties,
other than those that are expressly stated in the License.

*******************************************************************************/

#include "LZDriverChecker.h"

LZ_DriverChecker::LZ_DriverChecker() {
	// TODO Auto-generated constructor stub
}

LZ_DriverChecker::~LZ_DriverChecker() {
	// TODO Auto-generated destructor stub
}

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeInit)(
	ze_init_flags_t flags ///< [in] Initialization flags.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDriverGet)(
	uint32_t *pCount,			  ///< [in,out] Pointer to the number of driver instances.
	ze_driver_handle_t *phDrivers ///< [in,out][optional][range(0, *pCount)] Array of driver instance handles.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDriverGetApiVersion)(
	ze_driver_handle_t hDriver, ///< [in] Handle of the driver instance.
	ze_api_version_t *version	///< [out] API version.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDriverGetProperties)(
	ze_driver_handle_t hDriver,				  ///< [in] Handle of the driver instance.
	ze_driver_properties_t *pDriverProperties ///< [in,out] Query result for driver properties.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDriverGetIpcProperties)(
	ze_driver_handle_t hDriver,				   ///< [in] Handle of the driver instance.
	ze_driver_ipc_properties_t *pIpcProperties ///< [in,out] Query result for IPC properties.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDriverGetExtensionProperties)(
	ze_driver_handle_t hDriver,							   ///< [in] Handle of the driver instance.
	uint32_t *pCount,									   ///< [in,out] Pointer to the number of extension properties.
	ze_driver_extension_properties_t *pExtensionProperties ///< [in,out][optional][range(0, *pCount)] Array of query results for
														   ///< extension properties.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDeviceGet)(
	ze_driver_handle_t hDriver,	  ///< [in] Handle of the driver instance.
	uint32_t *pCount,			  ///< [in,out] Pointer to the number of devices.
	ze_device_handle_t *phDevices ///< [in,out][optional][range(0, *pCount)] Array of handle of devices.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDeviceGetProperties)(
	ze_device_handle_t hDevice,				  ///< [in] Handle of the device.
	ze_device_properties_t *pDeviceProperties ///< [in,out] Query result for device properties.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zeDeviceGetComputeProperties)(
	ze_device_handle_t hDevice,						   ///< [in] Handle of the device.
	ze_device_compute_properties_t *pComputeProperties ///< [in,out] Query result for compute properties.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zesDeviceEnumFrequencyDomains)(
	zes_device_handle_t hDevice,   ///< [in] Sysman handle of the device.
	uint32_t *pCount,			   ///< [in,out] Pointer to the number of components of this type.
								   ///< If count is zero, the driver shall update the value with the
								   ///< total number of available components of this type.
								   ///< If count is greater than the number of available components of this type,
								   ///< the driver shall update the value with the correct
								   ///< number of components.
	zes_freq_handle_t *phFrequency ///< [in,out][optional][range(0, *pCount)] Array of handle of components of
								   ///< this type.
								   ///< If count is less than the number of available components of this type,
								   ///< the driver shall only retrieve that number of
								   ///< component handles.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zesFrequencyGetProperties)(
	zes_freq_handle_t hFrequency,		///< [in] Handle for the component.
	zes_freq_properties_t* pProperties	///< [in,out] The frequency properties for the specified domain.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zesDeviceEnumMemoryModules)(
	zes_device_handle_t hDevice, ///< [in] Sysman handle of the device.
	uint32_t *pCount,			 ///< [in,out] Pointer to the number of components of this type.
								 ///< If count is zero, then the driver shall update the value with the
								 ///< total number of available components of this type.
								 ///< If count is greater than the number of available components of this type,
								 ///< the driver shall update the value with the correct
								 ///< number of components.
	zes_mem_handle_t *phMemory	 ///< [in,out][optional][range(0, *pCount)] Array of handle of components of
								 ///< this type.
								 ///< If count is less than the number of available components of this type,
								 ///< then the driver shall only retrieve that number of
								 ///< component handles.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zesMemoryGetBandwidth)(
	zes_mem_handle_t hMemory,		///< [in] Handle for the component.
	zes_mem_bandwidth_t *pBandwidth ///< [in,out] Will contain the current health, free memory, and total memory
									///< size.
	) = nullptr;

ZE_APIEXPORT ze_result_t ZE_APICALL
(*fp_zesDevicePciGetProperties)(
	zes_device_handle_t hDevice,		///< [in] Sysman handle of the device.
	zes_pci_properties_t *pProperties	///< [in,out] Will contain the PCI properties.
	) = nullptr;

bool LZ_DriverChecker::Load(string &message)
{
	void *handle;
	char *error;
	stringstream ss;

	dlerror(); /* Clear any existing error */
	handle = dlopen("libze_loader.so", RTLD_LAZY);
	if (!handle) {
		ss << "Intel® oneAPI Level Zero library loading error: " << dlerror();
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeInit) = dlsym(handle, "zeInit");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDriverGet) = dlsym(handle, "zeDriverGet");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDriverGetApiVersion) = dlsym(handle, "zeDriverGetApiVersion");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDriverGetProperties) = dlsym(handle, "zeDriverGetProperties");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDriverGetIpcProperties) = dlsym(handle, "zeDriverGetIpcProperties");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDriverGetExtensionProperties) = dlsym(handle, "zeDriverGetExtensionProperties");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDeviceGet) = dlsym(handle, "zeDeviceGet");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDeviceGetProperties) = dlsym(handle, "zeDeviceGetProperties");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zeDeviceGetComputeProperties) = dlsym(handle, "zeDeviceGetComputeProperties");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}

	*(void **)(&fp_zesDeviceEnumFrequencyDomains) = dlsym(handle, "zesDeviceEnumFrequencyDomains");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}
	*(void **)(&fp_zesFrequencyGetProperties) = dlsym(handle, "zesFrequencyGetProperties");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}
	*(void **)(&fp_zesDeviceEnumMemoryModules) = dlsym(handle, "zesDeviceEnumMemoryModules");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}
	*(void **)(&fp_zesMemoryGetBandwidth) = dlsym(handle, "zesMemoryGetBandwidth");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}
	*(void **)(&fp_zesDevicePciGetProperties) = dlsym(handle, "zesDevicePciGetProperties");
	error = dlerror();
	if (error != NULL) {
		ss << "Intel® oneAPI Level Zero library loading error: " << error;
		message = ss.str();
		return false;
	}
	return true;
}

bool LZ_DriverChecker::Initialize(string &message)
{
	setenv("ZES_ENABLE_SYSMAN", "1", 0);
	ze_result_t result = fp_zeInit(0);
	if (result == ZE_RESULT_SUCCESS) {
		message = "";
	}
	else {
		message = GetErrorMessage(result);
		return false;
	}

	return true;
}

bool LZ_DriverChecker::GetLoaderVersion(string &message)
{
	string out;
	// Extract information about installed packages
	int retval = CheckerHelper::RunCommand("dpkg --no-pager -l 'level-zero' 2>/dev/null | grep ii", out);
	if (retval != 0) {
		message = "Level-zero package is not installed or not accessible.";
		return false;
	}

	// Parse output and get version
	vector<string> fields = CheckerHelper::SplitString(out, "(\\s+)");
	string version = fields[2];
	fields = CheckerHelper::SplitString(version, "(\\+)");

	message = fields[0];
	return true;
}

void LZ_DriverChecker::GetDriverInfo(string &message)
{
	ze_result_t result;
	uint32_t driverCount = 0;

	json_object *node = json_object_new_object();

	if (GetLoaderVersion(message)) {
		JsonNode::AddJsonNode(node, "Loader Version", INFO, "", "dpkg --no-pager -l 'level-zero' | grep ii", 0, message);
	}

	// Get number of the drivers
	result = fp_zeDriverGet(&driverCount, nullptr);
	if (result == ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(node, "Installed driver number", INFO, "", "zeDriverGet(&driverCount, NULL)", 0, driverCount);
	}
	else {
		message = GetErrorMessage(result);
		JsonNode::AddJsonNode(node, "Installed driver number", ERROR, message, "zeDriverGet(&driverCount, NULL)", 0, driverCount);
		return;
	}

	char *user_driver_index = getenv("LZT_DEFAULT_DRIVER_IDX");
	if (user_driver_index != nullptr) {
		try {
			int default_idx = stoi(user_driver_index);
			if (default_idx >= 0)
				if (uint32_t(default_idx) < driverCount) {
					JsonNode::AddJsonNode(node, "Default driver index", INFO, "", "getenv(\"LZT_DEFAULT_DRIVER_IDX\")", 1, default_idx);
				}
				else {
					stringstream ss;
					ss << default_idx << " is invalid on this machine.";
					JsonNode::AddJsonNode(node, "Default driver index", ERROR, ss.str(), "getenv(\"LZT_DEFAULT_DRIVER_IDX\")", 0, default_idx);
				}
			else {
				stringstream ss;
				ss << default_idx << " is invalid on this machine.";
				JsonNode::AddJsonNode(node, "Default driver index", ERROR, ss.str(), "getenv(\"LZT_DEFAULT_DRIVER_IDX\")", 0, default_idx);
			}
		}
		catch (const std::invalid_argument &ia) {
			JsonNode::AddJsonNode(node, "Default driver index", ERROR, "Invalid value of LZT_DEFAULT_DRIVER_IDX environment variable.", "getenv(\"LZT_DEFAULT_DRIVER_IDX\")", 0, user_driver_index);
		}
	}
	else {
		JsonNode::AddJsonNode(node, "Default driver index", INFO, "", "getenv(\"LZT_DEFAULT_DRIVER_IDX\")", 1, "not set");
	}

	// Iterate through drivers and get their properties
	vector<ze_driver_handle_t> drivers(driverCount);
	result = fp_zeDriverGet(&driverCount, drivers.data());
	if (result == ZE_RESULT_SUCCESS) {
		for (uint32_t iter = 0; iter < driverCount; iter++) {
			_ze_api_version_t api_version;
			json_object *driver = json_object_new_object();
			stringstream ss;
			ss << string("Driver # ") << iter;
			JsonNode::AddJsonNode(node, ss.str(), INFO, "", "zeDriverGet(&driverCount, drivers)", 0, driver);
			if (fp_zeDriverGetApiVersion(drivers[iter], &api_version) == ZE_RESULT_SUCCESS)
				JsonNode::AddJsonNode(driver, "API version", INFO, "", "zeDriverGetApiVersion(driver, &version)", 0, GetAPIVersionString(api_version));
			else
				JsonNode::AddJsonNode(driver, "API version", ERROR, "", "zeDriverGetApiVersion(driver, &version)", 0, "unknown");

			ze_driver_properties_t driverProperties;
			if (fp_zeDriverGetProperties(drivers[iter], &driverProperties) == ZE_RESULT_SUCCESS) {
				JsonNode::AddJsonNode(driver, "Driver UUID", INFO, "", "zeDriverGetProperties(driver, &driverProperties)", 1, GetUUIDString(driverProperties.uuid));
				JsonNode::AddJsonNode(driver, "Driver version", INFO, "", "zeDriverGetProperties(driver, &driverProperties)", 1, GetDriverVersionString(driverProperties.driverVersion));
			}
			else {
				JsonNode::AddJsonNode(driver, "Driver UUID", ERROR, "", "zeDriverGetProperties(driver, &driverProperties)", 0, "unknown");
				JsonNode::AddJsonNode(driver, "Driver version", ERROR, "", "zeDriverGetProperties(driver, &driverProperties)", 0, "unknown");
			}

			ze_driver_ipc_properties_t ipcProperties;
			if (fp_zeDriverGetIpcProperties(drivers[iter], &ipcProperties) == ZE_RESULT_SUCCESS) {
				stringstream ss1;
				ss1 << "0x" << hex << ipcProperties.flags;
				JsonNode::AddJsonNode(driver, "IPC flags", INFO, "", "zeDriverGetIpcProperties(driver, &ipcProperties)", 1, ss1.str());
			}
			else {
				JsonNode::AddJsonNode(driver, "IPC flags", ERROR, "", "zeDriverGetIpcProperties(driver, &ipcProperties)", 0, "unknown");
			}

			uint32_t propCount = 0;
			result = fp_zeDriverGetExtensionProperties(drivers[iter], &propCount, nullptr);
			if (result == ZE_RESULT_SUCCESS) {
				vector<ze_driver_extension_properties_t> properties(propCount);
				if (fp_zeDriverGetExtensionProperties(drivers[iter], &propCount, properties.data()) == ZE_RESULT_SUCCESS) {
					json_object *ext_prop = json_object_new_object();
					JsonNode::AddJsonNode(driver, "Extension properties", INFO, "", "zeDriverGetExtensionProperties(driver, &propCount, properties)", 1, ext_prop);
					JsonNode::AddJsonNode(ext_prop, "Name", INFO, 1, "Version");
					for (uint32_t jter = 0; jter < propCount; jter++) {
						stringstream ss1;
						ss1 << "0x" << hex << properties[jter].version;
						JsonNode::AddJsonNode(ext_prop, properties[jter].name, INFO, 1, ss1.str());
					}
				}
			}
			else {
				JsonNode::AddJsonNode(driver, "Extension properties", ERROR, "", "zeDriverGetExtensionProperties(driver, &propCount, properties)", 0, "unknown");
			}

			GetDeviceInfo(drivers[iter], message);
			JsonNode::AddJsonNode(driver, "Devices", INFO, json_tokener_parse(message.c_str()));
		}
		const char *json_string = json_object_to_json_string(node);
		if (json_string == NULL) {
			message = "Cannot convert json object to string.";
		} else {
			message = string(json_string);
		}
	}
	else {
		message = GetErrorMessage(result);
	}

	return;
}

static void add_clock_rate(json_object *device, ze_device_handle_t device_handler, ze_device_properties_t device_properties)
{
	if (ZE_DEVICE_TYPE_GPU != device_properties.type) {
		JsonNode::AddJsonNode(device, "Core clock rate, MHz", INFO, 1, device_properties.coreClockRate);
		return;
	}

	zes_device_handle_t hSysmanDevice = static_cast<zes_device_handle_t>(device_handler);
	ze_result_t result;
	uint32_t numFreqDomains = 0;
	// Getting frequent domains count
	result = fp_zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, NULL);
	if (result != ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(device, "Device maximum frequency, MHz", ERROR, "Cannot get number of frequency domains", "zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, NULL)", 1, "unknown");
		JsonNode::AddJsonNode(device, "Device current frequency, MHz", ERROR, "Cannot get number of frequency domains", "zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, NULL)", 1, "unknown");
		JsonNode::AddJsonNode(device, "Device minimum frequency, MHz", ERROR, "Cannot get number of frequency domains", "zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, NULL)", 1, "unknown");
		return;
	}
	// Getting frequent handles
	zes_freq_handle_t *pFreqHandles = new zes_freq_handle_t[numFreqDomains];
	result = fp_zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, pFreqHandles);
	if (result != ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(device, "Device maximum frequency, MHz", ERROR, "Cannot get frequency domains", "zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, pFreqHandles)", 1, "unknown");
		JsonNode::AddJsonNode(device, "Device current frequency, MHz", ERROR, "Cannot get frequency domains", "zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, pFreqHandles)", 1, "unknown");
		JsonNode::AddJsonNode(device, "Device minimum frequency, MHz", ERROR, "Cannot get frequency domains", "zesDeviceEnumFrequencyDomains(hSysmanDevice, &numFreqDomains, pFreqHandles)", 1, "unknown");
		delete[] pFreqHandles;
		return;
	}
	zes_freq_properties_t pFreqProperties;
	result = fp_zesFrequencyGetProperties(pFreqHandles[0], &pFreqProperties);
	if (result != ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(device, "Device maximum frequency, MHz", ERROR, "Cannot get frequency properties", "zesFrequencyGetProperties(pFreqHandle, &pFreqProperties)", 1, "unknown");
		JsonNode::AddJsonNode(device, "Device current frequency, MHz", ERROR, "Cannot get frequency properties", "zesFrequencyGetProperties(pFreqHandle, &pFreqProperties)", 1, "unknown");
		JsonNode::AddJsonNode(device, "Device minimum frequency, MHz", ERROR, "Cannot get frequency properties", "zesFrequencyGetProperties(pFreqHandle, &pFreqProperties)", 1, "unknown");
		delete[] pFreqHandles;
		return;
	}
	JsonNode::AddJsonNode(device, "Device maximum frequency, MHz", INFO, "", "zesFrequencyGetProperties(pFreqHandle, &pFreqProperties)", 1, pFreqProperties.max);
	JsonNode::AddJsonNode(device, "Device minimum frequency, MHz", INFO, "", "zesFrequencyGetProperties(pFreqHandle, &pFreqProperties)", 1, pFreqProperties.min);
	JsonNode::AddJsonNode(device, "Device current frequency, MHz", INFO, "", "zesFrequencyGetProperties(pFreqHandle, &pFreqProperties)", 1, device_properties.coreClockRate);

	delete[] pFreqHandles;
}

static void add_memory_bandwidth(json_object *device, ze_device_handle_t device_handler)
{
	zes_device_handle_t hSysmanDevice = static_cast<zes_device_handle_t>(device_handler);
	ze_result_t result;
	uint32_t numMemDomains = 0;
	result = fp_zesDeviceEnumMemoryModules(hSysmanDevice, &numMemDomains, NULL);
	if (result != ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(device, "Memory bandwidth, GB/s", ERROR, "Cannot get number of memory domains", "zesDeviceEnumMemoryModules(hSysmanDevice, &numMemDomains, NULL)", 1, "unknown");
		return;
	}
	if (numMemDomains == 0) {
		JsonNode::AddJsonNode(device, "Memory bandwidth, GB/s", ERROR, "Driver cannot get memory bandwidth", "zesDeviceEnumMemoryModules(hSysmanDevice, &numMemDomains, NULL)", 1, "unknown");
		return;
	}
	zes_mem_handle_t *pMemHandles = new zes_mem_handle_t[numMemDomains];
	result = fp_zesDeviceEnumMemoryModules(hSysmanDevice, &numMemDomains, pMemHandles);
	if (result != ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(device, "Memory bandwidth, GB/s", ERROR, "Driver cannot get memory bandwidth", "zesDeviceEnumMemoryModules(hSysmanDevice, &numMemDomains, pMemHandles)", 1, "unknown");
		delete[] pMemHandles;
		return;
	}
	zes_mem_bandwidth_t pMemBandwidth;
	result = fp_zesMemoryGetBandwidth(pMemHandles[0], &pMemBandwidth);
	if (result != ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(device, "Memory bandwidth, GB/s", ERROR, "Driver cannot get memory bandwidth", "zesMemoryGetBandwidth(pMemHandle, &pMemBandwidth)", 1, "unknown");
		delete[] pMemHandles;
		return;
	}
	uint64_t raw_bandwidth = pMemBandwidth.maxBandwidth;
	long double bandwidth = (long double)(raw_bandwidth) / (long double)(1024 * 1024 * 1024); // convert B/s to GB/s
	JsonNode::AddJsonNode(device, "Memory bandwidth, GB/s", INFO, "", "zesMemoryGetBandwidth(pMemHandle, &pMemBandwidth)", 1, std::to_string(bandwidth));

	delete[] pMemHandles;
}

static void add_pci_bandwidth(json_object *device, ze_device_handle_t device_handler)
{
	zes_device_handle_t hSysmanDevice = static_cast<zes_device_handle_t>(device_handler);
	ze_result_t result;
	zes_pci_properties_t pPCIProps;
	result = fp_zesDevicePciGetProperties(hSysmanDevice, &pPCIProps);
	if (result != ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(device, "PCIe bandwidth, GB/s", ERROR, "Cannot get PCIe information", "zesDevicePciGetProperties(hSysmanDevice, &pPCIProps)", 1, "unknown");
		return;
	}
	int64_t raw_bandwidth = pPCIProps.maxSpeed.maxBandwidth;
	if (raw_bandwidth == -1) {
		JsonNode::AddJsonNode(device, "PCIe bandwidth, GB/s", ERROR, "Driver cannot get PCIe bandwidth", "zesDevicePciGetProperties(hSysmanDevice, &pPCIProps)", 1, "unknown");
		return;
	}
	long double bandwidth = (long double)(raw_bandwidth) / (long double)(1024 * 1024 * 1024); // convert B/s to GB/s
	JsonNode::AddJsonNode(device, "PCIe bandwidth, GB/s", INFO, "", "zesDevicePciGetProperties(hSysmanDevice, &pPCIProps)", 1, std::to_string(bandwidth));
}

void LZ_DriverChecker::GetDeviceInfo(ze_driver_handle_t driver, string &message)
{
	ze_result_t result;
	json_object *node = json_object_new_object();

	uint32_t deviceCount = 0;
	result = fp_zeDeviceGet(driver, &deviceCount, nullptr);
	if (result == ZE_RESULT_SUCCESS) {
		JsonNode::AddJsonNode(node, "Device number", INFO, "", "zeDeviceGet(driver, &deviceCount, NULL)", 0, deviceCount);
	}
	else {
		message = GetErrorMessage(result);
		JsonNode::AddJsonNode(node, "Device number", ERROR, message, "zeDeviceGet(driver, &deviceCount, NULL)", 0, "unknown");
		return;
	}

	vector<ze_device_handle_t> devices(deviceCount);
	result = fp_zeDeviceGet(driver, &deviceCount, devices.data());
	if (result != ZE_RESULT_SUCCESS) {
		message = GetErrorMessage(result);
		JsonNode::AddJsonNode(node, "Device number", ERROR, message, "zeDeviceGet(driver, &deviceCount, devices)", 0, "unknown");
		return;
	}
	else {
		ze_device_properties_t deviceProperties;
		deviceProperties.stype = ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES;
		deviceProperties.pNext = nullptr;
		ze_device_compute_properties_t deviceComputeProperties;
		deviceComputeProperties.stype = ZE_STRUCTURE_TYPE_DEVICE_COMPUTE_PROPERTIES;
		deviceComputeProperties.pNext = nullptr;
		for (uint32_t iter = 0; iter < deviceCount; iter++) {
			stringstream ss;
			ss << string("Device # ") << iter;
			json_object *device = json_object_new_object();
			JsonNode::AddJsonNode(node, ss.str(), INFO, "", "zeDeviceGetProperties(devices[iter], &deviceProperties)", 0, device);

			result = fp_zeDeviceGetProperties(devices[iter], &deviceProperties);
			if (result != ZE_RESULT_SUCCESS) {
				message = GetErrorMessage(result);
				continue;
			}
			else {
				JsonNode::AddJsonNode(device, "Device type", INFO, GetDeviceTypeString(deviceProperties.type));
				JsonNode::AddJsonNode(device, "Device name", INFO, deviceProperties.name);
				stringstream ss2;
				ss2 << "0x" << hex << deviceProperties.vendorId; // See http://pci-ids.ucw.cz/
				JsonNode::AddJsonNode(device, "Device vendorID", INFO, 1, ss2.str());
				ss2.str(string());
				ss2 << "0x" << hex << deviceProperties.deviceId;
				JsonNode::AddJsonNode(device, "Device ID", INFO, 1, ss2.str());
				ss2.str(string());
				ss2 << "0x" << hex << deviceProperties.flags;
				JsonNode::AddJsonNode(device, "Device flags", INFO, 1, ss2.str());
				ss2.str(string());
				ss2 << "0x" << hex << deviceProperties.subdeviceId;
				JsonNode::AddJsonNode(device, "Subdevice ID", INFO, 1, ss2.str());
				add_clock_rate(device, devices[iter], deviceProperties);
				add_memory_bandwidth(device, devices[iter]);
				add_pci_bandwidth(device, devices[iter]);
				JsonNode::AddJsonNode(device, "Maximum memory allocation size", INFO, 1, deviceProperties.maxMemAllocSize);
				JsonNode::AddJsonNode(device, "Maximum number of logical hardware context", INFO, 1, deviceProperties.maxHardwareContexts);
				JsonNode::AddJsonNode(device, "Maximum priority for command queues", INFO, 1, deviceProperties.maxCommandQueuePriority);
				JsonNode::AddJsonNode(device, "Number of threads per EU", INFO, 1, deviceProperties.numThreadsPerEU);
				JsonNode::AddJsonNode(device, "The physical EU simd width", INFO, 1, deviceProperties.physicalEUSimdWidth);
				JsonNode::AddJsonNode(device, "Number of EU per sub-slice", INFO, 1, deviceProperties.numEUsPerSubslice);
				JsonNode::AddJsonNode(device, "Number of sub-slices per slice", INFO, 1, deviceProperties.numSubslicesPerSlice);
				JsonNode::AddJsonNode(device, "Number of slices", INFO, 1, deviceProperties.numSlices);
				JsonNode::AddJsonNode(device, "Timer resolution (cycles per second)", INFO, 1, deviceProperties.timerResolution);
				JsonNode::AddJsonNode(device, "Number of valid bits in timestamp value", INFO, 1, deviceProperties.timestampValidBits);
				JsonNode::AddJsonNode(device, "Number of valid bits in the kernel timestamp value", INFO, 1, deviceProperties.kernelTimestampValidBits);
				JsonNode::AddJsonNode(device, "UUID", INFO, 1, GetUUIDString(deviceProperties.uuid));
			}

			json_object *c_props = json_object_new_object();
			result = fp_zeDeviceGetComputeProperties(devices[iter], &deviceComputeProperties);
			if (result != ZE_RESULT_SUCCESS) {
				message = GetErrorMessage(result);
				JsonNode::AddJsonNode(device, "Compute properties", ERROR, message, "zeDeviceGetComputeProperties(device, &deviceComputeProperties)", 0, c_props);
				continue;
			}
			else {
				JsonNode::AddJsonNode(device, "Compute properties", INFO, "", "zeDeviceGetComputeProperties(device, &deviceComputeProperties)", 1, c_props);
				JsonNode::AddJsonNode(c_props, "Maximum items per compute group", INFO, 1, deviceComputeProperties.maxTotalGroupSize);
				JsonNode::AddJsonNode(c_props, "Maximum items for X dimension in group", INFO, 1, deviceComputeProperties.maxGroupSizeX);
				JsonNode::AddJsonNode(c_props, "Maximum items for Y dimension in group", INFO, 1, deviceComputeProperties.maxGroupSizeY);
				JsonNode::AddJsonNode(c_props, "Maximum items for Z dimension in group", INFO, 1, deviceComputeProperties.maxGroupSizeZ);
				JsonNode::AddJsonNode(c_props, "Maximum groups that can be launched for X dimension", INFO, 1, deviceComputeProperties.maxGroupCountX);
				JsonNode::AddJsonNode(c_props, "Maximum groups that can be launched for Y dimension", INFO, 1, deviceComputeProperties.maxGroupCountY);
				JsonNode::AddJsonNode(c_props, "Maximum groups that can be launched for Z dimension", INFO, 1, deviceComputeProperties.maxGroupCountZ);
				JsonNode::AddJsonNode(c_props, "Maximum shared local memory per group", INFO, 1, deviceComputeProperties.maxSharedLocalMemory);
				JsonNode::AddJsonNode(c_props, "Number of subgroup sizes supported", INFO, 1, deviceComputeProperties.numSubGroupSizes);
			}
		}

		const char *json_string = json_object_to_json_string(node);
		if (json_string == NULL) {
			message = "Cannot convert json object to string.";
		} else {
			message = string(json_string);
		}
	}

	return;
}

string LZ_DriverChecker::GetErrorMessage(ze_result_t error)
{
	switch (error) {
		case ZE_RESULT_ERROR_UNINITIALIZED:
			return "Intel® oneAPI Level Zero driver is not initialized.";
		case ZE_RESULT_ERROR_INVALID_ENUMERATION:
			return "Internal error: Enumerator argument is not valid.";
		case ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY:
			return "Internal error: Insufficient host memory to satisfy call.";
		case ZE_RESULT_ERROR_INVALID_NULL_POINTER:
			return "Internal error: Pointer argument may not be nullptr.";
		case ZE_RESULT_ERROR_INVALID_NULL_HANDLE:
			return "Internal error: Handle argument is not valid.";
		case ZE_RESULT_ERROR_DEVICE_LOST:
			return "Device is lost; must be reset for use.";
		default:
			stringstream ss;
			ss << "Unknown internal error: 0x." << hex << error;
			return ss.str();
	}
}

string LZ_DriverChecker::GetAPIVersionString(_ze_api_version_t version)
{
	stringstream ss;
	ss << dec << ((version & 0xffff0000) >> 16) << "." << (version & 0x0000ffff);
	return ss.str();
}

string LZ_DriverChecker::GetDriverVersionString(uint32_t version)
{
	stringstream ss;
	ss << dec << ((version & 0xFF000000) >> 24) << "." << ((version & 0x00FF0000) >> 16) << "." << (version & 0x0000FFFF);
	return ss.str();
}

static char hexdigit(int i) { return (i > 9) ? 'a' - 10 + i : '0' + i; }
template <typename T>
static void uuid_to_string(T uuid, int bytes, char *s)
{
	int i;

	for (i = bytes - 1; i >= 0; --i) {
		*s++ = hexdigit(uuid.id[i] / 0x10);
		*s++ = hexdigit(uuid.id[i] % 0x10);
		if (i >= 6 && i <= 12 && (i & 1) == 0) {
			*s++ = '-';
		}
	}
	*s = '\0';
}

#define MAX_UUID_STRING_SIZE 49
string LZ_DriverChecker::GetUUIDString(ze_driver_uuid_t uuid)
{
	char uuid_string[MAX_UUID_STRING_SIZE];
	uuid_to_string(uuid, ZE_MAX_DRIVER_UUID_SIZE, uuid_string);
	return string(uuid_string);
}

string LZ_DriverChecker::GetUUIDString(ze_device_uuid_t uuid)
{
	char uuid_string[MAX_UUID_STRING_SIZE];
	uuid_to_string(uuid, ZE_MAX_DEVICE_UUID_SIZE, uuid_string);
	return string(uuid_string);
}

string LZ_DriverChecker::GetDeviceTypeString(ze_device_type_t type)
{
	switch (type) {
		case ZE_DEVICE_TYPE_GPU:
			return "Graphics Processing Unit";
		case ZE_DEVICE_TYPE_CPU:
			return "General Processing Unit";
		case ZE_DEVICE_TYPE_FPGA:
			return "Field Programmable Gate Array";
		case ZE_DEVICE_TYPE_MCA:
			return "Memory Copy Accelerator";

		default:
			return "Unknown";
	}
}

