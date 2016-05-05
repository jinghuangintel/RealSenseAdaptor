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
#include "RealSenseAdaptor.h"
#include "RealSenseDeviceFormat.h"
#include "RealSensePropGetListener.h"
#include "RealSensePropSetListener.h"
#include "RealSensePointCloudListener.hpp"
#include "RealSenseC2DAlignmentListener.hpp"
#include "RealSenseD2CAlignmentListener.hpp"
#include "pxcsensemanager.h"
#include "util_render.h"
#include <vector>

using namespace std;
// Class constructor
RealSenseAdaptor::RealSenseAdaptor(imaqkit::IEngine* engine, const
	imaqkit::IDeviceInfo* deviceInfo,
	const char* formatName) : imaqkit::IAdaptor(engine), _di(deviceInfo), _acquireThread(static_cast<Thread>(NULL)), _formatName(formatName), _acquisitionActive(false)
{
	initDevice();

	// Create a critical section handle.
	_driverGuard = imaqkit::createCriticalSection();
	_acquisitionActiveGuard = imaqkit::createCriticalSection();
}
// Class destructor
RealSenseAdaptor::~RealSenseAdaptor()
{
	// Stop the device.
	stop();

	// Close the device.
	close();

	// Cleanup - deallocate the critical section handle.
	delete _driverGuard;
}

void RealSenseAdaptor::initDevice()
{
	// get a handle to the property container
	imaqkit::IPropContainer* propContainer =
		getEngine()->getAdaptorPropContainer();

	// Determine the number of properties in the container.
	int numDeviceProps = propContainer->getNumberProps();

	// Retrieve the names of all the properties in the container
	const char **devicePropNames = new const
		char*[numDeviceProps];
	propContainer->getPropNames(devicePropNames);

	// Create a variable to point to a property get listener object.
	RealsensePropGetListener* getListener;
	// Create a variable to point to a property listener object.
	RealsensePropSetListener* setListener;

	// For each property in the container...
	for (int i = 0; i < numDeviceProps; i++){

		if (std::string("OutputPointCloudD2P") == std::string(devicePropNames[i])) 
		{
			propContainer->setCustomGetFcn(devicePropNames[i], new RealSensePointCloudGetListener(this));
			continue;
		}

		if (std::string("OutputAlignmentD2C") == std::string(devicePropNames[i])) 
		{
			propContainer->setCustomGetFcn(devicePropNames[i], new RealSenseD2CAlignmentGetListener(this));
			continue;
		}

		if (std::string("OutputAlignmentC2D") == std::string(devicePropNames[i])) 
		{
			propContainer->setCustomGetFcn(devicePropNames[i], new RealSenseC2DAlignmentGetListener(this));
			continue;
		}


		// There is no need to add listener for this property
		if (std::string("InputColorImage") == std::string(devicePropNames[i])) 
		{
			continue;
		}
		if (std::string("InputColorHeight") == std::string(devicePropNames[i])) 
		{
			continue;
		}
		if (std::string("InputColorWidth") == std::string(devicePropNames[i])) 
		{
			continue;
		}
		if (std::string("InputDepthMap") == std::string(devicePropNames[i])) 
		{
			continue;
		}
		if (std::string("InputDepthHeight") == std::string(devicePropNames[i])) 
		{
			continue;
		}
		if (std::string("InputDepthWidth") == std::string(devicePropNames[i])) 
		{
			continue;
		}
		if (std::string("CameraID") == std::string(devicePropNames[i])) 
		{
			continue;
		}
		// Create a get listener object...
		getListener = new RealsensePropGetListener(this);
		// and associate it with a specific property.
		propContainer->setCustomGetFcn(devicePropNames[i], getListener);

		// Create a set listener object...
		setListener = new RealsensePropSetListener(this);

		// and associate it with a specific property.
		propContainer->addListener(devicePropNames[i], setListener);

	}

	// clean up the array of property names.

	delete[] devicePropNames;

}

// Device driver information functions
const char* RealSenseAdaptor::getDriverDescription() const
{
	return "MyDevice_Driver";
}
const char* RealSenseAdaptor::getDriverVersion() const 
{
	return "1.0.0";
}
// Image data information functions
int RealSenseAdaptor::getMaxWidth() const 
{
	// Get the format information object.
	RealSenseDeviceFormat* formatInfo = getFormatInfo();
	//return 480;
	return formatInfo->getFormatWidth();

}
int RealSenseAdaptor::getMaxHeight() const 
{
	RealSenseDeviceFormat* formatInfo = getFormatInfo();
	//return 360;
	return formatInfo->getFormatHeight();
}
int RealSenseAdaptor::getNumberOfBands() const 
{
	RealSenseDeviceFormat* formatInfo = getFormatInfo();
	//return 1;
	return formatInfo->getFormatNumBands();
}

bool RealSenseAdaptor::isAcquisitionActive(void) const
{
	std::auto_ptr<imaqkit::IAutoCriticalSection> acquisitionActiveSection(imaqkit::createAutoCriticalSection(_acquisitionActiveGuard, true));
	return _acquisitionActive;
}

void RealSenseAdaptor::setAcquisitionActive(bool state)
{
	std::auto_ptr<imaqkit::IAutoCriticalSection> acquisitionActiveSection(imaqkit::createAutoCriticalSection(_acquisitionActiveGuard, true));
	_acquisitionActive = state;
}

const char*  RealSenseAdaptor::getFormatName() const
{
	return _formatName;
}
imaqkit::frametypes::FRAMETYPE RealSenseAdaptor::getFrameType()
const {
	RealSenseDeviceFormat* formatInfo = getFormatInfo();
	return formatInfo->getFormatFrameType();
	//return imaqkit::frametypes::MONO16;
}
// Image acquisition functions
bool RealSenseAdaptor::openDevice()
{
	if (isOpen())
		return true;// Create the image acquistion thread.

	_acquireThread = CreateThread(NULL, 0, acquireThread, this, 0, &_acquireThreadID);
	if (_acquireThread == NULL) {
		//closeDevice();
		return false;
	}
	// Wait for thread to create message queue.
	while (PostThreadMessage(_acquireThreadID, WM_USER + 1, 0, 0) == 0)
		Sleep(1);
	return true;

}

bool RealSenseAdaptor::startCapture()
{
	// Check if device is already acquiring frames.
	if (!isOpen())
		return false;

	getEngine()->getAdaptorPropContainer()->notifyAllListeners();
	// Set the acquiring frames flag.
	setAcquisitionActive(true);
	// Send start message to acquisition thread
	PostThreadMessage(_acquireThreadID, WM_USER, 0, 0);
	return true;

}


DWORD WINAPI RealSenseAdaptor::acquireThread(void* param) 
{
	
	RealSenseAdaptor* adaptor = reinterpret_cast<RealSenseAdaptor*>(param);
	int camera_id = adaptor->getDeviceID();
	
	// Set the thread priority.
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		switch (msg.message) {
		case WM_USER:
		{
			if (camera_id == 1)
			{
				sendf200colorframe(adaptor);
			}
			else if (camera_id == 3)
			{
				sendr200colorframe(adaptor);
			}
			else if (camera_id ==2)
			{
				sendf200depthframe(adaptor);
			}
			else
			{
				sendr200depthframe(adaptor);
			}
			break;
		} //switch-case WM_USER
		}//while message is not WM_QUIT
	}
	return 0;
}
	
void RealSenseAdaptor::sendf200colorframe(RealSenseAdaptor* adaptor)
	{
		// Check if a frame needs to be acquired
		std::auto_ptr<imaqkit::IAutoCriticalSection> driverSection(imaqkit::createAutoCriticalSection(adaptor->_driverGuard, false));
		// Get frame type & dimensions.

		imaqkit::frametypes::FRAMETYPE frameType =
			adaptor->getFrameType();
		int imWidth = adaptor->getMaxWidth();
		int imHeight = adaptor->getMaxHeight();
		int imBand = adaptor->getNumberOfBands();

		PXCSession *session = PXCSession_Create();
		if (!session) return ;
		PXCSenseManager *psm = session->CreateSenseManager();
		psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, imWidth, imHeight, RealSense::RS_FPS);
		UtilRender *renderColor = new UtilRender(L"COLOR STREAM");
		psm->Init();
		PXCImage *colorIm;

		while (adaptor->isAcquisitionNotComplete() && adaptor->isAcquisitionActive()) {
			driverSection->enter();
			if (psm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;
			// retrieve all available image samples
			PXCCapture::Sample *sample = psm->QuerySample();
			if (!sample) break;
			unsigned char *imBuffer1 = new unsigned char[imWidth * imHeight * imBand];
			
			colorIm = sample->color;
			if (!colorIm) break;
			PXCImage::ImageData cdata;
			pxcStatus sts = colorIm->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB32, &cdata);
			for (int ix = 0; ix < imHeight; ix++)
			{
				for (int jx = 0; jx < imWidth; jx++)
				{
					for (int c = 0; c < imBand; c++)
					{
						imBuffer1[(ix * imWidth + jx) * imBand + imBand - 1 - c] = *(pxcBYTE*)(cdata.planes[0] + ix * imWidth * 4 + 4 * jx + c);
					}
				}
			}
			colorIm->ReleaseAccess(&cdata);
			renderColor->RenderFrame(colorIm);

			if (adaptor->isSendFrame()) {
				int roiOriginX, roiOriginY, roiWidth, roiHeight;
				adaptor->getROI(roiOriginX,
					roiOriginY,
					roiWidth,
					roiHeight);
				// Create a frame object
				imaqkit::IAdaptorFrame* frame = adaptor->getEngine()->makeFrame(frameType,  //frametype
					roiWidth,  //width
					roiHeight);
				// Copy data from buffer into frame object.
				frame->setImage(imBuffer1,
					imWidth,
					imHeight,
					roiOriginX, // X Offset from origin
					roiOriginY); // Y Offset from origin
				// Set image's timestamp.
				frame->setTime(imaqkit::getCurrentTime());
				// Send frame object to engine.
				adaptor->getEngine()->receiveFrame(frame);
			} // if isSendFrame()		
			psm->ReleaseFrame();
			// Increment the frame count.
			adaptor->incrementFrameCount();
			// Cleanup. Deallocate imBuffer.
			delete[] imBuffer1;
			
			driverSection->leave();
		} // while(isAcquisitionNotComplete()
		delete renderColor;
		psm->Release();
	}

void RealSenseAdaptor::sendr200colorframe(RealSenseAdaptor* adaptor)
{
	// Check if a frame needs to be acquired
	std::auto_ptr<imaqkit::IAutoCriticalSection> driverSection(imaqkit::createAutoCriticalSection(adaptor->_driverGuard, false));
	// Get frame type & dimensions.

	imaqkit::frametypes::FRAMETYPE frameType =
		adaptor->getFrameType();
	int imWidth = adaptor->getMaxWidth();
	int imHeight = adaptor->getMaxHeight();
	int imBand = adaptor->getNumberOfBands();

	PXCSession *session = PXCSession_Create();
	if (!session) return;
	PXCSenseManager *psm = session->CreateSenseManager();
	psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, imWidth, imHeight, RealSense::RS_FPS);
	UtilRender *renderColor = new UtilRender(L"COLOR STREAM");
	psm->Init();
	PXCImage *colorIm;

	while (adaptor->isAcquisitionNotComplete() && adaptor->isAcquisitionActive()) {
		driverSection->enter();
		if (psm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;
		// retrieve all available image samples
		PXCCapture::Sample *sample = psm->QuerySample();
		if (!sample) break;
		unsigned char *imBuffer1 = new unsigned char[imWidth * imHeight * imBand];

		colorIm = sample->color;
		if (!colorIm) break;
		PXCImage::ImageData cdata;
		pxcStatus sts = colorIm->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB32, &cdata);
		for (int ix = 0; ix < imHeight; ix++)
		{
			for (int jx = 0; jx < imWidth; jx++)
			{
				for (int c = 0; c < imBand; c++)
				{
					imBuffer1[(ix * imWidth + jx) * imBand + imBand - 1 - c] = *(pxcBYTE*)(cdata.planes[0] + ix * imWidth * 4 + 4 * jx + c);
				}
			}
		}
		colorIm->ReleaseAccess(&cdata);
		renderColor->RenderFrame(colorIm);
		
		if (adaptor->isSendFrame()) {
			int roiOriginX, roiOriginY, roiWidth, roiHeight;
			adaptor->getROI(roiOriginX,
				roiOriginY,
				roiWidth,
				roiHeight);
			// Create a frame object
			imaqkit::IAdaptorFrame* frame = adaptor->getEngine()->makeFrame(frameType,  //frametype
				roiWidth,  //width
				roiHeight);
			// Copy data from buffer into frame object.
			frame->setImage(imBuffer1,
				imWidth,
				imHeight,
				roiOriginX, // X Offset from origin
				roiOriginY); // Y Offset from origin
			// Set image's timestamp.
			frame->setTime(imaqkit::getCurrentTime());
			// Send frame object to engine.
			adaptor->getEngine()->receiveFrame(frame);
		} // if isSendFrame()		
		psm->ReleaseFrame();
		// Increment the frame count.
		adaptor->incrementFrameCount();
		// Cleanup. Deallocate imBuffer.
		delete[] imBuffer1;

		driverSection->leave();
	} // while(isAcquisitionNotComplete()
	delete renderColor;
	psm->Release();

}
void RealSenseAdaptor::sendf200depthframe(RealSenseAdaptor* adaptor)
{
	
	// Check if a frame needs to be acquired
	std::auto_ptr<imaqkit::IAutoCriticalSection> driverSection(imaqkit::createAutoCriticalSection(adaptor->_driverGuard, false));
	// Get frame type & dimensions.

	imaqkit::frametypes::FRAMETYPE frameType =
		adaptor->getFrameType();
	int imWidth = adaptor->getMaxWidth();
	int imHeight = adaptor->getMaxHeight();
	int imBand = adaptor->getNumberOfBands();

	PXCSession *session = PXCSession_Create();
	if (!session) return;
	PXCSenseManager *psm = session->CreateSenseManager();
	psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, imWidth, imHeight, RealSense::RS_FPS);
	UtilRender *renderDepth = new UtilRender(L"DEPTH STREAM");
	psm->Init();
	PXCImage *depthIm;

	while (adaptor->isAcquisitionNotComplete() && adaptor->isAcquisitionActive()) {
		driverSection->enter();
		if (psm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;
		// retrieve all available image samples
		PXCCapture::Sample *sample = psm->QuerySample();
		if (!sample) break;
		unsigned short *imBuffer2 = new unsigned short[imWidth * imHeight];
		depthIm = sample->depth;
		if (!depthIm) break;

		PXCImage::ImageData ddata;
		pxcStatus sts = depthIm->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH, &ddata);
		for (int id = 0; id < imHeight; id++)
		{
			for (int jd = 0; jd < imWidth; jd++)
			{

				imBuffer2[id * imWidth + jd] = *(pxcU16*)(ddata.planes[0] + id * imWidth * 2 + jd * 2);

			}
		}
		depthIm->ReleaseAccess(&ddata);
		renderDepth->RenderFrame(depthIm);
		

		if (adaptor->isSendFrame()) {
			int roiOriginX, roiOriginY, roiWidth, roiHeight;
			adaptor->getROI(roiOriginX,
				roiOriginY,
				roiWidth,
				roiHeight);
			// Create a frame object
			imaqkit::IAdaptorFrame* frame = adaptor->getEngine()->makeFrame(frameType,  //frametype
				roiWidth,  //width
				roiHeight);
			// Copy data from buffer into frame object.
			frame->setImage(imBuffer2,
				imWidth,
				imHeight,
				roiOriginX, // X Offset from origin
				roiOriginY); // Y Offset from origin
			// Set image's timestamp.
			frame->setTime(imaqkit::getCurrentTime());
			// Send frame object to engine.
			adaptor->getEngine()->receiveFrame(frame);
		} // if isSendFrame()		
		psm->ReleaseFrame();
		// Increment the frame count.
		adaptor->incrementFrameCount();
		// Cleanup. Deallocate imBuffer.
		delete[] imBuffer2;

		driverSection->leave();
	} // while(isAcquisitionNotComplete()
	delete renderDepth;
	psm->Release();
	
}


void RealSenseAdaptor::sendr200depthframe(RealSenseAdaptor* adaptor)
{
	// Check if a frame needs to be acquired
	std::auto_ptr<imaqkit::IAutoCriticalSection> driverSection(imaqkit::createAutoCriticalSection(adaptor->_driverGuard, false));
	// Get frame type & dimensions.

	imaqkit::frametypes::FRAMETYPE frameType =
		adaptor->getFrameType();
	int imWidth = adaptor->getMaxWidth();
	int imHeight = adaptor->getMaxHeight();
	int imBand = adaptor->getNumberOfBands();

	PXCSession *session = PXCSession_Create();
	if (!session) return;
	PXCSenseManager *psm = session->CreateSenseManager();
	psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, imWidth, imHeight, RealSense::RS_FPS);
	
	UtilRender *renderDepth = new UtilRender(L"DEPTH STREAM");
	psm->Init();
	PXCImage *depthIm;

	while (adaptor->isAcquisitionNotComplete() && adaptor->isAcquisitionActive()) {
		driverSection->enter();
		if (psm->AcquireFrame(true) < PXC_STATUS_NO_ERROR) break;
		// retrieve all available image samples
		PXCCapture::Sample *sample = psm->QuerySample();
		if (!sample) break;
		unsigned short *imBuffer2 = new unsigned short[imWidth * imHeight];
		depthIm = sample->depth;
		if (!depthIm) break;

		PXCImage::ImageData ddata;
		pxcStatus sts = depthIm->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH, &ddata);
		for (int id = 0; id < imHeight; id++)
		{
			for (int jd = 0; jd < imWidth; jd++)
			{

				imBuffer2[id * imWidth + jd] = *(pxcU16*)(ddata.planes[0] + id * imWidth * 2 + jd * 2);

			}
		}
		depthIm->ReleaseAccess(&ddata);
		renderDepth->RenderFrame(depthIm);


		if (adaptor->isSendFrame()) {
			int roiOriginX, roiOriginY, roiWidth, roiHeight;
			adaptor->getROI(roiOriginX,
				roiOriginY,
				roiWidth,
				roiHeight);
			// Create a frame object
			imaqkit::IAdaptorFrame* frame = adaptor->getEngine()->makeFrame(frameType,  //frametype
				roiWidth,  //width
				roiHeight);
			// Copy data from buffer into frame object.
			frame->setImage(imBuffer2,
				imWidth,
				imHeight,
				roiOriginX, // X Offset from origin
				roiOriginY); // Y Offset from origin
			// Set image's timestamp.
			frame->setTime(imaqkit::getCurrentTime());
			// Send frame object to engine.
			adaptor->getEngine()->receiveFrame(frame);
		} // if isSendFrame()		
		psm->ReleaseFrame();
		// Increment the frame count.
		adaptor->incrementFrameCount();
		// Cleanup. Deallocate imBuffer.
		delete[] imBuffer2;

		driverSection->leave();
	} // while(isAcquisitionNotComplete()
	delete renderDepth;
	psm->Release();
}
bool RealSenseAdaptor::closeDevice()
{
	// If the device is not open, return.
	if (!isOpen())
		return true;
	// Terminate and close the acquisition thread.
	if (_acquireThread) {
		// Send WM_QUIT message to thread.
		PostThreadMessage(_acquireThreadID, WM_QUIT, 0, 0);
		// Give the thread a chance to finish. 
		WaitForSingleObject(_acquireThread, 10000);
		// Close thread handle.
		CloseHandle(_acquireThread);
		_acquireThread = NULL;
	}
	return true;
}


bool RealSenseAdaptor::stopCapture()
{
	if (!isAcquiring())
		return true;
	
	setAcquisitionActive(false);
	
    std::auto_ptr<imaqkit::IAutoCriticalSection> driverSection(imaqkit::createAutoCriticalSection(_driverGuard, true));

	driverSection->leave();
	return true;
}

RealSenseDeviceFormat* RealSenseAdaptor::getFormatInfo() const
{

	imaqkit::IDeviceFormat* selectedFormat = _di->getDeviceFormat(_formatName);

	if (selectedFormat) {
		return dynamic_cast<RealSenseDeviceFormat*>(selectedFormat->getAdaptorData());
	}
	else {
		return NULL;
	}
}

int RealSenseAdaptor::getDeviceID() const
{
	return _di->getDeviceID();
}