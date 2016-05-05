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

#ifndef __REALSENSE_ADAPTOR_HEADER__
#define __REALSENSE_ADAPTOR_HEADER__

#include "mwadaptorimaq.h" // required header
#include <windows.h>
#include "RealSenseDeviceFormat.h"
#include "pxcprojection.h"
typedef DWORD ThreadReturnType;
typedef HANDLE Thread;
class RealSenseAdaptor : public imaqkit::IAdaptor 
{
public:
	// Constructor and Destructor
	RealSenseAdaptor(imaqkit::IEngine* engine, const
		imaqkit::IDeviceInfo* deviceInfo,
		const char* formatName);
	virtual ~RealSenseAdaptor();
	// Adaptor and Image Information Functions
	virtual const char* getDriverDescription() const;
	virtual const char* getDriverVersion() const;
	virtual int getMaxWidth() const;
	virtual int getMaxHeight() const;
	virtual int getNumberOfBands() const;
	virtual imaqkit::frametypes::FRAMETYPE getFrameType() const;
	const char* getFormatName() const;
	bool isAcquisitionActive(void) const;
	// Image Acquisition Functions
	virtual bool openDevice();
	virtual bool closeDevice();
	virtual bool startCapture();
	virtual bool stopCapture();
private:
	void initDevice();
	void setAcquisitionActive(bool state);
	// Declaration of acquisition thread function
	static DWORD WINAPI acquireThread(void* param);
	static void sendf200colorframe(RealSenseAdaptor* adaptor);
	static void sendf200depthframe(RealSenseAdaptor* adaptor);
	static void sendr200colorframe(RealSenseAdaptor* adaptor);
	static void sendr200depthframe(RealSenseAdaptor* adaptor);
	/// The driver critical section.
	imaqkit::ICriticalSection* _driverGuard;
	const imaqkit::IDeviceInfo* _di;
	// Thread variable
	HANDLE _acquireThread;
	/// The acquisition active critical section.
	/// Handle to the thread.

	imaqkit::ICriticalSection* _acquisitionActiveGuard;
	// Thread ID returned by Windows.
	DWORD _acquireThreadID;
	const char* _formatName;
	RealSenseDeviceFormat* getFormatInfo() const;
	int getDeviceID() const;
	bool _acquisitionActive;
};

#endif