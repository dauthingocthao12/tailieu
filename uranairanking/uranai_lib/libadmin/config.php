<?php
error_reporting(E_ALL ^ E_NOTICE);

// サーバー毎の設定ファイル
require_once realpath(dirname(__FILE__) . '/server.php');
date_default_timezone_set('Asia/Tokyo');

//サイトマップ用
define('BASE_URL', "https://uranairanking.jp/"); //add kimura 2017/06/13

// admin 用
define("USER", "azet00001");	//edit okabe 2016/05/25
//define("PASS", "pass");	//del okabe 2016/05/25
define("PASS", "304f6befcaf637145c821f30f737b4fb");	//add okabe 2016/05/25 いつもの&md5処理化に変更
define("DEBUG", false);
define("CACHE_DATE","20180607");
define("PREV_DATE", "20160101");
define("PREV_DATE_DTL","20170623");//恋愛運等開始日
define("DB_DATE_DTL","20170701");//恋愛運等年間月間開始日
define("PREV_DATE_DTL_M","20191011");//金運等開始日
define("DB_DATE_DTL_M","20191101");//金運等年間月間開始日
define("DEFAULT_STAR", 6);

define('TITLE', '星座占いランキング');

// mail + account 用
define('COOKIE_ACCOUNT_PERIOD', 86400 * 30);	// one day=86400
define('ACCOUNT_DATA_SUBJECT', '12星座占いランキングのアカウントについて');
define('ACCOUNT_PASSWORD_SUBJECT', '12星座占いランキングのアカウントのパスワード');
define('MAIL_SENDER_EMAIL', 'sender@uranairanking.jp');	//edit okabe 2016/06/27 info->sender
define('MAIL_SENDER_NAME', 'uranairanking.jp');
define('MAIL_UPDATE_SUBJECT', '12星座占いランキング!');
define('ACCOUNT_ACTIVATE_SUBJECT', '12星座占いランキングのアカウント有効化');	//add okabe 2016/06/07
define('REGIST_LOG_FOLDER', dirname(__FILE__) . '/../log/');

// サイトのコメントに関して
define('COMMENT_NOTIFICATION_ADMIN_EMAIL',   'comment_review@uranairanking.jp'); // コメントが登録されたら、管理者に通知
define('COMMENT_NOTIFICATION_EMAIL_SUBJECT', 'サイトのコメントが届きました');
define('COMMENT_ADMIN_EMAIL_SUBJECT', 'いただいたコメントの審査結果について');            // 管理者がユーザのコメントに関してメールを送る時にタイトル
define('COMMENT_REPORT_NOTIFICATION_EMAIL_SUBJECT', 'サイトのコメントが報告されました');

// ユーザのマイページから
define('COMMENT_SHOW_ERROR',   'コメントの公開ができませんでした');
define('COMMENT_SHOW_SUCCESS', 'コメントを公開しました');
define('COMMENT_HIDE_ERROR',   'コメントの非公開ができませんでした');
define('COMMENT_HIDE_SUCCESS', 'コメントを非公開にしました');

define('COMMENT_REPORT_ERROR',    'エラーになりました。お手数ですが、しばらく待ってからもう一度報告して下さい。');
define('COMMENT_REPORT_THANKYOU', 'ご協力ありがとうございました。');
// 備考：コメントの違反カテゴリの設定は[site_comment_report.class.php]ファイルにあります。

//サイト説明文最大文字数
define('SITE_DESC_TEXT_LEN', 30);

// BAT用設定
define('LOG_FOLDER', realpath(dirname(__FILE__) . '/../bat/log').'/');
define('DATA_SAVE_FOLDER', realpath(dirname(__FILE__) . '/../bat/data_save').'/');
define('BACKUP_FOLDER', realpath(dirname(__FILE__) . '/../bat/backup').'/');
define('LOG_LISTING_LINES', 100);
define('BAT_PATTERN_TEST_FOLDER', realpath(dirname(__FILE__) . '/../bat/tests').'/');
define('MAIL_TMP_FOLDER', realpath(dirname(__FILE__) . '/../bat/tmp').'/');	//add okabe 2016/06/
define('MAIL_PAST_DAYS', 3);	//配信時に掲載する過去日数 add okabe 2016/06/
define('GRAPH_TMP_FOLDER', realpath(dirname(__FILE__) . '/../bat/tmpg').'/');	//add okabe 2016/08/23

//フリガナの確認の為
define('KATAKANA_REG_MASK', "/^[\p{Katakana}ー]+$/u");

//======================================================================
// 環境
define('PROD_SITE_ROOT_URL', "https://uranairanking.jp/"); //snsapiにもURLがあります
define('DEV_SITE_ROOT_URL',  "http://dev.uranairanking.jp/");

if(IS_SERVER) {
	//本番
	define('SITE_ROOT_URL', PROD_SITE_ROOT_URL);
	define('ACCOUNT_REGIST_MAILTO', "ans@uranairanking.jp");
	define('ACCOUNT_REGIST_MAILTO_MASKING',  "109,97,105,108,116,111,58, 97,110,115,64,117,114,97,110,97,105,114,97,110,107,105,110,103,46,106,112"); // mailto:含めて
}
else {
	//開発
	define('SITE_ROOT_URL', DEV_SITE_ROOT_URL);
	define('ACCOUNT_REGIST_MAILTO', "dev_ans@uranairanking.jp");
	define('ACCOUNT_REGIST_MAILTO_MASKING', "109,97,105,108,116,111,58, 100,101,118,95,97,110,115,64,117,114,97,110,97,105,114,97,110,107,105,110,103,46,106,112"); // mailto:含めて
}

//======================================================================
// 広告用
// AdsenseOFF設定
$AFFILIATE_OFF = array(
	'IP' => array(
		'14.3.254.40',
		'220.247.17.148',
		'202.171.139.14',
		'14.3.254.53',
		'157.107.57.61',
		'14.3.254.68'
	)
);
// Ad表示数を計算しないHOST名一覧
// regexp 形式
// テストのためにzoot\.jp（アゼットのプロバイダー）を追加すると計算されない
$BOTS = array(
	'REMOTE_HOST' => 'msnbot.*\.search\.msn|crawl.*googlebot|ipnet\.ua|yse\.yahoo'
	,'HTTP_USER_AGENT' => 'MJ12bot'
);

$conn = mysqli_connect(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
// Check connection
if (!$conn) {
	die("Connection failed: " . mysqli_connect_error());
}

$sql = 'SET CHARACTER SET utf8';
$result = mysqli_query($conn, $sql);

//----------------------------------------------------------------------
//新着情報表示件数

define('NEWS_PER_PAGE',10);

//======================================================================
// 星座

$name["star1"] = "みずがめ座";
$name["star2"] = "うお座";
$name["star3"] = "おひつじ座";
$name["star4"] = "おうし座";
$name["star5"] = "ふたご座";
$name["star6"] = "かに座";
$name["star7"] = "しし座";
$name["star8"] = "おとめ座";
$name["star9"] = "てんびん座";
$name["star10"] = "さそり座";
$name["star11"] = "いて座";
$name["star12"] = "やぎ座";

$name["star_kanji1"] = "みずがめ (水瓶)座";
$name["star_kanji2"] = "うお (魚)座";
$name["star_kanji3"] = "おひつじ (牡羊)座";
$name["star_kanji4"] = "おうし (牡牛)座";
$name["star_kanji5"] = "ふたご (双子)座";
$name["star_kanji6"] = "かに (蟹)座";
$name["star_kanji7"] = "しし (獅子)座";
$name["star_kanji8"] = "おとめ (乙女)座";
$name["star_kanji9"] = "てんびん (天秤)座";
$name["star_kanji10"] = "さそり (蠍)座";
$name["star_kanji11"] = "いて	(射手)座";
$name["star_kanji12"] = "やぎ (山羊)座";


$en_num_star = array(
	 "1" => "aquarius"
	,"2" => "pisces"
	,"3" => "aeris"
	,"4" => "taurus"
	,"5" => "gemini"
	,"6" => "cancer"
	,"7" => "leo"
	,"8" => "virgo"
	,"9" => "libra"
	,"10" => "scorpio"
	,"11" => "sagittarius"
	,"12" => "capricorn"
);

// star dates
// from/to: mm-dd
$star_dates = array(
	 '1' => array('from' => '01-20', 'to' => '02-18'),
	 '2' => array('from' => '02-19', 'to' => '03-20'),
	 '3' => array('from' => '03-21', 'to' => '04-19'),
	 '4' => array('from' => '04-20', 'to' => '05-20'),
	 '5' => array('from' => '05-21', 'to' => '06-20'),
	 '6' => array('from' => '06-21', 'to' => '07-22'),
	 '7' => array('from' => '07-23', 'to' => '08-22'),
	 '8' => array('from' => '08-23', 'to' => '09-22'),
	 '9' => array('from' => '09-23', 'to' => '10-22'),
	'10' => array('from' => '10-23', 'to' => '11-21'),
	'11' => array('from' => '11-22', 'to' => '12-21'),
	'12' => array('from' => '12-22', 'to' => '01-19')
);

//english names
$en_star = array(
	"みずがめ座" => "aquarius"
	,"うお座" => "pisces"
	,"おひつじ座" => "aeris"
	,"おうし座" => "taurus"
	,"ふたご座" => "gemini"
	,"かに座" => "cancer"
	,"しし座" => "leo"
	,"おとめ座" => "virgo"
	,"てんびん座" => "libra"
	,"さそり座" => "scorpio"
	,"いて座" => "sagittarius"
	,"やぎ座" => "capricorn"
);

$num_star = array(
	"みずがめ座" => "1"
	,"うお座" => "2"
	,"おひつじ座" => "3"
	,"おうし座" => "4"
	,"ふたご座" => "5"
	,"かに座" => "6"
	,"しし座" => "7"
	,"おとめ座" => "8"
	,"てんびん座" => "9"
	,"さそり座" => "10"
	,"いて座" => "11"
	,"やぎ座" => "12"
);

$kanji_star = array(
	"みずがめ座" => "水瓶座"
	,"うお座" => "魚座"
	,"おひつじ座" => "牡羊座"
	,"おうし座" => "牡牛座"
	,"ふたご座" => "双子座"
	,"かに座" => "蟹座"
	,"しし座" => "獅子座"
	,"おとめ座" => "乙女座"
	,"てんびん座" => "天秤座"
	,"さそり座" => "蠍座"
	,"いて座" => "射手座"
	,"やぎ座" => "山羊座"
);


$star_emojis = array(
	 "1" => "♒"
	,"2" => "♓"
	,"3" => "♈"
	,"4" => "♉"
	,"5" => "♊"
	,"6" => "♋"
	,"7" => "♌"
	,"8" => "♍"
	,"9" => "♎"
	,"10" => "♏"
	,"11" => "♐"
	,"12" => "♑"
);

// Holidays
// source: http://www.benri.com/calendar/
$holidays = array(
	//'2016-03-01' => 'TEST day',
	'2016-01-01' => '元日',
	'2016-01-11' => '成人の日',
	'2016-02-11' => '建国記念の日',
	'2016-03-20' => '春分の日',
	'2016-03-21' => '振替休日',
	'2016-04-29' => '昭和の日',
	'2016-05-03' => '憲法記念日',
	'2016-05-04' => 'みどりの日',
	'2016-05-05' => 'こどもの日',
	'2016-07-18' => '海の日',
	'2016-08-11' => '山の日',
	'2016-09-19' => '敬老の日',
	'2016-09-22' => '秋分の日',
	'2016-10-10' => '体育の日',
	'2016-11-03' => '文化の日',
	'2016-11-23' => '勤労感謝の日',
	'2016-12-23' => '天皇誕生日',

	'2017-01-01' => '元日',
	'2017-01-02' => '振替休日',
	'2017-01-09' => '成人の日',
	'2017-02-11' => '建国記念の日',
	'2017-03-20' => '春分の日',
	'2017-04-29' => '昭和の日',
	'2017-05-03' => '憲法記念日',
	'2017-05-04' => 'みどりの日',
	'2017-05-05' => 'こどもの日',
	'2017-07-17' => '海の日',
	'2017-08-11' => '山の日',
	'2017-09-18' => '敬老の日',
	'2017-09-23' => '秋分の日',
	'2017-10-09' => '体育の日',
	'2017-11-03' => '文化の日',
	'2017-11-23' => '勤労感謝の日',
	'2017-12-23' => '天皇誕生日',

	'2018-01-01' => '元日',
	'2018-01-08' => '成人の日',
	'2018-02-11' => '建国記念の日',
	'2018-02-12' => '振替休日',
	'2018-03-21' => '春分の日',
	'2018-04-29' => '昭和の日',
	'2018-04-30' => '振替休日',
	'2018-05-03' => '憲法記念日',
	'2018-05-04' => 'みどりの日',
	'2018-05-05' => 'こどもの日',
	'2018-07-16' => '海の日',
	'2018-08-11' => '山の日',
	'2018-09-17' => '敬老の日',
	'2018-09-23' => '秋分の日',
	'2018-09-24' => '振替休日',
	'2018-10-08' => '体育の日',
	'2018-11-03' => '文化の日',
	'2018-11-23' => '勤労感謝の日',
	'2018-12-23' => '天皇誕生日',
	'2018-12-24' => '振替休日',
	
	'2019-01-01' => '元日',
	'2019-01-14' => '成人の日',
	'2019-02-11' => '建国記念の日',
	'2019-03-21' => '春分の日',
	'2019-04-29' => '昭和の日',
	'2019-04-30' => '国民の休日',
	'2019-05-01' => '天皇の即位の日',
	'2019-05-02' => '国民の休日',
	'2019-05-03' => '憲法記念日',
	'2019-05-04' => 'みどりの日',
	'2019-05-05' => 'こどもの日',
	'2019-05-06' => '振替休日',
	'2019-07-15' => '海の日',
	'2019-08-11' => '山の日',
	'2019-08-12' => '振替休日',
	'2019-09-16' => '敬老の日',
	'2019-09-23' => '秋分の日',
	'2019-10-14' => '体育の日',
	'2019-10-22' => '即位礼正殿の儀の行われる日',
	'2019-11-03' => '文化の日',
	'2019-11-04' => '振替休日',
	'2019-11-23' => '勤労感謝の日',
	
	'2020-01-01' => '元日',
	'2020-01-13' => '成人の日',
	'2020-02-11' => '建国記念の日',
	'2020-02-23' => '天皇誕生日',
	'2020-02-24' => '振替休日',
	'2020-03-20' => '春分の日',
	'2020-04-29' => '昭和の日',
	'2020-05-03' => '憲法記念日',
	'2020-05-04' => 'みどりの日',
	'2020-05-05' => 'こどもの日',
	'2020-05-06' => '振替休日',
	'2020-07-23' => '海の日',
	'2020-07-24' => 'スポーツの日',
	'2020-08-10' => '山の日',
	'2020-09-21' => '敬老の日',
	'2020-09-22' => '秋分の日',
	'2020-11-03' => '文化の日',
	'2020-11-23' => '勤労感謝の日',

	'2021-01-01' => '元日',
	'2021-01-11' => '成人の日',
	'2021-02-11' => '建国記念の日',
	'2021-02-23' => '天皇誕生日',
	'2021-03-20' => '春分の日',
	'2021-04-29' => '昭和の日',
	'2021-05-03' => '憲法記念日',
	'2021-05-04' => 'みどりの日',
	'2021-05-05' => 'こどもの日',
	'2021-07-22' => '海の日',
	'2021-07-23' => 'スポーツの日',
	'2021-08-08' => '山の日',
	'2021-08-09' => '休日',
	'2021-09-20' => '敬老の日',
	'2021-09-23' => '秋分の日',
	'2021-11-03' => '文化の日',
	'2021-11-23' => '勤労感謝の日',

	'2022-01-01' => '元日',
	'2022-01-10' => '成人の日',
	'2022-02-11' => '建国記念の日',
	'2022-02-23' => '天皇誕生日',
	'2022-03-21' => '春分の日',
	'2022-04-29' => '昭和の日',
	'2022-05-03' => '憲法記念日',
	'2022-05-04' => 'みどりの日',
	'2022-05-05' => 'こどもの日',
	'2022-07-18' => '海の日',
	'2022-08-11' => '山の日',
	'2022-09-19' => '敬老の日',
	'2022-09-23' => '秋分の日',
	'2022-10-10' => 'スポーツの日',
	'2022-11-03' => '文化の日',
	'2022-11-23' => '勤労感謝の日',

	'2023-01-01' => '元日',
	'2023-01-02' => '振替休日',
	'2023-01-09' => '成人の日',
	'2023-02-11' => '建国記念の日',
	'2023-02-23' => '天皇誕生日',
	'2023-03-21' => '春分の日',
	'2023-04-29' => '昭和の日',
	'2023-05-03' => '憲法記念日',
	'2023-05-04' => 'みどりの日',
	'2023-05-05' => 'こどもの日',
	'2023-07-17' => '海の日',
	'2023-08-11' => '山の日',
	'2023-09-18' => '敬老の日',
	'2023-09-23' => '秋分の日',
	'2023-10-09' => 'スポーツの日',
	'2023-11-03' => '文化の日',
	'2023-11-23' => '勤労感謝の日',



);


// 星座コメント情報	//2016/07/09
define('DATA_FOLDER', realpath(dirname(__FILE__) . '/../data').'/');
$msg_data_file = array(
	"0" => "msg00.dat"		////総合
	,"1" => "msg01.dat"		//みずがめ座
	,"2" => "msg02.dat"		//うお座
	,"3" => "msg03.dat"		//おひつじ座
	,"4" => "msg04.dat"		//おうし座
	,"5" => "msg05.dat"		//ふたご座
	,"6" => "msg06.dat"		//かに座
	,"7" => "msg07.dat"		//しし座
	,"8" => "msg08.dat"		//おとめ座
	,"9" => "msg09.dat"		//てんびん座
	,"10" => "msg10.dat"	//さそり座
	,"11" => "msg11.dat"	//いて座
	,"12" => "msg12.dat"
);



//各月の英語名 2017/2/20 yamaguchi
$num_month= array(
	"january" => "01"
	,"february" => "02"
	,"march" => "03"
	,"april" => "04"
	,"may" => "05"
	,"june" => "06"
	,"july" => "07"
	,"august" => "08"
	,"september" => "09"
	,"october" => "10"
	,"november" => "11"
	,"december" => "12"
	,"total" => "total"
);

$month_past_formatB = array(
	'01' => 'january',
	'02' => 'february',
	'03' => 'march',
	'04' => 'april',
	'05' => 'may',
	'06' => 'june',
	'07' => 'july',
	'08' => 'august',
	'09' => 'september',
	'10' => 'october',
	'11' => 'november',
	'12' => 'december'
);

$jpn_num_star = array(
	 '1' => 'みずがめ座'
	,'2' => 'うお座'
	,'3' => 'おひつじ座'
	,'4' => 'おうし座'
	,'5' => 'ふたご座'
	,'6' => 'かに座'
	,'7' => 'しし座'
	,'8' => 'おとめ座'
	,'9' => 'てんびん座'
	,'10' => 'さそり座'
	,'11' => 'いて座'
	,'12' => 'やぎ座'
);

//topic 日本語変換
$topic_Jp = array(
	'' => '',
	'love' => '恋愛運',
	'money' => '金運',
	'work' => '仕事運'
);

//topicの種類
$DATA_TYPE = array(
	''
	,'love'
	,'money'
	,'work'
	,'health'
	//項目数が少なすぎるためコメントアウト
	//,'interpersonal'
	//,'outing'
	//,'info'
	//,'gambling'
);
//年間・月間 日本語変換
$topic_Jp_name = array(
	'defolt' => '総合運',
	'love' => '恋愛運',
	'money' => '金運',
	'work' => '仕事運'
);

$analysis_data_jpn = array(
	"date_start" => "開始日"
	,"date_end" => "終了日"
	,"ga_session" => "合計セッション"
	,"ga_weekday_session" => "平日合計セッション"
	,"ga_weekday_avg_session" => "平日平均セッション"
	,"ga_week_avg_session" => "週平均セッション"
	,"ga_max_session" => "期間最高セッション"
	,"ga_max_session_date" => "期間最高セッション日"
	,"ga_max_session_day" => "期間最高セッション曜日"
	,"ga_new_user_percentage" => "ユーザー新規(%)"
	,"ga_existing_user_percentage" => "ユーザー既存(%)"
	,"ga_new_user" => "ユーザー新規"
	,"ga_existing_user" => "ユーザー既存"
	,"ga_user" => "ユーザー全体"
	,"registed_user" => "期間登録ユーザー数"
	,"deleted_user" => "期間退会ユーザー数"
	,"all_user" => "登録ユーザー数"
	,"plugins" => "稼働プラグイン数"
	,"ga_tw_users" => "twitter流入ユーザー"
	,"ga_fb_users" => "facebook流入ユーザー"
	,"ga_mstdn_users" => "mastodon流入ユーザー"
	,"ga_pv_top1" => "PV上位1位"
	,"ga_pv_top1_value" => "PV上位1位(PV数)"
	,"ga_pv_top2" => "PV上位2位"
	,"ga_pv_top2_value" => "PV上位2位(PV数)"
	,"ga_pv_top3" => "PV上位3位"
	,"ga_pv_top3_value" => "PV上位3位(PV数)"
	,"ga_pv_top4" => "PV上位4位"
	,"ga_pv_top4_value" => "PV上位4位(PV数)"
	,"ga_pv_top5" => "PV上位5位"
	,"ga_pv_top5_value" => "PV上位5位(PV数)"
);

/*--------------------12星座アプリ用--------------------*/

//今日の最上位のサイト一覧の最低表示数
define("MIN_BEST_SITES",5);


// イベント用メッセージdatファイル
$event_msg_data = "ev_msg.dat";