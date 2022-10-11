#include <iostream>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_queue.h"
#include "msg_type.h"

using namespace std;

// Logger(RECV) <- Web UI(SND)
int camera_cfg_recv(int &WIDTH, int &HEIGHT, int &FPS)
{
	int camera_cfg_msgid;
	camera_cfg_msg_data data;
	int fps = FPS;
	unsigned char size, format;
	int width = WIDTH, height = HEIGHT;

	if (-1 == (camera_cfg_msgid = msgget((key_t)CAMERA_CFG_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}

	// datatype 0: receive all datatype
	// change datatype to 1 if you want to receive python data
	if (-1 == msgrcv(camera_cfg_msgid, &data, sizeof(camera_cfg_msg_data) - sizeof(long), 0, IPC_NOWAIT))
	{
		cout << "    No Camera Configuaration value received." << endl;
	}
	else
	{
		if (data.data_type == TYPE_CAMERA_CFG)
		{
			cout << "    Camera configuration received." << endl;
			memcpy(&size, data.data_buff, sizeof(unsigned char));
			memcpy(&format, data.data_buff + sizeof(unsigned char), sizeof(unsigned char));
			memcpy(&fps, data.data_buff + sizeof(unsigned char) * 2, sizeof(int));
		}

		// SET FPS
		FPS = fps;

		// SET SIZE
		switch (size)
		{
		case VGA:
			WIDTH = VGA_WIDTH;
			HEIGHT = VGA_HEIGHT;
			break;
		case HD:
			WIDTH = HD_WIDTH;
			HEIGHT = HD_HEIGHT;
			break;
		case CIF:
			WIDTH = CIF_WIDTH;
			HEIGHT = CIF_HEIGHT;
			break;
		}

		camera_cfg_res_send();
	}
	cout << "    width: " << WIDTH << "| height: " << HEIGHT << "| fps: " << FPS << endl;

	return 0;
}

// Logger(RES) -> WEB UI(RECV)
int camera_cfg_res_send()
{
	int camera_cfg_res_msgid;
	camera_cfg_res_msg_data data;
	unsigned char camera_cfg_res = CAMERA_CFG_RES;

	if (-1 == (camera_cfg_res_msgid = msgget((key_t)CAMERA_CFG_RES_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}

	// by Header
	data.data_type = TYPE_CAMERA_CFG_RES;
	memcpy(data.data_buff, &camera_cfg_res, sizeof(unsigned char));

	if (-1 == msgsnd(camera_cfg_res_msgid, &data, sizeof(camera_cfg_res_msg_data) - sizeof(long), 0))
	{
		perror("msgsnd() failed");
		exit(1);
	}

	else
	{
		cout << "    Camera_cfg_res sent." << endl;
	}
	return 0;
}

// Logger(RECV) <- Web UI(REQ)
int Image_Hash_request(string HASH){
	
	string hash = HASH;
	int image_hash_req_msqid;
	Image_hash_req_msg_data data;
	unsigned char image_hash_req = IMAGE_HASH_REQ;

	if( -1 == (image_hash_req_msqid == msgget((key_t)IMAGE_HASH_REQ_MQ, IPC_CREAT | 0666)))	
	{
		cout << "msgget failed" << endl;
		exit(0);
	}

	if(-1 == msgrcv(image_hash_req_msqid, &data, sizeof(Image_hash_req_msg_data) - sizeof(long), 0, IPC_NOWAIT)) 
	{
		cout << "    No request from Web UI. " << endl;
	}	
	else {
		if(data.data_type == TYPE_IMAGE_HASH_REQ) {
			// Image_HASH_send(hash);
			cout << "IMAGE_HASH sent " << endl; 
		}
	}
	return 0;
}

// Logger(SND) -> Web UI(RECV)
int Image_HASH_send(string HASH)
{
	string Img_HASH = HASH;
	int image_hash_send_msqid;
	Image_hash_msg_data data;

	if (-1 == (image_hash_send_msqid = msgget((key_t)IMAGE_HASH_MQ, IPC_CREAT | 0666)))
	{
		cout << "msgget failed" << endl;
		exit(0);
	}

	data.data_type = TYPE_IMAGE_HASH;
	strcpy(data.data_buff, Img_HASH.c_str());

	if ( -1 == msgsnd( image_hash_send_msqid, &data, sizeof( Image_hash_msg_data) - sizeof( long), 0))
	{
		perror( "msgsnd() failed");
		exit( 1);
	}
	else
	{
		cout << "Data: " << data.data_buff << endl;
		cout << "PATH and HASH sent." << endl;
	}
	return 0;
}

// Logger(RECV) <- Web UI(RES)
int Image_Hash_response()
{
	int image_hash_recv_msgid;
	Image_hash_recv_msg_data data;
	unsigned char recv;

	if (-1 == (image_hash_recv_msgid = msgget((key_t)IMAGE_HASH_RES_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}

	if (-1 == msgrcv(image_hash_recv_msgid, &data, sizeof(Image_hash_recv_msg_data) - sizeof(long), 0, IPC_NOWAIT))
	{
		cout << "Web UI didn't received path and hash." << endl;
	}
	else
	{
		if (data.data_type == TYPE_IMAGE_HASH_RES)
		{
			cout << "WebUI received Image PATH and HASH." << endl;
			memcpy(&recv, data.data_buff, sizeof(unsigned char));
		}
	}
	return (int)recv;
}

// Verifier(RECV) <- Server(SND)
int Server2Verifier_CID_send(string &CID)
{
	int cid_msgid;
	CID_msg_data data;
	if (-1 == (cid_msgid = msgget((key_t)CID_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	data.data_type = TYPE_CID;
	memcpy(&data.data_buff, CID.c_str(), CID.size());
	cout << data.data_buff << endl;
	if (-1 == msgsnd(cid_msgid, &data, sizeof(CID_msg_data) - sizeof(long), 0))
	{
		perror("msgsnd() failed");
		exit(1);
	}
	else
	{
		cout << "Data: " << data.data_buff << endl;
		cout << "CID sent. " << endl;
	}
	return 0;
}

// Verifier(RECV) <- Server(SND)
int Server2Verifier_CID_recv(string &CID)
{
	int cid_msgid;
	CID_msg_data data;
	char sCID[CID_BUFF_SIZE];
	if (-1 == (cid_msgid = msgget((key_t)CID_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	if (-1 == msgrcv(cid_msgid, &data, sizeof(CID_msg_data) - sizeof(long), 0, 0))
	{
		cout << "No CID from Server." << endl;
	}
	else
	{
		cout << "Server sent CID to Verifier" << endl;
		if (data.data_type == TYPE_CID)
		{
			memcpy(&sCID, data.data_buff, sizeof(char) * CID_BUFF_SIZE);
		}
	}
	CID = sCID;
	cout << CID << endl;
}

// Verfier(RES) -> Server(RECV)
int Verifier2Server_CID_res_send()
{
	int cid_recv_msgid;
	CID_recv_msg_data data;
	unsigned char recv = 0x01;
	if (-1 == (cid_recv_msgid = msgget((key_t)CID_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	data.data_type = TYPE_CID_RECV;
	memcpy(&data.data_buff, &recv, sizeof(unsigned char));
	if (-1 == msgsnd(cid_recv_msgid, &data, sizeof(CID_recv_msg_data) - sizeof(long), 0))
	{
		perror("msgsnd() failed");
		exit(1);
	}
	else
	{
		cout << "received sent." << endl;
	}
}

// Verifier(RES) -> Server(RECV)
int Verifier2Server_CID_res_recv()
{
	int cid_recv_msgid;
	CID_recv_msg_data data;
	unsigned char recv;
	if (-1 == (cid_recv_msgid = msgget((key_t)CID_RECV_MQ, IPC_CREAT | 0666)))
	{
		perror("msgget() failed");
		exit(1);
	}
	data.data_type = TYPE_CID_RECV;
	if (-1 == msgrcv(cid_recv_msgid, &data, sizeof(CID_recv_msg_data) - sizeof(long), 0, 0))
	{
		cout << "Verifier didn't received CID." << endl;
	}
	else
	{
		if (data.data_type == TYPE_CID_RECV)
		{
			cout << "Verifier received CID." << endl;
			memcpy(&recv, data.data_buff, sizeof(unsigned char));
		}
	}
	return (int)recv;
}
