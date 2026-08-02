// CORECALC5XDoc.cpp : CCORECALC5XDoc クラスの実装
//

#include "stdafx.h"
#include "CORECALC5X.h"

#include "CORECALC5XDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCORECALC5XDoc

IMPLEMENT_DYNCREATE(CCORECALC5XDoc, CDocument)

BEGIN_MESSAGE_MAP(CCORECALC5XDoc, CDocument)
END_MESSAGE_MAP()


// CCORECALC5XDoc コンストラクション/デストラクション

CCORECALC5XDoc::CCORECALC5XDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CCORECALC5XDoc::~CCORECALC5XDoc()
{
}

BOOL CCORECALC5XDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}




// CCORECALC5XDoc シリアル化

void CCORECALC5XDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
		int i,j,k,l,m,n,p;
		int ia,ja,ka,la,ma,na;	//計算ループの上限値が入る

		ar.Flush();//11.1.16 バッファリセット
		p_file=ar.GetFile();
//		long Actual;	//ﾌｧｲﾙの現在のポイント
		CString str;	//ReadCSV用の入れ物
		double dbl;	//ReadCSV用の入れ物
		int number;	//ReadCSV用の入れ物
		int code;	//ReadCSVの戻り

		//------------結果を出力するファイル準備---------------
		CFile F_LOG;
		CFile F_wt;
		CFile F_loss;
		CFile F_lossB;//2011.3.15２つめのlossファイルのため追加
		str=p_file->GetFilePath();
		i=str.GetLength();//\0を含まない長さを返す
		OutP=str.Mid(0,i-4);
		str=str.Mid(0,i-4)+"_wt.csv";
		F_wt.Open(str.GetString(),CFile::modeCreate | CFile::modeWrite);
		i=str.GetLength();//\0を含まない長さを返す
		str=str.Mid(0,i-7)+"_loss.csv";
		F_loss.Open(str.GetString(),CFile::modeCreate | CFile::modeWrite);
		i=str.GetLength();//\0を含まない長さを返す
		str=str.Mid(0,i-9)+"_LOG.txt";
		F_LOG.Open(str.GetString(),CFile::modeCreate | CFile::modeWrite);

		//2011.3.15２つめのlossファイルのため追加
		i=str.GetLength();//\0を含まない長さを返す
		str=str.Mid(0,i-8)+"_lossB.csv";
		F_lossB.Open(str.GetString(),CFile::modeCreate | CFile::modeWrite);
		//ここまで2011.3.15２つめのlossファイルのため追加

		CArchive A_LOG(&F_LOG,CArchive::store);
		CArchive A_wt(&F_wt,CArchive::store);
		CArchive A_loss(&F_loss,CArchive::store);
		CArchive A_lossB(&F_lossB,CArchive::store);//2011.3.15２つめのlossファイルのため追加

		CTime t = CTime::GetCurrentTime();
		txtout=t.Format( "START : %H:%M:%S %A, %B %d, %Y \r\n" );
		UpdateAllViews(NULL);
//		UpdateWindow(NULL);

//		str+=",\r\n";
		A_LOG.WriteString(txtout.GetString());
		unsigned long ULdiv;//全計算数の1%が入る。
		percent=0;//計算したパーセントをリセット


		unsigned long UL1=0;//dataのindexが入る変数 これが吟味した形状の統一ナンバーになる

		app=(CCORECALC5XApp*)AfxGetApp();
		CString mat;//素材名称
		double a1,a2,a3,dens;
		double Hdat;//,Bdat;//Lの計算で戻ってきたHとBが入る。
		double Ilen,Glen;//鉄の磁路長とgapの磁路長さ
		double S;//鉄の断面積
		double L1,L2;//計算したLが入る。平均を算出するためのもの。
		int num_dat=0;
		int dum;
		CBHArray BH;	//BHｶｰﾌﾞが入る配列
		CCalcArray RT_wt,RT_loss,RT_lossB;//Resultの略のつもり 2011/3/15新たにRT_lossBを追加。２つのlossが判定できるようにした。
		CCriteria CR;//この中で30000個を宣言している。
		CParaData PA;//計算で振る設定をここで指示する。
		CRatingCondition RA;//運転定格

		CRatingPack *pRAP;//
		CCrDataPack *pCRP;//

		//----------------計算時に入れるテンポラリーの結果---------------
			int turn;
			double w_wire;//巻き線幅 (mm)
			double t_wire;//巻き線高さ(mm)
			double l_coil; //ｺｲﾙ全長(mm)		2011/1/5追記
			double l_wire;	//巻き線全長さ(mm)	2011/1/5追記
			double h_core;//コア高さ(mm)
			double w_core;//コア幅(mm)
			double gap;//GAP幅(mm)
			//	int n_gap;//GAP数

			double height;//高さ(mm)
			double width;//幅(mm)
			double length;//全長(mm)
			double wt_core;//コアのみ重量(kg)
			double wt;//コア＋銅線重量(kg)
			double l_JIRO;	//磁路長さ(mm)	2011/1/5追記
		
			//----2011.1.5追加-------
			double LR_dis;//L,Rコア間隔(mm)
			double Side_dis;	//サイドコア間隔(mm)
			double In_Coil_h;	//ｺｲﾙ内側高さ(mm)
			double In_Coil_w;	//ｺｲﾙ内側幅(mm)
			//----2011.1.5追加-------

			//----暫定処置 2011.2.27 5Xの条件に合わせて変更--------------
			double max_C[7];
			double min_C[7];
			double max_B[7];
			double min_B[7];

			double P_Iron[7];//(W)
			double P_Cu_DC[7];
			double P_Cu_AC[7];	//交流銅損 2011/1/5追加
			double P[7];
			//--以下2xに対して追加
			double Q[7];

			double Lmin12;//条件1-2の最小L
			double Lmin2;//条件2の最小L
			double Qall4;//条件4の放熱量
			double Pmean4;//条件4の平均損失

			//5x運転条件
			double Imean[7];													//⇒5xは暫定で配列にした		
			double frq[7];
			double time[7];
			double V1[7];
			double V2[7];

			double VL[7];//リアクトルの起電力を入れるテンポラリー変数　V2-V1	⇒5xは暫定で配列にした
			double Toff[7];//Toffの時間　単位sec								⇒5xは暫定で配列にした上の暫定処置参照

			//判定条件
			//----ここで用いない判定条件には思い切りゆるい値を入れておくこと！
			double P12;//ここに最大4つの制限を入れる。 P12は条件1-2での損失を意味する
			double Q12;//Q12は条件1-2での熱量
			double I12;
			double P2;
			double I2;
			double Lcond12;//2011.3.17追加　ゼロアンペアに近い①-2でのminLを規定するための変数
			double Lcond2;
			double Q2;

			int num,numB;//RT_loss,RT_lossBを判定する条件の番号
			num=1;//ここでは条件①-2を設定
			numB=2;//ここでは条件②を設定

			//----暫定処置 ここまで2011.2.27 5Xの条件に合わせて変更--------------


//			double sat_max_C;//最大電流(A)
//			double sat_min_C;
//			double sat_max_B;//(T)
//			double sat_min_B;	//飽和保証動作条件時のminB 2011/1/5追記
	
//			double sat_P_Iron;	//2007.1.5追記
//		    double sat_P_Cu_DC;	//2007.1.5追記
//			double sat_P_Cu_AC;	//2007.1.5追記
//			double sat_P;	//2007.1.5追記

			double LI[21];	//LI特性のL 0から20A刻みに400AまでのLを代入する
			double I[21];		//LI特性の電流設定　これは固定
			for(i=0;i<=20;i++){
				I[i]=20*i;		//0Aから20A刻みで400Aまで入れる
			}

			double R;//銅線の抵抗が入るテンポラリー変数
			double wt_coil;//ｺｲﾙ重量 kg
//			double VL;//リアクトルの起電力を入れるテンポラリー変数　V2-V1	⇒5xは暫定で配列にした上の暫定処置参照
			double delta=1;//差分計算に用いる電流の小領域　ここでは1Aに指定
//			double Iave;//Toff中央の電圧。リプル電流の水準値				⇒5xは暫定で配列にした上の暫定処置参照
//			double Toff;//Toffの時間　単位sec								⇒5xは暫定で配列にした上の暫定処置参照

			unsigned long SIZE;
			unsigned long HALF;
			unsigned long STATE; //この3つは並べ替えのためのテンポラリー数値

		//----------------計算時に入れるテンポラリーの結果終了-----------

		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="素材名")){return;}
		code=ReadCSV(p_file,dbl,str,number);
		if((code&0X07)!=0X05){return;}
		mat=str;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="比重")){return;}
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		dens=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="H")){return;}
		for(i=0;;i++){
			if(!(code=ReadCSV(p_file,dbl,str,number))){return;}
				dum=code&0x0f;
				if((dum==0x03)||(dum==0x01)||(dum==0x09)||(dum==0x0b)){
					BH.AddData(dbl,0);
				}else if((dum==0X05)&&(str=="B")){break;
				}else{return;}
				dum=code&0xf0;
				if(dum){return;}//ﾌｧｲﾙ終了がonだとここで終りにしてしまう。
		}
		for(j=0;j<i;j++){
			if(!(code=ReadCSV(p_file,dbl,str,number))){return;}
				dum=code&0x0f;
				if((dum==0x03)||(dum==0x01)||(dum==0x09)||(dum==0x0b)){
					BH.SetB(j,dbl);
	//			}else if((dum==0X05)&&(str=="A1")){break;
				}else{return;}
				dum=code&0xf0;
				if(dum){return;}//ﾌｧｲﾙ終了がonだとここで終りにしてしまう。
		}
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="A1")){return;}
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		a1=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="A2")){return;}
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		a2=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="A3")){return;}
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		a3=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="GAP数")){return;}//整数ならok
		code=ReadCSV(p_file,dbl,str,number);
		if(!((code&0X07)==0x01)){return;}
		PA.n_gap=number;

		//---------計算条件設定----------------------
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="パラメータ")){return;}
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="gap")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.min_gap=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.max_gap=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.div_gap=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="ターン数")){return;}//整数ならok
		code=ReadCSV(p_file,dbl,str,number);
		if(!((code&0X07)==0x01)){return;}
		PA.min_turn=number;
		code=ReadCSV(p_file,dbl,str,number);
		if(!((code&0X07)==0x01)){return;}
		PA.max_turn=number;
		code=ReadCSV(p_file,dbl,str,number);
		if(!((code&0X07)==0x01)){return;}
		PA.div_turn=number;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="鉄縦")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.min_h_core=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.max_h_core=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.div_h_core=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="鉄横")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.min_w_core=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.max_w_core=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.div_w_core=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="銅縦")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.min_t_wire=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.max_t_wire=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.div_t_wire=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="銅横")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.min_w_wire=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.max_w_wire=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		PA.div_w_wire=dbl;
		//---------計算条件設定終了------------------

		//---------閾値設定--------------------------
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="しきい値")){return;}
//		code=ReadCSV(p_file,dbl,str,number);
//		if(((code&0X07)!=0X05)||(str!="損失")){return;}//W
//		code=ReadCSV(p_file,dbl,str,number);
//		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
//		CR.c_max_P=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="wt")){return;}//kg
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		CR.c_max_wt=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="全長")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		CR.max_len=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="横幅")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		CR.max_w=dbl;
		code=ReadCSV(p_file,dbl,str,number);
		if(((code&0X07)!=0X05)||(str!="高さ")){return;}//mm
		code=ReadCSV(p_file,dbl,str,number);
		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
		CR.max_h=dbl;
//		code=ReadCSV(p_file,dbl,str,number);
//		if(((code&0X07)!=0X05)||(str!="最大電流")){return;}//A
//		code=ReadCSV(p_file,dbl,str,number);
//		if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
//		CR.max_cur=dbl;
		//---------閾値設定いったん終了----------------------

		//---------運転条件&しきい値設定----------------------
		code=ReadCSV(p_file,dbl,str,number);//冒頭の文字列を先ず入力
		while(1){
			if(((code&0X07)!=0X05)||(str.Mid(0,4)!="条件")){return;}
			code=ReadCSV(p_file,dbl,str,number);
			RA.m_RaPackArray.Add(new CRatingPack());
			CR.m_CrPackArray.Add(new CCrDataPack());
			i=(int)RA.m_RaPackArray.GetSize();
			pRAP=(CRatingPack *)RA.m_RaPackArray.GetAt(i-1);
			pCRP=(CCrDataPack *)CR.m_CrPackArray.GetAt(i-1);
//条件設定にあたっての注意　CSV中でV1はゼロにしないこと。
			if(((code&0X07)!=0X05)||(str!="V1")){return;}//ここからV1よみこみと複合dataの入れ物準備
			j=0;
			while(1){
				code=ReadCSV(p_file,dbl,str,number);
				if(((code&0X07)==0X03)||((code&0X07)==0x01)){
					pRAP->m_RaDataArray.Add(new CRatingData());
					pCRP->m_CrDataArray.Add(new CCriData());
//					((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(i)))->m_RaDataArray.GetAt(j)))->V1=dbl;
					((CRatingData *)(pRAP->m_RaDataArray.GetAt(j)))->V1=dbl;
					j++;
				}else if((code&0x07)==0x05){
					break;//次のストリングの先読み
				}else{
					return;//これは不正フォーマットになるので、シリアライズ関数終了
				}
			}
			//iには読んだ条件の個数が入っている
//			code=ReadCSV(p_file,dbl,str,number);
			if(((code&0X07)!=0X05)||(str!="V2")){return;}
			for(k=0;k<j;k++){
				code=ReadCSV(p_file,dbl,str,number);
				if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
				((CRatingData *)(pRAP->m_RaDataArray.GetAt(k)))->V2=dbl;
			}
			code=ReadCSV(p_file,dbl,str,number);
			if(((code&0X07)!=0X05)||(str!="I1")){return;}
			for(k=0;k<j;k++){
				code=ReadCSV(p_file,dbl,str,number);
				if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
				((CRatingData *)(pRAP->m_RaDataArray.GetAt(k)))->Imean=dbl;
			}
			code=ReadCSV(p_file,dbl,str,number);
			if(((code&0X07)!=0X05)||(str!="f(KHz)")){return;}
			for(k=0;k<j;k++){
				code=ReadCSV(p_file,dbl,str,number);
				if(!(((code&0X07)==0X03)||((code&0X07)==0x01))){return;}
				((CRatingData *)(pRAP->m_RaDataArray.GetAt(k)))->frq=dbl;
			}
			code=ReadCSV(p_file,dbl,str,number);
			if(((code&0X07)!=0X05)||(str!="t")){return;}
			k=0;l=0;
			while(1){
				code=ReadCSV(p_file,dbl,str,number);
				if(((code&0X07)==0X03)||((code&0X07)==0x01)){//整数もしくは実数
					k+=(code&0xff00)/256;				
					((CRatingData *)(pRAP->m_RaDataArray.GetAt(k)))->time=dbl;
					if(k>l){pCRP->flg_time=0;}//空白の条件あったので時間定格無し
					k++;
					l=k;
				}else if((code&0x07)==0x05){
					break;//次のストリングの先読み
				}else{
					return;//これは不正フォーマットになるので、シリアライズ関数終了
				}
			}
			if(k<j){pCRP->flg_time=0;}//最後の１項目が空白だったので、時間定格無し jには複合ﾓｰﾄﾞの個数が入っている

			if(((code&0X07)!=0X05)||(str!="P")){return;}
			k=0;
			while(1){
				code=ReadCSV(p_file,dbl,str,number);
				if(((code&0X07)==0X03)||((code&0X07)==0x01)){//整数もしくは実数
					k+=(code&0xff00)/256;				
					CR.flg_cr[i]=1;
					((CCriData *)(pCRP->m_CrDataArray.GetAt(k)))->maxP=dbl;
					k++;
				}else if((code&0x07)==0x05){
					break;//次のストリングの先読み
				}else{
					return;//これは不正フォーマットになるので、シリアライズ関数終了
				}
			}

			if(((code&0X07)!=0X05)||(str!="Q")){return;}
			k=0;
			while(1){
				code=ReadCSV(p_file,dbl,str,number);
				if(((code&0X07)==0X03)||((code&0X07)==0x01)){//整数もしくは実数
					k+=(code&0xff00)/256;				
					CR.flg_cr[i]=1;
					((CCriData *)(pCRP->m_CrDataArray.GetAt(k)))->maxQ=dbl;
					k++;
				}else if((code&0x07)==0x05){
					break;//次のストリングの先読み
				}else{
					return;//これは不正フォーマットになるので、シリアライズ関数終了
				}
			}

			if(((code&0X07)!=0X05)||(str!="Lmin")){return;}
			k=0;
			while(1){
				code=ReadCSV(p_file,dbl,str,number);
				if(((code&0X07)==0X03)||((code&0X07)==0x01)){//整数もしくは実数
					k+=(code&0xff00)/256;				
					CR.flg_cr[i]=1;
					((CCriData *)(pCRP->m_CrDataArray.GetAt(k)))->minL=dbl;
					k++;
				}else if((code&0x07)==0x05){
					break;//次のストリングの先読み
				}else{
					return;//これは不正フォーマットになるので、シリアライズ関数終了
				}
			}

			if(((code&0X07)!=0X05)||(str!="Imax")){return;}
			k=0;
			while(1){
				code=ReadCSV(p_file,dbl,str,number);
				if(((code&0X07)==0X03)||((code&0X07)==0x01)){//整数もしくは実数
					k+=(code&0xff00)/256;				
					CR.flg_cr[i]=1;
					((CCriData *)(pCRP->m_CrDataArray.GetAt(k)))->maxC=dbl;
					k++;
				}else if((code&0x07)==0x05){
					break;//次のストリングの先読み
				}else{
					return;//これは不正フォーマットになるので、シリアライズ関数終了
				}
			}

			if(((code&0X07)!=0X05)||(str!="Qtotal")){return;}
			code=ReadCSV(p_file,dbl,str,number);
			if(((code&0X07)==0X03)||((code&0X07)==0x01)){//整数もしくは実数
				CR.flg_cr[i]=1;
				((CCrDataPack *)(pCRP))->Qtotal=dbl;
				code=ReadCSV(p_file,dbl,str,number);//次のストリング先読み
			}
			if(((code&0X07)!=0X05)||(str!="Pmean")){return;}
			code=ReadCSV(p_file,dbl,str,number);
			if(((code&0X07)==0X03)||((code&0X07)==0x01)){//整数もしくは実数
				CR.flg_cr[i]=1;
				((CCrDataPack *)(pCRP))->Pmean=dbl;//次のストリング先読み
				code=ReadCSV(p_file,dbl,str,number);
			}
			if(((code&0X07)!=0X05)){break;}

		}
		//---------CSVからの読み取り終了-------------
		//---------以下ここで指定する定数　変更する場合はここをいじること-------------
		PA.c_w_cl=2.8;		//コア～銅線間のcl
		PA.ctr_cl=2.7;		//ｺｲﾙ中央のクリアランス
		PA.side_cl=3.11;			//ｺｲﾙ端部からU字コアまでのCL 片側で指定
		PA.t_w_coat=0.0625;		//巻き線被覆厚さ
		PA.temp_wire=150;	//銅損計算する上でのワイヤ温度の設定
		//---------ここで指定する定数の設定終了---------------------------------------
		//---------運転条件設定終了------------------
//--------------暫定の処置 しきい値を5xのものでここで固定で設定する------------------------------
//将来的には、この行までの前段で読み込んだしきい値をもとに以下のループを廻すように改造すること
//----------2011.2.27----------------------------------------------------------------------------
		Imean[0]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(0)))->Imean;
		Imean[1]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(1)))->Imean;
		Imean[2]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(1)))->m_RaDataArray.GetAt(0)))->Imean;
		Imean[3]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(2)))->m_RaDataArray.GetAt(0)))->Imean;
		Imean[4]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(0)))->Imean;
		Imean[5]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(1)))->Imean;
		Imean[6]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(2)))->Imean;
		frq[0]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(0)))->frq;
		frq[1]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(1)))->frq;
		frq[2]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(1)))->m_RaDataArray.GetAt(0)))->frq;
		frq[3]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(2)))->m_RaDataArray.GetAt(0)))->frq;
		frq[4]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(0)))->frq;
		frq[5]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(1)))->frq;
		frq[6]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(2)))->frq;
		V1[0]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(0)))->V1;
		V1[1]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(1)))->V1;
		V1[2]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(1)))->m_RaDataArray.GetAt(0)))->V1;
		V1[3]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(2)))->m_RaDataArray.GetAt(0)))->V1;
		V1[4]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(0)))->V1;
		V1[5]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(1)))->V1;
		V1[6]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(2)))->V1;
		V2[0]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(0)))->V2;
		V2[1]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(1)))->V2;
		V2[2]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(1)))->m_RaDataArray.GetAt(0)))->V2;
		V2[3]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(2)))->m_RaDataArray.GetAt(0)))->V2;
		V2[4]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(0)))->V2;
		V2[5]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(1)))->V2;
		V2[6]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(2)))->V2;
		time[0]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(0)))->time;
		time[1]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(0)))->m_RaDataArray.GetAt(1)))->time;
		time[2]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(1)))->m_RaDataArray.GetAt(0)))->time;
		time[3]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(2)))->m_RaDataArray.GetAt(0)))->time;
		time[4]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(0)))->time;
		time[5]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(1)))->time;
		time[6]=((CRatingData *)(((CRatingPack *)(RA.m_RaPackArray.GetAt(3)))->m_RaDataArray.GetAt(2)))->time;
		for(i=0;i<7;i++){
			VL[i]=V2[i]-V1[i];
			if(VL[i]==0){Toff[i]=0;continue;}//ちょっと釈然としない 2011.3.17
			if(frq[i]==0){Toff[i]=0;continue;}//これもちょっと釈然としない 2011.3.17
			Toff[i]=1/(frq[i]*1000)*V1[i]/V2[i];
		}
		P12=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(0)))->m_CrDataArray.GetAt(1)))->maxP;
		Q12=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(0)))->m_CrDataArray.GetAt(1)))->maxQ;
		I12=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(0)))->m_CrDataArray.GetAt(1)))->maxC;
		Lcond12=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(0)))->m_CrDataArray.GetAt(1)))->minL;
		P2=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(1)))->m_CrDataArray.GetAt(0)))->maxP;
		I2=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(1)))->m_CrDataArray.GetAt(0)))->maxC;
		Lcond2=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(1)))->m_CrDataArray.GetAt(0)))->minL;
		Q2=((CCriData *)(((CCrDataPack *)(CR.m_CrPackArray.GetAt(1)))->m_CrDataArray.GetAt(0)))->maxQ;


//---------計算実行----------------------
		//----ループ上限の計算-------------------
		ia=int((PA.max_h_core-PA.min_h_core)/PA.div_h_core+0.5)+1;
		ja=int((PA.max_w_core-PA.min_w_core)/PA.div_w_core+0.5)+1;
		ka=int((PA.max_t_wire-PA.min_t_wire)/PA.div_t_wire+0.5)+1;
		la=int((PA.max_w_wire-PA.min_w_wire)/PA.div_w_wire+0.5)+1;
		ma=int((PA.max_turn-PA.min_turn)/PA.div_turn+0.5)+1;
		na=int((PA.max_gap-PA.min_gap)/PA.div_gap+0.5)+1;
		ULdiv=ia*ja*ka*la*ma*na/100;//1%の計算数を算出
		//----計算loop---------------------------
		//for(i=0;i<ia+1;i++)←この表現では毎回ia+1の足し算が生じるので、うえのiaの定義で+1にすることとした。
		for(i=0;i<ia;i++){//コア高さ
			h_core=PA.min_h_core+PA.div_h_core*i;
			for(j=0;j<ja;j++){//コア幅
				w_core=PA.min_w_core+PA.div_w_core*j;
				S=h_core*w_core;
				for(k=0;k<ka;k++){//巻き線厚み
					t_wire=PA.min_t_wire+PA.div_t_wire*k;
					In_Coil_w=w_core+PA.c_w_cl*2;
					width=(In_Coil_w+t_wire*2)*2+PA.ctr_cl+PA.t_w_coat*2;
					if(width>CR.max_w){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}UL1+=(ka-k)*la*ma*na-1;break;}//幅ｵｰﾊﾞｰ
					In_Coil_h=h_core+PA.c_w_cl*2;
					height=In_Coil_h+t_wire*2+PA.t_w_coat*2;
					if(height>CR.max_h){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}UL1+=(ka-k)*la*ma*na-1;break;}//高さｵｰﾊﾞｰ
					LR_dis=PA.c_w_cl*2+t_wire*2+PA.ctr_cl;//LRコア間隔
					for(l=0;l<la;l++){//巻き線厚み
						w_wire=PA.min_w_wire+PA.div_w_wire*l;
						for(m=0;m<ma;m++){//ターン数
							turn=PA.min_turn+PA.div_turn*m;
							l_wire=((In_Coil_h+t_wire)*2+(In_Coil_w+t_wire)*2)*turn;
							R=1.72*(1+(PA.temp_wire-20)*4.33/1000)*pow(10,-8)*l_wire/1000/(t_wire*w_wire/1000000);
//							P_Cu_DC=R*RA.cont_current*RA.cont_current;
//							if(P_Cu_DC>=CR.c_max_P){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
							l_coil=(w_wire+PA.t_w_coat*2)*(turn/2+1+1);//最後のプラス１は、初回試作の実測にあわせるために作った。
							Side_dis=l_coil+PA.side_cl*2;
							length=Side_dis+w_core*2;
							if(length>CR.max_len){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}UL1+=(ma-m)*na-1;break;}//全長ｵｰﾊﾞｰ
							wt_coil=l_wire/10*w_wire*t_wire/100*8.93/1000;//巻き線重量 kg
							l_JIRO=Side_dis*2+LR_dis*2+3.14159265358979*2*w_core/2;
							for(n=0;n<na;n++){//gap厚み mm
								gap=PA.min_gap+PA.div_gap*n;//gap 1箇所あたりの厚みのこと
								Glen=gap*PA.n_gap;//gap全長（磁路）
								Ilen=l_JIRO-Glen;//鉄の磁路
								wt_core=(3.14159265358979*w_core*w_core/100+(Side_dis*2-Glen)/10*w_core/10+LR_dis*2/10*w_core/10)*h_core/10*dens/1000;//コア重量 kg
								wt=wt_core+wt_coil;//全重量 kg
								if(wt>CR.c_max_wt){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);};continue;}//重量ｵｰﾊﾞｰ
								//-------条件1-2の損失計算-----------------------
								P_Cu_DC[1]=R*Imean[1]*Imean[1];
								if(P_Cu_DC[1]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
//								VL=V2[1]-V1[1];
//								Toff=1/(frq[1]*1000)*V1[1]/V2[1];
								max_C[1]=0;
								code=1;
								for(p=0;;p++){
									if(Imean[1]+p*delta>I12){code=0;break;}
									if(!(Cal_L(L1,Imean[1]+p*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									if(!(Cal_L(L2,Imean[1]+(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									max_C[1]+=((L1+L2)/2*delta);
									if(max_C[1]>=VL[1]*Toff[1]/2){max_C[1]=Imean[1]+p*delta;break;}//ここでmax_Cを最終的に入れ直している。
								}
								if((!(Cal_BandH(max_C[1],Glen,turn,Ilen,BH,Hdat,max_B[1])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								min_C[1]=0;
								code=1;
								for(p=0;;p++){
									if(Imean[1]-p*delta<-1*I12){code=0;break;}
									if(!(Cal_L(L1,Imean[1]-p*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									if(!(Cal_L(L2,Imean[1]-(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									min_C[1]+=((L1+L2)/2*delta);
									if(min_C[1]>=VL[1]*Toff[1]/2){min_C[1]=Imean[1]-p*delta;break;}
								}
								if((!(Cal_BandH(min_C[1],Glen,turn,Ilen,BH,Hdat,min_B[1])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								P_Cu_AC[1]=R*(max_C[1]-min_C[1])*(max_C[1]-min_C[1])/12;
								P_Iron[1]=wt_core*(a1*frq[1]*1000*pow((max_B[1]-min_B[1])/2,2)+a2*pow(frq[1]*1000*(max_B[1]-min_B[1])/2,2)+a3*pow(frq[1]*1000*(max_B[1]-min_B[1])/2,1.5));
								P[1]=P_Cu_DC[1]+P_Cu_AC[1]+P_Iron[1];
								if(max_C[1]>I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流maxｵｰﾊﾞｰ
								if(min_C[1]<-1*I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流minｵｰﾊﾞｰ
								if(P[1]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全損失ｵｰﾊﾞｰ
								Q[1]=P[1]*time[1];
								if(Q[1]>=Q12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全発熱量ｵｰﾊﾞｰ
								if(!(Cal_L(Lmin12,max_C[1],delta,S,Glen,turn,Ilen,BH))){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								if(Lmin12<Lcond12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//リプルピーク電流時 minL確保できず。
								//-------条件2の損失とLでの判定-----------------------
								P_Cu_DC[2]=R*Imean[2]*Imean[2];
								if(P_Cu_DC[2]>=P2){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
//								VL=V2[2]-V1[2];
//								Toff=1/(frq[2]*1000)*V1[2]/V2[2];
								max_C[2]=0;
								code=1;
								for(p=0;;p++){
									if(Imean[2]+p*delta>I2){code=0;break;}
									if(!(Cal_L(L1,Imean[2]+p*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									if(!(Cal_L(L2,Imean[2]+(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									max_C[2]+=((L1+L2)/2*delta);
									if(max_C[2]>=VL[2]*Toff[2]/2){max_C[2]=Imean[2]+p*delta;break;}
								}
								//このループでsat_current+p*deltaがmax_curを超えるので、ここでcode=0のためbreakとなってしまう 11.2.8
								if((!(Cal_BandH(max_C[2],Glen,turn,Ilen,BH,Hdat,max_B[2])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								min_C[2]=0;
								code=1;
								for(p=0;;p++){
									if(Imean[2]-p*delta<-1*I2){code=0;break;}
									if(!(Cal_L(L1,Imean[2]-p*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									if(!(Cal_L(L2,Imean[2]-(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									min_C[2]+=((L1+L2)/2*delta);
									if(min_C[2]>=VL[2]*Toff[2]/2){min_C[2]=Imean[2]-p*delta;break;}
								}
								if((!(Cal_BandH(min_C[2],Glen,turn,Ilen,BH,Hdat,min_B[2])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								P_Cu_AC[2]=R*(max_C[2]-min_C[2])*(max_C[2]-min_C[2])/12;
								P_Iron[2]=wt_core*(a1*frq[2]*1000*pow((max_B[2]-min_B[2])/2,2)+a2*pow(frq[2]*1000*(max_B[2]-min_B[2])/2,2)+a3*pow(frq[2]*1000*(max_B[2]-min_B[2])/2,1.5));
								P[2]=P_Cu_DC[2]+P_Cu_AC[2]+P_Iron[2];
								if(max_C[2]>I2){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流maxｵｰﾊﾞｰ
								if(min_C[2]<-1*I2){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流minｵｰﾊﾞｰ
								Q[2]=P[2]*time[2];
								if(Q[2]>=Q2){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全発熱量ｵｰﾊﾞｰ
								if(!(Cal_L(Lmin2,max_C[2],delta,S,Glen,turn,Ilen,BH))){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								if(Lmin2<Lcond2){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//リプルピーク電流時 minL確保できず。
					//--------------ここから判定に使わない条件の計算①-1,③,④-1～3-------------------------------------------
								//-----①-1------------------------------------------------------------------
								P_Cu_DC[0]=R*Imean[0]*Imean[0];
//								if(P_Cu_DC[0]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
//								VL=V2[0]-V1[0];
//								Toff=1/(frq[0]*1000)*V1[0]/V2[0];
								max_C[0]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[0]+p*delta>I12){code=0;break;}
									if(!(Cal_L(L1,Imean[0]+p*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									if(!(Cal_L(L2,Imean[0]+(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									max_C[0]+=((L1+L2)/2*delta);
									if(max_C[0]>=VL[0]*Toff[0]/2){max_C[0]=Imean[0]+p*delta;break;}
								}
								if((!(Cal_BandH(max_C[0],Glen,turn,Ilen,BH,Hdat,max_B[0])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								min_C[0]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[1]-p*delta<-1*I12){code=0;break;}
									if(!(Cal_L(L1,Imean[0]-p*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									if(!(Cal_L(L2,Imean[0]-(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									min_C[0]+=((L1+L2)/2*delta);
									if(min_C[0]>=VL[0]*Toff[0]/2){min_C[0]=Imean[0]-p*delta;break;}
								}
								if((!(Cal_BandH(min_C[0],Glen,turn,Ilen,BH,Hdat,min_B[0])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								P_Cu_AC[0]=R*(max_C[0]-min_C[0])*(max_C[0]-min_C[0])/12;
								P_Iron[0]=wt_core*(a1*frq[0]*1000*pow((max_B[0]-min_B[0])/2,2)+a2*pow(frq[0]*1000*(max_B[0]-min_B[0])/2,2)+a3*pow(frq[0]*1000*(max_B[0]-min_B[0])/2,1.5));
								P[0]=P_Cu_DC[0]+P_Cu_AC[0]+P_Iron[0];
//								if(max_C[1]>I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流maxｵｰﾊﾞｰ
//								if(min_C[1]<-1*I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流minｵｰﾊﾞｰ
//								if(P[1]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全損失ｵｰﾊﾞｰ
//								Q[1]=P[1]*time[1];
//								if(Q[1]>=Q12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全発熱量ｵｰﾊﾞｰ
								//-----③------------------------------------------------------------------
								P_Cu_DC[3]=R*Imean[3]*Imean[3];
//								if(P_Cu_DC[0]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
//								VL=V2[3]-V1[3];
//								Toff=1/(frq[3]*1000)*V1[3]/V2[3];
								max_C[3]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[0]+p*delta>I12){code=0;break;}
									if(!(Cal_L(L1,Imean[3]+p*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									if(!(Cal_L(L2,Imean[3]+(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									max_C[3]+=((L1+L2)/2*delta);
									if(max_C[3]>=VL[3]*Toff[3]/2){max_C[3]=Imean[3]+p*delta;break;}
								}
								if((!(Cal_BandH(max_C[3],Glen,turn,Ilen,BH,Hdat,max_B[3])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								min_C[3]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[1]-p*delta<-1*I12){code=0;break;}
									if(!(Cal_L(L1,Imean[3]-p*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									if(!(Cal_L(L2,Imean[3]-(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									min_C[3]+=((L1+L2)/2*delta);
									if(min_C[3]>=VL[3]*Toff[3]/2){min_C[3]=Imean[3]-p*delta;break;}
								}
								if((!(Cal_BandH(min_C[3],Glen,turn,Ilen,BH,Hdat,min_B[3])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								P_Cu_AC[3]=R*(max_C[3]-min_C[3])*(max_C[3]-min_C[3])/12;
								P_Iron[3]=wt_core*(a1*frq[3]*1000*pow((max_B[3]-min_B[3])/2,2)+a2*pow(frq[3]*1000*(max_B[3]-min_B[3])/2,2)+a3*pow(frq[3]*1000*(max_B[3]-min_B[3])/2,1.5));
								P[3]=P_Cu_DC[3]+P_Cu_AC[3]+P_Iron[3];
//								if(max_C[1]>I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流maxｵｰﾊﾞｰ
//								if(min_C[1]<-1*I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流minｵｰﾊﾞｰ
//								if(P[1]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全損失ｵｰﾊﾞｰ
//								Q[1]=P[1]*time[1];
//								if(Q[1]>=Q12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全発熱量ｵｰﾊﾞｰ
								//-----④-1------------------------------------------------------------------
								P_Cu_DC[4]=R*Imean[4]*Imean[4];
//								if(P_Cu_DC[0]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
//								VL=V2[4]-V1[4];
//								Toff=1/(frq[4]*1000)*V1[4]/V2[4];
								max_C[4]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[0]+p*delta>I12){code=0;break;}
									if(!(Cal_L(L1,Imean[4]+p*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									if(!(Cal_L(L2,Imean[4]+(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									max_C[4]+=((L1+L2)/2*delta);
									if(max_C[4]>=VL[4]*Toff[4]/2){max_C[4]=Imean[4]+p*delta;break;}
								}
								if((!(Cal_BandH(max_C[4],Glen,turn,Ilen,BH,Hdat,max_B[4])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								min_C[4]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[1]-p*delta<-1*I12){code=0;break;}
									if(!(Cal_L(L1,Imean[4]-p*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									if(!(Cal_L(L2,Imean[4]-(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									min_C[4]+=((L1+L2)/2*delta);
									if(min_C[4]>=VL[4]*Toff[4]/2){min_C[4]=Imean[4]-p*delta;break;}
								}
								if((!(Cal_BandH(min_C[4],Glen,turn,Ilen,BH,Hdat,min_B[4])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								P_Cu_AC[4]=R*(max_C[4]-min_C[4])*(max_C[4]-min_C[4])/12;
								P_Iron[4]=wt_core*(a1*frq[4]*1000*pow((max_B[4]-min_B[4])/2,2)+a2*pow(frq[4]*1000*(max_B[4]-min_B[4])/2,2)+a3*pow(frq[4]*1000*(max_B[4]-min_B[4])/2,1.5));
								P[4]=P_Cu_DC[4]+P_Cu_AC[4]+P_Iron[4];
//								if(max_C[1]>I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流maxｵｰﾊﾞｰ
//								if(min_C[1]<-1*I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流minｵｰﾊﾞｰ
//								if(P[1]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全損失ｵｰﾊﾞｰ
								Q[4]=P[4]*time[4];
//								if(Q[1]>=Q12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全発熱量ｵｰﾊﾞｰ
								//-----④-2------------------------------------------------------------------
								P_Cu_DC[5]=R*Imean[5]*Imean[5];
//								if(P_Cu_DC[0]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
//								VL=V2[5]-V1[5];
//								Toff=1/(frq[5]*1000)*V1[5]/V2[5];
								max_C[5]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[0]+p*delta>I12){code=0;break;}
									if(!(Cal_L(L1,Imean[5]+p*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									if(!(Cal_L(L2,Imean[5]+(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									max_C[5]+=((L1+L2)/2*delta);
									if(max_C[5]>=VL[5]*Toff[5]/2){max_C[5]=Imean[5]+p*delta;break;}
								}
								if((!(Cal_BandH(max_C[5],Glen,turn,Ilen,BH,Hdat,max_B[5])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								min_C[5]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[1]-p*delta<-1*I12){code=0;break;}
									if(!(Cal_L(L1,Imean[5]-p*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									if(!(Cal_L(L2,Imean[5]-(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									min_C[5]+=((L1+L2)/2*delta);
									if(min_C[5]>=VL[5]*Toff[5]/2){min_C[5]=Imean[5]-p*delta;break;}
								}
								if((!(Cal_BandH(min_C[5],Glen,turn,Ilen,BH,Hdat,min_B[5])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								P_Cu_AC[5]=R*(max_C[5]-min_C[5])*(max_C[5]-min_C[5])/12;
								P_Iron[5]=wt_core*(a1*frq[5]*1000*pow((max_B[5]-min_B[5])/2,2)+a2*pow(frq[5]*1000*(max_B[5]-min_B[5])/2,2)+a3*pow(frq[5]*1000*(max_B[5]-min_B[5])/2,1.5));
								P[5]=P_Cu_DC[5]+P_Cu_AC[5]+P_Iron[5];
//								if(max_C[1]>I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流maxｵｰﾊﾞｰ
//								if(min_C[1]<-1*I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流minｵｰﾊﾞｰ
//								if(P[1]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全損失ｵｰﾊﾞｰ
								Q[5]=P[5]*time[5];
//								if(Q[1]>=Q12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全発熱量ｵｰﾊﾞｰ
								//-----④-3------------------------------------------------------------------
								P_Cu_DC[6]=R*Imean[6]*Imean[6];
//								if(P_Cu_DC[0]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//銅損のみで損失ｵｰﾊﾞｰ
//								VL=V2[6]-V1[6];
//								Toff=1/(frq[6]*1000)*V1[6]/V2[6];
								max_C[6]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[0]+p*delta>I12){code=0;break;}
									if(!(Cal_L(L1,Imean[6]+p*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									if(!(Cal_L(L2,Imean[6]+(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){
										code=0;break;}
									max_C[6]+=((L1+L2)/2*delta);
									if(max_C[6]>=VL[6]*Toff[6]/2){max_C[6]=Imean[6]+p*delta;break;}
								}
								if((!(Cal_BandH(max_C[6],Glen,turn,Ilen,BH,Hdat,max_B[6])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								min_C[6]=0;
								code=1;
								for(p=0;;p++){
//									if(Imean[1]-p*delta<-1*I12){code=0;break;}
									if(!(Cal_L(L1,Imean[6]-p*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									if(!(Cal_L(L2,Imean[6]-(p+1)*delta,delta,S,Glen,turn,Ilen,BH))){code=0;break;}
									min_C[6]+=((L1+L2)/2*delta);
									if(min_C[6]>=VL[6]*Toff[6]/2){min_C[6]=Imean[6]-p*delta;break;}
								}
								if((!(Cal_BandH(min_C[6],Glen,turn,Ilen,BH,Hdat,min_B[6])))||(!code)){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}
								P_Cu_AC[6]=R*(max_C[6]-min_C[6])*(max_C[6]-min_C[6])/12;
								P_Iron[6]=wt_core*(a1*frq[6]*1000*pow((max_B[6]-min_B[6])/2,2)+a2*pow(frq[6]*1000*(max_B[6]-min_B[6])/2,2)+a3*pow(frq[6]*1000*(max_B[6]-min_B[6])/2,1.5));
								P[6]=P_Cu_DC[6]+P_Cu_AC[6]+P_Iron[6];
//								if(max_C[1]>I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流maxｵｰﾊﾞｰ
//								if(min_C[1]<-1*I12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//電流minｵｰﾊﾞｰ
//								if(P[1]>=P12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全損失ｵｰﾊﾞｰ
								Q[6]=P[6]*time[6];
//								if(Q[1]>=Q12){UL1++;if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);}continue;}//全発熱量ｵｰﾊﾞｰ
								Qall4=Q[4]+Q[5]+Q[6];
								Pmean4=Qall4/(time[4]+time[5]+time[6]);

					//--------------ここから判定に使わない条件の計算終了 ①-1,③,④-1～3-------------------------------------------
								//-----ここからはLI特性------------------------------
								for(p=0;p<=20;p++){
									LI[p]=0;
								}
								for(p=0;p<=20;p++){
									if(!(Cal_L(LI[p],I[p],delta,S,Glen,turn,Ilen,BH))){break;}
								}
								//-----1つの事例がやっとここで終了 UL1をインクリメントして、所定のパーセントに到達したならlogを出力-------------
								UL1++;
								if(UL1>=ULdiv*percent){percent++;out_percent(A_LOG);;}
								//--------------wt判定し、dataset-----------------------------------
								SIZE=RT_wt.GetSize();
								if(!(SIZE)){
									RT_wt.AddCalc(UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
										l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
										max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
										max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
										max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
										max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
										max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
										max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
										max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
										Qall4,Pmean4,LI);
								}else if(SIZE>=(unsigned long)(CR.num_data)){//個数が最大値に達した
									if (wt>RT_wt.Get_wt(SIZE-1)){//最大値より大きければなにもせず。捨てられる。
										//ここはなにも記載せず
									}else if(wt<RT_wt.Get_wt(0)){//最小値より小さければ、0のindexに入れる
										RT_wt.AddCalc(0,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
										RT_wt.RemoveData(SIZE);
									}else{
										HALF=SIZE/2;
										STATE=SIZE;
										while(HALF){
											if(wt<RT_wt.Get_wt(STATE-HALF)){STATE-=HALF;}
											HALF/=2;
										}
										RT_wt.AddCalc(STATE,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
										RT_wt.RemoveData(SIZE);
										
									}
									
								}else{//個数が最大値まで達していない
									if (wt>RT_wt.Get_wt(SIZE-1)){//最大値より大きければ末尾にたされる。
										RT_wt.AddCalc(UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
									}else if(wt<RT_wt.Get_wt(0)){//最小値より小さければ、0のindexに入れる
										RT_wt.AddCalc(0,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);

									}else{
										HALF=SIZE/2;
										STATE=SIZE;
										while(HALF){
											if(wt<RT_wt.Get_wt(STATE-HALF)){STATE-=HALF;}
											HALF/=2;
										}
										RT_wt.AddCalc(STATE,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
									}
								}
								//--------------①-2のloss判定し、dataset-----------------------------------
								SIZE=RT_loss.GetSize();
								if(!(SIZE)){
									RT_loss.AddCalc(UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
										l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
								}else if(SIZE>=(unsigned long)(CR.num_data)){//個数が最大値に達した
									if (P[num]>RT_loss.Get_P(SIZE-1,num)){//最大値より大きければなにもせず。捨てられる。
										//ここはなにも記載せず
									}else if(P[num]<RT_loss.Get_P(0,num)){//最小値より小さければ、0のindexに入れる
										RT_loss.AddCalc(0,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
										RT_loss.RemoveData(SIZE);
									}else{
										HALF=SIZE/2;
										STATE=SIZE;
										while(HALF){
											if(P[num]<RT_loss.Get_P(STATE-HALF,num)){STATE-=HALF;}
											HALF/=2;
										}
										RT_loss.AddCalc(STATE,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
										RT_loss.RemoveData(SIZE);
										
									}
									
								}else{//個数が最大値まで達していない
									if (P[num]>RT_loss.Get_P(SIZE-1,num)){//最大値より大きければ末尾にたされる。
										RT_loss.AddCalc(UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
									}else if(P[num]<RT_loss.Get_P(0,num)){//最小値より小さければ、0のindexに入れる
										RT_loss.AddCalc(0,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);

									}else{
										HALF=SIZE/2;
										STATE=SIZE;
										while(HALF){
											if(P[num]<RT_loss.Get_P(STATE-HALF,num)){STATE-=HALF;}
											HALF/=2;
										}
										RT_loss.AddCalc(STATE,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
									}
								}
								//--------------②のloss判定し、dataset-----------------------------------
								SIZE=RT_lossB.GetSize();
								if(!(SIZE)){
									RT_lossB.AddCalc(UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
										l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
								}else if(SIZE>=(unsigned long)(CR.num_data)){//個数が最大値に達した
									if (P[numB]>RT_lossB.Get_P(SIZE-1,numB)){//最大値より大きければなにもせず。捨てられる。
										//ここはなにも記載せず
									}else if(P[numB]<RT_lossB.Get_P(0,numB)){//最小値より小さければ、0のindexに入れる
										RT_lossB.AddCalc(0,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
										RT_lossB.RemoveData(SIZE);
									}else{
										HALF=SIZE/2;
										STATE=SIZE;
										while(HALF){
											if(P[numB]<RT_lossB.Get_P(STATE-HALF,numB)){STATE-=HALF;}
											HALF/=2;
										}
										RT_lossB.AddCalc(STATE,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
										RT_lossB.RemoveData(SIZE);
										
									}
									
								}else{//個数が最大値まで達していない
									if (P[numB]>RT_lossB.Get_P(SIZE-1,numB)){//最大値より大きければ末尾にたされる。
										RT_lossB.AddCalc(UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
									}else if(P[numB]<RT_lossB.Get_P(0,numB)){//最小値より小さければ、0のindexに入れる
										RT_lossB.AddCalc(0,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);

									}else{
										HALF=SIZE/2;
										STATE=SIZE;
										while(HALF){
											if(P[numB]<RT_lossB.Get_P(STATE-HALF,numB)){STATE-=HALF;}
											HALF/=2;
										}
										RT_lossB.AddCalc(STATE,UL1,turn,w_wire,t_wire,l_coil,l_wire,h_core,w_core,gap,PA.n_gap,height,width,length,wt_core,wt,
											l_JIRO,LR_dis,Side_dis,In_Coil_h,In_Coil_w,
											max_C[0],min_C[0],max_B[0],min_B[0],P_Iron[0],P_Cu_DC[0],P_Cu_AC[0],P[0],
											max_C[1],min_C[1],max_B[1],min_B[1],P_Iron[1],P_Cu_DC[1],P_Cu_AC[1],P[1],Q[1],Lmin12,
											max_C[2],min_C[2],max_B[2],min_B[2],P_Iron[2],P_Cu_DC[2],P_Cu_AC[2],P[2],Q[2],Lmin2,
											max_C[3],min_C[3],max_B[3],min_B[3],P_Iron[3],P_Cu_DC[3],P_Cu_AC[3],P[3],
											max_C[4],min_C[4],max_B[4],min_B[4],P_Iron[4],P_Cu_DC[4],P_Cu_AC[4],P[4],Q[4],
											max_C[5],min_C[5],max_B[5],min_B[5],P_Iron[5],P_Cu_DC[5],P_Cu_AC[5],P[5],Q[5],
											max_C[6],min_C[6],max_B[6],min_B[6],P_Iron[6],P_Cu_DC[6],P_Cu_AC[6],P[6],Q[6],
											Qall4,Pmean4,LI);
									}
								}
														

							}

						}
					}

				}
			}
		}




//---------計算終了----------------------
		percent=100;
		t = CTime::GetCurrentTime();
		txtout=t.Format( "FINISH : %H:%M:%S %A, %B %d, %Y \r\n" );
//		str+=",\r\n";
		A_LOG.WriteString(txtout.GetString());
		UpdateAllViews(NULL);
		out_percent(A_LOG);;

		//---------ファイルOutPut実行----------------------
	//	str=" """ID,""" ";
		str="I D,ターン数,巻き線幅(mm),巻き線高さ(mm),ｺｲﾙ全長(mm),銅線全長(mm),コア幅(mm),コア高さ(mm),Gap幅(mm),Gap数,全幅,全高,全長,コア重量(kg),電磁部重量(kg),磁路長(mm),LRコア間隔(mm),サイドコア間隔(mm),ｺｲﾙ内側幅(mm),ｺｲﾙ内側高(mm),";
		str+="1-1最大電流,1-1最小電流,1-1最大B,1-1最小B,1-1鉄損(w),1-1直流銅損(w),1-1交流銅損(w),1-1全損失(w),";
		str+="1-2最大電流,1-2最小電流,1-2最大B,1-2最小B,1-2鉄損(w),1-2直流銅損(w),1-2交流銅損(w),1-2全損失(w),1-2放熱量(J),1-2 最小L(H),";
		str+="2 最大電流,2 最小電流,2 最大B,2 最小B,2 鉄損(w),2 直流銅損(w),2 交流銅損(w),2 全損失(w),2 放熱量(J),2 最小L(H),";
		str+="3 最大電流,3 最小電流,3 最大B,3 最小B,3 鉄損(w),3 直流銅損(w),3 交流銅損(w),3 全損失(w),";
		str+="4-1最大電流,4-1最小電流,4-1最大B,4-1最小B,4-1鉄損(w),4-1直流銅損(w),4-1交流銅損(w),4-1全損失(w),4-1放熱量(J),";
		str+="4-2最大電流,4-2最小電流,4-2最大B,4-2最小B,4-2鉄損(w),4-2直流銅損(w),4-2交流銅損(w),4-2全損失(w),4-2放熱量(J),";
		str+="4-3最大電流,4-3最小電流,4-3最大B,4-3最小B,4-2鉄損(w),4-3直流銅損(w),4-3交流銅損(w),4-3全損失(w),4-3放熱量(J),";
		str+="4 全方熱量(J),4 平均損失(W),";
		str+="0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400";
		str+=",\r\n";
		A_wt.WriteString(str.GetString());
		A_loss.WriteString(str.GetString());
		A_lossB.WriteString(str.GetString());//2011.3.15２つめのlossファイルのため追加

		SIZE=RT_wt.GetSize();
		for(STATE=0;STATE<SIZE;STATE++){
			RT_wt.SetString(&str,STATE);
			A_wt.WriteString(str.GetString());
		}
		SIZE=RT_loss.GetSize();
		for(STATE=0;STATE<SIZE;STATE++){
			RT_loss.SetString(&str,STATE);
			A_loss.WriteString(str.GetString());
		}
		SIZE=RT_lossB.GetSize();
		for(STATE=0;STATE<SIZE;STATE++){
			RT_lossB.SetString(&str,STATE);
			A_lossB.WriteString(str.GetString());
		}

		t = CTime::GetCurrentTime();
		txtout=t.Format( "STORE COMPLETE : %H:%M:%S %A, %B %d, %Y \r\n" );
		A_LOG.WriteString(txtout.GetString());
		UpdateAllViews(NULL);

		A_LOG.Close();
		A_wt.Close();
		A_loss.Close();
		A_lossB.Close();//2011.3.15２つめのlossファイルのため追加
		F_LOG.Close();
		F_wt.Close();
		F_loss.Close();
		F_lossB.Close();//2011.3.15２つめのlossファイルのため追加
		//---------ファイルOutPut終了----------------------


	RT_wt.DelAllData();
	RT_loss.DelAllData();
	RT_lossB.DelAllData();
	BH.DelAllData();

	}
}


// CCORECALC5XDoc 診断

#ifdef _DEBUG
void CCORECALC5XDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCORECALC5XDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCORECALC5XDoc コマンド

int CCORECALC5XDoc::ReadCSV(CFile* pf, double &dat,CString & string,int &n)
{
	//RETURN 0 エラー　0001b(&0x01) 整数, 0011b(|0x03) double数値, 01?1b(|0x05) 文字列, 1??1b(|0x0F) 改行付き,	1???1b(|0x1f) ﾌｧｲﾙ終了
	//機能追加。空白のｾﾙをいくつ飛ばして読んだかをreturnの上位バイトに返すこととする。11.2.26東北大学理学部生協にて
	int result=1;//この関数が呼び出されたときに初期化
	int flg_point=0;//小数点が出現したかどうか
	int flg_pow=0;//べき乗のe or Eが出現したかどうか
	int flg_disit=0;//べき乗のまえに数字が出現したかどうかのチェック
	int pos_pow=0;//E or eが出現したポジションのチェック
	int flg_dbl_qt=0;//ダブルクォーテーション"が最初に出現したら1をたてる。
	double data;	//べき乗（pow）の際、Eより前の実数が入る変数
	string=p_file->GetFileName();
//	string=pf->GetFileName();


	int i,j;
	UINT num;
	char buf[100];
	char *stopstring;
	char c;
	ULONGLONG pos;
	ULONGLONG len;
	len=p_file->GetLength();
	pos=p_file->GetPosition();
	//------もしﾌｧｲﾙがすでに最後まで読んでいたら終了-------------
	if(pos>=len){
		result=result|0x10;
		return result;
	}
	//------1文字目読み取り-------------------------------------
	//   空白or,orCRorLF なら読み飛ばし　最初の有効な文字がでてくるまで読み飛ばし
	i=0;//このループではiはいつもゼロ。buf[0]しか使わない。
	j=0;//読み飛ばした個数＝','の個数が入る
	num=1;
	do{
		if((p_file->Read(&buf[i],num))<num){
			result=(result|0x11)&0x11;//10001の返信。正常終了だがdataなし。
			return result;	//ﾌｧｲﾙ終端につき終了
		}
		if(buf[i]==0x2c){j++;}
	}while(((buf[i]==0x20)||(buf[i]==0x2c)||(buf[i]==0x0d)||(buf[i]==0x0a)));
	j*=256;
	result=(result|j);

	//-----1文字目読み取り完了---------------------------------
	if((isdigit_mask(buf[i]))||(buf[i]=='-')||(buf[i]=='+')||(buf[i]=='.')){
		if(buf[i]=='.'){flg_point=1;}
		if(isdigit_mask(buf[i])){flg_disit=1;}
		//----2文字目以降------------------------
		j=1;
		i=0;
		while(1){
			num=1;
			i++;
			if((p_file->Read(&buf[i],num))<num){
				buf[i]='\0';
				if(flg_disit){
					if(flg_point){	//double
						dat=strtod(buf,&stopstring);
						result=(result&0xff00)+0x13;
						return result;
					}else {
						n=atoi(buf);//整数
						dat=n;//dblにも念のため値を入れて返す。もともとdbl想定の値でもたまたま整数形式になっていることが
								//ありえるので。ex BHｶｰﾌﾞの原点など。
						result=(result&0xff00)+0x11;
						return result;
					}
				}
				string=buf;
				result=(result&0xff00)+0x15;
				return result;	//ﾌｧｲﾙ終端につき終了
			}
			if(!((isdigit_mask(buf[i])||(buf[i]=='.')||(buf[i]=='E')||(buf[i]=='e')))){//数値構成要素以外の文字につき数値ではない
				break;
			}
			if(isdigit_mask(buf[i])){
				flg_disit=1;
				continue;	//数値ならdoを継続
			}
			if((buf[i]=='.')&&(flg_point)){//二重の小数点につき、数値ではない
				break;
			}else if((buf[i]=='.')&&(flg_point==0)){
				flg_point=1;
				continue;
			}
			if(((buf[i]=='E')||(buf[i]=='e'))&&(flg_pow==0)&&(flg_disit==1)){//これと矛盾する場合は数値でない
				c=buf[i];
				buf[i]=0x00;
				data=strtod(buf,&stopstring);
				buf[i]=c;
				flg_pow=1;
				pos_pow=i;
				//改めてe後の1文字目
				j=i;
				i++;
				num=1;
				if((p_file->Read(&buf[i],num))<num){
					buf[i]=0x00;
					string=buf;
					result=(result&0xff00)+0x15;	//中途半端に付き文字列となる。
					return result;	//ﾌｧｲﾙ終端につき終了
				}
				if(!((isdigit_mask(buf[i]))||(buf[i]=='+')||(buf[i]=='-'))){//べき乗後の数値構成要素以外の文字につき数値ではない
					break;
				}
				while(1){
					i++;
					if((p_file->Read(&buf[i],num))<num){
						if(isdigit_mask(buf[i])){
							buf[i+1]=0x00;
							dat=data*pow(10,atoi(&buf[pos_pow+1]));
							result=(result&0xff00)+0x1b;
							return result;	//ﾌｧｲﾙ終端につき終了
						}else{
							buf[i+1]=0x00;
							string=buf;
							result=(result&0xff00)+0x15;	//中途半端に付き文字列となる。
							return result;	//ﾌｧｲﾙ終端につき終了
						}
					}
					if(isdigit_mask(buf[i])){
						continue;
					}else if((buf[i]==0x0d)||(buf[i]==0x0a)){
						buf[i]=0x00;
						dat=data*pow(10,atoi(&buf[pos_pow+1]));
						result=(result&0xff00)+0x0b;
						return result;	//改行で戻り

					}else if((buf[i]==',')){
						buf[i]=0x00;
						dat=data*pow(10,atoi(&buf[pos_pow+1]));
						result=(result&0xff00)+0x03;
						return result;	//コンマで戻り

					}else{break;}
				}
				if((flg_pow=1)&&(!isdigit_mask(buf[i]))){
					break;
				}

			}
		}
		if((buf[i]==0x0d)||(buf[i]==0x0a)){//改行もどり
				buf[i]=0x00;
				if(flg_point){	//double
					dat=strtod(buf,&stopstring);
					result=(result&0xff00)+0x0b;
					return result;
				}else {
					n=atoi(buf);//整数
					dat=n;//dblにも念のため値を入れて返す。もともとdbl想定の値でもたまたま整数形式になっていることが
						//ありえるので。ex BHｶｰﾌﾞの原点など。
					result=(result&0xff00)+0x09;
					return result;
				}
		}
		if((buf[i]==',')){//コンマもどり
				buf[i]=0x00;
				if(flg_point){	//double
					dat=strtod(buf,&stopstring);
					result=(result&0xff00)+0x03;
					return result;
				}else {
					n=atoi(buf);//整数
					dat=n;//dblにも念のため値を入れて返す。もともとdbl想定の値でもたまたま整数形式になっていることが
						//ありえるので。ex BHｶｰﾌﾞの原点など。
					result=(result&0xff00)+0x01;
					return result;
				}
		}
		
	}
	//--------ここからは文字列として処理------------------------------
	while(1){
		if(((num>=0x81)&&(num<=0x91))||((num>=0xe0)&&(num<=0xfc))){//マルチバイトの入り口
				num=1;
				i++;
				if((p_file->Read(&buf[i],num))<num){
					buf[i-1]='\0';//ここでi-1にしていることに注意。マルチバイトの1文字目を捨てている。
					string=buf;
					result=(result&0xff00)+0x15;
					return result;	//ﾌｧｲﾙ終端につき終了
				}
				//2文字目は"や,や改行やLFの判定を行わずそのままbufに投入
		}
		
		num=1;
		i++;
		if((p_file->Read(&buf[i],num))<num){
			buf[i]='\0';
			string=buf;
			result=(result&0xff00)+0x15;
			return result;	//ﾌｧｲﾙ終端につき終了
		}
		if(buf[i]=='"'){
			if(flg_dbl_qt=0){
				flg_dbl_qt=1;
				i--;
			}else{
				flg_dbl_qt=0;
				i--;
			}
		}
		if((buf[i]==0x0d)||(buf[i]==0x0a)){
			buf[i]='\0';
			string=buf;
			result=(result&0xff00)+0x0d;
			return result;	//改行で戻り

		}
		if((buf[i]==',')&&(flg_dbl_qt==0)){
			buf[i]='\0';
			string=buf;
			result=(result&0xff00)+0x05;
			return result;	//コンマで戻り

		}
		



	}
	return 0;

}

int CCORECALC5XDoc::isdigit_mask(char num)
{
	int n;
	n=atoi(&num);
	if(((n>=0x81)&&(n<=0x91))||((n>=0xe0)&&(n<=0xfc))){
	return 0;
	}else if((num>=0x30)&&(num<=0x39)){return 1;}//true
	else{return 0;}//false

}

// Hを代入してBを求める関数
int CCORECALC5XDoc::Cal_B(double  H, double & B,CBHArray &BH)
{
	int i,j;
	double h1,h2,b1,b2;
	j=BH.GetSize();
//	j--;
	if((H>=BH.GetH(0))&&(H<=BH.GetH(j))){
		for(i=1;i<j;i++){//i=1からはじめていることに注意
			if(H<=BH.GetH(i)){
				h1=BH.GetH(i-1);
				h2=BH.GetH(i);
				b1=BH.GetB(i-1);
				b2=BH.GetB(i);
				B=b1+(b2-b1)*(H-h1)/(h2-h1);
				return 1;
			}
		}
		return 0;//ヒットしなかった。ここまで来るわけが無いが。
	}else{
	return 0;
	}
}

// //電流Iを入力し、B,Hを返す関数
int CCORECALC5XDoc::Cal_BandH(double  cur, double & d, int & n, double & len, CBHArray & BH, double & H, double & B)
{
	int i,j;
	double h1,h2,b1,b2;
	double P,Q,R,S;
	j=BH.GetSize();
//	j--;
	if(d==0){
		H=cur*n/(len/1000);
		if((H<BH.GetH(0))||(H>BH.GetH(j))){return 0;}
		for(i=1;i<j;i++){
			if(H<=BH.GetH(i)){
				h1=BH.GetH(i-1);
				h2=BH.GetH(i);
				b1=BH.GetB(i-1);
				b2=BH.GetB(i);
				B=b1+(b2-b1)*(H-h1)/(h2-h1);
				return 1;
			}
//			return 0;
		}
		return 0;
	}else{
		if(BH.GetB(0)>(mu0/(d/1000)*(cur*n-BH.GetH(0)*(len/1000)))){return 0;}//負の側でHが補完できる範囲外であるのでNG
		for(i=1;i<j;i++){
				h1=BH.GetB(i);										//debug用
				h2=mu0/(d/1000)*(cur*n-BH.GetH(i)*(len/1000));		//debug用
			if(BH.GetB(i) > mu0/(d/1000)*(cur*n-BH.GetH(i)*(len/1000))){
				h1=BH.GetH(i-1);
				h2=BH.GetH(i);
				b1=BH.GetB(i-1);
				b2=BH.GetB(i);
				P=(b2-b1)/(h2-h1);
				Q=b1-h1*(b2-b1)/(h2-h1);
				R=-1*mu0*(len/1000)/(d/1000);
				S=mu0*cur*n/(d/1000);
				H=(S-Q)/(P-R);
				B=P*H+Q;
				return 1;
			}
//		return 0;
		}
	return 0;
	}
//	return 0;
}

int CCORECALC5XDoc::Cal_L(double & L,double  cur, double  di,double & S, double & d, int & n, double & len, CBHArray & BH/*, double & H, double & B*/)
{
	double B1,B2;
	double B,H;//11.2.2ローカル変数に変更した。
	if(di==0){return 0;}
	if(!(Cal_BandH(cur,d,n,len,BH,H,B))){
		return 0;}
	B1=B;
	if(!(Cal_BandH(cur+di,d,n,len,BH,H,B))){
		return 0;}
	B2=B;
	L=n*S/1000000*(B2-B1)/di;
	return 1;
}

/*
int CCORECALC5XDoc::WriteCalcDataToCSV(CArchive * pA,CCalcArray * pt,unsigned long index)
{
	CString str;
	CString str2;
	CCalcData *cell;
//	int decimal,sign,i;
	int i;
	cell=pt->Get_cell(index);
	if(!cell){
		return 0;
	}else{
		str=_ultoa(cell->ID,(char *)(str2.GetString()),10);
		str=str2.GetString();

		str+=",";

		str+=_itoa(cell->turn,(char *)(str2.GetString()),10);
		str+=",";
		str+=_gcvt(cell->w_wire,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->t_wire,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->l_coil,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->l_wire,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->w_core,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->h_core,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->gap,10,(char *)(str2.GetString()));
		str+=",";
		str+=_itoa(cell->n_gap,(char *)(str2.GetString()),10);
		str+=",";
		str+=_gcvt(cell->width,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->height,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->length,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->l_JIRO,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->LR_dis,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->Side_dis,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->In_Coil_w,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->In_Coil_h,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->max_C,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->min_C,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->max_B,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->min_B,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->P_Iron,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->P_Cu_DC,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->P_Cu_AC,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->P,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_max_C,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_min_C,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_max_B,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_min_B,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_P_Iron,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_P_Cu_DC,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_P_Cu_AC,10,(char *)(str2.GetString()));
		str+=",";
		str+=_gcvt(cell->sat_P,10,(char *)(str2.GetString()));
		str+=",";

		for(i=0;i<=20;i++){
			str+=_gcvt(cell->LI[i],10,(char *)(str2.GetString()));
			str+=",";
		}
		
		str+="\n";
		
		pA->WriteString(str.GetString());
		
		return 1;
	}
	
}
*/

void CCORECALC5XDoc::out_percent(CArchive& ar)
{
	int i;
	int div;
	div=2;
	char str2[100];
	TRACE("percent=%d\n",percent);
	if(percent==(((int)(percent/div))*div)){
		CString str;
		str=OutP;
		str+=_itoa(percent,str2,10);
		str+=".txt";
		CFile F_OP;
		F_OP.Open(str.GetString(),CFile::modeCreate | CFile::modeWrite);
		F_OP.Close();
		str+="\r\n";
		ar<<str;
	}

}
