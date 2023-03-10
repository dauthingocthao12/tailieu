<?PHP
//	カテゴリー一覧
function sublist($VALUE,$CHECK) {
global $db,$CLASS_L,$LOG_DIR,$index;

	$URL = "/goods";
	$file = "s";
	foreach ($VALUE AS $VAL) {
		$VAL = trim($VAL);
		if (!$VAL) { continue; }
		if ($file) { $file .= "_"; }
		if ($URL) { $URL .= "/"; }
		$file .= $VAL;
##		if (!eregi(".htm",$VAL)) {
		if (!preg_match("/\.htm/i",$VAL)) {
			$URL .= $VAL;
		}
	}
##	if (eregi(".htm",$file)) { $htm = ""; }
	if (preg_match("/\.htm/i",$file)) { $htm = ""; }
	else { $htm = ".htm"; }
	$file = "./$LOG_DIR/$file$htm";

##	$URL = eregi_replace("/$","",$URL);
	$URL = preg_replace("/\/$/i","",$URL);

/*
	//	del ookawara 2014/12/18
	if (file_exists($file)) {
		$fp = fopen ($file, "r");
		if ($fp) {
			while (!feof ($fp)) {
				$html .= fgets($fp, 4096);
			}
		}
	}
	else {
*/
		//	表示確認
		if ($CHECK[m]) {
##			$maker_num = eregi_replace("^m","",$CHECK[m]);
			$maker_num = preg_replace("/^m/i","",$CHECK[m]);
			$where = " AND maker_num='$maker_num'";
		}
		if ($CHECK[main]) {
			$snum = $CHECK[main];
			$fnum = $snum + 99;
			$where .= " AND class_m::int BETWEEN $snum AND $fnum";
		}
		$CLASS_M_L = array();
		$HYOUJI = array();
		$sql  = "SELECT class_m, COUNT(class_m) AS count FROM list" .
				" WHERE display='2'$where" .
				" AND state!='1'".		//	2009/04/25	add ookawara
				" GROUP BY class_m;";
		if ($result = pg_query($db,$sql)) {
			while ($list = pg_fetch_array($result)) {
				$class_m = $list['class_m'];
				$count = $list['count'];
				$CLASS_M_L[$class_m] = $count;
				$num = floor($class_m / 100) * 100;
				if ($count > 0) { $HYOUJI[$num] = "ok"; }
			}
		}

		//	カテゴリー表示
		if ($CLASS_M_L) {

			$html = <<<WAKABA
		<section class="categories-list">
			<h2 class="sub-title-prod">カテゴリー 一覧</h2>

WAKABA;

		//	分類読込 表示
			$sql  = "SELECT class_m, class_m_n FROM class ORDER BY class_m;";
			if ($result = pg_query($db,$sql)) {
				$cls = "";
				$i = 0;
				$flag = 0;
				while ($list = pg_fetch_array($result)) {
					$class_m = $list['class_m'];
					$class_m_n = $list['class_m_n'];
					$count = $CLASS_M_L[$class_m];
					$num = floor($class_m / 100) * 100;

					if ($cls == "" && $class_m > 900 && $count > 0) {
						$cls = $num;
					}

					if ($cls != $num && $flag != 0) {
						$html .= "<hr />\n";
						$i = 0;
						$flag = 0;
					}

					if (!$HYOUJI[$num]) { continue; }

					//	900以上の場合
					if ($class_m > 900 && $count > 0 && $CHECK[s] == $class_m) {
							$num_url = "";
							$html .=<<<WAKABA
						<h3 class="sub-title-cat"> ■ <A href="$URL/s$class_m/$index" title="$class_m_n ($count$syouhin)"><B>$class_m_n</B></A></h3>

WAKABA;
					}
					//	900以下の場合
					elseif ($class_m < 900) {
						$flag = 1;
						if ($count > 0) {
							$num_url = "";
							$link = "<A href=\"$URL/s$class_m/$index\" class=\"item-box\" title=\"$class_m_n ($count" . "アイテム)\">$class_m_n</A>";
						}
						else {
							$link = "<span class=\"item-box cat-link-disabled\">$class_m_n</span>";
						}
						$html .= "<div class=\"cat-box\">$link</div>\n";
						$i++;
						if ($i > 2) { $i = 0; }
						$cls = $num;
					}
				}

				if ($i != 2) {
					if ($class_m < 900) {
						$html .= "<hr />\n";
					}
				}
			}

			$html .= "</section>\n";

		}

/*
	//	del ookawara 2014/12/18
		if ($html) {
			$OUT = fopen("$file","w");
			fwrite($OUT,$html);
			fclose($OUT);
			@chmod($file,0666);
		}

	}
*/

	return $html;

}
?>
