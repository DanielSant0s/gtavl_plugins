#ifndef CCORONAS_H_
#define CCORONAS_H_

#define sizeof_CPed 2064

enum ePedModel
{
	MODEL_NULL = 0,
	MODEL_MALE01	= 7,
	MODEL_BFORI		= 9,
	MODEL_BFOST,
	MODEL_VBFYCRP,
	MODEL_BFYRI,
	MODEL_BFYST,
	MODEL_BMORI,
	MODEL_BMOST,
	MODEL_BMYAP,
	MODEL_BMYBU,
	MODEL_BMYBE,
	MODEL_BMYDJ,
	MODEL_BMYRI,
	MODEL_BMYCR,
	MODEL_BMYST,
	MODEL_WMYBMX,
	MODEL_WBDYG1,
	MODEL_WBDYG2,
	MODEL_WMYBP,
	MODEL_WMYCON,
	MODEL_BMYDRUG,
	MODEL_WMYDRUG,
	MODEL_HMYDRUG,
	MODEL_DWFOLC,
	MODEL_DWMOLC1,
	MODEL_DWMOLC2,
	MODEL_DWMYLC1,
	MODEL_HMOGAR,
	MODEL_WMYGOL1,
	MODEL_WMYGOL2,
	MODEL_HFORI,
	MODEL_HFOST,
	MODEL_HFYRI,
	MODEL_HFYST,
	MODEL_HMORI		= 43,
	MODEL_HMOST,
	MODEL_HMYBE,
	MODEL_HMYRI,
	MODEL_HMYCR,
	MODEL_HMYST,
	MODEL_OMOKUNG,
	MODEL_WMYMECH,
	MODEL_BMYMOUN,
	MODEL_WMYMOUN,
	MODEL_OFORI,
	MODEL_OFOST,
	MODEL_OFYRI,
	MODEL_OFYST,
	MODEL_OMORI,
	MODEL_OMOST,
	MODEL_OMYRI,
	MODEL_OMYST,
	MODEL_WMYPLT,
	MODEL_WMOPJ,
	MODEL_BFYPRO,
	MODEL_HFYPRO,
	MODEL_BMYPOL1	= 66,
	MODEL_BMYPOL2,
	MODEL_WMOPREA,
	MODEL_SBFYST,
	MODEL_WMOSCI,
	MODEL_WMYSGRD,
	MODEL_SWMYHP1,
	MODEL_SWMYHP2,
	MODEL_SWFOPRO	= 75,
	MODEL_WFYSTEW,
	MODEL_SWMOTR1,
	MODEL_WMOTR1,
	MODEL_BMOTR1,
	MODEL_VBMYBOX,
	MODEL_VWMYBOX,
	MODEL_VHMYELV,
	MODEL_VBMYELV,
	MODEL_VIMYELV,
	MODEL_VWFYPRO,
	MODEL_VWFYST1	= 87,
	MODEL_WFORI,
	MODEL_WFOST,
	MODEL_WFYJG,
	MODEL_WFYRI,
	MODEL_WFYRO,
	MODEL_WFYST,
	MODEL_WMORI,
	MODEL_WMOST,
	MODEL_WMYJG,
	MODEL_WMYLG,
	MODEL_WMYRI,
	MODEL_WMYRO,
	MODEL_WMYCR,
	MODEL_WMYST,
	MODEL_BALLAS1,
	MODEL_BALLAS2,
	MODEL_BALLAS3,
	MODEL_FAM1,
	MODEL_FAM2,
	MODEL_FAM3,
	MODEL_LSV1,
	MODEL_LSV2,
	MODEL_LSV3,
	MODEL_MAFFA,
	MODEL_MAFFB,
	MODEL_MAFBOSS,
	MODEL_VLA1,
	MODEL_VLA2,
	MODEL_VLA3,
	MODEL_TRIADA,
	MODEL_TRIADB,
	MODEL_TRIBOSS	= 120,
	MODEL_DNB1,
	MODEL_DNB2,
	MODEL_DNB3,
	MODEL_VMAFF1,
	MODEL_VMAFF2,
	MODEL_VMAFF3,
	MODEL_VMAFF4,
	MODEL_DNMYLC,
	MODEL_DNFOLC1,
	MODEL_DNFOLC2,
	MODEL_DNFYLC,
	MODEL_DNMOLC1,
	MODEL_DNMOLC2,
	MODEL_SBMOTR2,
	MODEL_SWMOTR2,
	MODEL_SBMYTR3,
	MODEL_SWMOTR3,
	MODEL_WFYBE,
	MODEL_BFYBE,
	MODEL_HFYBE,
	MODEL_SOFYBU,
	MODEL_SBMYST,
	MODEL_SBMYCR,
	MODEL_BMYCG,
	MODEL_WFYCRK,
	MODEL_HMYCM,
	MODEL_WMYBU,
	MODEL_BFYBU,
	MODEL_WFYBU		= 150,
	MODEL_DWFYLC1,
	MODEL_WFYPRO,
	MODEL_WMYCONB,
	MODEL_WMYBE,
	MODEL_WMYPIZZ,
	MODEL_BMOBAR,
	MODEL_CWFYHB,
	MODEL_CWMOFR,
	MODEL_CWMOHB1,
	MODEL_CWMOHB2,
	MODEL_CWMYFR,
	MODEL_CWMYHB1,
	MODEL_BMYBOUN,
	MODEL_WMYBOUN,
	MODEL_WMOMIB,
	MODEL_BMYMIB,
	MODEL_WMYBELL,
	MODEL_BMOCHIL,
	MODEL_SOFYRI,
	MODEL_SOMYST,
	MODEL_VWMYBJD,
	MODEL_VWFYCRP,
	MODEL_SFR1,
	MODEL_SFR2,
	MODEL_SFR3,
	MODEL_BMYBAR,
	MODEL_WMYBAR,
	MODEL_WFYSEX,
	MODEL_WMYAMMO,
	MODEL_BMYTATT,
	MODEL_VWMYCR,
	MODEL_VBMOCD,
	MODEL_VBMYCR,
	MODEL_VHMYCR,
	MODEL_SBMYRI,
	MODEL_SOMYRI,
	MODEL_SOMYBU,
	MODEL_SWMYST,
	MODEL_WMYVA,
	MODEL_COPGRL3,
	MODEL_GUNGRL3,
	MODEL_MECGRL3,
	MODEL_NURGRL3,
	MODEL_CROGRL3,
	MODEL_GANGRL3,
	MODEL_CWFOFR,
	MODEL_CWFOHB,
	MODEL_CWFYFR1,
	MODEL_CWFYFR2,
	MODEL_CWMYHB2,
	MODEL_DWFYLC2,
	MODEL_DWMYLC2,
	MODEL_OMYKARA,
	MODEL_WMYKARA,
	MODEL_WFYBURG,
	MODEL_VWMYCD,
	MODEL_VHFYPRO,
	MODEL_OMONOOD	= 209,
	MODEL_OMOBOAT,
	MODEL_WFYCLOT,
	MODEL_VWMOTR1,
	MODEL_VWMOTR2,
	MODEL_VWFYWAI,
	MODEL_SBFORI,
	MODEL_SWFYRI,
	MODEL_WMYCLOT,
	MODEL_SBFOST,
	MODEL_SBFYRI,
	MODEL_SBMOCD,
	MODEL_SBMORI,
	MODEL_SBMOST,
	MODEL_SHMYCR,
	MODEL_SOFORI,
	MODEL_SOFOST,
	MODEL_SOFYST,
	MODEL_SOMOBU,
	MODEL_SOMORI,
	MODEL_SOMOST,
	MODEL_SWMOTR5,
	MODEL_SWFORI,
	MODEL_SWFOST,
	MODEL_SWFYST,
	MODEL_SWMOCD,
	MODEL_SWMORI,
	MODEL_SWMOST,
	MODEL_SHFYPRO,
	MODEL_SBFYPRO,
	MODEL_SWMOTR4,
	MODEL_SWMYRI,
	MODEL_SMYST,
	MODEL_SMYST2,
	MODEL_SFYPRO,
	MODEL_VBFYST2,
	MODEL_VBFYPRO,
	MODEL_VHFYST3,
	MODEL_BIKERA,
	MODEL_BIKERB,
	MODEL_BMYPIMP,
	MODEL_SWMYCR,
	MODEL_WFYLG,
	MODEL_WMYVA2,
	MODEL_BMOSEC,
	MODEL_BIKDRUG,
	MODEL_WMYCH,
	MODEL_SBFYSTR,
	MODEL_SWFYSTR,
	MODEL_HECK1,
	MODEL_HECK2,
	MODEL_BMYCON,
	MODEL_WMYCD1,
	MODEL_BMOCD,
	MODEL_VWFYWA2,
	MODEL_WMOICE,
	MODEL_LAEMT1	= 274,
	MODEL_LVEMT1,
	MODEL_SFEMT1,
	MODEL_LAFD1,
	MODEL_LVFD1,
	MODEL_SFFD1,
	MODEL_LAPD1,
	MODEL_SFPD1,
	MODEL_LVPD1,
	MODEL_CSHER,
	MODEL_LAPDM1,
	MODEL_SWAT,
	MODEL_FBI,
	MODEL_ARMY,
	MODEL_DSHER
};


#endif
