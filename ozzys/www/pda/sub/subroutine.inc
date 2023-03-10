<?PHP
//	サブルーチンファイル

//	初期ページ
function first() {

	//	検索部分
	$html = search();

	//	商品一覧
	$html .= goods_list();

	return $html;

}



//	検索部分
function search() {
global $PHP_SELF,$db,$TABLE;

	$word = $_SESSION['SEARCH'][word];

	$class_m = $_SESSION['SEARCH'][class_m];
	$maker = $_SESSION['SEARCH'][maker];
	$goods = $_SESSION['SEARCH'][goods];

	$word = trim($word);
	$WORD = explode(" ",$word);
	if ($WORD) {
		foreach ($WORD AS $VAL) {
			$VAL = trim($VAL);
			if (!$VAL) { continue; }
			$where .= " AND a.goods_name like '%$VAL%'";
		}
##		$where = eregi_replace("^ AND","WHERE",$where);
		$where = preg_replace("/^ AND/i","WHERE",$where);	
	}

	$html = <<<WAKABA
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="mode" value="change">
キーワード：<input size="26" type="text" name="word" value="$word"><br>

WAKABA;


	if ($word) {
		//	分類チェック
		$max = 0;
		$l_class = "";
		$sql  = "SELECT a.class_m FROM $TABLE a, class b" .
				" $where AND a.class_m=b.class_m GROUP BY a.class_m ORDER BY a.class_m;";
		if ($result = pg_query($db,$sql)) {
			$max = pg_numrows($result);
		}
		if ($class_m || $max > 1) {
			$sql =  "SELECT class_m, class_m_n FROM class ORDER BY class_m;";
			if ($result2 = pg_query($db,$sql)) {
				while ($list2 = pg_fetch_array($result2)) {
					$class_m_ = $list2['class_m'];
					$class_m_n_ = $list2['class_m_n'];
					$CLASS[$class_m_] = $class_m_n_;
				}
			}

			while ($list = pg_fetch_array($result)) {
				$class_m_ = $list['class_m'];
				$class_name_ = $CLASS[$class_m_];
				if (!$class_m_) { continue; }
				if ($class_m == $class_m_) { $selected = "selected"; } else { $selected = ""; }
				$l_class .= "  <option value=\"$class_m_\" $selected>$class_name_</option>\n";
			}
		}
		if ($l_class) {
			if (!$class_m) { $selected = "selected"; } else { $selected = ""; }
			$html .= <<<WAKABA
分類：<br>
<select name="class_m" OnChange="submit()">
  <option value="" $selected>全て</option>
$l_class
</select><br>

WAKABA;
		}


		//	メーカーチェック
		if ($class_m) {
			$where .= " AND a.class_m='$class_m'";
		}

		$max = 0;
		$l_maker = "";
		$sql  = "SELECT a.maker FROM $TABLE a" .
				" $where GROUP BY a.maker ORDER BY a.maker;";
		if ($result = pg_query($db,$sql)) {
			$max = pg_numrows($result);
		}
		if ($maker || $max > 1) {
			while ($list = pg_fetch_array($result)) {
				$maker_ = $list['maker'];
				if (!$maker_) { continue; }
				if ($maker == $maker_) { $selected = "selected"; } else { $selected = ""; }
				$l_maker .= "  <option value=\"$maker_\" $selected>$maker_</option>\n";
			}
		}
		if ($l_maker) {
			if (!$maker) { $selected = "selected"; } else { $selected = ""; }
			$html .= <<<WAKABA
メーカー：<br>
<select name="maker" OnChange="submit()">
  <option value="" $selected>全て</option>
$l_maker
</select><br>

WAKABA;
		}


		//	商品名チェック
		if ($maker) {
			$where .= " AND a.maker='$maker'";
		}

		$max = 0;
		$l_goods = "";
		$sql  = "SELECT a.goods FROM $TABLE a" .
				" $where GROUP BY a.goods ORDER BY a.goods;";
		if ($result = pg_query($db,$sql)) {
			$max = pg_numrows($result);
		}
		if ($goods || $max > 1) {
			while ($list = pg_fetch_array($result)) {
				$goods_ = $list['goods'];
				if (!$goods_) { continue; }
				if ($goods == $goods_) { $selected = "selected"; } else { $selected = ""; }
				$l_goods .= "  <option value=\"$goods_\" $selected>$goods_</option>\n";
			}
		}
		if ($l_goods) {
			if (!$goods) { $selected = "selected"; } else { $selected = ""; }
			$html .= <<<WAKABA
商品名：<br>
<select name="goods" OnChange="submit()">
  <option value="" $selected>全て</option>
$l_goods
</select><br>

WAKABA;
		}
	}


	$html .= <<<WAKABA
<input type="submit" name="mode" value="検索">　<input type="submit" name="mode" value="リセット"></td>
</form>

WAKABA;

	return $html;

}



//	商品一覧
function goods_list() {
global $PHP_SELF,$db,$TABLE,$view;

	$word = $_SESSION['SEARCH'][word];

	$class_m = $_SESSION['SEARCH'][class_m];
	$maker = $_SESSION['SEARCH'][maker];
	$goods = $_SESSION['SEARCH'][goods];

	$word = trim($word);
	$WORD = explode(" ",$word);
	if ($WORD) {
		foreach ($WORD AS $VAL) {
			$VAL = trim($VAL);
			if (!$VAL) { continue; }
			$where .= " AND goods_name like '%$VAL%'";
		}
##		$where = eregi_replace("^ AND","WHERE",$where);
		$where = preg_replace("/^ AND/i","WHERE",$where);
	}

	if ($class_m) {
		$where .= " AND class_m='$class_m'";
	}
	if ($maker) {
		$where .= " AND maker='$maker'";
	}
	if ($goods) {
		$where .= " AND goods='$goods'";
	}

	$sql  = "SELECT count(*) AS max FROM $TABLE $where;";
	if ($result = pg_query($db,$sql)) {
		$list = pg_fetch_array($result);
		$max = $list['max'];
	}

	if ($max > 0) {
		//	ページ
		$page = $_GET['page'];
		$page = trim($page);
		$page = mb_convert_kana($page,"n","EUC-JP");
		if  (!$page) { $page = 1; }

		$max_page = ceil($max/$view);
		$s = $view * ($page - 1);
		$e = ($view * $page) - 1;
		if ($e >= $max) { $e = $max - 1; }
		$view_s = $s + 1;
		$view_e = $e+1;

		//	商品読み込み数
		$offset = ($page - 1) * $view;
		$limit_max = $offset + $view;
		if ($max < $limit_max) {
			$limit = $max % $view;
		}
		else {
			$limit = $view;
		}
		$limit_num = " LIMIT $limit";
		if ($offset != 0) {
			$limit_num .= " OFFSET $offset";
		}

		$html = <<<WAKABA
該当数 {$view_s}-{$view_e}件目 {$page}/{$max_page}Page<br>
<table border="0" cellspacing="1" width="215" class="list_table">
  <tbody>
    <tr class="list_head">
      <th width="170">商品名</th>
      <th width="45">数量</th>
    </tr>
    <tr class="list_head">
      <th>メーカー</th>
      <th>金額</th>
    </tr>
    <tr class="list_head">
      <th>カラー</th>
      <th rowspan="2">詳細</th>
    </tr>
    <tr class="list_head">
      <th>サイズ</th>
    </tr>
    <tr class="list_head">
      <th colspan="2"></th>
    </tr>

WAKABA;

		$sql  = "SELECT * FROM $TABLE $where ORDER BY maker_name, class_m, goods_name $limit_num;";
		if ($result = pg_query($db,$sql)) {
			$i = 0;
			while ($list = pg_fetch_array($result)) {
				$amari = $i % 2;
				if ($amari == 1) { $num = 2; } else { $num = ""; }

				$pluid = $list['pluid'];
				$goods_name_ = $list['goods_name'];
##				if (eregi(":",$goods_name_)) {
				if (preg_match("/:/i",$goods_name_)) {
					$maker_name = $list['maker_name'];
					$goods_name = $list['goods'];
					$color = $list['color'];
					$size = $list['size'];
				}
				else {
					$goods_name = $goods_name_;
				}

				if (!$maker_name) {
					$maker_name = $list['maker_name'];
				}

				if (!$color) {
					$color = "<center>-----</center>";
				}

				if (!$size) {
					$size = "<center>-----</center>";
				}

				$stock = $list['stock'];
				$price = number_format($list['price']);

				$html .= <<<WAKABA
    <form action='$PHP_SELF' method="POST">
    <input type="hidden" name="pluid" value="$pluid">
    <input type="hidden" name="page" value="$page">
    <tr class="list_goods$num">
      <td>$goods_name</td>
      <td align="center">$stock</td>
    </tr>
    <tr class="list_goods$num">
      <td>$maker_name</td>
      <td align="right">{$price}円</td>
    </tr>
    <tr class="list_goods$num">
      <td>$color</td>
      <td rowspan="2" align="center"><input type="submit" name="mode" value="詳細"></td>
    </tr>
    <tr class="list_goods$num">
      <td>$size</td>
    </tr>
    <tr class="list_goods$num">
      <td colspan="2"></td>
    </tr>
    </form>

WAKABA;
				$i++;
			}
		}

		$html .= <<<WAKABA
  </tbody>
</table>

WAKABA;

		//	ページ処理
		if ($max_page > 1) {
			$html .= <<<WAKABA
      <TABLE border="0" width="95%">
        <TBODY>
          <TR>

WAKABA;

			if ($page != 1) {
				$page_b = $page - 1;
					$b_url = $PHP_SELF;
					if ($page_b > 1) { $b_url .= "?page=$page_b"; }

				$html .= <<<WAKABA
            <TD align="center">
            <A href="$b_url">前の $view 件</A>
            </TD>

WAKABA;
			}

			if ($max_page != $page && $max_page != 1) {
				$page_n = $page + 1;
				$view_n = $max-($page*$view);
				if ($view_n > $view) { $view_n = $view; } else { $view_n = $view_n; }
				$n_url = $PHP_SELF;
				if ($page_n > 1) { $n_url .= "?page=$page_n"; }
				$html .= <<<WAKABA
            <TD align="center">
            <A href="$n_url">次の $view_n 件</A>
            </TD>

WAKABA;
			}

			$html .= <<<WAKABA
　         </TR>
        </TBODY>
      </TABLE>

WAKABA;

		}
	}
	else {
		$html = <<<WAKABA
<br>
該当する商品はありません。

WAKABA;
	}

	return $html;

}



//	商品詳細
function detaile() {
global $PHP_SELF,$db,$TABLE;

	$pluid = $_GET['pluid'];

	if (!$pluid) {
		$ERROR[] = "ポス番号が確認出来ません。";
	}

	if (!$ERROR) {
		$sql  = "SELECT a.pluid, a.goods_name, a.stock, a.price, b.class_m_n, a.maker_id, a.maker_name," .
				" a.m_price, a.maker, a.goods, a.color, a.size FROM $TABLE a, class b" .
				" WHERE a.class_m=b.class_m AND a.pluid='$pluid' LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$max = pg_numrows($result);
		}
		if ($max < 1) { $ERROR[] = "詳細情報が確認出来ませんでした。"; }
	}

	if (!$ERROR) {
		$list = pg_fetch_array($result);
		if ($list) {
			foreach ($list AS $KEY => $VAL) {
				$$KEY = $VAL;
			}
		}

##		if (eregi(":",$goods_name)) {
		if (preg_match("/:/i",$goods_name)) {
			$maker_name = $list['maker_name'];
			$goods_name = $list['goods'];
			$color = $list['color'];
			$size = $list['size'];
		}
		else {
			$goods_name = $goods_name;
		}

		if (!$maker_name) {
			$maker_name = $list['maker_name'];
		}

		if (!$color) {
			$color = "<center>-----</center>";
		}

		if (!$size) {
			$size = "<center>-----</center>";
		}

		$price = number_format($price);

		$html = <<<WAKABA
<br>
<a href="$PHP_SELF">一覧へ戻る</a><br>
<br>
<table border="0" width="215" cellspacing="1" cellpadding="0" bgcolor="#666666">
  <tbody>
    <tr bgcolor="#cccccc">
      <td>商品名</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>$goods_name</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td>在庫数</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>$stock</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td>ポス番号</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>$pluid</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td>メーカー名</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>$maker_name</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td>分類</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>$class_m_n</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td>カラー</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>$color</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td>サイズ</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>$size</td>
    </tr>
    <tr bgcolor="#cccccc">
      <td>値段</td>
    </tr>
    <tr bgcolor="#ffffff">
      <td>{$price}円</td>
    </tr>
  </tbody>
</table>

WAKABA;
	}
	else {
		$html = ERROR($ERROR);
	}

	$html .= <<<WAKABA
<br>
<a href="$PHP_SELF">一覧へ戻る</a>

WAKABA;

	return $html;

}



//	検索項目セッション登録
function seach_check() {
global $PHP_SELF;

	unset($_SESSION['SEARCH']);

	if ($_POST) {
		foreach ($_POST AS $KEY => $VAL) {
			if ($VAL != "") {
				$VAL = mb_convert_kana($VAL,"asKV","EUC-JP");
				$VAL = trim($VAL);
				$_SESSION['SEARCH'][$KEY] = $VAL;
			}
		}
	}

	header ("Location: $PHP_SELF\n\n");

	exit;

}



//	検索項目セッションリセット
function search_reset() {
global $PHP_SELF;

	unset($_SESSION['SEARCH']);

	header ("Location: $PHP_SELF\n\n");

	exit;

}



//	商品詳細項目セット
function check_detaile() {
global $PHP_SELF;

	$pluid = $_POST['pluid'];
	$page = $_POST['page'];
	if ($page) { $_SESSION['SEARCH'][page] = $page; }

	header ("Location: $PHP_SELF?mode=detaile&pluid=$pluid\n\n");

	exit;

}
?>
