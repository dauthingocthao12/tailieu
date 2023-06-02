<?PHP
//	アフィリエイトリンクページ

function aff_link() {
global $PHP_SELF,$conn_id,$cate_table,$goods_table;

	$LIST = explode("/",$PHP_SELF);
	unset($LIST[0]);
	unset($LIST[1]);

	//	商品・ページ番号抜きだし
	if ($LIST) {
		foreach ($LIST AS $VAL) {
			if (!$VAL) { continue; }
##			if (ereg("^g",$VAL)) {
			if (preg_match("/^g/",$VAL)) {
				$item_num = $VAL;
				break;
			}
##			elseif (ereg("^l",$VAL) || ereg("^s",$VAL) || !ereg("[^0-9]",$VAL)) {
			elseif (preg_match("/^l/",$VAL) || preg_match("/^s/",$VAL) || !preg_match("/[^0-9]/",$VAL)) {
				$cate_num = $VAL;
				break;
			}
		}
	}

	//	商品チェック
	unset($GOODS);
	if ($item_num) {
##		$item_num_ = eregi_replace("[^0-9]","",$item_num);
		$item_num_ = preg_replace("/[^0-9]/i","",$item_num);
		//$sql  = "SELECT i.num, j.g_name, j.code" .											//	del ookawara 2015/09/24
		$sql  = "SELECT i.num, coalesce(j.name_head, '') || coalesce(j.g_name, '') ||  coalesce(j.name_foot, '') AS g_name, j.code" .	//	add ookawara 2015/09/24
				" FROM $cate_table i,$goods_table j" .
				" WHERE i.g_num=j.g_num AND i.g_num='$item_num_' AND i.display='1' AND i.state<'3' LIMIT 1";
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			if ($list['num'] > 0) {
				$page_num = $item_num_;
				$GOODS['page_num'] = $item_num;
				$GOODS['p_name'] = $list['g_name'];
				$GOODS['code'] = $list['code'];
			}
		}
	}
	//	カテゴリー
	elseif ($cate_num) {
##		if (("^l",$cate_num)) {
		if (("/^l/",$cate_num)) {
##			$cate_num_ = eregi_replace("[^0-9]","",$cate_num);
			$cate_num_ = preg_replace("/[^0-9]/i","",$cate_num);
			$cate1 = (int)substr($cate_num_,0,2);
			$cate2 = (int)substr($cate_num_,2,2);
			$cate3 = (int)substr($cate_num_,4,2);
		}
##		elseif (ereg("^s",$cate_num)) {
		elseif (preg_match("/^s/",$cate_num)) {
##			$cate_num_ = eregi_replace("[^0-9]","",$cate_num);
			$cate_num_ = preg_replace("/[^0-9]/i","",$cate_num);
			$cate1 = (int)substr($cate_num_,0,2);
			$cate2 = (int)substr($cate_num_,2,2);
			$cate3 = 0;
		}
		else {
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
		if ($result = mysqli_query($conn_id,$sql)) {
			$list = mysqli_fetch_array($result);
			$count = $list['count'];
		}

		unset($p_name);
		if ($count > 0) {
			if ($cate1) {
				$p_name = cate_name($cate1,0,0);
			}
			if ($cate2) {
				$p_name .= cate_name($cate1,$cate2,0);
			}
			if ($cate3) {
				$p_name .= cate_name($cate1,$cate2,$cate3);
			}
			$p_name = trim($p_name);
		}

		if ($p_name) {
			$GOODS['page_num'] = $cate_num;
			$GOODS['p_name'] = $p_name;
			$GOODS['code'] = "";
		}
	}
	else {
		$GOODS['page_num'] = "";
		$GOODS['p_name'] = "サッカーユニフォームショップ ネイバーズスポーツ";
		$GOODS['code'] = "";
	}

	//	アフィリエイター番号抽出
	if ($_SESSION['idpass']) {
		list($email,$pass,$check,$af_num) = explode("<>",$_SESSION['idpass']);
	}

	if ($af_num > 0 && $GOODS) {
		$GOODS['af_num'] = $af_num;
		list($html,$title) = ditaile($GOODS);
	}
	else {
		list($html,$title) = not_ditaile();
	}

	$html .= "<br>\n";

	return array($html,$title);

}



//	ページ詳細
function ditaile($GOODS) {

	$title = $GOODS['p_name'] . " ページリンク";
	if ($GOODS['page_num']) {
		if ($GOODS['code']) {
			$img_file = "/imagef/" . $GOODS['code'] . ".jpg";
		}
		else {
			$img_file = "/image/futobol_ba.gif";
		}
	}
	else {
		$img_file = "/image/futobol_ba.gif";
	}

	//	ページリンク
	if ($GOODS['page_num']) {
		$page_num = $GOODS['page_num'];
		$page_link = "/goods/$page_num/";
	}
	else {
		$page_link = "/";
	}

	$back_link = <<<WAKABA
      <br>
      <table width="750px">
          <tr>
            <th class="cate2"><a href="$page_link">ページに戻る</a></th>
          </tr>
      </table>

WAKABA;

	$html = $back_link;

	//	画像リンク
	if (file_exists(".$img_file")) {
		$GOODS['img_file'] = $img_file;
		$html .= g_html($GOODS);
	}

	//	テキストリンク
	$html .= t_html($GOODS);

	//	メールリンク
	$html .= m_html($GOODS);

	$html .= $back_link;

	return array($html,$title);

}



//	画像リンク
function g_html($GOODS) {
global $PHP_SELF,$URL,$aff_cb_url,$URL;

	$af_num = $GOODS['af_num'];
	$page_num = $GOODS['page_num'];
	$p_name = $GOODS['p_name'];
	$code = $GOODS['code'];
	$img_file = $GOODS['img_file'];

	if ($page_num) {
##		if (!ereg("^g",$page_num)) {
		if (!preg_match("/^g/",$page_num)) {
			$page_num_ = "";
			$page_num .= "/";
		}
		else {
			$page_num .= "/";
			$page_num_ = $page_num;
		}
		$p_name .= "\nサッカーユニフォームショップ ネイバーズスポーツ";
		$p_name = trim($p_name);
	}

##	$page_num = ereg_replace("g","a",$page_num);
	$page_num = preg_replace("/g/","a",$page_num);
##	$page_num_ = ereg_replace("g","a",$page_num_);
	$page_num_ = preg_replace("/g/","a",$page_num_);

	$html = <<<WAKABA
      <br>
      <table width="750px">
          <tr>
            <th class="cate2">画像リンク</th>
          </tr>
          <tr bgcolor="#ffffff">
            <td><br>

WAKABA;

	if ($img_file && file_exists(".$img_file")) {
		//	画像サイズ取得
		list($p_width,$p_height,$p_type) = getimagesize(".$img_file");

		$w_checked = $h_checked = "checked";

		$wh = $_POST['wh'];
		$size = $_POST['size'];
		if ($wh && $size) {
			$p_size = "$wh$size/";
			if ($wh == "w") {
				$width = $size;
				unset($h_checked);
			}
			elseif ($wh == "h") {
				$height = $size;
				unset($w_checked);
			}
		}
		if ($w_checked && $h_checked) { unset($h_checked); }

		if (($width >= 100 && $width < $p_width) || ($height >= 100 && $height < $p_height)) {
			$flag = 1;
			//	サムネイルサイズ
			if ($width) {
				$ritu = $width / $p_width;
				$height = floor($p_height * $ritu + 0.5);
			}
			else {
				$ritu = $height / $p_height;
				$width = floor($p_width * $ritu + 0.5);
			}
		}
		else {
			unset($wh);
			unset($size);
		}

		$msg = "<a href=\"$aff_cb_url/i_$af_num/$page_num\" target=\"_blank\" title=\"$p_name\"><img src=\"$URL/pic/$page_num_$p_size\" border=\"0\" alt=\"$p_name\"></a>";
		$msg_ = htmlspecialchars($msg);

		$html .= <<<WAKABA
            ・表示例<br>
            $msg<br>
            リンクURL：<a href="$aff_cb_url/i_$af_num/$page_num" target="_blank">$aff_cb_url/i_$af_num/$page_num</a><br>
WAKABA;

		if ($item_num) {
			$html .= <<<WAKABA
            <form action="$PHP_SELF" method="POST">・画像サイズ指定<br>
            <table border="0" cellspacing="5" cellpadding="10">
              <tbody>
                <tr>
                  <td bgcolor="#eeeeee"><input type="radio" name="wh" value="w" $w_checked>：幅　<input type="radio" name="wh" value="h" $h_checked>：高さ　サイズ：<input size="6" type="text" name="size" value="$size"> px　<input type="submit" value="変更"><br>
                  画像最大サイズ： {$p_width}px X {$p_height}px<br>
                  最小幅・高さ：100px
                  </td>
                </tr>
              </tbody>
            </table>
            　（※登録されている画像の幅・高さ以上指定してもそれ以上にはなりません。）<br>
            <br>

WAKABA;
		}

		$html .= <<<WAKABA
            <br>
            下記をコピーしてご利用下さい。<br>
            <textarea rows="8" cols="70" style="background-color : #ffffcc;">$msg_</textarea></form>

WAKABA;
	}
	else {
		$html .= <<<WAKABA
只今画像リンクはございません。<br>
<br>

WAKABA;
	}

	$html .= <<<WAKABA
           </td>
          </tr>
      </table>

WAKABA;

	return $html;

}



//	テキストリンク
function t_html($GOODS) {
global $PHP_SELF,$URL,$aff_cb_url;

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

##	$page_num = ereg_replace("g","a",$page_num);
	$page_num = preg_replace("/g/","a",$page_num);

	$msg = "<a href=\"$aff_cb_url/i_$af_num/$page_num\" target=\"_blank\" title=\"$p_name\">$p_name_</a>";
	$msg_ = htmlspecialchars($msg);

	$html = <<<WAKABA
      <br>
      <table width="750px">
          <tr>
            <th class="cate2">テキストリンク</th>
          </tr>
          <tr bgcolor="#ffffff">
            <td>・表示例<br>
            <table style="border:none;" cellpadding="10" cellspacing="5">
                <tr>
                  <td bgcolor="#ffff80" style="border:none;">$msg</td>
                </tr>
            </table>
            　リンクURL：<a href="$aff_cb_url/i_$af_num/$page_num" target="_blank">$aff_cb_url/i_$af_num/$page_num</a><br>
            <form>下記をコピーしてご利用下さい。<br>
            <textarea rows="8" cols="70" style="background-color : #ffffcc;">$msg_</textarea></form>
            </td>
          </tr>
      </table>

WAKABA;

	return $html;

}



//	メールリンク
function m_html($GOODS) {
global $PHP_SELF,$URL,$aff_cb_url;

	$af_num = $GOODS['af_num'];
	$page_num = $GOODS['page_num'];
	$p_name = $GOODS['p_name'];
	$code = $GOODS['code'];
	$img_file = $GOODS['img_file'];

	if ($page_num) {
		$page_num .= "/";
		$p_name = trim("$p_name\nサッカーユニフォームショップ ネイバーズスポーツ");
	}

##	$page_num = ereg_replace("g","a",$page_num);
	$page_num = preg_replace("/g/","a",$page_num);

	$msg = trim("$p_name\n$aff_cb_url/i_$af_num/$page_num");
	$msg_ = htmlspecialchars($msg);
	$msg = nl2br($msg);

	$html = <<<WAKABA
      <br>
      <table width="750px">
          <tr>
            <th class="cate2">メール用リンク</th>
          </tr>
          <tr bgcolor="#ffffff">
            <td>・表示例<br>
            <table style="border:0px;">
              <tbody>
                <tr>
                  <td bgcolor="#ffff80" style="border:none;">$msg</td>
                </tr>
              </tbody>
            </table>
            　リンクURL：<a href="$aff_cb_url/i_$af_num/$page_num" target="_blank">$aff_cb_url/i_$af_num/$page_num</a><br>
            <form>下記をコピーしてご利用下さい。<br>
            <textarea rows="8" cols="70" style="background-color : #ffffcc;">$msg_</textarea></form>
            </td>
          </tr>
      </table>

WAKABA;

	return $html;

}



//	商品該当無し
function not_ditaile() {

	$title = "エラー";

	$html = <<<WAKABA
<br>
<table border="0">
    <tr>
      <td>該当する商品又はページがございません。</td>
    </tr>
  </tbody>
<br>

WAKABA;

	return array($html,$title);

}



//	カテゴリー名読み込み
function cate_name($cate1,$cate2,$cate3) {
global $conn_id,$r_cate_table;

	$sql  = "SELECT c_name FROM $r_cate_table" .
			" WHERE cate1='$cate1' AND cate2='$cate2' AND cate3='$cate3';";
	if ($result = mysqli_query($conn_id,$sql)) {
		$list = mysqli_fetch_array($result);
		$p_name = " " . $list['c_name'];
	}

	return $p_name;

}
?>
