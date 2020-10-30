#ifndef __USER_H__
#define __USER_H__
#include "macrodef.h"

#include <time.h>
#include "list.h"

typedef struct
{
  int userID;
  char username[10];
  char password[11];

  int userType;
} userInfo;

typedef struct
{
  unsigned int id;     //�û�Ψһ����
  char name[10];       //�û�����,�4������
  char sex;            //�Ա�,1Ϊ��,0ΪŮ
  char marriagestatus; //����ʷ,0Ϊδ��,1Ϊ�ѻ�,2Ϊ����
  char id_number[19];  //18λ���֤��
  int year;            //�������
  char month;          //�����·�
  char telenum[12];    //�绰����
  char addr[32];       //סַ,15������
  char allergy[16];    //����ʷ,7������

  long money; //���,�Է�Ϊ��λ
} PatientInfo;

typedef struct
{
  unsigned int id;    //����Ψһ����
  char deptname[16];  //��������
  char deptintro[64]; //���Ҽ��
} DeptInfo;

typedef struct
{
  unsigned int id; //ҽ��Ψһ����
  char name[10];   //�û�����,�4������
  char sex;        //�Ա�,1Ϊ��,0ΪŮ
  int deptid;      //��������id
  char intro[50];  //ҽ�����
  char title;      //0=��ͨҽ��,1=������ҽʦ,2=����ҽʦ
} DoctorInfo;

typedef struct
{
  unsigned int id; //ҩƷΨһ����
  char name[32];   //ҩƷ����,15������
  char type[10];   //����
  char unit[3];    //��λ
  char kind[16];   //���
  long price;      //�۸�
  char supler[32]; //��Ӧ��
  char date[20];   // ��������
  int stock;       //�������
} DrugItem;

typedef struct
{
  unsigned char id; //�ֿ�Ψһ����
  char addr[32];    //�ֿ��ַ,������15��
  char name[15];    //�ֿ�����,������7��
} Warehouse;

// typedef struct
// {
//   DrugItem drug;   //ҩƷ��Ϣ
//   int num;         //ҩƷ����
//   DrugList *pnext; //ָ����һҩƷ
// } DrugList;

// typedef struct
// {
//   Warehouse warehouse; //�ֿ���Ϣ
//   DrugList *head;      //ҩƷ���ͷָ��
// } Storage;

typedef struct
{
  unsigned int id;     //����Ψһ����
  unsigned int userid; //�û�Ψһ����
  PatientInfo *patient;
  unsigned int doctorid; //ҽ��Ψһ����
  DoctorInfo *doctor;
  char date[20];
  char dept[12];  //����
  char status[8]; //����״̬
  long amount;    //���
  int postid;
  int registerid;
} Prescription;

typedef struct
{
  unsigned int prescriptionid;
  unsigned int drugItemid;
  long price;
  unsigned int amount;
  unsigned int count;
} PrescriptionDrugItem;

typedef struct
{
  unsigned int id;       //�Һŵ�Ψһ����
  unsigned int userid;   //�û�Ψһ����
  PatientInfo *puser;    //ָ���û�
  unsigned int doctorid; //ҽ��Ψһ����
  DoctorInfo *pdoctor;   //ָ��ҽ��
  char dept[10];         //����
  char datetime[20];     //�Һŵ�����ʱ��
  unsigned char serial;  //��ˮ��
  unsigned char status;  //�Һŵ�״̬,0δ�ɷ� 1�ѽɷ� 3�����
  char disease[256];     //�������� 127������
} RegisterInfo;

typedef struct
{
  unsigned int id;             //����Ψһ����
  unsigned int regid;          //�Һŵ�Ψһ����
  RegisterInfo *regiinfo;      //�Һŵ�
  char record_date[20];        //��������ʱ��
  char diagnosis[20];          //�������
  char handler[256];           //����
  unsigned int prescriptionid; //����Ψһ����
  Prescription *prescription;  //ָ�򴦷�
} MedicalRecord;

typedef struct
{
  unsigned int id;             //����Ψһ����
  time_t recordtime;           //��������ʱ��
  unsigned int prescriptionid; //����Ψһ����
  Prescription *pprescription; //ָ�򴦷�
  //Location *locationlist;
  char state; //����״̬,0δ����,1������,2��ǩ��
} Order;

//�˵�������Ϣ
typedef struct
{
  unsigned int userid;   //�û���Ϣ
  unsigned int postid;   //�˵���
  char shipper[32];      //������
  char shipperaddr[32];  //������ַ
  char shiptime[20];     //����ʱ��
  char receiver[16];     //�ջ���
  char receiveraddr[32]; //�ջ���ַ
  char tel[12];          //�ջ��˵绰

  char status[8]; //�˵�״̬
} postInfo;

//����������Ϣ
typedef struct postitem
{
  unsigned int billid;  //�˵����
  char arrivedtime[20]; //����ʱ��
  char arrivedaddr[32]; //�����ַ
  char status[16];      //����״̬
} postitem;

/*��filename������ļ��ж����û���Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadUserInfo(const char *filename);
/*��userinfo�����д洢���û���Ϣд��filename������ļ���*/
void SaveUserInfo(char *filename, list_t *userinfo);
/*ͨ���û�Ψһ����id�ҵ��û���Ϣuserinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
userInfo *FindUserInfo(list_t *userinfo, int id);

userInfo *fFindUserInfo(const char *filename, char *username, char *password);

/*��filename������ļ��ж���������Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadPatientInfo(const char *filename);
/*��patientinfo�����д洢�Ĳ�����Ϣд��filename������ļ���*/
void SavePatientInfo(char *filename, list_t *patientinfo);
/*ͨ������Ψһ����id�ҵ�������Ϣpatientinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
PatientInfo *FindPatientInfo(list_t *patientinfo, int id);
PatientInfo *fFindPatientInfo(const char *filename, int id);

/*��filename������ļ��ж���������Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadDeptInfo(const char *filename);
/*��deptinfo�����д洢�Ŀ�����Ϣд��filename������ļ���*/
void SaveDeptInfo(list_t *deptinfo, char *filename);
DeptInfo *fFindDeptInfo(const char *filename, int id);

/*ͨ������Ψһ����id�ҵ�������Ϣdeptinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
DeptInfo *FindDeptInfo(list_t *deptinfo, int id);
DeptInfo *fFindDeptInfo(const char *filename, int id);

/*��filename������ļ��ж���ҽ����Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadDoctorInfo(const char *filename);
/*��doctorinfo�����д洢��ҽ����Ϣд��filename������ļ���*/
void SaveDoctorInfo(char *filename, list_t *doctorinfo);
/*ͨ��ҽ��Ψһ����id�ҵ�ҽ����Ϣdoctorinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
DoctorInfo *FindDoctorInfo(list_t *doctorinfo, int id);
DoctorInfo *fFindDoctorInfo(const char *filename, int docid);

/*��ȡ������Ϣ*/
list_t *ReadPostInfo(const char *filename);
postInfo *fFindPostInfo(const char *filename, int postid);
void SavePostInfo(const char *filename, list_t *polist);

/*��ȡ������Ϣ*/
list_t *ReadPrescription(const char *filename);
Prescription *FindPrescription(list_t *ps, int id);
Prescription *fFindPrescription(const char *filename, int psid);
void SavePrescription(const char *filename, list_t *pslist);
list_t *ReadPSDrug(const char *filename);
void SavePSDrug(const char *filename, list_t *psdruglist);

/*�Һ�*/
list_t *ReadRegistration(const char *filename);
void SaveRegistration(const char *filename, list_t *rlist);
RegisterInfo *fFindRegisterInfo(const char *filename, int rgsid);
RegisterInfo *FindRegisterInfo(list_t *self, int rgsid);
/*����ҩƷ*/
DrugItem *fFindDrugItem(const char *filename, int drugid);
list_t *ReadDrugItembyName(const char *filename, char *name);

/*����*/
list_t *ReadMedicalRecord(const char *filename);
void SaveMedicalRecord(const char *filename, list_t *mrlist);
MedicalRecord *fFindMedicalRecordbyId(const char *filename, int mrid);
MedicalRecord *fFindMedicalRecordbyregId(const char *filename, int regid);
list_t *ReadMedicalRecordbyUserId(const char *filename, int userid);
list_t *ReadMedicalRecordbyDocId(const char *filename, int docid);

void strrpl(char *str, char oldch, char newch);
#endif