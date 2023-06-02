<?PHP

if(function_exists('is_new_tax')==false) { // add simon 2021/02/09 複数インクルードされない為
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
} // add simon 2021/02/09 複数インクルードされない為

$PRF_N = array("------","北海道","青森県","岩手県","宮城県","秋田県","山形県","福島県","茨城県","栃木県","群馬県",
	"埼玉県","千葉県","東京都","神奈川県","新潟県","富山県","石川県","福井県","山梨県","長野県",
	"岐阜県","静岡県","愛知県","三重県","滋賀県","京都府","大阪府","兵庫県","奈良県","和歌山県",
	"鳥取県","島根県","岡山県","広島県","山口県","徳島県","香川県","愛媛県","高知県","福岡県",
	"佐賀県","長崎県","熊本県","大分県","宮崎県","鹿児島県","沖縄県");

//	del ohkawara 2017/02/24
//$UN_N = array("","739","739","739","739","739","739","739","739","739","739",
//	"739","739","739","739","739","739","739","739","739","739",
//	"739","739","739","739","739","739","739","739","739","739",
//	"739","739","739","739","739","739","739","739","739","739",
//	"739","739","739","739","739","739","739");

//	add ohkawara 2017/02/24
$UN_N = array("","0","0","0","0","0","0","0","0","0","0",
	"0","0","0","0","0","0","0","0","0","0",
	"0","0","0","0","0","0","0","0","0","0",
	"0","0","0","0","0","0","0","0","0","0",
	"0","0","0","0","0","0","0");

$SEX_N = array("","男性","女性");

$MERUMA_N = array("","購読する","購読しない");

$R_POINT_N = array("","利用する","利用しない");

// add simon 2019-09-18 消費税の更新
if (is_new_tax()) {
    $TAX_ = 0.1;
}
else {
$TAX_ = 0.08;
}

$TESU_P = 0;	//	代引き手数料

$P_RITU = 5; // (○%)ポイント率

$SEBAN_N = array('無し','ラバー1色','ラバー2色','フロック1色','フロック2色');

$SEBAN_P_N = array('0','1000','1500','1200','1500');

$SENAME_N = array('無し','ラバー','フロック');

$SENAME_P_N = array('0','200','300');

$MUNEBAN_N = array('無し','ラバー','フロック');

$MUNEBAN_P_N = array('0','600','600');

$PANT_N = array('無し','ラバー','フロック');

$PANT_P_N = array('0','600','600');

$BACH_N = array('無し',
				'プレミアリーグバッジ(両袖)',
				'プレミアチャンピオンバッジ(両袖)',
				'チャンピオンズリーグバッジ(右袖)',
				'99/02セリエAバッジ(右袖)',
				'08/10セリエAバッジ(右袖)',
				'10/12セリエAバッジ(右袖)',
				'ブンデスリーガバッジ(右袖)',
				'07アジアカップバッジ(右袖)',
				'11アジアカップバッジ(両袖)',
				'06W杯本選+予選セット/ライセンス(右袖)',
				'10W杯バッジレプリカ版(右袖)',
				'コパバッジ(イタリア)',
				'トロフィーバッジCL(6)',
				'トロフィーバッジCL(7)',
				'トロフィーバッジCL(8)',
				'トロフィーバッジCL(9)');

$BACH_P_N = array('0','2000','2000','2500','1800','4000','4000','1800','1800','3100','2200','2100','1000','1800','1800','1800','1800');

//$DAIBIKI_N = array('10000','30000','100000','300000','500000');				//	del ohkawara 2017/02/24
$DAIBIKI_N = array('3650','10000','30000','100000','300000','500000');	//	add ohkawara 2017/02/24

//$DAIBIKI_P_N = array('300','400','600','1000','2000');						//	del ohkawara 2017/02/24
$DAIBIKI_P_N = array('600','300','400','600','1000','2000');				//	add ohkawara 2017/02/24

$HAITATU_N = array('特になし','午前10時から12時まで','11時から13時まで','12時から14時まで','13時から15時まで','14時から16時まで','15時から17時まで','16時から18時まで','17時から19時まで','18時から20時まで','19時から21時まで');
//	$HAITATU_N = array('特になし','午前10時から12時まで','11時から13時まで','12時から14時まで','13時から15時まで','14時から16時まで','15時から17時まで','16時から18時まで','17時から19時まで','18時から20時まで','19時から21時まで','20時から22時まで','21時から23時まで','22時から24時まで');

$mochi_pri = "1000";

$MERUMA_N = array('','購読する','購読しない');

//$ZAIKO_N = array('選択して下さい。','無制限で待てる。','1ヶ月くらい待てる。','2週間くらい待てる。','キャンセルする。');
$ZAIKO_N = array('選択して下さい。','','1ヶ月くらい待てる。','2週間くらい待てる。','キャンセルする。');

//$SHIHARAI_N = array('選択して下さい。','代金引換','(前払い)銀行振込','クレジットカード決済(クロネコペイメント)','(前払い)コンビニ払い ※請求書発行手数料(158円)と、ご入金が確認できるまで時間が掛かります。');

//	2012/01/15	代引き・後払い・コンビニ一時的停止
//	2012/01/31	銀行振込・代引・クロネコペイメント・後払い決済
$SHIHARAI_N1 = array(
//				'0' => '選択して下さい。',
				'1' => '代金引換(現金支払）',
//				'3' => 'クレジットカード決済(クロネコペイメント)',
				'2' => '(前払い)銀行振込',
//				'4' => '(前払い)コンビニ払い ※請求書発行手数料(158円)と、ご入金が確認できるまで時間が掛かります。',
				'5' => '後払い決済(後払い.com)',	//	add ookawara 2010/12/10
				'7' => 'PAY.JP(クレジットカード）',
				'6' => 'paypal(クレジットカード）'	//	add ookawara 2014/02/26
				);
//	セール用
$SHIHARAI_N2 = array(
//				'0' => '選択して下さい。',
				'1' => '代金引換(現金支払）',
//				'3' => 'クレジットカード決済(クロネコペイメント)'
//				'3' => 'クレジットカード決済(クロネコｗｅｂコレクト)',
				'5' => '後払い決済(後払い.com)',	//	add ookawara 2010/12/10
				'7' => 'PAY.JP(クレジットカード）',
				'6' => 'paypal(クレジットカード）'	//	add ookawara 2014/02/26
				);

//	後払い手数料％
$atobarai = "5";
define("atobarai", $atobarai);

//	コンビニ払い手数料
$CON_TESU = 150;

// 以下の設定はcone.incに移動されています
//$admin_mail = 'orders@futboljersey.com';
//$admin_mail_m = 'member@futboljersey.com';
//$admin_mail_l = 'mailinglist@futboljersey.com';
//$admin_mail_t = 'team@futboljersey.com';
//$admin_mail_a = 'affiliate@futboljersey.com';

$admin_name = 'NEIGHBOURS SPORTS';

$m_footer = <<<ALPHA
/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_

   株式会社ゼロアワー/ネイバーズスポーツ
　【定休日】毎週水曜日
　【URL】 $admin_url
　【住所】〒771-1262　徳島県板野郡藍住町笠木字中野72-1
　【電話】088-677-3170
　【FAX】 088-692-7031
　【E-mail】query@futboljersey.com

/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_

ALPHA;

//	割引会員番号（○番まで）0なら設定無し
$wa_member = 1071;

//	会員割引率（○％）0なら割引率無し
$waribiki = 0;

//	非会員割引率（○％）0なら割引率無し
$waribiki2 = 0;

//	ポイント利用可・不可
$point_riyou = 0;	//	0：利用可 1：利用不可

//	最低利用可能ポイント
//	add ookawara 2009/10/04
//define("MIN_POINT","1000");
define("MIN_POINT","1");	//	set ookawara 2012/01/31

//	購入金額割引	籠内割引（全ての商品について割引になる）
$DISCOUNT_C = 0;	//	0：割引無し　1：割引有り
$DISCOUNT = array(array('5249','25'),array('10499','25'),array('31499','25'),array('52499','25'));

//	商品毎割引
$GOODS_DISCOUNT_C = 1;	//	0：割引無し　1：割引有り
$GOODS_DISCOUNT_CATE = array();	//	割引をしない上位カテゴリ入力複数ある時はカンマ区切りで
$DISCOUNT_PAR = 50;	//	割引率
$DISCOUNT_PAR2 = 50;	//	定価・割引共通商品割引率

//	支払い方法
$pay_types = 1;		//	1:通常	2:セール時
if ($pay_types == 2) { $SHIHARAI_N = $SHIHARAI_N2; } else { $SHIHARAI_N = $SHIHARAI_N1; }

//	送料無料設定
//	円以上無料　設定をしない場合は必ず何も入れないでください。
//	0を入力した場合は、全て無料になってしまいます。
//$free_shipping = "10500";
//$free_shipping = "5250";	//	set ookawara 2011/11/16
//$free_shipping = "3240";	//	set ookawara 2014/03/31	//	del ohkawara 2017/02/24
//$free_shipping = "1620";	//	set ookawara 2014/10/14
$free_shipping = "0";		//	set ohkawara 2017/02/24


//	支払い方法制限	0:制限しない　1:制限する
//	2012/01/05	一時的制限解除
$pay_limited = 0;

//	index.html追加
$index = "";

//	アフェリエイト還元率（○％）
$aff_ritsu = 5;

//	返還種類
$CHENGE_TYPE_L = array('','ネイバーズスポーツ割引ポイントに返還する。','ご指定の口座にお振り込みをする。');
$CHENGE_TYPE2_L = array('','割引ポイント','口座にお振り込み');

//	振込先科目
$DEPOSIT_L = array('','普通','当座');

//	ネーム付きコメント
$attention = <<<WAKABA

<span style='color:red;'>マーキングの商品は、7～14日間程度お時間を頂く場合がございます。
プリント済みの場合は即納可能です。</span>
WAKABA;

//	楽天　wowmaで利用
$attention_wowma = <<<WAKABA
<font color=#FF0000>マーキングの商品は、7～14日間程度お時間を頂く場合がございます。
プリント済みの場合は即納可能です。</font>
WAKABA;

//	モバイル用
$attention2 = <<<WAKABA
マーキングの商品は、7～14日間程度お時間を頂く場合がございます。
WAKABA;

//	通常納期（楽天用）
//	add ookawara 2015/01/15
$nouki_days_msg = <<<WAKABA

<span style='color:red;'>通常翌営業日発送（一部例外あり）</span>
WAKABA;

//	モバイル用
$nouki_days_msg2 = <<<WAKABA
通常翌営業日発送（一部例外あり）
WAKABA;



//	ssl用
define("HTTP",$http);
define("HTTPS",$https);

//	後払い最低手数料	//	add ookawara 2011/01/20
//
define("ato_low_price", "250");

//	ポイントゲットシステム	//	add ookawara 2009/09/29
//	会員新規登録
define("MEM_REG_START","2009/10/05");	//	イベント開始日
define("MEM_REG_END","2011/12/31");		//	イベント終了日
define("MEM_REG_POINT","999");			//	ポイント

//	ポイントゲットページ
define("POINT_GET_START","2009/10/05");	//	イベント開始日時
define("POINT_GET_END","2010/12/31");	//	イベント終了日時
define("POINT_GET_POINT","500");		//	ポイント
define("GET_POINT_NUM","1");			//	イベント番号
define("LAST_KOJIN_NUM","1924");			//	ポイントを付与する最終会員番号
//	イベント履歴
//	番号：1、期間：2009/10/01-2010/12/31


//	NEWカテゴリー
$NEWMAINCATE = array('',
				'0' => '2022カタール大会',
				'1' => '新入荷',
				'2' => '特価商品',
				'3' => 'クラブチーム',
				'4' => 'Ｊリーグ',
				'5' => 'ナショナルチーム',
				'6' => 'フットサル',
				'7' => 'トレーニング',
				'8' => 'ゴールキーパー',
				'9' => 'レフェリー',
				'10' => 'ジュニア',
				'11' => 'ストリート',
				'12' => 'シューズ',
				'13' => 'バッグ',
				'14' => 'ボール',
				'15' => 'アクセサリー',
				'16' => '2010南アフリカ共和国大会',
				'17' => '2014ブラジル大会',
				'18' => '限定入荷一点限り',
				'19' => '2018ロシア大会',
				'20' => '日本女子代表(なでしこジャパン) '
			);

$NEWCATE[20]['name'] = $NEWMAINCATE[1];
$NEWCATE[20]['url'] = "/goods/20/1/1/";


$NEWCATE[23]['name'] = $NEWMAINCATE[2];
$NEWCATE[23]['url'] = "/goods/23/1/1/";


$NEWCATE[24]['name'] = $NEWMAINCATE[18];	//	add ookawara 2014/12/26
$NEWCATE[24]['url'] = "/goods/24/";		//	add ookawara 2014/12/26


$NEWCATE[26]['name'] = $NEWMAINCATE[3];
$NEWCATE[26]['url'] = "/good/3.htm";

$NEWCATE[29]['name'] = $NEWMAINCATE[3];
$NEWCATE[29]['url'] = "/good/3.htm";

$NEWCATE[32]['name'] = $NEWMAINCATE[3];
$NEWCATE[32]['url'] = "/good/3.htm";

$NEWCATE[33]['name'] = $NEWMAINCATE[3];
$NEWCATE[33]['url'] = "/good/3.htm";

$NEWCATE[35]['name'] = $NEWMAINCATE[3];
$NEWCATE[35]['url'] = "/good/3.htm";


$NEWCATE[38]['name'] = $NEWMAINCATE[4];
$NEWCATE[38]['url'] = "/goods/38/";


$NEWCATE[41]['name'] = $NEWMAINCATE[5];
$NEWCATE[41]['url'] = "/good/5.htm";

$NEWCATE[44]['name'] = $NEWMAINCATE[5];
$NEWCATE[44]['url'] = "/good/5.htm";

$NEWCATE[47]['name'] = $NEWMAINCATE[5];
$NEWCATE[47]['url'] = "/good/5.htm";

$NEWCATE[50]['name'] = $NEWMAINCATE[5];
$NEWCATE[50]['url'] = "/good/5.htm";

$NEWCATE[53]['name'] = $NEWMAINCATE[5];
$NEWCATE[53]['url'] = "/good/5.htm";

$NEWCATE[54]['name'] = $NEWMAINCATE[5];
$NEWCATE[54]['url'] = "/good/5.htm";

$NEWCATE[55]['name'] = $NEWMAINCATE[5];
$NEWCATE[55]['url'] = "/good/5.htm";

$NEWCATE[47][10]['name'] = $NEWMAINCATE[5];
$NEWCATE[47][10]['url'] = "/goods/47/10/";

$NEWCATE[57]['name'] = $NEWMAINCATE[5];
$NEWCATE[57]['url'] = "/good/5.htm";

$NEWCATE[58]["name"] = $NEWMAINCATE[5];
$NEWCATE[58]["url"] = "/good/5.htm";




$NEWCATE[56]['name'] = $NEWMAINCATE[6];
$NEWCATE[56]['url'] = "/goods/56/";


$NEWCATE[59]['name'] = $NEWMAINCATE[7];
$NEWCATE[59]['url'] = "/goods/59/";


$NEWCATE[62]['name'] = $NEWMAINCATE[8];
$NEWCATE[62]['url'] = "/goods/62/";


$NEWCATE[65]['name'] = $NEWMAINCATE[9];
$NEWCATE[65]['url'] = "/goods/65/";


$NEWCATE[68]['name'] = $NEWMAINCATE[10];
$NEWCATE[68]['url'] = "/good/10.htm";

$NEWCATE[71]['name'] = $NEWMAINCATE[10];
$NEWCATE[71]['url'] = "/good/10.htm";

$NEWCATE[80]['name'] = $NEWMAINCATE[10];
$NEWCATE[80]['url'] = "/good/10.htm";


$NEWCATE[74]['name'] = $NEWMAINCATE[11];
$NEWCATE[74]['url'] = "/goods/74/";


$NEWCATE[77]['name'] = $NEWMAINCATE[12];
$NEWCATE[77]['url'] = "/good/12.htm";

$NEWCATE[80]['name'] = $NEWMAINCATE[12];
$NEWCATE[80]['url'] = "/good/12.htm";


$NEWCATE[83]['name'] = $NEWMAINCATE[13];
$NEWCATE[83]['url'] = "/goods/83/";


$NEWCATE[86]['name'] = $NEWMAINCATE[14];
$NEWCATE[86]['url'] = "/good/14.htm";

$NEWCATE[87]['name'] = $NEWMAINCATE[14];
$NEWCATE[87]['url'] = "/good/14.htm";

$NEWCATE[89]['name'] = $NEWMAINCATE[14];
$NEWCATE[89]['url'] = "/good/14.htm";


$NEWCATE[92]['name'] = $NEWMAINCATE[15];
$NEWCATE[92]['url'] = "/goods/92/";

define("pay_limited",$pay_limited);
define("free_shipping",$free_shipping);

