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
#ifndef __RealSensePointCloudGetListener_Header__
#define __RealSensePointCloudGetListener_Header

#include <mwadaptorimaq.h>
#include <IPropCustomGetFcn.h>
#include "RealSenseAdaptor.h"
#include "pxcsensemanager.h"
#include <vector>
#include <assert.h>
using namespace std;
class RealSensePointCloudGetListener : public imaqkit::IPropCustomGetFcn
{
public:
	RealSensePointCloudGetListener(RealSenseAdaptor* parent) : _parent(parent) 
	{	
		_depthInput.reset(new UINT16[3]);
		// Initialize the Point cloud array which needs to passed to MATLAB
		_depthPointCloudValues.doubles = new double[9];
		_depthPointCloudValues.ndoubles = 9;
	};

	// Destructor.
	virtual ~RealSensePointCloudGetListener() 
	{
	};

	// Function that gets the value of the property.
	virtual void getValue(imaqkit::IPropInfo* propertyInfo, void* value) 
	{

		// Get the input depth image provided by user using "InputDepthMap" property from MATLAB
		void* val = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputDepthMap");
		imaqkit::propertytypes::NInts* intVal = reinterpret_cast<imaqkit::propertytypes::NInts*>(val);
		void *height = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputDepthHeight");
		
		//int h = *reinterpret_cast<int*>(height);
		imaqkit::propertytypes::NInts* hi = reinterpret_cast<imaqkit::propertytypes::NInts*>(height);
		void *Width = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputDepthWidth");
		//int w = *reinterpret_cast<int*>(Width);;
		imaqkit::propertytypes::NInts* wi = reinterpret_cast<imaqkit::propertytypes::NInts*>(Width);
		
		void *di = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("CameraID");
		//int w = *reinterpret_cast<int*>(Width);;
		imaqkit::propertytypes::NInts* cam_id = reinterpret_cast<imaqkit::propertytypes::NInts*>(di);
		if (intVal->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *intVal;
			return;
		}
		if (hi->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *hi;
			return;
		}
		if (wi->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *wi;
			return;
		}
		if (cam_id->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *cam_id;
			return;
		}
		int h = hi->ints[0];
		int w = wi->ints[0];
		int id = cam_id->ints[0];
		_depthInput.reset(new UINT16[intVal->nints]);

		for (int i = 0; i < intVal->nints; i++)
		{
			_depthInput[i] = intVal->ints[i];
		}
		// Map depth input image to Point Cloud coordinates
		const char* propname = propertyInfo->getPropertyName();
		PXCSession *session = PXCSession_Create();
		PXCSenseManager *psm = session->CreateSenseManager();
		
		if (id == 2)
		{
			psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, RealSense::RS_FDEPTH_DEFAULT_WIDTH, RealSense::RS_FDEPTH_DEFAULT_HEIGHT);
		}
		else if (id == 4)
		{
			psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, RealSense::RS_RCOLOR_DEFAULT_WIDTH, RealSense::RS_RCOLOR_DEFAULT_HEIGHT);
			psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, RealSense::RS_RDEPTH_DEFAULT_WIDTH, RealSense::RS_RDEPTH_DEFAULT_HEIGHT);
		}
		
		// initialize the PXCSenseManager
		if (psm->Init() != PXC_STATUS_NO_ERROR)
		{
			assert(false && "Adaptor do not support this depth map format");
		}
		PXCCapture::Device *device = psm->QueryCaptureManager()->QueryDevice();
		PXCProjection *projection = device->CreateProjection();
	    
		PXCCalibration *calib = projection->QueryInstance<PXCCalibration>();
		PXCCalibration::StreamCalibration calibData = {};
		PXCCalibration::StreamTransform calibTrans = {};
		pxcStatus sts = calib->QueryStreamProjectionParameters(PXCCapture::StreamType::STREAM_TYPE_DEPTH, &calibData, &calibTrans);

		double focal_length_x = (double)calibData.focalLength.x;
		double focal_length_y = (double)calibData.focalLength.y;

		double principle_point_x = (double)calibData.principalPoint.x;
		double principle_point_y = (double)calibData.principalPoint.y;
		double cx = 1.0f / focal_length_x;
		double cy = 1.0f / focal_length_y;

		vector<PXCPoint3DF32> vertices;
		vertices.resize(w*h);

		for (pxcI32 id = 0; id < h; id++)
		{
			for (pxcI32 jd = 0; jd < w; jd++) 
			{	
				unsigned short p = _depthInput[id*w + jd];
				if (p != 0)
				{
					unsigned short zPt = p;
					float xPt = (cx)*(principle_point_x - jd)*zPt;
					float yPt = (cy)*(principle_point_y - id)*zPt;
					vertices[id * w + jd].x = -xPt;
					vertices[id * w + jd].y = yPt;
					vertices[id * w + jd].z = zPt;
				}
				else
				{
					vertices[id * w + jd].x = 0;
					vertices[id * w + jd].y = 0;
					vertices[id * w + jd].z = 0;
				}

			}
	   }
		
		// Initialize the Point cloud array which needs to passed to MATLAB
		_depthPointCloudValues.doubles = new double[w*h * 3];
		_depthPointCloudValues.ndoubles = w*h*3;
		for (size_t pixID_x = 0; pixID_x < h; pixID_x++) 
		{ 
			for (size_t pixID_y = 0; pixID_y < w; pixID_y++)
			{
					_depthPointCloudValues.doubles[pixID_x + h * pixID_y + w * h * 0] = vertices[pixID_x*w + pixID_y].x;
					_depthPointCloudValues.doubles[pixID_x + h * pixID_y + w * h * 1] = vertices[pixID_x*w + pixID_y].y;
					_depthPointCloudValues.doubles[pixID_x + h * pixID_y + w * h * 2] = vertices[pixID_x*w + pixID_y].z;
			}
		}
		psm->Release();
		*(reinterpret_cast<imaqkit::propertytypes::NDoubles*>(value)) = _depthPointCloudValues;
	};

private:

	// TheDemoAdaptor object.
	RealSenseAdaptor* _parent;

	// Depth Point cloud values.
	imaqkit::propertytypes::NDoubles _depthPointCloudValues;

	// Depth Input.
	//unsigned short* _depthInput;
	std::unique_ptr<UINT16[]> _depthInput;
	// Calculates depth to point cloud.
	void getPointCloudD2P(void* value);

	
};

#endif