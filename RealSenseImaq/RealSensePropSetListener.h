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
#ifndef __REALSENSE_PROP_SET_LISTENER_HEADER__
#define __REALSENSE_PROP_SET_LISTENER_HEADER__


#include "mwadaptorimaq.h" 
#include "RealSenseAdaptor.h" // For this example

class RealsensePropSetListener : public imaqkit::IPropPostSetListener
{
public:

	// Constructor/Destructor
	RealsensePropSetListener(RealSenseAdaptor* parent) :
		_parent(parent) {}

	virtual ~RealsensePropSetListener() {};

	virtual void notify(imaqkit::IPropInfo* propertyInfo,
		void* newValue);

private:

	// Declare handle to parent as member data
	RealSenseAdaptor* _parent;

	// Property Information object.
	imaqkit::IPropInfo* _propInfo;

	// The new value for integer properties.
	int _lastIntValue;

	// The new value for double properties.
	double _lastDoubleValue;

	// The new value for string properties.
	char* _lastStrValue;

};

#endif