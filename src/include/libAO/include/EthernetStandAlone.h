


#ifndef __ETHERNET_STAND_ALONE___
#define __ETHERNET_STAND_ALONE___

#include "AOTYPES.h"
#include <windows.h>

#define MAX_SMPLES_UD_CHANNEL 3200


typedef struct information{
	int32 channelID;
	cChar channelName[30];
}SInformation;


#ifdef _WIN32
	#ifdef EXPORTING_DLL___
		#define DECLDIR __declspec(dllexport)
	#else
		#define DECLDIR  __declspec(dllimport)
	#endif//
#else
	#define DECLDIR
#endif


#ifdef __cplusplus
extern "C"
{
#endif



	typedef struct MAC_ADDR {
		int  addr[6];
	} MAC_ADDR;


	typedef enum { eSin=1 } ESignalTypes;


	///used for the call back function
	typedef void (*AOParseFunction)(int16 *pData, int *dataSize);

	typedef void (*AODisplayError)(cChar* Pmessage, int dataSize);//this function will be used as call back function to get back Error messages


DECLDIR	int SetCallBackDisplayError(AODisplayError Calback);
	/*
		List of errors retun from the functions:
		0 -> no error
		1 -> the freq must be bigger than 20
		2 -> duration of the signal is longer than 70 msec and its must be shorter
		3 -> null parameter
		4 -> dll is not  connected
		5 ->Not enough memory					
		6 ->				
	*/

	/*
	this function will return the version of the dll	
	*/
DECLDIR	int GetDllVersion();

	/*
	StartConnection will start the connection between this dll and the embedded system
	core_macAdd::contain the dsp mac  address
	client_macAdd:: mac adress of network adapter on  computer running the dll
	AdapterIndex::the index of the network adapter (number from zero to total number of network adapters+1 easiestway 
					if the number is unknown set as -1 (which allow the driver to search for the driver index) 
	to find this number is to try )
	AOParseFunction ::pointer to function which will be called when new data receved from the embedded
	return integer : 0 if there is no errors
	*/
DECLDIR int   StartConnection(MAC_ADDR *core_macAdd,MAC_ADDR *client_macAdd,int AdapterIndex, AOParseFunction ) ;

	/*
	DefaultStartConnection will start the connection between this dll and the embedded system with 
	deafult pc mac address and adapter index
	*/
DECLDIR int   DefaultStartConnection(MAC_ADDR *core_macAdd , AOParseFunction parse);
	/*
	set the system id
	*/
DECLDIR  int SetSystemID(int sysytemId);
	/*
	TranslateNameToID function translate the name of the channel to his specific ID 
	*/
DECLDIR	int   TranslateNameToID(cChar* channelName, int nameLength, int* channelID);
	/*
	CheckQualityConnection check the quality of the connection between this dll and the embedded system 
	pQualityType: 1 for poor connection , 2 for medium connection and 3 for high connection
	pQualityPercent: the system throughtput in percent
	 return: 0 if there is no errors.
	 */
DECLDIR  int CheckQualityConnection(int *qualityType , real32 *pQualityPercent);

	 /*ErrorHandlingfunc function find out how muny error there is in the 
	 program and what is the latest error*/
DECLDIR int ErrorHandlingfunc(int *errorCount , cChar* errorString , int stringLength);

	/*
	SendDout sending digtal event to the digtal output
	mask: indicates which bits are to be changed,
	value: indicate what is the value to be written to these bits.
	return: 0 if there is no errors 
	*/
DECLDIR	int   SendDout(int mask,int value );
	/*
	SendDigitalData sending digtal event to the digtal output
	DigtalPortNumber: the ID of digtal output port
	mask: indicates which bits are to be changed,
	value: indicate what is the value to be written to these bits.
	return: 0 if there is no errors 
	*/
DECLDIR int SendDigitalData(int DigtalChannelNumber,int mask,int value );



	/*
	SetChannelSaveState function Check the checkbox in v if the statesave=1 is on or unchecked if the statesave=0 is off
	channelID: the ID of the digital port
	stateSave: 1 is on , 0 is off
	return integer: 0 if there is no errors
r
	*/
DECLDIR int SetChannelSaveState(int channelID , BOOL BState);
	 /*
	stop saving file in the host
	*/
DECLDIR	int StopSaveTS(UINT TS);


	/*
	start saving file in the host
	*/
DECLDIR	int StartSaveTS(UINT TS);

	/*
	stop saving file in the host
	*/
DECLDIR	int StopSave();


	/*
	start saving file in the host
	*/
DECLDIR	int StartSave();

	/*
	set the path which we will save the file to the path must exist
	path::contain the path to the directory which we want to save to
	size::the count of the chars in the path no more than 100 character
	*/
DECLDIR	int SetSavePath(cChar* path,int size);

	/*
	set the file name of the saving file
	fileName::contain the name of the file
	size::the count of the chars in the fileName no more than 40 character

	Note:if a file exist with same name it will be deleted
	*/

DECLDIR	int SetSaveFileName(cChar* fileName,int size);


	/*
	check if the driver is connected
	note ::after start connection we must wait until this command return true

	*/
DECLDIR	int isConnected();


	/*
	closeConnection function is closing the connection between this dll and the embedded system 
	return integer : 0 if there is no errors
	*/

DECLDIR	int CloseConnection();

	/*
	SetStimulationParameters fuction set the stimulation parameters for channel channelnumber
	
	FirstPhaseAmpl_mA: the amplitude of the first phase in mA
	FirstPhaseWidth_mS: the duration of the first phase in mSec
	SecondPhaseAmpl_mA: the amplitude of the Second phase in mA
	SecondPhaseWidth_mS: the duration of the Second phase in mSec
	Freq: the freq of the stimulstion pulse
	ReturnChannel: the id of the return channel - -1 is the global return
	ChannelID: the id of the channel to set the parameters for
	FirstPhaseDelay_mS: the delay of the first phase
	SecondPhaseDelay_mS: the delay of the second phase
	return integer : 0 if there is no errors
	*/
DECLDIR	int SetStimualtionParameters(real32 FirstPhaseAmpl_mA,real32 FirstPhaseWidth_mS, real32 SecondPhaseAmpl_mA,real32 SecondPhaseWidth_mS,int Freq_hZ ,\
							  real32 Duration_sec,int ReturnChannel,int channelnumber ,real32 FirstPhaseDelay_mS =0 , real32 SecondPhaseDelay_m =0);

	/*
	StartStimulation function start the stimulation at channel ChannelID
	ChannelID: the id of the channel
	return integer : 0 if there is no errors
	*/
DECLDIR	int StartStimulation(int ChannelID);


	/*
	StopStimulation function: stop the stimulation at channel ChannelID
	ChannelID: the id of the channel
	return integer : 0 if there is no errors
	*/
DECLDIR	int StopStimulation(int ChannelID);


	/*
	arraydata ::array of data with the size sizeOfArrayWords
	sizeOfArrayWords::size of the array
	realDataSizeWords::the amount of the data in the array

	this function can be used only if the call back function =0
	*/
 DECLDIR	void GetNextBlock(int16 * arraydata,int sizeOfArrayWords,int* realDataSizeWords)	;



	/*
	this function will send block of data to the embedded,this block can contain data or commands

	with this function ou can send any comand found in the stream format
	*/

 DECLDIR int SendBlock(void* streamBlock);




	/*
	AddBufferChannel function will add channel id to the list of channels which the dll gather data for them
	ChannelsId channel id ,e.g 10000 ,10001 ...
	BufferSizemSec : the size of the buffer in mSec
	return integer : 0 if there is no errors
	*/


DECLDIR int AddBufferChannel(int ChannelsId,int BufferSizemSec);



	/*
	GetChannelData function will get data for the specific channel id
	pData: array which will contain the data
	ArrSize: the siae of the pData
	DataCapture: the amount of data inserted to the array pData

	Note:before calling this function you must tell the driver to collect data for this specific channels by calling AddBufferChannel
	you get data from FIFO buffer so the first data entered will be the first data outputed 
	return integer : 0 if there is no errors
	*/
DECLDIR int GetChannelData(int ChannelsId,int16* pData,int ArrSizeWords,int *DataCapture);

	/*
	GetAlignedData function let the user to get data of chanels aligned ,so if you want to get data for more than one channel and you need 
	the data to start at the same time Stamp this is the function
	pArray: this array must be allocatd by the user and the function will insert the data into it
	ArraySize: the size of the array in words
	actualData: the actual data of the amount of data we inserted into the pArray
	arrChannel: contain the list of channel we want to collect data for
	sizearrChannels: the count of the channel 
	TS_Begin: the timestamp of the first sample 
	return integer : 0 if there is no errors

	the data in the array will be sorted like the channels ,e.g if the channel are 10000,10001,10002
	then the data will be ,data for channel 10000,data for channel 10001,data for channel 10002
	the amount of data for each channel Must be the same ==actualData/sizearrChannels

	Note:before calling this function you must tell the driver to collect data for this specific channels by calling AddBufferChannel
	*/
DECLDIR int GetAlignedData(int16* pArray,int ArraySize,int* actualData,int* arrChannel,int sizearrChannels,ULONG *TS_Begin);


	/*
	ClearBuffers function clear the buffers in the driver for collecting data
	*/

DECLDIR int ClearBuffers();

	/*
	GetLastTimeStamp function get the last time stamp
	pLastTS: the last time stamp
	return integer: 0 if there is no errors 
	*/
DECLDIR int GetLatestTimeStamp(ULONG *pLastTS);

	/*
	retrurn the version of the dll
	*/
//DECLDIR	 UINT GetDllVersion();


DECLDIR int RemoveDataType(cChar type,BOOL bRemove=TRUE);


	/*
	ChannelId::the channel id to set the feature on ,eg,10000 for lfp 1
	FeatureType::the feature type , 1 for level crossing
	value1:value used for the feature 
	value2:value used for the feature 
	EventNumber::the event numbe which will be send to port 11225 if the feature exsit

	*/
DECLDIR int  SetChannelFeature(int ChannelId,int FeatureType,int* params ,int ParamSize);

	/*
	ChannelId::the channel id to set the feature on ,eg,10000 for lfp 1
	DurationUsec::the detection period 
	StartDetection::1 will start detection 0 ,will stop detection
	*/
DECLDIR int StartDetectionFeature(int ChannelId,int DurationUsec,int StartDetection);



/*

this function return the drive position 
nDepth: the depth of the drive in uMeter
output: is an integer, 0 = no function errors 
*/
DECLDIR int GetDriveDepth(int* nDepth);

/*
this function move the drive for number of steps (in mm)
stepmm: the amount of steps in mm that we want to move the drive 
output: is an integer, 0 = no function errors 
*/
DECLDIR int MoveDrive(float  stepmm);


/*
set the ThreshHold (level line )of a channel
channelID::contain the chanel id can be only SPIKE LFP OR segmented of the same electrode
ThreshHoldValue_uVolt the value of the level line in microVolts
Direction of the detection 1->down 0->up
output: is an integer, 0 = no function errors 
*/
DECLDIR int SetThreshHold(int channelID,int ThreshHoldValue_uVolt,int Direction);




/*
set the SetSsamplingRate (level line )of a channel
channelID::contain the chanel id can be only SPIKE LFP OR segmented of the same electrode
DownFactor: the sampling rate down factor ,alowed values {1,2,4,8,16}
output: is an integer, 0 = no function errors
*/
DECLDIR int SetSsamplingRate(int channelID,int DownFactor);



/*
send text to the mpx file with the current timestamp
text:text array of chars to be sent
length:the length of the text ,max 100
output: is an integer, 0 = no function errors
*/

DECLDIR int SendText(char *text ,int length);


/*
this function will return the cutoff frequency of the low and high pass filter 
channelID ::channel ID
dFCLP ::will contain the cutoff freq of the low pass filter
dFCHP ::will contain the cutoff freq of the high pass filter
output: is an integer, 0 = no function errors
*/
DECLDIR int GetCutOffFC(int channelID,real32 *dFCLP,real32 *dFCHP);


 /*
 this function get the save status of the specified channel 
 channelID: channel Id
 pSaveState: true or false 
 output: is an integer, 0 = no function errors
 */
DECLDIR int GetSaveStatues(int channelID,BOOL *pSaveState);

/*
this fuction set a new name for the specified channel
channelID: channel Id
newName: array of chars that include the new name for the specified channel
output: is an integer, 0 = no function errors
*/
DECLDIR int SetChannelName(int channelID , cChar* newName,int NameLength);
	
/*
This function will return the last time stop when the motor stopped moving
pLastStopTS: the last time stamp when the moter stopped moving
*/
DECLDIR int GetStopMotorTS(uint32* pLastStopTS);



/*
this function get the count of channels
pChannelCount: the cahnnels count
*/
DECLDIR int GetChannelsCount(uint32* pChannelCount);

/*this function return the type of stim for the specified channel: 0 no stimulation
																   1 one source
																   2 full sources*/
DECLDIR int GetStimType(int32 channelID , int32 *stimType);

DECLDIR int GetAllChannels( SInformation *pAllChannels,int32 ChnanelCount);


/*
pSource in micro Ampire
each 1 sample is 1*downSample/44000 Sec 
downSampleFactor must be 2^N : 1 , 2 , 4 , 8 , 16 
SamplesCount < = 8M
*/
DECLDIR int LoadWaveToEmbedded(short* pSource,int SamplesCount,int downSampleFactor,char *WaveName);



DECLDIR int StartAnalogStimulation(int StimChanel,int waveId,int Freq_hZ ,	 real32 Duration_sec,int ReturnChannel );


 DECLDIR int StartDigitalStimulation(int StimChanel,real32 FirstPhaseDelay_mS,real32 FirstPhaseAmpl_mA,real32 FirstPhaseWidth_mS, real32 SecondPhaseDelay_mS,\
	 real32 SecondPhaseAmpl_mA,real32 SecondPhaseWidth_mS,int Freq_hZ ,	 real32 Duration_sec,int ReturnChannel );


#ifdef __cplusplus
}
#endif

#endif


