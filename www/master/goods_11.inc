<?PHP
//	Yahoo商品検索登録ファイル
function goods_11() {
// global	$mode,$CATE_DIR,$DIR_F,
// 		$cate_table,$goods_table,$del_goods_table,$r_cate_table,
// 		$yahoo_file,$yahoo_z_file,$rakuten_file,$rakuten_s_file,$rakuten_c_file,
// 		$brakuten_file,$brakuten_s_file,$brakuten_c_file,
// 		$b_file,$list_file,$check_file,
// 		$img_f,$img_b,$ERROR,$NEWCATE,
// 		$tax,$taxable,$tokka;
global	$CATE_DIR,$DIR_F,$ERROR,$NEWCATE,
		$cate_table,$goods_table,$del_goods_table,$r_cate_table,$yahoo_file,$b_file,$list_file,$check_file,$img_f,$img_b;	 
$mode=$_POST['mode'];

	include("../sub/array.inc");


	//	Yahoo商品検索登録ファイル名
	$yahoo_file = "../yahoo_item.txt";

	//	カテゴリーメニューファイル名
	$cat_menu_file = "../data/cat_menu.txt";
	define("CAT_MENU_FILE",$cat_menu_file);

	//	画像保存フォルダー
	//	メイン画像
	$img_f = "$DIR_F/imagef";
	//	サブ画像
	$img_b = "$DIR_F/imageb";

	//	ブランドファイル
	$b_file = "../data/brand.dat";
	$list_file = "./data/ypath.dat";

	//	sqlテーブル
	$cate_table = "category";
	$goods_table = "goods";
	$del_goods_table = "delgoods";
	$r_cate_table = "r_cate";

	if ($mode == "make") { make_csv(); }
	elseif ($mode == "更新データー削除") { del_renew(); }

	first00();

}



function first00(){
// global	$PHP_SELF,$main,$goods,$yahoo_file,$yahoo_z_file,
// 		$rakuten_file,$rakuten_s_file,$rakuten_c_file,$ERROR,$msg,
// 		$tax,$taxable,$tokka,$delpass;
// global	$PHP_SELF,$ERROR,$tax,$taxable,$tokka,$yahoo_file;
global $ERROR,$tax,$PHP_SELF,$yahoo_file;
$main=$_POST['main'];
$goods=$_POST['goods'];
global $msg; // @source make_csv(),del_renew()
$taxable=$_POST['taxable'];
$tokka=$_POST['tokka'];

	if ($ERROR) { ERROR($ERROR); }

	if ($tax == "") { $tax = 5; }
	if ($taxable == 1) { $ta_checked = "checked"; } else { $ta_checked = ""; }
	if ($tokka == 1) { $tk_checked = "checked"; } else { $tk_checked = ""; }

	echo<<<WAKABA
$msg
<FORM action="$PHP_SELF" method="POST">
<INPUT type="hidden" name="main" value="$main">
<INPUT type="hidden" name="goods" value="$goods">
<INPUT type="hidden" name="mode" value="make">
<INPUT type="hidden" name="tax" value="5">
<INPUT type="hidden" name="taxable" value="1">
<INPUT type="hidden" name="tokka" value="1">
商品データーを更新した場合、作成ボタンを押し、<br>
Yahoo商品検索登録ファイルを更新して下さい。<br>
<INPUT type="submit" value="作成">
</FORM>

WAKABA;

	if (!file_exists($yahoo_file)) {
		echo("今現在、Yahoo商品検索登録ファイルはありません。<br>\n");
	}
	else {

		//	yahoo
		if (file_exists($yahoo_file)) {
			$ftime = date("Y/m/d G:i:s",filemtime($yahoo_file));
			echo <<<WAKABA
<br>
最終更新日：$ftime<br>
<br>

WAKABA;
		}
	}

}



function make_csv() {
// global	$CATE_DIR,$ERROR,$msg,$NEW_L,
// 		$cate_table,$goods_table,$del_goods_table,$r_cate_table,
// 		$yahoo_file,$yahoo_z_file,$rakuten_file,$rakuten_s_file,$rakuten_c_file,
// 		$brakuten_file,$brakuten_s_file,$brakuten_c_file,
// 		$c_file,$b_file,$list_file,$check_file,
// 		$img_f,$img_b,$conn_id,$check_time,
// 		$tax,$taxable,$tokka,$delpass,$attention,$attention2,
// 		$GOODS_DISCOUNT_C,$GOODS_DISCOUNT_CATE,$DISCOUNT_PAR;
global	$CATE_DIR,$ERROR,$NEW_L,$GOODS_DISCOUNT_C,$GOODS_DISCOUNT_CATE,$DISCOUNT_PAR,$conn_id,
			$cate_table,$taxable,$tax,$b_file,$r_cate_table,$yahoo_file,$goods_table,$r_cate_table;
global $msg;
$tokka=$_POST['tokka'];
$taxable=$_POST['taxable'];
$tax=$_POST['tax'];

	if ($taxable == 1 && $tax <= 0) { $ERROR[] = "消費税を入力して下さい。"; }

	include ("../../cone.inc");
	include ("../sub/array.inc");

	//	データー読み込み
	if (!$ERROR) {
		//	カテゴリーデーター
		unset($CATE);
		$sql = "SELECT c_name, cate1, cate2, cate3 FROM $r_cate_table ORDER BY cate1, cate2, cate3;";
		if ($result = pg_query($conn_id,$sql)) {
			while ($list = pg_fetch_array($result)) {
				$c_name = $list['c_name'];
				$cate1 = $list['cate1'];
				$cate2 = $list['cate2'];
				$cate3 = $list['cate3'];
				$CATE[$cate1][$cate2][$cate3] = $c_name;
			}
		}

		//	ブランドデーター
		if (file_exists($b_file)) {
			$B_LIST = file($b_file);
			foreach ($B_LIST AS $val) {
				list($b_num_,$b_name_,$del_) = explode("<>",$val);
				$BRAND[$b_num_] = $b_name_;
			}
		}
		else { $ERROR[] = "ブランドファイルを読み込めませんでした。"; }
	}

	//	登録データー読み込み
	unset($GDATA);
	$sql = "SELECT i.num, i.cate1, i.cate2, i.cate3, i.g_num, i.ypath, i.display, i.copy_num, i.state," .
			//" j.g_name, j.code, j.price, j.sale_price, j.options, j.caption, j.brand" . 	//	del ookawara 2015/09/24
			"  coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name, j.code, j.price, j.sale_price, j.options, j.caption, j.brand" . 	//	add ookawara 2015/09/24
			" FROM $cate_table i, $goods_table j" .
			" WHERE i.g_num=j.g_num AND j.soldout!='1' AND i.display='1' AND i.state<='2'" .
			" ORDER BY i.cate1, i.cate2, i.cate3;";
	if ($result = pg_query($conn_id,$sql)) {
$count = pg_num_rows($result);
//echo("count<>$count<br>\n");
		while ($list = pg_fetch_array($result)) {
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

			$GDATA[$num]['cate1'] = $cate1;
			$GDATA[$num]['cate2'] = $cate2;
			$GDATA[$num]['cate3'] = $cate3;
			$GDATA[$num]['g_num'] = $g_num;
			$GDATA[$num]['ypath'] = $ypath;
			$GDATA[$num]['display'] = $display;
			$GDATA[$num]['copy_num'] = $copy_num;
			$GDATA[$num]['g_name'] = $g_name;
			$GDATA[$num]['code'] = $code;
			$GDATA[$num]['price'] = $price;
			$GDATA[$num]['sale_price'] = $sale_price;
			$GDATA[$num]['options'] = $options;
			$GDATA[$num]['caption'] = $caption;
			$GDATA[$num]['brand'] = $brand;
			$GDATA[$num]['state'] = $state;

			$G_CAT_DATA[$cate1][$cate2][$cate3] = 1;
		}
	}

$count = count($GDATA);
//echo("gcount<>$count<br>\n");


	unset($y_csv);

	//	yahoo用ノーマルCSV
	if ($GDATA) {
		foreach ($GDATA AS $KEY => $VAL) {
			$num = trim($KEY);
			$type = "";
			unset($y_csv_);
			$y_csv_ = yahoo_csv($CATE,$BRAND,$YPATH,$GDATA,$num,$type);
			if ($y_csv_) { $y_csv .= $y_csv_; }
		}
	}

	if ($y_csv) {
		$y_csv = "id\ttitle\turl\tprice\tdesc\timage\tpath\tno\tjan\tisbn\r\n$y_csv";
#		$y_csv = mb_convert_encoding($y_csv,"SJIS","EUC");
		$y_csv = mb_convert_encoding($y_csv,"SJIS","UTF-8");
		$OUT = fopen($yahoo_file,"w");
		fwrite($OUT,$y_csv);
		fclose($OUT);
		@chmod($yahoo_file,0666);
		$msg = "Yahoo商品検索登録ファイル作成完了いたしました。<br>\n<br>\n";
	}

	//	カテゴリーメニュー作成
	make_cat_menu($G_CAT_DATA);

	//	メーカーメニュー作成
//	make_maker_memu($BRAND,$CATE);

	pg_close($conn_id);

}



//	yahoo用CSV
function yahoo_csv($CATE,$BRAND,$YPATH,$GDATA,$num,$type) {
// global $tax,$taxable,$tokka,$img_f,$img_b,$check_time,$attention,
// 		$GOODS_DISCOUNT_C,$GOODS_DISCOUNT_CATE,$DISCOUNT_PAR,$NEWCATE,$attention;
global $tax,$GOODS_DISCOUNT_C,$GOODS_DISCOUNT_CATE,$DISCOUNT_PAR,$NEWCATE,$attention;
	$cate1 = $GDATA[$num]['cate1'];
	$cate2 = $GDATA[$num]['cate2'];
	$cate3 = $GDATA[$num]['cate3'];
	$g_num = $GDATA[$num]['g_num'];
	$ypath = $GDATA[$num]['ypath'];
	$display = $GDATA[$num]['display'];
	$copy_num = $GDATA[$num]['copy_num'];
	$state = $GDATA[$num]['state'];
	$g_name = $GDATA[$num]['g_name'];
	$code = $GDATA[$num]['code'];
	$price = $GDATA[$num]['price'];
	$sale_price = $GDATA[$num]['sale_price'];
	$options = $GDATA[$num]['options'];
	$caption = $GDATA[$num]['caption'];
	$brand = $GDATA[$num]['brand'];

	if (strlen($g_name) > 80) {
		$g_name = mb_strimwidth($g_name,0,77,"...");
	}

	$url = "http://www.futboljersey.com/goods/g{$g_num}/";

	if ($GOODS_DISCOUNT_C == 1) {
		$flag = 0;
		if ($GOODS_DISCOUNT_CATE) {
			foreach ($GOODS_DISCOUNT_CATE AS $VAL) {
				$VAL = trim($VAL);
				if ($cate1 == $VAL) { $flag = 1; }
			}
		}
		if ($flag != 1) {
			$price = floor($price * (1-($DISCOUNT_PAR/100)));
		}
		$price = ($price * $tax / 100) + 0.5 + $price;
		$price = floor($price);
	}
	else {
		if ($sale_price > 0) { $price = $sale_price; }
		$price = ($price * $tax / 100) + 0.5 + $price;
		$price = floor($price);
	}

	//	画像チェック
	unset($image);
	$code = trim($code);
	$imagef_file = "../imagef/$code" . ".jpg";
	if (file_exists($imagef_file)) {
		$image = "http://www.futboljersey.com/imagef/{$code}.jpg";
	}

	//	カテゴリー
	$cate1_name = $CATE[$cate1][0][0];
	$cate2_name = $CATE[$cate1][$cate2][0];
	$cate3_name = $CATE[$cate1][$cate2][$cate3];

	$path = "";
	if ($NEWCATE[$cate1]['name']) {
		$name0 = $NEWCATE[$cate1]['name'];
		$path .= $name0;
		$path .= " > ";
	}

	if ($name0 == $cate1_name) { $path = ""; }
	$path .= $cate1_name;

	$path .= " > ";

	if ($cate1_name == $cate2_name) { $path = ""; }
	$path .= $cate2_name;

	$name .= " > ";

	if ($cate2_name == $cate3_name) { $path = ""; }
	$path .= $cate3_name;

	if (strlen($path) > 80) {
		$path = mb_strimwidth($path,0,77,"...");
	}

	//	options
	//	options
	$options = trim($options);
	$SIZE = explode("\n",$options);
	if ($SIZE) {
		foreach ($SIZE AS $size) {
			$LaunchDate = "";
			$Quantity = 100;
			$size = trim($size);
			if ($size == "") { continue; }
##			if (ereg("^\*",$size)) {
			if (preg_match("/^\*/",$size)) {
				$Quantity = 0;
			}
##			$size = eregi_replace("^\/","",$size);
			$size = preg_replace("/^\//i","",$size);
##			$size = eregi_replace("^\*","",$size);
			$size = preg_replace("/^\*/i","",$size);
			preg_match_all("|\[(.*)\]|U",$size,$jancode);
			$jancode = $jancode[1][0];
##			$size = eregi_replace("\[.*\]","",$size);
			$size = preg_replace("/\[.*\]/i","",$size);
			if ($size == "F" || $size == "フリー" || $size == "--" || $size == "フットサルサイズ" || $size == "設定なし") {
				$size = "";
			}

			$janlen = strlen($jancode);
			if ($janlen != 13) { $jancode = ""; }

			unset($desc);
			$desc = <<<WAKABA
メーカー名：$BRAND[$brand] 
$size
$caption
WAKABA;

##			if (ereg("<!-- Attention -->",$desc)) {
			if (preg_match("/<!-- Attention -->/",$desc)) {
##				$desc = ereg_replace("<!-- Attention -->","$attention",$desc);
				$desc = preg_replace("/<!-- Attention -->/","$attention",$desc);
				$desc = trim($desc);
			}

			$desc = trim($desc);
##			$desc = ereg_replace("\r","",$desc);
			$desc = preg_replace("/\r/","",$desc);
##			$desc = ereg_replace("\n"," ",$desc);
			$desc = preg_replace("/\n/"," ",$desc);
			if (strlen($desc) > 512) {
				$desc = mb_strimwidth($g_name,0,509,"...");
			}

			$csv .= "g{$g_num}\t{$g_name}\t{$url}\t{$price}\t{$desc}\t{$image}\t{$path}\t{$no}\t{$jancode}\t{$isbn}\r\n";
		}
	}

	return $csv;

}



function make_cat_menu($G_CAT_DATA) {
// global $conn_id,$msg;
global $conn_id;
global $msg;
	//	登録可能 $num
	$max_num = 16;
	//	カテゴリーフォルダー
	$DIR_CATE = "../category";
	define("DIR_CATE",$DIR_CATE);

	$cate_table = 'category';

	$cate_menu_file = "../data/cat_menu.inc";

	//	cate1 == 20 の商品有無
	$sql  = "SELECT count(*) AS count FROM $cate_table".
			" WHERE display='1' AND state!='3' AND cate1='20';";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		if ($list['count'] > 0) {
			$category1 = "<li><a href=\"/goods/20/1/\" title=\"新入荷\nサッカー用品\">新入荷</a></li>";
		}
	}

	//	cate1 == 23 の商品有無
	$sql  = "SELECT count(*) AS count FROM $cate_table".
			" WHERE display='1' AND state!='3' AND cate1='23';";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		if ($list['count'] > 0) {
			$category2 = "<li><a href=\"/goods/23/1/\" title=\"特価商品\nサッカー用品\">特価商品</a></li>";
		}
	}

	$html = <<<WAKABA
<div class="cat-menu">
<ul id="suckertree1">
$category1
$category2
<li><a href="/good/3.htm" title="クラブチーム
サッカー用品">クラブチーム</a></li>
<li><a href="/goods/38/" title="Ｊリーグ
サッカー用品">Ｊリーグ</a></li>
<li><a href="/good/5.htm" title="ナショナルチーム
サッカー用品">ナショナルチーム</a></li>
<li><a href="/goods/56/" title="フットサル
サッカー用品">フットサル</a></li>
<li><a href="/goods/59/" title="トレーニング
サッカー用品">トレーニング</a></li>
<li><a href="/goods/62/" title="ゴールキーパー
サッカー用品">ゴールキーパー</a></li>
<li><a href="/goods/65/" title="レフェリー
サッカー用品">レフェリー</a></li>
<li><a href="/good/10.htm" title="ジュニア
サッカー用品">ジュニア</a></li>
<li><a href="/goods/74/" title="ストリート
サッカー用品">ストリート</a></li>
<li><a href="/good/12.htm" title="シューズ
サッカー用品">シューズ</a></li>
<li><a href="/goods/83/" title="バッグ
サッカー用品">バッグ</a></li>
<li><a href="/good/14.htm" title="ボール
サッカー用品">ボール</a></li>
<li><a href="/goods/92/" title="アクセサリー
サッカー用品">アクセサリー</a></li>
</ul>
</div>

WAKABA;

	//	保存処理
	$OUT = fopen($cate_menu_file,"w");
	fwrite($OUT,$html);
	fclose($OUT);
	@chmod($cate_menu_file,0666);
	$msg .= "カテゴリーメニューファイル作成完了いたしました。<br>\n<br>\n";

}

function make_cat_list($CATE,$SET_CAT) {

	$mc_num = $SET_CAT['mc_num'];
	$file = DIR_CATE . "/{$mc_num}";
	if ($SET_CAT['sc_num'] != "") {
		$file .= "_" . $SET_CAT['sc_num'];
	}
	$file .= ".dat";
	if (file_exists($file)) {
		$LIST = file($file);
	}

	if ($LIST) {
		foreach ($LIST AS $line) {
			$line = trim($line);
			if (!$line) { continue; }
			list($h_num,$c_num,$c_name) = explode("<>",$line);

			if ($CATE[$mc_num][$c_num]) {
				$link = (int)sprintf("%02d",$c_num);

				//	最終カテゴリー
				$sub_html = "";

				$html .= "    <li><a href=\"/goods/{$mc_num}/{$link}/\" title=\"{$c_name}\">{$c_name}</a>";
				$html .= $sub_html;
				$html .= "</li>\n";
			}
		}
	}

	if ($html) {
		$html = <<<WAKABA

  <ul>
$html
  </ul>
WAKABA;
	}

	return $html;
}
?>
