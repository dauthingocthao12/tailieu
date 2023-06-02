<?PHP

//	設定項目

//	最上階層にカテゴリーを追加した場合、DBに追加とarray.incファイル追加を忘れずに

//	2009/10/30	楽天のみ消費税別で金額設定

	//	PHP設定一時変更	//add ohkawara 2018/08/31
	ini_set('memory_limit', '2048M');	//	update ookawara 2019/11/15	//	update ookawara 2018/08/31

	include("../sub/array.inc");
	include ("../../cone.inc");
	include_once("../include/movie_list.php");	//	add ookawara 2014/10/10

	include("./goods_12_yahoo.inc");
	include("./goods_12_rakuten.inc");
	include("./goods_12_amazon.inc");
	include("./goods_12_wowma.inc");



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
	$last_point_time = "";	
//	$nows_time = time()+(60*60*25);
//	$start_point_time = date("YmdH",$nows_time);
	$start_point_time = "";
	// if ($last_point_time > $start_point_time) {
	// 	$set_point_time = $start_point_time."_".$last_point_time;
	// }
	define("RAKUTEN_POINT_START",$start_point_time);// add thao 2023/06/01

	define("RAKUTEN_POINT_LAST",$start_point_time);// add thao 2023/06/01

	//	新規セット
	define("RAKUTEN_POINT_NEW","0");	//	0:未設定	1:設定

	//	通常商品在庫数
	define("RAKUTEN_DEFO_ZAIKO", 10);
	//	商品画像タイプ  // add thao 2023/06/01
	define("RAKUTEN_IMAGE_TYPE", "CABINET");

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
	// //	楽天商品CSV
	// define("RAKUTEN_ITEM_FILE","$DIR_F/item.csv");
	// //	楽天選択枠CSV
	// define("RAKUTEN_SELECT_FILE","$DIR_F/select.csv");
	// //	楽天カテゴリーCSV
	// define("RAKUTEN_CAT_FILE","$DIR_F/item-cat.csv");

	//	add ookawara 2015/06/26
	//	Amazon商品CSV
	//	楽天ノーマル商品CSV     add thao 2023/05/22
	define("RAKUTEN_NORMAL_ITEM_FILE","$DIR_F/normal-item.csv");
	//	楽天削除CSV
	define("RAKUTEN_DELETE_FILE","$DIR_F/item-delete.csv");
	//	楽天カテゴリーCSV
	define("RAKUTEN_CAT_FILE","$DIR_F/item-cat.csv");

	//	既存楽天商品CSV
	define("RAKUTEN_B_NORMAL_ITEM_FILE","$DIR_TEMP/normal-item.csv");
	//	既存楽天選択枠CSV
	define("RAKUTEN_B_DELETE_FILE","$DIR_TEMP/item-delete.csv");
	//	既存楽天カテゴリーCSV
	define("RAKUTEN_B_CAT_FILE","$DIR_TEMP/item-cat.csv");

	define("AMAZON_ITEM_FILE","$DIR_F/amazon_item.txt");
	//	Amazon在庫CSV
	define("AMAZON_ZAIKO_FILE","$DIR_F/amazon_zaiko.txt");
	//add thao 2023/05/23

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
function goods_12() {

	if ($_POST['mode'] == "make") {
		list($msg,$ERROR) = make_csv($msg,$ERROR);
	} elseif ($_POST['mode'] == "ファイル削除") {
		list($msg,$ERROR) = del($msg,$ERROR);
	} elseif ($_POST['mode'] == "更新データー削除") {
		list($msg,$ERROR) = del_renew($msg,$ERROR);
	} elseif ($_POST['mode'] == "全商品更新") {
		list($msg,$ERROR) = all_renew($msg,$ERROR);
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
		//  && !file_exists(RAKUTEN_ITEM_FILE)
		//  && !file_exists(RAKUTEN_SELECT_FILE)
		//  && !file_exists(RAKUTEN_CAT_FILE)
		 && !file_exists(RAKUTEN_NORMAL_ITEM_FILE)     //add thao 2023/05/22
		 && !file_exists(RAKUTEN_DELETE_FILE)	 //add thao 2023/05/22
		 && !file_exists(RAKUTEN_CAT_FILE)		 //add thao 2023/05/22
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


// 		//	rakuten
// 		if (file_exists(RAKUTEN_ITEM_FILE)) {
// 			$rakuten_item_file = RAKUTEN_ITEM_FILE;
// 			$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_ITEM_FILE));
// 			$rakuten_item_msg = <<<WAKABA
// ファイル作成日：$ftime<br>
// 楽天アイテム <A href="{$rakuten_item_file}" target="_blank">ダウンロード</A><br>
// <br>

// WAKABA;
// 		}
// 		if (file_exists(RAKUTEN_SELECT_FILE)) {
// 			$rakuten_select_file = RAKUTEN_SELECT_FILE;
// 			$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_SELECT_FILE));
// 			$rakuten_select_msg = <<<WAKABA
// ファイル作成日：$ftime<br>
// 楽天セレクト <A href="{$rakuten_select_file}" target="_blank">ダウンロード</A><br>
// <br>

// WAKABA;
// 		}
// 		if (file_exists(RAKUTEN_CAT_FILE)) {
// 			$rakuten_cat_file = RAKUTEN_CAT_FILE;
// 			$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_CAT_FILE));
// 			$rakuten_cat_msg = <<<WAKABA
// ファイル作成日：$ftime<br>
// 楽天カテゴリー <A href="{$rakuten_cat_file}" target="_blank">ダウンロード</A><br>
// <br>

// WAKABA;
// 		}

				//	rakuten
	if (file_exists(RAKUTEN_NORMAL_ITEM_FILE)) {
		$rakuten_normal_item_file = RAKUTEN_NORMAL_ITEM_FILE;
		$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_NORMAL_ITEM_FILE));
		$rakuten_normal_item_msg = <<<WAKABA
ファイル作成日：$ftime<br>
楽天ノーマルアイテム <A href="{$rakuten_normal_item_file}" target="_blank">ダウンロード</A><br>
<br>

WAKABA;
		}
		if (file_exists(RAKUTEN_DELETE_FILE)) {
			$rakuten_delete_file = RAKUTEN_DELETE_FILE;
			$ftime = date("Y/m/d G:i:s",filemtime(RAKUTEN_DELETE_FILE));
			$rakuten_delete_msg = <<<WAKABA
ファイル作成日：$ftime<br>
楽天デリート <A href="{$rakuten_delete_file}" target="_blank">ダウンロード</A><br>
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
$rakuten_normal_item_msg
$rakuten_delete_msg
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



function make_csv(&$msg = null,&$ERROR = null) {
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

	//	YAHOO
	list($msg,$ERROR) = make_csv_yahoo($msg,$ERROR);

	//	楽天
	list($msg,$ERROR) = make_csv_rakuten($msg,$ERROR);

	//	Amazon
	list($msg,$ERROR) = make_csv_amazon($msg,$ERROR);
	//	Wowma
	list($msg,$ERROR) = make_csv_wowma($msg,$ERROR);




/*-----------------------------------------------------------------------------*/



	// mysqli_close($conn_id);
	return array($msg,$ERROR);

}






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
function del(&$msg = null,&$ERROR = null) {

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

	// //	Rakuten
	// if (file_exists(RAKUTEN_ITEM_FILE)) {
	// 	unlink(RAKUTEN_ITEM_FILE);
	// 	$msg .= "楽天商品ファイルを削除しました。<br>\n<br>\n";
	// }
	// if (file_exists(RAKUTEN_SELECT_FILE)) {
	// 	unlink(RAKUTEN_SELECT_FILE);
	// 	$msg .= "楽天商品選択ファイルを削除しました。<br>\n<br>\n";
	// }
	// if (file_exists(RAKUTEN_CAT_FILE)) {
	// 	unlink(RAKUTEN_CAT_FILE);
	// 	$msg .= "楽天カテゴリーファイルを削除しました。<br>\n<br>\n";
	// }

	//	Rakuten
	if (file_exists(RAKUTEN_NORMAL_ITEM_FILE)) {
		unlink(RAKUTEN_NORMAL_ITEM_FILE);
		$msg .= "楽天商品ファイルを削除しました。<br>\n<br>\n";
	}
	if (file_exists(RAKUTEN_DELETE_FILE)) {
		unlink(RAKUTEN_DELETE_FILE);
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
function del_renew(&$msg = null,&$ERROR = null) {
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
function all_renew(&$msg = null,&$ERROR = null) {
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
