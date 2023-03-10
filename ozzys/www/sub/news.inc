<?PHP
//	入荷情報
function news() {
global $db,$LOGDATA_DIR,$news_file,$index;

	$html = "";

	$file = "$LOGDATA_DIR/$news_file";
	if (!$LIST && file_exists($file)) {
		$LIST = file($file);
	}

	$goods = "";
	$max = count($LIST);
	if ($max > 1) {
		for($i=1; $i<$max; $i++) {
			$VAL = $LIST[$i];
			$VAL = trim($VAL);
##			$val = eregi_replace("[^0-9]","",$VAL);
			$val = preg_replace("/[^0-9]/i","",$VAL);
			if ($val) {
				$sql  = "SELECT goods_name FROM list" .
						" WHERE list_num='$val'".
						" AND state!='1'".		//	2009/04/25	add ookawara
						" LIMIT 1;";
				if ($result = pg_query($db,$sql)) {
					$list = pg_fetch_array($result);
					$goods_name = $list[goods_name];
					$goods .= "<li>\n";
					$goods .= "<a href=\"/goods/$VAL/$index\">$goods_name</a>\n";
					$goods .= "</li>\n";
				}
			}
		}
	}

	if ($goods) {
		$days = $LIST[0];
		$days = trim($days);
		if ($days) {
			$day_msg = "<h3>入荷日：$days</h3>\n";
		}

		$html  = "<section>\n";
		$html .=   "<h2 class='main-title'>入荷情報</h2>\n";
		$html .=   "<div class='product-updates'>\n";
		$html .=     $day_msg;
		$html .=     "<ul>\n";
		$html .=     $goods;
		$html .=     "</ul>\n";
		$html .=   "</div>\n";
		$html .= "</section>\n";
	}

	return $html;
}
?>
