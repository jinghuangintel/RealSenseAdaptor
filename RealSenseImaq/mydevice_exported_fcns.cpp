/*
Copyright  (c) 2015 Intel Corporation.

The source code, information and material ("Material") contained 
herein is owned by Intel Corporation or its suppliers or 
licensors, and title to such Material remains with Intel 
Corporation or its suppliers or licensors. The Material contains 
proprietary information of Intel or its suppliers and licensors. 
The Material is protected by worldwide copyright laws and treaty 
provisions. No part of the Material may be used, copied, 
reproduced, modified, published, uploaded, posted, transmitted, 
distributed or disclosed in any way without Intel's prior express 
written permission. No license under any patent, copyright or 
other intellectual property rights in the Material is granted to 
or conferred upon you, either expressly, by implication, 
inducement, estoppel or otherwise. Any license under such 
intellectual property rights must be express and approved by 
Intel in writing.

Unless otherwise agreed by Intel in writing, you may not remove 
or alter this notice or any other notice embedded in Materials by 
Intel or Intel's suppliers or licensors in any way.

*/




#include "mydeviceimaq.h"
void initializeAdaptor()
{
}
void getAvailHW(imaqkit::IHardwareInfo* hardwareInfo)
{
	// Add the first device as F200 color sensor
	imaqkit::IDeviceInfo* deviceInfo =
		hardwareInfo->createDeviceInfo(1, "F200 color sensor");
	setF200ColorFormat(deviceInfo);
	hardwareInfo->addDevice(deviceInfo);

	// create the second device as F200 depth sensor
	deviceInfo =
		hardwareInfo->createDeviceInfo(2, "F200 depth sensor");
	setF200DepthFormat(deviceInfo);
	hardwareInfo->addDevice(deviceInfo);

	// Add the third device as R200 color sensor
	deviceInfo =
		hardwareInfo->createDeviceInfo(3, "R200 color sensor");
	setR200ColorFormat(deviceInfo);
	hardwareInfo->addDevice(deviceInfo);

	// Add the forth device as R200 color sensor
	deviceInfo =
		hardwareInfo->createDeviceInfo(4, "R200 depth sensor");
	setR200DepthFormat(deviceInfo);
	hardwareInfo->addDevice(deviceInfo);
}
void getDeviceAttributes(const imaqkit::IDeviceInfo* deviceInfo,
	const char* formatName,
	imaqkit::IPropFactory* devicePropFact,
	imaqkit::IVideoSourceInfo* sourceContainer,
	imaqkit::ITriggerInfo* hwTriggerInfo){

	void* hProp;
	// Create a video source
	int devID = deviceInfo->getDeviceID();
	if (devID == 1) {
		sourceContainer->addAdaptorSource("input1", 1);
	}
	else if (devID == 2) {
		sourceContainer->addAdaptorSource("input1", 1);
		hProp = devicePropFact->createDoubleProperty("F200FocalLengthX", 475.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleProperty("F200FocalLengthY", 475.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleProperty("F200PrincipalPointX", 327.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleProperty("F200PrincipalPointY", 246.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		// Intel Point Cloud functionality properties
		// Add InputDepthMap for point cloud calculations.
		hProp = devicePropFact->createIntArrayProperty("CameraID", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createIntArrayProperty("InputDepthMap", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createIntArrayProperty("InputDepthHeight", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createIntArrayProperty("InputDepthWidth", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		// Add depth to point cloud property.
		hProp = devicePropFact->createIntArrayProperty("InputColorImage", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		hProp = devicePropFact->createIntArrayProperty("InputColorHeight", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createIntArrayProperty("InputColorWidth", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		hProp = devicePropFact->createIntArrayProperty("OutputAlignmentD2C", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		hProp = devicePropFact->createIntArrayProperty("OutputAlignmentC2D", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleArrayProperty("OutputPointCloudD2P", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::ALWAYS);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);


	}
	else if (devID == 3) {
		sourceContainer->addAdaptorSource("input1", 1);
		hProp = devicePropFact->createDoubleProperty("R200BaseLine", 70.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.

		devicePropFact->addProperty(hProp);
	}
	else if (devID == 4){
		sourceContainer->addAdaptorSource("input1", 1);
		hProp = devicePropFact->createDoubleProperty("R200FocalLengthX", 475.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleProperty("R200FocalLengthY", 475.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleProperty("R200PrincipalPointX", 327.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleProperty("R200PrincipalPointY", 246.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createDoubleProperty("R200BaseLine", 70.0);
		// Set the property to read-only while running.
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::WHILE_RUNNING);
		// Add the property to the device-specific property container.

		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		// Intel Point Cloud functionality properties
		// Add InputDepthMap for point cloud calculations.
		hProp = devicePropFact->createIntArrayProperty("InputDepthMap", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createIntArrayProperty("InputDepthHeight", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createIntArrayProperty("InputDepthWidth", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		
		hProp = devicePropFact->createIntArrayProperty("InputColorImage", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		hProp = devicePropFact->createIntArrayProperty("InputColorHeight", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		hProp = devicePropFact->createIntArrayProperty("InputColorWidth", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		hProp = devicePropFact->createIntArrayProperty("OutputAlignmentD2C", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		hProp = devicePropFact->createIntArrayProperty("OutputAlignmentC2D", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
		// Add depth to point cloud property.
		hProp = devicePropFact->createDoubleArrayProperty("OutputPointCloudD2P", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::ALWAYS);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);

		hProp = devicePropFact->createIntArrayProperty("CameraID", 0, NULL);
		devicePropFact->setPropReadOnly(hProp, imaqkit::propreadonly::NEVER);
		devicePropFact->addProperty(hProp);
		devicePropFact->setVisibility(hProp, imaqkit::visibility::INVISIBLE);
	}

}
imaqkit::IAdaptor* createInstance(imaqkit::IEngine* engine, const
	imaqkit::IDeviceInfo* deviceInfo, const
	char* formatName){
	imaqkit::IAdaptor* adaptor = new
		RealSenseAdaptor(engine, deviceInfo, formatName);;
	return adaptor;
}
void uninitializeAdaptor(){
}