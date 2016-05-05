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
#ifndef __RealSenseD2CAlignmentGetListener_Header__
#define __RealSenseD2CAlignmentGetListener_Header

#include <mwadaptorimaq.h>
#include <IPropCustomGetFcn.h>
#include "RealSenseAdaptor.h"
#include "pxcsensemanager.h"
#include <vector>
#include <assert.h>
using namespace std;
class RealSenseD2CAlignmentGetListener : public imaqkit::IPropCustomGetFcn
{
public:
	RealSenseD2CAlignmentGetListener(RealSenseAdaptor* parent) : _parent(parent) 
	{

		_depthInput.reset(new UINT16[3]);
		_colorInput.reset(new UINT8[3]);
		// Initialize the Point cloud array which needs to passed to MATLAB
		_alignedDepthValues.ints = new int[9];
		_alignedDepthValues.nints = 9;
	};
	// Destructor.
	virtual ~RealSenseD2CAlignmentGetListener() 
	{
	};

	// Function that gets the value of the property.
	virtual void getValue(imaqkit::IPropInfo* propertyInfo, void* value) 
	{

		// Get the input depth image provided by user using "InputDepthMap" property from MATLAB
		void* val = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputDepthMap");
		imaqkit::propertytypes::NInts* intVald = reinterpret_cast<imaqkit::propertytypes::NInts*>(val);
		void *height = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputDepthHeight");
		imaqkit::propertytypes::NInts* hd = reinterpret_cast<imaqkit::propertytypes::NInts*>(height);
		void *Width = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputDepthWidth");
		imaqkit::propertytypes::NInts* wd = reinterpret_cast<imaqkit::propertytypes::NInts*>(Width);

		//get the input color image provided by user using "InputColorImage"
		val = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputColorImage");
		imaqkit::propertytypes::NInts* intValc = reinterpret_cast<imaqkit::propertytypes::NInts*>(val);
		height = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputColorHeight");
		imaqkit::propertytypes::NInts* hc = reinterpret_cast<imaqkit::propertytypes::NInts*>(height);
		Width = _parent->getEngine()->getAdaptorPropContainer()->getPropValue("InputColorWidth");
		imaqkit::propertytypes::NInts* wc = reinterpret_cast<imaqkit::propertytypes::NInts*>(Width);

		if (intVald->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *intVald;
			return;
		}
		if (hd->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *hd;
			return;
		}
		if (wd->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *wd;
			return;
		}
		if (intValc->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *intValc;
			return;
		}
		if (hc->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *hc;
			return;
		}
		if (wc->nints == 0) // When the source of the videoinput obj is queried, but there isn't an InputDepthMap yet.
		{
			*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = *wc;
			return;
		}

		int dh = hd->ints[0];
		int dw = wd->ints[0];
		_depthInput.reset(new UINT16[intVald->nints]);
		for (int i = 0; i < intVald->nints; i++)
		{
			_depthInput[i] = intVald->ints[i];
		}

		int ch = hc->ints[0];
		int cw = wc->ints[0];
		_colorInput.reset(new UINT8[intValc->nints]);
		for (int i = 0; i < intValc->nints; i++)
		{
			_colorInput[i] = intValc->ints[i];
		}
		// Map depth input image to realsense depth format
		PXCSession *session = PXCSession_Create();
		PXCSenseManager *psm = session->CreateSenseManager();
		psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, dw, dh);
		psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, cw, ch);
		// initialize the PXCSenseManager
		if (psm->Init() != PXC_STATUS_NO_ERROR)
		{
			assert(false && "Adaptor do not support this depth map format");
		}
		PXCCapture::Device *device = psm->QueryCaptureManager()->QueryDevice();
		PXCProjection *projection = device->CreateProjection();

		PXCImage *drawDepth;
		PXCImage::ImageInfo drawDepthInfo;
		memset(&drawDepthInfo, 0, sizeof(drawDepthInfo));
		drawDepthInfo.width = dw;
		drawDepthInfo.height = dh;
		drawDepthInfo.format = PXCImage::PIXEL_FORMAT_DEPTH;
		drawDepth = 0;
		drawDepth = session->CreateImage(&drawDepthInfo);

		PXCImage::ImageData drawDepthDat;
		if (PXC_STATUS_NO_ERROR > drawDepth->AcquireAccess(PXCImage::ACCESS_WRITE, drawDepthInfo.format, &drawDepthDat))
			return;
		for (pxcI32 y = 0; y < drawDepthInfo.height; y++)
		{
			for (pxcI32 x = 0; x < drawDepthInfo.width; x++) 
			{

				*(pxcU16*)(drawDepthDat.planes[0] + x * 2 + 2 * y *drawDepthInfo.width) = _depthInput[y*drawDepthInfo.width + x];

			}
		}
		drawDepth->ReleaseAccess(&drawDepthDat);

		PXCImage *drawColor;
		PXCImage::ImageInfo drawColorInfo;
		memset(&drawColorInfo, 0, sizeof(drawColorInfo));
		drawColorInfo.width = cw;
		drawColorInfo.height = ch;
		drawColorInfo.format = PXCImage::PIXEL_FORMAT_RGB32;
		drawColor = 0;
		drawColor = session->CreateImage(&drawColorInfo);

		PXCImage::ImageData drawColorDat;
		if (PXC_STATUS_NO_ERROR > drawColor->AcquireAccess(PXCImage::ACCESS_WRITE, drawColorInfo.format, &drawColorDat))
			return;
		for (pxcI32 y = 0; y < drawColorInfo.height; y++)
		{
			for (pxcI32 x = 0; x < drawColorInfo.width; x++) 
			{

				pxcBYTE *ptr = drawColorDat.planes[0] + x * 4 + 4 * y *drawColorInfo.width;
				ptr[0] = pxcBYTE(_colorInput[cw*ch * 2 + x*drawColorInfo.height + y]);
				ptr[1] = pxcBYTE(_colorInput[cw*ch * 1 + x*drawColorInfo.height + y]);
				ptr[2] = pxcBYTE(_colorInput[cw*ch * 0 + x*drawColorInfo.height + y]);
				ptr[3] = pxcBYTE(255.0f);

			}
		}
		drawColor->ReleaseAccess(&drawColorDat);

		PXCImage *resultDepth;
		PXCImage::ImageInfo resultDepthInfo;
		memset(&drawColorInfo, 0, sizeof(resultDepthInfo));
		resultDepthInfo.width = cw;
		resultDepthInfo.height = ch;
		resultDepthInfo.format = PXCImage::PIXEL_FORMAT_DEPTH;
		resultDepth = 0;
		resultDepth = session->CreateImage(&resultDepthInfo);
		resultDepth = projection->CreateDepthImageMappedToColor(drawDepth, drawColor);
		// Initialize the Point cloud array which needs to passed to MATLAB
		_alignedDepthValues.ints = new int[cw*ch];
		_alignedDepthValues.nints = cw*ch;
		PXCImage::ImageData rddata;
		pxcStatus sts = resultDepth->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH, &rddata);
		for (size_t pixID_x = 0; pixID_x < ch; pixID_x++) 
		{
			for (size_t pixID_y = 0; pixID_y < cw; pixID_y++)
			{
				_alignedDepthValues.ints[pixID_x + ch * pixID_y] = *(pxcU16*)(rddata.planes[0] + pixID_x * cw * 2 + 2 * pixID_y);
				
			}
		}
		resultDepth->ReleaseAccess(&rddata);
		psm->Release();
		*(reinterpret_cast<imaqkit::propertytypes::NInts*>(value)) = _alignedDepthValues;
	};

private:
	RealSenseAdaptor* _parent;

	// Depth Point cloud values.
	imaqkit::propertytypes::NInts _alignedDepthValues;

	// Depth Input.
	//unsigned short* _depthInput;
	std::unique_ptr<UINT16[]> _depthInput;
	std::unique_ptr<UINT8[]> _colorInput;
	// Calculates depth to point cloud.
	void getAlignmentD2C(void* value);


};

#endif