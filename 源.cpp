


#include <iostream>

#include <WinSock2.h>
#include<Windows.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

int main()
{

	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData = { 0 };
	int iResult = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}
	else
	{
		cout << "WSAStartup success" << endl;
	}


	//创建一个套接字，并检测是否创建成功
	int sockSer = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockSer == -1)
		perror("socket");

	struct sockaddr_in addrSer;  //创建一个记录地址信息的结构体 
	addrSer.sin_family = AF_INET;    //使用AF_INET协议族 
	addrSer.sin_port = htons(888);     //设置地址结构体中的端口号
	addrSer.sin_addr.s_addr = inet_addr("192.168.2.64");  //设置通信ip

														   //将套接字地址与所创建的套接字号联系起来，并检测是否绑定成功
	int addrlen = sizeof(struct sockaddr);
	int res = bind(sockSer, (struct sockaddr*)&addrSer, addrlen);
	if (res == -1)
		perror("bind");

	struct sockaddr_in addrCli;




	WAVEFORMATEX wfx = { 0 };
	wfx.wFormatTag = WAVE_FORMAT_PCM; //设置波形声音的格式
	wfx.nChannels = 2;            //设置音频文件的通道数量
	wfx.nSamplesPerSec = 48000; //设置每个声道播放和记录时的样本频率
	wfx.wBitsPerSample = 16;    //每隔采样点所占的大小
	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	//HANDLE wait = CreateEvent(NULL, 0, 0, NULL);
	HWAVEOUT hwo;
	//MMSYSERR_NOERROR
	int ret = waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0L, CALLBACK_NULL); //打开一个给定的波形音频输出装置来进行回放
	if (ret == MMSYSERR_NOERROR)
	{
		cout << "device open success" << endl;
	}


#define AUDIO_BUFLEN 1000000

	char * buf = new char[AUDIO_BUFLEN];
	
	int buf_index = 0;
	WAVEHDR wh[1000];

	FILE*fp = fopen("123.pcm", "wb");

	for(int i = 0;;)
	{
		//cout << "recv" << endl;
		//fwrite(recvbuf, bufrecved, 1, fp);
		int bufrecved = recvfrom(sockSer, buf + buf_index, 100000, 0, (struct  sockaddr*)&addrCli, &addrlen);     //从指定地址接收客户端数据
		printf("recv %d", bufrecved);
		
		wh[i].lpData = buf+buf_index;
		wh[i].dwBufferLength = bufrecved;
		wh[i].dwFlags = 0L;
		wh[i].dwLoops = 0L;

		buf_index += bufrecved;
		if (buf_index > (AUDIO_BUFLEN-100000) )
		{
			buf_index = 0;
		}


		int ret;
		ret = waveOutPrepareHeader(hwo, wh + i, sizeof(WAVEHDR)); //准备一个波形数据块用于播放
		if (MMSYSERR_NOERROR != ret)
			printf("waveOutPrepareHeader %d", ret);
		ret = waveOutWrite(hwo, wh + i, sizeof(WAVEHDR)); //在音频媒体中播放第二个函数wh指定的数据
		if (MMSYSERR_NOERROR != ret)
			printf("waveOutWrite %d", ret);


		i++;
		i %= 1000;
		
		


		//printf("Cli:>%s\n", recvbuf);
	}





	WSACleanup();

	return 0;
}
