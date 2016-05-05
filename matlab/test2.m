%%%%%%%%%%%% Render and Save F200 color and depth streams simultaneously
vid = videoinput('realsenseimaq', 2);
vid2 = videoinput('realsenseimaq',1);
triggerconfig([vid vid2],'manual');
vid.FramesPerTrigger = 30;
vid2.FramesPerTrigger = 30;
vid.TriggerRepeat = 1;
vid2.TriggerRepeat = 1;

start([vid vid2]);

trigger([vid vid2]);

colordata = getdata(vid2);
depthdata = getdata(vid); 

stop([vid vid2]);
%%%%%%%%%%%% Render and save R200 color and depth streams simultaneously
vid = videoinput('realsenseimaq', 4);
vid2 = videoinput('realsenseimaq',3);
triggerconfig([vid vid2],'manual');
vid.FramesPerTrigger = 30;
vid2.FramesPerTrigger = 30;
vid.TriggerRepeat = 1;
vid2.TriggerRepeat = 1;

start([vid vid2]);

trigger([vid vid2]);

colordata = getdata(vid2);
depthdata = getdata(vid); 

stop([vid vid2]);
%%%%%%%%%%%% Color-to-depth alignment and generate point cloud F200
vid = videoinput('realsenseimaq', 2);
vid2 = videoinput('realsenseimaq',1);
triggerconfig([vid vid2],'manual');
vid.FramesPerTrigger = 30;
vid2.FramesPerTrigger = 30;
vid.TriggerRepeat = 1;
vid2.TriggerRepeat = 1;

start([vid vid2]);

trigger([vid vid2]);

colordata = getdata(vid2);
depthdata = getdata(vid); 

stop([vid vid2]);

Nframes = size(depthdata,4);
md=size(depthdata,1);
nd=size(depthdata,2);
depth=depthdata(:,:,:,15);
inputdepth = reshape(depth',1,md*nd);

mc=size(colordata,1);
nc=size(colordata,2);
color=colordata(:,:,:,15);
inputcolor = reshape(color,1,mc*nc*3);
src = getselectedsource(vid);

src.InputColorImage = inputcolor; 
src.InputColorHeight=mc;
src.InputColorWidth=nc;
src.CameraID = 2;

src.InputDepthMap = inputdepth; 
src.InputDepthHeight=md;
src.InputDepthWidth=nd;
alignedcolor = src.OutputAlignmentC2D;
%aligneddepth = src.OutputAlignmentD2C;
%src.InputDepthMap = aligneddepth; 
%src.InputDepthHeight=mc;
%src.InputDepthWidth=nc;
d2pcl =  src.OutputPointCloudD2P;

alignedc=reshape(alignedcolor,[480,640,3]);
c=uint8(alignedc);
image(c);

%%%%%%%%%%%% Depth-to-color alignment and generate point cloud F200
vid = videoinput('realsenseimaq', 2);
vid2 = videoinput('realsenseimaq',1);
triggerconfig([vid vid2],'manual');
vid.FramesPerTrigger = 30;
vid2.FramesPerTrigger = 30;
vid.TriggerRepeat = 1;
vid2.TriggerRepeat = 1;

start([vid vid2]);

trigger([vid vid2]);

colordata = getdata(vid2);
depthdata = getdata(vid); 

stop([vid vid2]);

Nframes = size(depthdata,4);
md=size(depthdata,1);
nd=size(depthdata,2);
depth=depthdata(:,:,:,15);
inputdepth = reshape(depth',1,md*nd);

mc=size(colordata,1);
nc=size(colordata,2);
color=colordata(:,:,:,15);
inputcolor = reshape(color,1,mc*nc*3);
src = getselectedsource(vid);

src.InputColorImage = inputcolor; 
src.InputColorHeight=mc;
src.InputColorWidth=nc;
src.CameraID = 2;

src.InputDepthMap = inputdepth; 
src.InputDepthHeight=md;
src.InputDepthWidth=nd;
%alignedcolor = src.OutputAlignmentC2D;
aligneddepth = src.OutputAlignmentD2C;
src.InputDepthMap = aligneddepth; 
src.InputDepthHeight=mc;
src.InputDepthWidth=nc;
d2pcl =  src.OutputPointCloudD2P;

%alignedc=reshape(alignedcolor,[480,640,3]);
%c=uint8(alignedc);
%image(c);



%%%%%%%%%%%% Color-to-depth alignment and generate point cloud R200
vid = videoinput('realsenseimaq', 4);
vid2 = videoinput('realsenseimaq',3);
triggerconfig([vid vid2],'manual');
vid.FramesPerTrigger = 30;
vid2.FramesPerTrigger = 30;
vid.TriggerRepeat = 1;
vid2.TriggerRepeat = 1;

start([vid vid2]);

trigger([vid vid2]);

colordata = getdata(vid2);
depthdata = getdata(vid); 

stop([vid vid2]);

Nframes = size(depthdata,4);
md=size(depthdata,1);
nd=size(depthdata,2);
depth=depthdata(:,:,:,15);
inputdepth = reshape(depth',1,md*nd);

mc=size(colordata,1);
nc=size(colordata,2);
color=colordata(:,:,:,15);
inputcolor = reshape(color,1,mc*nc*3);
src = getselectedsource(vid);

src.InputColorImage = inputcolor; 
src.InputColorHeight=mc;
src.InputColorWidth=nc;
src.CameraID = 4;

src.InputDepthMap = inputdepth; 
src.InputDepthHeight=md;
src.InputDepthWidth=nd;
alignedcolor = src.OutputAlignmentC2D;
%aligneddepth = src.OutputAlignmentD2C;
%src.InputDepthMap = aligneddepth; 
%src.InputDepthHeight=mc;
%src.InputDepthWidth=nc;
d2pcl =  src.OutputPointCloudD2P;

alignedc=reshape(alignedcolor,[360,480,3]);
c=uint8(alignedc);
image(c);

%%%%%%%%%%%% Depth-to-color alignment and generate point cloud R200

vid = videoinput('realsenseimaq', 4);
vid2 = videoinput('realsenseimaq',3);
triggerconfig([vid vid2],'manual');
vid.FramesPerTrigger = 30;
vid2.FramesPerTrigger = 30;
vid.TriggerRepeat = 1;
vid2.TriggerRepeat = 1;

start([vid vid2]);

trigger([vid vid2]);

colordata = getdata(vid2);
depthdata = getdata(vid); 

stop([vid vid2]);

Nframes = size(depthdata,4);
md=size(depthdata,1);
nd=size(depthdata,2);
depth=depthdata(:,:,:,15);
inputdepth = reshape(depth',1,md*nd);

mc=size(colordata,1);
nc=size(colordata,2);
color=colordata(:,:,:,15);
inputcolor = reshape(color,1,mc*nc*3);
src = getselectedsource(vid);

src.InputColorImage = inputcolor; 
src.InputColorHeight=mc;
src.InputColorWidth=nc;
src.CameraID = 4;

src.InputDepthMap = inputdepth; 
src.InputDepthHeight=md;
src.InputDepthWidth=nd;
%alignedcolor = src.OutputAlignmentC2D;
aligneddepth = src.OutputAlignmentD2C;
src.InputDepthMap = aligneddepth; 
src.InputDepthHeight=mc;
src.InputDepthWidth=nc;
d2pcl =  src.OutputPointCloudD2P;

%alignedc=reshape(alignedcolor,[360,480,3]);
%c=uint8(alignedc);
%image(c);