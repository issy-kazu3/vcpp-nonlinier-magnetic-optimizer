#include "StdAfx.h"
#include "CalcArray.h"
#include "CalcData.h"

CCalcArray::CCalcArray(void)
{
}

CCalcArray::~CCalcArray(void)
{
}

void CCalcArray::AddCalc(unsigned long id,int tu,double w_w,double t_w,double l_coi,double l_w,double h_c,double w_c,double g,
						 int n_g,double h,double w,double l,double wt_c,double wAll,double l_J,
		double LR,double Side,double In_Ch,double In_Cw,
		double CU11,double CL11,double BU11,double BL11,double P_I11,double P_C_DC11,double P_C_AC11,double Pall11,
		double CU12,double CL12,double BU12,double BL12,double P_I12,double P_C_DC12,double P_C_AC12,double Pall12,double Q12,double Lm12,
		double CU2,double CL2,double BU2,double BL2,double P_I2,double P_C_DC2,double P_C_AC2,double Pall2,double Q2,double Lmin,
		double CU3,double CL3,double BU3,double BL3,double P_I3,double P_C_DC3,double P_C_AC3,double Pall3,
		double CU41,double CL41,double BU41,double BL41,double P_I41,double P_C_DC41,double P_C_AC41,double Pall41,double Q41,
		double CU42,double CL42,double BU42,double BL42,double P_I42,double P_C_DC42,double P_C_AC42,double Pall42,double Q42,
		double CU43,double CL43,double BU43,double BL43,double P_I43,double P_C_DC43,double P_C_AC43,double Pall43,double Q43,
		double Q4,double P4,
		double* li)
{
	m_CalcArray.Add(new CCalcData(id,tu,w_w,t_w, l_coi,l_w, h_c, w_c, g,n_g, h,w,l, wt_c,wAll, l_J,
		 LR, Side, In_Ch, In_Cw,
		CU11,CL11,BU11, BL11, P_I11, P_C_DC11, P_C_AC11, Pall11,
		 CU12, CL12, BU12, BL12, P_I12, P_C_DC12, P_C_AC12, Pall12, Q12,Lm12,
		 CU2, CL2, BU2, BL2, P_I2, P_C_DC2, P_C_AC2, Pall2, Q2, Lmin,
		 CU3, CL3, BU3, BL3, P_I3, P_C_DC3, P_C_AC3, Pall3,
		 CU41, CL41, BU41, BL41, P_I41, P_C_DC41, P_C_AC41, Pall41, Q41,
		 CU42, CL42, BU42, BL42, P_I42, P_C_DC42, P_C_AC42, Pall42, Q42,
		 CU43, CL43, BU43, BL43, P_I43, P_C_DC43, P_C_AC43, Pall43, Q43,
		 Q4, P4,
		 li));
}
//AddCalcのindex有り版
void CCalcArray::AddCalc(unsigned long index,unsigned long id,int tu,double w_w,double t_w,double l_coi,double l_w,double h_c,double w_c,double g,
						 int n_g,double h,double w,double l,double wt_c,double wAll,double l_J,
		double LR,double Side,double In_Ch,double In_Cw,
		double CU11,double CL11,double BU11,double BL11,double P_I11,double P_C_DC11,double P_C_AC11,double Pall11,
		double CU12,double CL12,double BU12,double BL12,double P_I12,double P_C_DC12,double P_C_AC12,double Pall12,double Q12,double Lm12,
		double CU2,double CL2,double BU2,double BL2,double P_I2,double P_C_DC2,double P_C_AC2,double Pall2,double Q2,double Lmin,
		double CU3,double CL3,double BU3,double BL3,double P_I3,double P_C_DC3,double P_C_AC3,double Pall3,
		double CU41,double CL41,double BU41,double BL41,double P_I41,double P_C_DC41,double P_C_AC41,double Pall41,double Q41,
		double CU42,double CL42,double BU42,double BL42,double P_I42,double P_C_DC42,double P_C_AC42,double Pall42,double Q42,
		double CU43,double CL43,double BU43,double BL43,double P_I43,double P_C_DC43,double P_C_AC43,double Pall43,double Q43,
		double Q4,double P4,
		double* li)
{
	m_CalcArray.InsertAt(index,new CCalcData(id,tu,w_w,t_w, l_coi,l_w, h_c, w_c, g,n_g, h,w,l, wt_c,wAll, l_J,
		 LR, Side, In_Ch, In_Cw,
		CU11,CL11,BU11, BL11, P_I11, P_C_DC11, P_C_AC11, Pall11,
		 CU12, CL12, BU12, BL12, P_I12, P_C_DC12, P_C_AC12, Pall12, Q12,Lm12,
		 CU2, CL2, BU2, BL2, P_I2, P_C_DC2, P_C_AC2, Pall2, Q2, Lmin,
		 CU3, CL3, BU3, BL3, P_I3, P_C_DC3, P_C_AC3, Pall3,
		 CU41, CL41, BU41, BL41, P_I41, P_C_DC41, P_C_AC41, Pall41, Q41,
		 CU42, CL42, BU42, BL42, P_I42, P_C_DC42, P_C_AC42, Pall42, Q42,
		 CU43, CL43, BU43, BL43, P_I43, P_C_DC43, P_C_AC43, Pall43, Q43,
		 Q4, P4,
		 li));
	//InsertAtのindexのところの変数は、ﾏﾆｭｱﾙを見るとINT_PTRになっている。けど、unsigned long変数で大丈夫なのかな？
}


void CCalcArray::DelAllData(void)
{
	CCalcData *cell;
	unsigned long chs=(unsigned long)(m_CalcArray.GetSize());
	for(unsigned long i=0;i<chs;i++){
		cell=(CCalcData *)m_CalcArray.GetAt(i);
		delete cell;
	}
	m_CalcArray.RemoveAll();
}

void CCalcArray::RemoveData(unsigned long index)
{
	CCalcData *cell;
		cell=(CCalcData *)m_CalcArray.GetAt(index);
		delete cell;
	m_CalcArray.RemoveAt(index);
}

unsigned long CCalcArray::GetSize(void)
{
	unsigned long i;
	i=(unsigned long)(m_CalcArray.GetSize());
	return i;
}
/*
unsigned long CCalcArray::Get_ID(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->ID;
}

int CCalcArray::Get_turn(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->turn;
}

double CCalcArray::Get_w_wire(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->w_wire;
}

double CCalcArray::Get_t_wire(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->t_wire;
}

double CCalcArray::Get_h_core(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->h_core;
}

double CCalcArray::Get_w_core(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->w_core;
}

double CCalcArray::Get_gap(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->gap;
}

int CCalcArray::Get_n_gap(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->n_gap;
}

double CCalcArray::Get_height(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->height;
}

double CCalcArray::Get_width(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->width;
}

double CCalcArray::Get_length(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->length;
}

double CCalcArray::Get_wt_core(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->wt_core;
}
*/
double CCalcArray::Get_wt(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->wt;
}
/*
double CCalcArray::Get_sat_max_C(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_max_C;
}

double CCalcArray::Get_sat_min_C(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_min_C;
}


double CCalcArray::Get_sat_max_B(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_max_B;
}

double CCalcArray::Get_sat_min_B(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_min_B;
}

double CCalcArray::Get_P_Iron(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->P_Iron;
}

double CCalcArray::Get_P_Cu_DC(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->P_Cu_DC;
}
*/

double CCalcArray::Get_P(unsigned long index,int i)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->P[i];
}

/*
double CCalcArray::Get_max_B(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->max_B;
}

double CCalcArray::Get_min_B(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->min_B;
}

double CCalcArray::Get_max_C(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->max_C;
}

double CCalcArray::Get_min_C(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->min_C;
}

double CCalcArray::Get_In_Coil_h(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->In_Coil_h;
}

double CCalcArray::Get_In_Coil_w(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->In_Coil_w;
}

double CCalcArray::Get_l_coil(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->l_coil;
}

double CCalcArray::Get_l_JIRO(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->l_JIRO;
}

double CCalcArray::Get_LR_dis(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->LR_dis;
}

double CCalcArray::Get_l_wire(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->l_wire;
}

double CCalcArray::Get_P_Cu_AC(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->P_Cu_AC;
}

double CCalcArray::Get_sat_P(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_P;
}

double CCalcArray::Get_sat_P_Iron(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_P_Iron;
}

double CCalcArray::Get_sat_P_Cu_AC(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_P_Cu_AC;
}

double CCalcArray::Get_sat_P_Cu_DC(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->sat_P_Cu_DC;
}

double CCalcArray::Get_Side_dis(unsigned long index)
{
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
		return NULL;
	return ((CCalcData *)(m_CalcArray.GetAt(index)))->Side_dis;
}

int CCalcArray::Get_LI(unsigned long index,double* li)
{
	int i;
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
	{
		return NULL;
	}
	else{
		for(i=0;i<=20;i++){
			li[i]=((CCalcData *)(m_CalcArray.GetAt(index)))->LI[i];
		}
	return 1;
	}
}

//CCalcData *CCalcArray::Get_cell(unsigned long index){
//	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1)))
//	{
//		return NULL;
//	}
//	else{
//		return (CCalcData *)(m_CalcArray.GetAt(index));
//	}
//}

*/

int CCalcArray::SetString(CString* str, unsigned long index)
{
	CString str2;
	char str3[1000];
	CCalcData *cell;
	int i;
	if((index<0)||(index>(unsigned long)(m_CalcArray.GetSize()-1))){
		return NULL;
	}else{
		cell= (CCalcData *)(m_CalcArray.GetAt(index));
		*str=_ultoa(cell->ID,str3,10);
		*str+=",";
		*str+=_itoa(cell->turn,str3,10);
		*str+=",";
		*str+=_gcvt(cell->w_wire,10,str3);
		*str+=",";
		*str+=_gcvt(cell->t_wire,10,str3);
		*str+=",";
		*str+=_gcvt(cell->l_coil,10,str3);
		*str+=",";
		*str+=_gcvt(cell->l_wire,10,str3);
		*str+=",";
		*str+=_gcvt(cell->w_core,10,str3);
		*str+=",";
		*str+=_gcvt(cell->h_core,10,str3);
		*str+=",";
		*str+=_gcvt(cell->gap,10,str3);
		*str+=",";
		*str+=_itoa(cell->n_gap,str3,10);
		*str+=",";
		*str+=_gcvt(cell->width,10,str3);
		*str+=",";
		*str+=_gcvt(cell->height,10,str3);
		*str+=",";
		*str+=_gcvt(cell->length,10,str3);
		*str+=",";
		*str+=_gcvt(cell->wt_core,10,str3);
		*str+=",";
		*str+=_gcvt(cell->wt,10,str3);
		*str+=",";
		*str+=_gcvt(cell->l_JIRO,10,str3);
		*str+=",";
		*str+=_gcvt(cell->LR_dis,10,str3);
		*str+=",";
		*str+=_gcvt(cell->Side_dis,10,str3);
		*str+=",";
		*str+=_gcvt(cell->In_Coil_w,10,str3);
		*str+=",";
		*str+=_gcvt(cell->In_Coil_h,10,str3);
		*str+=",";
		//----条件①-1----------------------
		*str+=_gcvt(cell->max_C[0],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_C[0],10,str3);
		*str+=",";
		*str+=_gcvt(cell->max_B[0],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_B[0],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Iron[0],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC[0],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC[0],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P[0],10,str3);
		*str+=",";
		//----条件①-2----------------------
		*str+=_gcvt(cell->max_C[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_C[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->max_B[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_B[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Iron[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Q[1],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Lmin12,10,str3);
		*str+=",";
		//----条件②----------------------
		*str+=_gcvt(cell->max_C[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_C[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->max_B[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_B[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Iron[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Q[2],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Lmin2,10,str3);
		*str+=",";
		//----条件③----------------------
		*str+=_gcvt(cell->max_C[3],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_C[3],10,str3);
		*str+=",";
		*str+=_gcvt(cell->max_B[3],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_B[3],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Iron[3],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC[3],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC[3],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P[3],10,str3);
		*str+=",";
		//----条件④-1----------------------
		*str+=_gcvt(cell->max_C[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_C[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->max_B[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_B[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Iron[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P[4],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Q[4],10,str3);
		*str+=",";
		//----条件④-2----------------------
		*str+=_gcvt(cell->max_C[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_C[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->max_B[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_B[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Iron[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P[5],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Q[5],10,str3);
		*str+=",";
		//----条件④-3----------------------
		*str+=_gcvt(cell->max_C[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_C[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->max_B[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->min_B[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Iron[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->P[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Q[6],10,str3);
		*str+=",";
		*str+=_gcvt(cell->Qall4,10,str3);
		*str+=",";
		*str+=_gcvt(cell->Pmean4,10,str3);
		*str+=",";


		for(i=0;i<=20;i++){
			*str+=_gcvt(cell->LI[i],10,str3);
			*str+=",";
		}
		*str+="\r\n";

/*
		*str=_ultoa(cell->ID,(char *)(str2.GetString()),10);
		*str=str2.GetString();
		*str+=",";
		*str+=_itoa(cell->turn,(char *)(str2.GetString()),10);
		*str+=",";
		*str+=_gcvt(cell->w_wire,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->t_wire,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->l_coil,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->l_wire,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->w_core,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->h_core,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->gap,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_itoa(cell->n_gap,(char *)(str2.GetString()),10);
		*str+=",";
		*str+=_gcvt(cell->width,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->height,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->length,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->l_JIRO,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->LR_dis,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->Side_dis,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->In_Coil_w,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->In_Coil_h,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->max_C,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->min_C,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->max_B,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->min_B,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->P_Iron,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->P_Cu_DC,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->P_Cu_AC,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->P,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_max_C,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_min_C,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_max_B,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_min_B,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_P_Iron,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_P_Cu_DC,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_P_Cu_AC,10,(char *)(str2.GetString()));
		*str+=",";
		*str+=_gcvt(cell->sat_P,10,(char *)(str2.GetString()));
		*str+=",";

		for(i=0;i<=20;i++){
			*str+=_gcvt(cell->LI[i],10,(char *)(str2.GetString()));
			*str+=",";
		}
		*str+="\n";
*/	
		
		return 1;
	}


}
