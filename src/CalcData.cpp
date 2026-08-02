#include "StdAfx.h"
#include "calcdata.h"

CCalcData::CCalcData(unsigned long id,int tu,double w_w,double t_w,double l_coi,double l_w,double h_c,double w_c,double g,int n_g,double h,double w,double l,
					 double wt_c,double wAll,double l_J,double LR,double Side,double In_Ch,double In_Cw,
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

	int i;
	ID=id;	//計算のID(順番)	2011/1/12追記
	turn=tu;//ターン数
	w_wire=w_w;//巻き線幅 (mm)
	t_wire=t_w;//巻き線高さ(mm)
	l_coil=l_coi; //ｺｲﾙ全長(mm)		2011/1/5追記
	l_wire=l_w;	//巻き線全長さ(mm)	2011/1/5追記
	h_core=h_c;//コア高さ(mm)
	w_core=w_c;//コア幅(mm)
	gap=g;//GAP幅(mm)
	n_gap=n_g;//GAP数

	height=h;//高さ(mm)
	width=w;//幅(mm)
	length=l;//全長(mm)
	wt_core=wt_c;//コアのみ重量(kg)
	wt=wAll;//コア＋銅線重量(kg)
	l_JIRO=l_J;	//磁路長さ(mm)	2011/1/5追記

	//----2011.1.5追加-------
	LR_dis=LR;//L,Rコア間隔(mm)
	Side_dis=Side;	//サイドコア間隔(mm)
	In_Coil_h=In_Ch;	//ｺｲﾙ内側高さ(mm)
	In_Coil_w=In_Cw;	//ｺｲﾙ内側幅(mm)
	//----2011.1.5追加-------

	max_C[0]=CU11;
	min_C[0]=CL11;
	max_B[0]=BU11;
	min_B[0]=BL11;
	P_Iron[0]=P_I11;
	P_Cu_DC[0]=P_C_DC11;
	P_Cu_AC[0]=P_C_AC11;
	P[0]=Pall11;

	max_C[1]=CU12;
	min_C[1]=CL12;
	max_B[1]=BU12;
	min_B[1]=BL12;
	P_Iron[1]=P_I12;
	P_Cu_DC[1]=P_C_DC12;
	P_Cu_AC[1]=P_C_AC12;
	P[1]=Pall12;
	Q[1]=Q12;
	Lmin12=Lm12;

	max_C[2]=CU2;
	min_C[2]=CL2;
	max_B[2]=BU2;
	min_B[2]=BL2;
	P_Iron[2]=P_I2;
	P_Cu_DC[2]=P_C_DC2;
	P_Cu_AC[2]=P_C_AC2;
	P[2]=Pall2;
	Q[2]=Q2;
	Lmin2=Lmin;

	max_C[3]=CU3;
	min_C[3]=CL3;
	max_B[3]=BU3;
	min_B[3]=BL3;
	P_Iron[3]=P_I3;
	P_Cu_DC[3]=P_C_DC3;
	P_Cu_AC[3]=P_C_AC3;
	P[3]=Pall3;

	max_C[4]=CU41;
	min_C[4]=CL41;
	max_B[4]=BU41;
	min_B[4]=BL41;
	P_Iron[4]=P_I41;
	P_Cu_DC[4]=P_C_DC41;
	P_Cu_AC[4]=P_C_AC41;
	P[4]=Pall41;
	Q[4]=Q41;

	max_C[5]=CU42;
	min_C[5]=CL42;
	max_B[5]=BU42;
	min_B[5]=BL42;
	P_Iron[5]=P_I42;
	P_Cu_DC[5]=P_C_DC42;
	P_Cu_AC[5]=P_C_AC42;
	P[5]=Pall42;
	Q[5]=Q42;

	max_C[6]=CU43;
	min_C[6]=CL43;
	max_B[6]=BU43;
	min_B[6]=BL43;
	P_Iron[6]=P_I43;
	P_Cu_DC[6]=P_C_DC43;
	P_Cu_AC[6]=P_C_AC43;
	P[6]=Pall43;
	Q[6]=Q43;
	
	Qall4=Q4;
	Pmean4=P4;

//	max_C=C1;
//	min_C=C2;
//	max_B=B1;
//	min_B=B2;

//	P_Iron=P_I;//(W)
//	P_Cu_DC=P_C_DC;
//	P_Cu_AC=P_C_AC;	//交流銅損 2011/1/5追加
//	P=Pall;
//	sat_max_C=C3;//最大電流(A)
//	sat_min_C=C4;
//	sat_max_B=B3;//(T)
//	sat_min_B=B4;	//飽和保証動作条件時のminB 2011/1/5追記
	
//	sat_P_Iron=P_Is;	//2007.1.5追記
//	sat_P_Cu_DC=P_C_DCs;	//2007.1.5追記
//	sat_P_Cu_AC=P_C_ACs;	//2007.1.5追記
//	sat_P=Palls;	//2007.1.5追記
	for(i=0;i<=20;i++){
		LI[i]=li[i];
	}
}

CCalcData::~CCalcData(void)
{
}
