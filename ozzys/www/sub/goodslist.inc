<?PHP
//	商品名一覧
function goodslist($VALUE,$CHECK) {
global $db,$CLASS_L,$LOG_DIR,$view1,$gpx,$gpy,$index;

	$file = "n";
	$URL = "/goods";
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

	//	ページ
##	if (eregi("^index+([0-9])+\\.htm",$VAL)) {
	if (preg_match("/^index+([0-9])+\\.htm/i",$VAL)) {
##		$page = eregi_replace("[^0-9]","",$VAL);
		$page = preg_replace("/[^0-9]/i","",$VAL);		
	}
	if  (!$page) { $page = 1; }
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
		if ($CHECK[s]) {
##			$class_m = eregi_replace("^s","",$CHECK[s]);
			$class_m = preg_replace("/^s/i","",$CHECK[s]);
			$where .= " AND class_m='$class_m'";
		}
		elseif ($CHECK[main]) {
			$snum = $CHECK[main];
			$fnum = $snum + 99;
			$where .= " AND class_m::int BETWEEN $snum AND $fnum";
		}

		$sql  = "SELECT count(*) FROM list" .
				" WHERE display='2'$where" .
				" AND state!='1'".			//	2009/04/25 add ookawara
				" GROUP BY goods_name;";
		if ($result = pg_query($db,$sql)) {
			$max = pg_numrows($result);
		}
		if ($max > 0) {
			$max_page = ceil($max/$view1);
			if ($max_page < $page) { $page = $max_page; }
			$s = $view1 * ($page - 1);
			$e = ($view1 * $page) - 1;
			if ($e >= $max) { $e = $max - 1; }
			$view_s = $s + 1;
			$view_e = $e+1;

			$page_msg = "全" . $max . "件中 " . $view_s . "-" . $view_e . "件目 ";
			if ($max_page > 1) { $page_msg .= $page . "/" . $max_page . "Page"; }

			$html = <<<WAKABA
			<section class="productname-list">
				<h2 class="sub-title-prod">商品名一覧</h2>

WAKABA;

			if ($max_page > 1) {
				$html .= <<<WAKABA
				<div class="pagination">$page_msg</div>

WAKABA;
			}

			$html .= "<nav class=\"clearfix\">\n<ul>\n";

			//	商品読み込み
			$offset = ($page - 1) * $view1;
			$limit_max = $offset + $view1;
			if ($max < $limit_max) {
				$limit = $max % $view1;
			}
			else {
				$limit = $view1;
			}
			$limit_num = " LIMIT $limit";
			if ($offset != 0) {
				$limit_num .= " OFFSET $offset";
			}

			$sql  = "SELECT goods_name, COUNT(goods_name) AS count FROM list" .
					" WHERE display='2'$where" .
					" AND state!='1'".		//	2009/04/25	add ookawara
					" GROUP BY goods_name ORDER BY goods_name$limit_num;";
			if ($result = pg_query($db,$sql)) {
				$i = 0;
				$bgoods = "";
				while ($list = pg_fetch_array($result)) {
					$goods_name = $list['goods_name'];
					$count = $list[count];
					if ($bgoods == $goods_name) { continue; }
					if ($count > 0) {
						$sql  = "SELECT list_num, maker_num, class_m, picture FROM list" .
								" WHERE display='2' AND goods_name='$goods_name'$where" .
								" AND state!='1'".		//	2009/04/25	add ookawara
								" ORDER BY random() LIMIT 1;";
						$result2 = pg_query($db,$sql);
						$list2 = pg_fetch_array($result2);
						$list_num = $list2[list_num];
						$maker_num = $list2[maker_num];
						$maker_num = sprintf("%04d",$maker_num);
						$class_m = $list2[class_m];
						$picture = $list2[picture];
						$num = floor($class_m/100) * 100;
						if ($CHECK[main]) { $num = ""; }
						elseif ($num) { $num = "/$num"; }
						if ($CHECK[m]) { $maker_num = ""; }
						elseif ($maker_num) { $maker_num = "/m$maker_num"; }
						if ($CHECK[s]) { $class_m = ""; }
						elseif ($class_m) { $class_m = "/s$class_m"; }

						if ($count) { $count = $count . "アイテム"; }

						$pic_url = "/pic/$picture";
						if ($picture && file_exists(".$pic_url")) {
							$plist = @getimagesize(".$pic_url");
							$wid = $plist[0];
							$hig = $plist[1];
							if ($wid > $gpx || $hig > $gpy) {
								$w_ritu = $gpx / $wid;
								$h_ritu = $gpy / $hig;
								if ($w_ritu < $h_ritu) { $ritu = $w_ritu; }
								else { $ritu = $h_ritu; }
								$width_ = $wid * $ritu;
								$height_ = $hig * $ritu;
							}
							else {
								$width_ = $wid;
								$height_ = $hig;
							}
							$img_msg = "<IMG src=\"$pic_url\" width=\"$width_\" height=\"$height_\" border=\"0\" alt=\"$goods_name ($count)\">\n";
						} else { $img_msg = ""; }
						$html .= <<<WAKABA
							<li>
								<A class="item-box" href="$URL$num$class_m$maker_num/n$list_num/$index" title="$goods_name ($count)">
									$img_msg
									<div class="product-name">$goods_name</div>
								</A>
							</li>

WAKABA;
						$i++;
						if ($i > 2) { $i = 0; }
					}
					$bgoods = $goods_name;
				} // end while loop
			}

			$html .= "</ul>\n</nav>\n";

			//	ページ処理
			if ($max_page > 1) {
				$html .= "<div class='pagination'>\n";

				if ($page != 1) {
					$page_b = $page-1;
					if ($page_b > 1) { $b_url = $URL . "/index" . $page_b . ".htm"; }
					else { $b_url = "$URL/"; }

					$html .= <<<WAKABA
						<A class="btn btn-primary" href="$b_url">前の $view1 件</A>

WAKABA;
				}

				if ($max_page != $page && $max_page != 1) {
					$page_n = $page + 1;
					$view_n = $max-($page*$view1);
					if ($view_n > $view1) { $view_n = $view1; } else { $view_n = $view_n; }
					if ($page_n > 1) { $n_url = $URL . "/index" . $page_n . ".htm"; }
					else { $n_url = "$URL/"; }
					$html .= <<<WAKABA
						<A class="btn btn-primary" href="$n_url">次の $view_n 件</A>

WAKABA;
				}
				$html .= "</div>\n"; // end pagination
			} // end pagination

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
