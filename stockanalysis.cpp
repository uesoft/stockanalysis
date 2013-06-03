// stockanalysis.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "stockanalysis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CstockanalysisApp

BEGIN_MESSAGE_MAP(CstockanalysisApp, CWinApp)
END_MESSAGE_MAP()


// CstockanalysisApp ����

CstockanalysisApp::CstockanalysisApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CstockanalysisApp ����

CstockanalysisApp theApp;


// CstockanalysisApp ��ʼ��

BOOL CstockanalysisApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

#include "mex.h"

#define DWORD long  
#define NUMBER_OF_STRUCTS (sizeof(friends)/sizeof(struct phonebook)) 
#define NUMBER_OF_FIELDS (sizeof(field_names)/sizeof(*field_names))

void mexFunction(int nlhs, 
    mxArray * plhs[] , int nrhs,const mxArray * pahs[]) 
{ 
typedef struct { 
        char   datatype[6];  //byte [6]��6 �ֽڳ��ȣ��̶�Ϊ {0x68��0x64��0x31��0x2E�� 0x30��0x00}������ʶ�������ļ����ͣ� 
DWORD lRecordsNum; //4 �ֽڳ��ȣ���¼�����ݡ�����ļ�¼������ 
WORD nPosStart;//2 �ֽڳ��ȣ���¼�����ݡ�����Ŀ�ʼλ�ã� 
WORD nRecordLength;//2 �ֽڳ��ȣ���¼�����ݡ�����ÿ����¼���ֽڳ��ȣ� 
WORD nFieldsNum;//2�ֽڳ��ȣ���¼���ж��塱���и�����
    }ST_K_DAY_HEADER; //�ļ�ͷ����
typedef struct { 
        char   fieldinfo[3];  //byte [3]��3 �ֽڳ��ȣ� 
byte nFiledLength; //1 �ֽڳ��ȣ������ݵ��ֽڳ��ȣ� 
    }ST_K_DAY_FIELDHEADER; //�ֶ�ͷ����
typedef struct { 
        DWORD    dt;  //����: �����������Ϊ20031110,����2003��11��10�� 
        DWORD    k_price;//���̼�*0.01����:�����ļ۸��0.01Ϊʵ�ʼ۸���ͬ�� 
        DWORD    h_price;//��߼�*0.01 
        DWORD    l_price;//��׼�*0.01 
        DWORD    s_price;//���̼� 
        float    s_tolmoney;//(��λ:��) 
        DWORD bur_tol;//�ɽ��� 
        DWORD unknown; ///û��ʹ�� 
    }ST_K_DATE; 
    FILE * f = 0; 
    int nfieldidx= 0; 
    char * pkdata; 
    int lday = 8; 
    mwSize dims[2] = {1, lday }; 
   char msg[128] = ""; 
   char path[512] = {0}; 
   int i,r,c ,*val; 
   double  *kp,*sp,*hp,*lp,*tm,*vol; 
   int lseekpos;     
   mxArray *field_value;    
   const char *field_names[] = {"����", "���̼�","��߼�","��ͼ�","���̼�","�ɽ����","�ɽ���"};   
   ST_K_DATE * pd = 0; 
   mexPrintf("good");
   r = mxGetM(pahs[0]); 
   c = mxGetN(pahs[0]); 
   sprintf(msg,"r:%d , c:%d  rn",r,c); 
   mexPrintf(msg); 
   mxGetString(pahs[0],msg,128); 
   if("600000"<msg)
   {
   sprintf(path,"D:\\ͬ��˳����\\history\\shase\\day\\%s.day",msg);
   }
   else
   {
	   sprintf(path,"D:\\ͬ��˳����\\history\\sznse\\day\\%s.day",msg); 
   }
   
   mexPrintf(path);   
   lday = (int)*mxGetPr(pahs[1]); 
   dims[1] = lday; 
   //pkdata = malloc (sizeof(ST_K_DATE)*lday); 
   plhs[0] = mxCreateStructArray(2, dims, NUMBER_OF_FIELDS, field_names); 
   plhs[1] = mxCreateDoubleMatrix(1,lday,mxREAL); 
   plhs[2] = mxCreateDoubleMatrix(1,lday,mxREAL); 
   plhs[3] = mxCreateDoubleMatrix(1,lday,mxREAL); 
   plhs[4] = mxCreateDoubleMatrix(1,lday,mxREAL); 
   plhs[5] = mxCreateDoubleMatrix(1,lday,mxREAL); 
   plhs[6] = mxCreateDoubleMatrix(1,lday,mxREAL); 
   f = fopen(path,"r+b"); 
   lseekpos = sizeof(ST_K_DATE)*lday; 
   fseek(f,-lseekpos,SEEK_END); 
   fread(pkdata,sizeof(ST_K_DATE),lday,f); 
   pd = (ST_K_DATE*)pkdata; 
   for (i=0;lday;i++){ 
       nfieldidx = 0; 
       msg[0] = 0; 
       sprintf(msg,"%d",pd[i].dt); 
       mxSetField(plhs[0],i,field_names[nfieldidx++],mxCreateString(msg));  
       field_value = mxCreateDoubleMatrix(1,1,mxREAL); 
       mxGetPr(plhs[nfieldidx])[i] = *mxGetPr(field_value) = pd[i].k_price*0.01f; 
       mxSetField(plhs[0],i,field_names[nfieldidx++],field_value); 
       field_value = mxCreateDoubleMatrix(1,1,mxREAL); 
       mxGetPr(plhs[nfieldidx])[i] = *mxGetPr(field_value) = pd[i].h_price*0.01f; 
       mxSetField(plhs[0],i,field_names[nfieldidx++],field_value); 
       field_value = mxCreateDoubleMatrix(1,1,mxREAL); 
       mxGetPr(plhs[nfieldidx])[i] = *mxGetPr(field_value) = pd[i].l_price*0.01f; 
       mxSetField(plhs[0],i,field_names[nfieldidx++],field_value); 
       field_value = mxCreateDoubleMatrix(1,1,mxREAL); 
       mxGetPr(plhs[nfieldidx])[i] = *mxGetPr(field_value) = pd[i].s_price*0.01f; 
       mxSetField(plhs[0],i,field_names[nfieldidx++],field_value); 
       field_value = mxCreateDoubleMatrix(1,1,mxREAL); 
       mxGetPr(plhs[nfieldidx])[i] = *mxGetPr(field_value) = pd[i].s_tolmoney; 
       mxSetField(plhs[0],i,field_names[nfieldidx++],field_value); 
       field_value = mxCreateDoubleMatrix(1,1,mxREAL); 
       mxGetPr(plhs[nfieldidx])[i] = *mxGetPr(field_value) = pd[i].bur_tol; 
       mxSetField(plhs[0],i,field_names[nfieldidx++],field_value); 
   } 
   fclose(f); 
} 

