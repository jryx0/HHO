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

  char userType;
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
  float price;     //ҩƷ�۸�
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
  unsigned int id;       //����Ψһ����
  unsigned int userid;   //�û�Ψһ����
  PatientInfo *puser;    //ָ���û�
  unsigned int doctorid; //ҽ��Ψһ����
  DoctorInfo *pdoctor;   //ָ��ҽ��
  time_t datetime;       //��������ʱ��
  //DrugList *head;        //ҩƷ�嵥ͷָ��
  //list_t * Druglist;
} Prescription;

typedef struct
{
  unsigned int id;       //�Һŵ�Ψһ����
  unsigned int userid;   //�û�Ψһ����
  PatientInfo *puser;    //ָ���û�
  unsigned int doctorid; //ҽ��Ψһ����
  DoctorInfo *pdoctor;   //ָ��ҽ��
  time_t datetime;       //�Һŵ�����ʱ��
  unsigned char serial;  //��ˮ��
  char state;            //�Һŵ�״̬,0δ����,1������,2�ѿ���
} RegisterInfo;

typedef struct
{
  unsigned int id;             //����Ψһ����
  unsigned int regid;          //�Һŵ�Ψһ����
  RegisterInfo *bookinfo;      //�Һŵ�
  time_t recordtime;           //��������ʱ��
  char illness[50];            //����
  char diagnosis[50];          //�������
  char advice;                 //���ý���
  unsigned int prescriptionid; //����Ψһ����
  Prescription *pprescription; //ָ�򴦷�
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
  unsigned int orderid;  //������
  unsigned int billid;   //�˵���
  char receiver[16];     //�ջ���
  char receiveraddr[32]; //�ջ���ַ
  char tel[12];          //�ջ��˵绰
  char shipper[32];      //������
  char shipperaddr[32];  //������ַ
  time_t shiptime;       //����ʱ��
  unsigned char status;  //�˵�״̬
} wallbill;

//����������Ϣ
typedef struct billitem
{
  unsigned int itemid;  // ���
  unsigned int billid;  //�˵����
  time_t arrivedtime;   //����ʱ��
  char arrivedaddr[32]; //�����ַ
  char *postman[16];    //������
} billitem;

/*��filename������ļ��ж����û���Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadUserInfo(const char *filename);
/*��userinfo�����д洢���û���Ϣд��filename������ļ���*/
void SaveUserInfo(list_t *userinfo, char *filename);
/*ͨ���û�Ψһ����id�ҵ��û���Ϣuserinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
userInfo *FindUserInfo(list_t *userinfo, int id);

userInfo *fFindUserInfo(const char *filename, char *username, char *password);

/*��filename������ļ��ж���������Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadPatientInfo(const char *filename);
/*��patientinfo�����д洢�Ĳ�����Ϣд��filename������ļ���*/
void SavePatientInfo(list_t *patientinfo, char *filename);
/*ͨ������Ψһ����id�ҵ�������Ϣpatientinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
PatientInfo *FindPatientInfo(list_t *patientinfo, int id);

/*��filename������ļ��ж���������Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadDeptInfo(const char *filename);
/*��deptinfo�����д洢�Ŀ�����Ϣд��filename������ļ���*/
void SaveDeptInfo(list_t *deptinfo, char *filename);
/*ͨ������Ψһ����id�ҵ�������Ϣdeptinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
DeptInfo *FindDeptInfo(list_t *deptinfo, int id);

/*��filename������ļ��ж���ҽ����Ϣ������Ϣ��������ʽ�����ڴ棬���ر�ͷ*/
list_t *ReadDoctorInfo(const char *filename);
/*��doctorinfo�����д洢��ҽ����Ϣд��filename������ļ���*/
void SaveDoctorInfo(list_t *doctorinfo, char *filename);
/*ͨ��ҽ��Ψһ����id�ҵ�ҽ����Ϣdoctorinfo�����ж�Ӧ���û���������ָ�����Ϣ��ָ��*/
DoctorInfo *FindDoctorInfo(list_t *doctorinfo, int id);

#endif