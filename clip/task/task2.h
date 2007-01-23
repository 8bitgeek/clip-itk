/*
   Copyright (C) 2007  ITK
   Author   : Uri <uri@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html
 */
/*
 $Log$
 Revision 1.1  2007/01/23 10:46:23  itk
 uri: some redisign for new task model

 */


#define TASK_STACK_MIN        16384
#define NEW(typ) ((typ*) calloc(sizeof(typ),1))

#include "../list.h"
#include "../coll.h"
#include "../hash.h"

struct TaskMessage
{
	long sender;
	long receiver;
	long id;
	int needResp:1;
	int needDel:1;

	void *data;
	void (*destroy) (void *data);
};

typedef enum
{
	Task_initial,		/*  �������, �� �� ���������� */
	Task_ready,		/*  ������ � ���������� */
	Task_wait,		/*  ���� ���������� ����� ��� ������� */
	Task_msg,		/*  ���� ��������� */
	Task_resp,		/*  ���� ������ */
	Task_zombie		/*  ��� ������, �� ��� �� ���������� */
}
TaskState;

struct Task
{
	ListEl listel;

	List recvlist;		/*  ������� ������� ��������� */
	List proclist;		/*  �������������� ��������� */

	int isMain:1;
	int isInited:1;		/*  ���������������� � ������ � ���������� */
	int isTimed:1;		/*  ���� ������� */
	int isRead:1;		/*  ���� ������ �� ����� */
	int isWrite:1;		/*  ���� ������ � ���� */
	int isExcept:1;		/*  ���� sigexcept */

	char *name;		/*  ��� ������ */
	long id;		/*  ������������� ������ (pid) */
	long wakeUp;		/*  ���� ������� ������ */

	Task *parent;		/*  �������� ������, ������������� ����� spawn */
	int result;		/*  ��������� ���������� run() */

	TaskState state;	/*  ��������� ������ */

	long stacklen;		/*  length of task's stack */

	jmp_buf taskEnv;	/* environment for context switching */
	char *stack;		/* this task's stack space */
	fd_set rfileset;	/* read-wait fileset */
	fd_set wfileset;	/* write-wait fileset */
	fd_set efileset;	/* except fileset */

	/*
	   #note russian
	   # ��������� ����� ������. ���� ����� ������ ���� �������������
	   # � ����������� ������ ��� ���������� �������� ������.
	   # ����� �� ������ run �������� ��������� ������.
	 */
	
	int (*run) (void *data);
	void (*destroy) (void *data);
	void *data;
	
};


static int seqNo = 0;		/*  ���������-����������� ����� */
