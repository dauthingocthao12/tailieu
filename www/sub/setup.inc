<?PHP
//	設定ファイル

//	商品表示プログラムファイル名
$script = "goods";
define("GOODS_SCRIPT", $script);	//	add ookawara 2013/07/22


//	カテゴリーフォルダー
$DIR_CATE = "category";
define("DIR_CATE", $DIR_CATE);	//	add ookawara 2013/07/22

//	カテゴリーファイル
$m_cate_file = "category.htm";

//	表示データーフォルダー
$LOG_DIR = "log";
define("LOG_DIR", $LOG_DIR);	//	add ookawara 2013/07/22

//	サイトドメイン	//	add ookawara 2014/01/24
define("DOMEIN", "futboljersey.com");


//	ポイント返還tempフォルダー
$DIR_CPT = "temp";

//	サイトURL
if($_SERVER["SERVER_PORT"]=="80"){
    $URL = "http://".$_SERVER['SERVER_NAME']; 
}else{
    $URL = "https://".$_SERVER['SERVER_NAME'];
}
//	カテゴリーテーブル
$cate_table = "category";
define("T_CATE", $cate_table);	//	add ookawara 2013/07/18

//	カテゴリー名テーブル
$r_cate_table = "r_cate";
define("T_R_CATE", $r_cate_table);	//	add ookawara 2013/07/18

//	商品テーブル
$goods_table = "goods";
define("T_GOODS", $goods_table);	//	add ookawara 2013/07/18

//	会員テーブル
$member_table = "kojin";
define("T_KOJIN", $member_table);	//	add ookawara 2013/07/18

//	販売情報テーブル
$sells_table = "sells";
define("T_SELLS", $sells_table);	//	add ookawara 2013/07/18

//	アフェリエイトユーザーテーブル
$afuser_table = "afuser";
define("T_AFUSER", $afuser_table);	//	add ookawara 2013/07/18

//	アフェリエイトポイントテーブル
$appoint_table = "appoint";
define("T_APPOINT", $appoint_table);	//	add ookawara 2013/07/18

//	アフェリエイトリファラー
$afrefere_table = "afrefere";
define("T_AFREFERE", $afrefere_table);	//	add ookawara 2013/07/18

//	アフェリエイト支払い
$application_table = "application";
define("T_APPLICATION", $application_table);	//	add ookawara 2013/07/18

//	アフェリエイト銀行
$bank_table = "bank";
define("T_BANK", $bank_table);	//	add ookawara 2013/07/18

//	アフェリエイトメール送信
$af_mail_table = "af_mail";
define("T_AF_MAIL", $af_mail_table);	//	add ookawara 2013/07/18

//	イメージフォルダー名
$IMAGE  = "image";
define("IMAGE", $IMAGE);	//	add ookawara 2013/07/18

//	商品１枚目画像フォルダー名
$IMAGEF = "imagef";
define("IMAGEF", $IMAGEF);	//	add ookawara 2013/07/18

//	商品２枚目画像フォルダー名
$IMAGEB = "imageb";
define("IMAGEB", $IMAGEB);	//	add ookawara 2013/07/18

//	add ookawara 2013/07/04
//	商品3枚目画像フォルダー名
define("IMAGE03", "image03");
//	商品4枚目画像フォルダー名
define("IMAGE04", "image04");
//	商品5枚目画像フォルダー名
define("IMAGE05", "image05");

//	add ookawara 2015/09/28
//	商品6枚目画像フォルダー名
define("IMAGE06", "image06");
//	商品7枚目画像フォルダー名
define("IMAGE07", "image07");
//	商品8枚目画像フォルダー名
define("IMAGE08", "image08");
//	商品9枚目画像フォルダー名
define("IMAGE09", "image09");

//	ブランドファイル
$b_file = "./data/brand.dat";
define("BRAND_FILE", $b_file);	//	add ookawara 2013/07/22

//	商品ページ雛形ファイル
$hina_file = "./sub/hinagata.inc";
$new_hina_file = "./sub/new_hinagata.inc";

//	検索時最大表示件数
$search_view = 15;

//	アフェリエイトcookie保存期間
$aff_time = 60 * 60 * 24 * 30;

//	アフェリエイトポイント確定時期（1ヶ月)
$set_point_mon = 1;

//	アフェリエイト返金可能ポイント
$change_point = 5000;



//	add ookawara 2016/12/02
//	description共通メッセージ
define("HEADDESCRIPTION", " サッカーショップ ネイバーズスポーツ");

?>
