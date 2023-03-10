<?PHP
//	メーカー名一覧
function makerlist($VALUE,$CHECK) {
global $db,$LOG_DIR,$m_maker_file,$index;

	$URL = "/goods";
	if (!$CHECK) {
		$file = "./$LOG_DIR/$m_maker_file";
	}
	else {
		$file = "m";
		foreach ($VALUE AS $VAL) {
			$VAL = trim($VAL);
			if (!$VAL) { continue; }
			if ($file) { $file .= "_"; }
			if ($URL) { $URL .= "/"; }
			$file .= $VAL;
##			if (!eregi(".htm",$VAL)) {
			if (!preg_match("/\.htm/i",$VAL)) {
				$URL .= $VAL;
			}
		}
##		if (eregi(".htm",$file)) { $htm = ""; }
		if (preg_match("/\.htm/i",$file)) { $htm = ""; }
		else { $htm = ".htm"; }
		$file = "./$LOG_DIR/$file$htm";

##		$URL = eregi_replace("/$","",$URL);
		$URL = preg_replace("/\/$/i","",$URL);
	}
	//	表示確認
	if ($CHECK[s]) {
##		$subcate_num = eregi_replace("^s","",$CHECK[s]);
		$subcate_num = preg_replace("/^s/i","",$CHECK[s]);
		$where = " AND class_m='$subcate_num'";
	}
	elseif ($CHECK[main]) {
		$snum = $CHECK[main];
		$fnum = $snum + 99;
		$where = " AND class_m::int BETWEEN $snum AND $fnum";
	}
	$MAKER_L = array();
	$sql  = "SELECT maker_num, COUNT(maker_num) AS count FROM list" .
			" WHERE display='2'$where" .
			" AND state!='1'".		//	2009/04/25	add ookawara
			" GROUP BY maker_num;";
	if ($result = pg_query($db,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$maker_num = $list['maker_num'];
			$count = $list['count'];
			$MAKER_L[$maker_num] = $count;
		}
	}

	//	メーカー表示
	if ($MAKER_L) {

		$html = <<<WAKABA
		<section class="maker-list clearfix">
			<h2 class="sub-title-prod">メーカー 一覧</h2>
			<nav><ul>

WAKABA;

		//	メーカー読込 表示
		$sql  = "SELECT maker_num, maker_name FROM maker ORDER BY maker_name;";
		if ($result = pg_query($db,$sql)) {
			$bmaker = "";
			while ($list = pg_fetch_array($result)) {
				$maker_num = $list['maker_num'];
				$maker_name = $list['maker_name'];
				$count = $MAKER_L[$maker_num];
				if ($bmaker == $maker_name) { continue; }
				if ($count > 0) {
					$maker_num = sprintf("%04d",$maker_num);
					$html .= "<li><A class='item-box' href=\"$URL/m$maker_num/$index\" title=\"$maker_name ($count" . "アイテム)\">$maker_name</A></li>\n";
				}
				$bmaker = $maker_name;
			}
		}
		$html .= "</ul></nav>\n</section>\n";

	}

	return $html;
}
?>
