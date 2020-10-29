#include "data.h"
#include "list.h"
#include <stdio.h>
#include <string.h>

/***********逻辑模块************/
#pragma warn - pia
list_t *GetUserList()
{
	// list_t *pUserlist = ReadUserFile(USERINFOFILE);
	// return pUserlist;
	return NULL;
}

int UserInfomatch(void *a, void *b)
{
	userInfo *pa, *pb;
	pa = (userInfo *)a;
	pb = (userInfo *)b;
	return (pa->userID == pb->userID);
}

int PatientInfomatch(void *a, void *b)
{
	PatientInfo *pa, *pb;
	pa = (PatientInfo *)a;
	pb = (PatientInfo *)b;
	return (pa->id == pb->id);
}

int DeptInfomatch(void *a, void *b)
{
	DeptInfo *pa, *pb;
	pa = (DeptInfo *)a;
	pb = (DeptInfo *)b;
	return (pa->id == pb->id);
}

int DoctorInfomatch(void *a, void *b)
{
	DoctorInfo *pa, *pb;
	pa = (DoctorInfo *)a;
	pb = (DoctorInfo *)b;
	return (pa->id == pb->id);
}

void Nonemallocfree(void *val)
{
	free(val);
}

/**
 * 对字符串中的字符进行替换
 * 使用 #代替回车 或 回车替换#
 **/
void strrpl(char *str, char oldch, char newch)
{
	char *s = str;
	while (*s)
	{
		if (*s == oldch)
			*s = newch;
		s++;
	}
}

/***********数据模块************/
//---------------User-----------------//
/// <summary>
/// 从文件中读出用户信息并将信息以链表形式存入内存，返回表头
/// </summary>
/// <param name="filename">文件名</param>
/// <returns>用户信息链表表头指针</returns>
list_t *ReadUserInfo(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[32]; //line的长度一定要够长，大于userinfo能存的最大字符长度
	userInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 32, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(userInfo));
		if (NULL == list)
		{
			list = list_new();
			list->match = UserInfomatch;
			list->free = Nonemallocfree;
		}
		//用sscanf将line中的各数据读入对应的变量中
		sscanf(line, "%d\t%s\t%s\t%d", &(Infotemp->userID), Infotemp->username, Infotemp->password, &(Infotemp->userType));
		list_rpush(list, list_node_new(Infotemp));
	}

	fflush(fp);
	fclose(fp);
	return list;
}
/// <summary>
/// 将链表中存储的用户信息写入文件中
/// </summary>
/// <param name="userinfo">用户信息链表</param>
/// <param name="filename">文件名</param>
void SaveUserInfo(char *filename, list_t *userinfo)
{
	FILE *fp;
	userInfo *Infotemp;
	list_node_t *node;

	list_iterator_t *it = list_iterator_new(userinfo, LIST_HEAD); //申请迭代器

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (userInfo *)node->val; //将空指针转换为userInfo以在之后的fprintf中利用->
		fprintf(fp, "%d\t%s\t%s\t%d\n", Infotemp->userID, Infotemp->username, Infotemp->password, Infotemp->userType);
		//用fprintf向文件输出
	}

	list_iterator_destroy(it);
	fflush(fp);
	fclose(fp);
}
/// <summary>
/// 通过用户唯一编码找到用户信息链表中对应的用户
/// </summary>
/// <param name="userinfo">用户信息链表</param>
/// <param name="id">用户唯一编码</param>
/// <returns>指向找到的用户的指针</returns>
userInfo *FindUserInfo(list_t *userinfo, int id)
{
	list_iterator_t *it = list_iterator_new(userinfo, LIST_HEAD);
	list_node_t *node;
	userInfo *Infotemp = NULL;

	while ((node = list_iterator_next(it))) //迭代
	{
		Infotemp = (userInfo *)node->val;
		if (Infotemp->userID == id) //比对到后跳出循环
			break;
		Infotemp = NULL; //为比对到，则将指针归零
	}

	list_iterator_destroy(it);
	return Infotemp;
}

userInfo *fFindUserInfo(const char *filename, char *username, char *password)
{
	FILE *fp;
	char line[32]; //line的长度一定要够长，大于userinfo能存的最大字符长度
	userInfo *Infotemp;

	if (username == NULL || password == NULL)
		return NULL;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}
	Infotemp = malloc(sizeof(userInfo));
	while (fgets(line, 32, fp))
	{ //用sscanf将line中的各数据读入对应的变量中
		if (line[0] == '#')
			continue;
		sscanf(line, "%d\t%s\t%s\t%d", &(Infotemp->userID), Infotemp->username, Infotemp->password, &(Infotemp->userType));
		if (strcmpi(username, Infotemp->username) == 0 && 0 == strcmpi(password, Infotemp->password))
		{ //忽略大小写
			fclose(fp);
			return Infotemp;
		}
	}
	free(Infotemp);
	fclose(fp);
	return NULL;
}

//---------------Patient-----------------//
list_t *ReadPatientInfo(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[130];
	PatientInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 130, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(PatientInfo));
		if (NULL == list)
		{
			list = list_new();
			list->match = PatientInfomatch;
			list->free = Nonemallocfree;
		}
		sscanf(line, "%d\t%s\t%c\t%c\t%s\t%d\t%d\t%s\t%s\t%s\t%ld", &(Infotemp->id), Infotemp->name, &(Infotemp->sex),
					 &(Infotemp->marriagestatus), Infotemp->id_number,
					 &(Infotemp->year), &(Infotemp->month), Infotemp->telenum,
					 Infotemp->addr, Infotemp->allergy, &(Infotemp->money));
		list_rpush(list, list_node_new(Infotemp));
	}

	fclose(fp);
	return list;
}

void SavePatientInfo(char *filename, list_t *patientinfo)
{
	FILE *fp;
	PatientInfo *Infotemp;
	list_node_t *node;

	list_iterator_t *it = list_iterator_new(patientinfo, LIST_HEAD);

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (PatientInfo *)node->val;
		fprintf(fp, "%d\t%s\t%c\t%c\t%s\t%d\t%d\t%s\t%s\t%s\t%ld\n", Infotemp->id, Infotemp->name, Infotemp->sex,
						Infotemp->marriagestatus, Infotemp->id_number,
						Infotemp->year, Infotemp->month, Infotemp->telenum,
						Infotemp->addr, Infotemp->allergy, Infotemp->money);
	}

	list_iterator_destroy(it);
	fflush(fp);
	fclose(fp);
}

PatientInfo *FindPatientInfo(list_t *patientinfo, int id)
{
	list_iterator_t *it = list_iterator_new(patientinfo, LIST_HEAD);
	list_node_t *node;
	PatientInfo *Infotemp = NULL;

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (PatientInfo *)node->val;
		if (Infotemp->id == id)
			break;
		Infotemp = NULL;
	}

	list_iterator_destroy(it);
	return Infotemp;
}
PatientInfo *fFindPatientInfo(const char *filename, int id)
{
	FILE *fp;
	char line[130];
	PatientInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	Infotemp = malloc(sizeof(PatientInfo));
	while (fgets(line, 130, fp))
	{
		if (line[0] == '#')
			continue;

		sscanf(line, "%d\t%s\t%c\t%c\t%s\t%d\t%d\t%s\t%s\t%s\t%ld", &(Infotemp->id), Infotemp->name, &(Infotemp->sex),
					 &(Infotemp->marriagestatus), Infotemp->id_number,
					 &(Infotemp->year), &(Infotemp->month), Infotemp->telenum,
					 Infotemp->addr, Infotemp->allergy, &(Infotemp->money));
		if (id == Infotemp->id)
		{
			fclose(fp);
			return Infotemp;
		}
	}

	fclose(fp);
	free(Infotemp);
	return NULL;
}

//---------------Department-----------------//
list_t *ReadDeptInfo(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[90];
	DeptInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 90, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(DeptInfo));
		if (NULL == list)
		{
			list = list_new();
			list->match = DeptInfomatch;
			list->free = Nonemallocfree;
		}
		sscanf(line, "%d\t%s\t%s", &(Infotemp->id), Infotemp->deptname, Infotemp->deptintro);
		list_rpush(list, list_node_new(Infotemp));
	}

	fclose(fp);
	return list;
}

void SaveDeptInfo(list_t *deptinfo, char *filename)
{
	FILE *fp;
	DeptInfo *Infotemp;
	list_node_t *node;

	list_iterator_t *it = list_iterator_new(deptinfo, LIST_HEAD);

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (DeptInfo *)node->val;
		fprintf(fp, "%d\t%s\t%s\n", Infotemp->id, Infotemp->deptname, Infotemp->deptintro);
	}

	list_iterator_destroy(it);
}

DeptInfo *FindDeptInfo(list_t *deptinfo, int id)
{
	list_iterator_t *it = list_iterator_new(deptinfo, LIST_HEAD);
	list_node_t *node;
	DeptInfo *Infotemp = NULL;

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (DeptInfo *)node->val;
		if (Infotemp->id == id)
			break;
		Infotemp = NULL;
	}

	list_iterator_destroy(it);
	return Infotemp;
}

DeptInfo *fFindDeptInfo(const char *filename, int id)
{
	FILE *fp;
	char line[90];
	DeptInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	Infotemp = malloc(sizeof(DeptInfo));
	while (fgets(line, 90, fp))
	{
		if (line[0] == '#')
			continue;

		sscanf(line, "%d\t%s\t%s", &(Infotemp->id), Infotemp->deptname, Infotemp->deptintro);

		if (id == Infotemp->id)
		{
			fclose(fp);
			return Infotemp;
		}
	}

	free(Infotemp);
	fclose(fp);
	return NULL;
}

//---------------Doctor-----------------//
list_t *ReadDoctorInfo(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[85];
	DoctorInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 85, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(DoctorInfo));
		if (NULL == list)
		{
			list = list_new();
			//list->match = DoctorInfomatch;
			list->free = Nonemallocfree;
		}
		sscanf(line, "%d\t%s\t%c\t%d\t%s\t%c", &(Infotemp->id), Infotemp->name, &(Infotemp->sex),
					 &(Infotemp->deptid), Infotemp->intro, &(Infotemp->title));
		list_rpush(list, list_node_new(Infotemp));
	}

	fclose(fp);
	return list;
}

void SaveDoctorInfo(char *filename, list_t *doctorinfo)
{
	FILE *fp;
	DoctorInfo *Infotemp;
	list_node_t *node;

	list_iterator_t *it = list_iterator_new(doctorinfo, LIST_HEAD);

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (DoctorInfo *)node->val;
		fprintf(fp, "%d\t%s\t%c\t%d\t%s\t%c\n", Infotemp->id, Infotemp->name, Infotemp->sex,
						Infotemp->deptid, Infotemp->intro, Infotemp->title);
	}

	list_iterator_destroy(it);
}

DoctorInfo *FindDoctorInfo(list_t *doctorinfo, int id)
{
	list_iterator_t *it = list_iterator_new(doctorinfo, LIST_HEAD);
	list_node_t *node;
	DoctorInfo *Infotemp = NULL;

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (DoctorInfo *)node->val;
		if (Infotemp->id == id)
			break;
		Infotemp = NULL;
	}

	list_iterator_destroy(it);
	return Infotemp;
}
DoctorInfo *fFindDoctorInfo(const char *filename, int docid)
{
	FILE *fp;
	char line[256];
	DoctorInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	Infotemp = malloc(sizeof(DoctorInfo));
	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		sscanf(line, "%d\t%s\t%c\t%d\t%s\t%c", &(Infotemp->id), Infotemp->name, &(Infotemp->sex),
					 &(Infotemp->deptid), Infotemp->intro, &(Infotemp->title));

		if (Infotemp->id == docid)
		{
			fclose(fp);
			return Infotemp;
		}
	}

	fclose(fp);
	free(Infotemp);
	TRACE(("not find\n"));
	return NULL;
}

//----------------物流信息----------------------------//

list_t *ReadPostInfo(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[256];
	postInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(postInfo));
		if (NULL == list)
		{
			list = list_new();
			//list->match = DoctorInfomatch;
			list->free = Nonemallocfree;
		}
		sscanf(line, "%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
					 &Infotemp->userid, &Infotemp->postid,
					 Infotemp->shipper, Infotemp->shipperaddr,
					 Infotemp->shiptime,
					 Infotemp->receiver,
					 Infotemp->receiveraddr,
					 Infotemp->tel,
					 Infotemp->status);
		list_rpush(list, list_node_new(Infotemp));
	}

	fclose(fp);
	return list;
}

postInfo *fFindPostInfo(const char *filename, int postid)
{
	FILE *fp;
	char line[256];
	postInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	Infotemp = malloc(sizeof(postInfo));
	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;
		sscanf(line, "%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
					 &Infotemp->userid, &Infotemp->postid,
					 Infotemp->shipper, Infotemp->shipperaddr,
					 Infotemp->shiptime,
					 Infotemp->receiver,
					 Infotemp->receiveraddr,
					 Infotemp->tel,
					 Infotemp->status);
		if (postid == Infotemp->postid)
		{
			fclose(fp);
			return Infotemp;
		}
	}

	free(Infotemp);
	fclose(fp);
	return NULL;
}

int fFindMaxPostId(const char *filename)
{
	FILE *fp;
	char line[256];
	postInfo *Infotemp;
	int maxid = 0;

	int userid = 0;
	int postid = 0;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		sscanf(line, "%d\t%d", //\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
					 &userid,
					 &postid
					 /*,Infotemp->shipper, Infotemp->shipperaddr,
					 Infotemp->shiptime,
					 Infotemp->receiver,
					 Infotemp->receiveraddr,
					 Infotemp->tel,
					 Infotemp->status*/
		);
		if (maxid < postid)
		{
			maxid = postid;
		}
	}

	//	free(Infotemp);
	fclose(fp);
	return maxid;
}

void SavePostInfo(const char *filename, list_t *polist)
{
	FILE *fp;
	list_node_t *node;
	postInfo *Infotemp;
	list_iterator_t *it;

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}

	it = list_iterator_new(polist, LIST_HEAD);
	fprintf(fp, "#用户id 运单号  发货人 发货地址   发货时间  收货人  收货地  电话	状态(d = 带取货 i = 在途 f = 已签收)\n");
	while ((node = list_iterator_next(it)))
	{
		Infotemp = (postInfo *)node->val;
		fprintf(fp, "%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
						Infotemp->userid, Infotemp->postid,
						Infotemp->shipper, Infotemp->shipperaddr,
						Infotemp->shiptime,
						Infotemp->receiver,
						Infotemp->receiveraddr,
						Infotemp->tel,
						Infotemp->status);
	}
	list_iterator_destroy(it);
	fflush(fp);
	fclose(fp);
}

//----------------处方信息----------------------------//
list_t *ReadPrescription(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[256];
	Prescription *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(postInfo));
		if (NULL == list)
		{
			list = list_new();
			//list->match = DoctorInfomatch;
			list->free = Nonemallocfree;
		}
		sscanf(line, "%d\t%s\t%d\t%d\t%s\t%ld\t%s\t%d",
					 &Infotemp->id, Infotemp->date,
					 &Infotemp->userid, &Infotemp->doctorid,
					 Infotemp->dept, &Infotemp->amount,
					 Infotemp->status, &Infotemp->postid);
		list_rpush(list, list_node_new(Infotemp));
	}

	fclose(fp);
	return list;
}
Prescription *FindPrescription(list_t *ps, int id)
{
	list_iterator_t *it = list_iterator_new(ps, LIST_HEAD);
	list_node_t *node;
	Prescription *Infotemp = NULL;

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (Prescription *)node->val;
		if (Infotemp->id == id)
		{
			list_iterator_destroy(it);
			return Infotemp;
		}
		Infotemp = NULL;
	}

	list_iterator_destroy(it);
	return NULL;
}
Prescription *fFindPrescription(const char *filename, int psid)
{
	FILE *fp;
	char line[256];
	Prescription *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	Infotemp = malloc(sizeof(postInfo));
	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		sscanf(line, "%d\t%s\t%d\t%d\t%s\t%ld\t%s\t%d",
					 &Infotemp->id, Infotemp->date,
					 &Infotemp->userid, &Infotemp->doctorid,
					 Infotemp->dept, &Infotemp->amount,
					 Infotemp->status, &Infotemp->postid);
		// sscanf(line, "%d",
		// 			 &Infotemp->id);

		if (Infotemp->id == psid)
		{
			fclose(fp);
			return Infotemp;
		}
	}

	fclose(fp);
	free(Infotemp);
	TRACE(("not find\n"));
	return NULL;
}
void SavePrescription(const char *filename, list_t *pslist)
{
	FILE *fp;
	list_node_t *node;
	Prescription *Infotemp;
	list_iterator_t *it;

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}
	fprintf(fp, "#处方id  日期 患者id 医生id 科室   金额  状态 \n");
	it = list_iterator_new(pslist, LIST_HEAD);
	while ((node = list_iterator_next(it)))
	{
		Infotemp = (Prescription *)node->val;
		fprintf(fp, "%d\t%s\t%d\t%d\t%s\t%ld\t%s\t%d\n",
						Infotemp->id, Infotemp->date,
						Infotemp->userid, Infotemp->doctorid,
						Infotemp->dept, Infotemp->amount,
						Infotemp->status, Infotemp->postid);
	}
	list_iterator_destroy(it);
	fflush(fp);
	fclose(fp);
}

//------------------挂号信息---------------------//
list_t *ReadRegistration(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[256];
	RegisterInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(RegisterInfo));
		if (NULL == list)
		{
			list = list_new();
			//list->match = DoctorInfomatch;
			list->free = Nonemallocfree;
		}
		sscanf(line, "%d\t%d\t%d\t%s\t%s\t%d\t%d\t%s",
					 &Infotemp->id, &Infotemp->userid, &Infotemp->doctorid,
					 Infotemp->dept, Infotemp->datetime, &Infotemp->serial,
					 &Infotemp->status, Infotemp->disease);
		//恢复回车
		strrpl(Infotemp->disease, '#', '\r');
		list_rpush(list, list_node_new(Infotemp));
	}

	fclose(fp);
	return list;
}
void SaveRegistration(const char *filename, list_t *rlist)
{
	FILE *fp;
	list_node_t *node;
	RegisterInfo *Infotemp;
	list_iterator_t *it;

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}
	fprintf(fp, "#挂号单号  患者id 医生id 科室 时间 流水号 状态(0未缴费 1已缴费 3已完成) 病情描述\n");
	it = list_iterator_new(rlist, LIST_HEAD);
	while ((node = list_iterator_next(it)))
	{
		Infotemp = (RegisterInfo *)node->val;
		//转换回车为#
		strrpl(Infotemp->disease, '\r', '#');
		fprintf(fp, "%d\t%d\t%d\t%s\t%s\t%d\t%d\t%s\n",
						Infotemp->id, Infotemp->userid, Infotemp->doctorid,
						Infotemp->dept, Infotemp->datetime, Infotemp->serial,
						Infotemp->status, Infotemp->disease);
	}

	list_iterator_destroy(it);
	fflush(fp);
	fclose(fp);
}
list_t *ReadRegistrationbyUser(const char *filename, int userid)
{
	FILE *fp;
	list_t *list = NULL;
	char line[256];
	RegisterInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(RegisterInfo));
		if (NULL == list)
		{
			list = list_new();
			//list->match = DoctorInfomatch;
			list->free = Nonemallocfree;
		}
		sscanf(line, "%d\t%d\t%d\t%s\t%s\t%d\t%d\t%s",
					 &Infotemp->id, &Infotemp->userid, &Infotemp->doctorid,
					 Infotemp->dept, Infotemp->datetime, &Infotemp->serial,
					 &Infotemp->status, Infotemp->disease);
		if (Infotemp->userid == userid)
		{
			list_rpush(list, list_node_new(Infotemp));
		}
		else
		{
			free(Infotemp);
			Infotemp = NULL;
		}
	}

	fclose(fp);
	return list;
}
RegisterInfo *FindRegisterInfo(list_t *self, int rgsid)
{
	list_iterator_t *it = list_iterator_new(self, LIST_HEAD);
	list_node_t *node;
	RegisterInfo *Infotemp = NULL;
	while ((node = list_iterator_next(it)))
	{
		if (node && node->val)
		{
			Infotemp = (RegisterInfo *)node->val;
			if (Infotemp->id == rgsid)
			{
				list_iterator_destroy(it);
				return Infotemp;
			}
			Infotemp = NULL;
		}
	}

	list_iterator_destroy(it);
	return NULL;
}

RegisterInfo *fFindRegisterInfo(const char *filename, int rgsid)
{
	FILE *fp;
	char line[256];
	RegisterInfo *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	Infotemp = malloc(sizeof(RegisterInfo));
	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		sscanf(line, "%d\t%d\t%d\t%s\t%s\t%d\t%d\t%s",
					 &Infotemp->id, &Infotemp->userid, &Infotemp->doctorid,
					 Infotemp->dept, Infotemp->datetime, &Infotemp->serial,
					 &Infotemp->status, Infotemp->disease);
		if (Infotemp->id == rgsid)
		{
			fclose(fp);
			return Infotemp;
		}
	}

	fclose(fp);
	free(Infotemp);
	return NULL;
}

////////////////////////药品///////////////////////////
DrugItem *fFindDrugItem(const char *filename, int drugid)
{
	FILE *fp;
	char line[256];
	DrugItem *Infotemp;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("%s(%d):unable to open %s\r\n", __FILE__, __LINE__, filename));
		return NULL;
	}

	Infotemp = malloc(sizeof(postInfo));
	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;
		sscanf(line, "%d\t%s\t%s\t%s\t%s\t%ld\t%s\t%s\t%d",
					 &Infotemp->id, Infotemp->name, Infotemp->type,
					 Infotemp->unit, Infotemp->kind,
					 &Infotemp->price, Infotemp->supler, Infotemp->date,
					 &Infotemp->supler);

		if (drugid == Infotemp->id)
		{
			fclose(fp);
			return Infotemp;
		}
	}

	free(Infotemp);
	fclose(fp);
	return NULL;
}

list_t *ReadDrugItembyName(const char *filename, char *name)
{
	FILE *fp;
	list_t *list = NULL;
	char line[256];
	DrugItem *Infotemp;
	int i = 0;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}

	while (fgets(line, 256, fp))
	{
		if (line[0] == '#')
			continue;

		Infotemp = malloc(sizeof(DrugItem));
		if (NULL == list)
		{
			list = list_new();
			//list->match = DoctorInfomatch;
			list->free = Nonemallocfree;
		}

		sscanf(line, "%d\t%s\t%s\t%s\t%s\t%ld\t%s\t%s\t%d",
					 &Infotemp->id, Infotemp->name, Infotemp->type,
					 Infotemp->unit, Infotemp->kind,
					 &Infotemp->price, Infotemp->supler, Infotemp->date,
					 &Infotemp->supler);

		if (strstr(Infotemp->name, name) != NULL)
		{
			list_rpush(list, list_node_new(Infotemp));
			if (++i > 5)
				break;
		}
		else
			free(Infotemp);
	}

	fclose(fp);
	return list;
}