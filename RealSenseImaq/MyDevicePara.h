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


#ifndef __MYDEVICEPARA_HEADER__
#define __MYDEVICEPARA_HEADER__

#ifdef _WIN32
#include <windows.h>
#else
#endif

namespace RealSense
{
	/// Default width/HEIGHT/BAND if no image format is found.
	const int RS_FCOLOR_DEFAULT_WIDTH = 640;
	const int RS_FCOLOR_DEFAULT_HEIGHT = 480;
	const int RS_FCOLOR_DEFAULT_NUMBER_OF_BANDS = 3;
	const int RS_RCOLOR_DEFAULT_WIDTH = 640;
	const int RS_RCOLOR_DEFAULT_HEIGHT = 480;
	const int RS_RCOLOR_DEFAULT_NUMBER_OF_BANDS = 3;

	const int RS_FDEPTH_DEFAULT_WIDTH = 640;
	const int RS_FDEPTH_DEFAULT_HEIGHT = 480;
	const int RS_FDEPTH_DEFAULT_NUMBER_OF_BANDS = 1;
	const int RS_RDEPTH_DEFAULT_WIDTH = 480;
	const int RS_RDEPTH_DEFAULT_HEIGHT = 360;
	const int RS_RDEPTH_DEFAULT_NUMBER_OF_BANDS = 1;
	const int RS_FPS = 30;
	const double R200_BASELINE_IN_MM = 70.0;

};


#endif