<?PHP
//	請求書作成プログラム

	include("./array.inc");
	include("../sub/array.inc");
	include ("../../cone.inc");


	$PHP_SELF = $_SERVER['PHP_SELF'];

	$html  = headers();
	$html .= first();
	$html .= footer();

	echo ($html);
	ob_start("mb_output_handler");

	exit;



//	初期ページ
function first() {
	global $PHP_SELF,$conn_id;

	//	メーカー読み込み
	$b_file = "../data/brand.dat";
	if (file_exists($b_file)) {
		$B_LIST = file($b_file);
		foreach ($B_LIST AS $val) {
			list($b_num_,$b_name_,$del_) = explode("<>",$val);
			if ($del_ == 1) { continue; }
			$B_LINE[$b_num_] = $b_name_;
		}
	}

	//$sql  = "SELECT g_num, g_name, code, price, sale_price, brand FROM goods".
	//		" WHERE soldout='1' ORDER BY brand, g_name;";

	$sql  = "CREATE TEMPORARY TABLE temp_category_goods".
			" (g_num integer, del integer);";
	@pg_query($conn_id,$sql);
	$sql  = "INSERT INTO temp_category_goods".
			" SELECT g_num, '1' AS del FROM category".
			" WHERE display='1'".
			" GROUP BY g_num;";
	@pg_query($conn_id,$sql);

	$sql  = "SELECT goods.g_num, goods.g_name, goods.code, goods.price, goods.sale_price, goods.brand FROM goods goods".
			" LEFT JOIN temp_category_goods cat ON cat.g_num=goods.g_num".
			" WHERE goods.soldout='1'".
			" AND cat.del='1'".
			" ORDER BY goods.g_num, goods.brand, goods.g_name;";
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			foreach ($list AS $key => $val) {
				$$key = $val;
			}

			$htm .= "<tr align=\"center\" bgcolor=\"#ffffff\">\n";
			$htm .= "<td><a href=\"/goods/g".$g_num."/\" target=\"_blank\">".$g_num."</a></td>\n";
			$htm .= "<td>".$g_name."</td>\n";
			$htm .= "<td>".$code."</td>\n";
			$htm .= "<td>".$B_LINE[$brand]."</td>\n";
			$htm .= "<td align=\"right\">".$price."円</td>\n";
			$htm .= "<td align=\"right\">".$sale_price."円</td>\n";
			$htm .= "</tr>\n";
		}
	}

	if ($htm) {
		$html  = "<table border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"#666666\">\n";
		$html .= "<tr bgcolor=\"#cccccc\">\n";
		$html .= "<th width=\"120\">管理商品番号</th>\n";
		$html .= "<th>商品名</th>\n";
		$html .= "<th>商品番号</th>\n";
		$html .= "<th>ブランド</th>\n";
		$html .= "<th width=\"70\">金額</th>\n";
		$html .= "<th width=\"70\">特価</th>\n";
		$html .= "</tr>\n";
		$html .= $htm;
		$html .= "</table>\n";
	}

	return $html;

}



//	管理画面用ヘッダ
function headers() {

	$html = <<<WAKABA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>Soldout商品</TITLE>
</HEAD>
<BODY>
<br>
<b>Soldout商品</b><br>
<br>

WAKABA;

	return $html;

}



//	管理画面用フッタ
function footer() {

	$html = <<<WAKABA
</BODY>
</HTML>

WAKABA;

	return $html;

}
?>
