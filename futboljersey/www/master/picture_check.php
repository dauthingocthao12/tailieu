<?PHP
//	請求書作成プログラム

	include("./array.inc");
	include("../sub/array.inc");
	include ("../../cone.inc");

	//	画像保存フォルダー
	//	メイン画像
	define("IMG_F_DIR","../imagef");
	//	サブ画像
	define("IMG_B_DIR","../imageb");

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

//	del ookawara 2009/09/03
//	$sql  = "SELECT g_num, g_name, code, price, sale_price, brand FROM goods".
//			" ORDER BY brand, g_name;";
	$sql  = "SELECT distinct g.g_num, g.g_name, g.code, g.price, g.sale_price, g.brand FROM goods g".
			" LEFT JOIN category c ON c.g_num=g.g_num".
			" WHERE c.display='1'".
			" ORDER BY g.brand, g.g_name;";
	if ($result = mysqli_query($conn_id,$sql)) {
		while ($list = mysqli_fetch_array($result)) {
			foreach ($list AS $key => $val) {
				$$key = $val;
			}

			$imagef = IMG_F_DIR."/$code" . ".jpg";
			$imageb = IMG_B_DIR."/$code" . ".jpg";
			if (file_exists($imagef) && file_exists($imageb)) { continue; }

			if (file_exists($imagef)) { $front_check = "○"; } else { $front_check = "×"; }
			if (file_exists($imageb)) { $back_check = "○"; } else { $back_check = "×"; }

			$html_name = "";
			if (!file_exists($imagef) && !file_exists($imageb)) {
				$html_name = "two";
			} else {
				$html_name = "one";
			}

			$$html_name .= "<tr align=\"center\" bgcolor=\"#ffffff\">\n";
			$$html_name .= "<td><a href=\"/goods/g".$g_num."/\" target=\"_blank\">".$g_num."</a></td>\n";
			$$html_name .= "<td>".$g_name."</td>\n";
			$$html_name .= "<td>".$code."</td>\n";
			$$html_name .= "<td>".$B_LINE[$brand]."</td>\n";
			$$html_name .= "<td>".$front_check."</td>\n";
			$$html_name .= "<td>".$back_check."</td>\n";
			$$html_name .= "<td align=\"right\">".$price."円</td>\n";
			$$html_name .= "<td align=\"right\">".$sale_price."円</td>\n";
			$$html_name .= "</tr>\n";
		}
	}

	if ($two || $one) {
		$html  = "<table border=\"0\" cellpadding=\"1\" cellspacing=\"1\" bgcolor=\"#666666\">\n";
		$html .= "<tr bgcolor=\"#cccccc\">\n";
		$html .= "<th width=\"120\">管理商品番号</th>\n";
		$html .= "<th>商品名</th>\n";
		$html .= "<th>商品番号</th>\n";
		$html .= "<th>ブランド</th>\n";
		$html .= "<th>Front</th>\n";
		$html .= "<th>Back</th>\n";
		$html .= "<th width=\"70\">金額</th>\n";
		$html .= "<th width=\"70\">特価</th>\n";
		$html .= "</tr>\n";
		$html .= $two;
		$html .= "<tr align=\"center\" bgcolor=\"#ffffff\">\n";
		$html .= "<td></td>\n";
		$html .= "<td></td>\n";
		$html .= "<td></td>\n";
		$html .= "<td></td>\n";
		$html .= "<td></td>\n";
		$html .= "<td></td>\n";
		$html .= "<td align=\"right\"></td>\n";
		$html .= "<td align=\"right\"></td>\n";
		$html .= "</tr>\n";
		$html .= $one;
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
<TITLE>画像登録確認</TITLE>
</HEAD>
<BODY>
<br>
<b>画像登録確認</b><br>
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
