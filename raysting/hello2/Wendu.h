#ifndef _WENDU_H_
#define _WENDU_H_


#define COEF_RANGE_POS	15
#define COEF_VALUE_RTP	14


double RValueToTValue(double r,const double coef[]);
double GetThmoVolt(double t,char type);
void WrtoCoef(double w_sn,double w_zn,double w_al, double w_in, double w_ga, double w_hg, double w_ar,double w_tp,int range, double &acoef, double &bcoef, double &ccoef, double *coef);
double MValueToTValue(double r,char type);
double GetWr(double t);
double GetdWrdT(double t);
double GetT(double w);
void GetABCcoef(int range, double &w_tp, double &acoef, double &bcoef, double &ccoef, double *coef);
void SaveABCcoef(int range,double w_tp, double acoef, double bcoef, double ccoef, double *coef);
#endif