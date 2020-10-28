#include "data.h"
#include "list.h"
#include <stdio.h>
#include <string.h>

/***********Âß¼­Ä£¿é************/
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

/***********Êı¾İÄ£¿é************/
//---------------User-----------------//
/// <summary>
/// ´ÓÎÄ¼şÖĞ¶Á³öÓÃ»§ĞÅÏ¢²¢½«ĞÅÏ¢ÒÔÁ´±íĞÎÊ½´æÈëÄÚ´æ£¬·µ»Ø±íÍ·
/// </summary>
/// <param name="filename">ÎÄ¼şÃû</param>
/// <returns>ÓÃ»§ĞÅÏ¢Á´±í±íÍ·Ö¸Õë</returns>
list_t *ReadUserInfo(const char *filename)
{
	FILE *fp;
	list_t *list = NULL;
	char line[32]; //lineµÄ³¤¶ÈÒ»¶¨Òª¹»³¤£¬´óÓÚuserinfoÄÜ´æµÄ×î´ó×Ö·û³¤¶È
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
		//ÓÃsscanf½«lineÖĞµÄ¸÷Êı¾İ¶ÁÈë¶ÔÓ¦µÄ±äÁ¿ÖĞ
		sscanf(line, "%d\t%s\t%s\t%d", &(Infotemp->userID), Infotemp->username, Infotemp->password, &(Infotemp->userType));
		list_rpush(list, list_node_new(Infotemp));
	}

	fclose(fp);
	return list;
}
/// <summary>
/// ½«Á´±íÖĞ´æ´¢µÄÓÃ»§ĞÅÏ¢Ğ´ÈëÎÄ¼şÖĞ
/// </summary>
/// <param name="userinfo">ÓÃ»§ĞÅÏ¢Á´±í</param>
/// <param name="filename">ÎÄ¼şÃû</param>
void SaveUserInfo(list_t *userinfo, char *filename)
{
	FILE *fp;
	userInfo *Infotemp;
	list_node_t *node;

	list_iterator_t *it = list_iterator_new(userinfo, LIST_HEAD); //ÉêÇëµü´úÆ÷

	if ((fp = fopen(filename, "wt")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return;
	}

	while ((node = list_iterator_next(it)))
	{
		Infotemp = (userInfo *)node->val; //½«¿ÕÖ¸Õë×ª»»ÎªuserInfoÒÔÔÚÖ®ºóµÄfprintfÖĞÀûÓÃ->
		fprintf(fp, "%d\t%s\t%s\t%d\n", Infotemp->userID, Infotemp->username, Infotemp->password, Infotemp->userType);
		//ÓÃfprintfÏòÎÄ¼şÊä³ö
	}

	list_iterator_destroy(it);
	fclose(fp);
}
/// <summary>
/// Í¨¹ıÓÃ»§Î¨Ò»±àÂëÕÒµ½ÓÃ»§ĞÅÏ¢Á´±íÖĞ¶ÔÓ¦µÄÓÃ»§
/// </summary>
/// <param name="userinfo">ÓÃ»§ĞÅÏ¢Á´±í</param>
/// <param name="id">ÓÃ»§Î¨Ò»±àÂë</param>
/// <returns>Ö¸ÏòÕÒµ½µÄÓÃ»§µÄÖ¸Õë</returns>
userInfo *FindUserInfo(list_t *userinfo, int id)
{
	list_iterator_t *it = list_iterator_new(userinfo, LIST_HEAD);
	list_node_t *node;
	userInfo *Infotemp = NULL;

	while ((node = list_iterator_next(it))) //µü´ú
	{
		Infotemp = (userInfo *)node->val;
		if (Infotemp->userID == id) //±È¶Ôµ½ºóÌø³öÑ­»·
			break;
		Infotemp = NULL; //Îª±È¶Ôµ½£¬Ôò½«Ö¸Õë¹éÁã
	}

	list_iterator_destroy(it);
	return Infotemp;
}

userInfo *fFindUserInfo(const char *filename, char *username, char *password)
{
	FILE *fp;
	char line[32]; //lineµÄ³¤¶ÈÒ»¶¨Òª¹»³¤£¬´óÓÚuserinfoÄÜ´æµÄ×î´ó×Ö·û³¤¶È
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
	{ //ÓÃsscanf½«lineÖĞµÄ¸÷Êı¾İ¶ÁÈë¶ÔÓ¦µÄ±äÁ¿ÖĞ
		if (line[0] == '#')
			continue;
		sscanf(line, "%d\t%s\t%s\t%d", &(Infotemp->userID), Infotemp->username, Infotemp->password, &(Infotemp->userType));
		if (strcmpi(username, Infotemp->username) == 0 && 0 == strcmpi(password, Infotemp->password))
		{ //ºöÂÔ´óĞ¡Ğ´
			fclose(fp);
			return Infotemp;
		}
	}
	free(Infotemp);
	fclose(fp);
	return NULL;
}

int fFindUsername(const char *filename, char *username)
{
	FILE *fp;
	char line[32]; //lineçš„é•¿åº¦ä¸€å®šè¦å¤Ÿé•¿ï¼Œå¤§äºuserinfoèƒ½å­˜çš„æœ€å¤§å­—ç¬¦é•¿åº¦
	userInfo *Infotemp;

	if (username == NULL)
		return NULL;

	if ((fp = fopen(filename, "r")) == NULL)
	{
		TRACE(("unable to open %s\r\n", filename));
		return NULL;
	}
	Infotemp = malloc(sizeof(userInfo));
	while (fgets(line, 32, fp))
	{ //ç”¨sscanfå°†lineä¸­çš„å„æ•°æ®è¯»å…¥å¯¹åº”çš„å˜é‡ä¸­
		if (line[0] == '#')
			continue;
		sscanf(line, "%d\t%s\t%s\t%d", &(Infotemp->userID), Infotemp->username, Infotemp->password, &(Infotemp->userType));
		if (strcmpi(username, Infotemp->username) == 0)
		{ //å¿½ç•¥å¤§å°å†™
			free(Infotemp);
			fclose(fp);
			return 1;
		}
	}
	free(Infotemp);
	fclose(fp);
	return 0;
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

void SavePatientInfo(list_t *patientinfo, char *filename)
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

void SaveDoctorInfo(list_t *doctorinfo, char *filename)
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

//----------------ÎïÁ÷ĞÅÏ¢----------------------------//

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
	fprintf(fp, "#ÓÃ»§id ÔËµ¥ºÅ  ·¢»õÈË ·¢»õµØÖ·   ·¢»õÊ±¼ä  ÊÕ»õÈË  ÊÕ»õµØ  µç»°	×´Ì¬(d = ´øÈ¡»õ i = ÔÚÍ¾ f = ÒÑÇ©ÊÕ)\n");
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

//----------------´¦·½ĞÅÏ¢----------------------------//
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
	fprintf(fp, "#´¦·½id  ÈÕÆÚ »¼Õßid Ò½Éúid ¿ÆÊÒ   ½ğ¶î  ×´Ì¬ \n");
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

//------------------¹ÒºÅĞÅÏ¢---------------------//
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
		sscanf(line, "%d\t%d\t%d\t%s\t%s\t%d\t%d",
					 &Infotemp->id, &Infotemp->userid, &Infotemp->doctorid,
					 Infotemp->dept, Infotemp->datetime, &Infotemp->serial,
					 &Infotemp->status);
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
	fprintf(fp, "#¹ÒºÅµ¥ºÅ  »¼Õßid Ò½Éúid ¿ÆÊÒ Ê±¼ä Á÷Ë®ºÅ ×´Ì¬\n");
	it = list_iterator_new(rlist, LIST_HEAD);
	while ((node = list_iterator_next(it)))
	{
		Infotemp = (RegisterInfo *)node->val;
		fprintf(fp, "%d\t%d\t%d\t%s\t%s\t%d\t%d",
						Infotemp->id, Infotemp->userid, Infotemp->doctorid,
						Infotemp->dept, Infotemp->datetime, Infotemp->serial,
						Infotemp->status);
	}

	list_iterator_destroy(it);
	fflush(fp);
	fclose(fp);
}

////////////////////////Ò©Æ·///////////////////////////
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