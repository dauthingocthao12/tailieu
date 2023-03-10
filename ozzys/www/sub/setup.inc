<?PHP
//	設定ファイル

//	サイトURL
$URL = "https://ozzys.jp";

//	商品表示フォルダー
$GOODS_DIR = "goods";

//	ひな形ファイル
$hina_file = "./sub/hinagata.inc";

//	表示データーフォルダー
$LOG_DIR = "log";

//	商品表示数(商品名一覧）３の倍数で入れること
$view1 = 60;

//	商品表示数(同商品名一覧）
$view2 = 20;

//	カテゴリーファイル
$m_cate_file = "category.htm";

//	メーカーファイル
$m_maker_file = "maker.htm";

//	商品名一覧画像最大サイズ
$gpx = 100;
$gpy = 50;

//	表示保存フォルダー
$LOGDATA_DIR = "./log_data";

//	メニューファイル名
$menu_file = "menu.inc";
$menu_file_resp = "menu_resp.inc";

//	ヘッドメッセージファイル名
$headmsg_file = "headmsg.inc";

//	お知らせファイル
$oshirase_file = "oshirase.inc";

//	おすすめファイル
$osusume_file = "osusume.inc";

//	入荷情報ファイル
$news_file = "news.inc";

//	ポイント関係
//	１ポイント得るための金額
$point_a = 0;
//	ポイントを利用してつかえる金額
$point_b = 2000;
//	ポイントの利用単位
$point_c = 50;

//	消費税
// add simon 2019-09-18 消費税の更新
if (is_new_tax()) {
    $tax = 0.10;
}
else {
    $tax = 0.08;
}

//	消費税加算方法
$h_tax = 1;	//	0:消費税別 1:消費税込み

//	環境設定 >>>
$admin_mail = 'info@ozzys.jp'; // default: 本番用

if($_SERVER['SERVER_NAME']!='www.ozzys.jp' && $_SERVER['SERVER_NAME']!='ozzys.jp' && $_SERVER['SERVER_NAME']!='https://ozzys.jp') {
	// 管理用メールアドレス
    //$admin_mail = 'simon@azet.jp';
	//$admin_mail = 'hirose@azet.jp';
	$admin_mail = 'ookawara@azet.jp';
	$admin_mail = 'info@ozzys.jp';

    // クロネコWebcollect用
    //define('WEBCOLLECT_URL', '/_webcollect/test_responses.php'); // ローカル
    define('WEBCOLLECT_URL', 'https://ptwebcollect.jp/test_gateway/settleSelectAction.gw'); // 開発

    // クロネコWebcollect返事URL
    //define('WEBCOLLECT_AUTO_URL', 'http://ozzys.local/endhtml/order_card_auto.php'); // ローカル環境
    define('WEBCOLLECT_AUTO_URL', 'https://ozzys.jp/endhtml/order_card_auto_dev.php'); // テスト用（本番環境で開発のDBにつなげている）
}
else {
    // クロネコWebcollectURL
    define('WEBCOLLECT_URL', 'https://payment.kuronekoyamato.co.jp/webcollect/settleSelectAction.gw');

    // クロネコWebcollect返事URL
    define('WEBCOLLECT_AUTO_URL', 'https://ozzys.jp/endhtml/order_card_auto.php');
}
//echo "Admin mail= $admin_mail";
// <<<

// メール送信者名
$admin_name = 'ozzys';

//	メールフッタ
$m_footer = <<<OZZYS
*/*/*/*/*/*/*/*/*/*/*/*/*/
Fishing Pro Shop OZZYS

〒373-0853
群馬県太田市浜町63-31
TEL    : 0276-49-2021
URL    : https://ozzys.jp/
E-mail : $admin_mail
*/*/*/*/*/*/*/*/*/*/*/*/*/
OZZYS;

//	売れ筋BEST10集計日数
$kikan = 7;


//	ページ拡張子
$index = "";


/**
 * 今日は、新しい消費税の更新日を超えたか確認
 * add simon 2019-09-18
 *
 * @return bool
 */
function is_new_tax() {
//    $change_date = 20190918; // DEBUG
    $change_date = 20191001; // 本番

    $today = (int) date("Ymd");

    return $today >= $change_date;
}

?>
