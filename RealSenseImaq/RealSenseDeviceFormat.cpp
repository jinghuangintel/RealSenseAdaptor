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

#include "RealSenseDeviceFormat.h"


RealSenseDeviceFormat::RealSenseDeviceFormat(void) :
_formatHeight(0), _formatWidth(0), _formatNumBands(0) {}

RealSenseDeviceFormat::~RealSenseDeviceFormat() {}

// Get/set frame height.
void RealSenseDeviceFormat::setFormatHeight(int value)
{
	_formatHeight = value;
}

int RealSenseDeviceFormat::getFormatHeight() const
{
	return _formatHeight;
}

// Get/set frame width.
void RealSenseDeviceFormat::setFormatWidth(int value)
{
	_formatWidth = value;
}

int RealSenseDeviceFormat::getFormatWidth() const
{
	return _formatWidth;
}

// Get/set number of bands.
void RealSenseDeviceFormat::setFormatNumBands(int value)
{
	_formatNumBands = value;
}

int RealSenseDeviceFormat::getFormatNumBands() const
{
	return _formatNumBands;
}

// Get/set frame type.
void RealSenseDeviceFormat::setFormatFrameType(imaqkit::frametypes::FRAMETYPE value)
{
	_formatFrameType = value;
}

imaqkit::frametypes::FRAMETYPE RealSenseDeviceFormat::getFormatFrameType() const
{
	return _formatFrameType;
}
