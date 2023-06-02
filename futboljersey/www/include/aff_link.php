<?PHP
//	アフィリエイトリンクページ

function aff_link()
{

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
		$cate_table,	//	cateテーブル
		$goods_table,	//	goodsテーブル
		$conn_id; // mysql DB

	//URLから商品番号を取得
	$LIST = explode("/", $PHP_SELF);
	unset($LIST[0]);
	unset($LIST[1]);

	//	商品・ページ番号抜きだし
	if ($LIST) {
		foreach ($LIST as $VAL) {
			if (!$VAL) {
				continue;
			}
			if (preg_match("/^g/", $VAL)) {
				$item_num = $VAL;
				break;
			} elseif (preg_match("/^l/", $VAL) || preg_match("/^s/", $VAL) || !preg_match("/[^0-9]/", $VAL)) {
				$cate_num = $VAL;
				break;
			}
		}
	}

	//	商品チェック
	unset($GOODS);
	if ($item_num) {
		$item_num_ = preg_replace("/[^0-9]/i", "", $item_num);
		//$sql  = "SELECT i.num, j.g_name, j.code" .											//	del ookawara 2015/09/24
		$sql  = "SELECT i.num, coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name, j.code" .	//	add ookawara 2015/09/24
			" FROM $cate_table i,$goods_table j" .
			" WHERE i.g_num=j.g_num AND i.g_num='$item_num_' AND i.display='1' AND i.state<'3' LIMIT 1";
		if ($result = mysqli_query($conn_id, $sql)) {
			$list = mysqli_fetch_array($result);
			if ($list['num'] > 0) {
				$page_num = $item_num_;
				$GOODS['page_num'] = $item_num;
				$GOODS['p_name'] = $list['g_name'];
				$GOODS['code'] = $list['code'];
			}
		}
		//	カテゴリー
	} elseif ($cate_num) {
		if (preg_match("/^l/", $cate_num)) {
			$cate_num_ = preg_replace("/[^0-9]/i", "", $cate_num);
			$cate1 = (int)substr($cate_num_, 0, 2);
			$cate2 = (int)substr($cate_num_, 2, 2);
			$cate3 = (int)substr($cate_num_, 4, 4);
		} elseif (preg_match("/^s/", $cate_num)) {
			$cate_num_ = preg_replace("/[^0-9]/i", "", $cate_num);
			$cate1 = (int)substr($cate_num_, 0, 2);
			$cate2 = (int)substr($cate_num_, 2, 2);
			$cate3 = 0;
		} else {
			$cate1 = (int)$cate_num;
			$cate2 = 0;
			$cate3 = 0;
		}

		//	カテゴリーに商品が登録されているかチェック
		if ($cate1 > 0) {
			$where = " AND cate1='$cate1'";
			if ($cate2 > 0) {
				$where .= " AND cate2='$cate2'";
				if ($cate3 > 0) {
					$where .= " AND cate3='$cate3'";
				}
			}
		}
		unset($count);
		$sql  = "SELECT count(*) AS count FROM $cate_table" .
			" WHERE display='1' $where;";
		if ($result = mysqli_query($conn_id, $sql)) {
			$list = mysqli_fetch_array($result);
			$count = $list['count'];
		}

		unset($p_name);
		if ($count > 0) {
			if ($cate1) {
				$p_name = cate_name($cate1, 0, 0);
			}
			if ($cate2) {
				$p_name .= cate_name($cate1, $cate2, 0);
			}
			if ($cate3) {
				$p_name .= cate_name($cate1, $cate2, $cate3);
			}
			$p_name = trim($p_name);
		}

		if ($p_name) {
			$GOODS['page_num'] = $cate_num;
			$GOODS['p_name'] = $p_name;
			$GOODS['code'] = "";
		}
	} else {
		$GOODS['page_num'] = "";
		$GOODS['p_name'] = "サッカーユニフォームショップ ネイバーズスポーツ";
		$GOODS['code'] = "";
	}

	//	アフィリエイター番号抽出
	if ($_SESSION['idpass']) {
		list($email, $pass, $check, $af_num) = explode("<>", $_SESSION['idpass']);
	}

	if ($af_num > 0 && $GOODS) {
		$GOODS['af_num'] = $af_num;
		list($title, $back_link, $g_html, $t_html, $m_html) = ditaile($GOODS, $item_num);
		$DEL_INPUTS['NOTDITAILE'] = 1;	//	該当商品なし部分削除
	} else {
		list($html, $title) = not_ditaile();
		$DEL_INPUTS['DITAILE'] = 1;		//	該当商品あり部分削除
	}

	$INPUTS['PAGELINK'] = $back_link;	//	「ページに戻る」
	$INPUTS['GHTML'] = $g_html;			//	画像リンク
	$INPUTS['THTML'] = $t_html;			//	テキストリンク
	$INPUTS['MHTML'] = $m_html;			//	メールリンク

	//	html作成・置換
	$make_html = new read_html();
	$make_html->set_dir(TEMPLATE_DIR);
	$make_html->set_file("aff_link.htm");
	$make_html->set_rep_cmd($INPUTS);
	$make_html->set_del_cmd($DEL_INPUTS);
	$html = $make_html->replace();

	return array($title, $html);
}

//	ページ詳細
function ditaile($GOODS, $item_num)
{

	$title = $GOODS['p_name'] . " ページリンク";
	if ($GOODS['page_num']) {
		if ($GOODS['code']) {
			$img_file = "/imagef/" . $GOODS['code'] . ".jpg";
		} else {
			$img_file = "/image/futobol_ba.gif";
		}
	} else {
		$img_file = "/image/futobol_ba.gif";
	}

	//	ページリンク
	if ($GOODS['page_num']) {
		$page_num = $GOODS['page_num'];
		$page_link = "/goods/$page_num/";
	} else {
		$page_link = "/";
	}

	$back_link  = "<div class='backlink'>\n";
	$back_link .= "<a href=\"" . $page_link . "\">戻る</a>\n";
	$back_link .= "</div>\n";

	//	画像リンクHTML
	if (file_exists(".$img_file")) {
		$GOODS['img_file'] = $img_file;
		$g_html = g_html($GOODS, $item_num);
	}

	//	テキストリンクHTML
	$t_html = t_html($GOODS);

	//	メールリンクHTML
	$m_html = m_html($GOODS);

	return array($title, $back_link, $g_html, $t_html, $m_html);
}

//	画像リンクHTML
function g_html($GOODS, $item_num)
{

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
		$URL,			//	サイトURL = "http://www.futboljersey.com"
		$aff_cb_url;	//	アフェリエイトリンク元ファイル名

	$af_num = $GOODS['af_num'];
	$page_num = $GOODS['page_num'];
	$p_name = $GOODS['p_name'];
	$code = $GOODS['code'];
	$img_file = $GOODS['img_file'];

	if ($page_num) {
		if (!preg_match("/^g/", $page_num)) {
			$page_num_ = "";
			$page_num .= "/";
		} else {
			$page_num .= "/";
			$page_num_ = $page_num;
		}
		$p_name .= "\nサッカーユニフォームショップ ネイバーズスポーツ";
		$p_name = trim($p_name);
	}

	$page_num = preg_replace("/g/", "a", $page_num);
	$page_num_ = preg_replace("/g/", "a", $page_num_);

	if ($img_file && file_exists(".$img_file")) {
		//	画像サイズ取得
		list($p_width, $p_height, $p_type) = getimagesize(".$img_file");

		$w_checked = $h_checked = "checked";

		$wh = $_POST['wh'];
		$size = $_POST['size'];
		#		$size = mb_convert_kana($size, "ns", "EUC-JP");	//	add ookawara 2014/01/08
		$size = mb_convert_kana($size, "ns", "UTF-8");
		$size = preg_replace("/[^0-9]/", "", $size);	//	add ookawara 2014/01/08
		if ($wh && $size) {
			//			$p_size = "$wh$size/";
			$p_size = $wh . $size . "/";
			//echo('$p_size=>'.$p_size."<br />");

			if ($wh == "w") {
				$width = $size;
				unset($h_checked);
			} elseif ($wh == "h") {
				$height = $size;
				unset($w_checked);
			}
		}
		if ($w_checked && $h_checked) {
			unset($h_checked);
		}

		if (($width >= 100 && $width < $p_width) || ($height >= 100 && $height < $p_height)) {
			$flag = 1;
			//	サムネイルサイズ
			if ($width) {
				$ritu = $width / $p_width;
				$height = floor($p_height * $ritu + 0.5);
			} else {
				$ritu = $height / $p_height;
				$width = floor($p_width * $ritu + 0.5);
			}
		} else {
			unset($wh);
			unset($size);
		}

		$msg = "<a href=\"" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "\" target=\"_blank\" title=\"" . $p_name . "\"><img src=\"" . $URL . "/pic/" . $page_num_ . $p_size . "\" alt=\"" . $p_name . "\"></a>";
		$msg_ = htmlspecialchars($msg);

		$html .= "	<div class=\"preview\">\n";
		$html .= "		<h5 class=\"preview-header\">＜＜表示例＞＞</h5>";
		$html .= "		<div class=\"preview-image\">";
		$html .= "			" . $msg . "\n";
		$html .= "		</div>";
		$html .= "	</div>\n";
		$html .= "	<div class=\"input-with-clipboard\">";
		$html .= "		<label>リンクURL：</label>";
		$html .= "		<a href=\"" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "\" target=\"_blank\" id=\"image-link-url\">" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "</a><br />\n";
		$html .= "		<div class=\"copy-button btn-standard\" onclick=\"copy({elementID:'image-link-url', onSuccess:transformWithRollback(this,{text: 'コピーしました。'},1000)})\")>コピー</div>";
		$html .= "	</div>";
		if ($item_num) {
			$html .= "<form action=\"" . $PHP_SELF . "\" method=\"POST\">\n";
			$html .= "		<h5 class=\"announce\">画像サイズ指定<span class=\"important\">（※登録されている画像の幅・高さ以上指定してもそれ以上にはなりません。）</span></h5>\n";
			$html .= "		<div class=\"preview-image-controller\">";
			$html .= "      	<div><input type=\"radio\" name=\"wh\" value=\"w\" id=\"dimension-width\"" . $w_checked . "><label for=\"dimension-width\">：幅</label></div>\n";
			$html .= "			<div><input type=\"radio\" name=\"wh\" value=\"h\" id=\"dimension-height\"" . $h_checked . "><label for=\"dimension-height\">：高さ</label></div>\n";
			$html .= "			<div><label for=\"image-size\">サイズ：<input type=\"text\" name=\"size\" class=\"input-length-4-letters\" id=\"image-size\" value=\"" . $size . "\"> px</div>\n";
			$html .= "			<div><input type=\"submit\" class=\"btn-standard\" value=\"変更\">\n";
			$html .= "			</div>";
			$html .= "			<ul>";
			$html .= "      		<li>画像最大サイズ：" . $p_width . "px X " . $p_height . "px<br /></li>\n";
			$html .= "      		<li>最小幅・高さ：100px<br /></li>\n";
			$html .= "			</ul>";
			$html .= "		</div>";
			$html .= "</form>\n";
		}
		$html .= "	<h5 class=\"announce\">下記をコピーしてご利用下さい。</h5>\n";
		$html .= "	<div class=\"textarea-with-clipboard\">";
		$html .= "		<textarea id=\"with-image-link\">" . $msg_ . "</textarea>\n";
		$html .= "		<div class=\"copy-button btn-standard\" onclick=\"copy({elementID:'with-image-link', onSuccess:transformWithRollback(this,{text: 'コピーしました。'},1000)})\">コピー</div>";
		$html .= "	</div>";
	} else {
		$html .= "<div class=\"primary-alert\">只今画像リンクはございません。</div>\n";
	}

	return $html;
}

//	テキストリンク
function t_html($GOODS)
{

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
		$URL,			//	サイトURL = "http://www.futboljersey.com"
		$aff_cb_url;	//	アフェリエイトリンク元ファイル名

	$af_num = $GOODS['af_num'];
	$page_num = $GOODS['page_num'];
	$p_name = $GOODS['p_name'];
	$code = $GOODS['code'];
	$img_file = $GOODS['img_file'];

	if ($page_num) {
		$page_num .= "/";
		$p_name = trim("$p_name\nサッカーユニフォームショップ ネイバーズスポーツ");
	}
	$p_name_ = nl2br($p_name);

	$page_num = preg_replace("/g/", "a", $page_num);

	$msg = "<a href=\"" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "\" target=\"_blank\" title=\"" . $p_name . "\">" . $p_name_ . "</a>";
	$msg_ = htmlspecialchars($msg);

	$html .= "	<div class=\"preview\">\n";
	$html .= "		<h5 class=\"preview-header\">＜＜表示例＞＞</h5>";
	$html .= "		<div class=\"preview-text\">";
	$html .= "			" . $msg . "\n";
	$html .= "		</div>";
	$html .= "	</div>\n";
	$html .= "	<div class=\"input-with-clipboard\">";
	$html .= "		<label>リンクURL：</label>";
	$html .= "		<a href=\"" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "\" target=\"_blank\" id=\"aff-link-url\">" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "</a>\n";
	$html .= "		<div class=\"copy-button btn-standard\" onclick=\"copy({elementID:'aff-link-url', onSuccess:transformWithRollback(this,{text: 'コピーしました。'},1000)})\")>コピー</div>";
	$html .= "	</div>";
	$html .= "	<h5 class=\"announce\">下記をコピーしてご利用下さい。</h5>\n";
	$html .= "	<div class=\"textarea-with-clipboard\">";
	$html .= "		<textarea id=\"aff-link\">" . $msg_ . "</textarea>\n";
	$html .= "		<div class=\"copy-button btn-standard\" onclick=\"copy({elementID:'aff-link', onSuccess:transformWithRollback(this,{text: 'コピーしました。'},1000)})\">コピー</div>";
	$html .= "	</div>";

	return $html;
}

//	メールリンク
function m_html($GOODS)
{

	global	$PHP_SELF,		//	現在実行しているスクリプトのファイル名
		$URL,           //	サイトURL = "http://www.futboljersey.com"
		$aff_cb_url;    //	アフェリエイトリンク元ファイル名

	$af_num = $GOODS['af_num'];
	$page_num = $GOODS['page_num'];
	$p_name = $GOODS['p_name'];
	$code = $GOODS['code'];
	$img_file = $GOODS['img_file'];

	if ($page_num) {
		$page_num .= "/";
		$p_name = trim("$p_name\nサッカーユニフォームショップ ネイバーズスポーツ");
	}

	$page_num = preg_replace("/g/", "a", $page_num);

	$msg = trim("$p_name\n$aff_cb_url/i_$af_num/$page_num");
	$msg_ = htmlspecialchars($msg);
	$msg = nl2br($msg);

	$html .= "	<div class=\"preview\">\n";
	$html .= "		<h5 class=\"preview-header\">＜＜表示例＞＞</h5>";
	$html .= "		<div class=\"preview-text\">";
	$html .= "			" . $msg . "\n";
	$html .= "		</div>";
	$html .= "	</div>\n";
	$html .= "	<div class=\"input-with-clipboard\">";
	$html .= "		<label>リンクURL：</label>";
	$html .= "		<a href=\"" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "\" target=\"_blank\" id=\"aff-mail-url\">" . $aff_cb_url . "/i_" . $af_num . "/" . $page_num . "</a><br />\n";
	$html .= "		<div class=\"copy-button btn-standard\" onclick=\"copy({elementID:'aff-mail-url', onSuccess:transformWithRollback(this,{text: 'コピーしました。'},1000)})\">コピー</div>";
	$html .= "	</div>";
	$html .= "	<h5 class=\"announce\">下記をコピーしてご利用下さい。</h5>\n";
	$html .= "	<div class=\"textarea-with-clipboard\">";
	$html .= "	    <textarea class='' id=\"aff-mail\">$msg_</textarea>\n";
	$html .= "		<div class=\"copy-button btn-standard\" onclick=\"copy({elementID:'aff-mail', onSuccess:transformWithRollback(this,{text: 'コピーしました。'},1000)})\">コピー</div>";
	$html .= "	</div>";

	return $html;
}

//	商品該当無し
function not_ditaile()
{

	$title = "エラー";

	$html .= "<table>\n";
	$html .= "	<tr>\n";
	$html .= "		<td>該当する商品又はページがございません。</td>\n";
	$html .= "	</tr>\n";
	$html .= "</tbody>\n";

	return array($html, $title);
}

//	カテゴリー名読み込み
function cate_name($cate1, $cate2, $cate3)
{

	global	$r_cate_table,	//	r_cateテーブル名
		$conn_id; // mysql DB

	$sql  = "SELECT c_name FROM $r_cate_table" .
		" WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3';";
	if ($result = mysqli_query($conn_id, $sql)) {
		$list = mysqli_fetch_array($result);
		$p_name = " " . $list['c_name'];
	}

	return $p_name;
}
