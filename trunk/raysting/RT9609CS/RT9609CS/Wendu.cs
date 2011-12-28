using System;

using System.Collections.Generic;
using System.Text;

namespace RT9609CS
{
    enum ConvertMethod
    {
        DIN90,
        DIN68,
        ASTM,
        ITS90,
        PT100,
        PT25,
        Cu60,
        T_TYPE,
        K_TYPE,
        N_TYPE,
        E_TYPE,
        B_TYPE,
        J_TYPE,
        S_TYPE,
        R_TYPE,
    }
    enum TempRange
    {
        RNG4,
        RNG5,
        RNG7,
        RNG8,
        RNG9,
        RNG10,
        RNG11
    }
    class ABCParam
    {
        public TempRange range;
        public double wtp = 0;
        public double a4 = 0;
        public double b4 = 0;
        public double a5 = 0;
        public double b5 = 0;
        public double a8 = 0;
        public double b8 = 0;
        public double a9 = 0;
        public double b9 = 0;
        public double a7 = 0;
        public double b7 = 0;
        public double c7 = 0;
        public double a10 = 0;
        public double a11 = 0;


        public double w_sn;
        public double w_zn;
        public double w_al;
        public double w_in;
        public double w_ga;
        public double w_hg;
        public double w_ar;
        public double w_tp;

        double GetWr(double r)
        {
            return Wendu.GetWr(r);
        }

        public void ConvertToCoef()
        {
            double delta_sn,delta_zn,delta_in,delta_ga,delta_hg,delta_al,DA,DZ;
            if( range == TempRange.RNG7)
            {
		delta_sn = w_sn - GetWr(231.928);
		delta_zn = w_zn - GetWr(419.527);
		delta_al = w_al - GetWr(660.323);
		DA = (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_al-1) * (w_al-1) * (w_al-1) + (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1) * (w_al-1);
		DA = DA +  (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_al-1) * (w_al-1) + (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1) * (w_al-1) * (w_al-1);
		DA = DA -  (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_al-1) * (w_al-1) * (w_al-1) + (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_al-1);

        a7 = delta_sn * ((w_zn - 1) * (w_zn - 1) * (w_al - 1) * (w_al - 1) * (w_al - 1) - (w_zn - 1) * (w_zn - 1) * (w_zn - 1) * (w_al - 1) * (w_al - 1));
		a7 = a7 +  delta_zn*((w_sn-1) * (w_sn-1) * (w_sn-1) * (w_al-1) * (w_al-1) - (w_sn-1) * (w_sn-1) * (w_al-1) * (w_al-1) * (w_al-1));
		a7 = (a7 +  delta_al*((w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1) - (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) * (w_zn-1)))/DA;
		
		b7 = delta_sn*((w_zn-1) * (w_zn-1) * (w_zn-1) * (w_al-1) - (w_zn-1) * (w_al-1) * (w_al-1) * (w_al-1));
		b7 = b7 +  delta_zn*((w_sn-1) * (w_al-1) * (w_al-1) * (w_al-1) - (w_sn-1) * (w_sn-1) * (w_sn-1) * (w_al-1));
		b7 = (b7 +  delta_al*((w_sn-1) * (w_sn-1) * (w_sn-1) * (w_zn-1) - (w_sn-1) * (w_zn-1) * (w_zn-1) * (w_zn-1)))/DA;
		
		c7 = delta_sn*((w_zn-1) * (w_al-1) * (w_al-1) - (w_zn-1) * (w_zn-1) * (w_al-1));
		c7 = c7 +  delta_zn*((w_sn-1) * (w_sn-1) * (w_al-1) - (w_sn-1) * (w_al-1) * (w_al-1));
		c7 = (c7 +  delta_al*((w_sn-1) * (w_zn-1) * (w_zn-1) - (w_sn-1) * (w_sn-1) * (w_zn-1)))/DA;
        }
	if(range == TempRange.RNG8) //a8
	{
		delta_in = w_in - GetWr(156.5985);
		delta_sn = w_sn - GetWr(231.928);
		DZ = (w_sn-1)*(w_in-1) * (w_in-1) - (w_sn-1) * (w_sn-1) * (w_in-1);
		a8 = ((w_in-1) * (w_in-1) *  delta_sn - (w_sn-1) * (w_sn-1) * delta_in)/DZ;
		b8 = ((w_sn-1) *  delta_in - (w_in-1) * delta_sn)/DZ;
	}
	if(range == TempRange.RNG9) //a9
	{
		delta_zn = w_zn - GetWr(419.527);
		delta_sn = w_sn - GetWr(231.928);
		DZ = (w_sn-1)*(w_zn-1) * (w_zn-1) - (w_sn-1) * (w_sn-1) * (w_zn-1);
		a9 = ((w_zn-1) * (w_zn-1) *  delta_sn - (w_sn-1) * (w_sn-1) * delta_zn)/DZ;
		b9 = ((w_sn-1) *  delta_zn - (w_zn-1) * delta_sn)/DZ;
	}
	
	if(range == TempRange.RNG10) //a10
	{
		delta_in = w_in - GetWr(156.598);
		a10 = delta_in/(w_in-1);
	}
	
	if(range == TempRange.RNG11) //a11
	{
		delta_ga = w_ga - GetWr(29.7646);
		a11 = delta_ga/(w_ga-1);
	}
	
	if(range == TempRange.RNG5) //a5
	{
		delta_ga = w_ga - GetWr(29.7646);
		delta_hg = w_hg - GetWr(-38.8344);
		DZ = (w_ga-1)*(w_hg-1) * (w_hg-1) - (w_ga-1) * (w_ga-1) * (w_hg-1);
		a5 = ((w_hg-1) * (w_hg-1) *  delta_ga - (w_ga-1) * (w_ga-1) * delta_hg)/DZ;
		b5 = ((w_ga-1) *  delta_hg - (w_hg-1) * delta_ga)/DZ;
	}
	
	if(range == TempRange.RNG4)// a4
	{
		double w_rhg,w_rar;
		w_rhg = GetWr(-38.8344);
		w_rar = GetWr(-189.3442);
        a4 = (w_hg - w_rhg) * (w_ar - 1) * Math.Log(w_ar) - (w_ar - w_rar) * (w_hg - 1) * Math.Log(w_hg);
        a4 = a4 / ((w_hg - 1) * (w_ar - 1) * (Math.Log(w_ar) - Math.Log(w_hg)));
		
		b4 = (w_ar-w_rar)*(w_hg-1) - (w_hg - w_rhg)*(w_ar-1);
        b4 = b4 / ((w_hg - 1) * (w_ar - 1) * (Math.Log(w_ar) - Math.Log(w_hg)));
	}
        }

        public double a
        {
            get
            {
                if (range == TempRange.RNG4)
                    return a4;
                if (range == TempRange.RNG5)
                    return a5;
                if (range == TempRange.RNG7)
                    return a7;
                if (range == TempRange.RNG8)
                    return a8;
                if (range == TempRange.RNG9)
                    return a9;
                if (range == TempRange.RNG10)
                    return a10;
                if (range == TempRange.RNG11)
                    return a11;

                return 0.0;
            }
        }
        public double b
        {
            get
            {
                if (range == TempRange.RNG4)
                    return b4;
                if (range == TempRange.RNG5)
                    return b5;
                if (range == TempRange.RNG7)
                    return b7;
                if (range == TempRange.RNG8)
                    return b8;
                if (range == TempRange.RNG9)
                    return b9;
                return 0.0;
            }
        }
        public double c
        {
            get
            {
                if (range == TempRange.RNG7)
                    return c7;
                return 0.0;
            }
        }
    }
    class Wendu
    {
        double[] TLow = new double[]{ 0.000000000000E+00, 0.387481063640E-01, 0.441944343470E-04, 0.118443231050E-06, 0.200329735540E-07, 0.901380195590E-09, 0.226511565930E-10, 0.360711542050E-12, 0.384939398830E-14, 0.282135219250E-16, 0.142515947790E-18, 0.487686622860E-21, 0.107955392700E-23, 0.139450270620E-26, 0.797951539270E-30 };
        double[] THigh = new double[]{ 0.000000000000E+00, 0.387481063640E-01, 0.332922278800E-04, 0.206182434040E-06, -0.218822568460E-08, 0.109968809280E-10, -0.308157587720E-13, 0.454791352900E-16, -0.275129016730E-19 };

        double[] SLow = new double[]{ 0.000000000000E+00, 0.540313308631E-02, 0.125934289740E-04, -0.232477968689E-07, 0.322028823036E-10, -0.331465196389E-13, 0.255744251786E-16, -0.125068871393E-19, 0.271443176145E-23 };
        double[] SMed = new double[]{ 0.132900444085E+01, 0.334509311344E-02, 0.654805192818E-05, -0.164856259209E-08, 0.129989605174E-13 };
        double[] SHigh = new double[]{ 0.146628232636E+03, -0.258430516752E+00, 0.163693574641E-03, -0.330439046987E-07, -0.943223690612E-14 };

        double[] RLow = new double[]{ 0.000000000000E+00, 0.528961729765E-02, 0.139166589782E-04, -0.238855693017E-07, 0.356916001063E-10, -0.462347666298E-13, 0.500777441034E-16, -0.373105886191E-19, 0.157716482367E-22, -0.281038625251E-26 };

        double[] RMed = new double[]{ 0.295157925316E+01, -0.252061251332E-02, 0.159564501865E-04, -0.764085947576E-08, 0.205305291024E-11, -0.293359668173E-15 };
        double[] RHigh = new double[]{ 0.152232118209E+03, -0.268819888545E+00, 0.171280280471E-03, -0.345895706453E-07, -0.934633971046E-14 };

        double[] NLow = new double[]{ 0.000000000000E+00, 0.261591059620E-01, 0.109574842280E-04, -0.938411115540E-07, -0.464120397590E-10, -0.263033577160E-11, -0.226534380030E-13, -0.760893007910E-16, -0.934196678350E-19 };
        double[] NHigh = new double[]{ 0.000000000000E+00, 0.259293946010E-01, 0.157101418800E-04, 0.438256272370E-07, -0.252611697940E-09, 0.643118193390E-12, -0.100634715190E-14, 0.997453389920E-18, -0.608632456070E-21, 0.208492293390E-24, -0.306821961510E-28 };

        double[] JLow = new double[]{ 0.000000000000E+00, 0.503811878150E-01, 0.304758369300E-04, -0.856810657200E-07, 0.132281952950E-09, -0.170529583370E-12, 0.209480906970E-15, -0.125383953360E-18, 0.156317256970E-22 };
        double[] JHigh = new double[]{ 0.296456256810E+03, -0.149761277860E+01, 0.317871039240E-02, -0.318476867010E-05, 0.157208190040E-08, -0.306913690560E-12 };

        double[] BLow = new double[]{ 0.000000000000E+00, 0.586655087100E-01, 0.450322755820E-04, 0.289084072120E-07, -0.330568966520E-09, 0.650244032700E-12, -0.191974955040E-15, -0.125366004970E-17, 0.214892175690E-20, -0.143880417820E-23, 0.359608994810E-27 };
        double[] BHigh = new double[]{ -0.389381686210E+01, 0.285717474700E-01, -0.848851047850E-04, 0.157852801640E-06, -0.168353448640E-09, 0.111097940130E-12, -0.445154310330E-16, 0.989756408210E-20, -0.937913302890E-24 };

        double[] EHigh = new double[]{ 0.000000000000E+00, 0.586655087100E-01, 0.450322755820E-04, 0.289084072120E-07, -0.330568966520E-09, 0.650244032700E-12, -0.191974955040E-15, -0.125366004970E-17, 0.214892175690E-20, -0.143880417820E-23, 0.359608994810E-27 };
        double[] ELow = new double[]{ 0.000000000000E+00, 0.586655087080E-01, 0.454109771240E-04, -0.779980486860E-06, -0.258001608430E-07, -0.594525830570E-09, -0.932140586670E-11, -0.102876055340E-12, -0.803701236210E-15, -0.439794973910E-17, -0.164147763550E-19, -0.396736195160E-22, -0.558273287210E-25, -0.346578420130E-28 };

        double[] KLow = new double[]{ 0.000000000000E+00, 0.394501280250E-01, 0.236223735980E-04, -0.328589067840E-06, -0.499048287770E-08, -0.675090591730E-10, -0.574103274280E-12, -0.310888728940E-14, -0.104516093650E-16, -0.198892668780E-19, -0.163226974860E-22 };
        double[] KHigh = new double[]{ -0.176004136860E-01, 0.389212049750E-01, 0.185587700320E-04, -0.994575928740E-07, 0.318409457190E-09, -0.560728448890E-12, 0.560750590590E-15, -0.320207200030E-18, 0.971511471520E-22, -0.121047212750E-25 };

        public static double GetWr(double t)
        {
            double result;
            double[] Ci = new double[]{ 2.78157254, 1.64650916, -0.13714390, -0.00649767, -0.00234444, 0.00511868, 0.00187982, -0.00204472, -0.00046122, 0.00045724 };
            double pert;
            double[] Ai = new double[]{ -2.13534729, 3.18324720, -1.80143597, 0.71727204, 0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872, 0.11868632, -0.05248134 };
            if (t < 0)
            {
                result = Ai[0];
                pert = (Math.Log((t + 273.15) / 273.16) + 1.5) / 1.5;
                t = pert;
                for (int i = 1; i < Ai.Length; i++)
                {
                    result = result + Ai[i] * t;
                    t = t * pert;
                }
                result = Math.Exp(result);
            }
            else
            {
                result = Ci[0];
                pert = t / 481.0 - 1;
                t = pert;
                for (int i = 1; i < Ci.Length; i++)
                {
                    result = result + Ci[i] * t;
                    t = t * pert;
                }
            }
            return result;
        }
        double GetT(double w)
        {
            double perw;
            double result;
            double[] Di = { 439.932854, 472.418020, 37.684494, 7.472018, 2.920828, 0.005184, -0.963864, -0.188732, 0.191203, 0.049025 };
            double[] Bi = { 0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526 };
            if (w >= 0)
            {
                perw = (w - 2.64) / 1.64;
                w = perw;
                result = Di[0];
                for (int i = 1; i < Di.Length; i++)
                {
                    result = result + Di[i] * w;
                    w = w * perw;
                }
            }
            else
            {
                perw = (Math.Pow(w, 1.0 / 6.0) - 0.65) / 0.35;
                w = perw;
                result = Bi[0];
                for (int i = 1; i < Bi.Length; i++)
                {
                    result = result + Bi[i] * w;
                    w = w * perw;
                }
                result = 273.15 * result - 273.5;
            }
            return result;
        }
        //mV to T(degree) convert
        public double MValueToTValue(double r, ConvertMethod type)
        {
            double tlow = 0;
            double tup = 0;
            switch (type)
            {
                case ConvertMethod.T_TYPE:
                    tlow = -270; tup = 400; break;
                case ConvertMethod.K_TYPE:
                    tlow = -270; tup = 1372; break;
                case ConvertMethod.N_TYPE:
                    tlow = -270; tup = 1300; break;
                case ConvertMethod.E_TYPE:
                    tlow = -270; tup = 1000; break;
                case ConvertMethod.B_TYPE:
                    tlow = 0; tup = 1280; break;
                case ConvertMethod.J_TYPE:
                    tlow = -210; tup = 1200; break;
                case ConvertMethod.S_TYPE:
                    tlow = -50; tup = 1768; break;
                case ConvertMethod.R_TYPE:
                    tlow = -50; tup = 1768; break;
                default:
                    return -9999.9999;
            }
            double rnew;
            double tnew;
            int count = 0;

            while ((tup - tlow > 0.00005) || (count++ < 100))
            {
                tnew = (tlow + tup) / 2.0;
                rnew = GetThmoVolt(tnew, type);

                if (r < rnew)
                    tup = tnew;
                else
                    tlow = tnew;
            }

            return (tlow + tup) / 2;
        }
        double GetThmoVolt(double t, ConvertMethod type)
        {
            double[] coef;
            int len = 0;
            switch (type)
            {
                case ConvertMethod.T_TYPE:
                    coef = (t < 0) ? TLow : THigh;
                    break;
                case ConvertMethod.K_TYPE:
                    coef = (t < 0) ? KLow : KHigh;
                    break;
                case ConvertMethod.N_TYPE:
                    coef = (t < 0) ? NLow : NHigh;
                    break;
                case ConvertMethod.E_TYPE:
                    coef = (t < 0) ? ELow : EHigh;
                    break;
                case ConvertMethod.B_TYPE:
                    coef = (t < 630.615) ? NLow : NHigh;
                    break;
                case ConvertMethod.J_TYPE:
                    coef = (t < 760) ? NLow : NHigh;
                    break;
                case ConvertMethod.S_TYPE:
                    coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
                    break;
                case ConvertMethod.R_TYPE:
                    coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
                    break;
                default:
                    return 0.0;
            }
            if (len == 0)
                return 0.0;

            double result = coef[0];
            if (type == ConvertMethod.K_TYPE)
            {
                result = result + (0.118597600000E+00) * Math.Exp((-0.118343200000E-03) * (t - 0.126968600000E+03) * (t - 0.126968600000E+03));
            }

            double pert = t;
            for (int i = 1; i < coef.Length; i++)
            {
                result = result + coef[i] * t;
                t = t * pert;
            }
            return result;
        }

        public double RValueToTValue(double r, ABCParam param, ConvertMethod method)
        {
            
            if (method == ConvertMethod.ITS90)
            {
                double ac, bc, cc;
                ac = param.a;
                bc = param.b;
                cc = param.c;

                //set the search range of T between GetT(r) +/- 1 degree
                double tlow = GetT(r) - 1;
                double tup = tlow + 2;

                double rnew;
                double tnew;
                int count = 0;

                while ((tup - tlow > 0.00005) && (count++ < 100))
                {
                    tnew = (tlow + tup) / 2.0;
                    rnew = GetWr(tnew);
                    rnew = rnew + ac * (rnew - 1) + bc * (rnew - 1) * (rnew - 1) + cc * (rnew - 1) * (rnew - 1) * (rnew - 1);
                    if (r < rnew)
                        tup = tnew;
                    else
                        tlow = tnew;
                }
                return (tlow + tup) / 2.0;
            }
            if (method == ConvertMethod.DIN68 || method == ConvertMethod.DIN90|| method == ConvertMethod.ASTM)
            {
                double alpha,A,B,C;
                alpha = A = B = C = 0;
                if (method == ConvertMethod.DIN68)
                {
                    alpha = 0.003850;
                    param.wtp = 100;
                    A = 3.90802e-3;
                    B = -5.8020e-7;
                    C = -4.2735e-12;
                }
                if (method == ConvertMethod.DIN90)
                {
                    alpha = 0;
                    param.wtp = 100;
                    A = 3.90823e-3;
                    B = -5.7750e-7;
                    C = -4.1830e-12;
                }
                if (method == ConvertMethod.ASTM)
                {
                    alpha = 0.003911;
                    param.wtp = 100;
                    A = 3.9692e-3;
                    B = -5.8495e-7;
                    C = -4.2325e-12;
                }
                double tlow = -201;
                double tup = 1000;

                double rnew;
                double tnew;
                int count = 0;

                while ((tup - tlow > 0.00005) && (count++ < 150))
                {
                    tnew = (tlow + tup) / 2.0;
                    if(tnew > 0)
                        rnew = param.wtp * (1 + A * tnew + B * tnew * tnew);
                    else
                        rnew = param.wtp*(1+A*tnew+B*tnew*tnew+C*(tnew-100)*tnew*tnew*tnew);
                    
                    if (r < rnew)
                        tup = tnew;
                    else
                        tlow = tnew;
                }
                return (tlow + tup) / 2.0;
            }
            if (method == ConvertMethod.PT100)
            {
                //todo
            }
            if (method == ConvertMethod.PT25)
            {
                //todo
            }
            return -9999.9999;
        }
    }
}
