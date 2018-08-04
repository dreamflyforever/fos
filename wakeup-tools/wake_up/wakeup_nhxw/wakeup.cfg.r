use_dnn_wakeup = 1;
use_vad=1;
dnnwakeup={
	vad={
dnnvad={
    parm={
        TARGERRORMAT = HTK;
        SOURCEFORMAT = WAV;
        SOURCERATE   = 625.0;
        SOURCEKIND   = WAVEFORM;
        TARGETRATE   = 200000.0;
        TARGETKIND   = FBANK_D;
        SAVECOMPRESSED = 0;
        SAVEWITHCRC = 0;
        ZMEANSOURCE = 1;
        WINDOWSIZE = 300000.0;

        USEHAMMING = 1;
        PREEMCOEF = 0.97;
        NUMCHANS = 24;
        ESCALE = 1.0;
        SILFLOOR = 50.0;
        ENORMALISE = 0;
        USEPOWER = 1;

        use_dnn=1;
        dnn={
            use_expand_vector=1;
            padding_frame=12;
            sil_thresh=0.1; #-0.23;
            speech_thresh=0.2; #-0.23;
            win=5;
            use_fix_float=1;
            use_layer0_fix_float=1;
            use_fix_trans_matrix=1;
            use_linear_output=0;
            data_type=1;
            max_w=2048;
            max_b=512;
            net_fn=${pwd}/vadnnet.bin;
            trans_fn=${pwd}/vadtransf.bin;
            use_fix_res=1;
            is_bin=1;
            skip_frame=0;
            cache_size=1;
        };
    };
    sil_thresh=-0.3;
    siltrap=10;
    speechtrap=10;
};
use_dnn=1;
left_margin=30;
right_margin=0;
min_speech=0;

version=wakeup_aifar_vad.v0.4;
	};
    parm={
        SOURCEKIND = WAVEFORM;
        SOURCEFORMAT = WAV;
        SOURCERATE = 625.0;
        TARGETFORMAT = HTK;
        TARGETKIND   = FBANK_D_A_Z;
        TARGETRATE = 200000.0;
        ZMEANSOURCE = 1;
        WINDOWSIZE = 300000.0;
        SAVECOMPRESSED = 0;
        SAVEWITHCRC = 0;
        USEHAMMING = 1;
        PREEMCOEF = 0.97;
        NUMCHANS = 24;
        ESCALE = 1.0;
        ENORMALISE = 0;
        USEPOWER = 1;

        use_z=0;
        use_cmn=1;
        zmean={
            #start_min_frame=12;
            post_update_frame=15;
#            smooth=1;
            left_seek_frame=15;
#            min_flush_frame=-1;
        };
        use_dnn=1;
        dnn={
			win=5;
			use_blas=0;
			skip_frame=0;
			cache_size=4;
			use_linear_output=0;
			min_flush_frame=0;
			max_w=2048;
			max_b=512;
			data_type=1;
			use_fix_float=1;
			use_layer0_fix_float=1;
			use_fix_trans_matrix=1;
			net_fn=${pwd}/nnet.addlinear.bin;
			trans_fn=${pwd}/transf.bin;
			use_fix_res=1;
			use_lazy_out=0;
			is_bin=1;
        };
    };
#    use_sil_restart=1;
#    sil_min_thresh=0.9;
    use_progressive_smooth = 1;
#   wsmooth=7;
#   wmax=20;
    smooth_array=[10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,7,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,7,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,7,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,7,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,7,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10];  #dict中每个字的平滑窗�?     max_array=[20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20];  #每个字的最大窗�?例如 你好小乐  你：15*3�?好：30*2�?小：45*1�?乐：0
     max_win=30;   #上述两项的最大�?     use_diff_win=1; #使能这种机制

dict=["sil", "a", "ai", "an", "ang", "ao", "ba", "bai", "ban", "bang", "bao", "bei", "ben", "beng", "bi", "bian", "biao", "bie", "bin", "bing", "bo", "bu", "ca", "cai", "can", "cang", "cao", "ce", "cen", "ceng", "cha", "chai", "chan", "chang", "chao", "che", "chen", "cheng", "chi", "chong", "chou", "chu", "chuai", "chuan", "chuang", "chui", "chun", "chuo", "ci", "cong", "cou", "cu", "cuan", "cui", "cun", "cuo", "da", "dai", "dan", "dang", "dao", "de", "dei", "den", "deng", "di", "dia", "dian", "diao", "die", "ding", "diu", "dong", "dou", "du", "duan", "dui", "dun", "duo", "e", "ei", "en", "er", "fa", "fan", "fang", "fei", "fen", "feng", "fo", "fou", "fu", "ga", "gai", "gan", "gang", "gao", "ge", "gei", "gen", "geng", "gong", "gou", "gu", "gua", "guai", "guan", "guang", "gui", "gun", "guo", "ha", "hai", "han", "hang", "hao", "he", "hei", "hen", "heng", "hng", "hong", "hou", "hu", "hua", "huai", "huan", "huang", "hui", "hun", "huo", "ji", "jia", "jian", "jiang", "jiao", "jie", "jin", "jing", "jiong", "jiu", "juan", "jue", "jun", "jv", "ka", "kai", "kan", "kang", "kao", "ke", "ken", "keng", "kong", "kou", "ku", "kua", "kuai", "kuan", "kuang", "kui", "kun", "kuo", "la", "lai", "lan", "lang", "lao", "le", "lei", "leng", "li", "lia", "lian", "liang", "liao", "lie", "lin", "ling", "liu", "lo", "long", "lou", "lu", "luan", "lue", "lun", "luo", "lv", "ma", "mai", "man", "mang", "mao", "me", "mei", "men", "meng", "mi", "mian", "miao", "mie", "min", "ming", "miu", "mo", "mou", "mu", "na", "nai", "nan", "nang", "nao", "ne", "nei", "nen", "neng", "ng", "ni", "nian", "niang", "niao", "nie", "nin", "ning", "niu", "nong", "nou", "nu", "nuan", "nue", "nuo", "nv", "o", "ou", "pa", "pai", "pan", "pang", "pao", "pei", "pen", "peng", "pi", "pian", "piao", "pie", "pin", "ping", "po", "pou", "pu", "qi", "qia", "qian", "qiang", "qiao", "qie", "qin", "qing", "qiong", "qiu", "quan", "que", "qun", "qv", "ran", "rang", "rao", "re", "ren", "reng", "ri", "rong", "rou", "ru", "ruan", "rui", "run", "ruo", "sa", "sai", "san", "sang", "sao", "se", "sen", "seng", "sha", "shai", "shan", "shang", "shao", "she", "shei", "shen", "sheng", "shi", "shou", "shu", "shua", "shuai", "shuan", "shuang", "shui", "shun", "shuo", "si", "song", "sou", "su", "suan", "sui", "sun", "suo", "ta", "tai", "tan", "tang", "tao", "te", "tei", "teng", "ti", "tian", "tiao", "tie", "ting", "tong", "tou", "tu", "tuan", "tui", "tun", "tuo", "wa", "wai", "wan", "wang", "wei", "wen", "weng", "wo", "wu", "xi", "xia", "xian", "xiang", "xiao", "xie", "xin", "xing", "xiong", "xiu", "xuan", "xue", "xun", "xv", "ya", "yan", "yang", "yao", "ye", "yi", "yin", "ying", "yo", "yong", "you", "yu", "yuan", "yue", "yun", "za", "zai", "zan", "zang", "zao", "ze", "zei", "zen", "zeng", "zha", "zhai", "zhan", "zhang", "zhao", "zhe", "zhei", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu", "zhua", "zhuai", "zhuan", "zhuang", "zhui", "zhun", "zhuo", "zi", "zong", "zou", "zu", "zuan", "zui", "zun", "zuo"];

#    dict=["sil","speech","ba","bian","bo","ci","da","dan","dao","dian","duo","er","fang","huan","ji","liang","mian","ni","qv","shang","sheng","shou","sui","tiao","ting","xi","xia","xiao","xiu","xun","xv","yi","yin","yue","zai","zan","zui","kai","jie","zou","deng","guan","bi","cang","ge"];
#    expwords=["er duo","bo fang yin yue","shang yi shou","xia yi shou","dan qv xun huan","zai ting yi ci","sui bian fang dian yin yue","zan ting","ting xia","zan ting bo fang","ji xv ","ji xv bo fang","da sheng yi dian","tiao dao zui da yin liang","xiao sheng yi dian","tiao dao zui xiao yin liang","ni xiu xi ba","xiu mian","da kai jie zou deng","guan bi jie zou deng","shou cang ge qv"];

    expwords=["ni hao xiao wang","xiao luo bo"];
    thresh_array=[0.20,0.20];
  #  expwords=["mo ting mo ting","yin liang jia","yin liang jian","bo fang","zan ting"];
   # thresh_array=[0.10,0.10,0.10,0.15,0.15];
    thresh = 1.5;   # env no thresh_array
#output_boundary=1;
use_penalty=1;
score_high_index=80;
score_low_index=100;
score_high_weight=1.0;
score_low_weight=0.7;
version="wakeup_aihome_ali_nhxp_20170527.bin";
};
