#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <string>
#include "../../bins/SVCam_Kit_v2.5.0_Linux_AMD64/include/sv_gen_sdk.h"
#define INFINITE 0xFFFFFFFF
using namespace std;

int main(){
// Parameters
	uint32_t uiIndex = 0;


//******************Initialization************************
	SV_RETURN stat= SVLibInit("/usr/lib/libsvu3vtl.so");
	if (stat != SV_ERROR_SUCCESS){
		cout<<"ERROR by initialization!"<<endl;		
	}else{
		cout<<"Initialization succeeded!"<<endl;
	}


//******************GetSystemCounts************************
	uint32_t tlCount=0;
	stat = SVLibSystemGetCount(&tlCount);

//******************Get_LIB_SystemInfo************************
	SV_TL_INFO tlInfo;
	stat = SVLibSystemGetInfo(uiIndex,&tlInfo);
	cout<<"state of LIB_System_GetInfo: "<<stat<<endl;
	printf("************LIB_System_Info*************\n");
	printf("id:%s\n",tlInfo.id);
	printf("Vendor:%s\n",tlInfo.vendor);
	printf("Model:%s\n",tlInfo.model);
	printf("Name: %s\n",tlInfo.name);
	printf("pathName: %s\n",tlInfo.pathName);
	printf("DisplayName:%s\n",tlInfo.displayName);
	printf("TLType:%s\n",tlInfo.tlType);
	printf("gentlVersionMajor:%d\n",tlInfo.gentlVersionMajor);
	printf("gentlVersionMinor:%d\n",tlInfo.gentlVersionMinor);
	printf("encoding: %d\n",tlInfo.encoding);	
	


//******************LIB_System_Open************************
	SV_SYSTEM_HANDLE hSystem = NULL;
	stat = SVLibSystemOpen(uiIndex,&hSystem);
	cout<<"state of SystemInitialization: "<<stat<<endl;


//******************SVSystemGetINFO************************
	stat = SVSystemGetInfo(hSystem, &tlInfo);
	printf("state of SVSystemGetInfo: %d\n", stat);
	

//******************EnumerationOfDevice*******************
	printf("Device discovery and enumeration...\n");
	bool bChanged = false;
	uint32_t numInterface=0;

	stat = SVSystemUpdateInterfaceList(hSystem, &bChanged,2000);
	cout<<"Update System Interface list, any Change? "<<bChanged<<endl;


	stat = SVSystemGetNumInterfaces(hSystem,&numInterface);
	cout<<"Number of interfaces on this System module: "<<numInterface<<endl;


	char interfaceId[SV_STRING_SIZE]={0};
	size_t interfaceIdSize = sizeof(interfaceId);
	stat = SVSystemGetInterfaceId(hSystem,uiIndex,interfaceId,&interfaceIdSize);
	cout<<"-------------- SVSystemGetInterface ---------------"<<endl;
	cout<<"InterfaceID: "<<interfaceId<<endl;
	cout<<"InterfaceIdSize: "<<interfaceIdSize<<endl;


	SV_INTERFACE_INFO interfaceInfo;
	stat = SVSystemInterfaceGetInfo(hSystem,interfaceId,&interfaceInfo);
	cout<<"Interface Info uid: "<<interfaceInfo.uid<<endl;
	cout<<"Interface Info displayName: "<<interfaceInfo.displayName<<endl;
	cout<<"Interface Info tyType: "<<interfaceInfo.tlType<<endl;



	SV_INTERFACE_HANDLE	hInterface;
	printf("open system interface...\n");
	stat=SVSystemInterfaceOpen(hSystem,interfaceId,&hInterface);
	printf("stat InterfaceOpen: %d\n",stat);

	
	cout<<"-----------After opening interface => SVInterfaceGetInfo ------------"<<endl;
	stat=SVInterfaceGetInfo(hInterface,&interfaceInfo);
	cout<<"Interface Info uid: "<<interfaceInfo.uid<<endl;
	cout<<"Interface Info displayName: "<<interfaceInfo.displayName<<endl;
	cout<<"Interface Info tyType: "<<interfaceInfo.tlType<<endl;


	stat=SVInterfaceUpdateDeviceList(hInterface, &bChanged, 5000);
	printf("stat SVInterfaceUpdateDeviceList: %d\n",stat);
	cout<<"Update the internal list of available Device on this interface, changed? "<<bChanged<<endl;

	uint32_t numDevices=0;
	stat=SVInterfaceGetNumDevices(hInterface,&numDevices);
	printf("InterfaceGetNumDevices: %d\n",numDevices);

	char deviceId[SV_STRING_SIZE]={0};
	size_t deviceIdSize=sizeof(deviceId);
	stat = SVInterfaceGetDeviceId(hInterface,uiIndex,deviceId,&deviceIdSize);
	cout<<"InterfaceGetDeviceId: "<<deviceId<<endl;
	cout<<"InterfaceGetDeviceID_Size: "<<deviceIdSize<<endl;
	
	// Get Interface device Info
	printf("--------------Without opening Device, Get Interface Device Info -----------\n");
	SV_DEVICE_INFO devInfo;
	stat=SVInterfaceDeviceGetInfo(hInterface,deviceId,&devInfo);
	printf("uid:%s\n",devInfo.uid);
	printf("Vendor:%s\n",devInfo.vendor);
	printf("Model:%s\n",devInfo.model);
	printf("DisplayName:%s\n",devInfo.displayName);
	printf("TLType:%s\n",devInfo.tlType);
	printf("Access Status: 0x:%x\n",devInfo.accessStatus);
	printf("User defined Name:%s\n",devInfo.userDefinedName);
	printf("Serial Number:%s\n",devInfo.serialNumber);
	printf("Version:%s\n",devInfo.version);
	printf("TimeStampFrequency: %ld\n",devInfo.timeStampFreq);	

	// open device
	SV_DEVICE_HANDLE hDevice;
	SV_REMOTE_DEVICE_HANDLE hRemoteDev;
	stat = SVInterfaceDeviceOpen(hInterface, deviceId, SV_DEVICE_ACCESS_CONTROL, &hDevice, &hRemoteDev);
	printf("state for InterfaceDeviceOpen: %d\n",stat);
	
	// Get Device Info
	printf("--------------After opening, Get Device Info ------------\n");
	stat = SVDeviceGetInfo(hDevice, &devInfo);
	printf("uid:%s\n",devInfo.uid);
	printf("Vendor:%s\n",devInfo.vendor);
	printf("Model:%s\n",devInfo.model);
	printf("DisplayName:%s\n",devInfo.displayName);
	printf("TLType:%s\n",devInfo.tlType);
	printf("Access Status: 0x:%x\n",devInfo.accessStatus);
	printf("User defined Name:%s\n",devInfo.userDefinedName);
	printf("Serial Number:%s\n",devInfo.serialNumber);
	printf("Version:%s\n",devInfo.version);
	printf("TimeStampFrequency: %ld\n",devInfo.timeStampFreq);	


// Queries the number of streams supported by device module
	uint32_t numOfStream=0;
	stat = SVDeviceGetNumStreams(hDevice, &numOfStream);
	printf("Supported number of streams: %d\n", numOfStream);

// Queries the stream id at index 0
	char streamId0[SV_STRING_SIZE]={0};
	size_t streamId0Size = SV_STRING_SIZE;
	stat = SVDeviceGetStreamId(hDevice, uiIndex, streamId0, &streamId0Size);
	cout << "StreamID0: "<<streamId0<< "; StreamID0Size: "<<streamId0Size<<endl;
	

// Open device Stream
	SV_STREAM_HANDLE hStream;
	stat = SVDeviceStreamOpen(hDevice, streamId0, &hStream);
	printf("Open Device Stream, state: %d\n", stat);

// -------------------- Feature Stuff -----------------------
	SV_FEATURE_HANDLE hFeature;
	SV_FEATURE_INFO featureInfo;
	uint32_t idxFeature = 0;
	while(true){
		stat = SVFeatureGetByIndex(hRemoteDev, idxFeature++, &hFeature);
		stat = SVFeatureGetInfo(hRemoteDev, hFeature, &featureInfo);
		if(stat!=SV_ERROR_SUCCESS){
			break;
		}
		
		if(featureInfo.visibility>0&&featureInfo.isImplemented>0&&featureInfo.isAvailable>0&&featureInfo.isLocked<1){
			printf("-------------Feature list index = %d --------------\n", idxFeature);
			cout<<"name: "<<featureInfo.name<<endl;
			cout<<"node: "<<featureInfo.node<<endl;
			cout<<"displayName: "<<featureInfo.displayName<<endl;
			cout<<"toolTip: "<<featureInfo.toolTip<<endl;
			printf("level: %d\n", featureInfo.level);
			printf("visibility: %d\n", featureInfo.visibility);
			printf("isImplemented: %d\n",featureInfo.isImplemented);
			printf("isAvailable: %d\n", featureInfo.isAvailable);
			printf("isLocked: %d\n", featureInfo.isLocked);
			printf("---------------------------------------------------\n");
		}
	}

// Get Temperature
	double devTemperature;
	SVFeatureGetByName(hRemoteDev, "DeviceTemperature", &hFeature);
	SVFeatureGetValueFloat(hRemoteDev, hFeature, &devTemperature);
	cout<<"Current temperature: "<<devTemperature<<endl;


// Loop for image acquisition
for(int i = 0; i<10; i++){
// Find payloadSize
	int64_t payloadSize;
	SVFeatureGetByName(hRemoteDev, "PayloadSize", &hFeature);
	SVFeatureGetValueInt64(hRemoteDev, hFeature, &payloadSize);
	printf("PayloadSize set, size: %ld \n",payloadSize);
	unsigned long psize = payloadSize;

// allocate buffers
SV_BUFFER_HANDLE hBuffer = NULL;
for(int c_buf=0; c_buf<1; c_buf++){
	size_t *buffer = new size_t[psize];
	// Allocate memory to the Data Stream associated with the hStream
	stat = SVStreamAnnounceBuffer(hStream, buffer, (uint32_t)payloadSize, NULL, &hBuffer);
	stat = SVStreamQueueBuffer(hStream, hBuffer);
	printf("SVStreamAnnounceBuffer and QueueBuffer %d done.\n", c_buf);
	}

	// FlushQueque and AcquisitionStart
	SVStreamFlushQueue(hStream, SV_ACQ_QUEUE_ALL_TO_INPUT);
	

// ---------------- Configure and Take images ---------------

	// Exposure time
	int fExposureMode=1; // 0-fixed exposure
	if(!fExposureMode){
		SVFeatureGetByName(hRemoteDev, "ExposureTime", &hFeature);
		stat = SVFeatureSetValueFloat(hRemoteDev, hFeature,60000);
		cout<<"FixedExposureTime set, State: "<<stat<<endl;
	}else{
		SVFeatureGetByName(hRemoteDev, "ExposureAutoOrder",&hFeature);
		stat = SVFeatureSetValueBool(hRemoteDev,hFeature,true);
		SVFeatureGetByName(hRemoteDev, "ExposureMode",&hFeature);
		stat = SVFeatureSetValueEnum(hRemoteDev,hFeature,"Timed");
		SVFeatureGetByName(hRemoteDev, "ExposureAuto",&hFeature);
		stat = SVFeatureSetValueEnum(hRemoteDev,hFeature,"Continuous");
		SVFeatureGetByName(hRemoteDev, "ExposureTimeMin",&hFeature);
		stat = SVFeatureSetValueFloat(hRemoteDev,hFeature,10000); //10ms
		SVFeatureGetByName(hRemoteDev, "ExposureTimeMax",&hFeature);
		stat = SVFeatureSetValueFloat(hRemoteDev,hFeature,5000000); //5s

		cout<<"AutoExposureTime set, State: "<<stat<<endl;
	}
	
	

	SVFeatureGetByName(hRemoteDev, "Gain", &hFeature);
	stat = SVFeatureSetValueInt64(hRemoteDev, hFeature,10,false);
	stat = SVFeatureGetByName(hRemoteDev, "TriggerSoftware", &hFeature);

	cout<<"state of setting AcquisitionMode: "<<stat<<endl;

// Acquisition start
	stat = SVStreamAcquisitionStart(hStream, SV_ACQ_START_FLAGS_DEFAULT, INFINITE);
	printf("state of StreamAcquisitionStart: %d\n", stat);

	// usleep: sleep in nanoseconds
	usleep(1000000);	



// !!!! Important: "SVFeatureCommandExecute"-OneExecution continues capturing image until all Buffer is full, and NONSTOP !!!!
	printf("------------------------\n");
	printf("Take the %d image...\n", i);
	stat = SVFeatureCommandExecute(hRemoteDev, hFeature, 10000);
	printf("state of FeatureCommandExecute: %d\n", stat);

	// StreamGetBufferId
	hBuffer = NULL;
	stat = SVStreamGetBufferId(hStream,0,&hBuffer);
	printf("StreamGetBufferId state: %d\n",stat);
	
	// StreamBufferGetInfo
	SV_BUFFER_INFO bufInfo;
	stat = SVStreamBufferGetInfo(hStream, hBuffer,&bufInfo);
	printf("---------- Print Buffer Info ----------\n");
	cout<<"iSizeX: "<<bufInfo.iSizeX<<"; iSizeY: "<<bufInfo.iSizeY<<endl;
	cout<<"iImageSize: "<<bufInfo.iImageSize<<endl;
	printf("bufferFlag: %d\n",bufInfo.flags.value);
	cout<<"iPixelType: "<<bufInfo.iPixelType<<endl;
	cout<<"iImageId: "<<bufInfo.iImageId<<endl;
	cout<<"iTimeStamp: "<<bufInfo.iTimeStamp<<endl;



// Try to save image
	string fileName = "./cap_";
	fileName.append(to_string(i));
	fileName.append(".png");
	stat = SVUtilSaveImageToPNGFile(bufInfo,fileName.c_str()); 
	printf("---------- Buffer Saving PNG ----------\n");
	cout<<"iSizeX: "<<bufInfo.iSizeX<<"; iSizeY: "<<bufInfo.iSizeY<<endl;
	cout<<"iImageSize: "<<bufInfo.iImageSize<<endl;
	printf("bufferFlag: %d\n",bufInfo.flags.value);
	cout<<"iPixelType: "<<bufInfo.iPixelType<<endl;
	cout<<"iImageId: "<<bufInfo.iImageId<<endl;
	cout<<"iTimeStamp: "<<bufInfo.iTimeStamp<<endl;

// Get PixelSize
	char pxSize[10];
	SVFeatureGetByName(hRemoteDev,"PixelSize",&hFeature);
	SVFeatureGetValueEnum(hRemoteDev, hFeature,pxSize,10);
	cout<<"PixelSize: "<<pxSize<<endl;

// Get PixelFormat
	char pxFormat[10];
	SVFeatureGetByName(hRemoteDev,"PixelFormat",&hFeature);
	SVFeatureGetValueEnum(hRemoteDev, hFeature,pxFormat,10);
	cout<<"PixelFormat: "<<pxFormat<<endl;

// Get PixelColorFilter
	char pxColoFilter[10];
	SVFeatureGetByName(hRemoteDev,"PixelColorFilter",&hFeature);
	SVFeatureGetValueEnum(hRemoteDev,hFeature,pxColoFilter,10);
	cout<<"PixelColorFilter: "<<pxColoFilter<<endl;


// Convert BAYXX8 or BAYXX12 Image format to 24 bit RGB
	stat = SVStreamBufferGetInfo(hStream, hBuffer, &bufInfo);

	int sizeFactor=1;
	int totalSize = sizeFactor*payloadSize*3;
	printf("Totalsize of SV_BUFFER_INFO: %d\n", totalSize);
	unsigned char *formatBuffer = new unsigned char[totalSize];

	stat = SVUtilBufferBayerToRGB(bufInfo,formatBuffer,totalSize);
	printf("state for BayerToRGB: %d\n", stat);
	ofstream bmfile("./bitmapfile.txt");
	for(int i=0; i<totalSize; i++){
		bmfile<<int(formatBuffer[i])<<";";
	}
	delete [] formatBuffer;


//Stop acquisition properly, by deleting existing buffers
	SVFeatureGetByName(hRemoteDev, "AcquisitionStop",&hFeature);
	SVFeatureCommandExecute(hRemoteDev, hFeature, 10000);
	stat = SVStreamAcquisitionStop(hStream, SV_ACQ_STOP_FLAGS_DEFAULT);
	printf("State of AcquisitionStop: %d\n",stat);
	SVStreamFlushQueue(hStream, SV_ACQ_QUEUE_INPUT_TO_OUTPUT);
	SVStreamFlushQueue(hStream, SV_ACQ_QUEUE_OUTPUT_DISCARD);


	uint8_t *pBuffer = NULL;
	SVStreamGetBufferId(hStream,0, &hBuffer);

	if(hBuffer)
	{
		SVStreamRevokeBuffer(hStream, hBuffer, (void**)&pBuffer, NULL);
		if(pBuffer)
			delete pBuffer;
	}


}
	return 0;

	
}
