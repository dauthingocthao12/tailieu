<?PHP
/*

	ネイバーズスポーツ　商品一覧表示

*/
function goods_disp_list($VALUE, $CHECK, $VC) {
	global $conn_id; // mysql DB

	//	設定値
	$view = 20;	//	1ページ表示数


	$html = "";

	if ($CHECK['l'] || $CHECK['g']) {
		return $html;
	}

	$page = 1;
	if ($CHECK['p'] > 0) { $page = $CHECK['p']; }

	$where = "";
	if ($VC) {
		$view = 44;
		$vc_where = "";
		foreach ($VC AS $val) {
			if ($vc_where) { $vc_where .= " OR"; }
			$vc_where .= " cate1='".$val."'";
		}
		if ($vc_where) {
			$where .= " AND (".$vc_where.")";
		}
	} elseif ($CHECK['main'] && $CHECK['s']) {
		$where .= " AND category.cate1='".$CHECK['main']."'";
		$where .= " AND category.cate2='".preg_replace("/[^0-9]/","",$CHECK['s'])."'";
	} elseif ($CHECK['main']) {
		$where .= " AND category.cate1='".$CHECK['main']."'";
	} else {
		return $html;
	}

	$count = 0;
	$sql  = "SELECT count(distinct category.g_num) AS count FROM category category".
			" LEFT JOIN goods goods ON  category.g_num=goods.g_num".
			" WHERE category.display='1'".
			" AND category.cate1 NOT IN ('99')".	//	add ookawara 2015/01/29
			" AND goods.g_num>0".			//	add ookawara 2016/01/27
			$where.";";
	if ($result = mysqli_query($conn_id, $sql)) {
		$list = mysqli_fetch_array($result);
		$count = $list['count'];
	}

	if ($count < 1) { return $html; }

	$offset = "";
	$limit = "";
	$limit_max = 0;
	$limit_num = "";
	$page_all = 1;
	$page_all = ceil($count / $view);
	$offset = ($page - 1) * $view;
	$limit_max = $offset + $view;
	if ($count < $limit_max) {
		$limit = $count % $view;
	} else {
		$limit = $view;
	}
	$limit_num = " LIMIT $limit";
	if ($offset != 0) {
		$limit_num .= " OFFSET $offset";
	}

	$goods_list_html = "";
	//$sql  = "SELECT distinct goods.g_num, goods.g_name, goods.code FROM category category".														//	del ookawara 2015/09/24
	$sql  = "SELECT distinct goods.g_num, coalesce(goods.name_head, '') || coalesce(goods.g_name, '') || coalesce(goods.name_foot, '') AS g_name, goods.code FROM category category".		//	add ookawara 2015/09/24
			" LEFT JOIN goods goods ON  category.g_num=goods.g_num".
			" WHERE category.display='1'".
			" AND goods.g_num>0".			//	add ookawara 2016/01/27
			$where.
			" ORDER BY goods.g_num DESC".
			$limit_num.";";
	if ($result = mysqli_query($conn_id, $sql)) {
		$i = 0;
		while ($list = mysqli_fetch_array($result)) {
			$g_num_ = $list['g_num'];
			$g_name_ = $list['g_name'];
			$code_ = $list['code'];

			// if ($i == 0) { $goods_list_html .= "<tr>\n"; }
			$goods_list_html .= "    <div class=\"each-goods-background\">\n";
			$goods_list_html .= "    <div class=\"each-goods\">\n";
			if ($VC) {
				$goods_list_html .= "<a href=\"/goods/g".$g_num_."/\" title=\"$g_name_\">\n";
			} else {
				$goods_list_html .= "<a href=\"./g".$g_num_."/\" title=\"$g_name_\">\n";
			}

			// 商品画像を設定
			$img_file = search_imgfile($code_);
			if ($img_file !== false) {
				$goods_list_html .= "<img src=\"".$img_file."\" border=\"0\" width=\"100\" alt=\"".$g_name_."\" /><br />\n";
			}
			else {
				$goods_list_html .= "<img src='/".IMAGE."/FILLER.jpg' border='0' width='100' alt='FUTBOLJERSEY.com' /><br />\n";
			}

			$g_name_after = change_g_name($g_name_);
			$goods_list_html .= $g_name_after."\n";
			$goods_list_html .= "</a>\n";
			$goods_list_html .= "    </div>\n";
			$goods_list_html .= "    </div>\n";

			// $i++;
			// if ($i >= 4) {
			// 	$i = 0;
			// 	$goods_list_html .= "</tr>\n";
			// }
		}

		// if ($i > 0) {
		// 	$end = 4 - $i;
		// 	for ($i=0; $i<$end; $i++) {
		// 		$goods_list_html .= "    <td>&nbsp;</td>\n";
		// 	}
		// 	$goods_list_html .= "  </tr>\n";
		// }
	}



	if ($goods_list_html) {
		$html .= "<section id=\"category-goods-list\">\n";
		$html .= $goods_list_html;
		$html .= "</section>\n";
	}

	//	ページ処理
	$set_url = ".";
	if (!$VC) {
		$html .= goods_page($set_url,$page,$page_all);
	}

	return $html;
}



//	ページ処理
function goods_page($set_url,$page,$page_all) {

	$html = "";
	$page_max_len = 20;

	if ($page_all <= 1) {
		return $html;
	}

	$page_html = "";
	if ($page > 1) {
		$urls = $set_url;
		$back_page = $page - 1;
		if ($page != 2) { $urls .= "/index".$back_page.".html"; }
		$page_html = "<a href=\"".$urls."\" class=\"next_back\">&lt;&lt;前へ</a>\n";
	}

	$check_s_page = $page - $page_max_len / 2;
	if ($check_s_page > 0) {
		$s = $check_s_page;
	} else {
		$s = 1;
	}
	$e = $s + $page_max_len;
	if ($e > $page_all) { $e = $page_all; }
	$s = $e - $page_max_len;
	if ($s < 1) { $s = 1; }
	for ($i=$s; $i<=$e; $i++) {
		if ($page == $i) {
			$page_html .= "&nbsp;<b>".$i."</b>\n";
		} else {
			if ($i == 1) {
				$urls = $set_url."/";
			} else {
				$urls = $set_url."/index".$i.".html";
			}
			$page_html .= "&nbsp;<a href=\"".$urls."\">".$i."</a>\n";
		}
	}

	if ($page < $page_all) {
		$next_page = $page + 1;
		$urls = $set_url."/index".$next_page.".html";
		$page_html .= "&nbsp;<a href=\"".$urls."\" class=\"next_back\">次へ&gt;&gt;</a>\n";
	}
	if (!$page_html) { return $html; }

	$html = "<div id=\"category-link\">".$page_html."</div>\n";

	return $html;
}

// 商品画像を検索し、あればファイル名を、なければfalseを返す
function search_imgfile($code_) {
	
	$img_file = "/".IMAGEF."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGEB."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGE03."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGE04."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGE05."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGE06."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGE07."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGE08."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	$img_file = "/".IMAGE09."/".$code_.".jpg";
	if (file_exists(".".$img_file)) {
		return $img_file;
	}
	return false;
}

//	ブランドと商品名を分けて、表示する商品名を途中までにする処理
function change_g_name($g_name_) {
	$g_brand_len = mb_strpos($g_name_, "/");
	$g_brand = mb_substr($g_name_, 0, $g_brand_len);
	$g_name_after = mb_substr($g_name_, $g_brand_len+1, 22) . "...";
	return $g_brand . "<br />" . $g_name_after;
}
?>