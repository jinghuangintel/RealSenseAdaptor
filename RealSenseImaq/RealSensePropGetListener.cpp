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
#include "MyDevicePara.h"
#include "RealSensePropGetListener.h"
#include "pxcsensemanager.h"
#include <string.h>
#include <assert.h>
#include "RealSenseAdaptor.h"
//#include "mwadaptorimaq.h"
//#include "RealSenseDeviceFormat.h"
void RealsensePropGetListener::getValue(imaqkit::IPropInfo* propertyInfo, void* value)
{
	//RealSenseAdaptor* adaptor = reinterpret_cast<RealSenseAdaptor*>(param);
	const char* propname = propertyInfo->getPropertyName();
	PXCSenseManager *psm = 0;
	psm = PXCSenseManager::CreateInstance();
	if (strcmp(propname, "F200FocalLengthX") == 0 || strcmp(propname, "F200FocalLengthY") == 0 || strcmp(propname, "F200PrincipalPointX") == 0 || strcmp(propname, "F200PrincipalPointY") == 0)
	{ 
		psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, RealSense::RS_FCOLOR_DEFAULT_WIDTH, RealSense::RS_FCOLOR_DEFAULT_HEIGHT);
	}
	else if (strcmp(propname, "R200FocalLengthX") == 0 || strcmp(propname, "R200FocalLengthY") == 0 || strcmp(propname, "R200PrincipalPointX") == 0 || strcmp(propname, "R200PrincipalPointY") == 0 || strcmp(propname, "R200BaseLine") == 0)
	{
		psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, RealSense::RS_RCOLOR_DEFAULT_WIDTH, RealSense::RS_RCOLOR_DEFAULT_HEIGHT);
		psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, RealSense::RS_RDEPTH_DEFAULT_WIDTH, RealSense::RS_RDEPTH_DEFAULT_HEIGHT);
	}
	psm->Init();
	PXCCapture::Sample *sample = psm->QuerySample();
	PXCCapture::Device *device = psm->QueryCaptureManager()->QueryDevice();
	PXCProjection *projection = device->CreateProjection();
	PXCCalibration *calib = projection->QueryInstance<PXCCalibration>();
	PXCCalibration::StreamCalibration calibData = {};
	PXCCalibration::StreamTransform calibTrans = {};
	pxcStatus sts = calib->QueryStreamProjectionParameters(PXCCapture::StreamType::STREAM_TYPE_DEPTH, &calibData, &calibTrans);
	if (strcmp(propname, "F200FocalLengthX") == 0 || strcmp(propname, "R200FocalLengthX") == 0)
	{	
		*(reinterpret_cast<double*>(value)) = (double)calibData.focalLength.x;
	}
	
	else if (strcmp(propname, "F200FocalLengthY") == 0 || strcmp(propname, "R200FocalLengthY") == 0)
	{
		*(reinterpret_cast<double*>(value)) = (double)calibData.focalLength.y;
	}
	
	else if (strcmp(propname, "F200PrincipalPointX") == 0 || strcmp(propname, "R200PrincipalPointX") == 0)
	{
		*(reinterpret_cast<double*>(value)) = (double)calibData.principalPoint.x;
	}
	else if (strcmp(propname, "F200PrincipalPointY") == 0 || strcmp(propname, "R200PrincipalPointY") == 0)
	{
		*(reinterpret_cast<double*>(value)) = (double)calibData.principalPoint.y;
	}

	else if (strcmp(propname, "R200BaseLine") == 0)
	{
		*(reinterpret_cast<double*>(value)) = RealSense::R200_BASELINE_IN_MM;;
	}
	else
	{
		assert(false && "Unhandled property . Need to add this new property.");
	}
	psm->Release();



}