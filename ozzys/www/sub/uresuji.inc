<?PHP
//	売れ筋BEST10
function uresuji() {
global $db,$LOG_DIR,$kikan,$index;

	$file_name = date("Ymd") . "top10.htm";
	$file = "./$LOG_DIR/$file_name";

	$html  = "<section>\n";
	$html .= "<h2 class='sub-title'>★売れ筋BEST10★</h2>\n";
	$html .= "<div class='product-updates'>\n";
	$html .= "<ol>\n";

	$kikan_day = date("Y-m-d", mktime(0, 0, 0, date("m"), date("d") - $kikan, date("Y")));
	$sql  = "SELECT d.goods_name, count(i.buy_num) AS max FROM" .
			" list d, total i" .
			" WHERE d.list_num=i.list_num AND d.display='2' AND i.s_date>='$kikan_day'" .
			" AND d.state!='1'".		//	2009/04/25	add ookawara
			" GROUP BY d.goods_name ORDER BY max DESC LIMIT 10;";
	$i = 1;
	if ($result = pg_query($db,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$goods_name = $list['goods_name'];
			$max = $list['max'];
			$sql  = "SELECT list_num FROM list" .
					" WHERE display='2' AND goods_name='$goods_name'".
					" AND state!='1'".		//	2009/04/25	add ookawara
					" ORDER BY random() LIMIT 1;";
			if ($result2 = pg_query($db,$sql)) {
				$list2 = pg_fetch_array($result2);
				$list_num = $list2['list_num'];

				$html .= "<li><a href=\"/goods/n$list_num/$index\">" . $goods_name . "</a></li>\n";

				$i++;
			}
		}
	}

	$html .= "</ol>\n";
	$html .= "</div>\n";
	$html .= "</section>\n";

	return $html;
}
?>
