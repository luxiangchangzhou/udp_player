


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


	//����һ���׽��֣�������Ƿ񴴽��ɹ�
	int sockSer = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockSer == -1)
		perror("socket");

	struct sockaddr_in addrSer;  //����һ����¼��ַ��Ϣ�Ľṹ�� 
	addrSer.sin_family = AF_INET;    //ʹ��AF_INETЭ���� 
	addrSer.sin_port = htons(888);     //���õ�ַ�ṹ���еĶ˿ں�
	addrSer.sin_addr.s_addr = inet_addr("192.168.2.64");  //����ͨ��ip

														   //���׽��ֵ�ַ�����������׽��ֺ���ϵ������������Ƿ�󶨳ɹ�
	int addrlen = sizeof(struct sockaddr);
	int res = bind(sockSer, (struct sockaddr*)&addrSer, addrlen);
	if (res == -1)
		perror("bind");

	struct sockaddr_in addrCli;




	WAVEFORMATEX wfx = { 0 };
	wfx.wFormatTag = WAVE_FORMAT_PCM; //���ò��������ĸ�ʽ
	wfx.nChannels = 2;            //������Ƶ�ļ���ͨ������
	wfx.nSamplesPerSec = 48000; //����ÿ���������źͼ�¼ʱ������Ƶ��
	wfx.wBitsPerSample = 16;    //ÿ����������ռ�Ĵ�С
	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	//HANDLE wait = CreateEvent(NULL, 0, 0, NULL);
	HWAVEOUT hwo;
	//MMSYSERR_NOERROR
	int ret = waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0L, CALLBACK_NULL); //��һ�������Ĳ�����Ƶ���װ�������лط�
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
		int bufrecved = recvfrom(sockSer, buf + buf_index, 100000, 0, (struct  sockaddr*)&addrCli, &addrlen);     //��ָ����ַ���տͻ�������
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
		ret = waveOutPrepareHeader(hwo, wh + i, sizeof(WAVEHDR)); //׼��һ���������ݿ����ڲ���
		if (MMSYSERR_NOERROR != ret)
			printf("waveOutPrepareHeader %d", ret);
		ret = waveOutWrite(hwo, wh + i, sizeof(WAVEHDR)); //����Ƶý���в��ŵڶ�������whָ��������
		if (MMSYSERR_NOERROR != ret)
			printf("waveOutWrite %d", ret);


		i++;
		i %= 1000;
		
		


		//printf("Cli:>%s\n", recvbuf);
	}





	WSACleanup();

	return 0;
}
