// stockanalysis.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "stockanalysis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CstockanalysisApp

BEGIN_MESSAGE_MAP(CstockanalysisApp, CWinApp)
END_MESSAGE_MAP()


// CstockanalysisApp 构造

CstockanalysisApp::CstockanalysisApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CstockanalysisApp 对象

CstockanalysisApp theApp;


// CstockanalysisApp 初始化

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
        char   datatype[6];  //byte [6]，6 字节长度，固定为 {0x68，0x64，0x31，0x2E， 0x30，0x00}，用于识别数据文件类型； 
DWORD lRecordsNum; //4 字节长度，记录“内容”区域的记录条数； 
WORD nPosStart;//2 字节长度，记录“内容”区域的开始位置； 
WORD nRecordLength;//2 字节长度，记录“内容”区域每条记录的字节长度； 
WORD nFieldsNum;//2字节长度，记录“列定义”的列个数；
    }ST_K_DAY_HEADER; //文件头定义
typedef struct { 
        char   fieldinfo[3];  //byte [3]，3 字节长度； 
byte nFiledLength; //1 字节长度，列内容的字节长度； 
    }ST_K_DAY_FIELDHEADER; //字段头定义
typedef struct { 
        DWORD    dt;  //日期: 比如读出数据为20031110,代表2003年11月10号 
        DWORD    k_price;//开盘价*0.01（）:读出的价格乘0.01为实际价格。下同。 
        DWORD    h_price;//最高价*0.01 
        DWORD    l_price;//最底价*0.01 
        DWORD    s_price;//收盘价 
        float    s_tolmoney;//(单位:亿) 
        DWORD bur_tol;//成交量 
        DWORD unknown; ///没有使用 
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
   const char *field_names[] = {"日期", "开盘价","最高价","最低价","收盘价","成交金额","成交量"};   
   ST_K_DATE * pd = 0; 
   mexPrintf("good");
   r = mxGetM(pahs[0]); 
   c = mxGetN(pahs[0]); 
   sprintf(msg,"r:%d , c:%d  rn",r,c); 
   mexPrintf(msg); 
   mxGetString(pahs[0],msg,128); 
   if("600000"<msg)
   {
   sprintf(path,"D:\\同花顺核新\\history\\shase\\day\\%s.day",msg);
   }
   else
   {
	   sprintf(path,"D:\\同花顺核新\\history\\sznse\\day\\%s.day",msg); 
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

