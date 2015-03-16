#pragma once

#include "type.h"
#include "xthread.h"
#include "xassert.h"
#include "watch.h"

#ifndef WIN32
#	include <unistd.h>
#	include <signal.h>
#	include <sys/param.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <sys/wait.h>
#	include <fcntl.h>
#	include <pthread.h>
#	include <sys/mman.h>
#endif

#include <iostream>

namespace global
{
	extern xrecursive_mutex exit_mtx;
	extern condition_variable_recursive_mutex exit_condition;
	extern bool* global_running_flag;

	//	���� true ˵�� guardian �Ѿ�������false ˵�� guardian ����ʧ�ܣ�guardian �Լ�������
	inline bool guardian(int child_cnt = 1)	
	{
#ifdef WIN32
		return true;
#else
		XASSERT(child_cnt > 0);
		int now_child_cnt = 0;
		int times_left = 10;
		global_running_flag = (bool*)mmap(NULL, sizeof(bool), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
		if (global_running_flag == NULL)
		{
			return false;
		}
		*global_running_flag = true;
		while (*global_running_flag)
		{
			if (now_child_cnt < child_cnt)
			{
				switch (fork()) 
				{
				case -1:
					return false;
				case 0:	
					//	child process				
					return true;
				default:
					//	guardian process
					++ now_child_cnt;								
				}
			}
			else
			{				
				int status;
				if (wait(&status) == -1)
				{
					return false;
				}
				else
				{
					if (--times_left == 0)
					{
						return false;
					}
					-- now_child_cnt;
				}
			}
		}
		if (*global_running_flag == false)
		{
			exit(0);
		}

		return true;
#endif
	}

#ifndef WIN32
	void terminate_handler(int signum);
#endif

	inline bool daemonize()
	try
#ifdef WIN32
	{
		return true;
	}
#else
	{
		//����һЩ�йؿ����ն˲������ź�
		signal(SIGTTOU, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGHUP , SIG_IGN);
		signal(SIGPIPE, SIG_IGN);
		signal(SIGTERM, &terminate_handler);

		//����һ���µĽ���
		int pid = fork();
		if(pid != 0)		//����������
		{
			exit(0);
		}
		else if(pid < 0)	//�����ӽ���ʧ��
		{
			//��¼��־��
			//ERROR:fork a child process failed!
			exit(1);
		}

		//����������Ľ��̳�Ϊ�Ự�鳤�ͽ����鳤
		if(setsid() < 0)
		{
			exit(1);
		}

		pid = fork();
		if(pid != 0)		//�����ý���
		{
			exit(0);
		}
		else if(pid < 0)	//�����ӽ���ʧ��
		{
			//��¼��־��
			//ERROR:fork a child process failed!
			exit(1);
		}

		for (int fd = 0, fd_num = 3; fd < fd_num; ++ fd)	//�رմ򿪵��ļ�������
		{
			close(fd);
		}

		open("/dev/null", O_RDWR);
		open("/dev/null", O_RDWR);
		open("/dev/null", O_RDWR);

		//�����ļ�������ģ
		//	umask(0);
		return true;
	}
#endif
	catch (...)
	{
		LWarning("fail to daemonize");
		return false;
	}


	void terminate();

}
