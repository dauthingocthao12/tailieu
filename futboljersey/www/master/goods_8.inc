<?PHP

//	設定項目

//	最上階層にカテゴリーを追加した場合、DBに追加とarray.incファイル追加を忘れずに

//	2009/10/30	楽天のみ消費税別で金額設定

	//	PHP設定一時変更	//add ohkawara 2018/08/31
	ini_set('memory_limit', '2048M');	//	update ookawara 2019/11/15	//	update ookawara 2018/08/31

	include("../sub/array.inc");
	include ("../../cone.inc");
	include_once("../include/movie_list.php");	//	add ookawara 2014/10/10


	//	添付フォルダー
	$DIR_TEMP = "./temp";


	//	割引設定
	//	新YAHOO --------------------------------------------------------------------------------------------
	//	0：割引無し　1：割引有り
	define("NEW_YAHOO_GOODS_DISCOUNT_C","1");
	//	割引率
	define("NEW_YAHOO_DISCOUNT_PAR","50");
	//	定価・割引共通商品割引率
	define("NEW_YAHOO_DISCOUNT_PAR2","50");
	//	割引をしない上位カテゴリ入力、複数ある時はカンマ区切り
	$NEW_YAHOO_GOODS_DISCOUNT_CATE = array();
	//	セール期間
	define("sale_period_start","2008030100");
	define("sale_period_end",  "2008040212");
	//define("sale_period_start","2008030409");
	//define("sale_period_end","2008031012");
	//define("YAHOO_DISCOUNT_TIME","2/15～2/19");
	//	送料無料表示設定　送料が無料になる金額を設定
	define("YAHOO_SOURYOU_PRICE","5500");
	//	送料無料メッセージ
	define("YAHOO_SOURYOU_MESSAGE","★送料無料★");
	//define("YAHOO_SOURYOU_MESSAGE","");
	//	送料　★税込○円以上お買い上げで送料無料★メッセージ
	define("YAHOO_ATO_SOURYOU_MESSAGE","★税抜5,500円以上で送料無料★");
	//	在庫1商品表記	//	add ookawara 2014/09/22
	define("YAHOO_ZIKO1_MESSAGE","残りあと一つ！お急ぎください！");

	//	通常商品在庫数	//	add ohkawara 2017/04/10
	define("YAHOO_DEFO_ZAIKO", 10);


	//	楽天 ------------------------------------------------------------------------------------------------

	//	税抜5,500円以上で送料無料月間
	//	●1,2,3,8,9,10,11
	//	★税抜5,500円以上で送料無料★

	//	税抜7,500円以上で送料無料月間
	//	●4,5,6,7,12
	//	★税抜7,500円以上で送料無料★

	//	0：割引無し　1：割引有り
	define("RAKUTEN_GOODS_DISCOUNT_C","1");
	//	割引率
	define("RAKUTEN_DISCOUNT_PAR","30");
	//	定価・割引共通商品割引率
	define("RAKUTEN_DISCOUNT_PAR2","30");
	//	割引をしない上位カテゴリ入力、複数ある時はカンマ区切り
	$RAKUTEN_GOODS_DISCOUNT_CATE = array();
	//	割引期間（セール用）
//	define("RAKUTEN_DISCOUNT_TIME","2/15～2/19");
	//	送料無料表示設定　送料が無料になる金額を設定
	define("RAKUTEN_SOURYOU_PRICE","7500");
	//	送料無料メッセージ
//	define("RAKUTEN_SOURYOU_MESSAGE","【送料無料】");
	define("RAKUTEN_SOURYOU_MESSAGE","");
	//	送料　★税込○円以上お買い上げで送料無料★メッセージ	カンマ（,）入れるとエラーになるので注意
//	define("RAKUTEN_ATO_SOURYOU_MESSAGE","★税抜7，500円以上で送料無料★");
	define("RAKUTEN_ATO_SOURYOU_MESSAGE","");
	//	楽天キーワード
//	$rakuten_keyword_tu = "【10】";	//	通常
//	$rakuten_keyword_te = "【02】";	//	定価
	$rakuten_keyword_tu = "";	//	通常
	$rakuten_keyword_te = "";	//	定価
	define("RAKUTEN_KEYWORD_TU",$rakuten_keyword_tu);
	define("RAKUTEN_KEYWORD_TE",$rakuten_keyword_te);


	//	楽天ポイントセット
	//	ポイント
	//	通常商品
	define("RAKUTEN_POINT_TU","10");

	//	定価商品
	define("RAKUTEN_POINT_TE","2");

	//	期間	最大○日まで設定可能
	$last_point_time = "2012012709";	//	2012年01月27日9時にセット	イベントがあったら必ずのること
//	$nows_time = time()+(60*60*25);
//	$start_point_time = date("YmdH",$nows_time);
	$start_point_time = "2011122311";
	if ($last_point_time > $start_point_time) {
		$set_point_time = $start_point_time."_".$last_point_time;
	}
	define("RAKUTEN_POINT_KIKAN",$set_point_time);

	//	新規セット
	define("RAKUTEN_POINT_NEW","0");	//	0:未設定	1:設定

	//	通常商品在庫数
	define("RAKUTEN_DEFO_ZAIKO", 10);

	//	Amazon ------------------------------------------------------------------------------------------------
	//	add ookawara 2015/06/26
/*
	出品ファイル（すでにAmazon.co.jp のカタログに登録されている商品を一括でアップロードするためのファイル）
	カテゴリー別在庫ファイル（Amazon.co.jp のカタログにない商品を登録したい場合）
		apparelとshoesは一緒
		sports

検索キーワード

カスタマーがAmazon.co.jpサイト上で商品を探す第一の手段がキーワードによる検索です。キーワードは、1商品に対して原則最大5個まで設定できます。検索をするカスタマーの気持ちになり、検索キーワードを設定してください。

*/

	//	既存Amazon出品レポートファイル
	define("AMAZON_B_ITEM_FILE","$DIR_TEMP/amazon.txt");

	//	Amazonファッション＆シューズ在庫アップロードファイル
	define("AMAZON_AP_ITEM_FILE","$DIR_F/amz_ap_import.txt");

	//	Amazonスポーツ在庫アップロードファイル
	define("AMAZON_SP_ITEM_FILE","$DIR_F/amz_sp_import.txt");

	//	Amazon出品ファイルアップロードファイル
	define("AMAZON_JAN_ITEM_FILE","$DIR_F/amz_jan_import.txt");

	//	0：割引無し　1：割引有り
	define("AMAZON_GOODS_DISCOUNT_C","1");

	//	割引率
	define("AMAZON_DISCOUNT_PAR","20");

	//	定価・割引共通商品割引率
	define("AMAZON_DISCOUNT_PAR2","20");

	//	割引をしない上位カテゴリ入力、複数ある時はカンマ区切り
	$AMAZON_GOODS_DISCOUNT_CATE = array();

	//	通常商品在庫数
	define("AMAZON_DEFO_ZAIKO", 2);

	//	マーキングユニフォーム出荷日数
	define("AMAZON_MARK_DAYS", 5);

	//	配送日時指定SKUリスト名（アマゾンの管理画面で設定した名）
	define("AMAZON_DEL_SCHE_GR_ID", "お届け日時指定");

	//	ポイント率(%)
	define("AMAZON_POINT", 1);


	//	add ohkawara 2017/02/09
	//	Wowma ------------------------------------------------------------------------------------------------

	//	既存Wowma出品済みファイル
	//define("WOWMA_B_ITEM_FILE","$DIR_TEMP/wowma.csv");	//	del ohkawara 2018/04/03

	//	既存Wowma出品済み商品ファイル
	define("WOWMA_B_ITEM_FILE","$DIR_TEMP/item_wowma.csv");	//	add ohkawara 2018/04/03

	//	既存Wowma出品済み在庫ファイル
	define("WOWMA_B_STOCK_FILE","$DIR_TEMP/stock_wowma.csv");	//	add ohkawara 2018/04/03

	//	Wowma商品CSV
	define("WOWMA_ITEM_FILE","$DIR_F/w_item.csv");		//	add ohkawara 2018/04/09
	//define("WOWMA_ITEM_FILE","$DIR_F/sku_data.csv");	//	del ohkawara 2018/04/09

	//	Wowma在庫CSV
	define("WOWMA_ZAIKO_FILE","$DIR_F/stock.csv");			//	add ohkawara 2018/04/09
	//define("WOWMA_ZAIKO_FILE","$DIR_F/sku_stock.csv");	//	del ohkawara 2018/04/09

	//	0：割引無し　1：割引有り
	define("WOWMA_GOODS_DISCOUNT_C","1");

	//	割引率
	define("WOWMA_DISCOUNT_PAR","20");

	//	定価・割引共通商品割引率
	define("WOWMA_DISCOUNT_PAR2","20");

	//	割引をしない上位カテゴリ入力、複数ある時はカンマ区切り
	$WOWMA_GOODS_DISCOUNT_CATE = array();

	//	通常商品在庫数
	define("WOWMA_DEFO_ZAIKO", 2);

	//	マーキングユニフォーム出荷日数
	define("WOWMA_MARK_DAYS", 5);

	//	ポイント率(%)
	define("WOWMA_POINT", 1);

	// -------------------------------------------------------------------------------------------------------------------


	//	最終更新ファイル
	define("CHECK_FILE","$DIR_F/checktime.dat");

	//	最終更新ファイル
	define("CHECK_TMP_FILE","$DIR_F/check_tmp_time.dat");


	//	新YAHOO用CSVファイル
	define("NEW_YAHOO_ALL_FILE","$DIR_F/data.csv");
	//	新YAHOO用在庫CSVファイル
	//	add ookawara 2014/07/31
	define("NEW_YAHOO_ZAIKO_FILE","$DIR_F/zaiko.csv");
	//	旧YAHOO用CSVファイル
	define("YAHOO_ALL_FILE","$DIR_F/old_data.csv");


	//	楽天商品CSV
	define("RAKUTEN_ITEM_FILE","$DIR_F/item.csv");
	//	楽天選択枠CSV
	define("RAKUTEN_SELECT_FILE","$DIR_F/select.csv");
	//	楽天カテゴリーCSV
	define("RAKUTEN_CAT_FILE","$DIR_F/item-cat.csv");

	//	add ookawara 2015/06/26
	//	Amazon商品CSV
	define("AMAZON_ITEM_FILE","$DIR_F/amazon_item.txt");
	//	Amazon在庫CSV
	define("AMAZON_ZAIKO_FILE","$DIR_F/amazon_zaiko.txt");

	//	既存楽天商品CSV
	define("RAKUTEN_B_ITEM_FILE","$DIR_TEMP/item.csv");
	//	既存楽天選択枠CSV
	define("RAKUTEN_B_SELECT_FILE","$DIR_TEMP/select.csv");
	//	既存楽天カテゴリーCSV
	define("RAKUTEN_B_CAT_FILE","$DIR_TEMP/item-cat.csv");

	//	アップ用画像保存フォルダー
	define("UP_IMG_DIR","$DIR_F/images");
	//	メイン画像
	define("UP_IMG_01_DIR", UP_IMG_DIR."/image01");
	//	サブ画像
	define("UP_IMG_02_DIR", UP_IMG_DIR."/image02");

	//	add ookawara 2013/07/03
	define("UP_IMG_03_DIR", UP_IMG_DIR."/image03");
	define("UP_IMG_04_DIR", UP_IMG_DIR."/image04");
	define("UP_IMG_05_DIR", UP_IMG_DIR."/image05");
	define("UP_IMG_06_DIR", UP_IMG_DIR."/image06");		//	add ookawara 2015/09/27
	define("UP_IMG_07_DIR", UP_IMG_DIR."/image07");		//	add ookawara 2015/09/27
	define("UP_IMG_08_DIR", UP_IMG_DIR."/image08");		//	add ookawara 2015/09/27
	define("UP_IMG_09_DIR", UP_IMG_DIR."/image09");		//	add ookawara 2015/09/27

	//	商品未アップブランド番号	//	add ookawara 2015/01/08
	$NON_BRAND = array();
	//$NON_BRAND[99] = 1;	//	クロックス	//	add ookawara 2015/01/08	//	del ookawara 2015/03/12

	//	画像保存フォルダー
	//	メイン画像
	define("IMG_F_DIR","../imagef");
	//	サブ画像
	define("IMG_B_DIR","../imageb");

	//	add ookawara 2013/07/03
	define("IMG_03_DIR","../image03");
	define("IMG_04_DIR","../image04");
	define("IMG_05_DIR","../image05");
	define("IMG_06_DIR","../image06");	//	add ookawara 2015/09/27
	define("IMG_07_DIR","../image07");	//	add ookawara 2015/09/27
	define("IMG_08_DIR","../image08");	//	add ookawara 2015/09/27
	define("IMG_09_DIR","../image09");	//	add ookawara 2015/09/27

	define("IMG_F_AMZ_DIR","../imagef_amz");	//	add ookawara 2015/09/27
	define("IMG_B_AMZ_DIR","../imageb_amz");	//	add ookawara 2015/09/27
	define("IMG_03_AMZ_DIR","../image03_amz");	//	add ookawara 2015/09/27
	define("IMG_04_AMZ_DIR","../image04_amz");	//	add ookawara 2015/09/27
	define("IMG_05_AMZ_DIR","../image05_amz");	//	add ookawara 2015/09/27
	define("IMG_M_AMZ_DIR","../image_main_amz");	//	add ohkawara 2018/10/23


	//	ブランドファイル
	define("BRAND_FILE","../data/brand.dat");
	//	他サイトカテゴリーファイル
	define("YPATH_FILE","./data/ypath.dat");

	//	sqlテーブル
	define("T_CATEGORY","category");
	define("T_GOODS","goods");
	define("T_DELGOODS","delgoods");
	define("T_R_CATE","r_cate");
	define("T_MOOL_RT","mool_rt");	//	楽天タグ			//	add ohkawara 2017/02/21
	define("T_MOOL_RD","mool_rd");	//	楽天ディレクトリ	//	add ohkawara 2017/02/22

	//	画像更新テーブル	define("T_UP_PIC", "up_pic");


	//	割引設定
	//	旧YAHOO
	//	0：割引無し　1：割引有り
	define("YAHOO_GOODS_DISCOUNT_C","1");
	//	割引率
	define("YAHOO_DISCOUNT_PAR","10");
	//	定価・割引共通商品割引率
	define("YAHOO_DISCOUNT_PAR2","10");
	//	割引をしない上位カテゴリ入力、複数ある時はカンマ区切り
	$YAHOO_GOODS_DISCOUNT_CATE = array();



//	csv作成
function goods_8() {

	if ($_POST['mode'] == "make") {
		list($msg,$ERROR) = make_csv();
	} elseif ($_POST['mode'] == "ファイル削除") {
		list($msg,$ERROR) = del();
	} elseif ($_POST['mode'] == "更新データー削除") {
		list($msg,$ERROR) = del_renew();
	} elseif ($_POST['mode'] == "全商品更新") {
		list($msg,$ERROR) = all_renew();
	}

	$html = first00($msg,$ERROR);

	echo($html);

}



function first00($msg,$ERROR){
// global	$PHP_SELF,$tax,$taxable,$tokka,$delpass;
global	$PHP_SELF;
	global $TAX_;	//	add ookawara 2014/03/31

	if ($ERROR) { ERROR($ERROR); }

	//if ($_POST['tax'] == "") { $_POST['tax'] = 5; }
	if ($_POST['tax'] == "") { $_POST['tax'] = $TAX_ * 100; }
	//if ($_POST['taxable'] == "" || $_POST['taxable'] == 1) { $ta_checked = "checked"; } else { $ta_checked = ""; }	//	del ookawara 2014/03/11
	$ta_checked = "";											//	add ookawara 2014/03/11
	if ($_POST['taxable'] == 1) { $ta_checked = "checked"; }	//	add ookawara 2014/03/11
	if ($_POST['tokka'] == "" || $_POST['tokka'] == 1) { $tk_checked = "checked"; } else { $tk_checked = ""; }

	$set_point_time = RAKUTEN_POINT_KIKAN;

	$html = <<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="{$_POST['main']}">
<INPUT type="hidden" name="goods" value="{$_POST['goods']}">
<INPUT type="hidden" name="mode" value="make">
<!--Ypathを更新して、下記を設定し作成を押して下さい。<br>-->
<TABLE border="0" cellspacing="1" cellpadding="4" bgcolor="#666666">
  <TBODY>
    <TR bgcolor="#ffffff">
      <TD bgcolor="#cccccc">金額に消費税も上乗せする(yahoo)</TD>
      <TD><INPUT type="checkbox" name="taxable" value="1" $ta_checked>：上乗せする</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TD bgcolor="#cccccc">特価の金額を載せる</TD>
      <TD><INPUT type="checkbox" name="tokka" value="1" $tk_checked>：載せる</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TD bgcolor="#cccccc">消費税</TD>
      <TD><INPUT size="4" type="text" maxlength="2" name="tax" value="{$_POST['tax']}">%</TD>
    </TR>
    <TR bgcolor="#ffffff">
      <TD colspan="2" align="center"><INPUT type="submit" value="作成">　<INPUT type="reset"></TD>
    </TR>
  </TBODY>
</TABLE>
</FORM>
<A href="./yahoo_henkan.php" target="_blank">YahooSHOPお勧め商品 全角→半角プログラム</A><br>
楽天ポイント設定終了期日：{$set_point_time}<br>

WAKABA;

	if (!file_exists(YAHOO_ALL_FILE)
		 && !file_exists(RAKUTEN_ITEM_FILE)
		 && !file_exists(RAKUTEN_SELECT_FILE)
		 && !file_exists(RAKUTEN_CAT_FILE)
		 && !file_exists(AMAZON_AP_ITEM_FILE)
		 && !file_exists(AMAZON_SP_ITEM_FILE)
		 && !file_exists(AMAZON_JAN_ITEM_FILE)
		 && !file_exists(WOWMA_ITEM_FILE)
		 && !file_exists(WOWMA_ZAIKO_FILE)
	) {
		echo("今現在、CSVデーターはありません。<br>\n");
	} else {


		//	新yahoo
		if (file_exists(NEW_YAHOO_ALL_FILE)) {
			$new_yahoo_all_file = NEW_YAHOO_ALL_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(NEW_YAHOO_ALL_FILE));
			$new_yahoo_msg = <<<WAKABA
ファイル作成日：$ftime<br>
新Yahoo <A href="{$new_yahoo_all_file}" target="_blank">ダウンロード</A><br>
<br>

WAKABA;
		}

		//	新yahoo zaiko
		if (file_exists(NEW_YAHOO_ZAIKO_FILE)) {
			$new_yahoo_zaiko_file = NEW_YAHOO_ZAIKO_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(NEW_YAHOO_ZAIKO_FILE));
			$new_yahoo_zaiko_msg = <<<WAKABA
ファイル作成日：$ftime<br>
新Yahoo在庫 <A href="{$new_yahoo_zaiko_file}" target="_blank">ダウンロード</A><br>
<br>

WAKABA;
		}

		//	旧yahoo
		if (file_exists(YAHOO_ALL_FILE)) {
			$yahoo_all_file = YAHOO_ALL_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(YAHOO_ALL_FILE));
			$yahoo_msg = <<<WAKABA
ファイル作成日：$ftime<br>
旧Yahoo <A href="{$yahoo_all_file}" target="_blank">ダウンロード</A><br>
<br>

WAKABA;
		}


		//	rakuten
		if (file_exists(RAKUTEN_ITEM_FILE)) {
			$rakuten_item_file = RAKUTEN_ITEM_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_ITEM_FILE));
			$rakuten_item_msg = <<<WAKABA
ファイル作成日：$ftime<br>
楽天アイテム <A href="{$rakuten_item_file}" target="_blank">ダウンロード</A><br>
<br>

WAKABA;
		}
		if (file_exists(RAKUTEN_SELECT_FILE)) {
			$rakuten_select_file = RAKUTEN_SELECT_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_SELECT_FILE));
			$rakuten_select_msg = <<<WAKABA
ファイル作成日：$ftime<br>
楽天セレクト <A href="{$rakuten_select_file}" target="_blank">ダウンロード</A><br>
<br>

WAKABA;
		}
		if (file_exists(RAKUTEN_CAT_FILE)) {
			$rakuten_cat_file = RAKUTEN_CAT_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_CAT_FILE));
			$rakuten_cat_msg = <<<WAKABA
ファイル作成日：$ftime<br>
楽天カテゴリー <A href="{$rakuten_cat_file}" target="_blank">ダウンロード</A><br>
<br>

WAKABA;
		}

		//	Amazonファッション＆シューズ在庫
		//	add ookawara 2015/09/25
		if (file_exists(AMAZON_AP_ITEM_FILE)) {
			$amazon_ap_item_file = AMAZON_AP_ITEM_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(AMAZON_AP_ITEM_FILE));
			$amazon_ap_item_msg = "ファイル作成日：".$ftime."<br>\n";
			$amazon_ap_item_msg .= "Amazonファッション＆シューズ在庫 <A href=\"".$amazon_ap_item_file."\" target=\"_blank\">ダウンロード</A><br>\n";
			$amazon_ap_item_msg .= "<br>\n";
		}

		//	Amazonスポーツ在庫
		//	add ookawara 2015/09/25
		if (file_exists(AMAZON_SP_ITEM_FILE)) {
			$amazon_sp_item_file = AMAZON_SP_ITEM_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(AMAZON_SP_ITEM_FILE));
			$amazon_sp_item_msg = "ファイル作成日：".$ftime."<br>\n";
			$amazon_sp_item_msg .= "Amazonスポーツ在庫 <A href=\"".$amazon_sp_item_file."\" target=\"_blank\">ダウンロード</A><br>\n";
			$amazon_sp_item_msg .= "<br>\n";
		}

		//	Amazon出品
		//	add ookawara 2015/06/26
		if (file_exists(AMAZON_JAN_ITEM_FILE)) {
			$amazon_jan_item_file = AMAZON_JAN_ITEM_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(AMAZON_JAN_ITEM_FILE));
			$amazon_jan_item_msg = "ファイル作成日：".$ftime."<br>\n";
			$amazon_jan_item_msg .= "Amazon出品 <A href=\"".$amazon_jan_item_file."\" target=\"_blank\">ダウンロード</A><br>\n";
			$amazon_jan_item_msg .= "<br>\n";
		}


		//	add ohkawara 2017/02/09
		//	Wowma商品
		$wowma_item_msg = "";
		if (file_exists(WOWMA_ITEM_FILE)) {
			$wowma_item_file = WOWMA_ITEM_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(WOWMA_ITEM_FILE));
			$wowma_item_msg = "ファイル作成日：".$ftime."<br>\n";
			$wowma_item_msg .= "Wowma商品 <A href=\"".$wowma_item_file."\" target=\"_blank\">ダウンロード</A><br>\n";
			$wowma_item_msg .= "<br>\n";
		}

		//	Wowma在庫
		$wowma_zaiko_msg = "";
		if (file_exists(WOWMA_ZAIKO_FILE)) {
			$wowma_zaiko_file = WOWMA_ZAIKO_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(WOWMA_ZAIKO_FILE));
			$wowma_zaiko_msg = "ファイル作成日：".$ftime."<br>\n";
			$wowma_zaiko_msg .= "Wowma在庫 <A href=\"".$wowma_zaiko_file."\" target=\"_blank\">ダウンロード</A><br>\n";
			$wowma_zaiko_msg .= "<br>\n";
		}

		$wowma_msg = "";
		if ($wowma_item_msg == "" && $wowma_select_msg == "" && $wowma_zaiko_msg == "") {
			$wowma_msg = "　ファイル無し<br>\n";
		}


		$html .= <<<WAKABA
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="{$_POST['main']}">
<INPUT type="hidden" name="goods" value="{$_POST['goods']}">
<br>
Amazonの情報作成するときは楽天と同じくAmazonサイトから出品レポートをダウンロードしてセットしてから作成して下さい<br>
※Amazon管理サイト ＞ 在庫 ＞ 出品レポート　レポートの種類の選択:出品レポート　を選択してレポートをリクエスト　ボタンをクリックしてファイルを作成する。<br>
<br>
AmazonShopDataをダウンロードする場合は、「ダウンロード」を、右クリック、対象をファイル保存するを選択して下さい。<br>
<br>
<b>YahooShopData</b><br />
$new_yahoo_msg
$new_yahoo_zaiko_msg
$yahoo_msg
<b>RakutenShopData</b><br />
$rakuten_item_msg
$rakuten_select_msg
$rakuten_cat_msg
<b>AmazonShopData</b><br />
$amazon_ap_item_msg
$amazon_sp_item_msg
$amazon_jan_item_msg
<b>WowmaShopData</b><br />
<span style="color:red;">※反映アップ時、item.csvのアップロードが完了後、stock.csvをアップしてください。順番にアップしないと処理されない場合があります。</span><br>
<span style="color:red;">※ファイル名も楽天と同じでitem.csvとなっていますので注意してください。</span><br>
$wowma_item_msg
$wowma_zaiko_msg
$wowma_msg
<br>
<BR>
<INPUT type="submit" name="mode" value="ファイル削除">
<br>
<BR>
更新データー削除パスワード：<INPUT size="10" type="text" name="delpass" value=""><br>
<INPUT type="submit" name="mode" value="更新データー削除">
<br>
<BR>
<!--
<INPUT type="submit" name="mode" value="全商品更新">
-->
</FORM>

WAKABA;

	}

	return $html;

}



function make_csv() {
global $conn_id;

	if ($_POST['taxable'] == 1 && $_POST['tax'] <= 0) { $ERROR[] = "消費税を入力して下さい。"; }

	//	add ookawara 2013/10/10
	if (!$ERROR) {
		$sql  = "UPDATE ".T_CATEGORY." SET".
				" state='2'".
				" WHERE state='0';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "全商品更新データーに変更できませんでした。"; }
	}

	//	既存アップロードファイル削除
	//	2015/09/27
	del();

	//	画像更新チェック
	if (!$ERROR) {
		$ERROR = pic_check();
	}

	//	更新処理時間一時記録ファイル
	//	add ookawara 2013/12/09
	//if (!file_exists(CHECK_TMP_FILE)) {	//	del ookawara 2013/12/17
		$check_time = time();
		$OUT = fopen(CHECK_TMP_FILE,"w");
		fwrite($OUT,$check_time);
		fclose($OUT);
		@chmod(CHECK_TMP_FILE,0666);
	//}	//	del ookawara 2013/12/17

	//	データー読み込み
	if (!$ERROR) {
		define("TAX",$_POST['tax']);
		define("TAXABLE",$_POST['taxable']);
		define("TOKKA",$_POST['tokka']);

		//	カテゴリーデーター
		unset($L_CATE);
		$sql  = "SELECT c_name, cate1, cate2, cate3 FROM ".T_R_CATE.
				" WHERE cate1>='20'".
				" AND cate1 NOT IN ('98','99');";	//	add ookawara 2013/08/23
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				$c_name = $list['c_name'];
				$cate1 = $list['cate1'];
				$cate2 = $list['cate2'];
				$cate3 = $list['cate3'];
				$L_CATE[$cate1][$cate2][$cate3] = $c_name;
			}
		}

		//	ブランドデーター
		if (file_exists(BRAND_FILE)) {
			$B_LIST = file(BRAND_FILE);
			foreach ($B_LIST AS $val) {
				list($b_num_,$b_name_,$del_,$y_num_) = explode("<>",$val);
				$L_BRAND[$b_num_] = $b_name_;
				if ($y_num_ > 0) { $L_Y_BEAND[$b_num_] = $y_num_; }
			}
		} else {
			$ERROR[] = "ブランドファイルを読み込めませんでした。";
		}


		$sql  = "SELECT id, path_name, spec_id, spec_value_id, spec_value_name FROM mool_yd". 
				" WHERE flg='1'".
				" ORDER BY id, spec_id, spec_value_id;";
		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				foreach ($list AS $key => $val) {
					$$key = $val;
				}
				if (!$spec_value_name) { continue; }
				$spec_value_name = moji_seiton($spec_value_name);
				$SPEC_ID_LIST[$id][$spec_id][$spec_value_id] = $spec_value_name;
			}
		}

	}

/*---------------------------------------------------------------------------------------------------------------------*/

	//	画像更新データー
	//	add ookawara 2014/02/06
	$UP_PIC_LIST = array();
	$sql  = "SELECT up.num, up.g_num FROM ".T_UP_PIC." up".
			" GROUP BY up.num, up.g_num;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$num = $list['num'];
			$g_num = $list['g_num'];
			$UP_PIC_LIST[$g_num] = $g_num;
		}
	}

	//	YAHOOデーター作成
	//	商品データー読み込み
	unset($y_csv);
	unset($new_y_csv);
	unset($new_yz_csv);	//	add ookawara 2014/07/31
	unset($YAHOO_LIST);
	unset($YAHOO_GOODS_LIST);
	$sql = "SELECT i.num, i.cate1, i.cate2, i.cate3, i.g_num, i.ypath, i.display, i.copy_num, i.state," .
			//" j.g_name, j.code, j.price, j.sale_price, j.options, j.caption, j.brand, j.headline," .	//	del ookawara 2015/09/24
			" coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name,".	//	add ookawara 2015/09/24
			" j.code, j.price, j.sale_price, j.options, j.caption, j.brand, j.headline," .	//	add ookawara 2015/09/24
			" j.yd_id,".
			//" j.zaiko, j.size_list, j.name_head".			//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
			" j.zaiko, j.size_list".						//	add ookawara 2015/09/24
			" FROM ".T_CATEGORY." i, ".T_GOODS." j" .
			" WHERE i.g_num=j.g_num AND j.yahooshop='1' AND j.soldout!='1' AND i.display='1'" .
			" AND i.cate1>='20' AND i.state<'3'".
			" AND i.cate1 NOT IN ('98','99')".	//	add ookawara 2013/08/23
			" ORDER BY i.cate1, i.cate2, i.cate3 DESC;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$num_ = $list['num'];
			$g_num_ = $list['g_num'];
			$YAHOO_LIST[$num_] = $list;
			$YAHOO_GOODS_LIST[$g_num_][] = $num_;

		}
	}

	if ($YAHOO_LIST) {
		foreach ($YAHOO_LIST AS $num_ => $list) {
			$g_num_ = $list['g_num'];
			if ($CHECK_GOODS_NUM[$g_num_]) { continue; }

			//	カテゴリー作成
			unset($category_list);
			foreach ($YAHOO_GOODS_LIST[$g_num_] AS $key => $num_) {
				$category_list .= make_category_list($num_,$YAHOO_LIST,$YAHOO_GOODS_LIST,$L_CATE);
			}
			$category_list = trim($category_list);

			//	change ookawara 2010/05/25
			$set_csv = "";
			$set_zaiko_csv = "";	//	add ookawara 2014/07/31
			//$set_csv = new_yahoo_csv($list,$L_CATE,$L_BRAND,$L_NEW_YPATH,$L_Y_BEAND,$category_list,$SPEC_ID_LIST, $UP_PIC_LIST);	//	add $UP_PIC_LIST ookawara 2014/02/06	//	del ookawara 2014/07/31
			list($set_csv, $set_zaiko_csv) = new_yahoo_csv($list,$L_CATE,$L_BRAND,$L_NEW_YPATH,$L_Y_BEAND,$category_list,$SPEC_ID_LIST, $UP_PIC_LIST);	//	add ookawara 2014/07/31
			if ($set_csv) {
				$new_y_csv .= $set_csv;
				unset($YAHOO_GOODS_LIST[$g_num_]);
				$CHECK_GOODS_NUM[$g_num_] = 1;
			}
			//	add ookawara 2014/07/31
			if ($set_zaiko_csv) {
				$new_yz_csv .= $set_zaiko_csv;
			}
		}
	}
	if ($new_y_csv) {
		//	add cal ohkawara 2017/04/10	lead-time-instock,lead-time-outstock
		$new_y_csv = "\"path\",\"name\",\"code\",\"sub-code\",\"original-price\",\"price\",\"sale-price\",\"options\",\"headline\",\"caption\",\"abstract\",\"explanation\",\"additional1\",\"additional2\",\"additional3\",\"relevant-links\",\"ship-weight\",\"taxable\",\"release-date\",\"temporary-point-term\",\"meta-key\",\"meta-desc\",\"display\",\"template\",\"sale-period-start\",\"sale-period-end\",\"sale-limit\",\"sp-code\",\"brand-code\",\"person-code\",\"yahoo-product-code\",\"product-code\",\"jan\",\"isbn\",\"delivery\",\"product-category\",\"spec1\",\"spec2\",\"spec3\",\"spec4\",\"spec5\",\"sp-additional\",\"lead-time-instock\",\"lead-time-outstock\"\r\n$new_y_csv";
		$new_y_csv = mb_convert_encoding($new_y_csv,"sjis-win","UTF-8");			//	ohkawara	2022/10/27
		$OUT = fopen(NEW_YAHOO_ALL_FILE,"w");
		fwrite($OUT,$new_y_csv);
		fclose($OUT);
		@chmod(NEW_YAHOO_ALL_FILE,0666);
		$msg = "新YahooCSVファイル作成完了いたしました。<br>\n<br>\n";
	} elseif (file_exists(NEW_YAHOO_ALL_FILE)) {
		unlink(NEW_YAHOO_ALL_FILE);
	}

	//	Yahoo在庫
	//	add ookawara 2014/07/31
	if ($new_yz_csv) {
		$new_yz_csv = "\"code\",\"sub-code\",\"quantity\",\"mode\"\r\n$new_yz_csv";
		$new_yz_csv = mb_convert_encoding($new_yz_csv,"sjis-win","UTF-8");			//	ohkawara	2022/10/24
		$OUT = fopen(NEW_YAHOO_ZAIKO_FILE,"w");
		fwrite($OUT,$new_yz_csv);
		fclose($OUT);
		@chmod(NEW_YAHOO_ZAIKO_FILE,0666);
		$msg .= "新Yahoo在庫CSVファイル作成完了いたしました。<br>\n<br>\n";
	} elseif (file_exists(NEW_YAHOO_ZAIKO_FILE)) {
		unlink(NEW_YAHOO_ZAIKO_FILE);
	}

	if ($y_csv) {
		$y_csv = "Path,Name,Code,Price,Sale-Price,Options,Headline,Caption,Abstract,Relevant-links,Ypath,Ship-weight,TAXABLE\r\n$y_csv";
		$y_csv = mb_convert_encoding($y_csv,"sjis-win","UTF-8");					//	ohkawara	2022/10/24
		$OUT = fopen(YAHOO_ALL_FILE,"w");
		fwrite($OUT,$y_csv);
		fclose($OUT);
		@chmod(YAHOO_ALL_FILE,0666);
		$msg .= "旧YahooCSVファイル作成完了いたしました。<br>\n<br>\n";
	} elseif (file_exists(YAHOO_ALL_FILE)) {
		unlink(YAHOO_ALL_FILE);
	}

	//	楽天商品データー作成
	//	楽天既存商品データー読み込み
	unset($RAKU_ITEM_LIST);
	unset($RAKU_SP_LIST);
	if (file_exists(RAKUTEN_B_ITEM_FILE)) {
		$LIST = file(RAKUTEN_B_ITEM_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		unset($LIST[0]);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$VAL = trim($VAL);
				list($a,$g_num,$a,$rd_id,$a,$a,$a,$a,$a,$r_sale_price) = explode(",",$VAL);	//	add $a, ookawara 2014/03/17	タグIDが追加されずれた為追加
##				$g_num = ereg_replace("[^0-9]","",$g_num);
				$g_num = preg_replace("/[^0-9]/","",$g_num);
##				$r_sale_price = ereg_replace("[^0-9]","",$r_sale_price);
				$r_sale_price = preg_replace("/[^0-9]/","",$r_sale_price);
				if ($g_num) {
					$RAKU_LIST[$g_num] = $VAL;
					if ($r_sale_price) { $RAKU_SP_LIST[$g_num] = $r_sale_price; }

					//	楽天デェレクトリー変更
					//$rd_id = preg_replace("/\"/", "" , $rd_id);
					//if ($rd_id) {
					//	$sql  = "UPDATE ".T_GOODS." SET rd_id='".$rd_id."'".
					//			" WHERE g_num='".$g_num."';";
					//	if (!pg_exec($conn_id,$sql)) { $ERROR[] = "商品オプションデーターを更新できませんでした。$g_num"; }
					//}

				}
			}
		}
	}

	//	楽天サイズデーター読み込み
	unset($RAKU_S_LIST);
	if (file_exists(RAKUTEN_B_SELECT_FILE)) {
		$LIST = file(RAKUTEN_B_SELECT_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		unset($LIST[0]);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$VAL = trim($VAL);
				list($a,$g_num,$a,$a,$size) = explode(",",$VAL);
##				$g_num = ereg_replace("[^0-9]","",$g_num);
				$g_num = preg_replace("/[^0-9]/","",$g_num);
##				$size = ereg_replace("\"","",$size);
				$size = preg_replace("/\"/","",$size);
				$size = trim($size);
				if ($g_num) {
					$RAKU_S_LIST[$g_num][$size] = $VAL;
				}
			}
		}
	}

	//	楽天カテゴリーデーター読み込み
	unset($RAKU_C_LIST);
	if (file_exists(RAKUTEN_B_CAT_FILE)) {
		$LIST = file(RAKUTEN_B_CAT_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		unset($LIST[0]);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
				$VAL = trim($VAL);
				list($a,$g_num,$a,$ctname) = explode(",",$VAL);
##				$g_num = ereg_replace("[^0-9]","",$g_num);
				$g_num = preg_replace("/[^0-9]/","",$g_num);
##				$ctname = ereg_replace("^\"|\"$","",$ctname);
			    $ctname = preg_replace("/^\"|\"$/","",$ctname);
				if ($g_num && $ctname) {
					$RAKU_C_LIST[$g_num][$ctname] = $VAL;
				}
			}
		}
	}

	//	タグID情報取得
	//	add ohkawara 2017/02/21
	$TAG_SIZE_LIST = array();
	$TAG_BRAND_LIST = array();
	$sql = "SELECT * FROM ".T_MOOL_RT.";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$rd_id = $list['rd_id'];						//	add ohkawara 2017/04/06
			$tag_id = $list['tag_id'];
			$tag = $list['tag'];
			$tag_class = $list['tag_class'];

			if (preg_match("/ブランド/", $tag_class)) {
				//$TAG_BRAND_LIST[$tag] = $tag_id;			//	del ohkawara 2017/04/06
				$TAG_BRAND_LIST[$rd_id][$tag] = $tag_id;	//	add ohkawara 2017/04/06
			} else {
				//$TAG_SIZE_LIST[$tag] = $tag_id;			//	del ohkawara 2017/04/06
				$TAG_SIZE_LIST[$rd_id][$tag] = $tag_id;	//	add ohkawara 2017/04/06
			}
		}
	}

	//	楽天ディレクトリ取得
	//	addd ohkawara 2017/02/22
	$RKT_DIR_LIST = array();
	$sql = "SELECT * FROM ".T_MOOL_RD.";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$dir_id = $list['dir_id'];
			$flg = $list['flg'];
			$RKT_DIR_LIST[$dir_id] = $flg;
		}
	}

	//	アイテム登録データー読み込み
	unset($CHECK_ITEM);
	$sql = "SELECT i.num, i.cate1, i.cate2, i.cate3, i.g_num, i.ypath, i.display, i.copy_num, i.state," .
			//" j.g_name, j.code, j.price, j.sale_price, j.options, j.caption, j.brand," . 
			" coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name,".	//	add ookawara 2015/09/24
			" j.code, j.price, j.sale_price, j.options, j.caption, j.brand," . 	//	add ookawara 2015/09/24
			" j.rd_id,".
			//" j.zaiko, j.size_list, j.name_head".			//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
			" j.zaiko, j.size_list".						//	add ookawara 2015/09/24
			" FROM ".T_CATEGORY." i, ".T_GOODS." j" .
			//" WHERE i.g_num=j.g_num AND j.yahooshop='1' AND j.soldout!='1' AND i.display='1'" .		//	del ohkawara 2020/12/07	yahooと楽天の出品の変数が共通だった為
			" WHERE i.g_num=j.g_num AND j.wowmashop='1' AND j.soldout!='1' AND i.display='1'" .			//	add ohkawara 2020/12/07	yahooでナイキが出品が出来なくなったのでwowmaと共通にした。
			" AND i.cate1>='20' AND i.state!='3'".
			" AND i.cate1 NOT IN ('98','99')".	//	add ookawara 2013/08/23
			" ORDER BY i.cate1, i.cate2, i.cate3 DESC;";

	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			unset($CHECK_SELECT);
			unset($check_cate_name);
			$g_num = $list['g_num'];
			$att_check = 0;	//	add ookawara 2015/01/15

			if (!$CHECK_ITEM[$g_num]) {
				//	アイテム
				$set_csv = "";
				//$set_csv = rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST);	//	add $UP_PIC_LIST ookawara 2014/02/06	//	del ookawara 2015/01/15
				//$set_csv = rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST, $att_check);	//	add ookawara 2015/01/15		//	del ohkawara 2017/02/21
				$set_csv = rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST, $att_check, $TAG_BRAND_LIST, $TAG_SIZE_LIST, $RKT_DIR_LIST);	//	add ohkawara 2017/02/21-22

				if ($set_csv == "") { continue; }
				$rakuten_item_csv .= $set_csv;

				//	セレクト
				//list($select_csv,$CHECK_SELECT) = rakuten_select_csv($list,$RAKU_S_LIST);				//	del ookawara 2015/01/15
				list($select_csv,$CHECK_SELECT) = rakuten_select_csv($list,$RAKU_S_LIST, $att_check);	//	add ookawara 2015/01/15
				if ($select_csv) { $rakuten_select_csv .= $select_csv; }
			}

			//	カテゴリー
			list($cat_csv,$check_cate_name) = rakuten_cat_csv($list,$L_CATE,$RAKU_C_LIST);
			if ($cat_csv) { $rakuten_cat_csv .= $cat_csv; }


			unset($RAKU_LIST[$g_num]);
			if ($CHECK_SELECT) {
				foreach ($CHECK_SELECT AS $val) {
					$val = trim($val);
					if (!$val) { continue; }
					unset($RAKU_S_LIST[$g_num][$val]);
				}
			}
			if ($check_cate_name) { unset($RAKU_C_LIST[$g_num][$check_cate_name]); }

			$CHECK_ITEM[$g_num] = $g_num;
		}
	}


	//	楽天既存アイテムデーター残り削除
	if ($RAKU_LIST) {
		foreach ($RAKU_LIST AS $key => $val) {
			$g_num = trim($key);
			$val = trim($val);
			if ($val) {
				$CALM = array();
				$CALM = explode(",",$val);
				$keys_max = count($CALM)-1;	//	add ohkawara 2020/02/11
				$val = "";
				foreach ($CALM AS $keys => $vals) {
					//if ($keys >= 52) { break; }	//	del ookawara 2015/07/06
					//if ($keys >= 53) { break; }	//	add ookawara 2015/07/06	//	del ohkawara 2017/02/21
					//if ($keys >= 60) { break; }		//	add ohkawara 2017/02/21	//	ohkawara 2018/10/22
					//if ($keys >= 61) { break; }		//	add ohkawara 2018/10/22	//	del  ohkawara 2020/02/11
					if ($keys == 0) { $vals = "d"; }
					$val .= $vals;
					if ($keys_max > $keys) { $val .= ","; }
				}

				$rakuten_item_csv .= $val."\n";
			}
		}
	}

	//	楽天既存セレクトデーター残り削除
	if ($RAKU_S_LIST) {
		foreach ($RAKU_S_LIST AS $KEY => $VAL) {
			if ($RAKU_S_LIST[$KEY]) {
				foreach ($RAKU_S_LIST[$KEY] AS $key => $val) {
					$val = trim($val);
					if ($val) {
##						//$val = ereg_replace("^\"\"","\"d\"",$val);	//	del ohkawara 2020/02/11
						//$val = preg_replace("/^\"\"","\"d\"/",$val);	//	del ohkawara 2020/02/11
##						//$val = ereg_replace("\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"",",,,,,,,",$val);		//	del ohkawara 2020/02/07
						//$val = preg_replace("/\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"/",",,,,,,,",$val);		//	del ohkawara 2020/02/07
##						$val = ereg_replace("\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"",",,,,,,,,,,,,,",$val);	//	add ohkawara 2020/02/07
						$val = preg_replace("/\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"\\,\"\"/",",,,,,,,,,,,,,",$val);	//	add ohkawara 2020/02/07
						//	いずれは分解して作った方が手間が少なくて済むかも？

##						//$val = ereg_replace("\,\,\,\,\,\,\,", ",", $val);	//	add ookawara 2012/05/11	//	del ookawara 2015/01/19
						//$val = preg_replace("/\,\,\,\,\,\,\,/", ",", $val);	//	add ookawara 2012/05/11	//	del ookawara 2015/01/19
						//$val = preg_replace("/,\"\",\"\"$/", "", $val);		//	add ookawara 2015/07/07	//	del ohkawara 2020/02/11

						//	add ohkawara 2020/02/11
						$CALM = array();
						$CALM = explode(",",$val);
						$keys_max = count($CALM)-1;
						$val = "";
						foreach ($CALM AS $keys => $vals) {
							if ($keys == 0) { $vals = "d"; }
							$val .= $vals;
							if ($keys_max > $keys) { $val .= ","; }
						}


						$rakuten_select_csv .= $val."\n";
					}
				}
			}
		}
	}

	//	楽天既存カテゴリーデーター残り削除
	if ($RAKU_C_LIST) {
		foreach ($RAKU_C_LIST AS $KEY => $VAL) {
			if ($VAL) {
				foreach ($RAKU_C_LIST[$KEY] AS $key => $val) {
					if ($val) {
						$CHECK_CATEGORYS = explode(",",$val);
						if ($CHECK_CATEGORYS['3'] == "\"その他\"") { continue; }
						if ($CHECK_CATEGORYS['3'] == "\"オークション\"") { continue; }
##						$val = ereg_replace("^\"\"","\"d\"",$val);
						$val = preg_replace("/^\"\"/","\"d\"",$val);
##						$val = ereg_replace("999999999","",$val);
						$val = preg_replace("/999999999/","",$val);
##						$val = ereg_replace(",\"\"$","",$val);
						$val = preg_replace("/,\"\"$/","",$val);
						$rakuten_cat_csv .= $val.",\"\"\n";
					}
				}
			}
		}
	}

	if ($rakuten_item_csv) {
		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,モバイル表示,のし対応,PC用商品説明文,モバイル用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示\n$rakuten_item_csv";		//	del ookawara 2015/07/06
		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,モバイル表示,のし対応,PC用商品説明文,モバイル用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示\n$rakuten_item_csv";		//	add ookawara 2015/07/06		//	del ohkawara 2017/02/21

		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,モバイル用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号\n$rakuten_item_csv";		//	add ohkawara 2017/02/21	//	del ohkawara 2018/10/22

		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号,白背景画像URL,メーカー提供情報表示\n$rakuten_item_csv";		//	add ohkawara 2018/10/22	del ohkawara 2020/02/07

		//$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号,白背景画像URL,メーカー提供情報表示,地域別個別送料管理番号,消費税率\n$rakuten_item_csv";		//	add ohkawara 2020/02/07	//	del ohkawara 2020/02/18

		$rakuten_item_csv = "コントロールカラム,商品管理番号（商品URL）,商品番号,全商品ディレクトリID,タグID,PC用キャッチコピー,モバイル用キャッチコピー,商品名,販売価格,表示価格,消費税,送料,個別送料,送料区分1,送料区分2,代引料,倉庫指定,商品情報レイアウト,注文ボタン,資料請求ボタン,商品問い合わせボタン,再入荷お知らせボタン,のし対応,PC用商品説明文,スマートフォン用商品説明文,PC用販売説明文,商品画像URL,商品画像名（ALT）,動画,販売期間指定,注文受付数,在庫タイプ,在庫数,在庫数表示,項目選択肢別在庫用横軸項目名,項目選択肢別在庫用縦軸項目名,項目選択肢別在庫用残り表示閾値,RAC番号,サーチ非表示,闇市パスワード,カタログID,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,予約商品発売日,ポイント変倍率,ポイント変倍率適用期間,ヘッダー・フッター・レフトナビ,表示項目の並び順,共通説明文（小）,目玉商品,共通説明文（大）,レビュー本文表示,海外配送管理番号,サイズ表リンク,二重価格文言管理番号,カタログIDなしの理由,配送方法セット管理番号,白背景画像URL,メーカー提供情報表示,地域別個別送料管理番号,消費税率,単品配送設定,単品配送設定使用理由\n$rakuten_item_csv";		//	add ohkawara 2020/02/18

#		$rakuten_item_csv = mb_convert_encoding($rakuten_item_csv,"sjis-win","eucjp-win");
		$rakuten_item_csv = mb_convert_encoding($rakuten_item_csv,"sjis-win","UTF-8");
		$OUT = fopen(RAKUTEN_ITEM_FILE,"w");
		fwrite($OUT,$rakuten_item_csv);
		fclose($OUT);
		@chmod(RAKUTEN_ITEM_FILE,0666);
		$msg .= "楽天アイテムcsvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(RAKUTEN_ITEM_FILE)) { unlink(RAKUTEN_ITEM_FILE); }
		$msg .= "最新の情報がないので楽天アイテムcsvファイル作成しませんでした。<br>\n<br>\n";
	}

	if ($rakuten_select_csv) {
		//$rakuten_select_csv = "項目選択肢用コントロールカラム,商品管理番号（商品URL）,選択肢タイプ,Select/Checkbox用項目名,Select/Checkbox用選択肢,項目選択肢別在庫用横軸選択肢,項目選択肢別在庫用横軸選択肢子番号,項目選択肢別在庫用縦軸選択肢,項目選択肢別在庫用縦軸選択肢子番号,項目選択肢別在庫用取り寄せ可能表示,項目選択肢別在庫用在庫数\n$rakuten_select_csv";	//	del ookawara 2015/01/15
		//$rakuten_select_csv = "項目選択肢用コントロールカラム,商品管理番号（商品URL）,選択肢タイプ,Select/Checkbox用項目名,Select/Checkbox用選択肢,項目選択肢別在庫用横軸選択肢,項目選択肢別在庫用横軸選択肢子番号,項目選択肢別在庫用縦軸選択肢,項目選択肢別在庫用縦軸選択肢子番号,項目選択肢別在庫用取り寄せ可能表示,項目選択肢別在庫用在庫数,在庫あり時納期管理番号\n$rakuten_select_csv";	//	add ookawara 2015/01/15	//	del ohkawara  2020/02/07
		$rakuten_select_csv = "項目選択肢用コントロールカラム,商品管理番号（商品URL）,選択肢タイプ,項目選択肢項目名,項目選択肢,項目選択肢別在庫用横軸選択肢,項目選択肢別在庫用横軸選択肢子番号,項目選択肢別在庫用縦軸選択肢,項目選択肢別在庫用縦軸選択肢子番号,項目選択肢別在庫用取り寄せ可能表示,項目選択肢別在庫用在庫数,在庫戻しフラグ,在庫切れ時の注文受付,在庫あり時納期管理番号,在庫切れ時納期管理番号,タグID,画像URL,項目選択肢選択必須\n$rakuten_select_csv";	//	add ohkawara 2020/02/07

#		$rakuten_select_csv = mb_convert_encoding($rakuten_select_csv,"sjis-win","eucjp-win");
		$rakuten_select_csv = mb_convert_encoding($rakuten_select_csv,"sjis-win","UTF-8");
		$OUT = fopen(RAKUTEN_SELECT_FILE,"w");
		fwrite($OUT,$rakuten_select_csv);
		fclose($OUT);
		@chmod(RAKUTEN_SELECT_FILE,0666);
		$msg .= "楽天セレクトcsvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(RAKUTEN_SELECT_FILE)) { unlink(RAKUTEN_SELECT_FILE); }
		$msg .= "最新の情報がないので楽天セレクトcsvファイル作成しませんでした。<br>\n<br>\n";
	}

	if ($rakuten_cat_csv) {
		$rakuten_cat_csv = "コントロールカラム,商品管理番号（商品URL）,商品名,表示先カテゴリ,優先度,URL,1ページ複数形式\n$rakuten_cat_csv";

#		$rakuten_cat_csv = mb_convert_encoding($rakuten_cat_csv,"sjis-win","eucjp-win");
		$rakuten_cat_csv = mb_convert_encoding($rakuten_cat_csv,"sjis-win","UTF-8");
		$OUT = fopen(RAKUTEN_CAT_FILE,"w");
		fwrite($OUT,$rakuten_cat_csv);
		fclose($OUT);
		@chmod(RAKUTEN_CAT_FILE,0666);
		$msg .= "楽天カテゴリーcsvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(RAKUTEN_CAT_FILE)) { unlink(RAKUTEN_CAT_FILE); }
		$msg .= "最新の情報がないので楽天カテゴリーcsvファイル作成しませんでした。<br>\n<br>\n";
	}

	//	配列削除
	//	add ohkawara 2017/02/22
	unset($TAG_SIZE_LIST);
	unset($RKT_DIR_LIST);


	//	配列・csvデータ削除	add ohkawara 2018/08/31
	unset($RAKU_C_LIST);
	unset($RAKU_S_LIST);
	unset($RAKU_LIST);
	unset($CHECK_ITEM);
	unset($UP_PIC_LIST);
	unset($TAG_BRAND_LIST);
	unset($L_CATE);
	unset($UP_PIC_LIST);

	unset($rakuten_item_csv);
	unset($rakuten_select_csv);
	unset($rakuten_cat_csv);


	//	空画像フォルダー削除
	del_dir();

/*-----------------------------------------------------------------------------*/

	//	Amazon処理-----------------------------------------------------------------------------------------
	//	add ookawara 2015/07/27
	//	アップ済み出品レポートファイル取得
	$AMZ_UP_LIST = array();
	if (file_exists(AMAZON_B_ITEM_FILE)) {
		$LIST = file(AMAZON_B_ITEM_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		foreach ($LIST AS $key => $val) {
			if ($key < 1) { continue; }
			list($sku, $asin, $price, $cnt) = explode("\t", $val);
			list($g_num, $size) = explode("-", $sku);
			if ($size == "") { $size = "none"; }
			$AMZ_UP_LIST[$sku]['asin'] = $asin;
			$AMZ_UP_LIST[$sku]['price'] = $price;
			$AMZ_UP_LIST[$sku]['cnt'] = $cnt;		//	add ohkawara 2019/11/15
		}
	}

	//	add ohkawara 2019/11/20
	//	反映されていないASINコードを登録する。
	regist_asin($AMZ_UP_LIST);


	//	出品商品取得
	$amz0_tsv = "";
	$amz1_tsv = "";
	$amz2_tsv = "";
	$sql  = "CREATE TEMPORARY TABLE tmp_category AS".
			" SELECT g_num,cate1 FROM category".
			" WHERE display='1'".
			" AND state<'3'".
			" GROUP BY g_num,cate1;";
	mysqli_query($conn_id,$sql);

	//	ASINが有る商品は、出品ファイルでアップする。
	$AMZ_SYU_LIST = array();

	//	ファッション＆シューズ
	$amz_tsv1 = "";
	$sql  = "SELECT goods.*,category.cate1".
			" FROM goods goods".
			" INNER JOIN tmp_category category ON category.g_num=goods.g_num".
			" WHERE goods.soldout!='1'".
			" AND goods.amazonshop='1'".
			" AND (goods.ad_id like '1_%' OR goods.ad_id like '3_%')".
			";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$set_tsv = "";
			$set_tsv = amazon_item_tsv($list, $L_BRAND, $AMZ_UP_LIST, $AMZ_SYU_LIST);
			if ($set_tsv == "") { continue; }
			$amz_tsv1 .= $set_tsv;
		}
	}

	//	スポーツ
	$amz_tsv2 = "";
	$sql  = "SELECT goods.*".
			" FROM goods goods".
			" INNER JOIN tmp_category category ON category.g_num=goods.g_num".
			" WHERE goods.soldout!='1'".
			" AND goods.amazonshop='1'".
			" AND goods.ad_id like '2_%'".
			";";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$set_tsv = "";
			$set_tsv = amazon_item_tsv($list, $L_BRAND, $AMZ_UP_LIST, $AMZ_SYU_LIST);
			if ($set_tsv == "") { continue; }
			$amz_tsv2 .= $set_tsv;
		}
	}


	//	出品ファイルL作成
	$amz_tsv0 = "";
	if (count($AMZ_UP_LIST) > 0 || $AMZ_SYU_LIST) {
		$amz_tsv0 = amazon_zaiko_item_tsv($AMZ_UP_LIST, $AMZ_SYU_LIST);
	}

	if ($amz_tsv1) {
		$amazon_tsv_head = amazon_tsv_head1();
		$amz_tsv1 = $amazon_tsv_head.$amz_tsv1;
#		$amz_tsv1 = mb_convert_encoding($amz_tsv1,"sjis-win","eucjp-win");
		$amz_tsv1 = mb_convert_encoding($amz_tsv1,"sjis-win","UTF-8");
		$OUT = fopen(AMAZON_AP_ITEM_FILE,"w");
		fwrite($OUT,$amz_tsv1);
		fclose($OUT);
		@chmod(AMAZON_AP_ITEM_FILE,0666);
		$msg .= "Amazonファッション＆シューズ在庫tsvファイル作成完了いたしました。<br>\n<br>\n";
	}

	//	JANで登録されるので基本利用されることはない
	if ($amz_tsv2) {
		$amazon_tsv_head = amazon_tsv_head2();
		$amz_tsv2 = $amazon_tsv_head.$amz_tsv2;
#		$amz_tsv2 = mb_convert_encoding($amz_tsv2,"sjis-win","eucjp-win");
		$amz_tsv2 = mb_convert_encoding($amz_tsv2,"sjis-win","UTF-8");
		$OUT = fopen(AMAZON_SP_ITEM_FILE,"w");
		fwrite($OUT,$amz_tsv2);
		fclose($OUT);
		@chmod(AMAZON_SP_ITEM_FILE,0666);
		$msg .= "Amazonスポーツ在庫tsvファイル作成完了いたしました。<br>\n<br>\n";
	}

	//	JANコードがある商品
	if ($amz_tsv0) {
		$amazon_tsv_head = amazon_tsv_head0();
		$amz_tsv0 = $amazon_tsv_head.$amz_tsv0;
#		$amz_tsv0 = mb_convert_encoding($amz_tsv0,"sjis-win","eucjp-win");
		$amz_tsv0 = mb_convert_encoding($amz_tsv0,"sjis-win","UTF-8");
		$OUT = fopen(AMAZON_JAN_ITEM_FILE,"w");
		fwrite($OUT,$amz_tsv0);
		fclose($OUT);
		@chmod(AMAZON_JAN_ITEM_FILE,0666);
		$msg .= "Amazon出品tsvファイル作成完了いたしました。<br>\n<br>\n";
	}

	//	配列・csvデータ削除	add ohkawara 2018/08/31
	unset($L_BRAND);
	unset($AMZ_UP_LIST);
	unset($AMZ_SYU_LIST);
	unset($amz_tsv1);
	unset($amz_tsv2);
	unset($amz_tsv0);

/*-----------------------------------------------------------------------------*/

	//	Wowma処理-----------------------------------------------------------------------------------------
	//	add ohkawara 2017/02/09

	//	アップ済み出品レポートファイル取得
	setlocale( LC_ALL, 'ja_JP' );
	//	登録済み在庫情報
	$WOWMA_UP_LIST = array();
	$WOWMA_UP_NAME_LIST = array();
	//if (file_exists(WOWMA_B_ITEM_FILE)) {		//	del ohkawara 2018/04/03
	//	$LIST = file(WOWMA_B_ITEM_FILE);		//	del ohkawara 2018/04/03
	if (file_exists(WOWMA_B_STOCK_FILE)) {		//	add ohkawara 2018/04/03
		$LIST = file(WOWMA_B_STOCK_FILE);		//	add ohkawara 2018/04/03
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		foreach ($LIST AS $key => $VAL) {
			if ($key < 1) {
				$WOWMA_UP_NAME_LIST = str_getcsv($VAL,',','"');
			} else {
				$SET_LIST = array();
				$list = str_getcsv($VAL,',','"');
				foreach ($WOWMA_UP_NAME_LIST AS $keys => $keysname) {
					$SET_LIST[$keysname] = $list[$keys];
				}

				// add ohkawara 2018/04/03
				//	サイズ設定が有る場合上書きされてしまう為、サイズの入れ替え作業を行う
				if ($SET_LIST['choicesStockHorizontalName']) {
					$size = $SET_LIST['choicesStockHorizontalName'];
					$SET_LIST['size'][$size] = $SET_LIST['choicesStockCount'];
				}

				//	資料
				//	$SET_LIST['itemCode']	g_num
				//	$SET_LIST['stockSegment']	1 or 2
				//	$SET_LIST['stockCount']	サイズ無　数
				//	$SET_LIST['choicesStockHorizontalName']	サイズ名
				//	$SET_LIST['choicesStockCount']	サイズ有　数

				//$Code = $SET_LIST['Code'];	//	del ohkawara 2018/04/02
				$Code = $SET_LIST['itemCode'];	//	add ohkawara 2018/04/02
				$g_num = preg_replace("/[^0-9]/", "", $Code);

				$WOWMA_UP_LIST[$g_num] = $SET_LIST;
			}
/*
			//	カテゴリー変更時処理	普段はコメント	ohkawara 2017/04/14
			$BCategID = $SET_LIST[BCategID];
			if ($g_num && $BCategID) {
				$sql  = "UPDATE ".T_GOODS." SET wd_id='".$BCategID."'".
						" WHERE g_num='".$g_num."';";
				if (!mysqcli_query($conn_id,$sql)) { $ERROR[] = "Wowma!商品オプションデーターを更新できませんでした。$g_num"; }
			}
			$g_num = "";
			$BCategID = "";
*/

		}
	}

	//	登録済み商品情報	add ohkawara 2018/04/12
	$WOWMA_UP_ITEM_LIST = array();
	$WOWMA_UP_ITEM_NAME_LIST = array();
	if (file_exists(WOWMA_B_ITEM_FILE)) {
		$LIST = file(WOWMA_B_ITEM_FILE);
#		mb_convert_variables("eucjp-win", "sjis-win", $LIST);
		mb_convert_variables("UTF-8", "sjis-win", $LIST);
		foreach ($LIST AS $key => $VAL) {
			if ($key < 1) {
				$WOWMA_UP_ITEM_NAME_LIST = str_getcsv($VAL,',','"');
			} else {
				$SET_LIST = array();
				$list = str_getcsv($VAL,',','"');
				foreach ($WOWMA_UP_ITEM_NAME_LIST AS $keys => $keysname) {
					$SET_LIST[$keysname] = $list[$keys];
				}
			}

			$check_g_num = $SET_LIST['itemCode'];
			$check_g_num = preg_replace("/[^0-9]/", "", $check_g_num);
			//$check_state = $SET_LIST['saleStatus'];
			if ($WOWMA_UP_LIST[$check_g_num]) {
			//	if ($check_state == 2) {
			//		unset($WOWMA_UP_LIST[$check_g_num]);
			//	} else {
					foreach ($SET_LIST AS $key_name => $VALUES) {
						$WOWMA_UP_LIST[$check_g_num][$key_name] = $VALUES;
					}
			//	}
			}
		}
	}


	$list_item_csv = "";
	$list_zaiko_csv = "";
	$sql  = "SELECT goods.* FROM ".T_GOODS." goods".
			" INNER JOIN (".
			"	select distinct g_num from ".T_CATEGORY.
			"	 where category.display='1' AND cate1>='20' AND state!='3' AND cate1 NOT IN ('98','99')".
			"	) category_s ON goods.g_num=category_s.g_num".
			//" INNER JOIN mool_wd mool_wd ON goods.wd_id=mool_wd.dir_id".
			" INNER JOIN mool_wd mool_wd ON cast(goods.wd_id AS SIGNED)=mool_wd.dir_id".
			" WHERE goods.wowmashop='1'".
			" AND goods.soldout!='1'".
			" ORDER BY goods.g_num;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			$g_num = $list['g_num'];
			$del_flg = 0;
			make_wowma_csv($list, $WOWMA_UP_LIST, $list_item_csv, $list_zaiko_csv, $del_flg);
		}
	}

	//	add ohkawara 2018/04/03
	//	$WOWMA_UP_LIST整理	販売終了商品データー作成
	if (count($WOWMA_UP_LIST)) {
		del_wowma_data($WOWMA_UP_LIST, $list_item_csv, $list_zaiko_csv);
	}


	//	削除した商品の未処理の時の処理
	//	del ohkawara 2018/04/09
	//if ($WOWMA_UP_LIST) {
	//	foreach ($WOWMA_UP_LIST AS $g_num => $list) {
	//		$del_flg = 1;
	//		make_wowma_csv($list, $WOWMA_UP_LIST, $list_item_csv, $list_zaiko_csv, $del_flg);
	//	}
	//	unset($WOWMA_UP_LIST);
	//}

	$wowma_item_csv = "";
	if ($list_item_csv) {
		//$wowma_item_csv  = "Code,BCategID,KtaiTitle,KtaiPrice,KtaiCaption,KtaiDealType,KtaiChargeType,AdmID,StockNum,Keyword,KtaiUserRate,KtaiImageLinkName1,KtaiImageType1,KtaiImageURL1,KtaiImageLinkName2,KtaiImageType2,KtaiImageURL2,KtaiImageLinkName3,KtaiImageType3,KtaiImageURL3,KtaiImageLinkName4,KtaiImageType4,KtaiImageURL4,KtaiImageLinkName5,KtaiImageType5,KtaiImageURL5,KtaiImageLinkName6,KtaiImageType6,KtaiImageURL6,KtaiImageLinkName7,KtaiImageType7,KtaiImageURL7,KtaiImageLinkName8,KtaiImageType8,KtaiImageURL8,KtaiImageLinkName9,KtaiImageType9,KtaiImageURL9,KtaiImageLinkName10,KtaiImageType10,KtaiImageURL10,ShipFeeType,ShipFee,DeliveryType,KtaiShipComment,KtaiDealComment,KtaiPriceLabel1,KtaiPriceLabel2,KtaiCaptionPC,KtaiCaptionSP,KtaiDetailLinkName,KtaiDetailComment,Option1,Option2,Option3,Option4,Option5,SalesStartDate,StartDate,ExhibitStatus,AdmTitle,JanCode,IsbnCode,NameModel,SecretPagePassword,SecretPageTitle,SecretPageMessage,StockShow,KtaiNoStockMessage,KtaiSpecLinkName,KtaiSpec1,KtaiSpec2,KtaiSpec3,KtaiSpec4,KtaiSpec5,MaxBuyNum,KtaiStockShowStartNum,KtaiStockMessage1,KtaiStockMessage2,Tax,StockType,SkuOptionNameCol,SkuOptionValueCol,SkuOptionTypeCol,SkuOptionNameRow,SkuOptionValueRow,SkuOptionTypeRow,SkuHeader,SkuFooter,SkuStockShowType,SkuLimitNum,exhibittype\r\n";		//	del ohkawara 2018/04/02

		$wowma_item_csv  = "ctrlCol,lotNumber,itemName,itemManagementId,itemManagementName,itemCode,itemPrice,sellMethodSegment,releaseDate,reserveRegstDate,taxSegment,postageSegment,postage,deliveryId,deliveryMethodId1,deliveryMethodId2,deliveryMethodId3,deliveryMethodId4,deliveryMethodId5,deliveryMethodName1,deliveryMethodName2,deliveryMethodName3,deliveryMethodName4,deliveryMethodName5,publicStartDate,limitedOrderSegment,limitedOrderCount,description,descriptionForSP,descriptionForPC,detailTitle,detailDescription,specTitle,spec1,spec2,spec3,spec4,spec5,searchKeyword1,searchKeyword2,searchKeyword3,imageName1,imageName2,imageName3,imageName4,imageName5,imageName6,imageName7,imageName8,imageName9,imageName10,imageUrl1,imageUrl2,imageUrl3,imageUrl4,imageUrl5,imageUrl6,imageUrl7,imageUrl8,imageUrl9,imageUrl10,categoryId,tagId,jan,isbn,itemModel,limitedPasswd,limitedPasswdPageTitle,limitedPasswdPageMessage,saleStatus,itemOption1,itemOption2,itemOption3,itemOption4,itemOption5,itemOption6,itemOption7,itemOption8,itemOption9,itemOption10,itemOption11,itemOption12,itemOption13,itemOption14,itemOption15,itemOption16,itemOption17,itemOption18,itemOption19,itemOption20,pointRate,stockSegment,stockShippingDayId,stockShippingDayDispTxt,displayBackorderMessage,stockCount,displayStockSegment,displayStockThreshold,choicesStockHorizontalItemName,choicesStockVerticalItemName,choicesStockUpperDescription,choicesStockLowerDescription,displayChoicesStockSegment,displayChoicesStockThreshold\r\n";	//	add ohkawara 2018/04/02

		$wowma_item_csv .= $list_item_csv;

#		$wowma_item_csv = mb_convert_encoding($wowma_item_csv,"sjis-win","eucjp-win");
		$wowma_item_csv = mb_convert_encoding($wowma_item_csv,"sjis-win","UTF-8");

		$OUT = fopen(WOWMA_ITEM_FILE,"w");
		fwrite($OUT,$wowma_item_csv);
		fclose($OUT);
		@chmod(WOWMA_ITEM_FILE,0666);
		$msg .= "Wowma商品csvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(WOWMA_ITEM_FILE)) { unlink(WOWMA_ITEM_FILE); }
		$msg .= "最新の情報がないのでWowma商品csvファイル作成しませんでした。<br>\n<br>\n";
	}

	$wowma_zaiko_csv = "";
	if ($list_zaiko_csv) {
		//$wowma_zaiko_csv  = "Code,SkuOptionTypeCol,SkuOptionTypeRow,SkuItemNum\r\n";		//	del ohkawara 2018/04/02
		//$wowma_zaiko_csv  = "ctrlCol,lotNumber,itemCode,stockSegment,stockCount,choicesStockHorizontalName,choicesStockHorizontalCode,choicesStockHorizontalSeq,choicesStockVerticalName,choicesStockVerticalCode,choicesStockVerticalSeq,choicesStockCount,choicesStockShippingDayId,choicesStockShippingDayDispTxt\r\n";		//	add ohkawara 2018/04/02	//	del ohkawara 2018/12/17
		$wowma_zaiko_csv  = "ctrlCol,lotNumber,itemCode,stockSegment,stockCount,choicesStockHorizontalName,choicesStockHorizontalCode,choicesStockHorizontalSeq,choicesStockVerticalName,choicesStockVerticalCode,choicesStockVerticalSeq,choicesStockCount,choicesStockShippingDayId,choicesStockShippingDayDispTxt,choicesStockImageUrl,choicesStockColorSegment\r\n";		//	add ohkawara 2018/12/17
		$wowma_zaiko_csv .= $list_zaiko_csv;

#		$wowma_zaiko_csv = mb_convert_encoding($wowma_zaiko_csv,"sjis-win","eucjp-win");
		$wowma_zaiko_csv = mb_convert_encoding($wowma_zaiko_csv,"sjis-win","UTF-8");

		$OUT = fopen(WOWMA_ZAIKO_FILE,"w");
		fwrite($OUT,$wowma_zaiko_csv);
		fclose($OUT);
		@chmod(WOWMA_ZAIKO_FILE,0666);
		$msg .= "Wowma在庫csvファイル作成完了いたしました。<br>\n<br>\n";
	} else {
		if (file_exists(WOWMA_ZAIKO_FILE)) { unlink(WOWMA_ZAIKO_FILE); }
		$msg .= "最新の情報がないのでWowma在庫csvファイル作成しませんでした。<br>\n<br>\n";
	}

	//	add ohkawara 2018/04/03
	//	メモリークリアー
	unset($list_item_csv);
	unset($list_zaiko_csv);
	unset($WOWMA_UP_LIST);

/*-----------------------------------------------------------------------------*/


	// mysqli_close($conn_id);

	return array($msg,$ERROR);

}



//-------------------------------------------------------------------------------------
//
//	Yahoo
//
//-------------------------------------------------------------------------------------

//	新yahoo用CSV
function new_yahoo_csv($list,$L_CATE,$L_BRAND,$L_YPATH,$L_Y_BEAND,$category_list,$SPEC_ID_LIST, $UP_PIC_LIST) {	//	add $UP_PIC_LIST ookawara 2014/02/06
	global $attention,$NEWCATE,$NEW_YAHOO_GOODS_DISCOUNT_CATE;
	global $MOVIE_LIST;	//	add ookawara 2014/10/10
	global $nouki_days_msg;		//	add ookawara 2015/01/30

	$num = $list['num'];
	$cate1 = $list['cate1'];
	$cate2 = $list['cate2'];
	$cate3 = $list['cate3'];
	$g_num = $list['g_num'];
	$ypath = $list['ypath'];
	$display = $list['display'];
	$copy_num = $list['copy_num'];
	$state = $list['state'];
	$g_name = $list['g_name'];
	$code = $list['code'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$headline = $list['headline'];
	$yd_id = $list['yd_id'];
	$zaiko = $list['zaiko'];			//	add ookawara 2015/09/17
	$size_list = $list['size_list'];	//	add ookawara 2015/09/17
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24

	//	price,sale_price
	//if (TAXABLE == 1) {	//	del ookawara 2014/03/10
		//	割引設定項目
		$GOODS_DISCOUNT_C = NEW_YAHOO_GOODS_DISCOUNT_C;
		$GOODS_DISCOUNT_CATE = $NEW_YAHOO_GOODS_DISCOUNT_CATE;
		$DISCOUNT_PAR = NEW_YAHOO_DISCOUNT_PAR;
		$DISCOUNT_PAR2 = NEW_YAHOO_DISCOUNT_PAR2;

		if ($base_price > 0 && $base_sale_price > 0 && $base_price < $base_sale_price) {
			$sale_price = "";
			$price = $base_price;
		} elseif ($base_price > 0 && $base_sale_price > 0 && $base_price == $base_sale_price) {
			if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
				$flag = 0;
				if ($GOODS_DISCOUNT_CATE) {
					foreach ($GOODS_DISCOUNT_CATE AS $val) {
						$val = trim($val);
						if ($cate1 == $val) { $flag = 1; }
					}
				}
				if ($flag != 1) {
					$sale_price = floor($base_price * ((100 - $DISCOUNT_PAR2) / 100));
				} else {
					$sale_price = "";
				}
			} else {
				$sale_price = "";
			}
			$price = $base_price;
		} elseif ($GOODS_DISCOUNT_C == 1) {
			$flag = 0;
			if ($GOODS_DISCOUNT_CATE) {
				foreach ($GOODS_DISCOUNT_CATE AS $val) {
					$val = trim($val);
					if ($cate1 == $val) { $flag = 1; }
				}
			}
			if ($flag != 1) {
				$sale_price_ = floor($base_price * ((100 - $DISCOUNT_PAR) / 100));
				if ($base_sale_price < 1) {
					$price = $base_price;
					$sale_price = $sale_price_;
				} elseif ($base_sale_price > $sale_price_) {
					$price = $base_sale_price;
					$sale_price = $sale_price_;
				} else {
					$price = $base_price;
					$sale_price = $base_sale_price;
				}
			} else {
				if ($base_sale_price) {
					$price = $base_sale_price;
					$sale_price = "";
				} else {
					$price = $base_price;
					$sale_price = "";
				}
			}
		}

		if (TAXABLE == 1) {	//	add ookawara 2014/03/10
			$price = floor($price * ((TAX / 100) + 1) + 0.5);
			$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
		}	//	add ookawara 2014/03/10
		if ($sale_price < 1) { unset($sale_price); }
/*
	//	del ookawara 2014/03/10
	} else {
		if ($base_price > 0 && $base_sale_price > 0 && $base_price == $base_sale_price) {
			$sale_price = "";
		}
		$price = floor($base_price);
		$sale_price = floor($base_sale_price);
	}
*/

	//	options
	//	add ookawara 2015/09/17	start
	$L_SIZE = array();
	if ($size_list) {
		$op_size = unserialize($size_list);
	}
	$jan_code = "";		//	add ookawara 2016/04/19
	if ($op_size) {
		foreach ($op_size AS $key => $VAL) {

			//	jan_code設定
			//	add ookawara 2016/04/19
			if ($VAL['jan'] != "none" || $VAL['jan'] > 0) {
				if (!preg_match("/[^0-9]/", $VAL['jan'])) {
					if (strlen($VAL['jan']) == 13 && preg_match("/^4/", $VAL['jan'])) {
						$jan_code = $VAL['jan'];
					}
				}
			}

			if ($VAL['type'] == 3) {
				continue;
			}
			$L_SIZE[] = $VAL['size'];
		}
	} else {
	//	add ookawara 2015/09/17	end
		$options = trim($options);
		$L_SIZE = explode("\n",$options);
	}	//	add ookawara 2015/09/17

	$flag = 0;
	$size_check = 0;
	$spec_id = "";
	$options = "";
	$new_pic_check = 0;
	if ($L_SIZE) {
		foreach ($L_SIZE AS $val) {
			$val = trim($val);
			if ($val == "") { continue; }
##			if (ereg("^\*",$val)) {
			if (preg_match("/^\*/",$val)) {
				$size_check = 1;
				continue;
			}
##			if (ereg("^\/",$val)) { $new_pic_check = 1; }
			if (preg_match("/^\//",$val)) { $new_pic_check = 1; }
##			$val = eregi_replace("^\/i","",$val);
			$val = preg_replace("/^\//","",$val);
##			$val = eregi_replace("\[.*\]","",$val);
			$val = preg_replace("/\[.*\]/i","",$val);
			if ($val != "F" || 
				$val != "フリー"  || 
				$val != "フリーサイズ" || 
				$val != "--" || 
				$val != "フットサルサイズ" || 
				$val != "設定なし"
			) {
				$val = trim($val);
				$id = $yd_id;
				$spec_value_id = "";
				if ($SPEC_ID_LIST[$id]) {
					foreach ($SPEC_ID_LIST[$id] AS $key1 => $val1) {
						if ($SPEC_ID_LIST[$id][$key1]) {
							foreach ($SPEC_ID_LIST[$id][$key1] AS $key2 => $val2) {
								$check_value = moji_seiton($val);

								//	add ookawara 2012/03/07
								if (!$check_value) {
									echo $g_num."<br>\n";
									continue;
								}

##								if (ereg($check_value,$val2)) {
								if (preg_match($check_value,$val2)) {
										$spec_value_id = $key2;
										$spec_id = $key1;
										break;
								}
							}
							if ($spec_value_id) { break; }
						}
					}

				}
				$options .= "{$val}|{$spec_value_id}| ";
				$flag = 1;
			}
		}
		$options = trim($options);
##		$options = ereg_replace(":","：",$options);
		$options = preg_replace("/:/","：",$options);
	}

	//	add ookawara 2010/05/25
	if ($size_check == 1 && $options == "") {
		return ;
	}

	if ($flag != 1) {
		unset($options);
	} else {
##		if (ereg("^UBS8260",$code)) {
		if (preg_match("/^UBS8260/",$code)) {
			$options = "カラー|{$spec_id}| ".$options;
		} else {
			$options = "サイズ|{$spec_id}| ".$options;
		}
	}

	//	カテゴリー
	$cate1_name = trim($L_CATE[$cate1][0][0]);
	$cate2_name = trim($L_CATE[$cate1][$cate2][0]);
	$cate3_name = trim($L_CATE[$cate1][$cate2][$cate3]);
	$cate0_name = trim($NEWCATE[$cate1]['name']);

	$meta_key = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$meta_key .= "|".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$meta_key .= "|".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$meta_key .= "|".$cate3_name;
	}
	$meta_key .= "|サッカー";

	//	画像チェック
	$imagef_file = IMG_F_DIR."/$code" . ".jpg";
	$c_imagef_file = UP_IMG_DIR."/$code" . ".jpg";
	//if (file_exists($imagef_file) && !file_exists($c_imagef_file) && (CHECK_TIME < filemtime($imagef_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($imagef_file) && !file_exists($c_imagef_file) && (CHECK_TIME < filemtime($imagef_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($imagef_file,$c_imagef_file);
	}

	$imageb_file = IMG_B_DIR."/$code" . ".jpg";
	$c_imageb_file = UP_IMG_DIR."/$code" . "_1.jpg";
	//if (file_exists($imageb_file) && !file_exists($c_imageb_file) && (CHECK_TIME < filemtime($imageb_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($imageb_file) && !file_exists($c_imageb_file) && (CHECK_TIME < filemtime($imageb_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($imageb_file,$c_imageb_file);
	}

	//	add ookawara 2013/07/03
	$image03_file = IMG_03_DIR."/$code" . ".jpg";
	$c_image03_file = UP_IMG_DIR."/$code" . "_2.jpg";
	//if (file_exists($image03_file) && !file_exists($c_image03_file) && (CHECK_TIME < filemtime($image03_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image03_file) && !file_exists($c_image03_file) && (CHECK_TIME < filemtime($image03_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($image03_file,$c_image03_file);
	}

	$image04_file = IMG_04_DIR."/$code" . ".jpg";
	$c_image04_file = UP_IMG_DIR."/$code" . "_3.jpg";
	//if (file_exists($image04_file) && !file_exists($c_image04_file) && (CHECK_TIME < filemtime($image04_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image04_file) && !file_exists($c_image04_file) && (CHECK_TIME < filemtime($image04_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($image04_file,$c_image04_file);
	}

	$image05_file = IMG_05_DIR."/$code" . ".jpg";
	$c_image05_file = UP_IMG_DIR."/$code" . "_4.jpg";
	//if (file_exists($image05_file) && !file_exists($c_image05_file) && (CHECK_TIME < filemtime($image05_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image05_file) && !file_exists($c_image05_file) && (CHECK_TIME < filemtime($image05_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {	//	add ookawara 2014/02/06
		copy($image05_file,$c_image05_file);
	}

	//	add ookawara 2015/09/27
	$image06_file = IMG_06_DIR."/$code" . ".jpg";
	$c_image06_file = UP_IMG_DIR."/$code" . "_5.jpg";
	//if (file_exists($image06_file) && !file_exists($c_image06_file) && (CHECK_TIME < filemtime($image06_file) || $new_pic_check == 1)) {	//	del ookawara 2014/02/06
	if (file_exists($image06_file) && !file_exists($c_image06_file) && (CHECK_TIME < filemtime($image06_file) || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "")) {
		copy($image06_file,$c_image06_file);
	}


	//	caption
	$att_flg = 0;	//	add ookawara 2015/01/30
	$check = 0;
##	$caption = eregi_replace("<br>","\n",$caption);
    $caption = preg_replace("/<br>/i","\n",$caption);
##	$caption = eregi_replace("<br />","\n",$caption);
	$caption = preg_replace("/<br \/>/i","\n",$caption);
##	$caption = ereg_replace("\r","",$caption);
	$caption = preg_replace("/\r/","",$caption);
##	$caption  = ereg_replace("&","＆",$caption);			//	add ookawara 2013/04/11
    $caption  = preg_replace("/&/","＆",$caption);			//	add ookawara 2013/04/11
##	$caption  = ereg_replace("<","&lt;",$caption);			//	add ookawara 2013/04/11
	$caption  = preg_replace("/</","&lt;",$caption);			//	add ookawara 2013/04/11
##	$caption  = ereg_replace(">","&gt;",$caption);			//	add ookawara 2013/04/11
    $caption  = preg_replace("/>/","&gt;",$caption);			//	add ookawara 2013/04/11


##	if (ereg("&lt;!-- Attention --&gt;",$caption)) {		//	add ookawara 2013/09/13
    if (preg_match("/&lt;!-- Attention --&gt;/",$caption)) {		//	add ookawara 2013/09/13
//	2011/07/12	コメント表示希望依頼が来た為表示するように変更（元に戻す）
##		$caption = ereg_replace("&lt;!-- Attention --&gt;", "", $caption);	//	add ookawara 2015/01/30
		$caption = preg_replace("/&lt;!-- Attention --&gt;/", "", $caption);	//	add ookawara 2015/01/30
##		//$caption = ereg_replace("&lt;!-- Attention --&gt;","$attention",$caption);	//	add ookawara 2013/09/13	//	del ookawara 2015/01/30
		//$caption = preg_replace("/&lt;!-- Attention --&gt;/","$attention",$caption);	//	add ookawara 2013/09/13	//	del ookawara 2015/01/30
##		//$caption = ereg_replace("<!-- Attention -->","$attention",$caption);		//	del ookawara 2013/09/13
		//$caption = preg_replace("/<!-- Attention -->/","$attention",$caption);		//	del ookawara 2013/09/13
##		//$caption = ereg_replace("<!-- Attention -->","",$caption);
		//$caption = preg_replace("/<!-- Attention -->/","",$caption);
		$caption = trim($caption);
		$check = 1;
		$att_flg = 1;	//	add ookawara 2015/01/30
	}
	$L_CAPTION = explode("\n",$caption);

	if ($L_CAPTION) {
		$flag = 0;
		foreach ($L_CAPTION AS $val) {
			$val = trim($val);
			if ($flag != 1 && !$val) {
				$flag = 1;
				continue;
			}
			if ($flag == 0) {
				$explanation .= $val." ";
			} else {
				$caption_msg .= $val."<br />";
			}
		}
	}

	$caption = <<<WAKABA
$attation
ブランド名：$L_BRAND[$brand]<br />
$caption_msg
WAKABA;
##	$caption = ereg_replace("\r","",$caption);
    $caption = preg_replace("/\r/","",$caption);
##	$caption = ereg_replace("\n","",$caption);
	$caption = preg_replace("/\n/","",$caption);


	//	設定
	$path = $category_list;
	$name = $g_name;				//	del ookawara 2015/09/17		//	add ookawara 2015/09/24
	//$name = $name_head.$g_name;		//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
	$code = $code;

	if (TAXABLE == 1) {
		$original_price = floor($base_price * ((TAX / 100) + 1) + 0.5);
	} else {
		$original_price = $base_price;
	}
	$price = $price;
	$sale_price = $sale_price;
	if ($sale_price < 1) { $sale_price = ""; }
	if ($sale_price > 0) { $check_price = $sale_price; } else { $check_price = $price; }
	$wariritu = wariritu($original_price,$check_price);
	$headline = "";
	if ($wariritu && strlen($headline) < 42) {
		$headline .= "★".mb_convert_kana($wariritu,"N")."％ＯＦＦ★";	//	add ookawara 2013/12/09
		//$headline .= "【".mb_convert_kana($wariritu,"N")."％ＯＦＦ】";	//	del ookawara 2013/12/09
		if (defined(YAHOO_DISCOUNT_TIME) && $wariritu <= NEW_YAHOO_DISCOUNT_PAR) {
			$headline .= "【".YAHOO_DISCOUNT_TIME."】";
		}
	}

	if ($sale_price && defined("sale_period_start")) {
		$sale_period_start = sale_period_start;
	} else {
		$sale_period_start = "";
	}
	if ($sale_price && defined("sale_period_end")) {
		$sale_period_end = sale_period_end;
	} else {
		$sale_period_end = "";
	}
	$options = $options;

	$caption = $caption;
	$explanation = $explanation;

	$meta_key = $meta_key;
	$meta_desc = $g_name;	//	add ookawara 2013/04/11

	$yahoo_product_code = "";
	$product_code = "";

	$product_category = $yd_id;

	$brand_code = $L_Y_BEAND[$brand];
	$headline = trim($headline);
	$taxable = "1";
	$display = "1";

	$abstract = "";
	$sub_code = "";
	$additional1 = "";
	$additional2 = "";
	$additional3 = "";
	$relevant_links = "";
	$ship_weight = "";
	$release_date = "";
	$temporary_point_term = "";
	$template = "";
	$sale_limit = "";
	$sp_code = "";
	$person_code = "";
	//$jan = "";		//	del ookawara 2016/04/19
	$jan = $jan_code;	//	add ookawara 2016/04/19
	$isbn = "";
	$delivery = "0";
	$spec1 = $spec2 = $spec3 = $spec4 = $spec5 = "";

	//	期間対策
	if ($price && $sale_price) {
		$price = $sale_price;
		$sale_price = "";
	}
	$sale_period_start = "";
	$sale_period_end = "";

	//	金額確認送料無料コメントセット
	//	add ookawara 2013/12/09
	if (
		($sale_price >= YAHOO_SOURYOU_PRICE)
		|| (!$sale_price && $price >= YAHOO_SOURYOU_PRICE)
	) {
		$headline = YAHOO_SOURYOU_MESSAGE.$headline;
		$delivery = "1";	//	add ookawara 2014/05/02

		//	在庫1商品表記	//	add ookawara 2014/09/22
		//if (preg_match("/^特価商品:/", $path)) {												//	del ookawara 2014/12/26
		if (preg_match("/^特価商品:/", $path) || preg_match("/^限定入荷一点限り:/", $path)) {	//	add ookawara 2014/12/26
			$headline = YAHOO_ZIKO1_MESSAGE.$headline;
		}

	} else {
		//	在庫1商品表記	//	add ookawara 2014/09/22
		//if (preg_match("/^特価商品:/", $path)) {												//	del ookawara 2014/12/26
		if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path)) {	//	add ookawara 2014/12/26
			$headline = YAHOO_ZIKO1_MESSAGE.$headline;
		} else {
			$headline = YAHOO_ATO_SOURYOU_MESSAGE.$headline;
		}
		//$headline = YAHOO_ATO_SOURYOU_MESSAGE.$headline;	//	del ookawara 2014/09/22
	}

/*
	//	del ookawara 2013/12/09
	if ($sale_price >= YAHOO_SOURYOU_PRICE) {
		$headline = YAHOO_SOURYOU_MESSAGE.$headline;
	} elseif (!$sale_price && $price >= YAHOO_SOURYOU_PRICE) {
		$headline = YAHOO_SOURYOU_MESSAGE.$headline;
	}
*/
	$path  = htmlspecialchars($path);
##	$meta_key  = ereg_replace("&","＆",$meta_key);
	$meta_key  = preg_replace("/&/","＆",$meta_key);
##	$meta_desc  = ereg_replace("&","＆",$meta_desc);
	$meta_desc  = preg_replace("/&/","＆",$meta_desc);
##	$name  = ereg_replace("&","＆",$name);						//	add ookawara 2013/04/11
	$name  = preg_replace("/&/","＆",$name);						//	add ookawara 2013/04/11
##	$name  = ereg_replace("<","&lt;",$name);					//	add ookawara 2013/04/11
	$name  = preg_replace("/</","&lt;",$name);					//	add ookawara 2013/04/11
##	$name  = ereg_replace(">","&gt;",$name);					//	add ookawara 2013/04/11
	$name  = preg_replace("/>/","&gt;",$name);					//	add ookawara 2013/04/11
	$name  = preg_replace("/\,/","、",$name);					//	add ookawara 2013/05/22
##	$explanation  = ereg_replace("&","＆",$explanation);		//	add ookawara 2013/04/11
	$explanation  = preg_replace("/&/","＆",$explanation);		//	add ookawara 2013/04/11
##	$explanation  = ereg_replace("<","&lt;",$explanation);	//	add ookawara 2013/04/11
	$explanation  = preg_replace("/</","&lt;",$explanation);	//	add ookawara 2013/04/11
##	$explanation  = ereg_replace(">","&gt;",$explanation);	//	add ookawara 2013/04/11
	$explanation  = preg_replace("/>/","&gt;",$explanation);	//	add ookawara 2013/04/11

	//	動画埋め込み
	//	add ookawara 2014/10/10
	preg_match_all("|＆lt;!-- MOVIE=(.*) --＆gt;|U", $explanation, $MOVIE);
	if ($MOVIE) {
		foreach ($MOVIE[1] AS $key => $VAL) {
			$movie_name = $MOVIE[0][$key];
			$movie_name = change_en($movie_name);

			$movie_num = trim($VAL);
			$movie_code = "";
			$movie_code = $MOVIE_LIST[$movie_num]['Y'];
			if ($movie_code) {
				$movie_code = $movie_code;
			}

			$explanation = preg_replace("/{$movie_name}/", "", $explanation);
			$caption = $movie_code.$caption;
		}
	}

#	$meta_key = mb_substr($meta_key, 0, 160, 'eucJP-win');	//	add ookawara 2013/04/11
	$meta_key = mb_substr($meta_key, 0, 160, 'UTF-8');
#	$meta_desc = mb_substr($meta_desc, 0, 160, 'eucJP-win');	//	add ookawara 2013/04/11
	$meta_desc = mb_substr($meta_desc, 0, 160, 'UTF-8');
	//	add ookawara 2014/02/19
//	if ($original_price < $price) {
//		$price = $original_price;
//	}


	//	商品情報納期表示
	//	add	ookawara 2015/01/30
	$caption .= "<b>納期：</b>";
	$lead_time_instock = 2000;		//	add ookawara 2017/04/10
	$lead_time_outstock = 4000;		//	add ookawara 2017/04/10
	if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path)) {
		//	特価商品、限定入荷一点限りのカテゴリーの商品は即納
		$caption .= $nouki_days_msg;
		$lead_time_outstock = "";	//	add ookawara 2017/04/10
	} elseif ($att_flg == 1) {
		//	商品説明にAttentionが入っていた場合
		$caption .= $attention;
		$lead_time_instock = 1;		//	add ookawara 2017/04/10
		$lead_time_outstock = "";	//	add ookawara 2017/04/10
	} else {
		$caption .= $nouki_days_msg;
	}

	//	add cal ohkawara lead-time-instock,lead-time-outstock
	$csv = "\"$path\",\"$name\",\"$code\",\"$sub_code\",\"$original_price\",\"$price\",\"$sale_price\",\"$options\",\"$headline\",\"$caption\",\"$abstract\",\"$explanation\",\"$additional1\",\"$additional2\",\"$additional3\",\"$relevant_links\",\"$ship_weight\",\"$taxable\",\"$release_date\",\"$temporary_point_term\",\"$meta_key\",\"$meta_desc\",\"$display\",\"$template\",\"$sale_period_start\",\"$sale_period_end\",\"$sale_limit\",\"$sp_code\",\"$brand_code\",\"$person_code\",\"$yahoo_product_code\",\"$product_code\",\"$jan\",\"$isbn\",\"$delivery\",\"$product_category\",\"$spec1\",\"$spec2\",\"$spec3\",\"$spec4\",\"$spec5\",\"$caption\",\"$lead_time_instock\",\"$lead_time_outstock\"\r\n";

	//	在庫csv
	//	add ookawara 2014/07/31
	//if (preg_match("/^特価商品:/", $path)) {												//	del ookawara 2014/12/24
	//if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path)) {	//	add ookawara 2014/12/24	//	del ookawara 2015/09/17
	if (preg_match("/特価商品:/", $path) || preg_match("/限定入荷一点限り:/", $path) || $zaiko == 1) {	//	add ookawara 2015/09/17
		$csv_zaiko = "\"$code\",\"$sub_code\",\"1\",\"\"\r\n";
	} else {																			//	add ohkawara 2017/04/10
		$csv_zaiko = "\"$code\",\"$sub_code\",\"".YAHOO_DEFO_ZAIKO."\",\"\"\r\n";		//	add ohkawara 2017/04/10
	}

	//return $csv;
	return array($csv, $csv_zaiko);

}

//	YAHOO	カテゴリー作成
function make_category_list($num,$YAHOO_LIST,$YAHOO_GOODS_LIST,$L_CATE) {
global $NEWCATE;

	$cate1 = $YAHOO_LIST[$num]['cate1'];
	$cate2 = $YAHOO_LIST[$num]['cate2'];
	$cate3 = $YAHOO_LIST[$num]['cate3'];

	$cate1_name = trim($L_CATE[$cate1][0][0]);
	$cate2_name = trim($L_CATE[$cate1][$cate2][0]);
	$cate3_name = trim($L_CATE[$cate1][$cate2][$cate3]);
	$cate0_name = trim($NEWCATE[$cate1]['name']);

	$category_list = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$category_list .= ":".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$category_list .= ":".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$category_list .= ":".$cate3_name;
	}

	return $category_list."\r\n";

}


//	YAHOO	カテゴリー作成
function moji_seiton($val) {

#	$val = mb_convert_kana($val,"asKV","EUC-JP");
	$val = mb_convert_kana($val,"asKV","UTF-8");
##	$val = eregi_replace("センチ","cm",$val);
	$val = preg_replace("/センチ/i","cm",$val);

	return $val;
}


//-------------------------------------------------------------------------------------
//
//	楽天
//
//-------------------------------------------------------------------------------------

//	楽天アイテム
//function rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST) {	//	add $UP_PIC_LIST ookawara 2014/02/06	//	del ookawara 2015/01/15
//function rakuten_item_csv($list,$L_CATE,$L_BRAND,$L_RAKU,$RAKU_LIST,$RAKU_SP_LIST, $UP_PIC_LIST, &$att_check) {	//	add ookawara 2015/01/15		//	del ohkawara 2017/02/21
function rakuten_item_csv($list, $L_CATE, $L_BRAND, $L_RAKU, &$RAKU_LIST, $RAKU_SP_LIST, $UP_PIC_LIST, &$att_check, $TAG_BRAND_LIST, $TAG_SIZE_LIST, $RKT_DIR_LIST) {	//	add ohkawara 2017/02/21-22
	global $attention,$NEWCATE,$RAKUTEN_GOODS_DISCOUNT_CATE;
	global $MOVIE_LIST;	//	add ookawara 2014/10/10
	global $conn_id;	//	add ookawara 2014/12/29
	global $NON_BRAND;	//	add ookawara 2015/01/09
	global $nouki_days_msg;	//	add ookawara 2015/01/15
	global $attention_wowma;	//	add ohkawara 2017/04/07

	$num = $list['num'];
	$cate1 = $list['cate1'];
	$cate2 = $list['cate2'];
	$cate3 = $list['cate3'];
	$g_num = $list['g_num'];
	$ypath = $list['ypath'];
	$display = $list['display'];
	$copy_num = $list['copy_num'];
	$state = $list['state'];
	$g_name = $list['g_name'];
	$code = $list['code'];
	$price = $list['price'];
	$sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$rd_id = $list['rd_id'];
	$zaiko = $list['zaiko'];		//	add ookawara 2015/09/17
	$size_list = $list['size_list'];	//	add ookawara 2015/09/17
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
	//$g_name = $name_head.$g_name;	//	add ookawara 2015/09/17		//	del ookawara 2015/09/24

	//	該当するブランドは削除
	//	add ookawara 2015/01/08
	if ($NON_BRAND) {
		if ($NON_BRAND[$brand] == 1) {
			return;
		}
	}

//	消費税別で表示する為コメント化	2009/10/30
//	$taxable = TAXABLE;
	$taxable = "0";

	//	コントロールカラム	concaramu
	if (!$RAKU_LIST[$g_num] && $state < 3) { $concaramu = "n"; }
	elseif ($RAKU_LIST[$g_num] && $state > 0 && $state < 3) { $concaramu = "u"; }
	elseif ($RAKU_LIST[$g_num] && $state == 3) { $concaramu = "d"; }

	//	ポイント設定
	$point = "";
	$point_kikan = "";
	$point_type = 0;
	if (RAKUTEN_POINT_KIKAN != "") {
		if ($price > 0 && $sale_price > 0 && $price < $sale_price && RAKUTEN_POINT_TE > 0) {
			$point = RAKUTEN_POINT_TE;
			$point_kikan = RAKUTEN_POINT_KIKAN;
			$point_type = 2;
		} elseif (RAKUTEN_POINT_TU > 0) {
			$point = RAKUTEN_POINT_TU;
			$point_kikan = RAKUTEN_POINT_KIKAN;
			$point_type = 1;
		}
	}

	//	price,sale_price
//	if (TAXABLE == 1) {	//	del ohkawara 2021/03/23
		//	消費税込みで表示する場合

		//	割引設定項目
		$GOODS_DISCOUNT_C = RAKUTEN_GOODS_DISCOUNT_C;
		$GOODS_DISCOUNT_CATE = $RAKUTEN_GOODS_DISCOUNT_CATE;
		$DISCOUNT_PAR = RAKUTEN_DISCOUNT_PAR;
		$DISCOUNT_PAR2 = RAKUTEN_DISCOUNT_PAR2;

		if ($price > 0 && $sale_price > 0 && $price < $sale_price) {
			//	金額と特価共に値が有、金額より特価の金額が大きい場合　定価（金額）売り
//			$sale_price = 0;
			$sale_price = $price;
		} elseif ($price > 0 && $sale_price > 0 && $price == $sale_price) {
			//	金額と特価共に値が有、金額と特価が同じ場合で、セールスしないカテゴリーの商品では無い場合　定価・割引共通商品割引率売り
			if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
				$flag = 0;
				if ($GOODS_DISCOUNT_CATE) {
					foreach ($GOODS_DISCOUNT_CATE AS $val) {
						$val = trim($val);
						if ($cate1 == $val) { $flag = 1; }
					}
				}
				if ($flag != 1) {
					$sale_price = floor($price * ((100 - $DISCOUNT_PAR2) / 100));
				} else {
					$sale_price = 0;
				}
			} else {
				$sale_price = 0;
			}
		} elseif ($GOODS_DISCOUNT_C == 1) {
			//	割引有の場合で、セールスしないカテゴリーの商品では無い場合　割引率売り
			$flag = 0;
			if ($GOODS_DISCOUNT_CATE) {
				foreach ($GOODS_DISCOUNT_CATE AS $val) {
					$val = trim($val);
					if ($cate1 == $val) { $flag = 1; }
				}
			}
			if ($flag != 1) {
				$sale_price_ = floor($price * ((100 - $DISCOUNT_PAR) / 100));
				if ($sale_price < 1 || ($sale_price > 0 && $sale_price > $sale_price_)) {
					$sale_price = $sale_price_;
				}
			} else {
				$sale_price = 0;
			}
		}
//	消費税別で表示する為コメント化	2009/10/30
//		$price = floor($price * ((TAX / 100) + 1) + 0.5);
//		$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);

//	del ohkawara 2021/03/23
//	} else {
//		//	消費税抜きで表示する場合
//		if ($price > 0 && $sale_price > 0 && $price < $sale_price) {
//			$sale_price = $price;
//		} elseif ($price > 0 && $sale_price > 0 && $price == $sale_price) {
//			$sale_price = "";
//		}
//		$price = floor($price);
//		$sale_price = floor($sale_price);
//	}

	//	add ohkawara 2021/03/23
	if (TAXABLE == 1) {
		//	税込み表示が必要になった場合
		$price = floor($price * ((TAX / 100) + 1) + 0.5);
		$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
	}

	if ($sale_price > 0) {
		$price1 = $sale_price;
		$price2 = $price;
	} else {
		$price1 = $price;
		$price2 = "0";
	}
	$wariritu = wariritu($price2,$price1);

	//	メーカー小売価格のみ消費税を追加する為の処理	2009/11/02
	if ($price2 > 0) {
		$price2 = floor($price2 * ((TAX / 100) + 1) + 0.5);
	}

/*
	//	下方に移動
	//	del ookawara 2013/12/17
	//	PC用キャッチコピー
	$pcccopy = "";
	if ($L_BRAND[$brand]) { $pcccopy = $L_BRAND[$brand]; }
	if ($wariritu) {
		//$pcccopy .= "【".mb_convert_kana($wariritu,"N")."％ＯＦＦ】";	//	del ookawara 2014/04/30
		if (defined(RAKUTEN_DISCOUNT_TIME) && $wariritu <= RAKUTEN_DISCOUNT_PAR) {
			$pcccopy .= "【".RAKUTEN_DISCOUNT_TIME."】";
		}
	}

	//	モバイル用キャッチコピー
	$moccopy = "";
	if ($L_BRAND[$brand]) { $moccopy = $L_BRAND[$brand]; }
	if ($wariritu && strlen($moccopy) < 42) {
		//$moccopy .= "【".mb_convert_kana($wariritu,"N")."％ＯＦＦ】";	//	del ookawara 2014/04/30
		if (defined(RAKUTEN_DISCOUNT_TIME) && $wariritu <= RAKUTEN_DISCOUNT_PAR) {
			$moccopy .= "【".RAKUTEN_DISCOUNT_TIME."】";
		}
	}
*/

	//	倉庫指定	souko
	$souko = "0";


	//	カテゴリー
	$cate1_name = $L_CATE[$cate1][0][0];
	$cate2_name = $L_CATE[$cate1][$cate2][0];
	$cate3_name = $L_CATE[$cate1][$cate2][$cate3];
	$cate0_name = $NEWCATE[$cate1]['name'];

	$category_name = $cate0_name;
	$category_name2 = $cate0_name;
	$category_list = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$category_name .= " / ".$cate1_name;
		$category_name2 .= " / ".$cate1_name;
		$category_list .= "\\".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$category_name .= " / ".$cate2_name;
		$category_name2 .= " / ".$cate2_name;
		$category_list .= "\\".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$category_name .= " / ".$cate3_name;
		$category_list .= "\\".$cate3_name;
	}


	//	動画埋め込み
	//	add ookawara 2014/10/10
	$movie_msg = "";
	preg_match_all("|<!-- MOVIE=(.*) -->|U", $caption, $MOVIE);
	if ($MOVIE) {
		foreach ($MOVIE[1] AS $key => $VAL) {
			$movie_name = $MOVIE[0][$key];
			$movie_name = change_en($movie_name);

			$movie_num = trim($VAL);
			$movie_code = "";
			$movie_code = $MOVIE_LIST[$movie_num]['R'];
			if ($movie_code) {
				$movie_code = $movie_code;
			}

			$caption = preg_replace("/{$movie_name}/", "", $caption);
			$movie_msg .= $movie_code;
		}
	}


	//	商品説明文
	//$att_check = 0;	//	del ookawara 2015/01/15
	//	PC用商品説明文	pc_msg
	$caption_ = $caption;
##	if (ereg("<!-- Attention -->",$caption)) {
	if (preg_match("/<!-- Attention -->/",$caption)) {
##		$caption = ereg_replace("<!-- Attention -->","$attention_wowma",$caption);	//	add ohkawara 2017/04/07
		$caption = preg_replace("/<!-- Attention -->/","$attention_wowma",$caption);	//	add ohkawara 2017/04/07
##		//$caption = ereg_replace("<!-- Attention -->","$attention",$caption);	//	del ookawara 2015/01/16
		//$caption = preg_replace("/<!-- Attention -->/","$attention",$caption);	//	del ookawara 2015/01/16
##		//$caption = ereg_replace("<!-- Attention -->", "", $caption);			//	add ookawara 2015/01/16		//	del ohkawara 2017/04/05
		//$caption = preg_replace("/<!-- Attention -->/", "", $caption);			//	add ookawara 2015/01/16		//	del ohkawara 2017/04/05
		$caption = trim($caption);
		$att_check = 1;
	} else {			//	add ookawara 2015/01/15
		//$caption  .= $nouki_days_msg;	//	add ookawara 2015/01/15		//	del ookawara 2015/01/16
		//$caption_ .= $nouki_days_msg;	//	add ookawara 2015/01/15		//	del ookawara 2015/01/16

		$caption  .= $nouki_days_msg2;	//	add ohkawara 2017/04/07
		$caption_ .= $nouki_days_msg2;	//	add ohkawara 2017/04/07
	}


##	$caption = ereg_replace("<","&lt;",$caption);
	$caption = preg_replace("/</","&lt;",$caption);
##	$caption = ereg_replace(">","&gt;",$caption);
	$caption = preg_replace("/>/","&gt;",$caption);

	$caption = preg_replace("/&lt;font color=#FF0000&gt;/", "<font color=#FF0000>", $caption);	//	add ohkawara 2017/04/07
	$caption = preg_replace("/&lt;\/font&gt;/", "</font>", $caption);							//	add ohkawara 2017/04/07

##	$caption = ereg_replace("&lt;font","<font",$caption);
	$caption = preg_replace("/&lt;font/","<font",$caption);
##	$caption = ereg_replace("\"&gt;","\">",$caption);
	$caption = preg_replace("/\"&gt;/","\">",$caption);
##	$caption = ereg_replace("&lt;\/font&gt;","</font>",$caption);
	$caption = preg_replace("/&lt;\/font&gt;/","</font>",$caption);
##	//$caption = ereg_replace("&lt;\br /&gt;","</font>",$caption);	//	del ookawara 2015/08/07
	//$caption = preg_replace("/&lt;\br \/&gt;/","</font>",$caption);	//	del ookawara 2015/08/07
##	//$caption = ereg_replace("&lt;\br&gt;","</font>",$caption);	//	del ookawara 2015/08/07
	//$caption = preg_replace("/&lt;\br&gt;/","</font>",$caption);	//	del ookawara 2015/08/07
##	$caption = ereg_replace("&lt;\br /&gt;","<br />",$caption);		//	add ookawara 2015/08/07
	$caption = preg_replace("/&lt;\br \/&gt;/","<br />",$caption);		//	add ookawara 2015/08/07
##	$caption = ereg_replace("&lt;\br&gt;","<br>",$caption);			//	add ookawara 2015/08/07
	$caption = preg_replace("/&lt;\br&gt;/","<br>",$caption);			//	add ookawara 2015/08/07



	$caption = nl2br($caption);
	$caption = <<<WAKABA
$attation
ブランド名：$L_BRAND[$brand]<br />
$caption
WAKABA;
##	$caption = ereg_replace(",","、",$caption);
	$caption = preg_replace("/,/","、",$caption);
	$pc_msg = "<p style=\"width: 500px;\">$caption";
##	$pc_msg = ereg_replace("\r","",$pc_msg);
	$pc_msg = preg_replace("/\r/","",$pc_msg);
##	$pc_msg = ereg_replace("\n","",$pc_msg);
	$pc_msg = preg_replace("/\n/","",$pc_msg);
##	$pc_msg = ereg_replace("<br />","<br>",$pc_msg);
	$pc_msg = preg_replace("/<br \/>/","<br>",$pc_msg);

	//	モバイル用商品説明文	mobile_msg
	$caption_ = <<<WAKABA
$attention_wowma
ブランド名：$L_BRAND[$brand]<br />
$caption_
WAKABA;


	if ($att_check == 1) {
##		//$caption_ = ereg_replace("<!-- Attention -->", "", $caption_);	//	del ookawara 2015/01/16
		//$caption_ = preg_replace("/<!-- Attention -->/", "", $caption_);	//	del ookawara 2015/01/16
##		$caption_ = ereg_replace("<!-- Attention -->", "", $caption_);	//	add ookawara 2015/01/16
		$caption_ = preg_replace("/<!-- Attention -->/", "", $caption_);	//	add ookawara 2015/01/16
	}
##	$caption_ = ereg_replace("<","&lt;",$caption_);
	$caption_ = preg_replace("/</","&lt;",$caption_);
##	$caption_ = ereg_replace(">","&gt;",$caption_);
	$caption_ = preg_replace("/>/","&gt;",$caption_);
	$caption_ = nl2br($caption_);
	$mobile_msg = strip_tags($caption_,"<br /><br />");
	$m_len = strlen($mobile_msg);
	if ($m_len > 512) {
		$mobile_msg = <<<WAKABA
$attation
ブランド名：$L_BRAND[$brand]
WAKABA;

		if ($att_check == 1) {
			//$mobile_msg2 = "$mobile_msg<br />$attention2";	//	del ookawara 2015/01/16
			$mobile_msg2 = "$mobile_msg";						//	add ookawara 2015/01/16
			$m_len = strlen($mobile_msg2);
			if ($m_len <= 512) {
				$mobile_msg = $mobile_msg2;
			} else {
				//$mobile_msg = "$attention2";					//	del ookawara 2015/01/16
			}
		}

	}

	$mobile_msg = nl2br($mobile_msg);																				//	add ohkawara 2017/04/07
	$mobile_msg = preg_replace("/&lt;br \/&gt;/", "<br />", $mobile_msg);											//	add ohkawara 2017/04/06
	$mobile_msg = preg_replace("/&lt;br&gt;/", "<br>", $mobile_msg);												//	add ohkawara 2017/04/06
	$mobile_msg = preg_replace("/&lt;font color=#FF0000&gt;/", "<font color=#FF0000>", $mobile_msg);				//	add ohkawara 2017/04/07
	$mobile_msg = preg_replace("/&lt;\/font&gt;/", "</font>", $mobile_msg);											//	add ohkawara 2017/04/07

##	$mobile_msg = ereg_replace("\r","",$mobile_msg);
    $mobile_msg = preg_replace("/\r/","",$mobile_msg);
##	$mobile_msg = ereg_replace("\n","",$mobile_msg);
	$mobile_msg = preg_replace("/\n/","",$mobile_msg);
##	$mobile_msg = ereg_replace("<br />","<br>",$mobile_msg);
    $mobile_msg = preg_replace("/<br \/>/","<br>",$mobile_msg);




	//	画像	img_msg	img_alt_msg
	if (!file_exists(UP_IMG_01_DIR)) {
		mkdir(UP_IMG_01_DIR,0777);
		@chmod(UP_IMG_01_DIR,0777);
	}
	if (!file_exists(UP_IMG_02_DIR)) {
		mkdir(UP_IMG_02_DIR,0777);
		@chmod(UP_IMG_02_DIR,0777);
	}
	if (!file_exists(UP_IMG_03_DIR)) {
		mkdir(UP_IMG_03_DIR,0777);
		@chmod(UP_IMG_03_DIR,0777);
	}
	if (!file_exists(UP_IMG_04_DIR)) {
		mkdir(UP_IMG_04_DIR,0777);
		@chmod(UP_IMG_04_DIR,0777);
	}
	if (!file_exists(UP_IMG_05_DIR)) {
		mkdir(UP_IMG_05_DIR,0777);
		@chmod(UP_IMG_05_DIR,0777);
	}

	//	add ookawara 2015/09/27
	if (!file_exists(UP_IMG_06_DIR)) {
		mkdir(UP_IMG_06_DIR,0777);
		@chmod(UP_IMG_06_DIR,0777);
	}
	if (!file_exists(UP_IMG_07_DIR)) {
		mkdir(UP_IMG_07_DIR,0777);
		@chmod(UP_IMG_07_DIR,0777);
	}
	if (!file_exists(UP_IMG_08_DIR)) {
		mkdir(UP_IMG_08_DIR,0777);
		@chmod(UP_IMG_08_DIR,0777);
	}
	if (!file_exists(UP_IMG_09_DIR)) {
		mkdir(UP_IMG_09_DIR,0777);
		@chmod(UP_IMG_09_DIR,0777);
	}

	$fd_num = floor($g_num / 2000);	//	update 500→2000 ookawara 2013/12/04
	$amari = $g_num % 2000;	//	update 500→2000 ookawara 2013/12/04
	if ($amari > 0) { $fd_num += 1; }
	$fd_num = $fd_num * 2000;	//	update 500→2000 ookawara 2013/12/04
	if ($fd_num > 0) {
		$image01 = "img01_$fd_num";
		$image02 = "img02_$fd_num";
		$image03 = "img03_$fd_num";	//	add ookawara 2013/07/03
		$image04 = "img04_$fd_num";	//	add ookawara 2013/10/10
		$image05 = "img05_$fd_num";	//	add ookawara 2013/10/10
		$dir01 = UP_IMG_01_DIR."/$image01";
		$dir02 = UP_IMG_02_DIR."/$image02";
		$dir03 = UP_IMG_03_DIR."/$image03";	//	add ookawara 2013/07/03
		$dir04 = UP_IMG_04_DIR."/$image04";	//	add ookawara 2013/10/10
		$dir05 = UP_IMG_05_DIR."/$image05";	//	add ookawara 2013/10/10

		$dir06 = UP_IMG_06_DIR."/$image06";	//	add ookawara 2015/09/27
		$dir07 = UP_IMG_07_DIR."/$image07";	//	add ookawara 2015/09/27
		$dir08 = UP_IMG_08_DIR."/$image08";	//	add ookawara 2015/09/27
		$dir09 = UP_IMG_09_DIR."/$image09";	//	add ookawara 2015/09/27

		if (!file_exists($dir01)) {
			mkdir($dir01,0777);
			@chmod($dir01,0777);
		}
		if (!file_exists($dir02)) {
			mkdir($dir02,0777);
			@chmod($dir02,0777);
		}

		//	add ookawara 2013/07/04
		if (!file_exists($dir03)) {
			mkdir($dir03,0777);
			@chmod($dir03,0777);
		}

		//	add ookawara 2013/10/10
		if (!file_exists($dir04)) {
			mkdir($dir04,0777);
			@chmod($dir04,0777);
		}

		//	add ookawara 2013/10/10
		if (!file_exists($dir05)) {
			mkdir($dir05,0777);
			@chmod($dir05,0777);
		}

		//	add ookawara 2015/09/27
		if (!file_exists($dir06)) {
			mkdir($dir06,0777);
			@chmod($dir06,0777);
		}
		if (!file_exists($dir07)) {
			mkdir($dir07,0777);
			@chmod($dir07,0777);
		}
		if (!file_exists($dir08)) {
			mkdir($dir08,0777);
			@chmod($dir08,0777);
		}
		if (!file_exists($dir09)) {
			mkdir($dir09,0777);
			@chmod($dir09,0777);
		}

	}

	//	全商品ディレクトリID	category_id
	$category_id = $rd_id;
	//	add ohkawara 2017/02/22
	$tag_chk_flg = 0;
	if ($RAKU_LIST) {
		$line = $RAKU_LIST[$g_num];
		$LINE_VAL = str_getcsv($line,',','"');
		$seted_category_id = $LINE_VAL[3];

		if ($category_id != $seted_category_id) {
			if (isset($RKT_DIR_LIST[$seted_category_id]) && $RKT_DIR_LIST[$seted_category_id] != 1) {
				$tag_chk_flg = 1;
				$category_id = $seted_category_id;
			}
		}
	}

	//	タグID
	//	add ohkawara 2017/02/21
	$set_tag_id = "";
	$TAG_LIST = array();
	$brand_name = $L_BRAND[$brand];

	//if ($TAG_BRAND_LIST) {											//	del ohkawara 2017/04/06
	//	foreach ($TAG_BRAND_LIST AS $key_name => $id_num) {				//	del ohkawara 2017/04/06
	if ($TAG_BRAND_LIST[$rd_id]) {										//	add ohkawara 2017/04/06
		foreach ($TAG_BRAND_LIST[$rd_id] AS $key_name => $id_num) {		//	add ohkawara 2017/04/06
			if (preg_match("/$key_name/", $brand_name)) {
				$TAG_LIST[] = $id_num;
				break;
			}
		}
	}
	//	サイズは以下で設定

	//	サイズチェック
	unset($val);
	$new_pic_check = 0;
	$op_check = 0;
	$op_check2 = 0;
	$jan_code = "";	//	add ookawara 2016/06/22
	if ($concaramu != "d") {
		//	add ookawara 2015/09/17	start
		$L_SIZE = array();
		if ($size_list) {
			$op_size = unserialize($size_list);
		}
		if ($op_size) {
			foreach ($op_size AS $key => $VAL) {

				//	add ookawara 2016/06/22
				if ($VAL['jan'] != "none" || $VAL['jan'] > 0) {
					if (!preg_match("/[^0-9]/", $VAL['jan'])) {
						$jan_code = $VAL['jan'];
					}
				}

				if ($VAL['type'] == 3) {
					continue;
				}
				$L_SIZE[] = $VAL['size'];
			}
		} else {
		//	add ookawara 2015/09/17	end
			$options = trim($options);
			$L_SIZE = explode("\n",$options);
		}	//	add ookawara 2015/09/17

		$flag = 0;
		if ($L_SIZE) {
			foreach ($L_SIZE AS $val) {
				$val = preg_replace("/㎝/","cm",$val);	//	2010/06/10 ookawara
				$val = trim($val);
				if ($val == "") { continue; }
##				$val = eregi_replace("\[.*\]","",$val);
                $val = preg_replace("/\[.*\]/i","",$val);

				if ($state <= 2) {
##					if (ereg("^\*",$val)) {
                    if ( preg_match("/^\*/",$val)) {
						$op_check = 1;
						continue;
					}
##					$val = eregi_replace("^\/","",$val);
					$val = preg_replace("/^\//i","",$val);
				}

##				if (ereg("^\/",$val)) { $new_pic_check = 1; }
				if (preg_match("/^\//",$val)) { $new_pic_check = 1; }

				if ($val == "F" || 
					$val == "フリー" || 
					$val == "フリーサイズ" || 
					$val == "--" || 
					$val == "フットサルサイズ" || 
					$val == "設定なし"
				) {
					$op_check2 = 1;	//	add ookawara 2010/05/25
					unset($val);
					continue;
				} elseif ($val) {
					$op_check2 = 1;	//	add ookawara 2010/05/25
					$flag = 1;
				}

				//	サイズ　タグID取得
				//	add ohkawara 2017/02/21
				$size_val = preg_replace("/cm/", "", $val);
				//if ($TAG_SIZE_LIST[$size_val]) {						//	del ohkawara 2017/04/06
				//	$TAG_LIST[] = $TAG_SIZE_LIST[$size_val];			//	del ohkawara 2017/04/06
				//}														//	del ohkawara 2017/04/06
				if ($TAG_SIZE_LIST[$rd_id][$size_val]) {				//	add ohkawara 2017/04/06
					$TAG_LIST[] = $TAG_SIZE_LIST[$rd_id][$size_val];	//	add ohkawara 2017/04/06
				}														//	add ohkawara 2017/04/06
			}
		}
	}

	//	サイズ　タグIDセット
	if ($TAG_LIST) {
		foreach ($TAG_LIST AS $set_id) {
			if ($set_tag_id != "") {
				$set_tag_id .= "/";
			}
			$set_tag_id .= $set_id;
		}
	}
	if ($tag_chk_flg == 1) {
		$set_tag_id = "";
	}

	//	add ookawara 2010/05/25
	if ($op_check == 1 && $op_check2 != 1) {
		return;
	}


	if ($concaramu != "d") {
#		$g_name_ = mb_convert_kana("$g_name","S","EUC-JP");
		$g_name_ = mb_convert_kana("$g_name","S","UTF-8");
		//	一枚目画像
		$imagef_file = IMG_F_DIR."/$code" . ".jpg";
		if (file_exists($imagef_file)) {
			$imgfile = "$dir01/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($imagef_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($imagef_file,$imgfile);
				}
			}
			$img_msg = "https://image.rakuten.co.jp/nbs-soccer/cabinet/image01/$image01/$g_num.jpg ";
		}

		//	二枚目画像
		$imageb_file = IMG_B_DIR."/$code" . ".jpg";
		if (file_exists($imageb_file)) {
			$imgfile = "$dir02/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($imageb_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
				//if (filectime($imageb_file) > CHECK_TIME || $new_pic_check == 1) {
				//if (filectime($imageb_file) > CHECK_TIME) {
					copy($imageb_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image02/$image02/$g_num.jpg ";
		}

		//	add ookawara 2013/07/04
		//	3枚目画像
		$image03_file = IMG_03_DIR."/$code" . ".jpg";
		if (file_exists($image03_file)) {
			$imgfile = "$dir03/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image03_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($image03_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image03/$image03/$g_num.jpg ";
		}

		//	add ookawara 2013/10/10
		//	4枚目画像
		$image04_file = IMG_04_DIR."/$code" . ".jpg";
		if (file_exists($image04_file)) {
			$imgfile = "$dir04/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image04_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($image04_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image04/$image04/$g_num.jpg ";
		}

		//	add ookawara 2013/10/10
		//	5枚目画像
		$image05_file = IMG_05_DIR."/$code" . ".jpg";
		if (file_exists($image05_file)) {
			$imgfile = "$dir05/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image05_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {	//	add  || $UP_PIC_LIST[$g_num] != "" ookawara 2014/02/06
					copy($image05_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image05/$image05/$g_num.jpg ";
		}

		//	add ookawara 2015/09/27
		//	6枚目画像
		$image06_file = IMG_06_DIR."/$code" . ".jpg";
		if (file_exists($image06_file)) {
			$imgfile = "$dir06/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image06_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image06_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image06/$image06/$g_num.jpg ";
		}
		$image07_file = IMG_07_DIR."/$code" . ".jpg";
		if (file_exists($image07_file)) {
			$imgfile = "$dir07/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image07_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image07_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image07/$image07/$g_num.jpg ";
		}
		$image08_file = IMG_08_DIR."/$code" . ".jpg";
		if (file_exists($image08_file)) {
			$imgfile = "$dir08/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image08_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image08_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image08/$image08/$g_num.jpg ";
		}
		$image09_file = IMG_09_DIR."/$code" . ".jpg";
		if (file_exists($image09_file)) {
			$imgfile = "$dir09/$g_num.jpg";
			if (!file_exists($imgfile)) {
				if (filemtime($image09_file) > CHECK_TIME || $new_pic_check == 1 || $UP_PIC_LIST[$g_num] != "") {
					copy($image09_file,$imgfile);
				}
			}
			$img_msg .= "https://image.rakuten.co.jp/nbs-soccer/cabinet/image09/$image09/$g_num.jpg ";
		}

	}


	$size_title = "";
	$zaiko_type = "0";


	if ($price2 < 1) { unset($price2); }
	if ($price1 == $price2) { unset($price2); }
	if ($RAKU_SP_LIST[$g_num] > 0 && $price2 < 1) {
		$price2 = "0";
	}

	//	金額確認送料無料コメントセット
	if ($price1 >= RAKUTEN_SOURYOU_PRICE && defined("RAKUTEN_SOURYOU_MESSAGE")) {
		$g_name = RAKUTEN_SOURYOU_MESSAGE.$g_name;
	}

	//	キーワードセット
	if ($point_type == 1 && $wariritu >= 50) {
		//	割引額によってポイント変更
		$point_type = 2;
		$point = RAKUTEN_POINT_TE;
	}

	if ($point_type == 1 && defined("RAKUTEN_KEYWORD_TU") && RAKUTEN_KEYWORD_TU) {
		$g_name .= RAKUTEN_KEYWORD_TU;
	} elseif ($point_type == 2 && defined("RAKUTEN_KEYWORD_TE") && RAKUTEN_KEYWORD_TE) {
		$g_name .= RAKUTEN_KEYWORD_TE;
	}
	$g_name  = preg_replace("/,/","、",$g_name);					//	add ookawara 2013/05/22

	if (RAKUTEN_POINT_NEW != 1) {
		$point = "";
		$point_kikan = "";
	}

	//	キャッチコピー
	//	add ookawara 2013/12/17 start
	//	表示優先順位
	//	1.○以上で送料無料
	//	2.割引率
	//	3.メーカー

	$pcccopy = "";
	$moccopy = "";
	$pc_max_num = 0;
	$mo_max_num = 0;
	//	1.○以上で送料無料
	if (defined("RAKUTEN_ATO_SOURYOU_MESSAGE") && $price1 < RAKUTEN_SOURYOU_PRICE) {
		$pcccopy = RAKUTEN_ATO_SOURYOU_MESSAGE."<br>";
		$moccopy = RAKUTEN_ATO_SOURYOU_MESSAGE." ";
	}
	$pc_max_num = strlen($pcccopy);
	$mo_max_num = strlen($moccopy);
	//	2.割引率
	if ($wariritu) {
		$pc_add_flg = 0;
		$mo_add_flg = 0;
		//$wariritsu_msg = "★".$wariritu."%OFF★";	//	del ookawara 2014/04/30
		$wariritsu_msg_num = strlen($wariritsu_msg);
		$sub_pc_max_num = $pc_max_num + $wariritsu_msg_num;
		$sub_mo_max_num = $mo_max_num + $wariritsu_msg_num;
		if ($sub_pc_max_num <= 174) {
			$pcccopy .= $wariritsu_msg;
			$pc_max_num = $sub_pc_max_num;
			$pc_add_flg = 1;
		}
		if ($sub_mo_max_num <= 60) {
			$moccopy .= $wariritsu_msg;
			$mo_max_num = $sub_mo_max_num;
			$mo_add_flg = 1;
		}

		if (defined(RAKUTEN_DISCOUNT_TIME) && $wariritu <= RAKUTEN_DISCOUNT_PAR) {
			$warikikan = "【".RAKUTEN_DISCOUNT_TIME."】";
			$warikikan_num = strlen($warikikan);
			$sub_pc_max_num = $pc_max_num + $warikikan_num;
			$sub_mo_max_num = $mo_max_num + $warikikan_num;
			if ($sub_pc_max_num <= 174 && $pc_add_flg == 1) {
				$pcccopy .= $warikikan;
				$pc_max_num = $sub_pc_max_num;
			}
			if ($sub_mo_max_num <= 60 && $mo_add_flg == 1) {
				$moccopy .= $warikikan;
				$mo_max_num = $sub_mo_max_num;
			}
		}
		$kaigyou = "<br>";
		$kaigyou_num = strlen($kaigyou);
		$sub_pc_max_num = $pc_max_num + $kaigyou_num;
		if ($sub_pc_max_num <= 174 && $pc_add_flg == 1) {
			$pcccopy .= $kaigyou;
			$pc_max_num = $sub_pc_max_num;
		}

		$kaigyou = " ";
		$kaigyou_num = strlen($kaigyou);
		$sub_mo_max_num = $mo_max_num + $kaigyou_num;
		if ($sub_mo_max_num <= 60 && $mo_add_flg == 1) {
			$moccopy .= $kaigyou;
			$mo_max_num = $sub_mo_max_num;
		}
	}
	if ($L_BRAND[$brand]) {
		$brand_msg = $L_BRAND[$brand]."<br>";
		$brand_msg_num = strlen($brand_msg);
		$sub_pc_max_num = $pc_max_num + $brand_msg_num;
		$sub_mo_max_num = $mo_max_num + $brand_msg_num;
		if ($sub_pc_max_num <= 174) {
			$pcccopy .= $brand_msg;
			$pc_max_num = $sub_pc_max_num;
		}

		$brand_msg = $L_BRAND[$brand];
		$brand_msg_num = strlen($brand_msg);
		$sub_mo_max_num = $mo_max_num + $brand_msg_num;
		if ($sub_mo_max_num <= 60) {
			$moccopy .= $brand_msg;
			$mo_max_num = $sub_mo_max_num;
		}
	}
	//	add ookawara 2013/12/17 end

	//	納期関係設定
	//	add ookawara 2015/01/16
	$zaiko_type = 1;
	$zaiko_num = 0;
	$zaiko_hyouji = 0;
	//$zaiko_nasi_order = 1;	//	del ohkawara 2017/02/22
	$zaiko_nasi_order = 0;		//	add ohkawara 2017/02/22		//	在庫が無い場合は注文を受け付けないようにすべて設定

	//	del ohkawara 2017/04/10
	//$zaiko_ari_nouki_msg = "";
	//$zaiko_nashi_nouki_msg = 1;
	//if ($att_check == 1) {
	//	$zaiko_ari_nouki_msg = "3";		//	add ohkawara	2017/04/05
	//	//$zaiko_ari_nouki_msg = 2;
	//	$zaiko_nashi_nouki_msg = "2";
	//}

	//	add ohkawara 2017/04/10
	$zaiko_ari_nouki_msg = 1;
	$zaiko_nashi_nouki_msg = 2;
	if ($att_check == 1) {
		$zaiko_ari_nouki_msg = "3";
	}


	//	特価商品在庫設定
	//	add ookawara 2014/07/31
	$cyumon_uketsuke = -1;
	//$zaiko_num = "";			//	del ookawara 2015/01/16
	//$zaiko_hyouji = "";		//	del ookawara 2015/01/16
	//	add ookawara 2014/12/29 start
	$cnt = 0;
	$sql  = "SELECT COUNT(*) AS cnt FROM ".T_CATEGORY.
			" WHERE cate1 IN ('23','24')".
			" AND g_num='".$g_num."';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$cnt = $list['cnt'];
	}
	//if ($cnt > 0) {					//	del ookawara 2015/09/17
	if ($cnt > 0 || $zaiko == 1) {		//	add ookawara 2015/09/17
	//	add ookawara 2014/12/29 end
	//if (preg_match("/特価商品/", $cate1_name)) {													//	del ookawara 2014/12/24
	//if (preg_match("/特価商品/", $cate1_name) || preg_match("/限定入荷一点限り/", $cate1_name)) {	//	add ookawara 2014/12/24	/
		$cyumon_uketsuke = 0;
		$zaiko_type = 1;
		$zaiko_num = 1;
		$zaiko_nasi_order = 0;			//	add ookawara 2015/01/16
		if ($att_check != 1) {			//	add ohkawara	2017/04/05
			$zaiko_ari_nouki_msg = 1;	//	add ookawara 2015/01/16
		}								//	add ohkawara	2017/04/05
		$zaiko_nashi_nouki_msg = "";	//	add ookawara 2015/01/19
	//	add ohkawara 2017/02/15	start
	} else {
		$cyumon_uketsuke = -1;
		$zaiko_type = "1";
		$zaiko_num = RAKUTEN_DEFO_ZAIKO;
		$zaiko_nasi_order = "0";
	//	add ohkawara 2017/02/15	end
	}

	//	add ookawara 2015/07/06
	$sp_msg = $mobile_msg;
	if (strlen($pc_msg) < 5120) {
		$sp_msg = $pc_msg;
		$sp_msg = preg_replace("/\<p style=\"width: 500px;\"\>/", "", $sp_msg);
	}

	//	add	ookawara	2016/06/22
	//$code2 = $code;	//	del ohkawara 2017/02/20
	$code2 = "";			//	add ohkawara 2017/02/20
	if ($jan_code > 0) {
		$code2 = $jan_code;
	}
	$code_none = "";
	if ($code2 == 0) {
		//$code2 = 0;			//	add ohkawara 2018/10/24	//	del ohkawara 2018/11/01
		$code_none = 5;
	}

	//	書籍などの時、カタログID（JAN）を削除する
	//	2017/08/07	様子見で一時削除
	//	add ohkawara 2017/08/02
	//if ($cate1 == 92 && $cate2 == 14) {
	//	$code = "";
	//	$code2 = "";
	//	$code_none = 5;
	//}

	if ($concaramu && $concaramu != "d") {

		$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$set_tag_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0,,0,,,,\n";	//	add ohkawara 2020/02/18

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$set_tag_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0,,0,,\n";	//	add ohkawara 2020/02/07	//	del ohkawara 2020/02/18

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$set_tag_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0,,0\n";	//	add ohkawara 2018/10/22	//	del ohkawara 2020/02/07

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$set_tag_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,,0,{$pc_msg},{$mobile_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,2,,0,0,{$code_none},0\n";	//	add ohkawara 2017/02/21	//	del ohkawara 2018/10/22

		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code2},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,\n";	//	add ookawara 2016/06/22
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},{$sp_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,\n";	//	add ookawara 2015/07/06	//	del ookawara 2016/06/22
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},,{$img_msg},{$img_alt_msg},,,-1,{$zaiko_type},,,{$size_title},,,,,{$code},,,,,,$point,$point_kikan,,,,,,\n";
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code},,,,,,$point,$point_kikan,,,,,,\n";
		//$r1_csv = "{$concaramu},g{$g_num},{$code},{$category_id},{$pcccopy},{$moccopy},{$g_name},{$price1},{$price2},{$taxable},0,,,,0,{$souko},1,1,0,1,1,0,{$pc_msg},{$mobile_msg},,{$img_msg},{$img_alt_msg},{$movie_msg},,{$cyumon_uketsuke},{$zaiko_type},{$zaiko_num},{$zaiko_hyouji},{$size_title},,,,,{$code},,{$zaiko_nasi_order},{$zaiko_ari_nouki_msg},{$zaiko_nashi_nouki_msg},,$point,$point_kikan,,,,,,\n";	//	del ookawara 2015/07/06
	}

	return $r1_csv;

}



//	割引率計算
function wariritu($price,$sale_price) {

	$wariritu = 0;
	if ($price && $sale_price) {
		$wariritu = 100 - round($sale_price/$price*100);
	}
	if ($wariritu < 0) { $wariritu = 0; }

	return $wariritu;
}



//	楽天セレクト
//function rakuten_select_csv($list,$RAKU_S_LIST) {				//	del ookawara 2015/01/15
function rakuten_select_csv($list, &$RAKU_S_LIST, $att_check) {	//	add ookawara 2015/01/15

	$num = $list['num'];
	$cate1 = $list['cate1'];
	$cate2 = $list['cate2'];
	$cate3 = $list['cate3'];
	$g_num = $list['g_num'];
	$ypath = $list['ypath'];
	$display = $list['display'];
	$copy_num = $list['copy_num'];
	$state = $list['state'];
	$g_name = $list['g_name'];
	$code = $list['code'];
	$price = $list['price'];
	$sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$taxable = TAXABLE;
	$zaiko = $list['zaiko'];			//	add ookawara 2015/09/17
	$size_list = $list['size_list'];	//	add ookawara 2015/09/17
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17		//	del ookawara 2015/09/24
	//$g_name = $name_head.$g_name;	//	add ookawara 2015/09/17			//	del ookawara 2015/09/24


	//	サイズ	サイズは、登録か、削除のみ　変更は無し
	//	add ookawara 2015/09/17	start
	$L_SIZE = array();
	if ($size_list) {
		$op_size = unserialize($size_list);
	}
	if ($op_size) {
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['type'] == 3) {
				continue;
			}
			$L_SIZE[] = $VAL['size'];
		}
	} else {
	//	add ookawara 2015/09/17	end
		$options = trim($options);
		$L_SIZE = explode("\n",$options);
	}	//	add ookawara 2015/09/17

	if ($L_SIZE) {
		foreach ($L_SIZE AS $val) {
			$val = trim($val);
			if ($val == "") { continue; }
##			$val = eregi_replace("\[.*\]","",$val);
			$val = preg_replace("/\[.*\]/i","",$val);

			if ($state < 3) {
##				if (ereg("^\*",$val)) { continue; }
				if (preg_match("/^\*/",$val)) { continue; }
##				$val = eregi_replace("^\/","",$val);
				$val = preg_replace("/^\//i","",$val);
			}

			if ($val == "F" || 
				$val == "フリー" || 
				$val == "フリーサイズ" || 
				$val == "--" || 
				$val == "フットサルサイズ" || 
				$val == "設定なし"
			) {
				unset($val);
				continue;
			}
##			$val = ereg_replace(":","：",$val);
			$val = preg_replace("/:/","：",$val);

			$size_name = "サイズ";
			if ($g_num == "7347" || $g_num == "7348" || $g_num == "7349") {
				$size_name = "カラー";
			}

			//	add ookawara 2015/01/15
			$haisou_kikan = 1;
			if ($att_check == 1) {
				$haisou_kikan = 2;
			}
			$haisou_kikan = "";		//	サイズ毎に在庫を設定する場合に利用するため値を利用しないようにする。

			//	すでに登録されているかチェック
			if ($val && !$RAKU_S_LIST[$g_num][$val]) {
				//$select_csv .= "\"n\",\"g{$g_num}\",\"s\",\"{$size_name}\",\"{$val}\",\"\",\"\",\"\",\"\",\"\",\"\"\n";		//	del ookawara 2015/01/15
				//$select_csv .= "\"n\",\"g{$g_num}\",\"s\",\"{$size_name}\",\"{$val}\",\"\",\"\",\"\",\"\",\"\",\"\",\"{$haisou_kikan}\"\n";	//	add ookawara 2015/01/15	//	del ohkawara 2020/02/07
				$select_csv .= "\"n\",\"g{$g_num}\",\"s\",\"{$size_name}\",\"{$val}\",\"\",\"\",\"\",\"\",\"\",\"\",,,,,,,\n";	//	add ohkawara 2020/02/07
			}
			$CHECK_SELECT[$val] = $val;
		}
	}

	return array($select_csv,$CHECK_SELECT);

}



//	楽天カテゴリー
function rakuten_cat_csv($list, $L_CATE, &$RAKU_C_LIST) {
global $attention,$NEWCATE;

	$num = $list['num'];
	$cate1 = $list['cate1'];
	$cate2 = $list['cate2'];
	$cate3 = $list['cate3'];
	$g_num = $list['g_num'];
	$ypath = $list['ypath'];
	$display = $list['display'];
	$copy_num = $list['copy_num'];
	$state = $list['state'];
	$g_name = $list['g_name'];
	$code = $list['code'];
	$price = $list['price'];
	$sale_price = $list['sale_price'];
	$options = $list['options'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$taxable = TAXABLE;
	//$name_head = $list['name_head'];	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24
	//$g_name = $name_head.$g_name;	//	add ookawara 2015/09/17	//	del ookawara 2015/09/24


	//	カテゴリー
	$cate1_name = $L_CATE[$cate1][0][0];
	$cate2_name = $L_CATE[$cate1][$cate2][0];
	$cate3_name = $L_CATE[$cate1][$cate2][$cate3];
	$cate0_name = $NEWCATE[$cate1]['name'];

	$category_list = $cate0_name;
	if ($cate0_name != $cate1_name) {
		$category_list .= "\\".$cate1_name;
	}
	if ($cate1_name != $cate2_name) {
		$category_list .= "\\".$cate2_name;
	}
	if ($cate2_name != $cate3_name) {
		$category_list .= "\\".$cate3_name;
	}
	$category_list = trim($category_list);

	if (!$RAKU_C_LIST[$g_num][$category_list]) {
		$cat_csv = "n,g{$g_num},{$g_name},{$category_list},{$yuusen},,\n";
	}

	return array($cat_csv,$category_list);

}



//-------------------------------------------------------------------------------------
//
//	Amazon
//
//-------------------------------------------------------------------------------------

function amazon_item_tsv($list, $L_BRAND, &$AMZ_UP_LIST, &$AMZ_SYU_LIST) {
	global $AMAZON_GOODS_DISCOUNT_CATE;

/*
	前回更新した情報を（update商品のみ）ログテーブルに保存しておく
	保存される情報は、ログ管理番号、商品管理番号、サイズ、削除フラグ、または、在庫０にしたcsvの情報を保存しておく。
	ログ保存は商品毎にまとめて行う

	親子関係が無い商品は・・・・・・
amazon_csv_log		log_num
amazon_csv_log_tmp	log_num

log_num_seq

nextval('"log_num_seq"'::text::regclass)
*/

	$tsv = "";

	$g_num = $list['g_num'];
	$g_name = $list['g_name'];
	$g_name = preg_replace("/\//", " ", $g_name);
	$code = $list['code'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$headline = $list['headline'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$comment = $list['comment'];

	$ad_id = $list['ad_id'];
	$ad_id2 = $list['ad_id2'];
	$size_list = $list['size_list'];
	$name_head = $list['name_head'];
	$bullet_point1 = $list['bullet_point1'];
	$bullet_point2 = $list['bullet_point2'];
	$bullet_point3 = $list['bullet_point3'];
	$bullet_point4 = $list['bullet_point4'];
	$bullet_point5 = $list['bullet_point5'];
	$generic_keywords1 = $list['generic_keywords1'];
	$generic_keywords2 = $list['generic_keywords2'];
	$generic_keywords3 = $list['generic_keywords3'];
	$generic_keywords4 = $list['generic_keywords4'];
	$generic_keywords5 = $list['generic_keywords5'];
	$generic_keywords = $generic_keywords1;	//	add ohkawara 2017/02/16

	$product_subtype = $list['product_subtype'];
	$department_name = $list['department_name'];

	$distribution_designation = $list['distribution_designation'];
	$model_year = $list['model_year'];
	$seasons = $list['seasons'];

	$amz_price = $list['amz_price'];	//	add ookawara 2015/10/08

	$zaiko = $list['zaiko'];		//	add ohkawara	2017/06/15


/*
asinが記載されている商品も一緒に処理をする！
削除ファイルも管理番号をまとめてサイズの配列を作っておいた方がよいかも？


*/
$ck_dis_flg = 0;
if ($g_num == "xxxxxx") {
	$ck_dis_flg = 1;
}

if ($ck_dis_flg == 1) {
	echo "start g_num<>".$g_num."<br>\n";
	echo "<br>\n";
}

	//	サイズがある商品か確認する！　有れば親を作成する!


	//	サイズ毎に商品管理番号を設定する為、サイズ確認を最初に行う
	$op_size = unserialize($size_list);
	$ALL_SIZE = array();
	if ($op_size) {
		$all_cnt = count($op_size);
		if ($all_cnt > 1) {
			$ALL_SIZE[0]['size'] = "";
			$ALL_SIZE[0]['jan'] = "";
			$ALL_SIZE[0]['asin'] = "";
			$ALL_SIZE[0]['type'] = "";		//	add ohkawara 2019/11/15
		}
		$i = 1;
		$asin_flg = 0;

if ($ck_dis_flg == 1) {
	echo "aaasin_flg<>".$asin_flg."<br>\n";
	echo "<br>\n";
}
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['type'] == 3) {
				//	サイズが非表示だったら飛ばす	駄目！
				continue;
			}

if ($ck_dis_flg == 1) {
	echo "jan<>".$VAL['jan']."<br>\n";
	echo "asin<>".$VAL['asin']."<br>\n";
	echo "<br>\n";
}

			//	add ohkawara 2019/11/18
			//	JANがnoneでなく、登録されていて、ASINが登録されていたら出品ファイルに保存する。
			if ($VAL['jan'] != "par" && $VAL['jan'] != "none" && $VAL['asin'] != "") {
				$asin_flg = 1;
			}

			if ($VAL['jan'] == "none") {
				$VAL['jan'] = "";
			}

			//	del ohkawara 2019/11/18
			//if ($VAL['asin'] != "") {
			//	//	asinが1サイズでも登録されていた場合は、出品ファイルに保存する。
			//	//	その時にasinが入っていたサイズは販売しない。
			//	$asin_flg = 1;
			//}

			if ($VAL['jan'] && !$VAL['asin']) {
				//	janコードが登録してありasinコードが登録されていない場合はアップしない
				continue;
			}

			//	サイズ確認	空設定用
			$check_size = $VAL['size'];
			$check_size = preg_replace("/\(日本サイズ\)/", "", $check_size);

			$check_size = preg_replace("/^F$/", "", $check_size);
			$check_size = preg_replace("/フリーサイズ/", "", $check_size);
			$check_size = preg_replace("/フリー/", "", $check_size);
			$check_size = preg_replace("/--/", "", $check_size);
			$check_size = preg_replace("/フットサルサイズ/", "", $check_size);
			$check_size = preg_replace("/設定なし/", "", $check_size);

			if ($check_size) {
				if ($i == 1 && !$ALL_SIZE[0] && $check_size) {
					$ALL_SIZE[0]['size'] = "";
					$ALL_SIZE[0]['jan'] = "";
					$ALL_SIZE[0]['asin'] = "";
					$ALL_SIZE[0]['type'] = "";				//	add ohkawara 2019/11/15
				}
				$ALL_SIZE[$i]['size'] = $check_size;
				$ALL_SIZE[$i]['jan'] = $VAL['jan'];
				$ALL_SIZE[$i]['asin'] = $VAL['asin'];
				$ALL_SIZE[$i]['type'] = $VAL['type'];		//	add ohkawara 2019/11/15
				$i++;
			} else {
				$ALL_SIZE[0]['size'] = "";
				$ALL_SIZE[0]['jan'] = $VAL['jan'];
				$ALL_SIZE[0]['asin'] = $VAL['asin'];
				$ALL_SIZE[0]['type'] = $VAL['type'];		//	add ohkawara 2019/11/15
			}
		}

		//	出品ファイルでアップする商品の場合中途半端な情報を削除する
		if($asin_flg == 1) {
			foreach ($ALL_SIZE AS $key => $VAL) {
				if ($key == 0) {
					unset($ALL_SIZE[0]);
				} elseif ($ALL_SIZE[$key]['jan'] && !$ALL_SIZE[$key]['asin']) {
					unset($ALL_SIZE[$key]);
				}
			}
		}
	}
	//	$ALL_SIZEの設定がなければこの商品の登録はしない（不具合があった場合）
	if (count($ALL_SIZE) < 1) {
		return;
	}

	//	固定設定値 -----------------------------------------------------------------------------------

	//	node_id
	$node_id = preg_replace("/^1_|^2_|^3_/", "", $ad_id);
	$node_id2 = preg_replace("/^1_|^2_|^3_/", "", $ad_id2);
	$node_type = 1;
	if (preg_match("/^2_/", $ad_id)) {
		$node_type = 2;
	} elseif (preg_match("/^3_/", $ad_id)) {
		$node_type = 3;
	}

	//	ブランド名設定(必須)
	$brand_name = $L_BRAND[$brand];

	//	商品タイプ(必須)
	//	服やシューズのみ必要
	//	$product_subtypeの値を表示

	//	メーカー型番(推奨)
	//	管理画面で設定していない為空設定
	$part_number = "";

	//	商品説明文(推奨)
	$att_check = 0;
	if (preg_match("/\<!-- Attention --\>/", $caption)) {
		$att_check = 1;
	}
	$caption = preg_replace("/\<!-- Attention --\>/", "", $caption);
	$caption = preg_replace("/\<!-- Attention--\>/", "", $caption);

	$product_description = $caption;

	//	メーカー名(必須)
	$manufacturer = $brand_name;

	//	メーカー型番(推奨)
	//	情報が無い為空
	$part_number = "";

	//	商品タイプ(必須)
	//	通常固定	ゴルフなどは適切な値をセット
	//$feed_product_type = "SportingGoods";		//	del ohkawara 2019/12/13
	$feed_product_type = $product_subtype;		//	add ohkawara 2019/12/13

	//	商品の販売価格(推奨)
	//	消費税を含んだ金額にする。
	//	割引設定項目
	$GOODS_DISCOUNT_C = AMAZON_GOODS_DISCOUNT_C;
	$GOODS_DISCOUNT_CATE = $AMAZON_GOODS_DISCOUNT_CATE;
	$DISCOUNT_PAR = AMAZON_DISCOUNT_PAR;
	$DISCOUNT_PAR2 = AMAZON_DISCOUNT_PAR2;

	if ($amz_price) {				//	add ookawara 2015/10/08
		$org_sale_price = "";		//	add ookawara 2015/10/08
		$price = $amz_price;		//	add ookawara 2015/10/08
	} elseif ($base_price > 0 && $base_sale_price > 0 && $base_price < $base_sale_price) {
		$org_sale_price = "";
		$price = $base_price;
	} elseif ($base_price > 0 && $base_sale_price > 0 && $base_price == $base_sale_price) {
		if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
			$flag = 0;
			if ($GOODS_DISCOUNT_CATE) {
				foreach ($GOODS_DISCOUNT_CATE AS $val) {
					$val = trim($val);
					if ($cate1 == $val) { $flag = 1; }
				}
			}
			if ($flag != 1) {
				$org_sale_price = floor($base_price * ((100 - $DISCOUNT_PAR2) / 100));
			} else {
				$org_sale_price = "";
			}
		} else {
			$org_sale_price = "";
		}
		$price = $base_price;
	} elseif ($GOODS_DISCOUNT_C == 1) {
		$flag = 0;
		if ($GOODS_DISCOUNT_CATE) {
			foreach ($GOODS_DISCOUNT_CATE AS $val) {
				$val = trim($val);
				if ($cate1 == $val) { $flag = 1; }
			}
		}
		if ($flag != 1) {
			$org_sale_price_ = floor($base_price * ((100 - $DISCOUNT_PAR) / 100));
			if ($base_sale_price < 1) {
				$price = $base_price;
				$org_sale_price = $org_sale_price_;
			} elseif ($base_sale_price > $org_sale_price_) {
				$price = $base_sale_price;
				$org_sale_price = $org_sale_price_;
			} else {
				$price = $base_price;
				$org_sale_price = $base_sale_price;
			}
		} else {
			if ($base_sale_price) {
				$price = $base_sale_price;
				$org_sale_price = "";
			} else {
				$price = $base_price;
				$org_sale_price = "";
			}
		}
	}
	$price = floor($price * ((TAX / 100) + 1) + 0.5);
	$org_sale_price = floor($org_sale_price * ((TAX / 100) + 1) + 0.5);
	if ($org_sale_price < 1) {
		$standard_price = $price;
	} else {
		$standard_price = $org_sale_price;
	}

	//	通貨コード(推奨)
	//	値は固定
	//$currency = "JPY";	//	del ohkawara 2017/02/16	設定無くなった

	//	商品の公開日(任意)
	$product_site_launch_date = "";

	//	商品のコンディション(推奨)
	//	新品のみの販売の他、「New」を固定とする。中古商品を販売する場合はシステム変更必要
	$condition_type = "New";

	//	商品のコンディション説明(推奨)
	//	新品のみの販売の他、「New」を固定とする。中古商品を販売する場合はシステム変更必要
	$condition_note = "新品";

	//	メーカー希望小売価格(推奨)
	$list_price = floor($base_price * ((TAX / 100) + 1) + 0.5);

	//	使用しない支払い方法(推奨)
	$optional_payment_type_exclusion = "";

	//	配送日時指定SKUリスト(推奨)
	$delivery_schedule_group_id = AMAZON_DEL_SCHE_GR_ID;

	//	セール価格(推奨)
	$sale_price = "";

	//	セール時ポイント(任意)
	$sale_price_points = "";

	//	セール開始日(任意)
	$sale_from_date = "";

	//	セール終了日(任意)
	$sale_end_date = "";

	//	最大注文個数(任意)
	$max_order_quantity = "";

	//	最大注文個数(任意)
	$item_package_quantity = "";

	//	ギフトメッセージ(任意)
	$offering_can_be_gift_messaged = "";

	//	ギフト包装(任意)
	$offering_can_be_giftwrapped = "";

	//	メーカー製造中止(任意)
	$is_discontinued_by_manufacturer = "";

	//	商品コードなしの理由(任意)
	//$missing_keyset_reason = "";	//	del ohkawara 2017/02/16	設定無くなった

	//	配送パターン(必須)
	//	add ohkawara 2017/02/16
	$merchant_shipping_group_name = "通常配送パターン";

	//	販売形態(並行輸入品)(推奨)
	//	ユニフォームは輸入品があるのでは？
	if ($distribution_designation == 1) {
		$distribution_designation = "JP Parallel Import";
	} else {
		$distribution_designation = "";
	}

	//	商品タックスコード(任意)
	//	add ohkawara 2017/08/01
	$product_tax_code = "";

	//	配送重量の単位(任意)
	$website_shipping_weight_unit_of_measure = "";

	//	配送重量(任意)
	$website_shipping_weight = "";

	//	商品の仕様1 - 商品の仕様5(推奨)
	//	上部で設定
	//$bullet_point1 = "";
	//$bullet_point2 = "";
	//$bullet_point3 = "";
	//$bullet_point4 = "";
	//$bullet_point5 = "";

	//	推奨ブラウズノード1(任意)
	$recommended_browse_nodes1 = $node_id;
	$recommended_browse_nodes  = $node_id;

	//	推奨ブラウズノード2(任意)
	$recommended_browse_nodes2 = $node_id2;

	//	検索キーワード1 - 検索キーワード5(任意)
	//	上部で設定
	//$generic_keywords1 = "";
	//$generic_keywords2 = "";
	//$generic_keywords3 = "";
	//$generic_keywords4 = "";
	//$generic_keywords5 = "";

	//	スタイルキーワード1 - スタイルキーワード5(任意)値有り
	$style_keywords  = "";	//	add ohkawara 2017/02/16
	$style_keywords1 = "";
	$style_keywords2 = "";
	$style_keywords3 = "";
	$style_keywords4 = "";
	$style_keywords5 = "";

	//	プラチナキーワード1 - プラチナキーワード5(任意)
	//	add ohkawara 2017/02/16	設定無くなった
	$platinum_keywords  = "";
	$platinum_keywords1 = "";
	$platinum_keywords2 = "";
	$platinum_keywords3 = "";
	$platinum_keywords4 = "";
	$platinum_keywords5 = "";

	//	対象(任意)
	//	add ohkawara 2017/02/16
	$target_audience_base = "";

	//	コート・ワンピース・チュニック着丈(任意)
	$specific_uses_keywords = "";

	//	add ohkawara 2019/12/12
	$specific_uses_keywords1 = "";
	$specific_uses_keywords2 = "";
	$specific_uses_keywords3 = "";
	$specific_uses_keywords4 = "";
	$specific_uses_keywords5 = "";

	//	商品メイン画像URL(必須)
	$PIC_LIST = array();
#	$g_name_ = mb_convert_kana("$g_name","S","EUC-JP");
	$g_name_ = mb_convert_kana("$g_name","S","UTF-8");
	$main_image_url = "";
	$imagef_amz_file = IMG_F_AMZ_DIR."/$code" . ".jpg";
	$imagef_file = IMG_F_DIR."/$code" . ".jpg";
	$image_main_file = IMG_M_AMZ_DIR."/$code" . ".jpg";	//	add ohkawara 2018/10/23

	//	update ohkawara 2019/12/13	http→httpsに変更　

	if (file_exists($image_main_file)) {										//	add ohkawara 2018/10/23
		$PIC_LIST[] = "https://www.futboljersey.com/image_main_amz/$code.jpg";	//	add ohkawara 2018/10/23
	} elseif (file_exists($imagef_amz_file)) {									//	add ohkawara 2018/10/23
	//if (file_exists($imagef_amz_file)) {										//	del ohkawara 2018/10/23
		$PIC_LIST[] = "https://www.futboljersey.com/imagef_amz/$code.jpg";
	} elseif (file_exists($imagef_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/imagef/$code.jpg";
	}

	//	カラーサンプル画像URL(任意)
	$swatch_image_url = "";

	//	商品のサブ画像URL1 - 商品のサブ画像URL8(任意)
	$other_image_url1 = "";
	$other_image_url2 = "";
	$other_image_url3 = "";
	$other_image_url4 = "";
	$other_image_url5 = "";
	$other_image_url6 = "";
	$other_image_url7 = "";
	$other_image_url8 = "";

	//	1枚目画像
	$imageb_amz_file = IMG_B_AMZ_DIR."/$code" . ".jpg";
	$imageb_file = IMG_B_DIR."/$code" . ".jpg";
	if (file_exists($imageb_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/imageb_amz/$code.jpg";
	} elseif (file_exists($imageb_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/imageb/$code.jpg";
	}
	//	2枚目画像
	$image03_amz_file = IMG_03_AMZ_DIR."/$code" . ".jpg";
	$image03_file = IMG_03_DIR."/$code" . ".jpg";
	if (file_exists($image03_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image03_amz/$code.jpg";
	} elseif (file_exists($image03_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image03/$code.jpg";
	}
	//	3枚目画像
	$image04_amz_file = IMG_04_AMZ_DIR."/$code" . ".jpg";
	$image04_file = IMG_04_DIR."/$code" . ".jpg";
	if (file_exists($image04_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image04_amz/$code.jpg";
	} elseif (file_exists($image04_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image04/$code.jpg";
	}
	//	4枚目画像
	$image05_amz_file = IMG_05_AMZ_DIR."/$code" . ".jpg";
	$image05_file = IMG_05_DIR."/$code" . ".jpg";
	if (file_exists($image05_amz_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image05_amz/$code.jpg";
	} elseif (file_exists($image05_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image05/$code.jpg";
	}
	//	5枚目画像
	$image06_file = IMG_06_DIR."/$code" . ".jpg";
	if (file_exists($image06_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image06/$code.jpg";
	}
	//	6枚目画像
	$image07_file = IMG_07_DIR."/$code" . ".jpg";
	if (file_exists($image07_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image07/$code.jpg";
	}
	//	7枚目画像
	$image08_file = IMG_08_DIR."/$code" . ".jpg";
	if (file_exists($image08_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image08/$code.jpg";
	}
	//	8枚目画像
	$image09_file = IMG_09_DIR."/$code" . ".jpg";
	if (file_exists($image09_file)) {
		$PIC_LIST[] = "https://www.futboljersey.com/image09/$code.jpg";
	}

	//	メイン画像が無かった場合の対応処理
	$PIC_NAME_LIST[0] = "main_image_url";
	$PIC_NAME_LIST[1] = "other_image_url1";
	$PIC_NAME_LIST[2] = "other_image_url2";
	$PIC_NAME_LIST[3] = "other_image_url3";
	$PIC_NAME_LIST[4] = "other_image_url4";
	$PIC_NAME_LIST[5] = "other_image_url5";
	$PIC_NAME_LIST[6] = "other_image_url6";
	$PIC_NAME_LIST[7] = "other_image_url7";
	$PIC_NAME_LIST[8] = "other_image_url8";

	if ($PIC_LIST) {
		foreach ($PIC_LIST AS $key => $val) {
			$$PIC_NAME_LIST[$key] = $val;
		}
	}

	//	対象年齢・性別(必須)
	//$department_name = "";	上部で設定

	//	モデル年(発売年・発表年)	Amazonから指定があった場合	西暦数字のみ
	//$model_year = "";	上部で設定

	//	シーズン	Amazonから指定があった場合	指定した値　春　夏　秋　冬　春夏　等
	//$seasons = "";	上部で設定


	//	スポーツ用設定値	上記は洋服専用または、スポーツ共通情報	----------------------

	//	商品タイプ(必須)
	//$feed_product_type = "SportingGoods";	//	del ohkawara 2019/12/13	上部で設定済み


	//	対象1 - 対象4(任意)
	$target_audience_keywords1 = "";
	$target_audience_keywords2 = "";
	$target_audience_keywords3 = "";
	$target_audience_keywords4 = "";
	$DE_NM = explode(",", $department_name);
	if ($DE_NM) {
		$i = 0;
		foreach ($DE_NM AS $val) {
			if (!$val) { continue; }
			if ($val != "メンズ" && $val != "レディース" && $val != "ボーイズ" && $val != "ガールズ") { continue; }
			$set_name = "target_audience_keywords".$i;
			$$set_name = $val;
			$i++;
		}
	}

	//	対象年齢・性別(必須)
	$department_name1 = "";
	$department_name2 = "";
	$department_name3 = "";
	$department_name4 = "";
	$department_name5 = "";
	$DE_NM = explode(",", $department_name);
	if ($DE_NM) {
		$i = 0;
		foreach ($DE_NM AS $val) {
			if (!$val) { continue; }
			$set_name = "department_name".$i;
			$$set_name = $val;
			$i++;
		}
	}

	$JUNIOR_CATEGORY=[68,71,80];
	

	//	未設定項目リセット
	$fulfillment_center_id = "";
	$package_length = "";
	$package_width = "";
	$package_height = "";
	$package_length_unit_of_measure = "";
	$package_weight = "";
	$package_weight_unit_of_measure = "";
	$color_name = "";
	$color_map = "";
	$style_name = "";
	$shoe_width = "";
	$heel_height_unit_of_measure = "";
	$heel_height = "";
	$strap_type = "";
	$toe_shape = "";
	$waist_style = "";
	$opacity = "";
	$heel_type = "";
	$shaft_height = "";
	$outer_material_type = "";
	$shaft_diameter = "";
	$sleeve_type = "";
	$closure_type = "";
	$lifestyle1 = "";
	$lifestyle2 = "";
	$lifestyle3 = "";
	$lifestyle4 = "";
	$lifestyle5 = "";
	$fabric_type = "商品説明文参照"; // 2022/12/9 DBでこの値定義されていない
	$is_adult_product = "";
	$minimum_height_recommendation_unit_of_measure = "";
	$minimum_height_recommendation = "";
	$maximum_height_recommendation_unit_of_measure = "";
	$maximum_height_recommendation = "";
	$waist_size_unit_of_measure = "";
	$waist_size = "";
	$inseam_length_unit_of_measure = "";
	$inseam_length = "";
	$sleeve_length_unit_of_measure = "";
	$sleeve_length = "";
	$collar_style = "";
	$neck_style = "";
	$neck_size_unit_of_measure = "";
	$neck_size = "";
	$bottom_style = "";
	$chest_size_unit_of_measure = "";
	$chest_size = "";
	$cup_size = "";
	$furisode_length_unit_of_measure = "";
	$furisode_length = "";
	$furisode_width_unit_of_measure = "";
	$furisode_width = "";
	$obi_length_unit_of_measure = "";
	$obi_length = "";
	$obi_width_unit_of_measure = "";
	$obi_width = "";
	$tsukeobi_width_unit_of_measure = "";
	$tsukeobi_width = "";
	$tsukeobi_height_unit_of_measure = "";
	$tsukeobi_height = "";
	$pillow_size = "";
	$pillow_size_unit_of_measure = "";
	$fit_to_size_description = "";

	//	スポーツ未設定
	$volume_capacity_name = "";
	$item_display_diameter = "";
	$item_display_diameter_unit_of_measure = "";
	$item_display_height = "";
	$item_display_height_unit_of_measure = "";
	$item_display_length = "";
	$item_display_length_unit_of_measure = "";
	$item_display_width = "";
	$item_display_width_unit_of_measure = "";
	$item_display_weight = "";
	$item_display_weight_unit_of_measure = "";
	$safety_warning = "";
	$country_of_origin = "";
	$legal_disclaimer_description = "";
	$mfg_warranty_description_type = "";
	$seller_warranty_description = "";
	$pattern_name = "";
	$material_type = "";
	$item_package_contents = "";
	$rise_style = "";
	$golf_club_flex = "";
	$golf_club_loft = "";
	$grip_size = "";
	$grip_type = "";
	$grip_material_type = "";
	$rim_size = "";
	$rim_size_unit_of_measure = "";
	$wheel_size = "";
	$wheel_size_unit_of_measure = "";
	$diving_clothing_thickness = "";
	$line_weight = "";
	$tension_level = "";
	$fishing_line_type = "";
	$unknown_subject = "";
	$caliber = "";
	$number_of_rounds = "";
	$boot_size = "";
	$calf_size = "";
	$capacity = "";
	$capacity_unit_of_measure = "";
	$flavor_name = "";
	$lens_color = "";
	$fuel_type = "";
	$is_autographed = "";
	$curvature = "";
	$fencing_pommel_type = "";
	$hand_orientation = "";
	$head_size = "";
	$is_memorabilia = "";
	$shaft_length = "";
	$shaft_length_unit_of_measure = "";
	$shaft_material = "";
	$shaft_style_type = "";
	$item_shape = "";
	$wattage = "";
	$display_maximum_weight_recommendation = "";
	$display_maximum_weight_recommendation_unit_of_measure = "";
	$specific_uses_for_product = "";
	$item_thickness_derived = "";
	$item_thickness_unit_of_measure = "";
	$are_batteries_included = "";
	$batteries_required = "";
	$battery_type = "";		//	add ohkawara 2017/02/16
	$battery_type1 = "";
	$battery_type2 = "";
	$battery_type3 = "";
	$number_of_batteries = "";		//	add ohkawara 2017/02/16
	$number_of_batteries1 = "";
	$number_of_batteries2 = "";
	$number_of_batteries3 = "";
	$lithium_battery_energy_content = "";
	$lithium_battery_packaging = "";
	$lithium_battery_voltage = "";
	$lithium_battery_weight = "";
	$number_of_lithium_ion_cells = "";
	$number_of_lithium_metal_cells = "";
	$team_name = "";			//	add ohkawara 2017/02/16
	$shipping_options = "";		//	add ohkawara 2017/02/16
	$seating_group = "";		//	add ohkawara 2017/02/16

	//	add ohkawara	2019/12/12
	$specific_uses_keywords1 = "";
	$specific_uses_keywords2 = "";
	$specific_uses_keywords3 = "";
	$specific_uses_keywords4 = "";
	$specific_uses_keywords5 = "";
	$item_length_description = "";
	$material_composition1 = "";
	$material_composition2 = "";
	$material_composition3 = "";
	$material_composition4 = "";
	$material_composition5 = "";
	$material_composition6 = "";
	$material_composition7 = "";
	$material_composition8 = "";
	$material_composition9 = "";
	$material_composition10 = "";
	$special_features1 = "";
	$special_features2 = "";
	$special_features3 = "";
	$special_features4 = "";
	$special_features5 = "";
	$band_size_num = "";
	$band_size_num_unit_of_measure = "";
	$inner_material_type1 = "";
	$inner_material_type2 = "";
	$inner_material_type3 = "";
	$inner_material_type4 = "";
	$inner_material_type5 = "";
	$sole_material = "";
	$target_gender = "ユニセックス"; // 2022/12/9 DBでこの値定義されていない	
	$age_range_description = in_array($list["cate1"], $JUNIOR_CATEGORY)?"キッズ":"大人";
	$footwear_size_system = "";
	$footwear_age_group = "";
	$footwear_size_class = "";
	$footwear_width = "";
	$footwear_size = "";
	$footwear_to_size = "";
	$platinum_keywords1 = "";
	$platinum_keywords2 = "";
	$platinum_keywords3 = "";
	$platinum_keywords4 = "";
	$platinum_keywords5 = "";
	$material_type = "";
	$shaft_style_type = "";
	$fit_type = "";
	$shaft_height_unit_of_measure = "";
	$item_width = "";
	$item_height = "";
	$shoe_width_unit_of_measure = "";
	$item_dimensions_unit_of_measure = "";
	$item_length = "";
	$batteries_required = "";
	$are_batteries_included = "";
	$battery_cell_composition = "";
	$battery_type1 = "";
	$battery_type2 = "";
	$battery_type3 = "";
	$number_of_batteries1 = "";
	$umber_of_batteries2 = "";
	$number_of_batteries3 = "";
	$number_of_lithium_metal_cells = "";
	$number_of_lithium_ion_cells = "";
	$lithium_battery_packaging = "";
	$lithium_battery_energy_content = "";
	$lithium_battery_energy_content_unit_of_measure = "";
	$lithium_battery_weight = "";
	$lithium_battery_weight_unit_of_measure = "";
	$item_weight = "";
	$item_weight_unit_of_measure = "";
	$item_volume = "";
	$item_volume_unit_of_measure = "";
	$country_of_origin = "";
	$offering_end_date = "";
	$offering_start_date = "";
	$business_price = "";
	$quantity_price_type = "";
	$quantity_lower_bound1 = "";
	$quantity_price1 = "";
	$quantity_lower_bound2 = "";
	$quantity_price2 = "";
	$quantity_lower_bound3 = "";
	$quantity_price3 = "";
	$quantity_lower_bound4 = "";
	$quantity_price4 = "";
	$quantity_lower_bound5 = "";
	$quantity_price5 = "";
	$pricing_action = "";
	$unspsc_code = "";
	$national_stock_number = "";

if ($ck_dis_flg == 1) {
	echo "asin_flg<>".$asin_flg."<br>\n";
	echo "<br>\n";
	pre($ALL_SIZE);
	echo "<br>\n";
}
	
	//シャツサイズ
	$shirt_size_system_group="JP";
	$shirt_size_class_group="";
	foreach ($ALL_SIZE as $record){
		if (preg_match("/^\\d+$/",$record['size'] )){
			if (in_array((int)$record['size'],[120,130,140,150,160,170])){
				$shirt_size_class_group="身長";
				break;
			} else {
				$shirt_size_class_group="数学サイズ";
				continue;
			}
		} else {
			if (strpos($record['size'], 'O')!==false){
				$shirt_size_class_group="アルファベット ジャスポ";
				break;
			} else {
				$shirt_size_class_group="アルファベット";
				continue;
			}
		}
	}

	//	各設定 -----------------------------------------------------------------------------------
	foreach ($ALL_SIZE AS $key => $val) {

		if ($asin_flg == 1 && count($ALL_SIZE) > 1 && $key == 0) {
			//	出品ファイルでは親情報はいらない為スキップさせる
			continue;
		}

		//	管理番号設定(必須)
		//	管理番号に枝記号を入れるか入れないか判断する。（サイズが無い場合や、親商品の場合は枝記号は無し）
		$size = $ALL_SIZE[$key]['size'];
		$ext_pro_id = $ALL_SIZE[$key]['jan'];
		$sku_size = "";
		$item_sku = "";
		if ($key == 0 && $size == "") {
			$item_sku = "g".$g_num;
		} else {
			$sku_size = $size;
			$set_sku_size = preg_replace("/[^0-9a-zA-Z.]/", "", $sku_size);
			$item_sku = "g".$g_num."-".$set_sku_size;
		}

		//	登録済み商品の配列削除
		$set_g_num = "g".$g_num;
		if ($key == 0) {
			$set_size = "none";
		} else {
			$set_size = $size;
		}
		unset($AMZ_UP_LIST[$item_sku]);

		//	商品名設定(必須)
		if ($key == 0) {
			$item_name = $g_name;
		} else {
			$item_name = $g_name." ".$size;
		}

		//	商品コード設定(必須)
		$external_product_id = "";
		if (count($ALL_SIZE) == 1) {	//	サイズが無い場合
			if ($ext_pro_id != "") {
				$external_product_id = $ext_pro_id;
			}
		} elseif ($key > 0 && $ext_pro_id != "") {	//	サイズが複数ある場合で子商品の場合
			$external_product_id = $ext_pro_id;
		}

		//	商品コード(JANコード等)設定(必須)
		$external_product_id_type = "";
		$len = 0;
		if ($external_product_id != "") {
			$len = strlen($external_product_id);
			if ($len == 16) {
				$external_product_id_type = "GCID";
			} elseif ($len == 12) {
				$external_product_id_type = "UPC";
			} elseif ($len == 13) {
				$external_product_id_type = "EAN";
			}
		}

		//	アップデート・削除(必須)
		//	Update or PartialUpdate or Delete 通常 Update or Delete
		$update_delete = "Update";

		//	自社で管理しているSKUごとの番号(推奨)
		//	出品者独自のSKUごとの識別コード。商品管理番号（item_sku）と同じでもよい。
		$codabar = $item_sku;

		//	在庫数(推奨)
		$quantity = 0;
		if (count($ALL_SIZE) > 1 && $key == 0 && $size == "") {
			//	親商品の場合は在庫数を入れない
			$quantity = "";
		//} elseif ($att_check == 1) {					//	del ohkawara 2017/06/15
		} elseif ($att_check == 1 || $zaiko == 1) {		//	add ohkawara 2017/06/15
			$quantity = 1;
		} else {
			$quantity = AMAZON_DEFO_ZAIKO;
		}

		//	リードタイム(出荷までにかかる作業日数)
		$fulfillment_latency = "";
		if ($att_check == 1) {
			$fulfillment_latency = AMAZON_MARK_DAYS;
		}
		if (count($ALL_SIZE) > 1 && $key == 0 && $size == "") {
			//	親商品の場合はリードタイムを入れない
			$fulfillment_latency = "";
		}

		//	商品の公開日(任意)
		$product_site_launch_date = "";

		//	予約商品の販売開始日(任意)
		$merchant_release_date = "";

		//	メーカー希望価格(任意)
		$list_price = "";

		//	商品の販売価格(推奨)
		//	親情報の時には金額を設定しない為の設定
		if (count($ALL_SIZE) > 1 && $key == 0) {	//	サイズがある場合で親商品の場合は価格を入れない
			$set_standard_price = "";
		} else {
			$set_standard_price = $standard_price;
		}

		//	ポイント(任意)
		$standard_price_points = "";
		if ($standard_price > 0) {
			$standard_price_points = round($standard_price * (AMAZON_POINT / 100));
		}
		//	ポイント(パーセント)
		$standard_points_percent=1;
		$sale_points_percent=0;


		//	商品の入荷予定日(任意)
		$restock_date = "";

		//	親子関係の指定(任意)	（現在日本では使用不可）
		$parent_child = "child";
		if (count($ALL_SIZE) > 1 && $key == 0) {
			$parent_child = "parent";
		} elseif (count($ALL_SIZE) == 1) {
			$parent_child = "";
		}

		//	親商品のSKU(商品管理番号)(任意)
		$parent_sku = "g".$g_num;
		if ($parent_child != "child") {
			$parent_sku = "";
		}

		//	親子関係のタイプ(任意)
		$relationship_type = "Variation";
		if (count($ALL_SIZE) > 1 && $key == 0) {
			$relationship_type = "";
		}

		//	バリエーションテーマ(任意)
		$variation_theme = "Size";
		if (count($ALL_SIZE) == 1) {
			$variation_theme = "";
		}


		//	サイズ(推奨)
		$size_name = "";
		if ($sku_size) {
			$size_name = $sku_size;
		}

		if ($size_name==""){
			$shirt_size_system="";
			$shirt_size_class="";
		} else{
			$shirt_size_system=$shirt_size_system_group;
			$shirt_size_class=$shirt_size_class_group;
			if ($shirt_size_class_group=="身長"){
				$size_name=	"Height {$size_name}";
			} elseif ($shirt_size_class_group=="アルファベット ジャスポ"){
			$size_name=	"JASPO {$size_name}";
			}
		}

		//	サイズマップ(任意)
		$size_map = "";
		if ($sku_size) {
			$size_map = $sku_size;
		}

		//	ブランド名設定(必須)	小文字→大文字変換
		//	add ohkawara 2017/08/01
		$brand_name = strtoupper($brand_name);

		if ($asin_flg == 1) {
			$set_tsv = "";

			$sku = $item_sku;
			$price = $set_standard_price;
			$product_id = $ALL_SIZE[$key]['asin'];
			$product_id_type = "ASIN";
			$condition_type = $condition_type;
			$operation_type = $update_delete;
			$sale_price = $sale_price;
			//$sale_price_points = $sale_price_points;
			$sale_start_date = $sale_from_date;
			//$sale_end_date = $sale_end_date;
			$leadtime_to_ship = $fulfillment_latency;
			$launch_date = $product_site_launch_date;
			$is_giftwrap_available = $offering_can_be_giftwrapped;
			$is_gift_message_available = $offering_can_be_gift_messaged;
			$optional_payment_type_exclusion = $optional_payment_type_exclusion;
			$main_offer_image = $main_image_url;
			$offer_image1 = $other_image_url1;
			$offer_image2 = $other_image_url2;
			$offer_image3 = $other_image_url3;
			$offer_image4 = $other_image_url4;
			$offer_image5 = $other_image_url5;

			if (!$product_id) {
				continue;
			}

			//	未設定変数
			$condition_note = "";
			$ASIN_hint = "";
			$title = "";
			$fulfillment_center_id = "";

			$set_tsv  = "\"".$sku."\"\t".
						"\"".$price."\"\t".
						"\"".$standard_points_percent."\"\t".
						"\"".$quantity."\"\t".
						"\"".$product_id."\"\t".
						"\"".$product_id_type."\"\t".
						"\"".$condition_type."\"\t".
						"\"".$condition_note."\"\t".
						"\"".$ASIN_hint."\"\t".
						"\"".$title."\"\t".
						"\"".$operation_type."\"\t".
						"\"".$sale_price."\"\t".
						"\"".$sale_points_percent."\"\t".
						"\"".$sale_start_date."\"\t".
						"\"".$sale_end_date."\"\t".
						"\"".$leadtime_to_ship."\"\t".
						"\"".$launch_date."\"\t".
						"\"".$is_giftwrap_available."\"\t".
						"\"".$is_gift_message_available."\"\t".
						"\"".$fulfillment_center_id."\"\t".
						"\"".$optional_payment_type_exclusion."\"\t".
						"\"".$main_offer_image."\"\t".
						"\"".$offer_image1."\"\t".
						"\"".$offer_image2."\"\t".
						"\"".$offer_image3."\"\t".
						"\"".$offer_image4."\"\t".
						"\"".$offer_image5."\"\t".
						"\"".$merchant_shipping_group_name."\"\n";		//	add ohkawara 2017/08/04
			$AMZ_SYU_LIST[] = $set_tsv;
		} elseif ($node_type != 2) {
			//	ファッション

			//	add ohkawara 2019/12/12	マーキングシャツのみなので
			$feed_product_type = "shirt";
			$lifestyle1 = "カジュアル";
			$lifestyle2 = "制服";
			// $department_name = "メンズ";
			$department_name = "男性";






			//	マーキング商品などオリジナル商品になってしまう商品の子商品（サイズ情報有り）は、独自のメーカー型番（g○○○○-○○）を記載する。
			//	add ohkawara 2017/11/16
			if ($parent_child == "child") {
				if ($part_number == "") {
					$part_number = $item_sku;
				}
			}

			$tsv .= "\"".$feed_product_type."\"\t". //	商品タイプ  feed_product_type
			"\"".$item_sku."\"\t". //	出品者SKU  item_sku
			"\"".$brand_name."\"\t". //	ブランド名  brand_name
			"\"".$item_name."\"\t". //	商品名  item_name
			"\"".$external_product_id."\"\t". //	商品コード(JANコード等)  external_product_id
			"\"".$external_product_id_type."\"\t". //	商品コードのタイプ  external_product_id_type
			"\"\"\t". //!!	モデル名  model_name
			"\"\"\t". //!!	模範  model
			"\"".$bullet_point1."\"\t". // 	商品の仕様  bullet_point1
			"\"".$bullet_point2."\"\t". // 	商品の仕様  bullet_point2
			"\"".$bullet_point3."\"\t". // 	商品の仕様  bullet_point3
			"\"".$bullet_point4."\"\t". // 	商品の仕様  bullet_point4
			"\"".$bullet_point5."\"\t". // 	商品の仕様  bullet_point5
			"\"".$department_name."\"\t". //	部門名  department_name
			"\"".$fabric_type."\"\t". //	素材構成  fabric_type
			"\"".$quantity."\"\t". //	在庫数  quantity
			"\"".$standard_price."\"\t". //	商品の販売価格  standard_price
			"\"".$main_image_url."\"\t". //	商品メイン画像URL  main_image_url
			"\"".$target_gender."\"\t". //	性別  target_gender
			"\"".$age_range_description."\"\t". //	対象年齢層  age_range_description
			"\"\"\t". //!!	アパレルサイズ表記単位(国)  apparel_size_system
			"\"\"\t". //!!	アパレルサイズの表記方法  apparel_size_class
			"\"\"\t". //	アパレルサイズの値  apparel_size
			"\"\"\t". //	アパレルサイズ(最大レンジ)  apparel_size_to
			"\"\"\t". //	アパレルサイズのボディタイプ  apparel_body_type
			"\"\"\t". //	アパレルサイズの身長タイプ  apparel_height_type
			"\"".$outer_material_type."\"\t". //	表地素材  outer_material_type
			"\"".$size_name."\"\t". //	サイズ  size_name
			"\"".$color_name."\"\t". //	カラー  color_name
			"\"".$color_map."\"\t". //	カラーマップ  color_map
			"\"".$size_map."\"\t". //	サイズマップ  size_map
			"\"".$is_adult_product."\"\t". //	アダルト商品  is_adult_product
			"\"".$recommended_browse_nodes."\"\t". //	推奨ブラウズノード  recommended_browse_nodes
			"\"\"\t". //!!	ユニット数  unit_count
			"\"\"\t". //!!	ユニット数のタイプ  unit_count_type
			"\"\"\t". //!!	この商品はAmazon.co.jp限定商品ですか？  is_exclusive_product
			"\"\"\t". //!! 	商品に適用される危険物規制の種類  supplier_declared_material_regulation1
			"\"\"\t". //!! 	商品に適用される危険物規制の種類  supplier_declared_material_regulation2
			"\"\"\t". //!! 	商品に適用される危険物規制の種類  supplier_declared_material_regulation3
			"\"".$product_description."\"\t". //	商品説明文  product_description
			"\"".$part_number."\"\t". //	メーカー型番  part_number
			"\"".$closure_type."\"\t". //	closure_type  closure_type
			"\"".$manufacturer."\"\t". //	manufacturer  manufacturer
			"\"".$style_name."\"\t". //	スタイル名  style_name
			"\"\"\t". //!!	種類  item_type_name
			"\"".$item_package_quantity."\"\t". //	パッケージ商品数  item_package_quantity
			"\"\"\t". //!!	care_instructions  care_instructions
			"\"\"\t". //!!	品目の前から後ろまでの奥行き  depth_front_to_back
			"\"\"\t". //!!	品目の奥行きの単位  depth_front_to_back_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅  depth_width_side_to_side
			"\"\"\t". //!!	品目の幅単位  depth_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	底部から上部までの高さ  depth_height_floor_to_top
			"\"\"\t". //!!	品目の高さ単位  depth_height_floor_to_top_unit_of_measure
			"\"\"\t". //!!	付属品総数  number_of_items
			"\"".$footwear_size_system."\"\t". //	靴サイズ表記単位（国）  footwear_size_system
			"\"".$footwear_age_group."\"\t". //	対象年齢層(詳細)  footwear_age_group
			"\"".$footwear_size_class."\"\t". //	靴サイズ表記単位  footwear_size_class
			"\"".$footwear_size."\"\t". //	靴サイズ  footwear_size
			"\"\"\t". //!!	シェイプウェアのサイズ システム  shapewear_size_system
			"\"\"\t". //!!	シェイプウェアのサイズ クラス  shapewear_size_class
			"\"\"\t". //	シェイプウェアのサイズ値  shapewear_size
			"\"\"\t". //	シェイプウェアのサイズ範囲  shapewear_size_to
			"\"\"\t". //	バンドサイズの値  shapewear_band_size
			"\"\"\t". //	バンドサイズ(最大レンジ)  shapewear_band_size_to
			"\"\"\t". //	カップサイズの値  shapewear_cup_size
			"\"\"\t". //	シェイプウェア ボディタイプ  shapewear_body_type
			"\"\"\t". //	シェイプウェアの身長タイプ  shapewear_height_type
			"\"\"\t". //!!	ボトムスサイズシステム  bottoms_size_system
			"\"\"\t". //!!	ボトムスサイズクラス  bottoms_size_class
			"\"\"\t". //	ボトムスサイズ値  bottoms_size
			"\"\"\t". //	ボトムスのサイズ範囲  bottoms_size_to
			"\"\"\t". //	ウエストサイズ値  bottoms_waist_size
			"\"\"\t". //	股下サイズ値  bottoms_inseam_size
			"\"\"\t". //	ボトムス体型タイプ  bottoms_body_type
			"\"\"\t". //	ボトムス身長タイプ  bottoms_height_type
			"\"\"\t". //!!	スカートサイズシステム  skirt_size_system
			"\"\"\t". //!!	スカートサイズクラス  skirt_size_class
			"\"\"\t". //	スカートサイズ値  skirt_size
			"\"\"\t". //	スカートのサイズ範囲  skirt_size_to
			"\"\"\t". //	ウエストサイズ値  skirt_waist_size
			"\"\"\t". //	スカート体型タイプ  skirt_body_type
			"\"\"\t". //	スカート身長タイプ  skirt_height_type
			"\"".$shirt_size_system."\"\t". //	シャツサイズシステム  shirt_size_system
			"\"".$shirt_size_class."\"\t". //	シャツサイズクラス  shirt_size_class
			"\"".$size_name."\"\t". //	シャツサイズ値  shirt_size
			"\"\"\t". //	シャツのサイズ範囲  shirt_size_to
			"\"\"\t". //	首回りサイズ値  shirt_neck_size
			"\"\"\t". //	裄丈サイズ値  shirt_sleeve_length
			"\"\"\t". //	シャツ体型タイプ  shirt_body_type
			"\"\"\t". //	シャツ身長タイプ  shirt_height_type
			"\"\"\t". //!!	水平方向の長端の長さ  length_head_to_toe
			"\"\"\t". //!!	長さ単位  length_head_to_toe_unit_of_measure
			"\"\"\t". //!!	水平方向の短い方の端の幅  length_width_side_to_side
			"\"\"\t". //!!	幅単位  length_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	底部から上部までの高さ  length_height_floor_to_top
			"\"\"\t". //!!	高さの単位  length_height_floor_to_top_unit_of_measure
			"\"".$special_features1."\"\t". // 	機能性  special_features1
			"\"".$special_features2."\"\t". // 	機能性  special_features2
			"\"".$special_features3."\"\t". // 	機能性  special_features3
			"\"".$special_features4."\"\t". // 	機能性  special_features4
			"\"".$special_features5."\"\t". // 	機能性  special_features5
			"\"\"\t". // 	機能性  special_features6
			"\"\"\t". // 	機能性  special_features7
			"\"\"\t". // 	機能性  special_features8
			"\"\"\t". // 	機能性  special_features9
			"\"\"\t". // 	機能性  special_features10
			"\"\"\t". // 	機能性  special_features11
			"\"\"\t". // 	機能性  special_features12
			"\"\"\t". // 	機能性  special_features13
			"\"\"\t". // 	機能性  special_features14
			"\"".$band_size_num."\"\t". //	バンドサイズ  band_size_num
			"\"".$band_size_num_unit_of_measure."\"\t". //	バンドサイズの単位  band_size_num_unit_of_measure
			"\"".$lifestyle1."\"\t". // 	ライフスタイル  lifestyle1
			"\"".$lifestyle2."\"\t". //     ライフスタイル  lifestyle2
			"\"\"\t". //	ライフスタイル  lifestyle3
			"\"\"\t". //	ライフスタイル  lifestyle4
			"\"\"\t". //	ライフスタイル  lifestyle5
			"\"\"\t". //	ライフスタイル  lifestyle6
			"\"\"\t". //	ライフスタイル  lifestyle7
			"\"\"\t". //	ライフスタイル  lifestyle8
			"\"\"\t". //	ライフスタイル  lifestyle9
			"\"\"\t". //	ライフスタイル  lifestyle10
			"\"".$neck_style."\"\t". //	首のタイプ  neck_style
			"\"".$item_length_description."\"\t". //	長さ  item_length_description
			"\"".$material_composition1."\"\t". //?	素材構成  material_composition
			"\"\"\t". //!!	帽子サイズ表記単位(国)   headwear_size_system
			"\"\"\t". //!!	帽子サイズの表記方法  headwear_size_class
			"\"\"\t". //	帽子サイズの値  headwear_size
			"\"\"\t". //	帽子のサイズ範囲  headwear_size_to
			//  画像 - 商品画像を表示させるために必要な項目。詳しくは画像説明タブを参照
			"\"".$other_image_url1."\"\t". // 	商品のサブ画像URL  other_image_url1
			"\"".$other_image_url2."\"\t". // 	商品のサブ画像URL  other_image_url2
			"\"".$other_image_url3."\"\t". // 	商品のサブ画像URL  other_image_url3
			"\"".$other_image_url4."\"\t". // 	商品のサブ画像URL  other_image_url4
			"\"".$other_image_url5."\"\t". // 	商品のサブ画像URL  other_image_url5
			"\"".$other_image_url6."\"\t". // 	商品のサブ画像URL  other_image_url6
			"\"".$other_image_url7."\"\t". // 	商品のサブ画像URL  other_image_url7
			"\"".$other_image_url8."\"\t". // 	商品のサブ画像URL  other_image_url8
			"\"".$swatch_image_url."\"\t". //	カラーサンプル画像URL  swatch_image_url
			//   バリエーション - 商品の色・サイズなどのバリエーションを作成する際に必須の項目   
			"\"".$parent_child."\"\t". //	親子関係の指定  parent_child
			"\"".$parent_sku."\"\t". //	親商品のSKU(商品管理番号)  parent_sku
			"\"".$relationship_type."\"\t". //	子関係タイプ  relationship_type
			"\"".$variation_theme."\"\t". //	バリエーション種類  variation_theme
			"\"\"\t". //	パッケージレベル  package_level
			"\"\"\t". //	パッケージ内包数量  package_contains_quantity
			"\"\"\t". //	パッケージには属性が含まれています  package_contains_identifier
			 //      商品基本情報 - すべての商品に必須の項目
			"\"".$update_delete."\"\t". //	アップデート・削除  update_delete
			"\"".$codabar."\"\t". //	自社で管理しているSKUごとの番号  codabar
			"\"".$heel_type."\"\t". //	ヒールのタイプ  heel_type
			"\"".$model_year."\"\t". //	モデル年(発売年・発表年)  model_year
			"\"".$inner_material_type1."\"\t". //	インナーの素材  inner_material_type
			"\"\"\t". //	バーコード適用除外理由  gtin_exemption_reason
			"\"\"\t". //	版  edition
			"\"".$sole_material."\"\t". //	ソール素材  sole_material
			"\"".$heel_height_unit_of_measure."\"\t". //	ヒールの高さの単位  heel_height_unit_of_measure
			"\"".$heel_height."\"\t". //	ヒールの高さ  heel_height
			"\"\"\t". //	footwear_gender  footwear_gender
			"\"\"\t". //	footwear_width_unisex  footwear_width_unisex
			"\"\"\t". //	footwear_to_size_unisex  footwear_to_size_unisex
			"\"\"\t". //	footwear_gender_unisex  footwear_gender_unisex
			"\"\"\t". //	footwear_size_unisex  footwear_size_unisex
			"\"\"\t". //	ヒール高  heel_height_string
			"\"".$footwear_width."\"\t". //	靴幅  footwear_width
			"\"".$footwear_to_size."\"\t". //	靴サイズの上限値  footwear_to_size
			 //      商品検索情報 - サーチ上で商品を検索されやすくするために必要な項目
			"\"".$generic_keywords."\"\t". //	検索キーワード  generic_keywords
			"\"".$is_autographed."\"\t". //	サイン入り商品  is_autographed
			"\"\"\t". // 	行事  occasion_type1
			"\"\"\t". // 	行事  occasion_type2
			"\"\"\t". // 	行事  occasion_type3
			"\"\"\t". // 	行事  occasion_type4
			"\"\"\t". // 	行事  occasion_type5
			"\"\"\t". // 	スポーツタイプ  sport_type1
			"\"\"\t". // 	スポーツタイプ  sport_type2
			"\"\"\t". // 	スポーツタイプ  sport_type3
			"\"\"\t". //	プレーヤー名  athlete
			"\"\"\t". //	チーム名  team_name
			"\"\"\t". //	コレクション名  collection_name
			"\"".$material_type."\"\t". //	素材の種類  material_type
			"\"".$target_audience_keywords1."\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords1
			"\"".$target_audience_keywords2."\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords2
			"\"\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords3
			"\"\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords4
			"\"\"\t". // 	ターゲットユーザーのキーワード  target_audience_keywords5
			"\"\"\t". //	毛皮の説明  fur_description
			"\"".$fit_type."\"\t". // 	フィットタイプ  fit_type1
			"\"\"\t". // 	フィットタイプ  fit_type2
			"\"\"\t". // 	フィットタイプ  fit_type3
			"\"\"\t". // 	フィットタイプ  fit_type4
			"\"\"\t". // 	フィットタイプ  fit_type5
			"\"".$collar_style."\"\t". //	襟のタイプ  collar_style
			"\"\"\t". //	織り方  weave_type
			"\"\"\t". //	ポケット記述  pocket_description
			"\"\"\t". //	テーマ  theme
			"\"\"\t". //	ライニングの説明  lining_description
			"\"\"\t". //	リーグ名  league_name
			"\"\"\t". // 	特殊サイズタイプ  special_size_type1
			"\"\"\t". // 	特殊サイズタイプ  special_size_type2
			"\"\"\t". // 	特殊サイズタイプ  special_size_type3
			"\"\"\t". // 	特殊サイズタイプ  special_size_type4
			"\"\"\t". // 	特殊サイズタイプ  special_size_type5
			"\"\"\t". // 	特殊サイズタイプ  special_size_type6
			"\"\"\t". // 	特殊サイズタイプ  special_size_type7
			"\"\"\t". // 	特殊サイズタイプ  special_size_type8
			"\"\"\t". // 	特殊サイズタイプ  special_size_type9
			"\"\"\t". // 	特殊サイズタイプ  special_size_type10
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type1
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type2
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type3
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type4
			"\"\"\t". // 	商品の補充情報  lifecycle_supply_type5
			"\"".$pattern_name."\"\t". //	パターン名  pattern_name
			"\"\"\t". //	オーダー締切日  item_booking_date
			"\"".$style_keywords."\"\t". // 	スタイルキーワード  style_keywords1
			"\"\"\t". // 	スタイルキーワード  style_keywords2
			"\"\"\t". // 	スタイルキーワード  style_keywords3
			"\"".$seasons."\"\t". //	シーズン  seasons
			"\"".$platinum_keywords1."\"\t". // 	プラチナキーワード  platinum_keywords1
			"\"".$platinum_keywords2."\"\t". // 	プラチナキーワード  platinum_keywords2
			"\"".$platinum_keywords3."\"\t". // 	プラチナキーワード  platinum_keywords3
			"\"".$platinum_keywords4."\"\t". // 	プラチナキーワード  platinum_keywords4
			"\"".$platinum_keywords5."\"\t". // 	プラチナキーワード  platinum_keywords5
			"\"".$specific_uses_keywords1."\"\t". // 	特定用途キーワード  specific_uses_keywords1
			"\"".$specific_uses_keywords2."\"\t". // 	特定用途キーワード  specific_uses_keywords2
			"\"".$specific_uses_keywords3."\"\t". // 	特定用途キーワード  specific_uses_keywords3
			"\"".$specific_uses_keywords4."\"\t". // 	特定用途キーワード  specific_uses_keywords4
			"\"".$specific_uses_keywords5."\"\t". // 	特定用途キーワード  specific_uses_keywords5
			"\"".$shaft_style_type."\"\t". //	シャフトスタイルタイプ  shaft_style_type
			"\"\"\t". //	UV加工  ultraviolet_light_protection
			"\"\"\t". //	要組み立て  is_assembly_required
			"\"\"\t". //	レンズのコーティング  lens_coating_description
			"\"\"\t". //	レンズ色  lens_color
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type1
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type2
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type3
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type4
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type5
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type6
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type7
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type8
			"\"\"\t". // 	適合車両の種類  compatible_with_vehicle_type9
			"\"\"\t". //	箱数  number_of_boxes
			"\"\"\t". // 	同梱商品  included_components1
			"\"\"\t". // 	同梱商品  included_components2
			"\"\"\t". // 	同梱商品  included_components3
			"\"\"\t". // 	同梱商品  included_components4
			"\"\"\t". // 	同梱商品  included_components5
			"\"\"\t". //	商品の個数（ピース数）  number_of_pieces
			"\"\"\t". //	耐水レベル  water_resistance_level
			"\"\"\t". //	シャーシ素材  frame_material_type
			"\"\"\t". //	バッグストラップの高さ  shoulder_strap_drop
			"\"\"\t". //	ショルダーストラップドロップの測定単位  shoulder_strap_drop_unit_of_measure
			"\"\"\t". //	防汚  is_stain_resistant
			"\"\"\t". //	ラベルに記載されている国/地域  country_as_labeled
			"\"\"\t". //	パターンのスタイル  pattern_type
			"\"\"\t". //	出品者カタログ番号  catalog_number
			"\"\"\t". //	メーカー対象最高年齢  mfg_maximum_unit_of_measure
			"\"\"\t". //	組み立て時間の単位  assembly_time_unit_of_measure
			"\"\"\t". //	assembly_time  assembly_time
			"\"\"\t". //	thesaurus_attribute_keywords1
			"\"\"\t". //	thesaurus_attribute_keywords2
			"\"\"\t". //	thesaurus_attribute_keywords3
			"\"\"\t". //	thesaurus_attribute_keywords4
			"\"\"\t". //	thesaurus_attribute_keywords5
			"\"\"\t". //	top_style  top_style
			"\"\"\t". // 	thesaurus_subject_keywords1
			"\"\"\t". // 	thesaurus_subject_keywords2
			"\"\"\t". // 	thesaurus_subject_keywords3
			"\"\"\t". // 	thesaurus_subject_keywords4
			"\"\"\t". // 	thesaurus_subject_keywords5
			"\"\"\t". //	メーカー対象最低年齢  mfg_minimum_unit_of_measure
			"\"\"\t". //	推奨使用用途  recommended_uses_for_product
			"\"\"\t". //	キャスター数  number_of_wheels
			"\"\"\t". //	正味個数単位  net_content_count_unit_of_measure
			"\"\"\t". //    防カビ加工  material_type_free1
			"\"\"\t". //    防カビ加工  material_type_free2
			"\"\"\t". //    防カビ加工  material_type_free3
			"\"\"\t". //    防カビ加工  material_type_free4
			"\"\"\t". //    防カビ加工  material_type_free5
			"\"\"\t". //	正味個数  net_content_count
			"\"\"\t". //	利き手  hand_orientation
			"\"\"\t". //	股上のスタイル  rise_style
			"\"\"\t". //	フロントスタイル  front_style
			"\"\"\t". //	ノイズ制御  noise_control
			"\"\"\t". //	ノイズ減衰単位  noise_attenuation_unit_of_measure
			"\"\"\t". //	ノイズ減衰値  noise_attenuation
			"\"\"\t". //	クラスタイプ  clasp_type
			"\"\"\t". // 	装飾機能  embellishment_feature1
			"\"\"\t". // 	装飾機能  embellishment_feature2
			"\"\"\t". // 	装飾機能  embellishment_feature3
			"\"\"\t". // 	装飾機能  embellishment_feature4
			"\"\"\t". // 	装飾機能  embellishment_feature5
			"\"\"\t". //	背面のスタイル  back_style
			"\"\"\t". //	ハンドル素材  handle_material
			"\"\"\t". //	シャフトの円周  shaft_circumference
			"\"\"\t". //	シャフトの円周の単位  shaft_circumference_unit_of_measure
			"\"\"\t". //	つま先のスタイル  toe_style
			"\"\"\t". //	ベルトの種類  belt_style
			"\"\"\t". //	抗菌加工  item_styling
			"\"\"\t". //	商品の利点  product_benefit
			"\"\"\t". //	ワット数の単位  wattage_unit_of_measure
			"\"\"\t". //	グリップタイプ  grip_type
			"\"\"\t". //	ワット数  wattage
			"\"\"\t". //	土踏まずの特徴  arch_type
			//      推奨ブラウズノード別の情報 - これらの項目は、より豊富な商品情報をカスタマーに提供するためのものです。選択した推奨ブラウズノードにより、必要となる項目が異なります。
			"\"\"\t". //	キャラクター  subject_character
			"\"".$sleeve_type."\"\t". //	袖のタイプ  sleeve_type
			"\"\"\t". //	ファブリックウォッシュ  fabric_wash
			"\"".$strap_type."\"\t". //	ストラップのタイプ  strap_type
			"\"".$toe_shape."\"\t". //	つま先の形状(トゥシェープ)  toe_shape
			"\"".$waist_style."\"\t". //	ウエストスタイル  waist_style
			"\"".$opacity."\"\t". //	不透明度  opacity
			"\"".$bottom_style."\"\t". //	ボトムススタイル  bottom_style
			"\"".$fit_to_size_description."\"\t". //	収納可能サイズ  fit_to_size_description
			"\"\"\t". //	サブブランド  sub_brand_name
			"\"\"\t". //	内蔵ライト  has_builtin_light
			"\"\"\t". //	表示サイズ  size_info_display_name
			"\"\"\t". //	underwire_type  underwire_type
			"\"\"\t". //	opacity_unit_of_measure  opacity_unit_of_measure
			"\"\"\t". //	leg_style  leg_style
			"\"\"\t". //	mfg_maximum  mfg_maximum
			"\"\"\t". //	プレーヤー数  number_of_players
			"\"\"\t". //	mfg_minimum  mfg_minimum
			"\"\"\t". //	ポケットの数  number_of_pockets
			"\"\"\t". //	正味容積単位  net_content_volume_unit_of_measure
			"\"\"\t". //	正味重量単位  net_content_weight_unit_of_measure
			"\"\"\t". //	正味の長さ  net_content_length
			"\"\"\t". //	正味の長さの単位  net_content_length_unit_of_measure
			"\"\"\t". //	正味面積  net_content_area
			"\"\"\t". //	正味重量  net_content_weight
			"\"\"\t". //	正味面積単位  net_content_area_unit_of_measure
			"\"\"\t". //	正味容積  net_content_volume
			"\"\"\t". //	ジャケットの長さ  jacket_length
			"\"\"\t". //	バンドサイズ  band_size
			"\"\"\t". //	ベルト幅の単位  belt_width_unit_of_measure
			"\"\"\t". //	ベルト幅  belt_width_string
			"\"\"\t". //	最低広告表示価格  map_policy
			"\"\"\t". //	ベルト幅の十進数値  belt_width_derived
			"\"\"\t". //	bra_padding_level  bra_padding_level
			"\"\"\t". //	梱包タイプ  customer_package_type
			"\"\"\t". //	つや消しスタイル  matte_style
			//      寸法 - 商品のサイズや重量を入力する項目
			"\"\"\t". //	商品の幅の単位  item_width_unit_of_measure
			"\"".$chest_size_unit_of_measure."\"\t". //	チェストサイズの単位  chest_size_unit_of_measure
			"\"".$chest_size."\"\t". //	チェストサイズ  chest_size
			"\"\"\t". //	商品の高さの単位  item_height_unit_of_measure
			"\"\"\t". //	えきたいりょう  liquid_volume
			"\"\"\t". //	液量単位  liquid_volume_unit_of_measure
			"\"\"\t". //	商品の長さの単位  item_length_unit_of_measure
			"\"".$shoe_width."\"\t". //	靴の幅  shoe_width
			"\"".$shaft_height."\"\t". //	シャフト(軸)の丈  shaft_height
			"\"".$waist_size_unit_of_measure."\"\t". //	ウエストサイズの単位  waist_size_unit_of_measure
			"\"".$waist_size."\"\t". //	ウエストのサイズ  waist_size
			"\"".$inseam_length_unit_of_measure."\"\t". //	仕立ての長さの単位  inseam_length_unit_of_measure
			"\"".$inseam_length."\"\t". //	股下の長さ  inseam_length
			"\"".$sleeve_length_unit_of_measure."\"\t". //	袖の長さの単位  sleeve_length_unit_of_measure
			"\"".$sleeve_length."\"\t". //	袖の長さ  sleeve_length
			"\"".$neck_size_unit_of_measure."\"\t". //	首のサイズの単位  neck_size_unit_of_measure
			"\"".$neck_size."\"\t". //	首のサイズ  neck_size
			"\"".$website_shipping_weight_unit_of_measure."\"\t". //	配送重量の単位  website_shipping_weight_unit_of_measure
			"\"".$website_shipping_weight."\"\t". //	配送重量  website_shipping_weight
			"\"".$shaft_height_unit_of_measure."\"\t". //	シャフトの高さの測定単位  shaft_height_unit_of_measure
			"\"".$item_width."\"\t". //	項目幅  item_width
			"\"".$item_height."\"\t". //	項目の高さ  item_height
			"\"".$shoe_width_unit_of_measure."\"\t". //	靴幅  shoe_width_unit_of_measure
			"\"".$item_length."\"\t". //	品目の長さ  item_length
			"\"\"\t". //	商品の重量（パッケージを含まない）  item_display_weight
			"\"\"\t". //	商品の重量の単位  item_display_weight_unit_of_measure
			"\"".$item_dimensions_unit_of_measure."\"\t". //	商品の寸法の単位  item_dimensions_unit_of_measure
			"\"".$cup_size."\"\t". //	カップサイズ  cup_size
			"\"\"\t". //	長さ  item_display_length
			"\"\"\t". //	商品の長さの単位  item_display_length_unit_of_measure
			"\"\"\t". //	幅  item_display_width
			"\"\"\t". //	商品の高さの単位  item_display_height_unit_of_measure
			"\"\"\t". //	商品の幅の単位  item_display_width_unit_of_measure
			"\"\"\t". //	容量の単位  capacity_unit_of_measure
			"\"\"\t". //	商品の高さ  item_display_height
			"\"\"\t". //	ディスプレイサイズ  display_size
			"\"\"\t". //	ディスプレイ最大解像度の単位  display_size_unit_of_measure
			"\"\"\t". //	性能・容量  capacity
			"\"\"\t". //	leg_diameter_unit_of_measure  leg_diameter_unit_of_measure
			"\"\"\t". //	推奨最大重量の単位  maximum_weight_recommendation_unit_of_measure
			"\"\"\t". //	メーカー推奨最大重量  maximum_weight_recommendation
			"\"\"\t". //	leg_diameter  leg_diameter
			"\"\"\t". //	商品の形状  item_shape
			"\"\"\t". //	ストレージ容量の測定単位  storage_volume_unit_of_measure
			"\"\"\t". //	ストレージボリューム  storage_volume
			"\"\"\t". //	商品の形状  item_form
			"\"\"\t". //	高さマップ  height_map
			"\"\"\t". //	股上の高さ  rise_height
			"\"\"\t". //	股上の高さの単位  rise_height_unit_of_measure
			"\"\"\t". //	伸びた長さの測定単位  extended_length_unit_of_measure
			"\"\"\t". //	拡張された長さ  extended_length
			"\"\"\t". //	minimum_neck_size_unit_of_measure  minimum_neck_size_unit_of_measure
			"\"\"\t". //	ヘッドサイズ名  head_size_name
			//      出荷関連情報 - フルフィルメント by Amazon (FBA) の利用、あるいは自社出荷の注文に関する出荷関連情報を、この項目に記入してください。FBA を利用する場合には必須の項目。
			"\"\"\t". //	商品パッケージの長さの単位  package_length_unit_of_measure
			"\"\"\t". //	長さ  package_length
			"\"\"\t". //	幅  package_width
			"\"\"\t". //	高さ  package_height
			"\"\"\t". //	商品パッケージの重量の単位  package_weight_unit_of_measure
			"\"\"\t". //	商品パッケージの重量  package_weight
			"\"\"\t". //	フルフィルメントセンターID  fulfillment_center_id
			"\"\"\t". //	商品パッケージの高さの単位  package_height_unit_of_measure
			"\"\"\t". //	商品パッケージの幅の単位  package_width_unit_of_measure
			"\"\"\t". //	商品パッケージの寸法の単位  package_dimensions_unit_of_measure
			//      コンプライアンス情報 - 商品を販売する国または地域の特定商取引に遵守するために利用される項目
			"\"".$batteries_required."\"\t". //	この商品は電池本体ですか？または電池を使用した商品ですか？  batteries_required
			"\"".$are_batteries_included."\"\t". //	電池付属  are_batteries_included
			"\"".$battery_cell_composition."\"\t". //	電池の組成  battery_cell_composition
			"\"".$battery_type1."\"\t". // 	電池の種類、サイズ  battery_type1
			"\"".$battery_type2."\"\t". // 	電池の種類、サイズ  battery_type2
			"\"".$battery_type3."\"\t". // 	電池の種類、サイズ  battery_type3
			"\"".$number_of_batteries1."\"\t". // 	電池の数  number_of_batteries1
			"\"".$number_of_batteries2."\"\t". // 	電池の数  number_of_batteries2
			"\"".$number_of_batteries3."\"\t". // 	電池の数  number_of_batteries3
			"\"\"\t". //!!	電池の重量(グラム)  battery_weight
			"\"\"\t". //!!	電池の重量の測定単位  battery_weight_unit_of_measure
			"\"".$number_of_lithium_metal_cells."\"\t". //	リチウムメタル電池単数  number_of_lithium_metal_cells
			"\"".$number_of_lithium_ion_cells."\"\t". //	リチウムイオン電池単数  number_of_lithium_ion_cells
			"\"".$lithium_battery_packaging."\"\t". //	リチウム電池パッケージ  lithium_battery_packaging
			"\"".$lithium_battery_energy_content."\"\t". //	電池当たりのワット時  lithium_battery_energy_content
			"\"".$lithium_battery_energy_content_unit_of_measure."\"\t". //	リチウム電池のエネルギー量測定単位  lithium_battery_energy_content_unit_of_measure
			"\"".$lithium_battery_weight."\"\t". //	リチウム含有量(グラム)  lithium_battery_weight
			"\"".$lithium_battery_weight_unit_of_measure."\"\t". //	リチウム電池の重量の測定単位  lithium_battery_weight_unit_of_measure
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation1
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation2
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation3
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation4
			"\"\"\t". //!! 	適用される危険物関連法令  supplier_declared_dg_hz_regulation5
			"\"\"\t". //!!	国連(UN)番号  hazmat_united_nations_regulatory_id
			"\"\"\t". //!!	安全データシート(SDS) URL  safety_data_sheet_url
			"\"".$item_weight."\"\t". //	商品の重量  item_weight
			"\"".$item_weight_unit_of_measure."\"\t". //	商品の重量の単位  item_weight_unit_of_measure
			"\"".$item_volume."\"\t". //	商品の体積  item_volume
			"\"".$item_volume_unit_of_measure."\"\t". //	商品の容量の単位  item_volume_unit_of_measure
			"\"".$country_of_origin."\"\t". //	原産国/地域  country_of_origin
			"\"\"\t". //	商品に適用される保管規制の区分  fulfillment_center_special_storage
			"\"\"\t". //	引火点(°C)  flash_point
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択)  ghs_classification_class1
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択)  ghs_classification_class2
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択)  ghs_classification_class3
			"\"\"\t". //	内用液は含まれていますか?  contains_liquid_contents
			"\"\"\t". //	メーカー保証の説明  warranty_description
			"\"\"\t". //	CPSIA警告の説明  cpsia_cautionary_description
			"\"\"\t". //	並行輸入品  import_designation
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement1
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement2
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement3
			"\"\"\t". // 	CPSIA警告  cpsia_cautionary_statement4
			"\"\"\t". //	法規上の免責条項  legal_disclaimer_description
			"\"\"\t". //	安全上のお知らせ  safety_warning
			"\"\"\t". //	external_testing_certification  external_testing_certification
			//       出品情報 - 商品をサイト上で販売可能にする際に必要な項目
			"\"".$fulfillment_latency."\"\t". //	出荷作業日数  fulfillment_latency
			"\"".$condition_type."\"\t". //	コンディション  condition_type
			"\"\"\t". //	取り寄せ  is_sourcing_on_demand
			"\"".$condition_note."\"\t". //	商品のコンディション説明  condition_note
			"\"\"\t". //	通貨コード  currency
			"\"".$merchant_release_date."\"\t". //	予約商品の販売開始日  merchant_release_date
			"\"".$list_price."\"\t". //	メーカー希望価格  list_price
			"\"".$optional_payment_type_exclusion."\"\t". //	使用しない支払い方法  optional_payment_type_exclusion
			"\"".$sale_price."\"\t". //	セール価格  sale_price
			"\"".$sale_from_date."\"\t". //	セール開始日  sale_from_date
			"\"".$sale_end_date."\"\t". //	セール終了日  sale_end_date
			"\"".$restock_date."\"\t". //	商品の入荷予定日  restock_date
			"\"".$max_order_quantity."\"\t". //	最大注文個数  max_order_quantity
			"\"".$offering_can_be_gift_messaged."\"\t". //	ギフトメッセージ  offering_can_be_gift_messaged
			"\"".$offering_can_be_giftwrapped."\"\t". //	ギフト包装  offering_can_be_giftwrapped
			"\"".$merchant_shipping_group_name."\"\t". //	配送パターン  merchant_shipping_group_name
			"\"\"\t". //	地図価格  map_price
			"\"".$distribution_designation."\"\t". //	販売形態(並行輸入品)  distribution_designation
			"\"".$offering_end_date."\"\t". //	終了日を提案する  offering_end_date
			"\"".$offering_start_date."\"\t". //	開始日を提案する  offering_start_date
			"\"".$product_tax_code."\"\t". //	商品タックスコード  product_tax_code
			"\"".$standard_points_percent."\"\t". //	ポイントパーセント  standard_price_points_percent
			"\"".$sale_points_percent."\"\t". //	セール時ポイントパーセント  sale_price_points_percent
			"\"".$product_site_launch_date."\"\t". //	商品の公開日  product_site_launch_date
			"\"".$delivery_schedule_group_id."\"\t". //	配送日時指定SKUリスト  delivery_schedule_group_id
			"\"".$is_discontinued_by_manufacturer."\"\t". //	メーカー製造中止  is_discontinued_by_manufacturer
			"\"\"\t". //	同梱発送時の最大個数  max_aggregate_ship_quantity
			"\"\"\t". //	メーカー希望価格  list_price_with_tax
			"\"\"\t". //	ポイント  standard_price_points
			"\"\"\t". //	セール時ポイント  sale_price_points
			"\n";
		} else {

			//	add ohkawara 2019/12/13
			//	商品タイプ(必須)
			$feed_product_type = "sportinggoods";


			//	スポーツ
			$tsv.="\"".$feed_product_type."\"\t". //	商品タイプ feed_product_type
			"\"".$item_sku."\"\t". //	出品者SKU item_sku
			"\"".$external_product_id."\"\t". //	商品コード(JANコード等) external_product_id
			"\"".$external_product_id_type."\"\t". //	商品コードのタイプ external_product_id_type
			"\"".$item_name."\"\t". //	商品名 item_name
			"\"".$department_name."\"\t". //	対象年齢・性別 department_name
			"\"".$standard_price."\"\t". //	商品の販売価格 standard_price
			"\"".$quantity."\"\t". //	在庫数 quantity
			"\"".$main_image_url."\"\t". //	商品メイン画像URL main_image_url
			"\"".$brand_name."\"\t". //	ブランド名 brand_name
			"\"".$manufacturer."\"\t". //	メーカー名 manufacturer
			"\"".$recommended_browse_nodes."\"\t". //	推奨ブラウズノード recommended_browse_nodes
			"\"\"\t". //!!	前から後ろまでのアイテムの奥行き depth_front_to_back
			"\"\"\t". //!!	品目の奥行きの単位 depth_front_to_back_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 depth_width_side_to_side
			"\"\"\t". //!!	品目の幅単位 depth_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	品目の底部から上部までの高さ depth_height_floor_to_top
			"\"\"\t". //!!	品目の高さ単位 depth_height_floor_to_top_unit_of_measure
			"\"\"\t". //	お客様の性別 target_gender
			"\"\"\t". //!!	unit_count unit_count
			"\"\"\t". //!!	unit_count_type unit_count_type
			"\"\"\t". //	お客様の年齢層 age_range_description
			"\"".$size_name."\"\t". //	サイズ size_name
			"\"\"\t". //!!	品目の上端から下端までの長さ length_longer_edge
			"\"\"\t". //!!	品目の長さ単位 length_longer_edge_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 width_shorter_edge
			"\"\"\t". //!!	幅単位 width_shorter_edge_unit_of_measure
			"\"".$color_name."\"\t". //	カラー color_name
			"\"\"\t". //!!	賞味期限管理商品 is_expiration_dated_product
			"\"\"\t". //!!	品目の上端から下端までの長さ thickness_head_to_toe
			"\"\"\t". //!!	品目の長さ単位 thickness_head_to_toe_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 thickness_width_side_to_side
			"\"\"\t". //!!	品目の幅単位 thickness_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	品目の厚さ thickness_floor_to_top
			"\"\"\t". //!!	品目の厚さ単位 thickness_floor_to_top_unit_of_measure
			"\"\"\t". //!!	組み立て時の長辺の長さ length_head_to_toe
			"\"\"\t". //!!	長さ単位 length_head_to_toe_unit_of_measure
			"\"\"\t". //!!	組み立て時の短辺の幅 length_width_side_to_side
			"\"\"\t". //!!	幅単位 length_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	組み立て時のベースからトップまでの高さ length_height_floor_to_top
			"\"\"\t". //!!	高さの単位 length_height_floor_to_top_unit_of_measure
			"\"\"\t". //!!	商品の直径 item_diameter_derived
			"\"\"\t". //!!	商品の直径の単位 item_diameter_unit_of_measure
			"\"\"\t". //!!	item_width_numeric item_width_numeric
			"\"\"\t". //!!	item_width_numeric_unit_of_measure item_width_numeric_unit_of_measure
			"\"\"\t". //!!	品目の左右の幅 height_width_side_to_side
			"\"\"\t". //!!	品目の幅単位 height_width_side_to_side_unit_of_measure
			"\"\"\t". //!!	底部から上部までの高さ height_floor_top
			"\"\"\t". //!!	品目の高さ単位 height_floor_top_unit_of_measure
			"\"".$other_image_url1."\"\t". //	商品のサブ画像URL1 other_image_url1
			"\"".$other_image_url2."\"\t". //	商品のサブ画像URL1 other_image_url2
			"\"".$other_image_url3."\"\t". //	商品のサブ画像URL1 other_image_url3
			"\"".$other_image_url4."\"\t". //	商品のサブ画像URL1 other_image_url4
			"\"".$other_image_url5."\"\t". //	商品のサブ画像URL1 other_image_url5
			"\"".$other_image_url6."\"\t". //	商品のサブ画像URL1 other_image_url6
			"\"".$other_image_url7."\"\t". //	商品のサブ画像URL1 other_image_url7
			"\"".$other_image_url8."\"\t". //	商品のサブ画像URL1 other_image_url8
			"\"".$swatch_image_url."\"\t". //	カラーサンプル画像URL swatch_image_url
			"\"".$parent_child."\"\t". //	親子関係の指定 parent_child
			"\"".$parent_sku."\"\t". //	親商品のSKU(商品管理番号) parent_sku
			"\"".$relationship_type."\"\t". //	子関係タイプ relationship_type
			"\"".$variation_theme."\"\t". //	バリエーションテーマ variation_theme
			"\"\"\t". //	パッケージレベル package_level
			"\"\"\t". //	パッケージ内包数量 package_contains_quantity
			"\"\"\t". //	パッケージには属性が含まれています package_contains_identifier
			"\"\"\t". //	アップデート・削除 update_delete
			"\"".$product_description."\"\t". //	商品説明文 product_description
			"\"".$part_number."\"\t". //	メーカー型番 part_number
			"\"\"\t". //	リールの型式名 model_name
			"\"\"\t". //	モデル model
			"\"\"\t". //	バーコード適用除外理由 gtin_exemption_reason
			"\"\"\t". //	自社で管理しているSKUごとの番号 codabar
			"\"\"\t". //	発売年・モデルイヤー model_year
			"\"\"\t". //	梱包タイプ closure_type
			"\"\"\t". //	表地素材 outer_material_type
			"\"\"\t". //	インナーの素材 inner_material_type
			"\"\"\t". //	使用する燃料タイプ fuel_type
			"\"\"\t". //	ヒールのタイプ heel_type
			"\"\"\t". //	ヒールの高さの単位 heel_height_unit_of_measure
			"\"\"\t". //	ソール材料 sole_material
			"\"\"\t". //	ヒールの高さ heel_height
			"\"\"\t". //	温度の定格 temperature_rating
			"\"\"\t". //	お手入れ方法 care_instructions1
			"\"\"\t". //	お手入れ方法 care_instructions2
			"\"\"\t". //	お手入れ方法 care_instructions3
			"\"\"\t". //	お手入れ方法 care_instructions4
			"\"".$bullet_point1."\"\t". // 	商品の仕様 bullet_point1
			"\"".$bullet_point2."\"\t". // 	商品の仕様 bullet_point2
			"\"".$bullet_point3."\"\t". // 	商品の仕様 bullet_point3
			"\"".$bullet_point4."\"\t". // 	商品の仕様 bullet_point4
			"\"".$bullet_point5."\"\t". // 	商品の仕様 bullet_point5
			"\"\"\t". // 	商品の仕様 bullet_point6
			"\"\"\t". // 	商品の仕様 bullet_point7
			"\"\"\t". // 	商品の仕様 bullet_point8
			"\"\"\t". // 	商品の仕様 bullet_point9
			"\"\"\t". // 	商品の仕様 bullet_point10
			"\"".$style_name."\"\t". //	スタイル名 style_name
			"\"".$generic_keywords."\"\t". //	検索キーワード generic_keywords
			"\"".$color_map."\"\t". //	カラーマップ color_map
			"\"".$material_type."\"\t". //	素材の種類 material_type
			"\"\"\t". // 	シーズン seasons1
			"\"\"\t". // 	シーズン seasons2
			"\"\"\t". // 	シーズン seasons3
			"\"\"\t". // 	シーズン seasons4
			"\"\"\t". // 	スポーツの名称 sport_type1
			"\"\"\t". // 	スポーツの名称 sport_type2
			"\"".$golf_club_loft."\"\t". //	ゴルフグラブのロフト（クラブフェースの傾斜角度） golf_club_loft
			"\"".$grip_type."\"\t". //	グリップタイプ grip_type
			"\"".$lens_color."\"\t". //	レンズの色 lens_color
			"\"\"\t". //	レンズの素材 lens_material_type
			"\"\"\t". //	サイン入り商品 is_autographed
			"\"\"\t". //	リーグ名 league_name
			"\"".$hand_orientation."\"\t". //	ハンド hand_orientation
			"\"".$shaft_length_unit_of_measure."\"\t". //	シャフト(軸)の素材 shaft_material
			"\"\"\t". //	プレーヤー名 athlete
			"\"".$team_name."\"\t". //	チーム名 team_name
			"\"\"\t". // 	箱の中身 included_components1
			"\"\"\t". // 	箱の中身 included_components2
			"\"\"\t". // 	箱の中身 included_components3
			"\"\"\t". // 	箱の中身 included_components4
			"\"\"\t". // 	箱の中身 included_components5
			"\"\"\t". // 	互換デバイス compatible_devices1
			"\"\"\t". // 	互換デバイス compatible_devices2
			"\"\"\t". // 	互換デバイス compatible_devices3
			"\"\"\t". // 	互換デバイス compatible_devices4
			"\"\"\t". // 	互換デバイス compatible_devices5
			"\"\"\t". //	毛皮の説明 fur_description
			"\"\"\t". //	golf_club_loft_unit_of_measure golf_club_loft_unit_of_measure
			"\"".$seating_group."\"\t". //	席種 seating_group
			"\"\"\t". //	商品種別 item_type_name
			"\"\"\t". //	オプション option_type
			"\"".$target_audience_base."\"\t". //	対象 target_audience_base
			"\"\"\t". //	この商品はAmazon.co.jp限定商品ですか？ is_exclusive_product
			"\"\"\t". //	箱数 number_of_boxes
			"\"\"\t". //	グローブのタイプ glove_type
			"\"\"\t". //	防水タイプか否か water_resistance_level
			"\"\"\t". //	ライニングの説明 lining_description
			"\"\"\t". //	BMXバイクのタイプ bike_type
			"\"\"\t". //	フレームの素材 frame_material_type
			"\"\"\t". //	設定数 num_of_settings
			"\"".$target_audience_keywords1."\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords1
			"\"".$target_audience_keywords2."\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords2
			"\"\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords3
			"\"\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords4
			"\"\"\t". // 	ターゲットユーザーのキーワード target_audience_keywords5
			"\"".$pattern_name."\"\t". //	デザイン pattern_name
			"\"".$style_keywords."\"\t". //	スタイルキーワード style_keywords
			"\"\"\t". //	シャフトスタイルタイプ shaft_style_type
			"\"".$lithium_battery_voltage."\"\t". //	リチウム電池ボルト数 lithium_battery_voltage
			"\"\"\t". // 	プラチナキーワード platinum_keywords1
			"\"\"\t". // 	プラチナキーワード platinum_keywords2
			"\"\"\t". // 	プラチナキーワード platinum_keywords3
			"\"\"\t". // 	プラチナキーワード platinum_keywords4
			"\"\"\t". // 	プラチナキーワード platinum_keywords15
			"\"\"\t". //	lithium_battery_voltage_unit_of_measure lithium_battery_voltage_unit_of_measure
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit1
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit2
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit3
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit4
			"\"\"\t". // 	ゴルフパターのライ角の単位 golf_putter_lie_angle_unit5
			"\"\"\t". //	メーカー対象最高年齢 mfg_maximum_unit_of_measure
			"\"\"\t". //	商品のオフセットの単位 item_offset_unit_of_measure
			"\"\"\t". //	オフセット item_offset
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle1
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle2
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle3
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle4
			"\"\"\t". // 	パターのライ角を指定する golf_putter_lie_angle5
			"\"\"\t". //	メーカー対象最低年齢 mfg_minimum_unit_of_measure
			"\"\"\t". //	ポケットの説明 pocket_description
			"\"\"\t". //	ウェイトベンチの種類 weight_bench_type
			"\"\"\t". //	対応スキルレベル skill_level
			"\"\"\t". // 	フィットタイプ fit_type1
			"\"\"\t". // 	フィットタイプ fit_type2
			"\"\"\t". // 	フィットタイプ fit_type3
			"\"\"\t". // 	フィットタイプ fit_type4
			"\"\"\t". // 	フィットタイプ fit_type5
			"\"\"\t". //	床の使用面積 floor_area
			"\"\"\t". //	首周りの形状(襟の形状) neck_style
			"\"\"\t". //	パターンのスタイル pattern_type
			"\"\"\t". //	靴下の長さ rise_style
			"\"\"\t". //	テーマ theme
			"\"\"\t". //	トップのスタイル top_style
			"\"\"\t". //	UVプロテクションのレベル ultraviolet_light_protection
			"\"\"\t". //	ホイールタイプ wheel_type
			"\"\"\t". //	フレームタイプ frame_type
			"\"\"\t". //	ブレーキのタイプ brake_style
			"\"\"\t". //	サスペンションのタイプ suspension_type
			"\"\"\t". //	ドアの数 number_of_doors
			"\"\"\t". //	詰め物素材のタイプ fill_material_type
			"\"\"\t". //	商品の据え付けタイプ mounting_type
			"\"\"\t". //	保温レベル temperature_rating_degrees
			"\"\"\t". //	レンズの形状 lens_type
			"\"\"\t". //	絶縁タイプ insulation_material_type
			"\"\"\t". //	パッドのタイプ pad_type
			"\"\"\t". //	ポール数 number_of_poles
			"\"\"\t". //	電球タイプ light_source_type
			"\"\"\t". //	座席数 seating_capacity
			"\"\"\t". //	電源 power_source_type
			"\"\"\t". // 	機能性 special_features1
			"\"\"\t". // 	機能性 special_features2
			"\"\"\t". // 	機能性 special_features3
			"\"\"\t". // 	機能性 special_features4
			"\"\"\t". // 	機能性 special_features5
			"\"\"\t". //	ラップトップ容量 capacity_name
			"\"\"\t". // 	バッテリ寿命 battery_average_life1
			"\"\"\t". // 	バッテリ寿命 battery_average_life2
			"\"\"\t". // 	バッテリ寿命 battery_average_life3
			"\"\"\t". //	magnification_strength magnification_strength
			"\"\"\t". //	wheelbase wheelbase
			"\"\"\t". //	wheelbase_unit_of_measure wheelbase_unit_of_measure
			"\"\"\t". //	shell_type shell_type
			"\"\"\t". //	thesaurus_attribute_keywords thesaurus_attribute_keywords
			"\"\"\t". //	推奨使用用途 recommended_uses_for_product
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords1
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords2
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords3
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords4
			"\"\"\t". // 	特定用途キーワード specific_uses_keywords5
			"\"\"\t". //    thesaurus_subject_keywords1
			"\"\"\t". //    thesaurus_subject_keywords2
			"\"\"\t". //    thesaurus_subject_keywords3
			"\"\"\t". //    thesaurus_subject_keywords4
			"\"\"\t". //    thesaurus_subject_keywords5
			"\"\"\t". //	magnification_strength_unit_of_measure magnification_strength_unit_of_measure
			"\"\"\t". //	ラベルに記載されている国/地域 country_as_labeled
			"\"\"\t". //	釣り技法 fishing_technique
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type1
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type2
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type3
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type4
			"\"\"\t". // 	商品の補充情報 lifecycle_supply_type5
			"\"\"\t". //	床の使用面積の単位 floor_area_unit_of_measure
			"\"\"\t". //	測定単位 temperature_rating_degrees_unit_of_measure
			"\"\"\t". // 	バッテリ平均持続時間の単位 battery_average_life_unit_of_measure1
			"\"\"\t". // 	バッテリ平均持続時間の単位 battery_average_life_unit_of_measure2
			"\"\"\t". // 	バッテリ平均持続時間の単位 battery_average_life_unit_of_measure3
			"\"\"\t". //	シャフトの円周 shaft_circumference
			"\"\"\t". //	シャフトの円周の単位 shaft_circumference_unit_of_measure
			"\"\"\t". //	つま先のスタイル toe_style
			"\"\"\t". //	ミッドソール素材 midsole_material_type
			"\"\"\t". //	仕切りの数 number_of_dividers
			"\"\"\t". //	銃器の口径 caliber
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type1
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type2
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type3
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type4
			"\"\"\t". // 	エアガンパワータイプ air_gun_power_type5
			"\"\"\t". //	絶縁抵抗の単位 insulation_resistance_unit_of_measure
			"\"\"\t". //	BTU で測定した絶縁 insulation_resistance
			"\"\"\t". //	組み立て方法 assembly_instructions
			"\"\"\t". //	ディスプレイタイプ display_type
			"\"\"\t". //	運動器具のアームタイプ exercise_machine_arm_type
			"\"\"\t". // 	スペース効率 space_efficiency1
			"\"\"\t". // 	スペース効率 space_efficiency2
			"\"\"\t". //	心拍数センサーのタイプ heart_rate_sensor_type
			"\"\"\t". //	商品の個数（ピース数） number_of_pieces
			"\"\"\t". //	溝深さ単位 tread_depth_unit_of_measure
			"\"\"\t". //	溝深さ tread_depth
			"\"\"\t". //	ロックタイプ lock_type
			"\"\"\t". //	ハンドル素材 handle_material
			"\"\"\t". //	収納時サイズ folded_size
			"\"\"\t". // 	ポール素材タイプ pole_material_type1
			"\"\"\t". // 	ポール素材タイプ pole_material_type2
			"\"\"\t". // 	ポール素材タイプ pole_material_type3
			"\"\"\t". // 	ポール素材タイプ pole_material_type4
			"\"\"\t". // 	ポール素材タイプ pole_material_type5
			"\"\"\t". //	ハンドルの直径 handle_diameter
			"\"\"\t". //	ハンドル長さの単位 handle_length_unit
			"\"\"\t". //	ハンドル長さの十進数値 handle_length_derived
			"\"\"\t". //	ハンドルのタイプ handle_type
			"\"\"\t". //	ホイール素材 wheel_material
			"\"\"\t". //	ホイールの数 number_of_wheels
			"\"\"\t". // 	対象となる動植物 target_species1
			"\"\"\t". // 	対象となる動植物 target_species2
			"\"\"\t". // 	対象となる動植物 target_species3
			"\"\"\t". // 	対象となる動植物 target_species4
			"\"\"\t". // 	対象となる動植物 target_species5
			"\"\"\t". //	チャンバーの深さの単位 chamber_depth_unit_of_measure
			"\"\"\t". //	チャンバーの深さ chamber_depth
			"\"\"\t". //	バンド長 band_length
			"\"\"\t". //	バンド材質タイプ band_material_type
			"\"\"\t". //	バンド色 band_color
			"\"\"\t". //	バンド長の単位 band_length_unit
			"\"\"\t". //	速度測定 speed_rating
			"\"\"\t". //	プログラム数 number_of_programs
			"\"\"\t". //	モーターサイズ maximum_horsepower
			"\"\"\t". //	最大馬力単位 maximum_horsepower_unit_of_measure
			"\"\"\t". //	最低速度の単位 minimum_speed_unit_of_measure
			"\"\"\t". //	組み立てる必要がありますか? is_assembly_required
			"\"\"\t". //	最大速度の単位 maximum_speed_unit_of_measure
			"\"\"\t". //	最大速度 maximum_speed
			"\"\"\t". //	組み立て時間の単位 assembly_time_unit_of_measure
			"\"\"\t". //	電源プラグ power_plug_type
			"\"\"\t". //	組み立て時間 assembly_time
			"\"\"\t". //	最低速度 minimum_speed
			"\"\"\t". //	動作モード operation_mode
			"\"\"\t". //	気流量 air_flow_capacity
			"\"\"\t". //	空気流量の測定単位 air_flow_capacity_unit_of_measure
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type1
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type2
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type3
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type4
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type5
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type6
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type7
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type8
			"\"\"\t". // 	適合車両の種類 compatible_with_vehicle_type9
			"\"".$golf_club_flex."\"\t". //	ゴルフクラブのシャフトの硬さ golf_club_flex
			"\"".$tension_level."\"\t". //	張力（テンション） tension_level
			"\"\"\t". //	代金引換 is_cod_included
			"\"\"\t". //	FCの保存可能期間の単位 fc_shelf_life_unit_of_measure
			"\"".$shipping_options."\"\t". //	配送オプション shipping_options
			"\"\"\t". //	コンビニ決済 is_cvs_included
			"\"\"\t". //	レザーの種類 leather_type
			"\"\"\t". //	標準抵抗メカニズムの値 resistance_mechanism
			"\"".$grip_material_type."\"\t". //	グリップ素材タイプ grip_material_type
			"\"".$fishing_line_type."\"\t". //	釣り糸素材のタイプ fishing_line_type
			"\"\"\t". //	軸径の十進数値 shaft_diameter_derived
			"\"\"\t". //	軸径の単位 shaft_diameter_unit_of_measure
			"\"\"\t". //	シャフト素材タイプ shaft_material_type
			"\"\"\t". //	メーカー推奨最高年齢 mfg_maximum
			"\"\"\t". //	構成 configuration
			"\"\"\t". //	メーカー推奨最少年齢 mfg_minimum
			"\"\"\t". //	ポケットの数 number_of_pockets
			"\"\"\t". //	Item Depth Unit Of Measure item_depth_unit_of_measure
			"\"\"\t". //	スラット付きフレーム調整タイプ adjustment_type
			"\"\"\t". //	調節可能な長さ has_adjustable_length
			"\"\"\t". //	ストラップのタイプ strap_type
			"\"\"\t". //	構造 construction_type
			"\"\"\t". //	ボトムのタイプ bottom_style
			"\"\"\t". //	脚のスタイル leg_style
			"\"\"\t". //	袖のタイプ sleeve_type
			"\"\"\t". //	サポートタイプ support_type
			"\"\"\t". //	線の太さ line_weight
			"\"\"\t". //	速度の数 number_of_speeds
			"\"\"\t". //	レベル数 number_of_power_levels
			"\"\"\t". //	装具タイプ fitting_type
			"\"\"\t". //	ギアループの数 number_of_gear_loops
			"\"\"\t". //	最小胴体適合サイズ minimum_torso_measurement
			"\"\"\t". //	就寝人数 occupancy
			"\"\"\t". //	最大倍率 magnification_maximum
			"\"\"\t". //	使用容量の単位 capacity_name_unit_of_measure
			"\"\"\t". //	台座タイプ strand_type
			"\"\"\t". //	反射技術 reflecting_technology
			"\"\"\t". //	base_material_type base_material_type
			"\"\"\t". // 	仕上げ finish_type1
			"\"\"\t". // 	仕上げ finish_type2
			"\"\"\t". //	シャフト長（10進値） shaft_length_derived
			"\"\"\t". //	取付穴の単位 mounting_hole_diameter_unit_of_measure
			"\"\"\t". //	取り付け穴の直径の十進数値 mounting_hole_diameter_derived
			"\"\"\t". //	ライフベストのタイプ life_vest_type
			"\"\"\t". //	インソール素材 insole_material_type
			"\"\"\t". // 	防しわ性 wrinkle_resistance1
			"\"\"\t". // 	防しわ性 wrinkle_resistance2
			"\"\"\t". // 	防しわ性 wrinkle_resistance3
			"\"\"\t". // 	防しわ性 wrinkle_resistance4
			"\"\"\t". // 	防しわ性 wrinkle_resistance5
			"\"\"\t". //	legal_compliance_certification_certifying_authority_name legal_compliance_certification_certifying_authority_name
			"\"\"\t". //	legal_compliance_certification_geographic_jurisdiction legal_compliance_certification_geographic_jurisdiction
			"\"\"\t". //	ラウンド数 number_of_rounds
			"\"\"\t". //	バレル素材タイプ barrel_material_type
			"\"\"\t". //	品種 variety
			"\"\"\t". //	抵抗レベル数 number_of_resistance_levels
			"\"\"\t". //	駆動システム drive_system
			"\"\"\t". //	トレッドタイプ tread_type
			"\"\"\t". //	タイヤタイプ tire_type
			"\"\"\t". //	レイヤ数 number_of_layers
			"\"\"\t". //	耐水技術 water_resistance_technology
			"\"\"\t". //	材料の特徴 material_features
			"\"\"\t". //	ケーブル長（10進値） cable_length_derived
			"\"\"\t". //	十字穴番号 screw_point_style
			"\"\"\t". //	期間の単位 duration_unit_of_measure
			"\"\"\t". //	期間 duration
			"\"\"\t". //	バンド長 band_size
			"\"\"\t". //	引張バネの数 number_of_springs
			"\"\"\t". //	ベアリング材質のタイプ bearing_material_type
			"\"\"\t". //	変速機減速比 gearbox_ratio
			"\"\"\t". //	デュロメーターの硬さ durometer_hardness
			"\"\"\t". //	トレッドミルの最大傾斜 maximum_incline_percentage
			"\"\"\t". //	サイクルタイプ motor_type
			"\"\"\t". //	ベルト長派生 belt_length_derived
			"\"\"\t". //	長ベルト長の測定単位 belt_length_unit_of_measure
			"\"\"\t". //	ベルト幅の単位 belt_width_unit_of_measure
			"\"\"\t". //	belt_length_string belt_length_string
			"\"\"\t". //	ベルト幅 belt_width_string
			"\"\"\t". //	ベルト幅の十進数値 belt_width_derived
			"\"".$size_map."\"\t". //	サイズマップ size_map
			"\"".$grip_size."\"\t". //	グリップサイズ grip_size
			"\"".$volume_capacity_name."\"\t". //	定格内容積（総容積） volume_capacity_name
			"\"".$shaft_length."\"\t". //	シャフト(軸)の長さ shaft_length
			"\"".$shaft_length_unit_of_measure."\"\t". //	シャフト(軸)の長さの単位 shaft_length_unit_of_measure
			"\"".$item_shape."\"\t". //	形状 item_shape
			"\"".$display_maximum_weight_recommendation."\"\t". //	対応重量 display_maximum_weight_recommendation
			"\"".$display_maximum_weight_recommendation_unit_of_measure."\"\t". //	対応重量の単位 display_maximum_weight_recommendation_unit_of_measure
			"\"".$item_display_length."\"\t". //	ロッド(棒)の長さ item_display_length
			"\"".$item_display_length_unit_of_measure."\"\t". //	ロッド(棒)の長さの単位 item_display_length_unit_of_measure
			"\"".$item_display_width."\"\t". //	幅 item_display_width
			"\"".$item_display_width_unit_of_measure."\"\t". //	幅の単位 item_display_width_unit_of_measure
			"\"".$item_display_weight."\"\t". //	商品の重量（パッケージを含まない） item_display_weight
			"\"".$item_display_weight_unit_of_measure."\"\t". //	ロッド(棒)の重量の単位 item_display_weight_unit_of_measure
			"\"\"\t". //	ヘッドサイズ名 head_size_name
			"\"\"\t". //	商品の高さの単位 item_height_unit_of_measure
			"\"\"\t". //	液体の容量 liquid_volume
			"\"\"\t". //	液体量の単位 liquid_volume_unit_of_measure
			"\"\"\t". //	商品本体サイズ：長さ item_length
			"\"\"\t". //	商品の幅の単位 item_width_unit_of_measure
			"\"\"\t". //	容積容量名の単位 volume_capacity_name_unit_of_measure
			"\"\"\t". //	商品本体サイズ：幅 item_width
			"\"\"\t". //	商品本体サイズ：高さ item_height
			"\"\"\t". //	商品の長さの単位 item_length_unit_of_measure
			"\"".$wheel_size."\"\t". //	ホイールサイズ wheel_size
			"\"".$wheel_size_unit_of_measure."\"\t". //	ホイールサイズの単位 wheel_size_unit_of_measure
			"\"\"\t". //	負荷容量 load_capacity
			"\"\"\t". //	負荷容量の単位 load_capacity_unit_of_measure
			"\"".$website_shipping_weight."\"\t". //	配送重量 website_shipping_weight
			"\"".$website_shipping_weight_unit_of_measure."\"\t". //	配送重量の単位 website_shipping_weight_unit_of_measure
			"\"".$rim_size."\"\t". //	リムサイズ rim_size
			"\"".$rim_size_unit_of_measure."\"\t". //	バイクのリムサイズの単位 rim_size_unit_of_measure
			"\"".$capacity."\"\t". //	最大の高さ（ピーク）　 capacity
			"\"".$capacity_unit_of_measure."\"\t". //	容量の単位 capacity_unit_of_measure
			"\"".$item_display_diameter."\"\t". //	商品の直径 item_display_diameter
			"\"".$item_display_diameter_unit_of_measure."\"\t". //	リールの直径の単位 item_display_diameter_unit_of_measure
			"\"".$item_display_height."\"\t". //	商品の高さ item_display_height
			"\"".$item_display_height_unit_of_measure."\"\t". //	高さの単位 item_display_height_unit_of_measure
			"\"\"\t". //	シャフト(軸)の丈 shaft_height
			"\"\"\t". //	シャフトの高さの測定単位 shaft_height_unit_of_measure
			"\"\"\t". //	商品の寸法の単位 item_dimensions_unit_of_measure
			"\"\"\t". //	商品の深さ item_depth
			"\"\"\t". //	最大積載可能重量の単位 maximum_weight_capacity
			"\"\"\t". //	最大荷重の単位 maximum_weight_capacity_unit_of_measure
			"\"\"\t". //	メーカー推奨最大重量 maximum_weight_recommendation
			"\"\"\t". //	推奨最大重量の単位 maximum_weight_recommendation_unit_of_measure
			"\"\"\t". //	最低の高さの単位 minimum_height_unit_of_measure
			"\"\"\t". //	最大高さ maximum_height
			"\"\"\t". //	最小の高さ minimum_height
			"\"\"\t". //	最大高さの単位 maximum_height_unit_of_measure
			"\"\"\t". //	ブレーキの幅 brake_width
			"\"\"\t". //	ブレーキの幅の単位 brake_width_unit_of_measure
			"\"\"\t". //	フロアの長さ floor_length
			"\"\"\t". //	フロアの幅 floor_width
			"\"\"\t". //	靴の幅 shoe_width
			"\"\"\t". //	袖の長さ sleeve_length
			"\"\"\t". //	ウエストのサイズ waist_size
			"\"\"\t". //	クランクの長さ crank_length
			"\"\"\t". //	トップチューブの長さ top_tube_length
			"\"\"\t". //	股下の長さ inseam_length
			"\"\"\t". //	有効エッジの長さ effective_edge_length
			"\"\"\t". //	刃の長さ blade_length
			"\"\"\t". //	メーカー推奨最大身長 maximum_height_recommendation
			"\"\"\t". //	破断強度 breaking_strength
			"\"\"\t". //	破断強度の単位 breaking_strength_unit_of_measure
			"\"\"\t". //	対物レンズのサイズ objective_lens_diameter
			"\"\"\t". //	商品対応範囲 item_area
			"\"\"\t". //	商品の厚さ item_thickness_derived
			"\"\"\t". //	袖の長さの単位 sleeve_length_unit_of_measure
			"\"\"\t". //	フロアの長さの単位 floor_length_unit_of_measure
			"\"\"\t". //	フロアの幅の単位 floor_width_unit_of_measure
			"\"\"\t". //	ウエストサイズの単位 waist_size_unit_of_measure
			"\"\"\t". //	クランクの長さの単位 crank_length_unit_of_measure
			"\"\"\t". //	トップチューブの長さの単位 top_tube_length_unit_of_measure
			"\"\"\t". //	有効エッジの長さの単位 effective_edge_length_unit_of_measure
			"\"\"\t". //	刃の断面の形状の単位 blade_length_unit_of_measure
			"\"\"\t". //	推奨最高身長の単位 maximum_height_recommendation_unit_of_measure
			"\"\"\t". //	対物レンズのサイズの単位 objective_lens_diameter_unit_of_measure
			"\"\"\t". //	商品対応範囲の単位 item_area_unit_of_measure
			"\"\"\t". //	商品の厚さの単位 item_thickness_unit_of_measure
			"\"\"\t". //	股下の長さの単位 inseam_length_unit_of_measure
			"\"\"\t". //	靴幅の単位 shoe_width_unit_of_measure
			"\"\"\t". //	高さマップ height_map
			"\"\"\t". //	胸囲サイズの単位 chest_size_unit_of_measure
			"\"\"\t". //	胸囲サイズ chest_size
			"\"\"\t". //	強度 tensile_strength
			"\"\"\t". //	強度の単位 tensile_strength_unit_of_measure
			"\"\"\t". //	タイヤの直径 tire_diameter
			"\"\"\t". //	タイヤ直径の単位 tire_diameter_unit_of_measure
			"\"\"\t". //	拡張された長さ extended_length
			"\"\"\t". //	伸びた長さの測定単位 extended_length_unit_of_measure
			"\"\"\t". //	ケーブル長さの単位 cable_length_unit_of_measure
			"\"\"\t". //	ハンドルの高さ handle_height
			"\"\"\t". //	ハンドルの高さの単位 handle_height_unit_of_measure
			"\"\"\t". //	チャンバーの高さの単位 chamber_height_unit_of_measure
			"\"\"\t". //	チャンバーの幅単位 chamber_width_unit_of_measure
			"\"\"\t". //	チャンバーの幅 chamber_width
			"\"\"\t". //	チャンバーの高さ chamber_height
			"\"\"\t". //	バンドの幅の単位 band_width_unit_of_measure
			"\"\"\t". //	バンド幅 band_width
			"\"\"\t". //	フレームの高さ frame_size
			"\"\"\t". //	フレームの高さの単位 frame_size_unit_of_measure
			"\"\"\t". //	デッキの長さ deck_length
			"\"\"\t". //	デッキの幅 deck_width
			"\"\"\t". //	デッキ長の単位 deck_length_unit_of_measure
			"\"\"\t". //	デッキ幅の単位 deck_width_unit_of_measure
			"\"\"\t". //	ディスプレイサイズ display_size
			"\"\"\t". //	ディスプレイ最大解像度の単位 display_size_unit_of_measure
			"\"\"\t". //	ディスプレイサイズ monitor_size
			"\"\"\t". //	モニターサイズの単位 monitor_size_unit_of_measure
			"\"\"\t". //	最大圧力の単位 maximum_pressure_unit_of_measure
			"\"\"\t". //	最大圧力 maximum_pressure 	
			"\"".$fulfillment_center_id."\"\t". //	フルフィルメントセンターID fulfillment_center_id
			"\"".$package_height."\"\t". //	商品パッケージの高さ package_height
			"\"".$package_width."\"\t". //	商品パッケージの幅 package_width
			"\"".$package_length."\"\t". //	商品パッケージの長さ package_length
			"\"".$package_length_unit_of_measure."\"\t". //	商品パッケージの長さの単位 package_length_unit_of_measure
			"\"".$package_weight."\"\t". //	商品パッケージの重量 package_weight
			"\"".$package_weight_unit_of_measure."\"\t". //	商品パッケージの重量の単位 package_weight_unit_of_measure
			"\"\"\t". //	商品パッケージの高さの単位 package_height_unit_of_measure
			"\"\"\t". //	商品パッケージの幅の単位 package_width_unit_of_measure
			"\"\"\t". //	商品パッケージの寸法の単位 package_dimensions_unit_of_measure
			"\"".$country_of_origin."\"\t". //	原産国/地域 country_of_origin
			"\"\"\t". // 	布タイプ fabric_type1
			"\"\"\t". // 	布タイプ fabric_type2
			"\"\"\t". // 	布タイプ fabric_type3
			"\"\"\t". // 	布タイプ fabric_type4
			"\"\"\t". // 	布タイプ fabric_type5
			"\"".$are_batteries_included."\"\t". //	電池付属 are_batteries_included
			"\"".$batteries_required."\"\t". //	この商品は電池本体ですか？または電池を使用した商品ですか？ batteries_required
			"\"".$battery_type."\"\t". // 	電池の種類、サイズ battery_type1
			"\"\"\t". //!! 	電池の種類、サイズ battery_type2
			"\"\"\t". //!! 	電池の種類、サイズ battery_type3
			"\"".$number_of_batteries."\"\t". // 	電池の数 number_of_batteries1
			"\"\"\t". //!! 	電池の数 number_of_batteries2
			"\"\"\t". //!! 	電池の数 number_of_batteries3
			"\"".$lithium_battery_energy_content."\"\t". //	電池当たりのワット時 lithium_battery_energy_content
			"\"".$lithium_battery_packaging."\"\t". //	リチウム電池パッケージ lithium_battery_packaging
			"\"".$lithium_battery_weight."\"\t". //	リチウム含有量(グラム) lithium_battery_weight
			"\"\"\t". //!!	リチウムイオン電池単数 number_of_lithium_ion_cells
			"\"\"\t". //!!	リチウムメタル電池単数 number_of_lithium_metal_cells
			"\"\"\t". //!!	電池の組成 battery_cell_composition
			"\"\"\t". //!!	電池の重量(グラム) battery_weight
			"\"\"\t". //!!	電池の重量の測定単位 battery_weight_unit_of_measure
			"\"\"\t". //!!	リチウム電池のエネルギー量測定単位 lithium_battery_energy_content_unit_of_measure
			"\"\"\t". //!!	リチウム電池の重量の測定単位 lithium_battery_weight_unit_of_measure
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation1
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation2
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation3
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation4
			"\"\"\t". //!! 	適用される危険物関連法令 supplier_declared_dg_hz_regulation5
			"\"\"\t". //!!	国連(UN)番号 hazmat_united_nations_regulatory_id
			"\"\"\t". //!!	安全データシート(SDS) URL safety_data_sheet_url
			"\"\"\t". //!!	商品の重量 item_weight
			"\"\"\t". //!!	商品の重量の単位 item_weight_unit_of_measure
			"\"\"\t". //!!	商品の体積 item_volume
			"\"\"\t". //!!	商品の容量の単位 item_volume_unit_of_measure
			"\"\"\t". //!!	内用液は含まれていますか? contains_liquid_contents
			"\"\"\t". //	引火点(°C) flash_point
			"\"\"\t". //	商品に適用される保管規制の区分 fulfillment_center_special_storage
			"\"\"\t". //	メーカー保証の説明 warranty_description
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択) ghs_classification_class1
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択) ghs_classification_class2
			"\"\"\t". //!! 	分類／危険物ラベル(適用されるものをすべて選択) ghs_classification_class3
			"\"".$mfg_warranty_description_type."\"\t". //	保証 mfg_warranty_description_type
			"\"".$safety_warning."\"\t". //	安全性に関する注意 safety_warning
			"\"".$legal_disclaimer_description."\"\t". //	法規上の免責条項 legal_disclaimer_description
			"\"".$specific_uses_for_product."\"\t". //	特徴・用途 specific_uses_for_product
			"\"\"\t". //	legal_compliance_certification_date_of_issue legal_compliance_certification_date_of_issue
			"\"\"\t". //	import_designation import_designation
			"\"\"\t". //	legal_compliance_certification_metadata legal_compliance_certification_metadata
			"\"\"\t". //	legal_compliance_certification_expiration_date legal_compliance_certification_expiration_date
			"\"\"\t". //	cpsia_cautionary_statement cpsia_cautionary_statement
			"\"\"\t". //	cpsia_cautionary_description cpsia_cautionary_description
			"\"\"\t". //	適合性の認証の状況 legal_compliance_certification_status
			"\"\"\t". //	規制機関の名前 legal_compliance_certification_regulatory_organization_name
			"\"\"\t". //	コンプライアンス認証値 legal_compliance_certification_value
			"\"\"\t". //	保証タイプ warranty_type
			"\"\"\t". //	熱に弱い is_heat_sensitive
			"\"\"\t". //	通貨コード currency
			"\"".$condition_type."\"\t". //	コンディション condition_type
			"\"".$condition_note."\"\t". //	商品のコンディション説明 condition_note
			"\"".$list_price."\"\t". //	メーカー希望価格 list_price
			"\"".$product_site_launch_date."\"\t". //	商品の公開日 product_site_launch_date
			"\"".$merchant_release_date."\"\t". //	予約商品の販売開始日 merchant_release_date
			"\"".$restock_date."\"\t". //	商品の入荷予定日 restock_date
			"\"".$fulfillment_latency."\"\t". //	出荷作業日数 fulfillment_latency
			"\"\"\t". //	取り寄せ is_sourcing_on_demand
			"\"".$optional_payment_type_exclusion."\"\t". //	使用しない支払い方法 optional_payment_type_exclusion
			"\"".$sale_price."\"\t". //	セール価格 sale_price
			"\"".$sale_from_date."\"\t". //	セール開始日 sale_from_date
			"\"".$sale_end_date."\"\t". //	セール終了日 sale_end_date
			"\"".$max_order_quantity."\"\t". //	最大注文個数 max_order_quantity
			"\"\"\t". //	パッケージ数 item_package_quantity
			"\"".$offering_can_be_gift_messaged."\"\t". //	ギフトメッセージ offering_can_be_gift_messaged
			"\"".$offering_can_be_gift_messaged."\"\t". //	ギフト包装 offering_can_be_giftwrapped
			"\"".$merchant_shipping_group_name."\"\t". //	配送パターン merchant_shipping_group_name
			"\"".$distribution_designation."\"\t". //	販売形態(並行輸入品) distribution_designation
			"\"\"\t". //	在庫期間 fc_shelf_life
			"\"\"\t". //	終了日を提案する offering_end_date
			"\"\"\t". //	最低広告表示価格 map_price
			"\"\"\t". //	賞味期限もしくは消費期限タイプ product_expiration_type
			"\"\"\t". //	付属品総数 number_of_items
			"\"\"\t". //	開始日を提案する offering_start_date
			"\"\"\t". //	商品タックスコード product_tax_code
			"\"".$standard_points_percent."\"\t". //	ポイントパーセント standard_price_points_percent
			"\"".$sale_points_percent."\"\t". //	セール時ポイントパーセント sale_price_points_percent
			"\"".$delivery_schedule_group_id."\"\t". //	配送日時指定SKUリスト delivery_schedule_group_id
			"\"".$is_discontinued_by_manufacturer."\"\t". //	メーカー製造中止 is_discontinued_by_manufacturer
			"\"\"\t". //	サイン入り signed_by
			"\"\"\t". //	max_aggregate_ship_quantity max_aggregate_ship_quantity			
			"\n";

		}
	}

if ($ck_dis_flg == 1) {
	echo "end g_num<>".$g_num."<br>\n";
	echo "<br>\n";
	echo $tsv."<br>\n";
	echo "<br>\n";
}

	return $tsv;
}



//	出品ファイルL用
function amazon_zaiko_item_tsv($AMZ_UP_LIST, $AMZ_SYU_LIST) {

	$tsv = "";
	if ($AMZ_SYU_LIST) {
		foreach ($AMZ_SYU_LIST AS $val) {
			$tsv .= $val;
		}
	}

	//	削除リスト作成
	if ($AMZ_UP_LIST) {
		foreach ($AMZ_UP_LIST AS $sku => $VAL) {
			//$sku = "";
			$price = "";
			$standard_price_points = "";
			$quantity = "";
			$product_id = "";
			$product_id_type = "";
			$condition_type = "";
			$condition_note = "";
			$ASIN_hint = "";
			$title = "";
			$operation_type = "";
			$sale_price = "";
			$sale_price_points = "";
			$sale_start_date = "";
			$sale_end_date = "";
			$leadtime_to_ship = "";
			$launch_date = "";
			$is_giftwrap_available = "";
			$is_gift_message_available = "";
			$fulfillment_center_id = "";
			$optional_payment_type_exclusion = "";
			$main_offer_image = "";
			$offer_image1 = "";
			$offer_image2 = "";
			$offer_image3 = "";
			$offer_image4 = "";
			$offer_image5 = "";
			$merchant_shipping_group_name = "";		//	add ohkawara 2017/08/04

			$price = $VAL['price'];
			$product_id = $VAL['asin'];
			$quantity = 0;
			//$operation_type = "Update";	//	後にDeleteになるかも？	//	del ohkawara 2019/11/18
			$operation_type = "Delete";									//	add ohkawara 2019/11/18

			if ($product_id != "") {
				$len = strlen($product_id);
				if ($len == 16) {
					$product_id_type = "GCID";
				} elseif ($len == 12) {
					$product_id_type = "UPC";
				} elseif ($len == 13) {
					$product_id_type = "EAN";
				} elseif ($len == 10) {
					$product_id_type = "ASIN";
				}
			} else {
				continue;
			}

			$tsv .= "\"".$sku."\"\t".  // 	商品管理番号sku
			"\"".$price."\"\t".  // 	販売価格  price
			"\"".$standard_price_points."\"\t".  //	ポイントパーセント  standard-price-points-percent
			"\"".$quantity."\"\t".  // 	在庫数  quantity
			"\"".$product_id."\"\t".  // 	商品コード(JANコード等)  product-id
			"\"".$product_id_type."\"\t".  // 	商品コードのタイプ  product-id-type
			"\"".$condition_type."\"\t".  // 	商品のコンディション  condition-type
			"\"".$condition_note."\"\t".  // 	商品のコンディション説明  condition-note
			"\"".$ASIN_hint."\"\t".  // 	対象  ASIN-hint
			"\"".$title."\"\t".  // 	商品名  title
			"\"".$operation_type."\"\t".  // 	動作タイプ  operation-type
			"\"".$sale_price."\"\t".  // 	セール価格  sale-price
			"\"".$sale_price_points."\"\t".  // 	セール時ポイントパーセントsale-price-points-percent
			"\"".$sale_start_date."\"\t".  // 	セール開始日  sale-start-date
			"\"".$sale_end_date."\"\t".  // 	セール終了日  sale-end-date
			"\"".$leadtime_to_ship."\"\t".  // 	リードタイム(出荷までにかかる作業日数)  leadtime-to-ship
			"\"".$launch_date."\"\t".  // 	商品の公開日  launch-date
			"\"".$is_giftwrap_available."\"\t".  // 	ギフト包装  is-giftwrap-available
			"\"".$is_gift_message_available."\"\t".  // 	ギフトメッセージ  is-gift-message-available
			"\"".$fulfillment_center_id."\"\t".  // 	フルフィルメントセンターID  fulfillment-center-id
			"\"".$optional_payment_type_exclusion."\"\t".  // 	使用しない支払い方法  optional-payment-type-exclusion
			"\"".$main_offer_image."\"\t".  // 	出品者SKUのメイン画像  URLmain-offer-image
			"\"".$offer_image1."\"\t".  //  	出品者SKUのサブ画像  URL1 - 出品者SKUのサブ画像URL5offer-image1- offer-image5
			"\"".$merchant_shipping_group_name."\"\t".  // 	配送パターン merchant_shipping_group_name
			"\"\"\t".  // 	商品タックスコード  product_tax_code
			"\"\"\t".  // 	電池の有無  batteries_required
			"\"\"\t".  // 	電池の付属有無  are_batteries_included
			"\"\"\t".  // 	電池の種類  battery_cell_composition
			"\"\"\t".  // 	電池の規格  battery_type
			"\"\"\t".  // 	付属する電池の数  number_of_batteries
			"\"\"\t".  // 	電池の重量(グラム)  battery_weight
			"\"\"\t".  // 	リチウムイオンのセル数  number_of_lithium_ion_cells
			"\"\"\t".  // 	リチウム金属のセル数  number_of_lithium_metal_cells
			"\"\"\t".  // 	リチウム電池の梱包状態  lithium_battery_packaging
			"\"\"\t".  // 	リチウム電池のエネルギー量  lithium_battery_energy_content
			"\"\"\t".  // 	リチウム電池の重量  lithium_battery_weight
			"\"\"\t".  // 	危険物該当の該否 supplier_declared_dg_hz_regulation1-supplier_declared_dg_hz_regulation5
			"\"\"\t".  // 	国連(UN)番号  hazmat_united_nations_regulatory_id
			"\"\"\t".  // 	安全データシート(SDS) のURL  safety_data_sheet_url
			"\"\"\t".  // 	商品の重量  item_weight
			"\"\"\t".  // 	商品の体積  item_volume
			"\"\"\t".  // 	引火点(°C)  flash_point
			"\"\"\t".  // 	危険物ラベル(該当するものをすべて選択)  ghs_classification_class1-ghs_classification_class3
			"\"\"\t".  // 	商品の重量の単位  item_weight_unit_of_measure
			"\"\"\t".  // 	商品の容積の単位  item_volume_unit_of_measure
			"\"\"\t".  // 	リチウム電池のエネルギー量の単位  lithium_battery_energy_content_unit_of_measure
			"\"\"\t".  // 	リチウム電池の重量単位  lithium_battery_weight_unit_of_measure
			"\"\"\t".  // 	電池の重量単位  battery_weight_unit_of_measure
			"\n";		//	add ohkawara 2017/08/04
		}
	}

	return $tsv;
}



//	出品ファイルL用ヘッダ
function amazon_tsv_head0() {

	$head  = "TemplateType=Offer	Version=2020.000\n";
	$head .= "商品管理番号	販売価格	ポイントパーセント	在庫数	商品コード(JANコード等)	商品コードのタイプ	商品のコンディション	商品のコンディション説明	対象ASIN	商品名	動作タイプ	セール価格	セール時ポイントパーセント	セール開始日	セール終了日	リードタイム(出荷までにかかる作業日数)	商品の公開日	ギフト包装	ギフトメッセージ	フルフィルメントセンターID	使用しない支払い方法	出品者SKUのメイン画像URL	出品者SKUのサブ画像URL1 	出品者SKUのサブ画像URL2	出品者SKUのサブ画像URL3	出品者SKUのサブ画像URL4	出品者SKUのサブ画像URL5	 配送パターン	商品タックスコード	\"電池の有無\"	\"電池付属\"	\"バッテリーセルタイプ\"	\"電池のタイプ\"	\"付属バッテリ個数\"	電池の重量(グラム)	\"リチウムイオン電池単数\"	\"リチウムメタル電池単数\"	\"リチウム電池パッケージ\"	\"リチウム電池エネルギー量\"	\"リチウム電池重量\"	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。1	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。2	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。3	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。4	この商品は、危険物に該当しますか？詳細については危険物確認ガイド{X}をご覧ください。5	国連(UN)番号	安全データシート(SDS) URL	\"商品の重量\"	\"体積\"	引火点(°C)	分類／危険物ラベル(適用されるものをすべて選択)1	分類／危険物ラベル(適用されるものをすべて選択)2	分類／危険物ラベル(適用されるものをすべて選択)3	商品の重量の単位	商品の容量の単位	リチウム電池のエネルギー量測定単位	リチウム電池の重量の測定単位	リチウム電池の重量の測定単位\n";
	//$head .= "商品管理番号	販売価格	ポイント	在庫数	商品コード(JANコード等)	商品コードのタイプ	商品のコンディション	商品のコンディション説明	対象ASIN	商品名	動作タイプ	セール価格	セール時ポイント	セール開始日	セール終了日	リードタイム(出荷までにかかる作業日数)	商品の公開日	ギフト包装	ギフトメッセージ	フルフィルメントセンターID	使用しない支払い方法	出品者SKUのメイン画像URL	出品者SKUのサブ画像URL1 	出品者SKUのサブ画像URL2	出品者SKUのサブ画像URL3	出品者SKUのサブ画像URL4	出品者SKUのサブ画像URL5	配送パターン\n";
	$head .= "sku	price	standard-price-points-percent	quantity	product-id	product-id-type	condition-type	condition-note	ASIN-hint	title	operation-type	sale-price	sale-price-points-percent	sale-start-date	sale-end-date	leadtime-to-ship	launch-date	is-giftwrap-available	is-gift-message-available	fulfillment-center-id	optional-payment-type-exclusion	main-offer-image	offer-image1	offer-image2	offer-image3	offer-image4	offer-image5	merchant_shipping_group_name	product-tax-code	batteries_required	are_batteries_included	battery_cell_composition	battery_type	number_of_batteries	battery_weight	number_of_lithium_ion_cells	number_of_lithium_metal_cells	lithium_battery_packaging	lithium_battery_energy_content	lithium_battery_weight	supplier_declared_dg_hz_regulation1	supplier_declared_dg_hz_regulation2	supplier_declared_dg_hz_regulation3	supplier_declared_dg_hz_regulation4	supplier_declared_dg_hz_regulation5	hazmat_united_nations_regulatory_id	safety_data_sheet_url	item_weight	item_volume	flash_point	ghs_classification_class1	ghs_classification_class2	ghs_classification_class3	item_weight_unit_of_measure	item_volume_unit_of_measure	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight_unit_of_measure	battery_weight_unit_of_measure\n";
	//$head .= "sku	price	standard-price-points	quantity	product-id	product-id-type	condition-type	condition-note	ASIN-hint	title	operation-type	sale-price	sale-price-points	sale-start-date	sale-end-date	leadtime-to-ship	launch-date	is-giftwrap-available	is-gift-message-available	fulfillment-center-id	optional-payment-type-exclusion	main-offer-image	offer-image1	offer-image2	offer-image3	offer-image4	offer-image5	merchant_shipping_group_name\n";

	return $head;

}



//	アパレル＆シューズ用ヘッダ
function amazon_tsv_head1() {

	//	ohkawara 2017/08/01	更新
	//	ohkawara 2017/08/07	更新

	//	ohkawara 2019/12/12
	// $head  = "TemplateType=fptcustomcustom	Version=2019.1210	TemplateSignature=Q09BVCxUVU5JQyxERVNJR05FUl9ZVUtBVEEsT1VURVJXRUFSLFNXRUFURVIsU0FTSCxORUNLVElFLERVUEFUVEEsQ0hJTERSRU5TX0NPU1RVTUUsU09DS1NIT1NJRVJZLEVUSE5JQ19XRUFSLEFQUk9OLEJBQ0tQQUNLLExFSEVOR0EsQkxBWkVSLEJBQllfQ09TVFVNRSxTVUlULFBST1RFQ1RJVkVfR0VBUixERVNJR05FUl9DSEFOQ0hBTktPLFBFUlNPTkFMQk9EWUNBUkUsREVTSUdORVJfT0JJLFNXSU1XRUFSLERFU0lHTkVSX0tJTU9OTyxBUFBBUkVMX0dMT1ZFUyxTT0NLUyxQQU5UUyxIT01FQUNDRVNTT1JZLENBTUlTT0xFLENIQU5DSEFOS08sQ0xPVEhfV1JBUCxOT05BUFBBUkVMTUlTQyxTS0lSVCxDT1JTRVQsU0hPUlRTLEhPU0lFUlksTEVPVEFSRCxPUkNBX1NISVJULFVOREVSV0VBUixTTEVFUFdFQVIsU0NBUkYsU0FSRUUsQ0hPTEksTEVIRU5HQV9DSE9MSV9TRVQsT0JJLEtVUlRBLFNBTFdBUl9TVUlUX1NFVCxBRFVMVF9DT1NUVU1FLFNISVJULEtJTU9OTyxST0JFLFVOSU9OX1NVSVQsQVBQQVJFTCxUSUdIVFMsQkFHLEFQUEFSRUxfQkVMVCxZVUtBVEEsQlJBLE9WRVJBTExTLERSRVNTLEJPRFlTVE9DS0lORyxBQ0NFU1NPUlksQkxPVVNFLFZFU1QsSklOQkVJLEhBVCxTSE9FUw==	上3行は Amazon.com 記入用です。上3行は変更または削除しないでください。.																																				画像									バリエーション				商品基本情報																				商品検索情報																						推奨ブラウズノード別の情報								寸法																					コンプライアンス情報																						出品情報																							b2b														\n";
	// $head .= "feed_product_type	出品者SKU	ブランド名	商品名	商品コード(JANコード等)	商品コードのタイプ	表地素材	推奨ブラウズノード	item_length_description	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	素材構成	サイズ	カラー	カラー	スタイル名	lifestyle1	lifestyle2	department_name	機能性	機能性	機能性	機能性	機能性	サイズマップ	バンドサイズ	バンドサイズの単位	アダルト商品	在庫数	商品の販売価格	item_package_quantity	商品メイン画像URL	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	カラーサンプル画像URL	親子関係の指定	親商品のSKU(商品管理番号)	親子関係のタイプ	バリエーションテーマ	アップデート・削除	メーカー型番	商品説明文	ヒールのタイプ	留め具のタイプ	モデル年(発売年・発表年)	inner_material_type1	inner_material_type2	inner_material_type3	inner_material_type4	inner_material_type5	sole_material	性別	対象年齢層	靴サイズ表記単位（国）	対象年齢層(詳細)	靴サイズ表記単位	靴幅	靴サイズ	靴サイズの上限値	商品説明の箇条書き	商品説明の箇条書き	商品説明の箇条書き	商品説明の箇条書き	商品説明の箇条書き	検索キーワード	スタイルキーワード	首のタイプ	シーズン	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	コート・ワンピース・チュニック着丈	material_type	shaft_style_type	fit_type	ストラップのタイプ	つま先の形状(トゥシェープ)	ウエストのスタイル	袖のタイプ	シャツカラースタイル	ボトムススタイル	fit_to_size_description	opacity	靴の幅	シャフト(軸)の丈	カップサイズ	配送重量の単位	配送重量	inseam_length	inseam_length_unit_of_measure	sleeve_length	袖の長さの単位	waist_size	neck_size_unit_of_measure	ウエストサイズの単位	chest_size_unit_of_measure	chest_size	neck_size	シャフトの高さの測定単位	item_width	item_height	shoe_width_unit_of_measure	商品の寸法の単位	item_length	素材または繊維	この商品は電池本体ですか？または電池を使用した商品ですか？	電池付属	電池の組成	電池の種類、サイズ	電池の種類、サイズ	電池の種類、サイズ	電池の数	電池の数	電池の数	リチウムメタル電池単数	リチウムイオン電池単数	リチウム電池パッケージ	電池当たりのワット時	リチウム電池のエネルギー量測定単位	リチウム含有量(グラム)	リチウム電池の重量の測定単位	商品の重量	商品の重量の単位	体積	商品の容量の単位	原産国/地域	出荷作業日数	商品のコンディション	商品のコンディション説明	ポイント	セール時ポイント	商品の公開日	予約商品の販売開始日	メーカー希望価格	使用しない支払い方法	配送日時指定SKUリスト	セール価格	セール開始日	セール終了日	商品の入荷予定日	最大注文個数	ギフトメッセージ	ギフト包装	メーカー製造中止	配送パターン	販売形態(並行輸入品)	廃盤日	販売開始日	商品タックスコード	法人価格	数量割引のタイプ	数量の下限1	数量割引1	数量の下限2	数量割引2	数量の下限3	数量割引3	数量の下限4	数量割引4	数量の下限5	数量割引5	価格設定操作	unspsc_code	national_stock_number\n";
	// $head .= "feed_product_type	item_sku	brand_name	item_name	external_product_id	external_product_id_type	outer_material_type	recommended_browse_nodes	item_length_description	material_composition1	material_composition2	material_composition3	material_composition4	material_composition5	material_composition6	material_composition7	material_composition8	material_composition9	material_composition10	size_name	color_name	color_map	style_name	lifestyle1	lifestyle2	department_name	special_features1	special_features2	special_features3	special_features4	special_features5	size_map	band_size_num	band_size_num_unit_of_measure	is_adult_product	quantity	standard_price	item_package_quantity	main_image_url	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	swatch_image_url	parent_child	parent_sku	relationship_type	variation_theme	update_delete	part_number	product_description	heel_type	closure_type	model_year	inner_material_type1	inner_material_type2	inner_material_type3	inner_material_type4	inner_material_type5	sole_material	target_gender	age_range_description	footwear_size_system	footwear_age_group	footwear_size_class	footwear_width	footwear_size	footwear_to_size	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	generic_keywords	style_keywords	neck_style	seasons	platinum_keywords1	platinum_keywords2	platinum_keywords3	platinum_keywords4	platinum_keywords5	specific_uses_keywords1	specific_uses_keywords2	specific_uses_keywords3	specific_uses_keywords4	specific_uses_keywords5	material_type	shaft_style_type	fit_type	strap_type	toe_shape	waist_style	sleeve_type	collar_style	bottom_style	fit_to_size_description	opacity	shoe_width	shaft_height	cup_size	website_shipping_weight_unit_of_measure	website_shipping_weight	inseam_length	inseam_length_unit_of_measure	sleeve_length	sleeve_length_unit_of_measure	waist_size	neck_size_unit_of_measure	waist_size_unit_of_measure	chest_size_unit_of_measure	chest_size	neck_size	shaft_height_unit_of_measure	item_width	item_height	shoe_width_unit_of_measure	item_dimensions_unit_of_measure	item_length	fabric_type	batteries_required	are_batteries_included	battery_cell_composition	battery_type1	battery_type2	battery_type3	number_of_batteries1	number_of_batteries2	number_of_batteries3	number_of_lithium_metal_cells	number_of_lithium_ion_cells	lithium_battery_packaging	lithium_battery_energy_content	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight	lithium_battery_weight_unit_of_measure	item_weight	item_weight_unit_of_measure	item_volume	item_volume_unit_of_measure	country_of_origin	fulfillment_latency	condition_type	condition_note	standard_price_points	sale_price_points	product_site_launch_date	merchant_release_date	list_price	optional_payment_type_exclusion	delivery_schedule_group_id	sale_price	sale_from_date	sale_end_date	restock_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	is_discontinued_by_manufacturer	merchant_shipping_group_name	distribution_designation	offering_end_date	offering_start_date	product_tax_code	business_price	quantity_price_type	quantity_lower_bound1	quantity_price1	quantity_lower_bound2	quantity_price2	quantity_lower_bound3	quantity_price3	quantity_lower_bound4	quantity_price4	quantity_lower_bound5	quantity_price5	pricing_action	unspsc_code	national_stock_number\n";
	$head = "TemplateType=fptcustom	Version=2022.1208	Category=clothing	TemplateSignature=Q09BVCxUVU5JQyxTQUZFVFlfR0xBU1NFUyxTSE9FTEFDRSxPVVRFUldFQVIsU1dFQVRFUixORUNLVElFLFVOREVSUEFOVFMsSEFOREJBRyxDSElMRFJFTlNfQ09TVFVNRSxOSUdIVEdPV05fTklHSFRTSElSVCxTT0NLU0hPU0lFUlksRVRITklDX1dFQVIsQVBST04sQkFDS1BBQ0ssQkxBWkVSLFNIT0VfQUNDRVNTT1JZLFBST1RFQ1RJVkVfR0xPVkUsQVJNX1NMRUVWRSxCQUJZX0NPU1RVTUUsVEVDSE5JQ0FMX1NQT1JUX1NIT0UsU1VJVCxGT09EX0JJQixTV0lNV0VBUixBUFBBUkVMX0dMT1ZFUyxTT0NLUyxQQU5UUyxDQU1JU09MRSxDSEFOQ0hBTktPLEVBUk1VRkYsU1dFQVRTSElSVCxTVVNQRU5ERVIsU0tJUlQsQ09SU0VULFNIT1JUUyxMRU9UQVJELE9SQ0FfU0hJUlQsVU5ERVJXRUFSLFNMRUVQV0VBUixTQ0FSRixTQVJFRSxDSE9MSSxMRUhFTkdBX0NIT0xJX1NFVCxPQkksUEFKQU1BUyxLVVJUQSxVTkRFUkdBUk1FTlRfU0xJUCxTQUxXQVJfU1VJVF9TRVQsQURVTFRfQ09TVFVNRSxTSElSVCxLSU1PTk8sUk9CRSxUSUdIVFMsQkFHLFRPVEVfQkFHLEJPT1QsVFJBQ0tfU1VJVCxBUFBBUkVMX0JFTFQsWVVLQVRBLEJSQSxXQUlTVF9DSU5DSEVSLE9WRVJBTExTLERSRVNTLEJPRFlTVE9DS0lORyxLRVlDSEFJTixTQU5EQUwsQUNDRVNTT1JZLFZFU1QsSklOQkVJLEhBVCxTSE9FUw==	settings=attributeRow=3&contentLanguageTag=ja_JP&dataRow=4&feedType=113&headerLanguageTag=ja_JP&isEdit=false&isExpose=false&isProcessingSummary=false&labelRow=2&primaryMarketplaceId=amzn1.mp.o.A1VC38T7YXB528&templateIdentifier=9dab827a-a66f-43f8-9689-9d3148d0847a&timestamp=2022-12-08T00%3A00%3A49.563Z	     上3行はAmazon.comのみで使用します。上3行は変更または削除しないでください。																																																																																																																												画像									バリエーション							商品基本情報																		商品検索情報																																																																																																																																											推奨ブラウズノード別の情報																																				寸法																																																					出荷関連情報										コンプライアンス情報																																														出品情報																													\n"; 
	$head .= "商品タイプ	出品者SKU	ブランド名	商品名	商品コード(JANコード等)	商品コードのタイプ	モデル名	模範	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	部門名	素材構成	在庫数	商品の販売価格	商品メイン画像URL	性別	対象年齢層	アパレルサイズ表記単位(国)	アパレルサイズの表記方法	アパレルサイズの値	アパレルサイズ(最大レンジ)	アパレルサイズのボディタイプ	アパレルサイズの身長タイプ	表地素材	サイズ	カラー	カラーマップ	サイズマップ	アダルト商品	推奨ブラウズノード	ユニット数	ユニット数のタイプ	この商品はAmazon.co.jp限定商品ですか？	商品に適用される危険物規制の種類	商品に適用される危険物規制の種類	商品に適用される危険物規制の種類	商品説明文	メーカー型番	closure_type	manufacturer	スタイル名	種類	パッケージ商品数	care_instructions	品目の前から後ろまでの奥行き	品目の奥行きの単位	品目の左右の幅	品目の幅単位	底部から上部までの高さ	品目の高さ単位	付属品総数	靴サイズ表記単位（国）	対象年齢層(詳細)	靴サイズ表記単位	靴サイズ	シェイプウェアのサイズ システム	シェイプウェアのサイズ クラス	シェイプウェアのサイズ値	シェイプウェアのサイズ範囲	バンドサイズの値	バンドサイズ(最大レンジ)	カップサイズの値	シェイプウェア ボディタイプ	シェイプウェアの身長タイプ	ボトムスサイズシステム	ボトムスサイズクラス	ボトムスサイズ値	ボトムスのサイズ範囲	ウエストサイズ値	股下サイズ値	ボトムス体型タイプ	ボトムス身長タイプ	スカートサイズシステム	スカートサイズクラス	スカートサイズ値	スカートのサイズ範囲	ウエストサイズ値	スカート体型タイプ	スカート身長タイプ	シャツサイズシステム	シャツサイズクラス	シャツサイズ値	シャツのサイズ範囲	首回りサイズ値	裄丈サイズ値	シャツ体型タイプ	シャツ身長タイプ	水平方向の長端の長さ	長さ単位	水平方向の短い方の端の幅	幅単位	底部から上部までの高さ	高さの単位	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	機能性	バンドサイズ	バンドサイズの単位	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	ライフスタイル	首のタイプ	長さ	素材構成	帽子サイズ表記単位(国) 	帽子サイズの表記方法	帽子サイズの値	帽子のサイズ範囲	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	カラーサンプル画像URL	親子関係の指定	親商品のSKU(商品管理番号)	子関係タイプ	バリエーション種類	パッケージレベル	パッケージ内包数量	パッケージには属性が含まれています	アップデート・削除	自社で管理しているSKUごとの番号	ヒールのタイプ	モデル年(発売年・発表年)	インナーの素材	バーコード適用除外理由	版	ソール素材	ヒールの高さの単位	ヒールの高さ	footwear_gender	footwear_width_unisex	footwear_to_size_unisex	footwear_gender_unisex	footwear_size_unisex	ヒール高	靴幅	靴サイズの上限値	検索キーワード	サイン入り商品	行事	行事	行事	行事	行事	スポーツタイプ	スポーツタイプ	スポーツタイプ	プレーヤー名	チーム名	コレクション名	素材の種類	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	毛皮の説明	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	襟のタイプ	織り方	ポケット記述	テーマ	ライニングの説明	リーグ名	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	特殊サイズタイプ	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	パターン名	オーダー締切日	スタイルキーワード	スタイルキーワード	スタイルキーワード	シーズン	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	シャフトスタイルタイプ	UV加工	要組み立て	レンズのコーティング	レンズ色	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	箱数	同梱商品	同梱商品	同梱商品	同梱商品	同梱商品	商品の個数（ピース数）	耐水レベル	シャーシ素材	バッグストラップの高さ	ショルダーストラップドロップの測定単位	防汚	ラベルに記載されている国/地域	パターンのスタイル	出品者カタログ番号	メーカー対象最高年齢	組み立て時間の単位	assembly_time	thesaurus_attribute_keywords1	thesaurus_attribute_keywords2	thesaurus_attribute_keywords3	thesaurus_attribute_keywords4	thesaurus_attribute_keywords5	top_style	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	メーカー対象最低年齢	推奨使用用途	キャスター数	正味個数単位	防カビ加工	防カビ加工	防カビ加工	防カビ加工	防カビ加工	正味個数	利き手	股上のスタイル	フロントスタイル	ノイズ制御	ノイズ減衰単位	ノイズ減衰値	クラスタイプ	装飾機能	装飾機能	装飾機能	装飾機能	装飾機能	背面のスタイル	ハンドル素材	シャフトの円周	シャフトの円周の単位	つま先のスタイル	ベルトの種類	抗菌加工	商品の利点	ワット数の単位	グリップタイプ	ワット数	土踏まずの特徴	キャラクター	袖のタイプ	ファブリックウォッシュ	ストラップのタイプ	つま先の形状(トゥシェープ)	ウエストスタイル	不透明度	ボトムススタイル	収納可能サイズ	サブブランド	内蔵ライト	表示サイズ	underwire_type	opacity_unit_of_measure	leg_style	mfg_maximum	プレーヤー数	mfg_minimum	ポケットの数	正味容積単位	正味重量単位	正味の長さ	正味の長さの単位	正味面積	正味重量	正味面積単位	正味容積	ジャケットの長さ	バンドサイズ	ベルト幅の単位	ベルト幅	最低広告表示価格	ベルト幅の十進数値	bra_padding_level	梱包タイプ	つや消しスタイル	商品の幅の単位	チェストサイズの単位	チェストサイズ	商品の高さの単位	えきたいりょう	液量単位	商品の長さの単位	靴の幅	シャフト(軸)の丈	ウエストサイズの単位	ウエストのサイズ	仕立ての長さの単位	股下の長さ	袖の長さの単位	袖の長さ	首のサイズの単位	首のサイズ	配送重量の単位	配送重量	シャフトの高さの測定単位	項目幅	項目の高さ	靴幅	品目の長さ	商品の重量（パッケージを含まない）	商品の重量の単位	商品の寸法の単位	カップサイズ	長さ	商品の長さの単位	幅	商品の高さの単位	商品の幅の単位	容量の単位	商品の高さ	ディスプレイサイズ	ディスプレイ最大解像度の単位	性能・容量	leg_diameter_unit_of_measure	推奨最大重量の単位	メーカー推奨最大重量	leg_diameter	商品の形状	ストレージ容量の測定単位	ストレージボリューム	商品の形状	高さマップ	股上の高さ	股上の高さの単位	伸びた長さの測定単位	拡張された長さ	minimum_neck_size_unit_of_measure	ヘッドサイズ名	商品パッケージの長さの単位	長さ	幅	高さ	商品パッケージの重量の単位	商品パッケージの重量	フルフィルメントセンターID	商品パッケージの高さの単位	商品パッケージの幅の単位	商品パッケージの寸法の単位	この商品は電池本体ですか？または電池を使用した商品ですか？	電池付属	電池の組成	電池の種類、サイズ	電池の種類、サイズ	電池の種類、サイズ	電池の数	電池の数	電池の数	電池の重量(グラム)	電池の重量の測定単位	リチウムメタル電池単数	リチウムイオン電池単数	リチウム電池パッケージ	電池当たりのワット時	リチウム電池のエネルギー量測定単位	リチウム含有量(グラム)	リチウム電池の重量の測定単位	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	国連(UN)番号	安全データシート(SDS) URL	商品の重量	商品の重量の単位	商品の体積	商品の容量の単位	原産国/地域	商品に適用される保管規制の区分	引火点(°C)	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	内用液は含まれていますか?	メーカー保証の説明	CPSIA警告の説明	並行輸入品	CPSIA警告	CPSIA警告	CPSIA警告	CPSIA警告	法規上の免責条項	安全上のお知らせ	external_testing_certification	出荷作業日数	コンディション	取り寄せ	商品のコンディション説明	通貨コード	予約商品の販売開始日	メーカー希望価格	使用しない支払い方法	セール価格	セール開始日	セール終了日	商品の入荷予定日	最大注文個数	ギフトメッセージ	ギフト包装	配送パターン	地図価格	販売形態(並行輸入品)	終了日を提案する	開始日を提案する	商品タックスコード	ポイントパーセント	セール時ポイントパーセント	商品の公開日	配送日時指定SKUリスト	メーカー製造中止	同梱発送時の最大個数	メーカー希望価格	ポイント	セール時ポイント\n";
	$head .="feed_product_type	item_sku	brand_name	item_name	external_product_id	external_product_id_type	model_name	model	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	department_name	fabric_type	quantity	standard_price	main_image_url	target_gender	age_range_description	apparel_size_system	apparel_size_class	apparel_size	apparel_size_to	apparel_body_type	apparel_height_type	outer_material_type	size_name	color_name	color_map	size_map	is_adult_product	recommended_browse_nodes	unit_count	unit_count_type	is_exclusive_product	supplier_declared_material_regulation1	supplier_declared_material_regulation2	supplier_declared_material_regulation3	product_description	part_number	closure_type	manufacturer	style_name	item_type_name	item_package_quantity	care_instructions	depth_front_to_back	depth_front_to_back_unit_of_measure	depth_width_side_to_side	depth_width_side_to_side_unit_of_measure	depth_height_floor_to_top	depth_height_floor_to_top_unit_of_measure	number_of_items	footwear_size_system	footwear_age_group	footwear_size_class	footwear_size	shapewear_size_system	shapewear_size_class	shapewear_size	shapewear_size_to	shapewear_band_size	shapewear_band_size_to	shapewear_cup_size	shapewear_body_type	shapewear_height_type	bottoms_size_system	bottoms_size_class	bottoms_size	bottoms_size_to	bottoms_waist_size	bottoms_inseam_size	bottoms_body_type	bottoms_height_type	skirt_size_system	skirt_size_class	skirt_size	skirt_size_to	skirt_waist_size	skirt_body_type	skirt_height_type	shirt_size_system	shirt_size_class	shirt_size	shirt_size_to	shirt_neck_size	shirt_sleeve_length	shirt_body_type	shirt_height_type	length_head_to_toe	length_head_to_toe_unit_of_measure	length_width_side_to_side	length_width_side_to_side_unit_of_measure	length_height_floor_to_top	length_height_floor_to_top_unit_of_measure	special_features1	special_features2	special_features3	special_features4	special_features5	special_features6	special_features7	special_features8	special_features9	special_features10	special_features11	special_features12	special_features13	special_features14	band_size_num	band_size_num_unit_of_measure	lifestyle1	lifestyle2	lifestyle3	lifestyle4	lifestyle5	lifestyle6	lifestyle7	lifestyle8	lifestyle9	lifestyle10	neck_style	item_length_description	material_composition	headwear_size_system	headwear_size_class	headwear_size	headwear_size_to	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	swatch_image_url	parent_child	parent_sku	relationship_type	variation_theme	package_level	package_contains_quantity	package_contains_identifier	update_delete	codabar	heel_type	model_year	inner_material_type	gtin_exemption_reason	edition	sole_material	heel_height_unit_of_measure	heel_height	footwear_gender	footwear_width_unisex	footwear_to_size_unisex	footwear_gender_unisex	footwear_size_unisex	heel_height_string	footwear_width	footwear_to_size	generic_keywords	is_autographed	occasion_type1	occasion_type2	occasion_type3	occasion_type4	occasion_type5	sport_type1	sport_type2	sport_type3	athlete	team_name	collection_name	material_type	target_audience_keywords1	target_audience_keywords2	target_audience_keywords3	target_audience_keywords4	target_audience_keywords5	fur_description	fit_type1	fit_type2	fit_type3	fit_type4	fit_type5	collar_style	weave_type	pocket_description	theme	lining_description	league_name	special_size_type1	special_size_type2	special_size_type3	special_size_type4	special_size_type5	special_size_type6	special_size_type7	special_size_type8	special_size_type9	special_size_type10	lifecycle_supply_type1	lifecycle_supply_type2	lifecycle_supply_type3	lifecycle_supply_type4	lifecycle_supply_type5	pattern_name	item_booking_date	style_keywords1	style_keywords2	style_keywords3	seasons	platinum_keywords1	platinum_keywords2	platinum_keywords3	platinum_keywords4	platinum_keywords5	specific_uses_keywords1	specific_uses_keywords2	specific_uses_keywords3	specific_uses_keywords4	specific_uses_keywords5	shaft_style_type	ultraviolet_light_protection	is_assembly_required	lens_coating_description	lens_color	compatible_with_vehicle_type1	compatible_with_vehicle_type2	compatible_with_vehicle_type3	compatible_with_vehicle_type4	compatible_with_vehicle_type5	compatible_with_vehicle_type6	compatible_with_vehicle_type7	compatible_with_vehicle_type8	compatible_with_vehicle_type9	number_of_boxes	included_components1	included_components2	included_components3	included_components4	included_components5	number_of_pieces	water_resistance_level	frame_material_type	shoulder_strap_drop	shoulder_strap_drop_unit_of_measure	is_stain_resistant	country_as_labeled	pattern_type	catalog_number	mfg_maximum_unit_of_measure	assembly_time_unit_of_measure	assembly_time	thesaurus_attribute_keywords1	thesaurus_attribute_keywords2	thesaurus_attribute_keywords3	thesaurus_attribute_keywords4	thesaurus_attribute_keywords5	top_style	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	mfg_minimum_unit_of_measure	recommended_uses_for_product	number_of_wheels	net_content_count_unit_of_measure	material_type_free1	material_type_free2	material_type_free3	material_type_free4	material_type_free5	net_content_count	hand_orientation	rise_style	front_style	noise_control	noise_attenuation_unit_of_measure	noise_attenuation	clasp_type	embellishment_feature1	embellishment_feature2	embellishment_feature3	embellishment_feature4	embellishment_feature5	back_style	handle_material	shaft_circumference	shaft_circumference_unit_of_measure	toe_style	belt_style	item_styling	product_benefit	wattage_unit_of_measure	grip_type	wattage	arch_type	subject_character	sleeve_type	fabric_wash	strap_type	toe_shape	waist_style	opacity	bottom_style	fit_to_size_description	sub_brand_name	has_builtin_light	size_info_display_name	underwire_type	opacity_unit_of_measure	leg_style	mfg_maximum	number_of_players	mfg_minimum	number_of_pockets	net_content_volume_unit_of_measure	net_content_weight_unit_of_measure	net_content_length	net_content_length_unit_of_measure	net_content_area	net_content_weight	net_content_area_unit_of_measure	net_content_volume	jacket_length	band_size	belt_width_unit_of_measure	belt_width_string	map_policy	belt_width_derived	bra_padding_level	customer_package_type	matte_style	item_width_unit_of_measure	chest_size_unit_of_measure	chest_size	item_height_unit_of_measure	liquid_volume	liquid_volume_unit_of_measure	item_length_unit_of_measure	shoe_width	shaft_height	waist_size_unit_of_measure	waist_size	inseam_length_unit_of_measure	inseam_length	sleeve_length_unit_of_measure	sleeve_length	neck_size_unit_of_measure	neck_size	website_shipping_weight_unit_of_measure	website_shipping_weight	shaft_height_unit_of_measure	item_width	item_height	shoe_width_unit_of_measure	item_length	item_display_weight	item_display_weight_unit_of_measure	item_dimensions_unit_of_measure	cup_size	item_display_length	item_display_length_unit_of_measure	item_display_width	item_display_height_unit_of_measure	item_display_width_unit_of_measure	capacity_unit_of_measure	item_display_height	display_size	display_size_unit_of_measure	capacity	leg_diameter_unit_of_measure	maximum_weight_recommendation_unit_of_measure	maximum_weight_recommendation	leg_diameter	item_shape	storage_volume_unit_of_measure	storage_volume	item_form	height_map	rise_height	rise_height_unit_of_measure	extended_length_unit_of_measure	extended_length	minimum_neck_size_unit_of_measure	head_size_name	package_length_unit_of_measure	package_length	package_width	package_height	package_weight_unit_of_measure	package_weight	fulfillment_center_id	package_height_unit_of_measure	package_width_unit_of_measure	package_dimensions_unit_of_measure	batteries_required	are_batteries_included	battery_cell_composition	battery_type1	battery_type2	battery_type3	number_of_batteries1	number_of_batteries2	number_of_batteries3	battery_weight	battery_weight_unit_of_measure	number_of_lithium_metal_cells	number_of_lithium_ion_cells	lithium_battery_packaging	lithium_battery_energy_content	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight	lithium_battery_weight_unit_of_measure	supplier_declared_dg_hz_regulation1	supplier_declared_dg_hz_regulation2	supplier_declared_dg_hz_regulation3	supplier_declared_dg_hz_regulation4	supplier_declared_dg_hz_regulation5	hazmat_united_nations_regulatory_id	safety_data_sheet_url	item_weight	item_weight_unit_of_measure	item_volume	item_volume_unit_of_measure	country_of_origin	fulfillment_center_special_storage	flash_point	ghs_classification_class1	ghs_classification_class2	ghs_classification_class3	contains_liquid_contents	warranty_description	cpsia_cautionary_description	import_designation	cpsia_cautionary_statement1	cpsia_cautionary_statement2	cpsia_cautionary_statement3	cpsia_cautionary_statement4	legal_disclaimer_description	safety_warning	external_testing_certification	fulfillment_latency	condition_type	is_sourcing_on_demand	condition_note	currency	merchant_release_date	list_price	optional_payment_type_exclusion	sale_price	sale_from_date	sale_end_date	restock_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	merchant_shipping_group_name	map_price	distribution_designation	offering_end_date	offering_start_date	product_tax_code	standard_price_points_percent	sale_price_points_percent	product_site_launch_date	delivery_schedule_group_id	is_discontinued_by_manufacturer	max_aggregate_ship_quantity	list_price_with_tax	standard_price_points	sale_price_points\n"; 
	//	del ohkawara 2019/12/12
	//$head  = "TemplateType=clothing	Version=2017.1102	上3行は Amazon.com 記入用です。上3行は変更または削除しないでください。.								出品情報-商品をサイト上で販売可能にする際に必要な項目																						寸法-商品のサイズや重量を入力する項目		商品検索情報-サーチ上で商品を検索されやすくするために必要な項目									画像-商品画像を表示させるために必要な項目。詳しくは画像説明タブを参照										出荷関連情報-フルフィルメント by Amazon (FBA) の利用、あるいは自社出荷の注文に関する出荷関連情報を、この項目に記入してください。FBA を利用する場合には必須の項目。							バリエーション-商品の色・サイズなどのバリエーションを作成する際に必須の項目				ProductTypeによって必須となる項目																																				\n";

	//$head .= "出品者SKU	商品名	商品コード(JANコード等)	商品コードのタイプ	ブランド名	商品タイプ	メーカー型番	商品説明文	アップデート・削除	自社で管理しているSKUごとの番号	在庫数	出荷作業日数	商品のコンディション	商品のコンディション説明	商品の販売価格	ポイント	商品の公開日	予約商品の販売開始日	メーカー希望価格	使用しない支払い方法	配送日時指定SKUリスト	セール価格	セール時ポイント	セール開始日	セール終了日	商品の入荷予定日	最大注文個数	ギフトメッセージ	ギフト包装	メーカー製造中止	配送パターン	販売形態(並行輸入品)	商品タックスコード	配送重量の単位	配送重量	商品説明の箇条書き1	商品説明の箇条書き2	商品説明の箇条書き3	商品説明の箇条書き4	商品説明の箇条書き5	検索キーワード	推奨ブラウズノード	スタイルキーワード	コート・ワンピース・チュニック着丈	商品メイン画像URL	カラーサンプル画像URL	商品のサブ画像URL11	商品のサブ画像URL12	商品のサブ画像URL13	商品のサブ画像URL14	商品のサブ画像URL15	商品のサブ画像URL16	商品のサブ画像URL17	商品のサブ画像URL18	商品パッケージの長さの単位	商品パッケージの長さ	商品パッケージの幅	商品パッケージの高さ	商品パッケージの重量の単位	商品パッケージの重量	フルフィルメントセンターID	親子関係の指定	親商品のSKU(商品管理番号)	親子関係のタイプ	バリエーションテーマ	サイズ	サイズマップ	カラー	カラーマップ	スタイル名	靴の幅	ストラップのタイプ	つま先の形状(トゥシェープ)	ウエストのスタイル	素材不透明度	ヒールのタイプ	シャフト(軸)の丈	表地素材	袖のタイプ	留め具のタイプ	ライフスタイル1	ライフスタイル2	素材または繊維	対象年齢・性別	アダルト商品	ウエストサイズの単位	ウエストサイズ	仕立ての長さの単位	仕立ての長さ	袖の長さの単位	袖の長さ	シャツカラースタイル	首のタイプ	首のサイズの単位	首のサイズ	ボトムススタイル	胸囲サイズの単位	胸囲サイズ	モデル年(発売年・発表年)	シーズン	収納可能サイズ	カップサイズ\n";

	//$head .= "item_sku	item_name	external_product_id	external_product_id_type	brand_name	product_subtype	part_number	product_description	update_delete	codabar	quantity	fulfillment_latency	condition_type	condition_note	standard_price	standard_price_points	product_site_launch_date	merchant_release_date	list_price	optional_payment_type_exclusion	delivery_schedule_group_id	sale_price	sale_price_points	sale_from_date	sale_end_date	restock_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	is_discontinued_by_manufacturer	merchant_shipping_group_name	distribution_designation	product_tax_code	website_shipping_weight_unit_of_measure	website_shipping_weight	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	generic_keywords	recommended_browse_nodes	style_keywords	specific_uses_keywords	main_image_url	swatch_image_url	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	package_length_unit_of_measure	package_length	package_width	package_height	package_weight_unit_of_measure	package_weight	fulfillment_center_id	parent_child	parent_sku	relationship_type	variation_theme	size_name	size_map	color_name	color_map	style_name	shoe_width	strap_type	toe_shape	waist_style	opacity	heel_type	shaft_height	outer_material_type	sleeve_type	closure_type	lifestyle1	lifestyle2	fabric_type	department_name	is_adult_product	waist_size_unit_of_measure	waist_size	inseam_length_unit_of_measure	inseam_length	sleeve_length_unit_of_measure	sleeve_length	collar_style	neck_style	neck_size_unit_of_measure	neck_size	bottom_style	chest_size_unit_of_measure	chest_size	model_year	seasons	fit_to_size_description	cup_size\n";

	return $head;
}


//	スポーツ用ヘッダ
function amazon_tsv_head2() {

	// $head  = "TemplateType=sports	Version=2017.0109	上3行は Amazon.com 記入用です。上3行は変更または削除しないでください。.								出品情報-商品をサイト上で販売可能にする際に必要な項目																						寸法-商品のサイズや重量を入力する項目													商品検索情報-サーチ上で商品を検索されやすくするために必要な項目											画像-商品画像を表示させるために必要な項目。詳しくは画像説明タブを参照										出荷関連情報-フルフィルメント by Amazon (FBA) の利用、あるいは自社出荷の注文に関する出荷関連情報を、この項目に記入してください。FBA を利用する場合には必須の項目。							バリエーション-商品の色・サイズなどのバリエーションを作成する際に必須の項目				コンプライアンス情報-商品を販売する国または地域の特定商取引に遵守するために利用される項目			ProductTypeによって必須となる項目																																								\n";

	// $head .= "出品者SKU	商品コード(JANコード等)	商品コードのタイプ	商品名	商品説明文	メーカー名	メーカー型番	商品タイプ	ブランド名	アップデート・削除	商品の販売価格	ポイント	商品のコンディション	商品のコンディション説明	在庫数	メーカー希望価格	商品の公開日	予約商品の販売開始日	商品の入荷予定日	リードタイム(出荷までにかかる作業日数)	使用しない支払い方法	セール価格	セール時ポイント	配送日時指定SKUリスト	セール開始日	セール終了日	最大注文個数	ギフトメッセージ	ギフト包装	メーカー製造中止	配送パターン	販売形態(並行輸入品)	配送重量	配送重量の単位	ラインの巻き容量	リールの直径	リールの直径の単位	高さ	高さの単位	ロッド(棒)の長さ	ロッド(棒)の長さの単位	幅	幅の単位	重量	ロッド(棒)の重量の単位	商品の仕様1	商品の仕様2	商品の仕様3	商品の仕様4	商品の仕様5	推奨ブラウズノード	検索キーワード	対象1	対象2	スタイルキーワード	対象	商品メイン画像URL	カラーサンプル画像URL	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	フルフィルメントセンターID	商品パッケージの高さ	商品パッケージの幅	商品パッケージの長さ	商品パッケージの長さの単位	商品パッケージの重量	商品パッケージの重量の単位	親子関係の指定	親商品のSKU(商品管理番号)	親子関係のタイプ	バリエーションテーマ	安全性に関する注意	原産国	法規上の免責条項	保証	スタイル	カラー	カラーマップ	デザイン	素材	サイズマップ	サイズ	ゴルフクラブのシャフトの硬さ	ゴルフグラブのロフト（クラブフェースの傾斜角度）	グリップのサイズ	グリップのタイプ	グリップの素材タイプ	バイクのリムサイズ	バイクのリムサイズの単位	ホイールサイズ	ホイールサイズの単位	張力（テンション）	釣り糸素材のタイプ	最大の高さ（ピーク）　	容量の単位	レンズの色	ハンド	シャフト(軸)の長さ	シャフト(軸)の長さの単位	シャフト(軸)の素材	形状	対応重量	対応重量の単位	チーム名	魚種	電池付属	電池のタイプ	付属バッテリ個数	リチウム電池エネルギー含有量	リチウム電池パッケージ仕様	リチウム電池ボルト数	リチウム電池重量	配送オプション	席種	オプション\n";

	// $head .= "item_sku	external_product_id	external_product_id_type	item_name	product_description	manufacturer	part_number	feed_product_type	brand_name	update_delete	standard_price	standard_price_points	condition_type	condition_note	quantity	list_price	product_site_launch_date	merchant_release_date	restock_date	fulfillment_latency	optional_payment_type_exclusion	sale_price	sale_price_points	delivery_schedule_group_id	sale_from_date	sale_end_date	max_order_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	is_discontinued_by_manufacturer	merchant_shipping_group_name	distribution_designation	website_shipping_weight	website_shipping_weight_unit_of_measure	volume_capacity_name	item_display_diameter	item_display_diameter_unit_of_measure	item_display_height	item_display_height_unit_of_measure	item_display_length	item_display_length_unit_of_measure	item_display_width	item_display_width_unit_of_measure	item_display_weight	item_display_weight_unit_of_measure	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	recommended_browse_nodes	generic_keywords	target_audience_keywords1	target_audience_keywords2	style_keywords	target_audience_base	main_image_url	swatch_image_url	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	fulfillment_center_id	package_height	package_width	package_length	package_length_unit_of_measure	package_weight	package_weight_unit_of_measure	parent_child	parent_sku	relationship_type	variation_theme	safety_warning	country_of_origin	legal_disclaimer_description	mfg_warranty_description_type	style_name	color_name	color_map	pattern_name	material_type	size_map	size_name	golf_club_flex	golf_club_loft	grip_size	grip_type	grip_material_type	rim_size	rim_size_unit_of_measure	wheel_size	wheel_size_unit_of_measure	tension_level	fishing_line_type	capacity	capacity_unit_of_measure	lens_color	hand_orientation	shaft_length	shaft_length_unit_of_measure	shaft_material	item_shape	display_maximum_weight_recommendation	display_maximum_weight_recommendation_unit_of_measure	team_name	specific_uses_for_product	are_batteries_included	battery_type	number_of_batteries	lithium_battery_energy_content	lithium_battery_packaging	lithium_battery_voltage	lithium_battery_weight	shipping_options	seating_group	option_type\n";

	$head ="TemplateType=fptcustom	Version=2022.1208	Category=sports	TemplateSignature=QklPTUVUUklDX01PTklUT1IsU1BPUlRfTUlUVCxCSUNZQ0xFX1RSQUlORVIsR09MRl9DTFVCX1BVVFRFUixVVElMSVRZX0hPTFNURVJfUE9VQ0gsRklUTkVTU19CRU5DSCxTUE9SVF9SQUNLRVQsU05PV1NIT0UsQUlNSU5HX1NDT1BFX1NJR0hULFNQT1JUX0JJTkRJTkcsU1BPUlRJTkdfR09PRFMsRklTSElOR19MSU5FLEdPTEZfQ0xVQl9XRURHRSxTUE9SVF9BQ1RJVklUWV9HTE9WRSxCQVJCRUxMLEZJU0hJTkdfRVFVSVBNRU5ULFdBVEVSX0ZMT1RBVElPTl9ERVZJQ0UsU05PV0JPQVJEX0JPT1QsR09MRl9DTFVCX0JBRyxTUE9SVF9XRVRTVUlULFJFQ1JFQVRJT05fQkFMTCxTTEVFUElOR19CQUcsQUlSX0dVTixHT0xGX0NMVUJfSVJPTixCQVNLRVRCQUxMX0hPT1AsQUJET01JTkFMX0VYRVJDSVNFUixFWEVSQ0lTRV9NQVQsU0xFRVBJTkdfTUFULFNUQVRJT05BUllfQklDWUNMRSxTUE9SVF9UQUJMRV9HQU1FLEdMT1ZFUyxST1dJTkdfTUFDSElORSxNRVNTX0tJVCxHT0xGX0NMVUJfV09PRCxHVU5fSE9MU1RFUixDWUNMSU5HX1RJUkUsR09MRl9DTFVCX0hZQlJJRCxXQUxLSU5HX1NUSUNLLFRFTlQsSlVNUF9ST1BFLEVYRVJDSVNFX0JBTkQsS0lDS19TQ09PVEVSLFJPTExFUl9TS0FURSxGSVNISU5HX0hPT0ssSUNFX0NIRVNULFNXRUFUQkFORCxDQU1QSU5HX0VRVUlQTUVOVCxDWUNMSU5HX0VRVUlQTUVOVCxUUkFNUE9MSU5FLEdPTEZfQ0xVQixCT1hJTkdfR0xPVkUsRklTSElOR19SRUVMLFNLQVRFQk9BUkQsVFJFQURNSUxMLFBVTkNISU5HX0JBRyxBSVJfUFVNUCxTUE9SVF9IRUxNRVQ=	settings=attributeRow=3&contentLanguageTag=ja_JP&dataRow=4&feedType=113&headerLanguageTag=ja_JP&isEdit=false&isExpose=false&isProcessingSummary=false&labelRow=2&primaryMarketplaceId=amzn1.mp.o.A1VC38T7YXB528&templateIdentifier=d7c254e1-fd65-4aaf-b2f8-89022f05935f&timestamp=2022-12-08T00%3A01%3A32.927Z	     上3行はAmazon.comのみで使用します。上3行は変更または削除しないでください。																																												画像									バリエーション							商品基本情報																					商品検索情報																																																																																																																																																																																																																														推奨ブラウズノード別の情報																																																																															寸法																																																																																																																出荷関連情報										コンプライアンス情報																																																									出品情報																															\n";
	$head .= "商品タイプ	出品者SKU	商品コード(JANコード等)	商品コードのタイプ	商品名	対象年齢・性別	商品の販売価格	在庫数	商品メイン画像URL	ブランド名	メーカー名	推奨ブラウズノード	前から後ろまでのアイテムの奥行き	品目の奥行きの単位	品目の左右の幅	品目の幅単位	品目の底部から上部までの高さ	品目の高さ単位	お客様の性別	unit_count	unit_count_type	お客様の年齢層	サイズ	品目の上端から下端までの長さ	品目の長さ単位	品目の左右の幅	幅単位	カラー	賞味期限管理商品	品目の上端から下端までの長さ	品目の長さ単位	品目の左右の幅	品目の幅単位	品目の厚さ	品目の厚さ単位	組み立て時の長辺の長さ	長さ単位	組み立て時の短辺の幅	幅単位	組み立て時のベースからトップまでの高さ	高さの単位	商品の直径	商品の直径の単位	item_width_numeric	item_width_numeric_unit_of_measure	品目の左右の幅	品目の幅単位	底部から上部までの高さ	品目の高さ単位	商品のサブ画像URL1	商品のサブ画像URL2	商品のサブ画像URL3	商品のサブ画像URL4	商品のサブ画像URL5	商品のサブ画像URL6	商品のサブ画像URL7	商品のサブ画像URL8	カラーサンプル画像URL	親子関係の指定	親商品のSKU(商品管理番号)	子関係タイプ	バリエーションテーマ	パッケージレベル	パッケージ内包数量	パッケージには属性が含まれています	アップデート・削除	商品説明文	メーカー型番	リールの型式名	モデル	バーコード適用除外理由	自社で管理しているSKUごとの番号	発売年・モデルイヤー	梱包タイプ	表地素材	インナーの素材	使用する燃料タイプ	ヒールのタイプ	ヒールの高さの単位	ソール材料	ヒールの高さ	温度の定格	お手入れ方法	お手入れ方法	お手入れ方法	お手入れ方法	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	商品の仕様	スタイル名	検索キーワード	カラーマップ	素材の種類	シーズン	シーズン	シーズン	シーズン	スポーツの名称	スポーツの名称	ゴルフグラブのロフト（クラブフェースの傾斜角度）	グリップタイプ	レンズの色	レンズの素材	サイン入り商品	リーグ名	ハンド	シャフト(軸)の素材	プレーヤー名	チーム名	箱の中身	箱の中身	箱の中身	箱の中身	箱の中身	互換デバイス	互換デバイス	互換デバイス	互換デバイス	互換デバイス	毛皮の説明	golf_club_loft_unit_of_measure	席種	商品種別	オプション	対象	この商品はAmazon.co.jp限定商品ですか？	箱数	グローブのタイプ	防水タイプか否か	ライニングの説明	BMXバイクのタイプ	フレームの素材	設定数	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	ターゲットユーザーのキーワード	デザイン	スタイルキーワード	シャフトスタイルタイプ	リチウム電池ボルト数	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	プラチナキーワード	lithium_battery_voltage_unit_of_measure	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	ゴルフパターのライ角の単位	メーカー対象最高年齢	商品のオフセットの単位	オフセット	パターのライ角を指定する	パターのライ角を指定する	パターのライ角を指定する	パターのライ角を指定する	パターのライ角を指定する	メーカー対象最低年齢	ポケットの説明	ウェイトベンチの種類	対応スキルレベル	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	フィットタイプ	床の使用面積	首周りの形状(襟の形状)	パターンのスタイル	靴下の長さ	テーマ	トップのスタイル	UVプロテクションのレベル	ホイールタイプ	フレームタイプ	ブレーキのタイプ	サスペンションのタイプ	ドアの数	詰め物素材のタイプ	商品の据え付けタイプ	保温レベル	レンズの形状	絶縁タイプ	パッドのタイプ	ポール数	電球タイプ	座席数	電源	機能性	機能性	機能性	機能性	機能性	ラップトップ容量	バッテリ寿命	バッテリ寿命	バッテリ寿命	magnification_strength	wheelbase	wheelbase_unit_of_measure	shell_type	thesaurus_attribute_keywords	推奨使用用途	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	特定用途キーワード	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	magnification_strength_unit_of_measure	ラベルに記載されている国/地域	釣り技法	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	商品の補充情報	床の使用面積の単位	測定単位	バッテリ平均持続時間の単位	バッテリ平均持続時間の単位	バッテリ平均持続時間の単位	シャフトの円周	シャフトの円周の単位	つま先のスタイル	ミッドソール素材	仕切りの数	銃器の口径	エアガンパワータイプ	エアガンパワータイプ	エアガンパワータイプ	エアガンパワータイプ	エアガンパワータイプ	絶縁抵抗の単位	BTU で測定した絶縁	組み立て方法	ディスプレイタイプ	運動器具のアームタイプ	スペース効率	スペース効率	心拍数センサーのタイプ	商品の個数（ピース数）	溝深さ単位	溝深さ	ロックタイプ	ハンドル素材	収納時サイズ	ポール素材タイプ	ポール素材タイプ	ポール素材タイプ	ポール素材タイプ	ポール素材タイプ	ハンドルの直径	ハンドル長さの単位	ハンドル長さの十進数値	ハンドルのタイプ	ホイール素材	ホイールの数	対象となる動植物	対象となる動植物	対象となる動植物	対象となる動植物	対象となる動植物	チャンバーの深さの単位	チャンバーの深さ	バンド長	バンド材質タイプ	バンド色	バンド長の単位	速度測定	プログラム数	モーターサイズ	最大馬力単位	最低速度の単位	組み立てる必要がありますか?	最大速度の単位	最大速度	組み立て時間の単位	電源プラグ	組み立て時間	最低速度	動作モード	気流量	空気流量の測定単位	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	適合車両の種類	ゴルフクラブのシャフトの硬さ	張力（テンション）	代金引換	FCの保存可能期間の単位	配送オプション	コンビニ決済	レザーの種類	標準抵抗メカニズムの値	グリップ素材タイプ	釣り糸素材のタイプ	軸径の十進数値	軸径の単位	シャフト素材タイプ	メーカー推奨最高年齢	構成	メーカー推奨最少年齢	ポケットの数	Item Depth Unit Of Measure	スラット付きフレーム調整タイプ	調節可能な長さ	ストラップのタイプ	構造	ボトムのタイプ	脚のスタイル	袖のタイプ	サポートタイプ	線の太さ	速度の数	レベル数	装具タイプ	ギアループの数	最小胴体適合サイズ	就寝人数	最大倍率	使用容量の単位	台座タイプ	反射技術	base_material_type	仕上げ	仕上げ	シャフト長（10進値）	取付穴の単位	取り付け穴の直径の十進数値	ライフベストのタイプ	インソール素材	防しわ性	防しわ性	防しわ性	防しわ性	防しわ性	legal_compliance_certification_certifying_authority_name	legal_compliance_certification_geographic_jurisdiction	ラウンド数	バレル素材タイプ	品種	抵抗レベル数	駆動システム	トレッドタイプ	タイヤタイプ	レイヤ数	耐水技術	材料の特徴	ケーブル長（10進値）	十字穴番号	期間の単位	期間	バンド長	引張バネの数	ベアリング材質のタイプ	変速機減速比	デュロメーターの硬さ	トレッドミルの最大傾斜	サイクルタイプ	ベルト長派生	長ベルト長の測定単位	ベルト幅の単位	belt_length_string	ベルト幅	ベルト幅の十進数値	サイズマップ	グリップサイズ	定格内容積（総容積）	シャフト(軸)の長さ	シャフト(軸)の長さの単位	形状	対応重量	対応重量の単位	ロッド(棒)の長さ	ロッド(棒)の長さの単位	幅	幅の単位	商品の重量（パッケージを含まない）	ロッド(棒)の重量の単位	ヘッドサイズ名	商品の高さの単位	液体の容量	液体量の単位	商品本体サイズ：長さ	商品の幅の単位	容積容量名の単位	商品本体サイズ：幅	商品本体サイズ：高さ	商品の長さの単位	ホイールサイズ	ホイールサイズの単位	負荷容量	負荷容量の単位	配送重量	配送重量の単位	リムサイズ	バイクのリムサイズの単位	最大の高さ（ピーク）　	容量の単位	商品の直径	リールの直径の単位	商品の高さ	高さの単位	シャフト(軸)の丈	シャフトの高さの測定単位	商品の寸法の単位	商品の深さ	最大積載可能重量の単位	最大荷重の単位	メーカー推奨最大重量	推奨最大重量の単位	最低の高さの単位	最大高さ	最小の高さ	最大高さの単位	ブレーキの幅	ブレーキの幅の単位	フロアの長さ	フロアの幅	靴の幅	袖の長さ	ウエストのサイズ	クランクの長さ	トップチューブの長さ	股下の長さ	有効エッジの長さ	刃の長さ	メーカー推奨最大身長	破断強度	破断強度の単位	対物レンズのサイズ	商品対応範囲	商品の厚さ	袖の長さの単位	フロアの長さの単位	フロアの幅の単位	ウエストサイズの単位	クランクの長さの単位	トップチューブの長さの単位	有効エッジの長さの単位	刃の断面の形状の単位	推奨最高身長の単位	対物レンズのサイズの単位	商品対応範囲の単位	商品の厚さの単位	股下の長さの単位	靴幅の単位	高さマップ	胸囲サイズの単位	胸囲サイズ	強度	強度の単位	タイヤの直径	タイヤ直径の単位	拡張された長さ	伸びた長さの測定単位	ケーブル長さの単位	ハンドルの高さ	ハンドルの高さの単位	チャンバーの高さの単位	チャンバーの幅単位	チャンバーの幅	チャンバーの高さ	バンドの幅の単位	バンド幅	フレームの高さ	フレームの高さの単位	デッキの長さ	デッキの幅	デッキ長の単位	デッキ幅の単位	ディスプレイサイズ	ディスプレイ最大解像度の単位	ディスプレイサイズ	モニターサイズの単位	最大圧力の単位	最大圧力	フルフィルメントセンターID	商品パッケージの高さ	商品パッケージの幅	商品パッケージの長さ	商品パッケージの長さの単位	商品パッケージの重量	商品パッケージの重量の単位	商品パッケージの高さの単位	商品パッケージの幅の単位	商品パッケージの寸法の単位	原産国/地域	布タイプ	布タイプ	布タイプ	布タイプ	布タイプ	電池付属	この商品は電池本体ですか？または電池を使用した商品ですか？	電池の種類、サイズ	電池の種類、サイズ	電池の種類、サイズ	電池の数	電池の数	電池の数	電池当たりのワット時	リチウム電池パッケージ	リチウム含有量(グラム)	リチウムイオン電池単数	リチウムメタル電池単数	電池の組成	電池の重量(グラム)	電池の重量の測定単位	リチウム電池のエネルギー量測定単位	リチウム電池の重量の測定単位	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	適用される危険物関連法令	国連(UN)番号	安全データシート(SDS) URL	商品の重量	商品の重量の単位	商品の体積	商品の容量の単位	内用液は含まれていますか?	引火点(°C)	商品に適用される保管規制の区分	メーカー保証の説明	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	分類／危険物ラベル(適用されるものをすべて選択)	保証	安全性に関する注意	法規上の免責条項	特徴・用途	legal_compliance_certification_date_of_issue	import_designation	legal_compliance_certification_metadata	legal_compliance_certification_expiration_date	cpsia_cautionary_statement	cpsia_cautionary_description	適合性の認証の状況	規制機関の名前	コンプライアンス認証値	保証タイプ	熱に弱い	通貨コード	コンディション	商品のコンディション説明	メーカー希望価格	商品の公開日	予約商品の販売開始日	商品の入荷予定日	出荷作業日数	取り寄せ	使用しない支払い方法	セール価格	セール開始日	セール終了日	最大注文個数	パッケージ数	ギフトメッセージ	ギフト包装	配送パターン	販売形態(並行輸入品)	在庫期間	終了日を提案する	最低広告表示価格	賞味期限もしくは消費期限タイプ	付属品総数	開始日を提案する	商品タックスコード	ポイントパーセント	セール時ポイントパーセント	配送日時指定SKUリスト	メーカー製造中止	サイン入り	max_aggregate_ship_quantity\n";
	$head .= "feed_product_type	item_sku	external_product_id	external_product_id_type	item_name	department_name	standard_price	quantity	main_image_url	brand_name	manufacturer	recommended_browse_nodes	depth_front_to_back	depth_front_to_back_unit_of_measure	depth_width_side_to_side	depth_width_side_to_side_unit_of_measure	depth_height_floor_to_top	depth_height_floor_to_top_unit_of_measure	target_gender	unit_count	unit_count_type	age_range_description	size_name	length_longer_edge	length_longer_edge_unit_of_measure	width_shorter_edge	width_shorter_edge_unit_of_measure	color_name	is_expiration_dated_product	thickness_head_to_toe	thickness_head_to_toe_unit_of_measure	thickness_width_side_to_side	thickness_width_side_to_side_unit_of_measure	thickness_floor_to_top	thickness_floor_to_top_unit_of_measure	length_head_to_toe	length_head_to_toe_unit_of_measure	length_width_side_to_side	length_width_side_to_side_unit_of_measure	length_height_floor_to_top	length_height_floor_to_top_unit_of_measure	item_diameter_derived	item_diameter_unit_of_measure	item_width_numeric	item_width_numeric_unit_of_measure	height_width_side_to_side	height_width_side_to_side_unit_of_measure	height_floor_top	height_floor_top_unit_of_measure	other_image_url1	other_image_url2	other_image_url3	other_image_url4	other_image_url5	other_image_url6	other_image_url7	other_image_url8	swatch_image_url	parent_child	parent_sku	relationship_type	variation_theme	package_level	package_contains_quantity	package_contains_identifier	update_delete	product_description	part_number	model_name	model	gtin_exemption_reason	codabar	model_year	closure_type	outer_material_type	inner_material_type	fuel_type	heel_type	heel_height_unit_of_measure	sole_material	heel_height	temperature_rating	care_instructions1	care_instructions2	care_instructions3	care_instructions4	bullet_point1	bullet_point2	bullet_point3	bullet_point4	bullet_point5	bullet_point6	bullet_point7	bullet_point8	bullet_point9	bullet_point10	style_name	generic_keywords	color_map	material_type	seasons1	seasons2	seasons3	seasons4	sport_type1	sport_type2	golf_club_loft	grip_type	lens_color	lens_material_type	is_autographed	league_name	hand_orientation	shaft_material	athlete	team_name	included_components1	included_components2	included_components3	included_components4	included_components5	compatible_devices1	compatible_devices2	compatible_devices3	compatible_devices4	compatible_devices5	fur_description	golf_club_loft_unit_of_measure	seating_group	item_type_name	option_type	target_audience_base	is_exclusive_product	number_of_boxes	glove_type	water_resistance_level	lining_description	bike_type	frame_material_type	num_of_settings	target_audience_keywords1	target_audience_keywords2	target_audience_keywords3	target_audience_keywords4	target_audience_keywords5	pattern_name	style_keywords	shaft_style_type	lithium_battery_voltage	platinum_keywords1	platinum_keywords2	platinum_keywords3	platinum_keywords4	platinum_keywords5	lithium_battery_voltage_unit_of_measure	golf_putter_lie_angle_unit1	golf_putter_lie_angle_unit2	golf_putter_lie_angle_unit3	golf_putter_lie_angle_unit4	golf_putter_lie_angle_unit5	mfg_maximum_unit_of_measure	item_offset_unit_of_measure	item_offset	golf_putter_lie_angle1	golf_putter_lie_angle2	golf_putter_lie_angle3	golf_putter_lie_angle4	golf_putter_lie_angle5	mfg_minimum_unit_of_measure	pocket_description	weight_bench_type	skill_level	fit_type1	fit_type2	fit_type3	fit_type4	fit_type5	floor_area	neck_style	pattern_type	rise_style	theme	top_style	ultraviolet_light_protection	wheel_type	frame_type	brake_style	suspension_type	number_of_doors	fill_material_type	mounting_type	temperature_rating_degrees	lens_type	insulation_material_type	pad_type	number_of_poles	light_source_type	seating_capacity	power_source_type	special_features1	special_features2	special_features3	special_features4	special_features5	capacity_name	battery_average_life1	battery_average_life2	battery_average_life3	magnification_strength	wheelbase	wheelbase_unit_of_measure	shell_type	thesaurus_attribute_keywords	recommended_uses_for_product	specific_uses_keywords1	specific_uses_keywords2	specific_uses_keywords3	specific_uses_keywords4	specific_uses_keywords5	thesaurus_subject_keywords1	thesaurus_subject_keywords2	thesaurus_subject_keywords3	thesaurus_subject_keywords4	thesaurus_subject_keywords5	magnification_strength_unit_of_measure	country_as_labeled	fishing_technique	lifecycle_supply_type1	lifecycle_supply_type2	lifecycle_supply_type3	lifecycle_supply_type4	lifecycle_supply_type5	floor_area_unit_of_measure	temperature_rating_degrees_unit_of_measure	battery_average_life_unit_of_measure1	battery_average_life_unit_of_measure2	battery_average_life_unit_of_measure3	shaft_circumference	shaft_circumference_unit_of_measure	toe_style	midsole_material_type	number_of_dividers	caliber	air_gun_power_type1	air_gun_power_type2	air_gun_power_type3	air_gun_power_type4	air_gun_power_type5	insulation_resistance_unit_of_measure	insulation_resistance	assembly_instructions	display_type	exercise_machine_arm_type	space_efficiency1	space_efficiency2	heart_rate_sensor_type	number_of_pieces	tread_depth_unit_of_measure	tread_depth	lock_type	handle_material	folded_size	pole_material_type1	pole_material_type2	pole_material_type3	pole_material_type4	pole_material_type5	handle_diameter	handle_length_unit	handle_length_derived	handle_type	wheel_material	number_of_wheels	target_species1	target_species2	target_species3	target_species4	target_species5	chamber_depth_unit_of_measure	chamber_depth	band_length	band_material_type	band_color	band_length_unit	speed_rating	number_of_programs	maximum_horsepower	maximum_horsepower_unit_of_measure	minimum_speed_unit_of_measure	is_assembly_required	maximum_speed_unit_of_measure	maximum_speed	assembly_time_unit_of_measure	power_plug_type	assembly_time	minimum_speed	operation_mode	air_flow_capacity	air_flow_capacity_unit_of_measure	compatible_with_vehicle_type1	compatible_with_vehicle_type2	compatible_with_vehicle_type3	compatible_with_vehicle_type4	compatible_with_vehicle_type5	compatible_with_vehicle_type6	compatible_with_vehicle_type7	compatible_with_vehicle_type8	compatible_with_vehicle_type9	golf_club_flex	tension_level	is_cod_included	fc_shelf_life_unit_of_measure	shipping_options	is_cvs_included	leather_type	resistance_mechanism	grip_material_type	fishing_line_type	shaft_diameter_derived	shaft_diameter_unit_of_measure	shaft_material_type	mfg_maximum	configuration	mfg_minimum	number_of_pockets	item_depth_unit_of_measure	adjustment_type	has_adjustable_length	strap_type	construction_type	bottom_style	leg_style	sleeve_type	support_type	line_weight	number_of_speeds	number_of_power_levels	fitting_type	number_of_gear_loops	minimum_torso_measurement	occupancy	magnification_maximum	capacity_name_unit_of_measure	strand_type	reflecting_technology	base_material_type	finish_type1	finish_type2	shaft_length_derived	mounting_hole_diameter_unit_of_measure	mounting_hole_diameter_derived	life_vest_type	insole_material_type	wrinkle_resistance1	wrinkle_resistance2	wrinkle_resistance3	wrinkle_resistance4	wrinkle_resistance5	legal_compliance_certification_certifying_authority_name	legal_compliance_certification_geographic_jurisdiction	number_of_rounds	barrel_material_type	variety	number_of_resistance_levels	drive_system	tread_type	tire_type	number_of_layers	water_resistance_technology	material_features	cable_length_derived	screw_point_style	duration_unit_of_measure	duration	band_size	number_of_springs	bearing_material_type	gearbox_ratio	durometer_hardness	maximum_incline_percentage	motor_type	belt_length_derived	belt_length_unit_of_measure	belt_width_unit_of_measure	belt_length_string	belt_width_string	belt_width_derived	size_map	grip_size	volume_capacity_name	shaft_length	shaft_length_unit_of_measure	item_shape	display_maximum_weight_recommendation	display_maximum_weight_recommendation_unit_of_measure	item_display_length	item_display_length_unit_of_measure	item_display_width	item_display_width_unit_of_measure	item_display_weight	item_display_weight_unit_of_measure	head_size_name	item_height_unit_of_measure	liquid_volume	liquid_volume_unit_of_measure	item_length	item_width_unit_of_measure	volume_capacity_name_unit_of_measure	item_width	item_height	item_length_unit_of_measure	wheel_size	wheel_size_unit_of_measure	load_capacity	load_capacity_unit_of_measure	website_shipping_weight	website_shipping_weight_unit_of_measure	rim_size	rim_size_unit_of_measure	capacity	capacity_unit_of_measure	item_display_diameter	item_display_diameter_unit_of_measure	item_display_height	item_display_height_unit_of_measure	shaft_height	shaft_height_unit_of_measure	item_dimensions_unit_of_measure	item_depth	maximum_weight_capacity	maximum_weight_capacity_unit_of_measure	maximum_weight_recommendation	maximum_weight_recommendation_unit_of_measure	minimum_height_unit_of_measure	maximum_height	minimum_height	maximum_height_unit_of_measure	brake_width	brake_width_unit_of_measure	floor_length	floor_width	shoe_width	sleeve_length	waist_size	crank_length	top_tube_length	inseam_length	effective_edge_length	blade_length	maximum_height_recommendation	breaking_strength	breaking_strength_unit_of_measure	objective_lens_diameter	item_area	item_thickness_derived	sleeve_length_unit_of_measure	floor_length_unit_of_measure	floor_width_unit_of_measure	waist_size_unit_of_measure	crank_length_unit_of_measure	top_tube_length_unit_of_measure	effective_edge_length_unit_of_measure	blade_length_unit_of_measure	maximum_height_recommendation_unit_of_measure	objective_lens_diameter_unit_of_measure	item_area_unit_of_measure	item_thickness_unit_of_measure	inseam_length_unit_of_measure	shoe_width_unit_of_measure	height_map	chest_size_unit_of_measure	chest_size	tensile_strength	tensile_strength_unit_of_measure	tire_diameter	tire_diameter_unit_of_measure	extended_length	extended_length_unit_of_measure	cable_length_unit_of_measure	handle_height	handle_height_unit_of_measure	chamber_height_unit_of_measure	chamber_width_unit_of_measure	chamber_width	chamber_height	band_width_unit_of_measure	band_width	frame_size	frame_size_unit_of_measure	deck_length	deck_width	deck_length_unit_of_measure	deck_width_unit_of_measure	display_size	display_size_unit_of_measure	monitor_size	monitor_size_unit_of_measure	maximum_pressure_unit_of_measure	maximum_pressure	fulfillment_center_id	package_height	package_width	package_length	package_length_unit_of_measure	package_weight	package_weight_unit_of_measure	package_height_unit_of_measure	package_width_unit_of_measure	package_dimensions_unit_of_measure	country_of_origin	fabric_type1	fabric_type2	fabric_type3	fabric_type4	fabric_type5	are_batteries_included	batteries_required	battery_type1	battery_type2	battery_type3	number_of_batteries1	number_of_batteries2	number_of_batteries3	lithium_battery_energy_content	lithium_battery_packaging	lithium_battery_weight	number_of_lithium_ion_cells	number_of_lithium_metal_cells	battery_cell_composition	battery_weight	battery_weight_unit_of_measure	lithium_battery_energy_content_unit_of_measure	lithium_battery_weight_unit_of_measure	supplier_declared_dg_hz_regulation1	supplier_declared_dg_hz_regulation2	supplier_declared_dg_hz_regulation3	supplier_declared_dg_hz_regulation4	supplier_declared_dg_hz_regulation5	hazmat_united_nations_regulatory_id	safety_data_sheet_url	item_weight	item_weight_unit_of_measure	item_volume	item_volume_unit_of_measure	contains_liquid_contents	flash_point	fulfillment_center_special_storage	warranty_description	ghs_classification_class1	ghs_classification_class2	ghs_classification_class3	mfg_warranty_description_type	safety_warning	legal_disclaimer_description	specific_uses_for_product	legal_compliance_certification_date_of_issue	import_designation	legal_compliance_certification_metadata	legal_compliance_certification_expiration_date	cpsia_cautionary_statement	cpsia_cautionary_description	legal_compliance_certification_status	legal_compliance_certification_regulatory_organization_name	legal_compliance_certification_value	warranty_type	is_heat_sensitive	currency	condition_type	condition_note	list_price	product_site_launch_date	merchant_release_date	restock_date	fulfillment_latency	is_sourcing_on_demand	optional_payment_type_exclusion	sale_price	sale_from_date	sale_end_date	max_order_quantity	item_package_quantity	offering_can_be_gift_messaged	offering_can_be_giftwrapped	merchant_shipping_group_name	distribution_designation	fc_shelf_life	offering_end_date	map_price	product_expiration_type	number_of_items	offering_start_date	product_tax_code	standard_price_points_percent	sale_price_points_percent	delivery_schedule_group_id	is_discontinued_by_manufacturer	signed_by	max_aggregate_ship_quantity\n";
	return $head;
}



//	Amazon ASIN登録
//	add ohkawara 2019/11/20
function regist_asin(&$AMZ_UP_LIST) {
	global $conn_id;

	//	商品情報を取得
	$sql  = "SELECT g_num, size_list FROM goods;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {

			$g_num = $list['g_num'];
			$c_p_sku = "g".$g_num;				//	親SKU作成
			$size_list = $list['size_list'];

			$op_size = unserialize($size_list);
			if ($op_size) {
				$up_flg = 0;
				$none_flg = 0;

				//	親情報のSKU登録が有った場合
				if ($AMZ_UP_LIST[$c_p_sku]['asin'] != "") {
					$op_size[0]['jan'] = "par";
					$op_size[0]['asin'] = $AMZ_UP_LIST[$c_p_sku]['asin'];
					$up_flg = 1;
					$none_flg = 1;
				}

				foreach ($op_size AS $key => $VAL) {
					$c_jan = $VAL['jan'];				//	変更するサイズを取得
					$c_size = $VAL['size'];				//	変更するサイズを取得
					$c_sku  = $c_p_sku."-".$c_size;		//	SKUを作成

					//	親情報だったら場合
					if ($c_jan == "par") {
						continue;
					}

					//	オリジナル商品か確認
					if ($c_jan == "none") {
						$none_flg = 1;
					}

					//	出品レポートにASINが記録されていたならば配列に入れる
					if ($AMZ_UP_LIST[$c_sku]['asin'] != "") {
						$op_size[$key]['asin'] = $AMZ_UP_LIST[$c_sku]['asin'];
						$up_flg = 1;
					}

					//	サイズの状態がNEW(2)になっていたら販売(1)に変える
					if ($op_size[$key]['type'] == 2) {
						$op_size[$key]['type'] = 1;
						$up_flg = 1;
					}
				}

				//	オリジナル商品情報に更新があったらDBに反映する。
				if ($up_flg == 1 && $none_flg == 1) {
					$size_list = serialize($op_size);

					$sql  = "UPDATE ".T_GOODS." SET".
							" size_list='".$size_list."'".
							" WHERE g_num='".$g_num."';";
					if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "商品データーを変更できませんでした。(1):".$g_num; }
				}
			}

		}
	}

}



//	Wowma
//	add ohkawara 2017/02/09
function make_wowma_csv($list, &$WOWMA_UP_LIST, &$list_item_csv, &$list_zaiko_csv, $del_flg) {
	global $WOWMA_GOODS_DISCOUNT_CATE;
	global $attention_wowma;
	global $L_BRAND;

	$size_list = $list['size_list'];

	$g_num = $list['g_num'];
	$wd_id = $list['wd_id'];
	$g_name = $list['g_name'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$code = $list['code'];
	$generic_keywords1 = $list['generic_keywords1'];
	$generic_keywords2 = $list['generic_keywords2'];
	$generic_keywords3 = $list['generic_keywords3'];
	$generic_keywords4 = $list['generic_keywords4'];
	$generic_keywords5 = $list['generic_keywords5'];

	$zaiko = $list['zaiko'];		//	add ohkawara 2017/06/15




	//--------------------------------------------//

//	アップ済み商品のチェックとフラグを追加
	//	サイズが有る無しによって設定が有るので、サイズ確認を最初に行う
	$op_size = unserialize($size_list);

	$ALL_SIZE = array();
	$hanbai_flg = 0;	//	最終的に0だったら販売終了商品
	if ($op_size) {
		$i = 1;
		$asin_flg = 0;
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['jan'] == "none") {
				$VAL['jan'] = "";
			}
			if ($VAL['type'] != 3) {
				$hanbai_flg = 1;
			}
			//	サイズ確認	空設定用
			$check_size = $VAL['size'];
			$check_size = preg_replace("/\(日本サイズ\)/", "", $check_size);

			$check_size = preg_replace("/^F$/", "", $check_size);
			$check_size = preg_replace("/フリーサイズ/", "", $check_size);
			$check_size = preg_replace("/フリー/", "", $check_size);
			$check_size = preg_replace("/--/", "", $check_size);
			$check_size = preg_replace("/フットサルサイズ/", "", $check_size);
			$check_size = preg_replace("/設定なし/", "", $check_size);

			if ($check_size) {
				if ($i == 1 && !$ALL_SIZE[0]) {
					$ALL_SIZE[0]['size'] = "";
					$ALL_SIZE[0]['jan'] = "";
					$ALL_SIZE[0]['asin'] = "";
					$ALL_SIZE[0]['type'] = "";
				}
				$ALL_SIZE[$i]['size'] = $check_size;
				$ALL_SIZE[$i]['jan'] = $VAL['jan'];
				$ALL_SIZE[$i]['asin'] = $VAL['asin'];
				$ALL_SIZE[$i]['type'] = $VAL['type'];
				$i++;
			} else {
				$ALL_SIZE[0]['size'] = "";
				$ALL_SIZE[0]['jan'] = $VAL['jan'];
				$ALL_SIZE[0]['asin'] = $VAL['asin'];
				$ALL_SIZE[0]['type'] = $VAL['type'];
			}
		}
	}
	//	$ALL_SIZEの設定がなければこの商品の登録はしない（不具合があった場合）
	if (count($ALL_SIZE) < 1) {
		unset($WOWMA_UP_LIST[$g_num]);	//	add ohkawara 2018/09/14
		return;
	}

	$size_option_flg = 0;
	//if ($ALL_SIZE[1]['size'] != "") {		//	del ohkawara 2018/12/17
	if (!empty($ALL_SIZE[1]['size'])) {		//	add ohkawara 2018/12/17
		$size_option_flg = 1;
	}

	//--------------------------------------------//

	//	コントロールカラム
	$ctrlCol = "N";		//	N:新規登録	U:更新
	if ($WOWMA_UP_LIST[$g_num]) {
		$ctrlCol = "U";
	}

	//	ロットナンバー
	$lotNumber = $WOWMA_UP_LIST[$g_num]['lotNumber'];

	//	商品名
	$itemName = $g_name;

	//	管理用ID
	$itemManagementId = $code;

	//	管理用商品名	未設定
	$itemManagementName = "";

	//	商品コード
	$itemCode = "g".$g_num;

	//	販売価格
	//	割引設定項目
	$GOODS_DISCOUNT_C = WOWMA_GOODS_DISCOUNT_C;
	$GOODS_DISCOUNT_CATE = $WOWMA_GOODS_DISCOUNT_CATE;
	$DISCOUNT_PAR = WOWMA_DISCOUNT_PAR;
	$DISCOUNT_PAR2 = WOWMA_DISCOUNT_PAR2;

	if ($base_price > 0 && $base_sale_price > 0 && $base_price < $base_sale_price) {
		$sale_price = "";
		$price = $base_price;
	} elseif ($base_price > 0 && $base_sale_price > 0 && $base_price == $base_sale_price) {
		if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
			$flag = 0;
			if ($GOODS_DISCOUNT_CATE) {
				foreach ($GOODS_DISCOUNT_CATE AS $val) {
					$val = trim($val);
					if ($cate1 == $val) { $flag = 1; }
				}
			}
			if ($flag != 1) {
				$sale_price = floor($base_price * ((100 - $DISCOUNT_PAR2) / 100));
			} else {
				$sale_price = "";
			}
		} else {
			$sale_price = "";
		}
		$price = $base_price;
	} elseif ($GOODS_DISCOUNT_C == 1) {
		$flag = 0;
		if ($GOODS_DISCOUNT_CATE) {
			foreach ($GOODS_DISCOUNT_CATE AS $val) {
				$val = trim($val);
				if ($cate1 == $val) { $flag = 1; }
			}
		}
		if ($flag != 1) {
			$sale_price_ = floor($base_price * ((100 - $DISCOUNT_PAR) / 100));
			if ($base_sale_price < 1) {
				$price = $base_price;
				$sale_price = $sale_price_;
			} elseif ($base_sale_price > $sale_price_) {
				$price = $base_sale_price;
				$sale_price = $sale_price_;
			} else {
				$price = $base_price;
				$sale_price = $base_sale_price;
			}
		} else {
			if ($base_sale_price) {
				$price = $base_sale_price;
				$sale_price = "";
			} else {
				$price = $base_price;
				$sale_price = "";
			}
		}
	}

	$price = floor($price * ((TAX / 100) + 1) + 0.5);
	$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
	if ($sale_price < 1) { unset($sale_price); }
	$itemPrice = $price;
	if ($sale_price > 0) {
		$itemPrice = $sale_price;
	}

	//	販売方法区分		1:通常販売（空白で入力しても自動的に1（通常販売）で登録する）	2:予約販売
	$sellMethodSegment = "";

	//	発売(入荷)予定日	日付フォーマット　"yyyyMMdd"	※15日後から1年先までの日付を指定できる。
	$releaseDate = "";

	//	予約登録日	日付フォーマット　"yyyyMMdd"	※15日後から1年先までの日付を指定できる。
	$reserveRegstDate = "";

	//	税区分	1:内税	2:税なし	半角英字 大文字	※外税指定はありません。
	$taxSegment = 1;

	//	送料設定区分	1:送料別	2:送料無料	3:個別送料
	$postageSegment = "1";

	//	個別送料
	$postage = "";	//	設定無し

	//	(旧)配送方法ID	M:メール便
	$deliveryId = "";	//	設定無し

	//	(新)配送方法ID1～5		//	設定無し
	$deliveryMethodId1 = "";
	$deliveryMethodId2 = "";
	$deliveryMethodId3 = "";
	$deliveryMethodId4 = "";
	$deliveryMethodId5 = "";

	//	(新)配送方法名1～5		//	設定無し
	$deliveryMethodName1 = "";
	$deliveryMethodName2 = "";
	$deliveryMethodName3 = "";
	$deliveryMethodName4 = "";
	$deliveryMethodName5 = "";

	//	掲載開始日時	//	設定無し
	$publicStartDate = "";

	//	最大購入設定区分	1:指定なし	2:指定あり
	$att_check = 0;
	if (preg_match("/<!--[\s]*Attention[\s]*-->/", $caption)) {
		$att_check = 1;
	}
	$limitedOrderSegment = "1";
	if ($att_check == 1) {
		$limitedOrderSegment = 2;
	}
	//	最大購入数をすべて制限する為
	$limitedOrderSegment = 2;

	//	最大購入個数
	$limitedOrderCount = "";
	if ($att_check == 1) {
		$limitedOrderCount = 1;
	} else {
		$limitedOrderCount = WOWMA_DEFO_ZAIKO;
	}

	//	商品説明(共通)
	$description = "";
	if ($att_check == 1) {
		$description .= $attention_wowma."<br>";
	}
	$caption = preg_replace("/<!--[\s]*Attention[\s]*-->/", "", $caption);
	$caption = preg_replace("/<br \/>/", "<br>", $caption);
	$description .= "ブランド名：".$L_BRAND[$brand]."<br>";
	$description .= $caption."<br>";
	$description = nl2br($description, false);
	$description = preg_replace('/(?:\n|\r|\r\n)/', '', $description );
	strip_tags($description, '<br><center><font><hr><a><b>');
	$description = trim($description);
	if (strlen($description) > 980) {	//	本当は1024
#		$description = mb_strimwidth($description, 0, 980, "", "eucjp-win");	//	1024だけど縮まらないので980
		$description = mb_strimwidth($description, 0, 980, "", "UTF-8");	//	1024だけど縮まらないので980
	}
	$description = trim($description);

	//	SP用商品説明
	$descriptionForSP = "";	//	設定無し

	//	PC用商品説明
	$descriptionForPC = "";	//	設定無し

	//	商品詳細タイトル
	$detailTitle = "";	//	設定無し

	//	商品詳細説明
	$detailDescription = "";	//	設定無し

	//	スペックタイトル
	$specTitle = "";	//	設定無し

	//	スペック1～5
	$spec1 = "";	//	設定無し
	$spec2 = "";	//	設定無し
	$spec3 = "";	//	設定無し
	$spec4 = "";	//	設定無し
	$spec5 = "";	//	設定無し


	//	検索キーワード１～３
	$searchKeyword1 = "";
	$searchKeyword2 = "";
	$searchKeyword3 = "";
	for ($i=1; $i<=3; $i++) {
		$searchKeyword_name = "searchKeyword".$i;
		$key_names = "generic_keywords".$i;
		$names = $$key_names;
		if (strlen($names) <= 20) {
			$$searchKeyword_name = $names;
		}
	}

	//	商品画像名1～10
	$imageName1 = "";
	$imageName2 = "";
	$imageName3 = "";
	$imageName4 = "";
	$imageName5 = "";
	$imageName6 = "";
	$imageName7 = "";
	$imageName8 = "";
	$imageName9 = "";
	$imageName10 = "";

	//	商品画像URL1～10
	$imageUrl1 = "";
	$imageUrl2 = "";
	$imageUrl3 = "";
	$imageUrl4 = "";
	$imageUrl5 = "";
	$imageUrl6 = "";
	$imageUrl7 = "";
	$imageUrl8 = "";
	$imageUrl9 = "";
	$imageUrl10 = "";
	$ii = 1;
	for ($i=1; $i<=10; $i++) {
		$KILN = "KtaiImageLinkName".$i;
		$KIT  = "KtaiImageType".$i;

		$image_file = "";

		if ($i == 1) {
			$image_file = IMG_F_DIR."/$code" . ".jpg";
		} elseif ($i == 2) {
			$image_file = IMG_B_DIR."/$code" . ".jpg";
		} elseif ($i == 3) {
			$image_file = IMG_03_DIR."/$code" . ".jpg";
		} elseif ($i == 4) {
			$image_file = IMG_04_DIR."/$code" . ".jpg";
		} elseif ($i == 5) {
			$image_file = IMG_05_DIR."/$code" . ".jpg";
		} elseif ($i == 6) {
			$image_file = IMG_06_DIR."/$code" . ".jpg";
		} elseif ($i == 7) {
			$image_file = IMG_07_DIR."/$code" . ".jpg";
		} elseif ($i == 8) {
			$image_file = IMG_08_DIR."/$code" . ".jpg";
		} elseif ($i == 9) {
			$image_file = IMG_09_DIR."/$code" . ".jpg";
		} elseif ($i == 10) {
			//	10枚目は無し
			$image_file = IMG_10_DIR."/$code" . ".jpg";
		}

		$$KILN = "NULL";
		$$KIT  = "N";
		if (file_exists($image_file)) {
			$$KIT  = "L";

			$fd_name = "";
			if ($i == 1) {
				$fd_name = "imagef";
			} elseif ($i == 2) {
				$fd_name = "imageb";
			} else {
				$fd_name = "image".sprintf("%02s", $i);
			}
			$KIU  = "imageUrl".$ii;
			++$ii;

			$$KIU = "https://www.futboljersey.com/".$fd_name."/".$code.".jpg";
		}
	}

	//	カテゴリID
	$categoryId = $wd_id;

	//	検索タグID
	$tagId = "";

	//	JANコード
	//	ISBNコード
	//	両方とも13桁になった為判別できない為書籍関係は出品却下！
	$jan = "";
	$isbn = "";
	foreach ($ALL_SIZE AS $key => $VAL) {
		if ($ALL_SIZE[$key]['jan']) {
			if (strlen($ALL_SIZE[$key]['jan']) == 13) {
				$jan = $ALL_SIZE[$key]['jan'];
				break;
			} elseif (strlen($ALL_SIZE[$key]['jan']) == 10) {
				$isbn = $ALL_SIZE[$key]['jan'];
				break;
			}
		}
	}

	//	型番
	$itemModel = "";	//	設定無し

	//	パスワード制限設定
	$limitedPasswd = "";	//	設定無し

	//	パスワード制限ページタイトル
	$limitedPasswdPageTitle = "";	//	設定無し

	//	パスワード制限ページメッセージ
	$limitedPasswdPageMessage = "";	//	設定無し

	//	販売ステータス	1:販売中	2:販売終了
	$saleStatus = 1;	//	出品中

	//	購入オプション1～20
	$itemOption1 = "";
	$itemOption2 = "";
	$itemOption3 = "";
	$itemOption4 = "";
	$itemOption5 = "";
	$itemOption6 = "";
	$itemOption7 = "";
	$itemOption8 = "";
	$itemOption9 = "";
	$itemOption10 = "";
	$itemOption11 = "";
	$itemOption12 = "";
	$itemOption13 = "";
	$itemOption14 = "";
	$itemOption15 = "";
	$itemOption16 = "";
	$itemOption17 = "";
	$itemOption18 = "";
	$itemOption19 = "";
	$itemOption20 = "";

	//	ポイント倍率
	$pointRate = WOWMA_POINT;

	//	在庫区分	1:通常在庫	2:選択肢別在庫
	$stockSegment = 1;
	if ($size_option_flg == 1) {
		$stockSegment = 2;
	}

	//	選択肢別在庫発送日ID
	$stockShippingDayId = "";				//	設定無し

	//	選択肢別在庫発送日表示文言
	$stockShippingDayDispTxt = "";			//	設定無し

	//	在庫切れメッセージ
	$displayBackorderMessage = "";			//	設定無し

	//	通常在庫数
	$stockCount = "";
	if ($stockSegment == 1) {
		if ($att_check == 1 || $zaiko == 1) {
			$stockCount = 1;
		} else {
			$stockCount = WOWMA_DEFO_ZAIKO;
		}
	}

	//	通常在庫発送日ID
	$stockShippingDayId = "";

	//	残在庫表示区分	1:在庫数を表示しない。	2:在庫数を表示する。	3:指定した在庫数以下の場合表示する。
	$displayStockSegment = "";
	if ($stockSegment != 2) {
		$displayStockSegment = 1;	//	非表示設定
	}

	//	在庫表示閾値
	$displayStockThreshold = "";	//	設定しない

	//	選択肢別在庫横軸項目名
	$choicesStockHorizontalItemName = "";
	if ($stockSegment == 2) {
		$choicesStockHorizontalItemName = "サイズ";
	}

	//	選択肢別在庫縦軸項目名
	$choicesStockVerticalItemName = "";

	//	選択肢別在庫上説明
	$choicesStockUpperDescription = "";

	//	選択肢別在庫下説明
	$choicesStockLowerDescription = "";

	//	残選択肢別在庫表示区分	1:在庫数を表示しない。	2:在庫数を表示する。	3:指定した在庫数以下の場合、「△」を表示。
	$displayChoicesStockSegment = "";
	if ($stockSegment == 2) {
		$displayChoicesStockSegment = 1;	//	在庫数の数を数多く設定しない為無し
	}

	//	選択肢別在庫表示閾値
	$displayChoicesStockThreshold = "";

	//--------------------------------------------//

	//	削除用の商品の場合
	if ($del_flg == 1 && $list) {
		foreach ($list AS $key => $val) {
			$$key = $val;
		}
		$StockNum = "0";
		$StockType = "B";
		$SkuOptionNameCol = "";
		$SkuOptionValueCol = "";
		$SkuOptionTypeCol = "";
		$SkuStockShowType = "";
		$SkuLimitNum = "";
	}

	//	csv設定
	$list_item_csv	.= "\"".$ctrlCol."\",";							//	
	$list_item_csv	.= "\"".$lotNumber."\",";						//	
	$list_item_csv	.= "\"".$itemName."\",";						//	
	$list_item_csv	.= "\"".$itemManagementId."\",";				//	
	$list_item_csv	.= "\"".$itemManagementName."\",";				//	
	$list_item_csv	.= "\"".$itemCode."\",";						//	
	$list_item_csv	.= "\"".$itemPrice."\",";						//	
	$list_item_csv	.= "\"".$sellMethodSegment."\",";				//	
	$list_item_csv	.= "\"".$releaseDate."\",";						//	
	$list_item_csv	.= "\"".$reserveRegstDate."\",";				//	
	$list_item_csv	.= "\"".$taxSegment."\",";						//	
	$list_item_csv	.= "\"".$postageSegment."\",";					//	
	$list_item_csv	.= "\"".$postage."\",";							//	
	$list_item_csv	.= "\"".$deliveryId."\",";						//	
	$list_item_csv	.= "\"".$deliveryMethodId1."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId2."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId3."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId4."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodId5."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName1."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName2."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName3."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName4."\",";				//	
	$list_item_csv	.= "\"".$deliveryMethodName5."\",";				//	
	$list_item_csv	.= "\"".$publicStartDate."\",";					//	
	$list_item_csv	.= "\"".$limitedOrderSegment."\",";				//	
	$list_item_csv	.= "\"".$limitedOrderCount."\",";				//	
	$list_item_csv	.= "\"".$description."\",";						//	
	$list_item_csv	.= "\"".$descriptionForSP."\",";				//	
	$list_item_csv	.= "\"".$descriptionForPC."\",";				//	
	$list_item_csv	.= "\"".$detailTitle."\",";						//	
	$list_item_csv	.= "\"".$detailDescription."\",";				//	
	$list_item_csv	.= "\"".$specTitle."\",";						//	
	$list_item_csv	.= "\"".$spec1."\",";							//	
	$list_item_csv	.= "\"".$spec2."\",";							//	
	$list_item_csv	.= "\"".$spec3."\",";							//	
	$list_item_csv	.= "\"".$spec4."\",";							//	
	$list_item_csv	.= "\"".$spec5."\",";							//	
	$list_item_csv	.= "\"".$searchKeyword1."\",";					//	
	$list_item_csv	.= "\"".$searchKeyword2."\",";					//	
	$list_item_csv	.= "\"".$searchKeyword3."\",";					//	
	$list_item_csv	.= "\"".$imageName1."\",";						//	
	$list_item_csv	.= "\"".$imageName2."\",";						//	
	$list_item_csv	.= "\"".$imageName3."\",";						//	
	$list_item_csv	.= "\"".$imageName4."\",";						//	
	$list_item_csv	.= "\"".$imageName5."\",";						//	
	$list_item_csv	.= "\"".$imageName6."\",";						//	
	$list_item_csv	.= "\"".$imageName7."\",";						//	
	$list_item_csv	.= "\"".$imageName8."\",";						//	
	$list_item_csv	.= "\"".$imageName9."\",";						//	
	$list_item_csv	.= "\"".$imageName10."\",";						//	
	$list_item_csv	.= "\"".$imageUrl1."\",";						//	
	$list_item_csv	.= "\"".$imageUrl2."\",";						//	
	$list_item_csv	.= "\"".$imageUrl3."\",";						//	
	$list_item_csv	.= "\"".$imageUrl4."\",";						//	
	$list_item_csv	.= "\"".$imageUrl5."\",";						//	
	$list_item_csv	.= "\"".$imageUrl6."\",";						//	
	$list_item_csv	.= "\"".$imageUrl7."\",";						//	
	$list_item_csv	.= "\"".$imageUrl8."\",";						//	
	$list_item_csv	.= "\"".$imageUrl9."\",";						//	
	$list_item_csv	.= "\"".$imageUrl10."\",";						//	
	$list_item_csv	.= "\"".$categoryId."\",";						//	
	$list_item_csv	.= "\"".$tagId."\",";							//	
	$list_item_csv	.= "\"".$jan."\",";								//	
	$list_item_csv	.= "\"".$isbn."\",";							//	
	$list_item_csv	.= "\"".$itemModel."\",";						//	
	$list_item_csv	.= "\"".$limitedPasswd."\",";					//	
	$list_item_csv	.= "\"".$limitedPasswdPageTitle."\",";			//	
	$list_item_csv	.= "\"".$limitedPasswdPageMessage."\",";		//	
	$list_item_csv	.= "\"".$saleStatus."\",";						//	
	$list_item_csv	.= "\"".$itemOption1."\",";						//	
	$list_item_csv	.= "\"".$itemOption2."\",";						//	
	$list_item_csv	.= "\"".$itemOption3."\",";						//	
	$list_item_csv	.= "\"".$itemOption4."\",";						//	
	$list_item_csv	.= "\"".$itemOption5."\",";						//	
	$list_item_csv	.= "\"".$itemOption6."\",";						//	
	$list_item_csv	.= "\"".$itemOption7."\",";						//	
	$list_item_csv	.= "\"".$itemOption8."\",";						//	
	$list_item_csv	.= "\"".$itemOption9."\",";						//	
	$list_item_csv	.= "\"".$itemOption10."\",";					//	
	$list_item_csv	.= "\"".$itemOption11."\",";					//	
	$list_item_csv	.= "\"".$itemOption12."\",";					//	
	$list_item_csv	.= "\"".$itemOption13."\",";					//	
	$list_item_csv	.= "\"".$itemOption14."\",";					//	
	$list_item_csv	.= "\"".$itemOption15."\",";					//	
	$list_item_csv	.= "\"".$itemOption16."\",";					//	
	$list_item_csv	.= "\"".$itemOption17."\",";					//	
	$list_item_csv	.= "\"".$itemOption18."\",";					//	
	$list_item_csv	.= "\"".$itemOption19."\",";					//	
	$list_item_csv	.= "\"".$itemOption20."\",";					//	
	$list_item_csv	.= "\"".$pointRate."\",";						//	
	$list_item_csv	.= "\"".$stockSegment."\",";					//	
	$list_item_csv	.= "\"".$stockShippingDayId."\",";				//	
	$list_item_csv	.= "\"".$stockShippingDayDispTxt."\",";			//	
	$list_item_csv	.= "\"".$displayBackorderMessage."\",";			//	
	$list_item_csv	.= "\"".$stockCount."\",";						//	
	$list_item_csv	.= "\"".$displayStockSegment."\",";				//	
	$list_item_csv	.= "\"".$displayStockThreshold."\",";			//	
	$list_item_csv	.= "\"".$choicesStockHorizontalItemName."\",";	//	
	$list_item_csv	.= "\"".$choicesStockVerticalItemName."\",";	//	
	$list_item_csv	.= "\"".$choicesStockUpperDescription."\",";	//	
	$list_item_csv	.= "\"".$choicesStockLowerDescription."\",";	//	
	$list_item_csv	.= "\"".$displayChoicesStockSegment."\",";		//	
	$list_item_csv	.= "\"".$displayChoicesStockThreshold."\"";		//	
	$list_item_csv	.= "\r\n";

	//	add ohkawara 2018/05/23
	//	在庫区分が1:通常在庫の場合、在庫CSVは作らなくてよい？はず。
	if ($stockSegment == 1) {
		unset($WOWMA_UP_LIST[$g_num]);	//	add ohkawara 2018/09/14
		return;
	}

	//	在庫ファイル
	//	$lotNumber	//	設定済み
	//	$itemCode	//	設定済み
	$i = 1;
	foreach ($ALL_SIZE AS $VAL) {

		if ($att_check == 1 || $zaiko == 1) {
			$SkuItemNum = 1;
			$choicesStockCount = 1;
		} else {
			$SkuItemNum = WOWMA_DEFO_ZAIKO;
			$choicesStockCount = WOWMA_DEFO_ZAIKO;
		}

		$choicesStockHorizontalName = "";
		$choicesStockHorizontalCode = "";
		$choicesStockHorizontalSeq = "";

		//	販売終了商品またはサイズが非表示だった場合は在庫0にする
		if ($saleStatus == 2 || $VAL['type'] == 3) {
			$SkuItemNum = 0;
			$choicesStockCount = 0;
		}

		if ($VAL['size'] != "") {
			//	サイズが有る場合
			$stockCount = "";
			$stockSegment = 2;
			$choicesStockHorizontalName = $VAL['size'];
			$choicesStockHorizontalCode = "-".$VAL['size'];
			$choicesStockHorizontalSeq = $i;
			//$i++;	//	del ohkawara 2018/05/21
		} else {
			//	サイズが無い場合
			$choicesStockCount = "";
			$stockSegment = 1;

			//	add ohkawara 2018/05/21
			if ($WOWMA_UP_LIST[$g_num]['stockSegment'] == 1 && $WOWMA_UP_LIST[$g_num]['stockCount'] == 2) {
				continue;
			}
		}

		$choicesStockVerticalName = "";
		$choicesStockVerticalCode = "";
		$choicesStockVerticalSeq = "";

		//	在庫区分が選択があるのに通常の設定になっていた場合データーを作らない
		if ($stockSegment == 1 && $stockCount === "") {
			continue;
		}

		//	すでに在庫設定が0になっている場合はデーターを作らない
		$check_stockSegment = $WOWMA_UP_LIST[$g_num]['stockSegment'];
		$check_stockCount = $WOWMA_UP_LIST[$g_num]['stockCount'];
		$check_choicesStockCount = $WOWMA_UP_LIST[$g_num]['size'][$choicesStockHorizontalName];
		if ($check_stockSegment && $VAL['type'] == 3) {
			if ($stockSegment == 1 && $stockCount === 0 && $check_stockCount < 1) {
				continue;
			} elseif ($stockSegment == 2 && $choicesStockCount === 0 && $check_choicesStockCount < 1) {
				continue;
			}
		}

		//	項目軸を変更する場合はすべてNで登録する必要がある。
		$ctrlCol = "N";
		/*	//	2018/05/18	とりあえずすべてNで登録してみて大丈夫か確認してみる。
		//if (count($WOWMA_UP_LIST[$g_num]['size'][$choicesStockHorizontalName]) > 0) {		//	del ohkawara 2018/05/18	NとUを混合してよい場合
		if (count($WOWMA_UP_LIST[$g_num]['size']) > 0) {									//	add ohkawara 2018/05/18	一つでもUが合ったらすべてUにする。但しまだ不具合有り
		//if ($WOWMA_UP_LIST[$g_num]['lotNumber'] > 0) {
			$ctrlCol = "U";
		}
		*/

		$choicesStockShippingDayId = "";

		$choicesStockImageUrl = "";			//	add ohkawara 2018/12/17
		$choicesStockColorSegment = "";		//	add ohkawara 2018/12/17

		$list_zaiko_csv	.= "\"".$ctrlCol."\",";							//	ctrlCol
		$list_zaiko_csv	.= "\"".$lotNumber."\",";						//	lotNumber
		$list_zaiko_csv	.= "\"".$itemCode."\",";						//	itemCode		Code
		$list_zaiko_csv	.= "\"".$stockSegment."\",";					//	stockSegment	SkuOptionTypeCol
		$list_zaiko_csv	.= "\"".$stockCount."\",";						//	stockCount		SkuItemNum
		$list_zaiko_csv	.= "\"".$choicesStockHorizontalName."\",";		//	choicesStockHorizontalName
		$list_zaiko_csv	.= "\"".$choicesStockHorizontalCode."\",";		//	choicesStockHorizontalCode
		$list_zaiko_csv	.= "\"".$choicesStockHorizontalSeq."\",";		//	choicesStockHorizontalSeq
		$list_zaiko_csv	.= "\"".$choicesStockVerticalName."\",";		//	choicesStockVerticalName	空
		$list_zaiko_csv	.= "\"".$choicesStockVerticalCode."\",";		//	choicesStockVerticalCode	空
		$list_zaiko_csv	.= "\"".$choicesStockVerticalSeq."\",";			//	choicesStockVerticalSeq		空
		$list_zaiko_csv	.= "\"".$choicesStockCount."\",";				//	choicesStockCount
		$list_zaiko_csv	.= "\"".$choicesStockShippingDayId."\",";		//	choicesStockShippingDayId	設定しない
		$list_zaiko_csv	.= "\"".$choicesStockShippingDayDispTxt."\",";	//	
		$list_zaiko_csv	.= "\"".$choicesStockImageUrl."\",";			//	add ohkawara 2018/12/17
		$list_zaiko_csv	.= "\"".$choicesStockColorSegment."\"";			//	add ohkawara 2018/12/17
		$list_zaiko_csv	.= "\r\n";
		$i++;	//	add ohkawara 2018/05/21
	}

//if ($g_num == 1052) {
//exit;
//}
	unset($WOWMA_UP_LIST[$g_num]);

}


//	wowma!アップ済み商品で、在庫0設定商品、オリジナルで販売終了設定商品を削除
function del_wowma_data(&$WOWMA_UP_LIST, &$list_item_csv, &$list_zaiko_csv) {

	//	削除データー作成
	if ($WOWMA_UP_LIST) {
		foreach ($WOWMA_UP_LIST AS $key_num => $LIST) {

			foreach ($LIST AS $key_name => $value) {
				$$key_name = $value;
			}
			$ctrlCol = "U";
			$saleStatus = 2;


			//	スペックタイトル
			$specTitle = "";	//	設定無し
			//	スペック1～5
			$spec1 = "";	//	設定無し
			$spec2 = "";	//	設定無し
			$spec3 = "";	//	設定無し
			$spec4 = "";	//	設定無し
			$spec5 = "";	//	設定無し


			//	csv設定
			$list_item_csv	.= "\"".$ctrlCol."\",";							//	
			$list_item_csv	.= "\"".$lotNumber."\",";						//	
			$list_item_csv	.= "\"".$itemName."\",";						//	
			$list_item_csv	.= "\"".$itemManagementId."\",";				//	
			$list_item_csv	.= "\"".$itemManagementName."\",";				//	
			$list_item_csv	.= "\"".$itemCode."\",";						//	
			$list_item_csv	.= "\"".$itemPrice."\",";						//	
			$list_item_csv	.= "\"".$sellMethodSegment."\",";				//	
			$list_item_csv	.= "\"".$releaseDate."\",";						//	
			$list_item_csv	.= "\"".$reserveRegstDate."\",";				//	
			$list_item_csv	.= "\"".$taxSegment."\",";						//	
			$list_item_csv	.= "\"".$postageSegment."\",";					//	
			$list_item_csv	.= "\"".$postage."\",";							//	
			$list_item_csv	.= "\"".$deliveryId."\",";						//	
			$list_item_csv	.= "\"".$deliveryMethodId1."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId2."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId3."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId4."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodId5."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName1."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName2."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName3."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName4."\",";				//	
			$list_item_csv	.= "\"".$deliveryMethodName5."\",";				//	
			$list_item_csv	.= "\"".$publicStartDate."\",";					//	
			$list_item_csv	.= "\"".$limitedOrderSegment."\",";				//	
			$list_item_csv	.= "\"".$limitedOrderCount."\",";				//	
			$list_item_csv	.= "\"".$description."\",";						//	
			$list_item_csv	.= "\"".$descriptionForSP."\",";				//	
			$list_item_csv	.= "\"".$descriptionForPC."\",";				//	
			$list_item_csv	.= "\"".$detailTitle."\",";						//	
			$list_item_csv	.= "\"".$detailDescription."\",";				//	
			$list_item_csv	.= "\"".$specTitle."\",";						//	
			$list_item_csv	.= "\"".$spec1."\",";							//	
			$list_item_csv	.= "\"".$spec2."\",";							//	
			$list_item_csv	.= "\"".$spec3."\",";							//	
			$list_item_csv	.= "\"".$spec4."\",";							//	
			$list_item_csv	.= "\"".$spec5."\",";							//	
			$list_item_csv	.= "\"".$searchKeyword1."\",";					//	
			$list_item_csv	.= "\"".$searchKeyword2."\",";					//	
			$list_item_csv	.= "\"".$searchKeyword3."\",";					//	
			$list_item_csv	.= "\"".$imageName1."\",";						//	
			$list_item_csv	.= "\"".$imageName2."\",";						//	
			$list_item_csv	.= "\"".$imageName3."\",";						//	
			$list_item_csv	.= "\"".$imageName4."\",";						//	
			$list_item_csv	.= "\"".$imageName5."\",";						//	
			$list_item_csv	.= "\"".$imageName6."\",";						//	
			$list_item_csv	.= "\"".$imageName7."\",";						//	
			$list_item_csv	.= "\"".$imageName8."\",";						//	
			$list_item_csv	.= "\"".$imageName9."\",";						//	
			$list_item_csv	.= "\"".$imageName10."\",";						//	
			$list_item_csv	.= "\"".$imageUrl1."\",";						//	
			$list_item_csv	.= "\"".$imageUrl2."\",";						//	
			$list_item_csv	.= "\"".$imageUrl3."\",";						//	
			$list_item_csv	.= "\"".$imageUrl4."\",";						//	
			$list_item_csv	.= "\"".$imageUrl5."\",";						//	
			$list_item_csv	.= "\"".$imageUrl6."\",";						//	
			$list_item_csv	.= "\"".$imageUrl7."\",";						//	
			$list_item_csv	.= "\"".$imageUrl8."\",";						//	
			$list_item_csv	.= "\"".$imageUrl9."\",";						//	
			$list_item_csv	.= "\"".$imageUrl10."\",";						//	
			$list_item_csv	.= "\"".$categoryId."\",";						//	
			$list_item_csv	.= "\"".$tagId."\",";							//	
			$list_item_csv	.= "\"".$jan."\",";								//	
			$list_item_csv	.= "\"".$isbn."\",";							//	
			$list_item_csv	.= "\"".$itemModel."\",";						//	
			$list_item_csv	.= "\"".$limitedPasswd."\",";					//	
			$list_item_csv	.= "\"".$limitedPasswdPageTitle."\",";			//	
			$list_item_csv	.= "\"".$limitedPasswdPageMessage."\",";		//	
			$list_item_csv	.= "\"".$saleStatus."\",";						//	
			$list_item_csv	.= "\"".$itemOption1."\",";						//	
			$list_item_csv	.= "\"".$itemOption2."\",";						//	
			$list_item_csv	.= "\"".$itemOption3."\",";						//	
			$list_item_csv	.= "\"".$itemOption4."\",";						//	
			$list_item_csv	.= "\"".$itemOption5."\",";						//	
			$list_item_csv	.= "\"".$itemOption6."\",";						//	
			$list_item_csv	.= "\"".$itemOption7."\",";						//	
			$list_item_csv	.= "\"".$itemOption8."\",";						//	
			$list_item_csv	.= "\"".$itemOption9."\",";						//	
			$list_item_csv	.= "\"".$itemOption10."\",";					//	
			$list_item_csv	.= "\"".$itemOption11."\",";					//	
			$list_item_csv	.= "\"".$itemOption12."\",";					//	
			$list_item_csv	.= "\"".$itemOption13."\",";					//	
			$list_item_csv	.= "\"".$itemOption14."\",";					//	
			$list_item_csv	.= "\"".$itemOption15."\",";					//	
			$list_item_csv	.= "\"".$itemOption16."\",";					//	
			$list_item_csv	.= "\"".$itemOption17."\",";					//	
			$list_item_csv	.= "\"".$itemOption18."\",";					//	
			$list_item_csv	.= "\"".$itemOption19."\",";					//	
			$list_item_csv	.= "\"".$itemOption20."\",";					//	
			$list_item_csv	.= "\"".$pointRate."\",";						//	
			$list_item_csv	.= "\"".$stockSegment."\",";					//	
			$list_item_csv	.= "\"".$stockShippingDayId."\",";				//	
			$list_item_csv	.= "\"".$stockShippingDayDispTxt."\",";			//	
			$list_item_csv	.= "\"".$displayBackorderMessage."\",";			//	
			$list_item_csv	.= "\"".$stockCount."\",";						//	
			$list_item_csv	.= "\"".$displayStockSegment."\",";				//	
			$list_item_csv	.= "\"".$displayStockThreshold."\",";			//	
			$list_item_csv	.= "\"".$choicesStockHorizontalItemName."\",";	//	
			$list_item_csv	.= "\"".$choicesStockVerticalItemName."\",";	//	
			$list_item_csv	.= "\"".$choicesStockUpperDescription."\",";	//	
			$list_item_csv	.= "\"".$choicesStockLowerDescription."\",";	//	
			$list_item_csv	.= "\"".$displayChoicesStockSegment."\",";		//	
			$list_item_csv	.= "\"".$displayChoicesStockThreshold."\"";		//	
			$list_item_csv	.= "\r\n";

			foreach ($size AS $size_name => $size_cnt) {
				if ($size_cnt < 1 || ($stockCount < 1 && $choicesStockCount < 1)) {
					continue;
				}

				if ($stockSegment == 1) {
					$stockCount = 0;
				} else {
					$choicesStockCount = 0;
				}

				$choicesStockShippingDayId = "";

				$list_zaiko_csv	.= "\"U\",";									//	ctrlCol
				$list_zaiko_csv	.= "\"".$lotNumber."\",";						//	lotNumber
				$list_zaiko_csv	.= "\"".$itemCode."\",";						//	itemCode		Code
				$list_zaiko_csv	.= "\"".$stockSegment."\",";					//	stockSegment	SkuOptionTypeCol
				$list_zaiko_csv	.= "\"".$stockCount."\",";						//	stockCount		SkuItemNum
				$list_zaiko_csv	.= "\"".$choicesStockHorizontalName."\",";		//	choicesStockHorizontalName
				$list_zaiko_csv	.= "\"".$choicesStockHorizontalCode."\",";		//	choicesStockHorizontalCode
				$list_zaiko_csv	.= "\"".$choicesStockHorizontalSeq."\",";		//	choicesStockHorizontalSeq
				$list_zaiko_csv	.= "\"".$choicesStockVerticalName."\",";		//	choicesStockVerticalName	空
				$list_zaiko_csv	.= "\"".$choicesStockVerticalCode."\",";		//	choicesStockVerticalCode	空
				$list_zaiko_csv	.= "\"".$choicesStockVerticalSeq."\",";			//	choicesStockVerticalSeq		空
				$list_zaiko_csv	.= "\"".$choicesStockCount."\",";				//	choicesStockCount
				$list_zaiko_csv	.= "\"".$choicesStockShippingDayId."\",";		//	choicesStockShippingDayId	設定しない
				$list_zaiko_csv	.= "\"".$choicesStockShippingDayDispTxt."\",";	//	
				$list_zaiko_csv	.= "\"".$choicesStockImageUrl."\",";			//	add ohkawara 2018/12/17
				$list_zaiko_csv	.= "\"".$choicesStockColorSegment."\"";			//	add ohkawara 2018/12/17
				$list_zaiko_csv	.= "\r\n";
			}
		}
	}
}

//	Wowma
//	del ohkawara 2018/04/02
/*
function make_wowma_csv_bak($list, &$WOWMA_UP_LIST, &$list_item_csv, &$list_zaiko_csv, $del_flg) {
	global $WOWMA_GOODS_DISCOUNT_CATE;
	global $attention_wowma;
	global $L_BRAND;

	$size_list = $list['size_list'];

	$g_num = $list['g_num'];
	unset($WOWMA_UP_LIST[$g_num]);
	$wd_id = $list['wd_id'];
	$g_name = $list['g_name'];
	$base_price = $list['price'];
	$base_sale_price = $list['sale_price'];
	$caption = $list['caption'];
	$brand = $list['brand'];
	$code = $list['code'];
	$generic_keywords1 = $list['generic_keywords1'];
	$generic_keywords2 = $list['generic_keywords2'];
	$generic_keywords3 = $list['generic_keywords3'];
	$generic_keywords4 = $list['generic_keywords4'];
	$generic_keywords5 = $list['generic_keywords5'];

	$zaiko = $list['zaiko'];		//	add ohkawara 2017/06/15

	//--------------------------------------------//

	//	アップ済み商品のチェックとフラグを追加
	//	サイズが有る無しによって設定が有るので、サイズ確認を最初に行う
	$op_size = unserialize($size_list);
	$ALL_SIZE = array();
	if ($op_size) {
		$all_cnt = count($op_size);
		if ($all_cnt > 1) {
			$ALL_SIZE[0]['size'] = "";
			$ALL_SIZE[0]['jan'] = "";
			$ALL_SIZE[0]['asin'] = "";
		}
		$i = 1;
		$asin_flg = 0;
		foreach ($op_size AS $key => $VAL) {
			if ($VAL['type'] == 3) {
				//	サイズが非表示だったら飛ばす
				continue;
			}
			if ($VAL['jan'] == "none") {
				$VAL['jan'] = "";
			}

			if ($VAL['asin'] != "") {
				//	asinが1サイズでも登録されていた場合は、出品ファイルに保存する。
				//	その時にasinが入っていたサイズは販売しない。
				$asin_flg = 1;
			}

			if ($VAL['jan'] && !$VAL['asin']) {
				//	janコードが登録してありasinコードが登録されていない場合はアップしない
				continue;
			}

			//	サイズ確認	空設定用
			$check_size = $VAL['size'];
			$check_size = preg_replace("/\(日本サイズ\)/", "", $check_size);

			$check_size = preg_replace("/^F$/", "", $check_size);
			$check_size = preg_replace("/フリーサイズ/", "", $check_size);
			$check_size = preg_replace("/フリー/", "", $check_size);
			$check_size = preg_replace("/--/", "", $check_size);
			$check_size = preg_replace("/フットサルサイズ/", "", $check_size);
			$check_size = preg_replace("/設定なし/", "", $check_size);

			if ($check_size) {
				if ($i == 1 && !$ALL_SIZE[0] && $check_size) {
					$ALL_SIZE[0]['size'] = "";
					$ALL_SIZE[0]['jan'] = "";
					$ALL_SIZE[0]['asin'] = "";
				}
				$ALL_SIZE[$i]['size'] = $check_size;
				$ALL_SIZE[$i]['jan'] = $VAL['jan'];
				$ALL_SIZE[$i]['asin'] = $VAL['asin'];
				$i++;
			} else {
				$ALL_SIZE[0]['size'] = "";
				$ALL_SIZE[0]['jan'] = $VAL['jan'];
				$ALL_SIZE[0]['asin'] = $VAL['asin'];
			}
		}

		//	出品ファイルでアップする商品の場合中途半端な情報を削除する
		if($asin_flg == 1) {
			foreach ($ALL_SIZE AS $key => $VAL) {
				if ($key == 0) {
					unset($ALL_SIZE[0]);
				} elseif ($ALL_SIZE[$key]['jan'] && !$ALL_SIZE[$key]['asin']) {
					unset($ALL_SIZE[$key]);
				}
			}
		}
	}
	//	$ALL_SIZEの設定がなければこの商品の登録はしない（不具合があった場合）
	if (count($ALL_SIZE) < 1 && $del_flg != 1) {
		return;
	}

	$size_option_flg = 0;
	if ($ALL_SIZE[1]['size'] != "") {
		$size_option_flg = 1;
	}

	//--------------------------------------------//

	//	商品コード
	$Code = "g".$g_num;

	//	商品属性
	$BCategID = $wd_id;

	//	商品名
	$KtaiTitle = $g_name;

	//	販売価格
	//	割引設定項目
	$GOODS_DISCOUNT_C = WOWMA_GOODS_DISCOUNT_C;
	$GOODS_DISCOUNT_CATE = $WOWMA_GOODS_DISCOUNT_CATE;
	$DISCOUNT_PAR = WOWMA_DISCOUNT_PAR;
	$DISCOUNT_PAR2 = WOWMA_DISCOUNT_PAR2;

	if ($base_price > 0 && $base_sale_price > 0 && $base_price < $base_sale_price) {
		$sale_price = "";
		$price = $base_price;
	} elseif ($base_price > 0 && $base_sale_price > 0 && $base_price == $base_sale_price) {
		if ($GOODS_DISCOUNT_C == 1 && $DISCOUNT_PAR2 > 0) {
			$flag = 0;
			if ($GOODS_DISCOUNT_CATE) {
				foreach ($GOODS_DISCOUNT_CATE AS $val) {
					$val = trim($val);
					if ($cate1 == $val) { $flag = 1; }
				}
			}
			if ($flag != 1) {
				$sale_price = floor($base_price * ((100 - $DISCOUNT_PAR2) / 100));
			} else {
				$sale_price = "";
			}
		} else {
			$sale_price = "";
		}
		$price = $base_price;
	} elseif ($GOODS_DISCOUNT_C == 1) {
		$flag = 0;
		if ($GOODS_DISCOUNT_CATE) {
			foreach ($GOODS_DISCOUNT_CATE AS $val) {
				$val = trim($val);
				if ($cate1 == $val) { $flag = 1; }
			}
		}
		if ($flag != 1) {
			$sale_price_ = floor($base_price * ((100 - $DISCOUNT_PAR) / 100));
			if ($base_sale_price < 1) {
				$price = $base_price;
				$sale_price = $sale_price_;
			} elseif ($base_sale_price > $sale_price_) {
				$price = $base_sale_price;
				$sale_price = $sale_price_;
			} else {
				$price = $base_price;
				$sale_price = $base_sale_price;
			}
		} else {
			if ($base_sale_price) {
				$price = $base_sale_price;
				$sale_price = "";
			} else {
				$price = $base_price;
				$sale_price = "";
			}
		}
	}

	//if (TAXABLE == 1) {												//	del ookawara 2017/04/21
	$price = floor($price * ((TAX / 100) + 1) + 0.5);
	$sale_price = floor($sale_price * ((TAX / 100) + 1) + 0.5);
	//}																	//	del ookawara 2017/04/21
	if ($sale_price < 1) { unset($sale_price); }
	$KtaiPrice = $price;
	if ($sale_price > 0) {
		$KtaiPrice = $sale_price;
	}

	//	商品説明
	$KtaiCaption = "";
	$att_check = 0;
	if (preg_match("/<!--[\s]*Attention[\s]*-->/", $caption)) {
		$KtaiCaption .= $attention_wowma."<br>";
		$att_check = 1;
	}
	$caption = preg_replace("/<!--[\s]*Attention[\s]*-->/", "", $caption);
	$caption = preg_replace("/<br \/>/", "<br>", $caption);
	$KtaiCaption .= "ブランド名：".$L_BRAND[$brand]."<br>";
	$KtaiCaption .= $caption."<br>";
	$KtaiCaption = nl2br($KtaiCaption, false);
	$KtaiCaption = preg_replace('/(?:\n|\r|\r\n)/', '', $KtaiCaption );
	strip_tags($KtaiCaption, '<br><center><font><hr><a><b>');
	$KtaiCaption = trim($KtaiCaption);
	if (strlen($KtaiCaption) > 1024) {
#		$KtaiCaption = mb_strimwidth($KtaiCaption, 0, 1000, "", "eucjp-win");	//	1024だけど縮まらないので1000
		$KtaiCaption = mb_strimwidth($KtaiCaption, 0, 1000, "", "UTF-8");	//	1024だけど縮まらないので1000
	}
	$KtaiCaption = trim($KtaiCaption);

	//	決済方法
	$KtaiDealType = "CBMrtsPhic";	//	DeNAショッピング決済サービス 全サービス利用

	//	決算手数料
	$KtaiChargeType = "CBPhi";	//	DeNAショッピング決済サービス利用後

	//	管理ID
	$AdmID = $code;	//	品番

	//	基本在庫数
	$StockNum = "";
	if ($size_option_flg != 1) {
		//if ($att_check == 1) {					//	del ohkawara 2017/06/16
		if ($att_check == 1 || $zaiko == 1) {		//	add ohkawara 2017/06/16
			$StockNum = 1;
		} else {
			$StockNum = WOWMA_DEFO_ZAIKO;
		}
	}

	//	検索キーワード(3つまで)
	$Keyword = "";
	for ($i=1; $i<=3; $i++) {
		$key_names = "generic_keywords".$i;
		if ($Keyword != "") {
			$Keyword .= ":";
		}
		$names = $$key_names;
		$size_flg = 0;
		if (strlen($names) > 20) {
			$size_flg = 1;
		}
		if ($names && $size_flg != 1) {
			$Keyword .= $names;
		} else {
			$Keyword .= "NULL";
		}
	}

	//	ポイント付与率
	$KtaiUserRate = WOWMA_POINT;

	//	画像リンク名1-10
	//	画像タイプ1-10
	//	画像URL1-10
	for ($i=1; $i<=10; $i++) {
		$KILN = "KtaiImageLinkName".$i;
		$KIT  = "KtaiImageType".$i;
		$KIU  = "KtaiImageURL".$i;

		$image_file = "";

		if ($i == 1) {
			$image_file = IMG_F_DIR."/$code" . ".jpg";
		} elseif ($i == 2) {
			$image_file = IMG_B_DIR."/$code" . ".jpg";
		} elseif ($i == 3) {
			$image_file = IMG_03_DIR."/$code" . ".jpg";
		} elseif ($i == 4) {
			$image_file = IMG_04_DIR."/$code" . ".jpg";
		} elseif ($i == 5) {
			$image_file = IMG_05_DIR."/$code" . ".jpg";
		} elseif ($i == 6) {
			$image_file = IMG_06_DIR."/$code" . ".jpg";
		} elseif ($i == 7) {
			$image_file = IMG_07_DIR."/$code" . ".jpg";
		} elseif ($i == 8) {
			$image_file = IMG_08_DIR."/$code" . ".jpg";
		} elseif ($i == 9) {
			$image_file = IMG_09_DIR."/$code" . ".jpg";
		} elseif ($i == 10) {
			//	10枚目は無し
			$image_file = IMG_10_DIR."/$code" . ".jpg";
		}

		$$KILN = "NULL";
		$$KIT  = "N";
		$$KIU  = "";
		if (file_exists($image_file)) {
			$$KIT  = "L";

			$fd_name = "";
			if ($i == 1) {
				$fd_name = "imagef";
			} elseif ($i == 2) {
				$fd_name = "imageb";
			} else {
				$fd_name = "image".sprintf("%02s", $i);
			}
			$$KIU = "http://www.futboljersey.com/".$fd_name."/".$code.".jpg";
		}
	}

	//	送料設定
	$ShipFeeType = "B";	//	送料別

	//	個別送料
	$ShipFee = "";	//	設定無し

	//	メール便設定
	$DeliveryType = "";	//	設定無し

	//	送料コメント
	$KtaiShipComment = "";	//	設定無し

	//	決算方法コメント
	$KtaiDealComment = "";	//	設定無し

	//	販売価格コメント1
	$KtaiPriceLabel1 = "";	//	設定無し

	//	販売価格コメント2
	$KtaiPriceLabel2 = "";	//	設定無し

	//	PC用商品説明文
	$KtaiCaptionPC = "";	//	設定無し

	//	スマートフォン用商品説明文
	$KtaiCaptionSP = "";	//	設定無し

	//	商品詳細説明
	$KtaiDetailLinkName = "";	//	設定無し

	//	商品詳細説明
	$KtaiDetailComment = "";	//	設定無し

	//	購入オプション1-5	//	サイズ選択で必要時は利用する
	$Option1 = "";
	$Option2 = "";
	$Option3 = "";
	$Option4 = "";
	$Option5 = "";

	//	発売入荷予定日
	$SalesStartDate = "";	//	設定無し

	//	掲載開始日時
	$StartDate = "";

	//	販売状態
	$ExhibitStatus = "O";	//	出品中

	//	管理用商品名
	$AdmTitle = "";	//	設定無し

	//	JANコード
	//	ISBNコード
	//	両方とも13桁になった為判別できない為書籍関係は出品却下！
	$JanCode = "";
	$IsbnCode = "";
	foreach ($ALL_SIZE AS $key => $VAL) {
		if ($ALL_SIZE[$key]['jan']) {
			if (strlen($ALL_SIZE[$key]['jan']) == 13) {
				$JanCode = $ALL_SIZE[$key]['jan'];
				break;
			} elseif (strlen($ALL_SIZE[$key]['jan']) == 10) {
				$IsbnCode = $ALL_SIZE[$key]['jan'];
				break;
			}
		}
	}

	//	型番
	$NameModel = "";	//	設定無し

	//	秘密ページパスワード
	$SecretPagePassword = "";	//	設定無し

	//	秘密ページタイトル
	$SecretPageTitle = "";	//	設定無し

	//	秘密ページメッセージ
	$SecretPageMessage = "";	//	設定無し

	//	在庫表示タイプ
	$StockShow = "H";	//	非表示設定

	//	在庫切れメッセージ
	$KtaiNoStockMessage = "";	//	設定無し

	//	スペックリンク名
	$KtaiSpecLinkName = "";	//	設定無し

	//	スペック1-5
	$KtaiSpec1 = "";	//	設定無し
	$KtaiSpec2 = "";	//	設定無し
	$KtaiSpec3 = "";	//	設定無し
	$KtaiSpec4 = "";	//	設定無し
	$KtaiSpec5 = "";	//	設定無し

	//	最大購入個数
	$MaxBuyNum = "";
	if ($att_check == 1) {
		$MaxBuyNum = 1;
	} else {
		$MaxBuyNum = WOWMA_DEFO_ZAIKO;
	}

	//	在庫数表示許容数
	$KtaiStockShowStartNum = "";	//	設定しない

	//	在庫表示メッセージ1－2
	$KtaiStockMessage1 = "";	//	設定無し
	$KtaiStockMessage2 = "";	//	設定無し

	//	消費税
	$Tax = "I";	//	内税

	//	在庫管理タイプ
	$StockType = "B";
	if ($size_option_flg == 1) {
		$StockType = "S";
	}

	//	SKU横軸項目名
	$SkuOptionNameCol = "";
	if ($size_option_flg == 1) {
		$SkuOptionNameCol = "サイズ";
	}

	//	SKU横軸選択枠
	$SkuOptionValueCol = "";
	//	SKU横軸管理ID
	$SkuOptionTypeCol = "";

	$SET_LIST_V = array();
	$SET_LIST_T = array();
	if ($size_option_flg == 1) {
		foreach ($ALL_SIZE AS $key => $VAL) {
			if ($VAL['size']) {
				$SET_LIST_V[] = $VAL['size'];
				$SET_LIST_T[] = "-".$VAL['size'];
			}
		}
		$SkuOptionValueCol = implode(":", $SET_LIST_V);
		$SkuOptionTypeCol = implode(":", $SET_LIST_T);
	}

	//	SKU縦軸項目名
	$SkuOptionNameRow = "";	//	特になし

	//	SKU縦軸選択枠
	$SkuOptionValueRow = "";	//	特になし

	//	SKU縦軸管理ID
	$SkuOptionTypeRow = "";	//	特になし

	//	SKUヘッダ情報
	$SkuHeader = "";	//	特になし

	//	SKUフッダー情報
	$SkuFooter = "";	//	特になし

	//	SKU在庫表示タイプ
	$SkuStockShowType = "H";	//	在庫数の数を数多く設定しない為無し
	if ($StockType == "B") {
		$SkuStockShowType = "";
	}

	//	SKU在庫表示閾値
	$SkuLimitNum = "";	//	在庫数の数を数多く設定しない為無し

	//	出品先
	$exhibittype = "MX";

	//--------------------------------------------//

	//	削除用の商品の場合
	if ($del_flg == 1 && $list) {
		foreach ($list AS $key => $val) {
			$$key = $val;
		}
		$StockNum = "0";
		$StockType = "B";
		$SkuOptionNameCol = "";
		$SkuOptionValueCol = "";
		$SkuOptionTypeCol = "";
		$SkuStockShowType = "";
		$SkuLimitNum = "";
	}

	//	csv設定
	$list_item_csv	.= "\"".$Code."\",";					//	Code
	$list_item_csv	.= "\"".$BCategID."\",";				//	BCategID
	$list_item_csv	.= "\"".$KtaiTitle."\",";				//	KtaiTitle
	$list_item_csv	.= "\"".$KtaiPrice."\",";				//	KtaiPrice
	$list_item_csv	.= "\"".$KtaiCaption."\",";				//	KtaiCaption
	$list_item_csv	.= "\"".$KtaiDealType."\",";			//	KtaiDealType
	$list_item_csv	.= "\"".$KtaiChargeType."\",";			//	KtaiChargeType
	$list_item_csv	.= "\"".$AdmID."\",";					//	AdmID
	$list_item_csv	.= "\"".$StockNum."\",";				//	StockNum
	$list_item_csv	.= "\"".$Keyword."\",";					//	Keyword
	$list_item_csv	.= "\"".$KtaiUserRate."\",";			//	KtaiUserRate
	$list_item_csv	.= "\"".$KtaiImageLinkName1."\",";		//	KtaiImageLinkName1
	$list_item_csv	.= "\"".$KtaiImageType1."\",";			//	KtaiImageType1
	$list_item_csv	.= "\"".$KtaiImageURL1."\",";			//	KtaiImageURL1
	$list_item_csv	.= "\"".$KtaiImageLinkName2."\",";		//	KtaiImageLinkName2
	$list_item_csv	.= "\"".$KtaiImageType2."\",";			//	KtaiImageType2
	$list_item_csv	.= "\"".$KtaiImageURL2."\",";			//	KtaiImageURL2
	$list_item_csv	.= "\"".$KtaiImageLinkName3."\",";		//	KtaiImageLinkName3
	$list_item_csv	.= "\"".$KtaiImageType3."\",";			//	KtaiImageType3
	$list_item_csv	.= "\"".$KtaiImageURL3."\",";			//	KtaiImageURL3
	$list_item_csv	.= "\"".$KtaiImageLinkName4."\",";		//	KtaiImageLinkName4
	$list_item_csv	.= "\"".$KtaiImageType4."\",";			//	KtaiImageType4
	$list_item_csv	.= "\"".$KtaiImageURL4."\",";			//	KtaiImageURL4
	$list_item_csv	.= "\"".$KtaiImageLinkName5."\",";		//	KtaiImageLinkName5
	$list_item_csv	.= "\"".$KtaiImageType5."\",";			//	KtaiImageType5
	$list_item_csv	.= "\"".$KtaiImageURL5."\",";			//	KtaiImageURL5
	$list_item_csv	.= "\"".$KtaiImageLinkName6."\",";		//	KtaiImageLinkName6
	$list_item_csv	.= "\"".$KtaiImageType6."\",";			//	KtaiImageType6
	$list_item_csv	.= "\"".$KtaiImageURL6."\",";			//	KtaiImageURL6
	$list_item_csv	.= "\"".$KtaiImageLinkName7."\",";		//	KtaiImageLinkName7
	$list_item_csv	.= "\"".$KtaiImageType7."\",";			//	KtaiImageType7
	$list_item_csv	.= "\"".$KtaiImageURL7."\",";			//	KtaiImageURL7
	$list_item_csv	.= "\"".$KtaiImageLinkName8."\",";		//	KtaiImageLinkName8
	$list_item_csv	.= "\"".$KtaiImageType8."\",";			//	KtaiImageType8
	$list_item_csv	.= "\"".$KtaiImageURL8."\",";			//	KtaiImageURL8
	$list_item_csv	.= "\"".$KtaiImageLinkName9."\",";		//	KtaiImageLinkName9
	$list_item_csv	.= "\"".$KtaiImageType9."\",";			//	KtaiImageType9
	$list_item_csv	.= "\"".$KtaiImageURL9."\",";			//	KtaiImageURL9
	$list_item_csv	.= "\"".$KtaiImageLinkName10."\",";		//	KtaiImageLinkName10
	$list_item_csv	.= "\"".$KtaiImageType10."\",";			//	KtaiImageType10
	$list_item_csv	.= "\"".$KtaiImageURL10."\",";			//	KtaiImageURL10
	$list_item_csv	.= "\"".$ShipFeeType."\",";				//	ShipFeeType
	$list_item_csv	.= "\"".$ShipFee."\",";					//	ShipFee
	$list_item_csv	.= "\"".$DeliveryType."\",";			//	DeliveryType
	$list_item_csv	.= "\"".$KtaiShipComment."\",";			//	KtaiShipComment
	$list_item_csv	.= "\"".$KtaiDealComment."\",";			//	KtaiDealComment
	$list_item_csv	.= "\"".$KtaiPriceLabel1."\",";			//	KtaiPriceLabel1
	$list_item_csv	.= "\"".$KtaiPriceLabel2."\",";			//	KtaiPriceLabel2
	$list_item_csv	.= "\"".$KtaiCaptionPC."\",";			//	KtaiCaptionPC
	$list_item_csv	.= "\"".$KtaiCaptionSP."\",";			//	KtaiCaptionSP
	$list_item_csv	.= "\"".$KtaiDetailLinkName."\",";		//	KtaiDetailLinkName
	$list_item_csv	.= "\"".$KtaiDetailComment."\",";		//	KtaiDetailComment
	$list_item_csv	.= "\"".$Option1."\",";					//	Option1
	$list_item_csv	.= "\"".$Option2."\",";					//	Option2
	$list_item_csv	.= "\"".$Option3."\",";					//	Option3
	$list_item_csv	.= "\"".$Option4."\",";					//	Option4
	$list_item_csv	.= "\"".$Option5."\",";					//	Option5
	$list_item_csv	.= "\"".$SalesStartDate."\",";			//	SalesStartDate
	$list_item_csv	.= "\"".$StartDate."\",";				//	StartDate
	$list_item_csv	.= "\"".$ExhibitStatus."\",";			//	ExhibitStatus
	$list_item_csv	.= "\"".$AdmTitle."\",";				//	AdmTitle
	$list_item_csv	.= "\"".$JanCode."\",";					//	JanCode
	$list_item_csv	.= "\"".$IsbnCode."\",";				//	IsbnCode
	$list_item_csv	.= "\"".$NameModel."\",";				//	NameModel
	$list_item_csv	.= "\"".$SecretPagePassword."\",";		//	SecretPagePassword
	$list_item_csv	.= "\"".$SecretPageTitle."\",";			//	SecretPageTitle
	$list_item_csv	.= "\"".$SecretPageMessage."\",";		//	SecretPageMessage
	$list_item_csv	.= "\"".$StockShow."\",";				//	StockShow
	$list_item_csv	.= "\"".$KtaiNoStockMessage."\",";		//	KtaiNoStockMessage
	$list_item_csv	.= "\"".$KtaiSpecLinkName."\",";		//	KtaiSpecLinkName
	$list_item_csv	.= "\"".$KtaiSpec1."\",";				//	KtaiSpec1
	$list_item_csv	.= "\"".$KtaiSpec2."\",";				//	KtaiSpec2
	$list_item_csv	.= "\"".$KtaiSpec3."\",";				//	KtaiSpec3
	$list_item_csv	.= "\"".$KtaiSpec4."\",";				//	KtaiSpec4
	$list_item_csv	.= "\"".$KtaiSpec5."\",";				//	KtaiSpec5
	$list_item_csv	.= "\"".$MaxBuyNum."\",";				//	KtaMaxBuyNumiSpec5
	$list_item_csv	.= "\"".$KtaiStockShowStartNum."\",";	//	KtaiStockShowStartNum
	$list_item_csv	.= "\"".$KtaiStockMessage1."\",";		//	KtaiStockMessage1
	$list_item_csv	.= "\"".$KtaiStockMessage2."\",";		//	KtaiStockMessage2
	$list_item_csv	.= "\"".$Tax."\",";						//	Tax
	$list_item_csv	.= "\"".$StockType."\",";				//	StockType
	$list_item_csv	.= "\"".$SkuOptionNameCol."\",";		//	SkuOptionNameCol
	$list_item_csv	.= "\"".$SkuOptionValueCol."\",";		//	SkuOptionValueCol
	$list_item_csv	.= "\"".$SkuOptionTypeCol."\",";		//	SkuOptionTypeCol
	$list_item_csv	.= "\"".$SkuOptionNameRow."\",";		//	SkuOptionNameRow
	$list_item_csv	.= "\"".$SkuOptionValueRow."\",";		//	SkuOptionValueRow
	$list_item_csv	.= "\"".$SkuOptionTypeRow."\",";		//	SkuOptionTypeRow
	$list_item_csv	.= "\"".$SkuHeader."\",";				//	SkuHeader
	$list_item_csv	.= "\"".$SkuFooter."\",";				//	SkuFooter
	$list_item_csv	.= "\"".$SkuStockShowType."\",";		//	SkuStockShowType
	$list_item_csv	.= "\"".$SkuLimitNum."\",";				//	SkuLimitNum
	$list_item_csv	.= "\"".$exhibittype."\"";				//	exhibittype
	$list_item_csv	.= "\r\n";


	if (count($SET_LIST_T) < 1 || $del_flg == 1) {	//	選択肢が無い場合、または削除用の場合
		return;
	}

	//	在庫ファイル
	//	$Code	//	設定済み
	$SkuItemNum = 0;
	//if ($att_check == 1) {					//	del ohkawara 2017/06/16
	if ($att_check == 1 || $zaiko == 1) {		//	add ohkawara 2017/06/16
		$SkuItemNum = 1;
	} else {
		$SkuItemNum = WOWMA_DEFO_ZAIKO;
	}
	$SkuOptionTypeRow = "";	//	設定無し

	foreach ($SET_LIST_T AS $val) {
		$SkuOptionTypeCol = $val;

		$list_zaiko_csv	.= "\"".$Code."\",";					//	Code
		$list_zaiko_csv	.= "\"".$SkuOptionTypeCol."\",";		//	SkuOptionTypeCol
		$list_zaiko_csv	.= "\"".$SkuOptionTypeRow."\",";		//	SkuOptionTypeRow
		$list_zaiko_csv	.= "\"".$SkuItemNum."\"";				//	SkuItemNum
		$list_zaiko_csv	.= "\r\n";
	}

}
*/

//-------------------------------------------------------------------------------------
//
//	共通
//
//-------------------------------------------------------------------------------------

//	画像更新チェック
function pic_check() {
global $conn_id;

	if (file_exists(CHECK_FILE)) {
		$CHECKLIST = file(CHECK_FILE);
		$check_time = trim($CHECKLIST[0]);
	}

	if ($check_time) {
		define("CHECK_TIME",$check_time);
		//	一枚目チェック
		unset($LISTF);
		if (file_exists(IMG_F_DIR)) {
			$strDir = opendir(IMG_F_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_F_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LISTF[] = eregi_replace(".jpg$","",$str);
					$LISTF[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}

		//	二枚目チェック
		unset($LISTB);
		if (file_exists(IMG_B_DIR)) {
			$strDir = opendir(IMG_B_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_B_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LISTB[] = eregi_replace(".jpg$","",$str);
					$LISTB[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}

		//	add ookawara 2013/07/03
		//	3枚目チェック
		unset($LIST03);
		if (file_exists(IMG_03_DIR)) {
			$strDir = opendir(IMG_03_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_03_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LIST03[] = eregi_replace(".jpg$","",$str);
					$LIST03[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}
		//	4枚目チェック
		unset($LIST04);
		if (file_exists(IMG_04_DIR)) {
			$strDir = opendir(IMG_04_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_04_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LIST04[] = eregi_replace(".jpg$","",$str);
					$LIST04[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}
		//	5枚目チェック
		unset($LIST05);
		if (file_exists(IMG_05_DIR)) {
			$strDir = opendir(IMG_05_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_05_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LIST05[] = eregi_replace(".jpg$","",$str);
					$LIST05[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}

		//	add ookawara 2015/09/27
		//	6枚目チェック
		unset($LIST06);
		if (file_exists(IMG_06_DIR)) {
			$strDir = opendir(IMG_06_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_06_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LIST06[] = eregi_replace(".jpg$","",$str);
					$LIST06[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}
		//	7枚目チェック
		unset($LIST07);
		if (file_exists(IMG_07_DIR)) {
			$strDir = opendir(IMG_07_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_07_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LIST07[] = eregi_replace(".jpg$","",$str);
					$LIST07[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}
		//	8枚目チェック
		unset($LIST08);
		if (file_exists(IMG_08_DIR)) {
			$strDir = opendir(IMG_08_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_08_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LIST08[] = eregi_replace(".jpg$","",$str);
					$LIST08[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}
		//	9枚目チェック
		unset($LIST09);
		if (file_exists(IMG_09_DIR)) {
			$strDir = opendir(IMG_09_DIR);
			while($str = readdir($strDir)) {
##				if (eregi("^\.",$str)) { continue; }
				if (preg_match("/^\./i",$str)) { continue; }
				clearstatcache();
				$file = IMG_09_DIR."/$str";
				if ($check_time < filemtime($file)) {
##					$LIST09[] = eregi_replace(".jpg$","",$str);
					$LIST09[] = preg_replace("/\.jpg$/i","",$str);
				}
			}
		}

		//	商品番号抜き出し
		unset($NEW_LIST);
		$NEW_CHECK_LIST = array();	//	add ookawara 2013/07/04
		if ($LISTF) {
			foreach ($LISTF AS $val) {
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
						$NEW_CHECK_LIST[$val] = $g_num;
					}
				}
			}
		}

		if ($LISTB) {
			foreach ($LISTB AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }	//	add ookawara 2013/07/04
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}

		//	add ookawara 2013/07/04
		if ($LIST03) {
			foreach ($LIST03 AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}

		if ($LIST04) {
			foreach ($LIST04 AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}

		if ($LIST05) {
			foreach ($LIST05 AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}

		//	add ookawara 2015/09/27
		if ($LIST06) {
			foreach ($LIST06 AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}
		if ($LIST07) {
			foreach ($LIST07 AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}
		if ($LIST08) {
			foreach ($LIST08 AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}
		if ($LIST09) {
			foreach ($LIST09 AS $val) {
				if ($val != "" && $NEW_CHECK_LIST[$val]) { continue; }
				$sql  = "SELECT g_num FROM ".T_GOODS.
						" WHERE code='$val';";
				if ($result = mysqli_query($conn_id,$sql)) {
					while ($list = mysqli_fetch_array($result)) {
						$g_num = $list['g_num'];
						$NEW_LIST[$g_num] = $g_num;
					}
				}
			}
		}


		if ($NEW_LIST) {
			foreach ($NEW_LIST AS $key => $val) {
				$sql  = "UPDATE ".T_CATEGORY." SET".
						" state='2'".
						" WHERE g_num='$key' AND state='0';";
				if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "更新できませんでした。"; }
			}
		}
	}

	return $ERROR;

}



//	画像アップ空フォルダー削除
function del_dir() {

	//	フロント
	if (file_exists(UP_IMG_01_DIR)) {
		$strDir = opendir(UP_IMG_01_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_01_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}

	//	バック
	if (file_exists(UP_IMG_02_DIR)) {
		$strDir = opendir(UP_IMG_02_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_02_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}

	//	add ookawara 2013/07/04
	//	3枚目
	if (file_exists(UP_IMG_03_DIR)) {
		$strDir = opendir(UP_IMG_03_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_03_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}

	//	4枚目
	if (file_exists(UP_IMG_04_DIR)) {
		$strDir = opendir(UP_IMG_04_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_04_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}

	//	5枚目
	if (file_exists(UP_IMG_05_DIR)) {
		$strDir = opendir(UP_IMG_05_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_05_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if ( preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}

	//	2015/09/27
	//	6枚目
	if (file_exists(UP_IMG_06_DIR)) {
		$strDir = opendir(UP_IMG_06_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_06_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}
	//	7枚目
	if (file_exists(UP_IMG_07_DIR)) {
		$strDir = opendir(UP_IMG_07_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_07_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}
	//	8枚目
	if (file_exists(UP_IMG_08_DIR)) {
		$strDir = opendir(UP_IMG_08_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_08_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}
	//	9枚目
	if (file_exists(UP_IMG_09_DIR)) {
		$strDir = opendir(UP_IMG_09_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_09_DIR."/$str";
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}


	//	各フォルダー
	if (file_exists(UP_IMG_DIR)) {
		$strDir = opendir(UP_IMG_DIR);
		while($str = readdir($strDir)) {
##			if (ereg("\.",$str)) { continue; }
			if (preg_match("/\./",$str)) { continue; }
			$sub_dir = UP_IMG_DIR."/".$str;
			if (file_exists($sub_dir)) {
				$flag = 0;
				$strDir2 = opendir($sub_dir);
				while($str2 = readdir($strDir2)) {
##					if (ereg("^\.",$str2)) { continue; }
					if (preg_match("/^\./",$str2)) { continue; }
					$sub_piv = "$sub_dir/$str2";
					if (file_exists($sub_piv)) {
						$flag = 1;
						break;
					}
				}
				if ($flag != 1) { rmdir($sub_dir); }
			}
		}
	}

}



//	csvファイル削除
function del() {

	//	Yahoo
	if (file_exists(NEW_YAHOO_ALL_FILE)) {
		unlink(NEW_YAHOO_ALL_FILE);
		$msg .= "新Yahooファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(YAHOO_ALL_FILE)) {
		unlink(YAHOO_ALL_FILE);
		$msg .= "旧Yahooファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(NEW_YAHOO_ZAIKO_FILE)) {
		unlink(NEW_YAHOO_ZAIKO_FILE);
		$msg .= "Yahoo在庫ファイルを削除しました。<br>\n<br>\n";
	}

	//	Rakuten
	if (file_exists(RAKUTEN_ITEM_FILE)) {
		unlink(RAKUTEN_ITEM_FILE);
		$msg .= "楽天商品ファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(RAKUTEN_SELECT_FILE)) {
		unlink(RAKUTEN_SELECT_FILE);
		$msg .= "楽天商品選択ファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(RAKUTEN_CAT_FILE)) {
		unlink(RAKUTEN_CAT_FILE);
		$msg .= "楽天カテゴリーファイルを削除しました。<br>\n<br>\n";
	}


	//	Amazon
	//	add ookawara 2015/06/26
	if (file_exists(AMAZON_AP_ITEM_FILE)) {
		unlink(AMAZON_AP_ITEM_FILE);
		$msg .= "Amazonファッション＆シューズ在庫ファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(AMAZON_SP_ITEM_FILE)) {
		unlink(AMAZON_SP_ITEM_FILE);
		$msg .= "Amazonスポーツ在庫ファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(AMAZON_JAN_ITEM_FILE)) {
		unlink(AMAZON_JAN_ITEM_FILE);
		$msg .= "Amazon出品ファイルを削除しました。<br>\n<br>\n";
	}


	//	Wowma
	//	add ohkawara 2017/02/09
	if (file_exists(WOWMA_ITEM_FILE)) {
		unlink(WOWMA_ITEM_FILE);
		$msg .= "Wowma商品ファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(WOWMA_SELECT_FILE)) {
		unlink(WOWMA_SELECT_FILE);
		$msg .= "WowmaSelectファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(WOWMA_ZAIKO_FILE)) {
		unlink(WOWMA_ZAIKO_FILE);
		$msg .= "Wowma在庫ファイルを削除しました。<br>\n<br>\n";
	}


	return array($msg,$ERROR);

}



//	更新データー削除
function del_renew() {
// global $delpass; //　未使用
global $conn_id;

	//if (!$ERROR && $delpass == "ookawara") {	//	del ookawara 2016/01/27
	if (!$ERROR) {								//	add ookawara 2016/01/27
		//	新規オプション情報削除
		//$sql = "SELECT a.g_num, a.options FROM ".T_GOODS." a, ".T_CATEGORY." b" .
		//		" WHERE a.g_num=b.g_num AND a.options LIKE '%/%' AND b.cate1!='99';";

		$sql  = "SELECT a.g_num, a.size_list FROM ".T_GOODS." a, ".T_CATEGORY." b".
				" WHERE a.g_num=b.g_num".
				" AND a.size_list LIKE '%\"type\";s:1:\"2\"%'".
				//" AND b.cate1!='99';";					//	del ookawara 2016/02/15
				" AND (b.cate1!='98' || b.cate1!='99');";	//	add ookawara 2016/02/15

		if ($result = mysqli_query($conn_id,$sql)) {
			while ($list = mysqli_fetch_array($result)) {
				$g_num = $list['g_num'];
				//	add ookawara 2015/09/22 start
				$set_size_list = "";
				$size_list = $list['size_list'];
				$op_size = unserialize($size_list);
				foreach ($op_size AS $key => $VAL) {
					if ($VAL['type'] == 2) {
						$op_size[$key]['type'] = 1;
					}
				}
				$set_size_list = serialize($op_size);
				$sql2 = "UPDATE ".T_GOODS." SET size_list='$set_size_list' WHERE g_num='$g_num';";
				//	add ookawara 2015/09/22 end

				//$options = $list['options'];													//	del ookawara 2015/09/22
##				//$options = ereg_replace("\/","",$options);									//	del ookawara 2015/09/22
				//$options = preg_replace("/\//","",$options);									//	del ookawara 2015/09/22
				//$options = trim($options);													//	del ookawara 2015/09/22
				//$sql2 = "UPDATE ".T_GOODS." SET size_list='$size_list' WHERE g_num='$g_num';";	//	del ookawara 2015/09/22
				if (!mysqli_query($conn_id,$sql2)) { $ERROR[] = "商品オプションデーターを更新できませんでした。$g_num"; }
			}
		}

		if (!file_exists(CHECK_TMP_FILE)) {	//	add ookawara 2013/12/09
			$check_time = time();
			$OUT = fopen(CHECK_FILE,"w");
			fwrite($OUT,$check_time);
			fclose($OUT);
			@chmod(CHECK_FILE,0666);
		//	add ookawara 2013/12/09 start
		} else {
			if (file_exists(CHECK_FILE)) {
				unlink(CHECK_FILE);
			}
			rename( CHECK_TMP_FILE, CHECK_FILE );
		}
		//	add ookawara 2013/12/09 end

		//	カテゴリー商品データー更新
		$sql = "UPDATE ".T_CATEGORY." SET state='0' WHERE state>'0' AND state<='2';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "カテゴリーの商品データーを更新できませんでした。"; }

		//	カテゴリー商品データー削除
		$sql = "DELETE FROM ".T_CATEGORY." WHERE state='3';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "カテゴリーから商品データーを削除できませんでした。"; }

		//	削除テーブル削除
		//$sql = "DELETE FROM ".T_DELGOODS.";";																	//	del ookawara 2015/08/11
		//if (!pg_exec($conn_id,$sql)) { $ERROR[] = "削除商品データーをクリアーできませんでした。"; }			//	del ookawara 2015/08/11

		$delpass = "";
	}

	//	画像更新情報削除
	//	add ookawara 2014/02/06
	if (file_exists(CHECK_FILE)) {
		$CHECKLIST = file(CHECK_FILE);
		$check_time = trim($CHECKLIST[0]);

		$del_time = date("Y-m-d H:i:s", $check_time);
		$sql  = "DELETE FROM ".T_UP_PIC.
				" WHERE regist_date<'".$del_time."';";
		if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "削除商品データーをクリアーできませんでした。"; }
	}

	if (!$ERROR) { $msg = "更新情報クリアーしました。<br>\n<br>\n"; }

	return array($msg,$ERROR);

}



// 全商品更新
function all_renew() {
global $conn_id;

	$sql  = "UPDATE ".T_CATEGORY." SET".
			" state='2'".
			" WHERE state='0';";
	if (!mysqli_query($conn_id,$sql)) { $ERROR[] = "全商品更新データーに変更できませんでした。"; }

	if (!$ERROR) { $msg = "全商品更新データーに更新しました。<br>\n<br>\n"; }

	return array($msg,$ERROR);

}


//	[]()変換
//	add ookawara 2014/10/10
function change_en($text) {

##	$text = ereg_replace("\[","\\[",$text);
	$text = preg_replace("/\[/","\\[",$text);
##	$text = ereg_replace("\{","\\{",$text);	//	add ookawara 2009/10/02
	$text = preg_replace("/\{/","\\{",$text);	//	add ookawara 2009/10/02
##	$text = ereg_replace("\(","\\(",$text);
	$text = preg_replace("/\(/","\\(",$text);
##	$text = ereg_replace("\)","\\)",$text);
	$text = preg_replace("/\)/","\\)",$text);
##	$text = ereg_replace("\}","\\}",$text);	//	add ookawara 2009/10/02
	$text = preg_replace("/\}/","\\}",$text);	//	add ookawara 2009/10/02
##	$text = ereg_replace("\]","\\]",$text);
	$text = preg_replace("/\]/","\\]",$text);
##	$text = ereg_replace("\*","\\*",$text);
	$text = preg_replace("/\*/","\\*",$text);
##	$text = ereg_replace("\!","\\!",$text);
	$text = preg_replace("/\!/","\\!",$text);
	return $text;
}
