/****************************************************************************************************
 *                                                                                                  *
 *                                        DoT Object Loader                                         *
 *                                                                                                  *
 * Copyright � 2016 Abyss Morgan. All rights reserved.                                              *
 *                                                                                                  *
 * Download: https://github.com/AbyssMorgan/SA-MP/tree/master/include/SAM                           *
 *                                                                                                  *
 * Plugins: None                                                                                    *
 * Modules: ADM                                                                                     *
 *                                                                                                  *
 * File Version: 1.3.2                                                                              *
 * SA:MP Version: 0.3.7                                                                             *
 * ADM Version: 2.2.1                                                                               *
 *                                                                                                  *
 * Object Functions:                                                                                *
 * ADM:LoadDoTMap(const name[],&count=0);                                                           *
 * ADM:SaveDoTMap(const input[],const output[]);                                                    *
 * Float:DoT_CalculateObjectDistance(objectid);                                                     *
 *                                                                                                  *
 ****************************************************************************************************/

/*
Permitted record before converting on the DoT

CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid);
CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid, interiorid);
CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid, interiorid, playerid);
CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid, interiorid, playerid, Float:streamdistance);
CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid, interiorid, playerid, Float:streamdistance, Float:drawdistance);

Example:

SaveDoTMap("Christmas.txt","Christmas.dot"); //Converting Text File Object to DoT File
//Note: The first line of a TXT file may not contain objects required clearance!
LoadDoTMap("Christmas.dot"); //Load DoT File

*/

/*
//Check Version DoT.inc
#if !defined _DoT_Loader
	#error [ADM] You need DoT.inc v1.3.2
#elseif !defined DoT_Loader_Version
	#error [ADM] Update you DoT.inc to v1.3.2
#elseif (DoT_Loader_Version < 10302)
	#error [ADM] Update you DoT.inc to v1.3.2
#endif
*/

#if ((!defined Streamer_AppendArrayData) || (!defined INVALID_STREAMER_ID))
	#error [ADM] Unknown Streamer Version
#endif

#if !defined sscanf
	#error [ADM] Unknown SScanf Version
#endif

//Check Version ADM.inc
#if !defined _adm_encryption
	#error [ADM] You need ADM.inc v2.2.1
#elseif !defined ADM_Encryption_Version
	#error [ADM] Update you ADM.inc to v2.2.1
#elseif (ADM_Encryption_Version < 20201)
	#error [ADM] Update you ADM.inc to v2.2.1
#endif

#if defined _DoT_Loader
	#endinput
#endif
#define _DoT_Loader

#define DoT_Loader_Version 				(10302) //a.b.c 10000*a+100*b+c

#define DOT_HEADER						"DoTv1.0"
#define DOT_HEADER_SIZE					(32)

stock Float:DoT_CalculateObjectDistance(objectid){
	switch(objectid){
		case
			// wszystko z http://wiki.sa-mp.com/wiki/Object_Transportation_Roads%2C_Bridges%2C_and_Tunnels
			3330,3331,3381,3411,3412,3990,3991,3992,3993,3994,3995,3996,4085,4086,4087,4088,4089,4090,4091,4107,4108,4125,4127,4128,4129,4131,4133,4139,4142,4144,4146,4148,4150,4152,4154,4156,4158,4160,4163,4165,
			4168,4182,4203,4207,4209,4233,4553,4557,4567,4589,4644,4645,4646,4647,4648,4649,4650,4651,4652,4653,4654,4656,4658,4660,4662,4664,4666,4679,4692,4694,4695,4710,4807,4808,4809,4820,4821,4822,4823,4827,
			4834,4835,4836,4837,4839,4840,4841,4846,4868,4870,4871,4872,4878,4895,5013,5021,5026,5028,5038,5046,5052,5106,5112,5113,5120,5121,5124,5125,5128,5133,5141,5147,5149,5178,5188,5191,5250,5271,5276,5296,5297,5298,
			5314,5329,5330,5333,5349,5353,5391,5394,5395,5411,5431,5432,5433,5434,5435,5436,5437,5438,5439,5440,5441,5442,5456,5469,5470,5472,5473,5481,5482,5483,5484,5485,5486,5487,5488,5489,5490,5491,5492,5493,5494,5495,
			5496,5497,5498,5499,5500,5501,5502,5503,5504,5505,5506,5507,5508,5509,5510,5511,5512,5528,5650,5668,5703,5707,5744,5745,5746,5747,5748,5749,5750,5751,5752,5753,5754,5755,5756,5757,5758,5759,5793,5794,5795,5796,
			5797,5798,5799,5800,5801,5802,5803,5804,5805,5806,5807,5808,5809,5859,5860,5861,5862,5866,5994,5995,6035,6054,6055,6111,6112,6113,6114,6115,6116,6117,6118,6119,6120,6121,6122,6123,6124,6125,6126,6127,6128,6129,
			6225,6231,6235,6291,6301,6302,6303,6304,6305,6306,6307,6308,6309,6310,6311,6314,6316,6317,6318,6319,6320,6321,6322,6323,6324,6325,6326,6327,6329,6330,6331,6333,6341,6345,6427,6428,6448,6449,6450,6507,6508,6509,
			6876,6877,6878,6879,6880,6881,6886,6887,6888,6897,6898,6899,6900,6945,6948,6949,6950,6951,6952,6953,6956,6974,6990,6991,6999,7036,7041,7042,7043,7052,7053,7054,7055,7056,7057,7064,7069,7320,7321,7324,7326,7327,7334,7335,
			7336,7337,7355,7362,7364,7383,7427,7428,7429,7430,7431,7432,7433,7434,7435,7436,7437,7438,7439,7440,7441,7442,7443,7444,7445,7446,7447,7476,7477,7478,7479,7480,7481,7482,7483,7484,7485,7486,7544,7545,7546,7547,7548,7549,
			7550,7551,7552,7558,7559,7580,7581,7587,7589,7590,7605,7629,7631,7632,7633,7634,7729,7730,7731,7755,7849,7852,7854,7863,7864,7865,7866,7867,7868,7878,7881,7938,7945,7963,7965,7967,7969,7987,7988,7989,7990,7991,7992,7993,
			7995,8009,8010,8036,8039,8045,8046,8047,8048,8049,8050,8051,8052,8053,8054,8055,8056,8070,8080,8128,8135,8137,8212,8213,8214,8215,8216,8217,8218,8219,8236,8244,8245,8246,8256,8290,8305,8368,8377,8380,8382,8383,8386,8388,
			8438,8439,8440,8441,8442,8443,8444,8445,8446,8447,8448,8449,8450,8451,8452,8453,8454,8455,8456,8457,8458,8471,8472,8473,8474,8475,8476,8477,8510,8511,8512,8514,8517,8518,8519,8520,8521,8522,8523,8524,8525,8543,8552,8561,
			8562,8609,8610,8611,8612,8616,8622,8637,8638,8824,8832,8838,8932,9000,9001,9002,9003,9004,9005,9006,9007,9008,9021,9022,9023,9024,9025,9026,9027,9028,9036,9042,9115,9116,9117,9118,9119,9120,9150,9205,9222,9231,9232,9233,
			9250,9251,9252,9262,9264,9265,9266,9267,9269,9476,9485,9486,9487,9488,9489,9490,9491,9492,9493,9570,9571,9575,9591,9600,9601,9602,9603,9652,9653,9683,9685,9689,9690,9693,9694,9696,9699,9700,9701,9702,9703,9704,9706,9707,
			9708,9709,9710,9711,9712,9713,9714,9715,9716,9717,9718,9719,9720,9721,9722,9723,9724,9725,9726,9727,9728,9729,9730,9731,9732,9733,9734,9735,9736,9747,9827,9832,9837,9838,10018,10065,10066,10067,10068,10069,10070,10071,
			10072,10073,10074,10075,10076,10077,10078,10110,10111,10112,10113,10114,10115,10116,10117,10118,10119,10120,10121,10122,10123,10124,10125,10126,10127,10128,10129,10130,10131,10132,10133,10134,10135,10136,10137,10138,
			10139,10165,10235,10247,10275,10276,10294,10295,10296,10359,10360,10361,10362,10363,10364,10365,10367,10424,10426,10440,10448,10449,10450,10452,10455,10456,10457,10458,10459,10460,10461,10462,10463,10464,10465,10466,
			10467,10468,10469,10470,10471,10472,10473,10474,10475,10476,10477,10478,10479,10480,10481,10482,10483,10484,10485,10486,10487,10488,10489,10490,10493,10614,10617,10636,10639,10649,10750,10751,10753,10754,10759,10770,
			10777,10790,10791,10792,10820,10821,10822,10823,10848,10849,10852,10854,10855,10857,10858,10859,10860,10866,10867,10868,10869,10870,10928,10929,10930,10937,10940,10958,10967,10968,10970,10971,11003,11071,11072,11073,
			11074,11075,11076,11077,11078,11079,11080,11084,11094,11095,11096,11098,11100,11104,11105,11110,11111,11112,11113,11114,11115,11116,11117,11118,11119,11120,11121,11122,11123,11124,11125,11126,11127,11128,11129,11130,
			11131,11132,11133,11134,11135,11136,11137,11138,11252,11253,11254,11255,11256,11257,11258,11259,11260,11261,11299,11302,11308,11345,11351,11365,11386,11409,11421,11462,12800,12801,12802,12803,12806,12809,12810,12811,
			12812,12813,12815,12816,12817,12818,12819,12820,12826,12827,12828,12829,12830,12851,12852,12856,12857,12867,12873,12874,12875,12876,12877,12878,12879,12880,12881,12882,12883,12884,12885,12886,12887,12888,12889,12890,
			12891,12892,12893,12894,12895,12896,12897,12898,12899,12900,12901,12902,12903,12904,12905,12906,12907,12909,12910,12965,12966,12967,12968,12970,12971,12972,12973,12974,12975,12992,12993,12994,12995,12996,12997,12998,
			12999,13000,13001,13020,13033,13034,13038,13058,13088,13092,13095,13119,13127,13128,13129,13138,13139,13141,13142,13168,13169,13170,13173,13321,13323,13324,13325,13332,13342,13345,13347,13348,13349,13368,13422,13470,
			13626,13652,13655,13664,13672,13673,13674,13676,13677,13678,13680,13682,13683,13684,13685,13688,13689,13690,13693,13703,13704,13706,13707,13708,13709,13713,13717,13718,13720,13726,13727,13730,13732,13733,13735,13736,
			13738,13739,13751,13752,13784,13789,13814,13845,13865,13882,13887,16037,16266,16357,16358,16384,16430,16571,16610,17002,17003,17004,17043,17062,17077,17078,17110,17111,17112,17120,17146,17148,17150,17152,17154,17156,
			17158,17160,17162,17164,17166,17168,17170,17172,17174,17176,17178,17180,17182,17184,17186,17188,17190,17192,17194,17196,17198,17200,17202,17204,17208,17210,17212,17214,17216,17218,17220,17222,17224,17226,17228,17230,
			17232,17234,17236,17238,17240,17242,17244,17246,17248,17250,17252,17254,17256,17258,17260,17262,17267,17269,17271,17273,17275,17277,17279,17281,17293,17294,17295,17300,17303,17305,17307,17308,17309,17310,17326,17327,
			17329,17331,17333,17334,17501,17502,17525,17550,17561,17576,17595,17596,17597,17598,17599,17600,17602,17603,17604,17605,17606,17607,17608,17609,17610,17611,17612,17613,17621,17622,17623,17624,17625,17626,17627,17628,
			17629,17630,17631,17632,17634,17635,17637,17638,17639,17640,17641,17642,17643,17644,17646,17647,17648,17649,17650,17651,17652,17653,17654,17655,17656,17657,17658,17659,17660,17661,17662,17663,17666,17667,17668,17669,
			17670,17671,17672,17673,17674,17675,17676,17680,17681,17682,17683,17684,17686,17687,17692,17693,17695,17829,17849,17867,17920,17921,17927,17936,17968,18229,18256,18369,18370,18371,18372,18373,18374,18375,18376,18377,
			18378,18379,18380,18381,18382,18383,18384,18385,18386,18387,18388,18389,18390,18391,18392,18393,18394,18449,18450,18561,
			18788..18807,   // drogi 0.3c
			19300..19307, // ltalc bridge 0.3c
			12814:  // duza plyta
		return 600.0;
		
		case
			6959, // plyta srednia
			8661, // plyta srednia
			10828,  // duza brama
			16501,  // plyta mala ale moze uzywana do budow
			16775:  // plyta - brama garazowa
		return 200.0;
		
		case
			973,  // barierki
			3361, // schody
			3499, // pal
			3570,// kontenery
			8650..8652, // murki
			9339, // murek
			9583, // lamp post
			16090:  // konstrukcja metalowa nad droga
		return 150.0;
		
		case
			718,  // palma
			1257, // przystanek
			1281, // stolik
			2960, // szyna
		3472: // lampa
		return 200.0;
	}
	return 200.0;
}

stock ADM:LoadDoTMap(const name[],&count=0){
	new NTTime = GetTickCount();
	if(!fexist(name)) return ERROR_CODE_ADM_INPUT;
	new File:NTMap = fopen(name,io_read), mod[8];
	if(!NTMap) return ERROR_CODE_ADM_OPEN_INPUT;
	new tempfile[80], tempkey[MAX_KEY_LEN_ADMx256];
	for(new i = 0; i < (MAX_MODE_ADMx256*2); i++){
		if(i >= 0 && i < 7){
			mod[i] = fgetchar(NTMap,0,false);
		} else if(i >= 8 && i < 32){
			fgetchar(NTMap,0,false);
		} else if(i == 32){
			format(tempkey,sizeof(tempkey),"%d",fgetchar(NTMap,0,false));
		} else {
			format(tempkey,sizeof(tempkey),"%s %d",tempkey,fgetchar(NTMap,0,false));
		}
	}
	mod[7] = EOS;
	fclose(NTMap);
	if(!strcmp(mod,DOT_HEADER,true)){
	
	} else {
		return ERROR_CODE_ADM_OPEN_INPUT;
	}
	format(tempfile,sizeof(tempfile),"%s.tmp",name);
	if(fexist(tempfile)) fremove(tempfile);
	new ADM:edi = ADMx256(name, tempfile, tempkey);
	if(edi != ERROR_CODE_ADM_OK){
		if(fexist(tempfile)) fremove(tempfile);
		return edi;
	}
	new File:NTObj = fopen(tempfile, io_read), buf[255], line[255], cnt = 0, lnum = 0, ecnt = 0;
	
	while(fread(NTObj,line)){
		lnum++;
		if(line[0]!='/' && strfind(line,"CreateDynamicObject",true)!=-1){
			new objectid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, worldid = -1, interiorid = -1, playerid = -1, Float:streamdistance = 200.0, Float:drawdistance = 0.0, Float:nul;
			if(sscanf(line,"p<,>'('ifffffP<,)>fP<,)>D(-1)D(-1)D(-1)F(-1)F(0)p<)>F(0)", objectid, x, y, z, rx, ry, rz, worldid, interiorid, playerid, streamdistance, drawdistance, nul)){
				if(streamdistance == -1) streamdistance = DoT_CalculateObjectDistance(objectid);
				CreateDynamicObject(objectid,x,y,z,rx,ry,rz,worldid,interiorid,playerid,streamdistance,drawdistance);
				cnt++;
			} else {
				ecnt++;
				format(buf,sizeof buf,"%s, line %d, error in parsing: %s",name,lnum,line);
				printf(buf);
			}
		}
	}
	fclose(NTObj);
	fremove(tempfile);
	format(line,sizeof line,"%s, lines: %d, objects: %d, errors %d, load time %d ms",name,lnum,cnt,ecnt,GetTickCount()-NTTime);
	printf(line);
	return ERROR_CODE_ADM_OK;
}

stock ADM:SaveDoTMap(const input[], const output[]){
	if(!fexist(input)) return ERROR_CODE_ADM_INPUT;
	if(fexist(output)) return ERROR_CODE_ADM_OUTPUT;
	
	new File:InputMap = fopen(input,io_read);
	if(!InputMap) return ERROR_CODE_ADM_OPEN_INPUT;
	
	new File:OutputMap = fopen(output,io_write);
	if(!OutputMap){
		fclose(InputMap);
		return ERROR_CODE_ADM_OPEN_OUTPUT;
	}
	new mode[MAX_MODE_ADMx256],
		tmpkey[MAX_KEY_LEN_ADMx256],
		tmpheader[DOT_HEADER_SIZE],
		tmpheaderkey[MAX_KEY_LEN_ADMx256],
		ADM:dotikey = CreateADMKey(tmpkey,KEY_TYPE_ADMx256),
		ADM:dotokey = DecodeADMKey(tmpkey,tmpheaderkey,KEY_TYPE_ADMx256);
		
	if(dotikey != ERROR_CODE_ADM_OK || dotokey != ERROR_CODE_ADM_OK){
		fclose(InputMap);
		fclose(OutputMap);
		if(fexist(output)){
			fremove(output);
		}
		if(dotikey != ERROR_CODE_ADM_OK){
			return dotikey;
		} else {
			return dotokey;
		}
	}
	
	sscanf(tmpheaderkey,"D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)",
		mode[0],mode[1],mode[2],mode[3],mode[4],mode[5],mode[6],mode[7],
		mode[8],mode[9],mode[10],mode[11],mode[12],mode[13],mode[14],mode[15],
		mode[16],mode[17],mode[18],mode[19],mode[20],mode[21],mode[22],mode[23],
		mode[24],mode[25],mode[26],mode[27],mode[28],mode[29],mode[30],mode[31]
	);
	
	format(tmpheader,sizeof tmpheader,"%s",DOT_HEADER);
	
	for(new i = 0, j = strlen(tmpheader); i < DOT_HEADER_SIZE; i++){
		if(i < j){
			fputchar(OutputMap,tmpheader[i],false);
		} else {
			fputchar(OutputMap,random(256),false);
		}
	}
	
	for(new i = DOT_HEADER_SIZE; i < (DOT_HEADER_SIZE*2); i++){
		fputchar(OutputMap,mode[i-DOT_HEADER_SIZE],false);
	}
	
	sscanf(tmpkey,"D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)D(0)",
		mode[0],mode[1],mode[2],mode[3],mode[4],mode[5],mode[6],mode[7],
		mode[8],mode[9],mode[10],mode[11],mode[12],mode[13],mode[14],mode[15],
		mode[16],mode[17],mode[18],mode[19],mode[20],mode[21],mode[22],mode[23],
		mode[24],mode[25],mode[26],mode[27],mode[28],mode[29],mode[30],mode[31]
	);
	
	new asize = flength(InputMap), idx = 0, s = 0;
	
	while(idx < asize){
		fputchar(OutputMap,((fgetchar(InputMap,0,false)+mode[s]) & 0xFF),false);
		idx++;
		s++;
		if(s >= MAX_MODE_ADMx256) s = 0;
	}
	
	fclose(InputMap);
	fclose(OutputMap);
	
	return ERROR_CODE_ADM_OK;
}

#define DoT(%0,%1) 		LoadDoTMap(%0,%1)

//EOF