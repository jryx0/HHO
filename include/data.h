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
  unsigned int id;     //用户唯一编码
  char name[10];       //用户姓名,最长4个汉字
  char sex;            //性别,1为男,0为女
  char marriagestatus; //婚姻史,0为未婚,1为已婚,2为离异
  char id_number[19];  //18位身份证号
  int year;            //出生年份
  char month;          //出生月份
  char telenum[12];    //电话号码
  char addr[32];       //住址,15字以内
  char allergy[16];    //过敏史,7字以内

  long money; //余额,以分为单位
} PatientInfo;

typedef struct
{
  unsigned int id;    //科室唯一编码
  char deptname[16];  //科室名称
  char deptintro[64]; //科室简介
} DeptInfo;

typedef struct
{
  unsigned int id; //医生唯一编码
  char name[10];   //用户姓名,最长4个汉字
  char sex;        //性别,1为男,0为女
  int deptid;      //所属科室id
  char intro[50];  //医生简介
  char title;      //0=普通医生,1=副主任医师,2=主任医师
} DoctorInfo;

typedef struct
{
  unsigned int id; //药品唯一编码
  char name[32];   //药品名称,15字以内
  char type[10];   //剂型
  char unit[3];    //单位
  char kind[16];   //规格
  long price;      //价格
  char supler[32]; //供应商
  char date[20];   // 生产日期
  int stock;       //库存数量
} DrugItem;

typedef struct
{
  unsigned char id; //仓库唯一编码
  char addr[32];    //仓库地址,不超过15字
  char name[15];    //仓库名称,不超过7字
} Warehouse;

// typedef struct
// {
//   DrugItem drug;   //药品信息
//   int num;         //药品数量
//   DrugList *pnext; //指向下一药品
// } DrugList;

// typedef struct
// {
//   Warehouse warehouse; //仓库信息
//   DrugList *head;      //药品库存头指针
// } Storage;

typedef struct
{
  unsigned int id;     //处方唯一编码
  unsigned int userid; //用户唯一编码
  PatientInfo *patient;
  unsigned int doctorid; //医生唯一编码
  DoctorInfo *doctor;
  char date[20];
  char dept[12];  //科室
  char status[8]; //处方状态
  long amount;    //金额
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
  unsigned int id;       //挂号单唯一编码
  unsigned int userid;   //用户唯一编码
  PatientInfo *puser;    //指向用户
  unsigned int doctorid; //医生唯一编码
  DoctorInfo *pdoctor;   //指向医生
  char dept[10];         //科室
  char datetime[20];     //挂号单生成时间
  unsigned char serial;  //流水号
  unsigned char status;  //挂号单状态,0未缴费 1已缴费 3已完成
  char disease[256];     //病情描述 127个汉字
} RegisterInfo;

typedef struct
{
  unsigned int id;             //病历唯一编码
  unsigned int regid;          //挂号单唯一编码
  RegisterInfo *regiinfo;      //挂号单
  char record_date[20];        //病历生成时间
  char diagnosis[20];          //病情诊断
  char handler[256];           //处置
  unsigned int prescriptionid; //处方唯一编码
  Prescription *prescription;  //指向处方
} MedicalRecord;

typedef struct
{
  unsigned int id;             //订单唯一编码
  time_t recordtime;           //订单生成时间
  unsigned int prescriptionid; //处方唯一编码
  Prescription *pprescription; //指向处方
  //Location *locationlist;
  char state; //订单状态,0未出库,1运送中,2已签收
} Order;

//运单主表信息
typedef struct
{
  unsigned int userid;   //用户信息
  unsigned int postid;   //运单号
  char shipper[32];      //发货人
  char shipperaddr[32];  //发货地址
  char shiptime[20];     //发货时间
  char receiver[16];     //收货人
  char receiveraddr[32]; //收货地址
  char tel[12];          //收货人电话

  char status[8]; //运单状态
} postInfo;

//物流过程信息
typedef struct postitem
{
  unsigned int billid;  //运单编号
  char arrivedtime[20]; //到达时间
  char arrivedaddr[32]; //到达地址
  char status[16];      //配送状态
} postitem;

/*从filename代表的文件中读出用户信息并将信息以链表形式存入内存，返回表头*/
list_t *ReadUserInfo(const char *filename);
/*将userinfo链表中存储的用户信息写入filename代表的文件中*/
void SaveUserInfo(char *filename, list_t *userinfo);
/*通过用户唯一编码id找到用户信息userinfo链表中对应的用户，并返回指向该信息的指针*/
userInfo *FindUserInfo(list_t *userinfo, int id);

userInfo *fFindUserInfo(const char *filename, char *username, char *password);

/*从filename代表的文件中读出病人信息并将信息以链表形式存入内存，返回表头*/
list_t *ReadPatientInfo(const char *filename);
/*将patientinfo链表中存储的病人信息写入filename代表的文件中*/
void SavePatientInfo(char *filename, list_t *patientinfo);
/*通过病人唯一编码id找到病人信息patientinfo链表中对应的用户，并返回指向该信息的指针*/
PatientInfo *FindPatientInfo(list_t *patientinfo, int id);
PatientInfo *fFindPatientInfo(const char *filename, int id);

/*从filename代表的文件中读出科室信息并将信息以链表形式存入内存，返回表头*/
list_t *ReadDeptInfo(const char *filename);
/*将deptinfo链表中存储的科室信息写入filename代表的文件中*/
void SaveDeptInfo(list_t *deptinfo, char *filename);
DeptInfo *fFindDeptInfo(const char *filename, int id);

/*通过科室唯一编码id找到科室信息deptinfo链表中对应的用户，并返回指向该信息的指针*/
DeptInfo *FindDeptInfo(list_t *deptinfo, int id);
DeptInfo *fFindDeptInfo(const char *filename, int id);

/*从filename代表的文件中读出医生信息并将信息以链表形式存入内存，返回表头*/
list_t *ReadDoctorInfo(const char *filename);
/*将doctorinfo链表中存储的医生信息写入filename代表的文件中*/
void SaveDoctorInfo(char *filename, list_t *doctorinfo);
/*通过医生唯一编码id找到医生信息doctorinfo链表中对应的用户，并返回指向该信息的指针*/
DoctorInfo *FindDoctorInfo(list_t *doctorinfo, int id);
DoctorInfo *fFindDoctorInfo(const char *filename, int docid);

/*获取物流信息*/
list_t *ReadPostInfo(const char *filename);
postInfo *fFindPostInfo(const char *filename, int postid);
void SavePostInfo(const char *filename, list_t *polist);

/*获取处方信息*/
list_t *ReadPrescription(const char *filename);
Prescription *FindPrescription(list_t *ps, int id);
Prescription *fFindPrescription(const char *filename, int psid);
void SavePrescription(const char *filename, list_t *pslist);
list_t *ReadPSDrug(const char *filename);
void SavePSDrug(const char *filename, list_t *psdruglist);

/*挂号*/
list_t *ReadRegistration(const char *filename);
void SaveRegistration(const char *filename, list_t *rlist);
RegisterInfo *fFindRegisterInfo(const char *filename, int rgsid);
RegisterInfo *FindRegisterInfo(list_t *self, int rgsid);
/*查找药品*/
DrugItem *fFindDrugItem(const char *filename, int drugid);
list_t *ReadDrugItembyName(const char *filename, char *name);

/*病例*/
list_t *ReadMedicalRecord(const char *filename);
void SaveMedicalRecord(const char *filename, list_t *mrlist);
MedicalRecord *fFindMedicalRecordbyId(const char *filename, int mrid);
MedicalRecord *fFindMedicalRecordbyregId(const char *filename, int regid);
list_t *ReadMedicalRecordbyUserId(const char *filename, int userid);
list_t *ReadMedicalRecordbyDocId(const char *filename, int docid);

void strrpl(char *str, char oldch, char newch);
#endif