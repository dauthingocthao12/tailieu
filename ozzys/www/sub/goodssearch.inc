<?PHP
//	商品検索
function goodssearch($word) {
global $PHP_SELF,$db,$view1,$gpx,$gpy,$index;

	$word = $_GET['word'];
	$word = trim($word);
	## $words = mb_convert_kana($word,"asKV","EUC-JP");
	$words = mb_convert_kana($word,"asKV","UTF-8");
	$WORD = explode(" ",$words);
	$where = "";
	if ($WORD) {
		foreach ($WORD AS $VAL) {
			$VAL = trim($VAL);
			if (!$VAL) { continue; }
//			$where .= " AND goods_name like '%$VAL%'";
			$where .= " AND (goods_name like '%$VAL%' OR list_num::text like '%$VAL%')";
		}
	}

	$html = <<<WAKABA

	<section class="productname-list">
		<h2 class="sub-title-prod">商品検索</h2>

		<div class="text-center">
			<FORM action="$PHP_SELF" method="GET">
				<INPUT class="input-inline" type="text" name="word" value="$word"> <button class="btn btn-submit" type="submit">検索</button>
			</FORM>
			<HR>
		</div>

WAKABA;

	//	ページ
	$page = $_GET['page'];
	$page = trim($page);
	## $page = mb_convert_kana($page,"n","EUC-JP");
	$page = mb_convert_kana($page,"n","UTF-8");
	if  (!$page) { $page = 1; }

	if ($where) {
		$sql  = "SELECT count(*) FROM list" .
				" WHERE display='2'$where" .
				" AND state!='1'".		//	2009/04/25	add ookawara
				" GROUP BY goods_name;";
		if ($result = pg_query($db,$sql)) {
			$max = pg_numrows($result);
		}
		if ($max > 0) {
			$max_page = ceil($max/$view1);
			$s = $view1 * ($page - 1);
			$e = ($view1 * $page) - 1;
			if ($e >= $max) { $e = $max - 1; }
			$view_s = $s + 1;
			$view_e = $e+1;

			$page_msg = $max . "件キーワードに当てはまる商品がありました。 ";
			if ($max_page > 1) { $page_msg .= $view_s . "-" . $view_e . "件目 " . $page . "/" . $max_page . "Page"; }
		}
	}

	if (!$word) { $s_msg = "検索する商品名を入力し検索ボタンを押してください。<BR>\n<BR>\n"; }
	elseif (!$page_msg) { $s_msg = "キーワードに当てはまる商品はありませんでした。<BR>\n<BR>\n"; }

	if ($s_msg) {
		$html .= <<<WAKABA
			<div class="pagination">$s_msg</div>

WAKABA;
	}
	else {

		$html .= <<<WAKABA
			<div class="pagination">$page_msg</div>

WAKABA;

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

		$sql  = "SELECT goods_name, COUNT(goods_name) AS count  FROM list" .
			" WHERE display='2'$where" .
			" AND state!='1'".		//	2009/04/25	add ookawara
			" GROUP BY goods_name ORDER BY goods_name$limit_num;";
		if ($result = pg_query($db,$sql)) {
			$html .= "<nav class=\"clearfix\">\n<ul>\n";

			$i = 0;
			$bgoods = "";
			while ($list = pg_fetch_array($result)) {
				$goods_name = $list['goods_name'];
				$count = $list[count];
				if ($bgoods == $goods_name) { continue; }
				if ($count > 0) {

					$sql  = "SELECT list_num, maker_num, class_m, picture FROM list" .
							" WHERE display='2' AND goods_name='$goods_name'" .
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
						$plist = getimagesize(".$pic_url");
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
							<A class="item-box" href="/goods$num$class_m$maker_num/n$list_num/$index" title="$goods_name ($count)">
								$img_msg
								<div class="product-name">
									$goods_name
								</div>
							</A>
						</li>

WAKABA;
					$i++;
					if ($i > 2) { $i = 0; }

				}
				$bgoods = $goods_name;
			}
		}
		$html .= "</ul>\n</nav>\n";

		//	ページ処理
		if ($max_page > 1) {

			$html .= <<<WAKABA
				<div class="pagination">

WAKABA;

			if ($page != 1) {
				$page_b = $page-1;
					$b_url = "$PHP_SELF?word=" . urlencode($words);
					if ($page_b > 1) { $b_url .= "&page=$page_b"; }

				$html .= <<<WAKABA
					<A class="btn btn-primary" href="$b_url">前の $view1 件</A>

WAKABA;
			}

			if ($max_page != $page && $max_page != 1) {
				$page_n = $page + 1;
				$view_n = $max-($page*$view1);
				if ($view_n > $view) { $view_n = $view1; } else { $view_n = $view_n; }
				$n_url = "$PHP_SELF?word=" . urlencode($words);
				if ($page_n > 1) { $n_url .= "&page=$page_n"; }

				$html .= <<<WAKABA
					<A class="btn btn-primary" href="$n_url">次の $view_n 件</A>

WAKABA;
			}

			$html .= <<<WAKABA
				</div>

WAKABA;

		}
	}

	$html .= "</section>\n";

	return $html;

}
?>
