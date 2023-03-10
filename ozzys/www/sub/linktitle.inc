<?PHP
//	ページリンク表示
function linktitle($CHECK) {
	global $db,$CLASS_L,$index;

	$max = count($CHECK);
	$i = 1;
	$title = "";
	$link = "";
	$url = "/goods";

	// seo対応
	$seoKeywords = array();
	$position = 0;

	foreach ($CHECK AS $KEY => $VAL) {
		$val = $VAL;
		$name = "";
		if ($KEY == "main") {
			$class = floor($val/100);
			$name = $CLASS_L[$class];
		}
		elseif ($KEY == "s") {
			$name = sname($val);
		}
		elseif ($KEY == "m") {
			$name = mname($val);
			$mname = $name . " ";
		}
		elseif ($KEY == "n") {
			$name = nname($val);
		}
		elseif ($KEY == "g") {
			$name = gname($val);
		}

		if ($name) {
			// seo対応 2017-01-17 
			$seoKeywords[] = $name;
			OzzysSEO::addKeywordBreadcrumb($name);

			$url .= "/$VAL";
			if ($title) {
				$title .= " &raquo; ";
				$link .= " &raquo; ";
			}

			// SEO用のマイクロデータ
			$link .= '<span  itemprop="itemListElement" itemscope itemtype="http://schema.org/ListItem">';
			// パンくずリストリンク作成
			$link .= "<A itemprop=\"item\" href=\"$url/$index\"><span itemprop=\"name\">$name</span></A>";
			$position += 1;
			$link .= "<meta itemprop=\"position\" content=\"$position\" />";
			$link .= "</span>\n";
			
			if ($KEY == "g") {
				$title = $mname . $name;
				break;
			}
			else {
				$title .= $name;
			}
		}
		$i++;
	}

	if ($link) {
		// seo対応 2017-01-17 
		foreach(array_reverse($seoKeywords) as $kw) {
			OzzysSEO::addKeyword($kw);
		}

		$main = <<<WAKABA
		<nav itemscope itemtype="http://schema.org/BreadcrumbList" class="breadcrumbs">
			$link
		</nav>
WAKABA;
	}

	return array($title,$main);

}



//	サブカテゴリー呼び込み
function sname($val) {
	global $db;

##	$val = ereg_replace("[^0-9]","",$val);
	$val = preg_replace("/[^0-9]/","",$val);
	$sql = "SELECT class_m_n FROM class WHERE class_m='$val' LIMIT 1;";
	if ($result = pg_query($db,$sql)) {
		$list = pg_fetch_array($result);
		$class_m_n = $list['class_m_n'];
	}

	return $class_m_n;

}



//	メーカー名呼び込み
function mname($val) {
	global $db;

##	$val = ereg_replace("[^0-9]","",$val);
	$sql = "SELECT maker_name FROM maker WHERE maker_num='$val' LIMIT 1;";
	if ($result = pg_query($db,$sql)) {
		$list = pg_fetch_array($result);
		$maker_name = $list['maker_name'];
	}

	return $maker_name;

}



//	商品名呼び込み
function nname($val) {
	global $db;

##	$val = ereg_replace("[^0-9]","",$val);
	$val = preg_replace("/[^0-9]/","",$val);
	$sql  = "SELECT goods_name FROM list WHERE list_num='$val'".
			" AND state!='1'".		//	2009/04/25	add ookawara
			" LIMIT 1;";
	if ($result = pg_query($db,$sql)) {
		$list = pg_fetch_array($result);
		$goods_name = $list['goods_name'];
	}
	if ($goods_name) { $goods_name .= " 一覧"; }

	return $goods_name;

}



//	詳細商品名呼び込み
function gname($val) {
	global $db;

##	$val = ereg_replace("[^0-9]","",$val);
$val = preg_replace("/[^0-9]/","",$val);
	/*
	   //	del ookawara 2014/04/11
	   $sql  = "SELECT goods_name FROM list".
	   " WHERE list_num='$val'".
	   " AND state!='1'".		//	2009/04/25	add ookawara
	   " LIMIT 1;";
	 */
	//	add ookawara 2014/04/11
	$sql  = "SELECT d.goods_name FROM list d, goods g".
			" WHERE d.pluid=g.pluid".
			" AND d.display='2'".
			" AND d.list_num='$val'".
			" AND d.state!='1'".
			" LIMIT 1;";
	if ($result = pg_query($db,$sql)) {
		$list = pg_fetch_array($result);
		$goods_name = $list['goods_name'];
	}
	if ($goods_name) { $goods_name = "$goods_name (No.$val)"; }

	return $goods_name;

}
?>
