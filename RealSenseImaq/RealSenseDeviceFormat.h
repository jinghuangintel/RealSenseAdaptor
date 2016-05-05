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
#ifndef __REALSENSE_DEVICEFORMAT_HEADER__
#define __REALSENSE_DEVICEFORMAT_HEADER__

#include "mwadaptorimaq.h"

class RealSenseDeviceFormat : public imaqkit::IMAQInterface
{

public:

	// *******************************************************************
	// CONSTRUCTOR/DESTRUCTOR
	// *******************************************************************
	RealSenseDeviceFormat(void);
	virtual ~RealSenseDeviceFormat(void);

	// *******************************************************************
	// METHODS FOR ACCESSING VIDEO FORMAT INFORMATION.
	// *******************************************************************
	/// Retrieve height.
	int getFormatHeight() const;

	/// Retrieve width.
	int getFormatWidth() const;

	/// Retrieve number of bands.
	int getFormatNumBands() const;

	/// Retrieve frame type.
	imaqkit::frametypes::FRAMETYPE getFormatFrameType() const;


	// *******************************************************************
	// METHODS FOR SAVING VIDEO INFORMATION.
	// *******************************************************************
	/// Save video format height.
	void setFormatHeight(int value);

	/// Save video format width.
	void setFormatWidth(int value);

	/// Save number of bands.
	void setFormatNumBands(int value);

	/// Save frame type.
	void setFormatFrameType(imaqkit::frametypes::FRAMETYPE value);


private:
	/// The height of the frame in pixels
	int _formatHeight;

	/// The width of the frame in pixels
	int _formatWidth;

	/// The number of bands of data returned
	int _formatNumBands;

	/// The frame type
	imaqkit::frametypes::FRAMETYPE _formatFrameType;
};

#endif
