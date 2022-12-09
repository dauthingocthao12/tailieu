<?PHP
//	請求書作成プログラム

	include("./array.inc");
	include("../sub/array.inc");
	include ("../../cone.inc");

	//	セッション設定
	session_start();
	$_SESSION["USER"];
	$_SESSION["ERROR"];

	$PHP_SELF = $_SERVER['PHP_SELF'];

	if ($_POST['mode']) { $mode = $_POST['mode']; }
	elseif ($_GET['mode']) { $mode = $_GET['mode']; }
	$mode = stripslashes($mode);

	if ($mode == "更新") {
		check_data();
	}
	elseif ($mode == "印刷ページ") {
		regist();
	}
	elseif ($mode == "update" || $mode == "print") {
		$USER = $_SESSION['USER'];
		if (!is_array( $USER )) {
			header ("Location: $PHP_SELF\n\n");
			exit;
		}
	}

	if ($mode == "print") {
		$htm = print_page();
	}
	elseif ($mode == "yahoo") {
		$htm = yahoo($mode);
	}
	elseif ($mode == "rakuten") {
		$htm = rakuten($mode);
	}
	else {
		$htm = first($mode);
	}

	$html  = headers();
	$html .= $htm;
	$html .= footer();

	echo ($html);
	ob_start("mb_output_handler");

	exit;



//	初期ページ
function first($mode) {
global $PHP_SELF,$SITE_TYPE_L;
	$sells_num = $_POST['sells_num'];
//	$sells_num = $_GET['sells_num'];
	$bill_num = $_POST['bill_num'];
//	$bill_num = $_GET['bill_num'];

	if (!$mode && !$sells_num && !$bill_num) {
		unset($_SESSION['USER']);
	}

	if ($mode == "リセット") {
		unset($_SESSION['USER']);
	}
	elseif ($mode == "update" && $_SESSION['USER']) {
		$USER = $_SESSION['USER'];
	}
	elseif ($sells_num) {
		$USER = read_data($sells_num);
	}
	elseif ($bill_num) { 
		$USER = read_pri_data($bill_num);
		$bill_num = $USER['bill_num'];
	}
	elseif ($mode == "yahoo_data") {
		$USER = yahoo_data();
	}
	elseif ($mode == "rakuten_data") {
		$USER = rakuten_data();
	}


	if ($_SESSION['ERROR']) {
		$ERROR = $_SESSION['ERROR'];
		$html = ERROR($ERROR);
		unset($_SESSION['ERROR']);
	}

	//	データー変換
	if ($USER) {
		$bill_num = $USER['bill_num'];
		$kojin_num = $USER['kojin_num'];
		$year = $USER['year'];
		$mon = $USER['mon'];
		$day = $USER['day'];
		$order_num = $USER['order_num'];
		$site_type = $USER['site_type'];
		$GOODS_LIST = $USER['goods_list'];
		$zip = $USER['zip'];
		$zip1 = $USER['zip1'];
		$zip2 = $USER['zip2'];
		$add1 = $USER['add1'];
		$add2 = $USER['add2'];
		$name = $USER['name'];
		$time = $USER['time'];
		$tel = $USER['tel'];
	}

	if (!$GOODS_LIST) {
		//	送料
		$GOODS_LIST[24]['goods_name'] = "送料";
		$GOODS_LIST[24]['num'] = "1";
		$GOODS_LIST[24]['price'] = "698";
		//	送料
		$GOODS_LIST[25]['goods_name'] = "代引手数料";
		$GOODS_LIST[25]['num'] = "1";
		$GOODS_LIST[25]['price'] = "315";
	}

	//	日付表示
	if (!$year && !$mon && !$day) {
		$year = date("Y");
		$mon = date("m");
		$day = date("d");
	}

	//	ショップ選択
	if (!$site_type) { $selected = "selected"; } else { $selected = ""; }
	$l_site_type = "        <option value=\"\" $selected>----------------</option>\n";
	$max = count($SITE_TYPE_L);
	for($i=1; $i<$max; $i++) {
		if ($i == $site_type) { $selected = "selected"; } else { $selected = ""; }
		$l_site_type .= "        <option value=\"$i\" $selected>$SITE_TYPE_L[$i]</option>\n";
	}

	//	合計金額
	$all_price = 0;
	if (!$ERROR && $GOODS_LIST) {
		foreach ($GOODS_LIST AS $KEY => $VAL) {
			$num = $GOODS_LIST[$KEY]['num'];
			$price = $GOODS_LIST[$KEY]['price'];
			$all_price += $num * $price;
		}
	}

	if (!$ERROR && $mode == "update") {
		$submit = "　<input type=\"submit\" name=\"mode\"value=\"印刷ページ\">";
	}
	$html .= <<<WAKABA
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="bill_num" value="$bill_num">
<input type="hidden" name="kojin_num" value="$kojin_num">
<input type="hidden" name="time" value="$time">
<table border="0" height="1050" width="750" cellpadding="0" cellspacing="0">
  <tbody>
    <tr>
      <td height="120" valign="top" colspan="2">
      <table border="0" width="100%">
        <tbody>
          <tr>
            <td width="210"><img src="img/rogo.gif" width="210" height="120" border="0"></td>
            <td align="center" valign="top">
            <h1>　請　求　書 / 納　品　書　</h1>
            <input size="8" type="text" maxlength="4" name="year_$time" value="$year" style="ime-mode: inactive">年<input size="4" type="text" maxlength="2" name="mon_$time" value="$mon" style="ime-mode: inactive">月<input size="4" type="text" maxlength="2" name="day_$time" value="$day" style="ime-mode: inactive">日締切分<br>
            <table border="0">
              <tbody>
                <tr>
                  <td>毎度有り難うございます。
                  下記の通りご請求申し上げます。<br>
                  （入金済の方は納品書になります。）<br>
<font size="-2">
***商品代金10.500円以上で送料無料は自社サイトからの注文だけになります。***<br>
***後払い決済を選択された場合は請求書が後払い.com/株式会社キャッチボールより後日送付されます。***
</font>

                  </td>
                </tr>
              </tbody>
            </table>
            </td>
          </tr>
        </tbody>
      </table>
      </td>
    </tr>
    <tr>
      <td valign="top" height="120" width="350">
      <table border="0" cellpadding="5" class="user_data">
        <tbody>
          <tr>
            <td>〒<input size="10" type="text" maxlength="8" name="zip_$time" value="$zip" style="ime-mode: inactive">
<!--
<input size="6" type="text" maxlength="3" name="zip1_$time" value="$zip1">-<input size="8" type="text" maxlength="4" name="zip2_$time" value="$zip2">
-->
<br>
            　<input size="60" type="text" name="add1_$time" value="$add1" style="ime-mode: active"><br>
            　<input size="60" type="text" name="add2_$time" value="$add2" style="ime-mode: active"><br>
            TEL：<input size="50" type="text" name="tel_$time" value="$tel" style="ime-mode: inactive"><br>
            </td>
          </tr>
          <tr>
            <td align="right"><input size="30" type="text" name="name_$time" value="$name" style="ime-mode: active"> 様</td>
          </tr>
        </tbody>
      </table>
      </td>
      <td align="center" width="400">ショップ選択：<select name="site_type_$time">
$l_site_type
      </select><br>
      <br>
      ご注文番号：<input size="40" type="text" name="order_num_$time" value="$order_num"><br>
      <br>
      ご請求金額　<input size="10" type="text" name="all_price" value="$all_price" readonly>円</td>
    </tr>
    <tr>
      <td colspan="2" valign="top" align="center"><br>
      一度更新ボタンを押し更新しないと印刷ページへのボタンは出てきません。<br>
      <input type="submit" name="mode" value="更新">{$submit}　<input type="submit" name="mode" value="リセット"><br>
      <br>
      <table border="0" width="740" bgcolor="#cccccc" cellspacing="1" cellpadding="3">
        <tbody>
          <tr>
            <th height="25" width="130" class="th_line">日付</th>
            <th width="405" class="th_line">商品名</th>
            <th width="55" class="th_line">数量</th>
            <th width="85" class="th_line">単価</th>
            <th width="85">金額</th>
          </tr>

WAKABA;

	//	商品一覧
	for($i=1; $i<=26; $i++) {
		$amari = $i % 2;
		if ($amari == 1) { $bgcolor = "#ffffff"; } else { $bgcolor = "#efecff"; }
		$year_ = $GOODS_LIST[$i]['year'];
		$mon_ = $GOODS_LIST[$i]['mon'];
		$day_ = $GOODS_LIST[$i]['day'];
		$goods_name_ = $GOODS_LIST[$i]['goods_name'];
		$num_ = $GOODS_LIST[$i]['num'];
		$price_ = $GOODS_LIST[$i]['price'];
		$subtotal_ = $num_ * $price_;
		if ($subtotal_ != 0) {
			$subtotal_ = number_format($subtotal_) . "円";
			if ($subtotal_ < 0) {
				$subtotal_ = "<font color=\"#ff0000\">$subtotal_</font>";
			}
		}
		else { unset($subtotal_); }

		$html .= <<<WAKABA
          <tr bgcolor="$bgcolor" align="right">
            <td height="25" align="center"><input size="4" type="text" maxlength="4" name="goods_list_{$time}[$i][year]" value="$year_" style="ime-mode: inactive">/<input size="2" type="text" maxlength="2" name="goods_list_{$time}[$i][mon]" value="$mon_" style="ime-mode: inactive">/<input size="2" type="text" maxlength="2" name="goods_list_{$time}[$i][day]" value="$day_" style="ime-mode: inactive"></td>
            <td align="left"><input size="60" type="text" name="goods_list_{$time}[$i][goods_name]" value="$goods_name_" style="ime-mode: active"></td>
            <td><input size="5" type="text" name="goods_list_{$time}[$i][num]" value="$num_" style="ime-mode: inactive"></td>
            <td><input size="10" type="text" name="goods_list_{$time}[$i][price]" value="$price_" style="ime-mode: inactive">円</td>
            <td>$subtotal_</td>
          </tr>

WAKABA;
	}

	if ($bill_num) {
		$bill_num = sprintf("%06d",$bill_num);
		$bill_num = "[{$bill_num}]&nbsp;&nbsp;";
	}

	$html .= <<<WAKABA
        </tbody>
      </table>
      </td>
    </tr>
    <tr>
      <td align="right" class="bno" height="30">
      {$bill_num}
      </td>
    </tr>
    <tr>
      <td align="center" height="100" colspan="2"><br>
      <input type="submit" name="mode" value="更新">{$submit}　<input type="submit" name="mode" value="リセット"><br>
      <br>
      </td>
    </tr>
  </tbody>
</table>
</form>

WAKABA;

	return $html;

}



//	印刷ページ
function print_page() {

	$USER = $_SESSION['USER'];
	//	データー変換
	if ($USER) {
		$bill_num = $USER['bill_num'];
		$year = $USER['year'];
		$mon = $USER['mon'];
		$day = $USER['day'];
		$order_num = $USER['order_num'];
		$site_type = $USER['site_type'];
		$GOODS_LIST = $USER['goods_list'];
		$zip = $USER['zip'];
		$zip1 = $USER['zip1'];
		$zip2 = $USER['zip2'];
		$add1 = $USER['add1'];
		$add2 = $USER['add2'];
		$name = $USER['name'];
		$tel = $USER['tel'];
	}

	//	合計金額
	$all_price = 0;
	if (!$ERROR && $GOODS_LIST) {
		foreach ($GOODS_LIST AS $KEY => $VAL) {
			$num = $GOODS_LIST[$KEY]['num'];
			$price = $GOODS_LIST[$KEY]['price'];
			$all_price += $num * $price;
		}
	}
	if ($all_price < 0) { $all_price = 0; }
	$all_price = number_format($all_price);

	if ($tel) { $tel_msg = "            TEL：{$tel}<br>"; }

	$html = <<<WAKABA
<br>
<table border="0" height="1020" width="735" cellpadding="0" cellspacing="0">
  <tbody>
    <tr>
      <td height="120" valign="top">
      <table border="0" width="100%">
        <tbody>
          <tr>
            <td width="210"><img src="img/rogo.gif" width="210" height="120" border="0"></td>
            <td align="center" valign="top">
            <h1>　請　求　書 / 納　品　書　</h1>
            <span class="hiduke">{$year}年{$mon}月{$day}日締切分</span><br>
            <table border="0">
              <tbody>
                <tr>
                  <td class="aisatsu">毎度有り難うございます。
                  下記の通りご請求申し上げます。<br>
                  （入金済の方は納品書になります。）<br>
<font size="-2">
***商品代金10.500円以上で送料無料は自社サイトからの注文だけになります。***<br>
***後払い決済を選択された場合は請求書が後払い.com/株式会社キャッチボールより後日送付されます。***
</font>
                  </td>
                </tr>
              </tbody>
            </table>
            </td>
          </tr>
        </tbody>
      </table>
      </td>
    </tr>
    <tr>
      <td valign="top" height="110">
      <table border="0" cellpadding="5" cellspacing="0" class="user_data_waku">
        <tbody>
          <tr>
            <td class="user_data" nowrap>
            〒{$zip}<!--{$zip1}-{$zip2}--><br>
            　{$add1}<br>
            　{$add2}<br>
$tel_msg
            </td>
            <td rowspan="2" align="center" width="100%"><span class="ordermsg">ご注文番号：{$order_num}</span><br>
            <br>
            <span class="allprice">ご請求金額　{$all_price}円</span></td>
          </tr>
          <tr>
            <td align="right" class="user_data">{$name} 様</td>
          </tr>
        </tbody>
      </table>
      </td>
    </tr>
    <tr>
      <td valign="top" align="center">
      <table border="0" width="725" bgcolor="#cccccc" cellspacing="1" cellpadding="3">
        <tbody>
          <tr>
            <th height="25" width="130" class="th_line">日付</th>
            <th width="405" class="th_line">商品名</th>
            <th width="50" class="th_line">数量</th>
            <th width="80" class="th_line">単価</th>
            <th width="80">金額</th>
          </tr>

WAKABA;

	//	商品一覧
	for($i=1; $i<=26; $i++) {
		$amari = $i % 2;
		if ($amari == 1) { $bgcolor = "#ffffff"; } else { $bgcolor = "#efecff"; }
		$year_ = $GOODS_LIST[$i]['year'];
		$mon_ = $GOODS_LIST[$i]['mon'];
		$day_ = $GOODS_LIST[$i]['day'];
		$goods_name_ = $GOODS_LIST[$i]['goods_name'];
		$num_ = $GOODS_LIST[$i]['num'];
		$price_ = $GOODS_LIST[$i]['price'];
		$subtotal_ = $num_ * $price_;
		if ($subtotal_ != 0) {
			$subtotal_ = number_format($subtotal_) . "円";
			if ($subtotal_ < 0) {
				$subtotal_ = "<font color=\"#ff0000\">$subtotal_</font>";
			}
		}
		else { unset($subtotal_); }

		$date_msg = "";
		if ($year_ && $mon_ && $day_) {
			$date_msg = $year_ . "年" . $mon_ . "月" . $day_ . "日";
		}

		if ($num_ > 0) { $num = number_format($num_); }
		else { $num = ""; }

		if ($price_ > 0) { $price = number_format($price_) . "円"; }
		else { $price = ""; }

		if ($goods_name_ == "del") { continue; }

		$html .= <<<WAKABA
          <tr bgcolor="$bgcolor" align="right">
            <td height="23" align="center">$date_msg</td>
            <td align="left">$goods_name_</td>
            <td>$num</td>
            <td>$price</td>
            <td>$subtotal_</td>
          </tr>

WAKABA;
	}

	$bill_num = sprintf("%06d",$bill_num);

	$html .= <<<WAKABA
        </tbody>
      </table>
      </td>
    </tr>
    <tr>
      <td align="right" class="bno" height="20">
      [{$bill_num}]&nbsp;&nbsp;
      </td>
    </tr>
    <tr>
      <td align="center" height="100" class="footer" valign="top">
      株式会社ゼロアワー<span class="footername">ネイバーズスポーツ</span><br>
      〒771-1262　徳島県板野郡藍住町笠木字中野72-1<br>
      TEL：088-677-3170 FAX：088-692-7031<br>
      E-mail：query@futboljersey.com
      </td>
    </tr>
  </tbody>
</table>

WAKABA;

	unset($USER);
	unset($_SESSION['USER']);

	return $html;

}



//	既存データー読み込み
function read_data($sells_num) {
global $conn_id,$TAX_,$PRF_N,$UN_N,$DAIBIKI_N,$DAIBIKI_P_N,$TESU_P,$HAITATU_N,$SHIHARAI_N,$ZAIKO_N,
		$SEBAN_N,$SEBAN_P_N,$SENAME_N,$SENAME_P_N,$MUNEBAN_N,$MUNEBAN_P_N,$PANT_N,$PANT_P_N,$BACH_N,
		$BACH_P_N,$mochi_pri,$CON_TESU,$SHIHARAI_N1;

	$SHIHARAI_N = $SHIHARAI_N1;

	//	ショップ
	$USER['site_type'] = 1;

	//	受注日付
	$USER['year'] = "20" . substr($sells_num,0,2);
	$USER['mon'] = substr($sells_num,2,2);
	$USER['day'] = substr($sells_num,4,2);

	//	ご注文番号
	$USER['order_num'] = "ORG-" . $sells_num;

	//	商品データー抜き出し
	$i = 1;
	$all_price = 0;
	$sql = "SELECT * FROM sells WHERE send!='2' AND sells_num='$sells_num';";
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$kojin_num = $list['kojin_num'];
			$add_num = $list['add_num'];
			$hinban = $list['hinban'];
			$title = $list['title'];
			$price = $list['price'];
			$num = $list['buy_n'];
			$bargain = $list['bargain'];
			$TAX_ = $list['tax'];	//	add ookawara 2014/03/31

			$price = floor($price + ($price * $TAX_) + 0.5);
			$all_price += $price * $num;

			//	add ookawara 2010/07/31
			$g_num = "";
			if ($hinban) {
				$code = "";
				$CODE = explode("-",$hinban);
				if ($CODE) {
					$name_count = count($CODE) - 1;
					for($ad=0; $ad<$name_count; $ad++) {
						if ($code) { $code .= "-"; }
						$code .= $CODE[$ad];
					}
				}
				if ($code) {
					$sql  = "SELECT g_num FROM goods".
							" WHERE code='".$code."';";
					if ($result2 = pg_query($conn_id,$sql)) {
						$list2 = pg_fetch_array($result2);
						$g_num = $list2['g_num'];
					}
				}
			}

			if ($hinban != "option") {
				$USER['goods_list'][$i]['goods_name'] = $title;
				$USER['goods_list'][$i]['price'] = $price;
				$USER['goods_list'][$i]['num'] = $num;
				$i++;
				$USER['goods_list'][$i]['goods_name'] = "　({$hinban})";

				//	add ookawara 2010/07/31
				if ($g_num > 0) {
					$USER['goods_list'][$i]['goods_name'] .= " [g" . $g_num . "]";
				}

				$i++;
			}
		}

	}

	//	オプション抜き出し
	$sql = "SELECT * FROM option WHERE send!='2' AND sells_num='$sells_num';";
	if ($result = pg_query($conn_id,$sql)) {
		while ($list = pg_fetch_array($result)) {
			$kojin_num = $list['kojin_num'];
			$hinban = $list['hinban'];
			$title = $list['title'];
			$seban_l = $list['seban_l'];
			$seban_num = $list['seban_num'];
			$seban_price = $list['seban_price'];
			$sename_l = $list['sename_l'];
			$sename_name = $list['sename_name'];
			$sename_price = $list['sename_price'];
			$muneban_l = $list['muneban_l'];
			$muneban_num = $list['muneban_num'];
			$muneban_price = $list['muneban_price'];
			$pant_l = $list['pant_l'];
			$pant_num = $list['pant_num'];
			$pant_price = $list['pant_price'];
			$bach_l = $list['bach_l'];
			$bach_name = $list['bach_name'];
			$bach_price = $list['bach_price'];

			//	持ち込み手数料
			if ($hinban == "mochikomi") {
				$num = 1;
				$price = floor($mochi_pri + ($mochi_pri * $TAX_) + 0.5);
				$all_price += $price * $num;

				$USER['goods_list'][$i]['goods_name'] = $title;
				$USER['goods_list'][$i]['price'] = $price;
				$USER['goods_list'][$i]['num'] = $num;
				$i++;
			}
			else {
				$USER['goods_list'][$i]['goods_name'] = "マーキング ($hinban)";
				$i++;
			}

			//	背番号
			if ($seban_l) {
				$num = strlen($seban_num);
				$price = $seban_price / $num;
				$price = floor($price + ($price * $TAX_) + 0.5);
				$all_price += $price * $num;

				$USER['goods_list'][$i]['goods_name'] = "背番号 {$SEBAN_N[$seban_l]} 番号：{$seban_num}";
				$USER['goods_list'][$i]['price'] = $price;
				$USER['goods_list'][$i]['num'] = $num;
				$i++;
			}

			//	背ネーム
			if ($sename_l) {
				$sename_name = str_replace('\\', '', $sename_name);
				$sename_name_m = str_replace(' ', '', $sename_name);
				$num = strlen($sename_name_m);
				$price = $sename_price / $num;
				$price = floor($price + ($price * $TAX_) + 0.5);
				$all_price += $price * $num;

				$USER['goods_list'][$i]['goods_name'] = "背ネーム {$SENAME_N[$sename_l]} ネーム：{$sename_name}";
				$USER['goods_list'][$i]['price'] = $price;
				$USER['goods_list'][$i]['num'] = $num;
				$i++;
			}

			//	胸番号
			if ($muneban_l) {
				$num = strlen($muneban_num);
				$price = $muneban_price / $num;;
				$price = floor($price + ($price * $TAX_) + 0.5);
				$all_price += $price * $num;

				$USER['goods_list'][$i]['goods_name'] = "胸番号 {$MUNEBAN_N[$muneban_l]} 番号：{$muneban_num}";
				$USER['goods_list'][$i]['price'] = $price;
				$USER['goods_list'][$i]['num'] = $num;
				$i++;
			}

			//	パンツ番号
			if ($pant_l) {
				$num = strlen($pant_num);
				$price = $pant_price / $num;
				$price = floor($price + ($price * $TAX_) + 0.5);
				$all_price += $price * $num;

				$USER['goods_list'][$i]['goods_name'] = "パンツ番号 {$PANT_N[$pant_l]} 番号：{$pant_num}";
				$USER['goods_list'][$i]['price'] = $price;
				$USER['goods_list'][$i]['num'] = $num;
				$i++;
			}

			//	バッジ
			if ($bach_l) {
				$num = 1;
				$price = $bach_price / $num;
				$price = floor($price + ($price * $TAX_) + 0.5);
				$all_price += $price * $num;

				$USER['goods_list'][$i]['goods_name'] = "バッジ {$BACH_N[$bach_l]}";
				$USER['goods_list'][$i]['price'] = $price;
				$USER['goods_list'][$i]['num'] = $num;
				$i++;
			}
		}
	}

	$USER['goods_list'][1]['year'] = date("Y");
	$USER['goods_list'][1]['mon'] = date("m");
	$USER['goods_list'][1]['day'] = date("d");

//$bargain = 20;

	$USER['kojin_num'] = $kojin_num;
	$USER['add_num'] = $add_num;
	$USER['bargain'] = $bargain;

	if ($bargain > 0) {
		$num = 1;
		//$price = 0 - floor(($all_price * $bargain / 100) + 0.5);	//	del ookawara 2014/02/25
		$price = 0 - floor($all_price * $bargain / 100);	//	add ookawara 2014/02/25
		$all_price += $price;

		$USER['goods_list'][$i]['goods_name'] = "割引 ({$bargain}%)";
		$USER['goods_list'][$i]['price'] = $price;
		$USER['goods_list'][$i]['num'] = $num;
		$i++;
	}

	//	お客様情報読み込み
	$sql  = "SELECT name_s, name_n, zip1, zip2, prf, city, add1, add2, zaiko, siharai, t_time, msr, g_point, shipping," .
			" tel1, tel2, tel3, kei1, kei2, kei3" .
			" FROM add" .
			" WHERE add_num='$add_num';";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$name_s_ = $list['name_s'];
		$name_n_ = $list['name_n'];
		$zip1_ = sprintf("%03d",$list['zip1']);
		$zip2_ = sprintf("%04d",$list['zip2']);
		$prf_ = $list['prf'];
		$city_ = $list['city'];
		$add1_ = $list['add1'];
		$add2_ = $list['add2'];
		$zaiko_ = $list['zaiko'];
		$siharai_ = $list['siharai'];
		$t_time_ = $list['t_time'];
		$msr_ = $list['msr'];
		$g_point_ = $list['g_point'];
		$shipping_ = $list['shipping'];
		$tel1_ = $list['tel1'];
		$tel2_ = $list['tel2'];
		$tel3_ = $list['tel3'];
		$kei1_ = $list['kei1'];
		$kei2_ = $list['kei2'];
		$kei3_ = $list['kei3'];
	}

	if ($tel1_ && $tel2_ && $tel3_) {
		$tel = $tel1_ . "-" . $tel2_ . "-" . $tel3_;
	}
	elseif ($kei1_ && $kei2_ && $kei3_) {
		$tel = $kei1_ . "-" . $kei2_ . "-" . $kei3_;
	}

	$USER['zip'] = "$zip1_" . "-" . "$zip2_";
	$USER['zip1'] = $zip1_;
	$USER['zip2'] = $zip2_;
	$USER['add1'] = $PRF_N[$prf_] . $city_ . $add1_;
	$USER['add2'] = $add2_;
	$USER['name'] = "$name_s_ $name_n_";
	$USER['shipping'] = $shipping_;
	$USER['tel'] = $tel;

//$g_point_ = 3000;
	if ($g_point_ > 0) {
		$num = 1;
		$price = 0 - $g_point_;
		$all_price += $price;

		$USER['goods_list'][$i]['goods_name'] = "割引ポイント利用 ({$g_point_}pt)";
		$USER['goods_list'][$i]['price'] = $price;
		$USER['goods_list'][$i]['num'] = $num;
		$i++;
	}

	//	送料
	$num = 1;
	$flag = 0;
	$shipping_msg = "";
	if ($shipping_ == "") {
		$price = $UN_N[$prf_];
		$price = floor($price + ($price * $TAX_) + 0.5);
	}
	else {
		$price = $shipping_;
		$flag = 1;
	}
	$all_price += $price;
	$unchin = $price;

	if ($flag == 1 && $shipping_ == 0) {
		$shipping_msg = " (サービス)";
		unset($price);
		unset($num);
	}

	$USER['goods_list'][$i]['goods_name'] = "送料{$shipping_msg}";
	$USER['goods_list'][$i]['price'] = $price;
	$USER['goods_list'][$i]['num'] = $num;
	$i++;

//$TESU_P = 100;
	//	代引き手数料
	if ($siharai_ == 1 && $all_price > 0) {
		$TESU_P = floor($TESU_P + ($TESU_P * $TAX_) + 0.5);
		if ($TESU_P > 0) {
			$num = 1;
			$price = $TESU_P;
			$USER['goods_list'][$i]['goods_name'] = "支払手数料";
			$USER['goods_list'][$i]['price'] = $price;
			$USER['goods_list'][$i]['num'] = $num;
			$i++;
		}

		$all_price += $TESU_P;
		$max = count($DAIBIKI_N);
		$daibiki = "";
		for($d=0; $d<$max; $d++) {
			$p_all = $all_price;
			if (!$daibiki && $DAIBIKI_N[$d] >= $p_all) {
				$daibiki = $DAIBIKI_P_N[$d];
				$daibiki = floor($daibiki + ($daibiki * $TAX_) + 0.5);
				$p_all += $daibiki;
				if ($DAIBIKI_N[$d] >= $p_all) {
					$price = $daibiki;
				}
				else {
					$daibiki = "";
					$tax = "";
				}
			}
		}
		$num = 1;
		$USER['goods_list'][$i]['goods_name'] = "代引手数料";
		$USER['goods_list'][$i]['price'] = $price;
		$USER['goods_list'][$i]['num'] = $num;
		$i++;
	} elseif ($siharai_ == 4 && $all_price > 0) {
		$con_tesu = floor($CON_TESU * ($TAX_ + 1) + 0.5);
		$num = 1;
		$USER['goods_list'][$i]['goods_name'] = "請求書発行手数料";
		$USER['goods_list'][$i]['price'] = $con_tesu;
		$USER['goods_list'][$i]['num'] = $num;
		$i++;
	//	後払い決済手数料追加 2010/12/10 add ookawara
	} elseif ($siharai_ == 5 && $all_price > 0) {
		$ato_tesu = floor($all_price * (atobarai / 100) + 0.5);
		if ($ato_tesu < ato_low_price) { $ato_tesu = ato_low_price; }	// add ookawara 2011/01/20
		$num = 1;
		$USER['goods_list'][$i]['goods_name'] = "後払い決済手数料";
		$USER['goods_list'][$i]['price'] = $ato_tesu;
		$USER['goods_list'][$i]['num'] = $num;
		$i++;
	}

	//	支払い方法
	$USER['goods_list'][$i]['goods_name'] = "支払方法：{$SHIHARAI_N[$siharai_]}";
	$i++;

	//	配達希望時間
	if ($t_time_ > 0) {
		$USER['goods_list'][$i]['goods_name'] = "配達希望時間：{$HAITATU_N[$t_time_]}";
		$i++;
	}

	//	在庫無き場合
	if ($zaiko_) {
		$USER['goods_list'][$i]['goods_name'] = "在庫無き場合：{$ZAIKO_N[$zaiko_]}";
		$i++;
	}

	//	メッセージ
	if ($msr_) {
##		$msr_ = eregi_replace("\r","",$msr_);
        $msr_ = preg_replace("/\r/i","",$msr_);
##		$msr_ = eregi_replace("\n","",$msr_);
        $msr_ = preg_replace("/\n/i","",$msr_);
		$USER['goods_list'][$i]['goods_name'] = "{$msr_}";
		$i++;
	}

	$USER['time'] = time();

	return $USER;

}



//	保存データー読み込み
function read_pri_data($bill_num) {
global $conn_id;

	//	基本データー読み込み
	$sql  = "SELECT * FROM bill_base WHERE bill_num='$bill_num' LIMIT 1;";
	if ($result = pg_query($conn_id,$sql)) {
		$list = pg_fetch_array($result);
		$USER['bill_num'] = $list['bill_num'];
		$bill_date = $list['bill_date'];
		$USER['site_type'] = $list['site_type'];
		$USER['order_num'] = $list['order_num'];
		$USER['zip1'] = $zip1 = sprintf("%03d",$list['zip1']);
		$USER['zip2'] = $zip2 = sprintf("%04d",$list['zip2']);
		$USER['zip'] = "$zip1" . "-" . "$zip2";
		$USER['add1'] = $list['add1'];
		$USER['add2'] = $list['add2'];
		$USER['name'] = $list['name'];
		$USER['kojin_num'] = $list['kojin_num'];
		$USER['tel'] = $list['tel'];
	}

	list($year,$mon,$day) = explode("-",$bill_date);
	$USER['year'] = $year;
	$USER['mon'] = $mon;
	$USER['day'] = $day;

	//	詳細データー読み込み
	$sql  = "SELECT * FROM bill_list WHERE bill_num='$bill_num' AND state='0' ORDER BY list_num;";
	if ($result = pg_query($conn_id,$sql)) {
		WHILE($list = pg_fetch_array($result)) {
			$i = $list['list_num'];
			$USER['goods_list'][$i]['goods_name'] = $list['goods_name'];
			$USER['goods_list'][$i]['num'] = $list['num'];
			$USER['goods_list'][$i]['price'] = $list['price'];
			$list_date = $list['list_date'];

			list($year,$mon,$day) = explode("-",$list_date);
			$USER['goods_list'][$i]['year'] = $year;
			$USER['goods_list'][$i]['mon'] = $mon;
			$USER['goods_list'][$i]['day'] = $day;
		}
	}

	$USER['time'] = time();

	return $USER;

}



//	入力データー確認
function check_data() {
global $PHP_SELF;

	unset($_SESSION['USER']);

	$time = $_POST['time'];
	$bill_num = $_POST['bill_num'];
	$kojin_num = $_POST['kojin_num'];
	$year = $_POST["year_$time"];
#	$year = mb_convert_kana($year,"ns","EUC-JP");
	$year = mb_convert_kana($year,"ns","UTF-8");
	$year = trim($year);
	$mon = $_POST["mon_$time"];
#	$mon = mb_convert_kana($mon,"ns","EUC-JP");
	$mon = mb_convert_kana($mon,"ns","UTF-8");
	$mon = trim($mon);
	$day = $_POST["day_$time"];
#	$day = mb_convert_kana($day,"ns","EUC-JP");
	$day = mb_convert_kana($day,"ns","UTF-8");
	$day = trim($day);
	$order_num = $_POST["order_num_$time"];
#	$order_num = mb_convert_kana($order_num,"ns","EUC-JP");
	$order_num = mb_convert_kana($order_num,"ns","UTF-8");
	$order_num = trim($order_num);
	$site_type = $_POST["site_type_$time"];
	$goods_list = $_POST["goods_list_$time"];
	$zip = $_POST["zip_$time"];
#	$zip = mb_convert_kana($zip,"asKV","EUC-JP");
	$zip = mb_convert_kana($zip,"asKV","UTF-8");
	$zip = trim($zip);
	$zip1 = $_POST["zip1_$time"];
#	$zip1 = mb_convert_kana($zip1,"ns","EUC-JP");
	$zip1 = mb_convert_kana($zip1,"ns","UTF-8");
	$zip1 = trim($zip1);
	$zip2 = $_POST["zip2_$time"];
#	$zip2 = mb_convert_kana($zip2,"ns","EUC-JP");
	$zip2 = mb_convert_kana($zip2,"ns","UTF-8");
	$zip2 = trim($zip2);
	$add1 = $_POST["add1_$time"];
#	$add1 = mb_convert_kana($add1,"asKV","EUC-JP");
	$add1 = mb_convert_kana($add1,"asKV","UTF-8");
	$add1 = trim($add1);
	$add2 = $_POST["add2_$time"];
#	$add2 = mb_convert_kana($add2,"asKV","EUC-JP");
	$add2 = mb_convert_kana($add2,"asKV","UTF-8");
	$add2 = trim($add2);
	$name = $_POST["name_$time"];
#	$name = mb_convert_kana($name,"asKV","EUC-JP");
	$name = mb_convert_kana($name,"asKV","UTF-8");
	$name = trim($name);
	$tel = $_POST["tel_$time"];
#	$tel = mb_convert_kana($tel,"ns","EUC-JP");
	$tel = mb_convert_kana($tel,"ns","UTF-8");
	$tel = trim($tel);

	if ($goods_list) {
		foreach ($goods_list AS $KEY=>$VAL) {
			if ($VAL) {
				foreach ($VAL AS $key=>$val) {
#					$val = mb_convert_kana($val,"asKV","EUC-JP");
					$val = mb_convert_kana($val,"asKV","UTF-8");
					$val = trim($val);
					if ($val == "") { continue; }
					$GOODS_LIST[$KEY][$key] = $val;
				}
			}
		}
	}

	if (!$year) { $ERROR[] = "締切分の日付（年）が入力されておりません。"; }
	if (!$mon) { $ERROR[] = "締切分の日付（月）が入力されておりません。"; }
	if (!$day) { $ERROR[] = "締切分の日付（日）が入力されておりません。"; }
	if (!$order_num) { $ERROR[] = "ご注文番号が確認出来ません。"; }
	if (!$site_type) { $ERROR[] = "ショップ選択が選択されておりません。"; }
	if (!$GOODS_LIST) { $ERROR[] = "請求内容が入力されておりません。"; }
	if (!$zip) { $ERROR[] = "郵便番号が入力されておりません。"; }
##	elseif ($zip && !eregi("([0-9]{3})-([0-9]{4})",$zip)) { $ERROR[] = "郵便番号が不正です。"; }
	elseif ($zip && !preg_match("/([0-9]{3})-([0-9]{4})/i",$zip)) { $ERROR[] = "郵便番号が不正です。"; }
//	if (!$zip1) { $ERROR[] = "郵便番号（３桁）が入力されておりません。"; }
//	if (!$zip2) { $ERROR[] = "郵便番号（４桁）が入力されておりません。"; }
	if (!$add1) { $ERROR[] = "住所が入力されておりません。"; }
//	if (!$add2) { $ERROR[] = "住所が入力されておりません。"; }
	if (!$name) { $ERROR[] = "お客様のお名前が入力されておりません。"; }
//	if (!$tel) { $ERROR[] = "お客様の電話番号が入力されておりません。"; }

	$_SESSION['USER']['time'] = $time;
	$_SESSION['USER']['bill_num'] = $bill_num;
	$_SESSION['USER']['kojin_num'] = $kojin_num;
	$_SESSION['USER']['year'] = $year;
	$_SESSION['USER']['mon'] = $mon;
	$_SESSION['USER']['day'] = $day;
	$_SESSION['USER']['order_num'] = $order_num;
	$_SESSION['USER']['site_type'] = $site_type;
	$_SESSION['USER']['goods_list'] = $GOODS_LIST;
	$_SESSION['USER']['zip'] = $zip;
	$_SESSION['USER']['zip1'] = $zip1;
	$_SESSION['USER']['zip2'] = $zip2;
	$_SESSION['USER']['add1'] = $add1;
	$_SESSION['USER']['add2'] = $add2;
	$_SESSION['USER']['name'] = $name;
	$_SESSION['USER']['tel'] = $tel;

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
	}

	header ("Location: $PHP_SELF?mode=update\n\n");

	exit;

}



//	データー保存
function regist() {
global $PHP_SELF,$conn_id;

	$USER = $_SESSION['USER'];
	//	データー変換
	if ($USER) {
		$bill_num = $USER['bill_num'];
		$kojin_num = $USER['kojin_num'];
		$year = $USER['year'];
		$mon = $USER['mon'];
		$day = $USER['day'];
		$order_num = $USER['order_num'];
		$site_type = $USER['site_type'];
		$GOODS_LIST = $USER['goods_list'];
		$zip = $USER['zip'];
		$zip1 = $USER['zip1'];
		$zip2 = $USER['zip2'];
		$add1 = $USER['add1'];
		$add2 = $USER['add2'];
		$name = $USER['name'];
		$tel = $USER['tel'];
	}

	if (!$kojin_num) { $kojin_num = "NULL"; } else { $kojin_num = "'" . $kojin_num . "'"; }

	$bill_date = $year . "-" . $mon . "-" . $day;

	if ($zip) {
##		$zip = eregi_replace("[^0-9]","",$zip);
		$zip = preg_replace("/[^0-9]/i","",$zip);
		$zip1 = substr($zip,0,3);
		$zip2 = substr($zip,3,4);
	}

	if ($bill_num) {	//	更新
		$sql  = "UPDATE bill_base SET" .
				" bill_date='$bill_date'," .
				" order_num='$order_num'," .
				" zip1='$zip1'," .
				" zip2='$zip2'," .
				" add1='$add1'," .
				" add2='$add2'," .
				" name='$name'," .
				" kojin_num=$kojin_num," .
				" update_date=now()," .
				" tel='$tel'" .
				" WHERE bill_num='$bill_num';";
		if (!$result = pg_query($conn_id,$sql)) { $ERROR[] = "基本データーを更新出来ませんでした。"; }
		else {
			$sql  = "UPDATE bill_list SET" .
					" state='1'" .
					" WHERE bill_num='$bill_num' AND state='0';";
			if (!$result = pg_query($conn_id,$sql)) { $ERROR[] = "詳細データーを削除出来ませんでした。"; }
		}
	}
	else {	//	新規追加
		$sql  = "INSERT INTO bill_base" .
				" (bill_date,site_type,order_num,zip1,zip2,add1,add2,name,kojin_num,regist_date,update_date,tel)" .
				" VALUES('$bill_date','$site_type','$order_num','$zip1','$zip2'," .
				"'$add1','$add2','$name',$kojin_num,now(),now(),'$tel');";
		if (!$result = pg_query($conn_id,$sql)) { $ERROR[] = "基本データーを保存出来ませんでした。"; }
		else {
			$sql =  "SELECT MAX(bill_num) AS max FROM bill_base;";
			if ($result = pg_query($conn_id,$sql)) {
				$list = pg_fetch_array($result);
				$bill_num = $list['max'];
				$_SESSION['USER']['bill_num'] = $bill_num;
			}
		}

		if (!$ERROR && $bill_num < 1) { $ERROR[] = "請求書番号が確認出来ません。"; }
	}

	if (!$ERROR && $GOODS_LIST && $bill_num) {
		foreach ($GOODS_LIST AS $KEY => $VAL) {
			$year = $GOODS_LIST[$KEY]['year'];
			$mon = $GOODS_LIST[$KEY]['mon'];
			$day = $GOODS_LIST[$KEY]['day'];
			$goods_name = $GOODS_LIST[$KEY]['goods_name'];
			$num = $GOODS_LIST[$KEY]['num'];
			$price = $GOODS_LIST[$KEY]['price'];

			$list_num = $KEY;
			if ($year && $mon && $day) {
				$list_date = "'" . $year . "-" . $mon . "-" . $day . "'";
			}
			else { $list_date = "NULL"; }

			if (!$goods_name) { continue; }
			if (!$num) { $num = "NULL"; } else { $num = "'" . $num . "'"; }
			if (!$price) { $price = "NULL"; } else { $price = "'" . $price . "'"; }

			$sql  = "INSERT INTO bill_list" .
					" (bill_num,list_num,list_date,goods_name,num,price,regist_date)" .
					" VALUES('$bill_num','$list_num',$list_date,'$goods_name',$num,$price,now());";
			if (!$result = pg_query($conn_id,$sql)) { $ERROR[] = "詳細データーを保存出来ませんでした。"; }
		}
	}

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		$mode = "update";
	}
	else {
		$mode = "print";
	}

	header ("Location: $PHP_SELF?mode=$mode\n\n");

	exit;

}



//	yahooメールメッセージ登録画面
function yahoo() {
global $PHP_SELF;

	if ($_SESSION['ERROR']) {
		$ERROR = $_SESSION['ERROR'];
		$html = ERROR($ERROR);
		unset($_SESSION['ERROR']);
	}

//	$yahoo_num = $_SESSION['yahoo_num'];
//	$yahoo = $_SESSION['yahoo'];

	$html .= <<<WAKABA
<br>
<h3>Yahoo請求書作成ページ</h3>
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="mode" value="yahoo_data">
Yahooの注文ページをコピー＆ペースとして頂き次へのボタンを押してください。<br>
<textarea rows="30" cols="100" name="yahoo">$yahoo</textarea><br>
<input type="submit" value="次へ">　<input type="reset">
</form>

WAKABA;

	return $html;

}



//	yahooメールデーター処理
//	新規注文ページをコピペして反映（帳票ではないよ）
function yahoo_data() {
global $PHP_SELF;
	global $conn_id; //	add ookawara 2010/07/31

	unset($_SESSION['USER']);

	$yahoo = $_POST['yahoo'];

	if (!$yahoo) { $ERROR[] = "メール情報が確認出来ません。"; }

	$_SESSION['yahoo'] = $yahoo;

	if (!$ERROR) {
##		$yahoo = ereg_replace("\r","",$yahoo);
		$yahoo = preg_replace("/\r/","",$yahoo);
		$yahoo = preg_replace("/注文番号 \n/", "注文番号 ", $yahoo);	//	add ookawara 2014/02/06

		$LIST = explode("\n",$yahoo);
		if ($LIST) {
			foreach ($LIST AS $VAL) {

#				$VAL = mb_convert_kana($VAL,"saKV","EUC-JP");
				$VAL = mb_convert_kana($VAL,"saKV","UTF-8");
				$VAL = trim($VAL);
##				$VAL = ereg_replace("[ ]{2,}","\t",$VAL);
				$VAL = preg_replace("/[ ]{2,}/","\t",$VAL);
##				$VAL = ereg_replace(" ","\t",$VAL);
				$VAL = preg_replace("/ /","\t",$VAL);

##				if (!$check && ereg("^ご注文日",$VAL)) { $check = 1; $i=1; }
				if (!$check && preg_match("/^ご注文日/",$VAL)) { $check = 1; $i=1; }
##				elseif ($check == 1 && ereg("メールアドレス",$VAL)) { $check = 2; $i=1; }
				elseif ($check == 1 && preg_match("/メールアドレス/",$VAL)) { $check = 2; $i=1; }
##				elseif ($check == 2 && ereg("^ご要望",$VAL)) { $check = 3; $i=1; }
				elseif ($check == 2 && preg_match("/^ご要望/",$VAL)) { $check = 3; $i=1; }
##				elseif ($check == 3 && ereg("入金情報",$VAL)) { $check = 4; $i=1; }
				elseif ($check == 3 && preg_match("/入金情報/",$VAL)) { $check = 4; $i=1; }
##				elseif ($check == 3 && ereg("^クレジットカード情報",$VAL)) { $check = 5; $i=1; }
				elseif ($check == 3 && preg_match("/^クレジットカード情報/",$VAL)) { $check = 5; $i=1; }
##				elseif ($check >= 4 && ereg("お届け先\t",$VAL)) { $check = 6; $i=1; }
				elseif ($check >= 4 && preg_match("/お届け先\t/",$VAL)) { $check = 6; $i=1; }
##				elseif ($check == 6 && ereg("お届け方法",$VAL)) { $check = 7; $i=1; }
				elseif ($check == 6 && preg_match("/お届け方法/",$VAL)) { $check = 7; $i=1; }
##				elseif ($check == 7 && ereg("^配送希望日",$VAL)) { $check = 8; $i=1; }
				elseif ($check == 7 && preg_match("/^配送希望日/",$VAL)) { $check = 8; $i=1; }
##				elseif ($check == 8 && ereg("^配送希望時間",$VAL)) { $check = 9; $i=1; }
				elseif ($check == 8 && preg_match("/^配送希望時間/",$VAL)) { $check = 9; $i=1; }
##				elseif ($check == 9 && ereg("^出荷日",$VAL)) { $check = 10; $i=1; }
 				elseif ($check == 9 && preg_match("/^出荷日/",$VAL)) { $check = 10; $i=1; }
##				elseif ($check == 10 && ereg("^着荷日",$VAL)) { $check = 11; $i=1; }
				elseif ($check == 10 && preg_match("/^着荷日/",$VAL)) { $check = 11; $i=1; }
##				elseif ($check == 11 && ereg("^お問い合わせ伝票番号",$VAL)) { $check = 12; $i=1; }
				elseif ($check == 11 && preg_match("/^お問い合わせ伝票番号/",$VAL)) { $check = 12; $i=1; }
##				elseif ($check == 12 && ereg("お届け時のご要望",$VAL)) { $check = 13; $i=1; }
				elseif ($check == 12 && preg_match("/お届け時のご要望/",$VAL)) { $check = 13; $i=1; }
##				elseif ($check == 13 && ereg("拡張フィールド",$VAL)) { $check = 14; $i=1; }
				elseif ($check == 13 && preg_match("/拡張フィールド/",$VAL)) { $check = 14; $i=1; }
##				elseif ($check == 14 && ereg("商品名/オプション",$VAL)) { $check = 15; $i=1; }
				elseif ($check == 14 && preg_match("/商品名\/オプション/",$VAL)) { $check = 15; $i=1; }
##				elseif ($check == 15 && ereg("^小計",$VAL)) { $check = 16; $i=1; }
				elseif ($check == 15 && preg_match("/^小計/",$VAL)) { $check = 16; $i=1; }
##				elseif ($check >= 16 && ereg("^手数料",$VAL)) { $check = 17; $i=1; }
				elseif ($check >= 16 && preg_match("/^手数料/",$VAL)) { $check = 17; $i=1; }
##				elseif ($check >= 16 && ereg("^送料",$VAL)) { $check = 18; $i=1; }
				elseif ($check >= 16 && preg_match("/^送料/",$VAL)) { $check = 18; $i=1; }
##				elseif ($check >= 16 && ereg("^ポイント利用分",$VAL)) { $check = 19; $i=1; }
				elseif ($check >= 16 && preg_match("/^ポイント利用分/",$VAL)) { $check = 19; $i=1; }

				//	add ookawara 2013/07/01
				if (preg_match("/^注文番号/", $VAL)) {
					$yahoo_num = preg_replace("/[^0-9]/", "", $VAL);
				}

				if ($check == 1) {
					if ($i == 1) {
						//list($d,$days,$d,$yahoo_num) = explode("\t",$VAL);	//	del ookawara 2013/07/01
						//list($year,$mon,$day) = explode("/",$days);			//	del ookawara 2013/07/01

						//	add ookawara 2013/07/01
						$VAL = preg_replace("/月曜日/", "げつようび", $VAL);
						$VAL = preg_replace("/年/", "年\t", $VAL);
						$VAL = preg_replace("/月/", "月\t", $VAL);
						$VAL = preg_replace("/\t\t/", "\t", $VAL);
						$DAYS = explode("\t",$VAL);
						$year = preg_replace("/[^0-9]/", "", $DAYS[1]);
						$mon = preg_replace("/[^0-9]/", "", $DAYS[2]);
						$day = preg_replace("/[^0-9]/", "", $DAYS[3]);

						//	add ookawara 2014/02/06
						if ($DAYS[2] == "注文番号") {
							list($year, $mon, $day) = explode("/",$VAL);
							$yahoo_num = $DAYS[3];
						}
					}
					$i++;
				} elseif ($check == 2) {
					if ($i == 1) {
						list($d,$email) = explode("\t",$VAL);
					}
					$i++;
				} elseif ($check == 3) {
					if ($i == 1) {
						list($d,$comment) = explode("\t",$VAL);
					} else {
						$comment .= $VAL;
					}
					$i++;
				} elseif ($check == 4) {
					$VAL = trim($VAL);	//	add ookawara 2013/07/02
					if (!$VAL) { continue; }	//	add ookawara 2013/07/02

					//if ($i == 5) {	//	del ookawara 2013/07/02
					if ($i == 3) {	//	add ookawara 2013/07/02
						$LINE = explode("\t",$VAL);
						if ($LINE) {
							foreach ($LINE AS $value) {
##								if (eregi("円",$value)) {
								if ( preg_match("/円/i",$value)) {
									$pay = $check_value;
									break;
								}
								$check_value = $value;
							}
						}
					}
					$i++;
				} elseif ($check == 5) {
					if ($i == 5) {
						list($pay) = explode("\t",$VAL);
					}
					$i++;
				} elseif ($check == 6) {
					if ( preg_match("/JP/",$VAL)) { $kyuu = 1; continue; }
					if ($i == 1 && $kyuu == 1) {
						list($zip,$add11,$add12,$add13,$add14,$add15) = explode("\t",$VAL);
						$add1 = trim("$add11$add12$add13$add14$add15");
					} elseif ($i == 1) {
						list($ti,$zip,$add11,$add12,$add13,$add14,$add15) = explode("\t",$VAL);
						$add1 = trim("$add11$add12$add13$add14$add15");
					} elseif ($i == 2) {
						list($add21,$add22,$add23,$add24,$add25) = explode("\t",$VAL);
						$add1 .= trim("$add21$add22$add23$add24$add25");
					} elseif ($i == 3) {
						$add2 = trim($VAL);
					} elseif ($i == 4) {
						$name_c = trim($VAL);
						list($name,$kana) = explode("(",$name_c);
					}
					$i++;
				} elseif ($check == 7) {
					if ($i == 1) {
						if (!$name) {
							$name_c = $add2;
							list($name,$kana) = explode("\(",$name_c);
							$add2 = "";
						}
##						$name = ereg_replace("\t"," ",$name);
						$name = preg_replace("/\t/"," ",$name);
						list($tel,$d,$haisou) = explode("\t",$VAL);
##						$tel = ereg_replace("電話番号:","",$tel);
						$tel = preg_replace("/電話番号:/","",$tel);
					}
					$i++;
				} elseif ($check == 8) {
					if ($i == 1) {
						list($d,$haisou_day) = explode("\t",$VAL);
					}
					$i++;
				} elseif ($check == 9) {
					if ($i == 1) {
						list($d,$haisou_time) = explode("\t",$VAL);
					}
					$i++;
				} elseif ($check == 10) {
					if ($i == 1) {
						list($d,$syuka_day) = explode("\t",$VAL);
					}
					$i++;
				} elseif ($check == 11) {
					if ($i == 1) {
						list($d,$chaku_day) = explode("\t",$VAL);
					}
					$i++;
				} elseif ($check == 12) {
					if ($i == 1) {
						list($d,$toiden) = explode("\t",$VAL);
						if ($kyuu != 1) { $check = 14; }
					}
					$i++;
				} elseif ($check == 13) {
					if ($i == 1) {
						list($d,$haisou_time_msg) = explode("\t",$VAL);
					} else {
						$haisou_time_msg .= $VAL;
					}
					$i++;
				} elseif ($check == 15) {
					if ($i > 1) {
##						if (ereg("^確定予定日:",$VAL)) {
						if (preg_match("/^確定予定日:/",$VAL)) {
							continue;
						}
						$amari = $i % 2;
						if ($amari == 0) {
							list($gl_num,$goods_name) = explode("\t",$VAL);
							$gl_num = trim($gl_num);
##							if (ereg("[^0-9]",$gl_num)) {
							if (preg_match("/[^0-9]/",$gl_num)) {
								$check = 16;
								continue;
							}
							$GOODS_LINE[$gl_num]['goods_name'] = $goods_name;
						} else {
							list($d,$d,$size,$code,$price,$kazu) = explode("\t",$VAL);
##							$price = ereg_replace("[^0-9]","",$price);
							$price = preg_replace("/[^0-9]/","",$price);
##							$kazu = ereg_replace("[^0-9]","",$kazu);
							$kazu = preg_replace("/[^0-9]/","",$kazu);
							$GOODS_LINE[$gl_num]['code'] = $code;
							$GOODS_LINE[$gl_num]['kazu'] = $kazu;
							$GOODS_LINE[$gl_num]['price'] = $price;
							$GOODS_LINE[$gl_num]['size'] = $size;
						}
					}
					$i++;
				} elseif ($check == 17) {
					if ($i == 1) {
						list($d,$tesu) = explode("\t",$VAL);
##						$tesu = ereg_replace("[^0-9]","",$tesu);
						$tesu = preg_replace("/[^0-9]/","",$tesu);
					}
					$i++;
				}
				elseif ($check == 18) {
					if ($i == 1) {
						list($d,$unchin) = explode("\t",$VAL);
##						$unchin = ereg_replace("[^0-9]","",$unchin);
						$unchin = preg_replace("/[^0-9]/","",$unchin);
					}
					$i++;
				}
				elseif ($check == 19) {
					if ($i == 1) {
						list($d,$point) = explode("\t",$VAL);
##						$point = ereg_replace("[^0-9]","",$point);
						$point = preg_replace("/[^0-9]/","",$point);
					}
					$i++;
				}

			}
		}
	}

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		$mode = "yahoo";
		header ("Location: $PHP_SELF?mode=$mode\n\n");
		exit;
	}

	$_SESSION['yahoo_num'] = $yahoo_num;
	$USER['order_num'] = $yahoo_num;

	$USER['year'] = date("Y");
	$USER['mon'] = date("m");
	$USER['day'] = date("d");
	$USER['site_type'] = 2;
#	$zip = mb_convert_kana($zip,"saKV","EUC-JP");
	$zip = mb_convert_kana($zip,"saKV","UTF-8");
##	if (ereg("-",$zip)) {
	if (preg_match("/-/",$zip)) {
		$USER['zip'] = $zip;
		list($zip1,$zip2) = explode("-",$zip);
	} else {
		$zip1 = substr($zip,0,3);
		$zip2 = substr($zip,-4);
		$USER['zip'] = $zip1."-".$zip2;
	}
	$USER['zip1'] = $zip1;
	$USER['zip2'] = $zip2;
	$USER['add1'] = $add1;
	$USER['add2'] = $add2;
	$USER['name'] = $name;
	$USER['time'] = time();
	$USER['goods_list'][1]['year'] = $year;
	$USER['goods_list'][1]['mon'] = $mon;
	$USER['goods_list'][1]['day'] = $day;
	$USER['tel'] = $tel;

	if ($GOODS_LINE) {
		$i=1;
		foreach ($GOODS_LINE AS $KEY => $VAL) {
			if (!$KEY) { continue; }
			if ($GOODS_LINE[$KEY]['kazu'] == 0) { continue; }

			if ($GOODS_LINE[$KEY]['goods_name'] && !$GOODS_LINE[$KEY]['code'] && !$GOODS_LINE[$KEY]['kazu'] && !$GOODS_LINE[$KEY]['price'] && !$GOODS_LINE[$KEY]['size']) { continue; }

			$goods_name = $GOODS_LIST[$KEY]['goods_name'];
			$num = $GOODS_LIST[$KEY]['num'];
			$price = $GOODS_LIST[$KEY]['price'];

			$KEY2 = $KEY + 1;
			if ($GOODS_LINE[$KEY2]['goods_name'] && !$GOODS_LINE[$KEY2]['code'] && !$GOODS_LINE[$KEY2]['kazu'] && !$GOODS_LINE[$KEY2]['price'] && !$GOODS_LINE[$KEY2]['size']) {
				$GOODS_LINE[$KEY]['goods_name'] .= " " . $GOODS_LINE[$KEY2]['goods_name'];
			}

			$USER['goods_list'][$i]['goods_name'] = $GOODS_LINE[$KEY]['goods_name'] . "/" . $GOODS_LINE[$KEY]['size'];
			$USER['goods_list'][$i]['num'] = $GOODS_LINE[$KEY]['kazu'];
			$USER['goods_list'][$i]['price'] = $GOODS_LINE[$KEY]['price'];
			$i++;

			$USER['goods_list'][$i]['goods_name'] = "(" . $GOODS_LINE[$KEY]['code'] . ")";

			//	add ookawara 2010/07/31
			$g_num = "";
			if ($GOODS_LINE[$KEY]['code']) {
				$sql  = "SELECT g_num FROM goods".
						" WHERE code='".$GOODS_LINE[$KEY]['code']."';";
				if ($result = pg_query($conn_id,$sql)) {
					$list = pg_fetch_array($result);
					$g_num = $list['g_num'];
				}
				if ($g_num > 0) {
					$USER['goods_list'][$i]['goods_name'] .= " [g" . $g_num . "]";
				}
			}


			$i++;
		}

		//	送料
		if ($unchin) {
			$USER['goods_list'][$i]['goods_name'] = "送料";
			$USER['goods_list'][$i]['price'] = $unchin;
			$USER['goods_list'][$i]['num'] = 1;
			$i++;
		}

		//	手数料
		if ($tesu) {
			$USER['goods_list'][$i]['goods_name'] = "代引手数料";
			$USER['goods_list'][$i]['price'] = $tesu;
			$USER['goods_list'][$i]['num'] = 1;
			$i++;
		}

		//	ポイント利用
		if ($point) {
			$USER['goods_list'][$i]['goods_name'] = "ポイント利用分({$point}pt)";
			$USER['goods_list'][$i]['price'] = -$point;
			$USER['goods_list'][$i]['num'] = 1;
			$i++;
		}

		//	支払い方法
		$USER['goods_list'][$i]['goods_name'] = "支払方法：{$pay}";
		$i++;

		//	お届け方法
		$USER['goods_list'][$i]['goods_name'] = "お届け方法：{$haisou}";
		$i++;

		//	配送希望日
		if ($haisou_day) {
			$USER['goods_list'][$i]['goods_name'] = "配送希望日：{$haisou_day}";
			$i++;
		}

		//	配送希望時間
		if ($haisou_time) {
			$USER['goods_list'][$i]['goods_name'] = "配送希望時間：{$haisou_time}";
			$i++;
		}

		//	出荷日
		if ($syuka_day) {
			$USER['goods_list'][$i]['goods_name'] = "出荷日：{$syuka_day}";
			$i++;
		}

		//	着荷日
		if ($chaku_day) {
			$USER['goods_list'][$i]['goods_name'] = "着荷日：{$chaku_day}";
			$i++;
		}

		//	お問い合わせ伝票番号
		if ($toiden) {
			$USER['goods_list'][$i]['goods_name'] = "お問い合わせ伝票番号：{$toiden}";
			$i++;
		}

		//	お届け時のご要望
		if ($haisou_time_msg) {
			$haisou_time_msg = trim($haisou_time_msg);
			$USER['goods_list'][$i]['goods_name'] = "お届け時のご要望：{$haisou_time_msg}";
			$i++;
		}

		//	コメント
		if ($comment) {
			$comment = trim($comment);
			$USER['goods_list'][$i]['goods_name'] = "コメント：{$comment}";
			$i++;
		}
	}

	return $USER;

}



//	楽天メールメッセージ登録画面
function rakuten() {
global $PHP_SELF;

	if ($_SESSION['ERROR']) {
		$ERROR = $_SESSION['ERROR'];
		$html = ERROR($ERROR);
		unset($_SESSION['ERROR']);
	}

//	$rakuten = $_SESSION['rakuten'];

	$html .= <<<WAKABA
<br>
<h3>楽天請求書作成ページ</h3>
<form action="$PHP_SELF" method="POST">
<input type="hidden" name="mode" value="rakuten_data">
楽天の注文メールをコピー＆ペースとして頂き次へのボタンを押してください。<br>
※【楽天】注文内容ご確認(携帯)のデーターは利用できません。<br>
<textarea rows="15" cols="100" name="rakuten">$rakuten</textarea><br>
<input type="submit" value="次へ">　<input type="reset">
</form>

WAKABA;

	return $html;

}



//	楽天メールデーター処理
function rakuten_data() {
global $PHP_SELF;
	global $conn_id; //	add ookawara 2010/07/31
	global $TAX_;	//	add ookawara 2015/02/09

	unset($_SESSION['USER']);

	$rakuten = $_POST['rakuten'];

	if (!$rakuten) { $ERROR[] = "メール情報が確認出来ません。"; }

//	$_SESSION['rakuten'] = $rakuten;

	if (!$ERROR) {
##		$rakuten = ereg_replace("\r","",$rakuten);
		$rakuten = preg_replace("/\r/","",$rakuten);
		$LIST = explode("\n",$rakuten);
		if ($LIST) {
			foreach ($LIST AS $VAL) {
$aa++;
#				$VAL = mb_convert_kana($VAL,"saKV","EUC-JP");
				$VAL = mb_convert_kana($VAL,"saKV","UTF-8");
				$VAL = trim($VAL);
##				$VAL = ereg_replace("[ ]{2,}","\t",$VAL);
				$VAL = preg_replace("/[ ]{2,}/","\t",$VAL);
##				$VAL = ereg_replace(" ","\t",$VAL);
				$VAL = preg_replace("/ /","\t",$VAL);
//echo("$aa<>$check<>$VAL<br>\n");
##				if (!$check && ereg("^\[受注番号\]",$VAL)) { $check = 1; $i=1; }
				if (!$check && preg_match("/^\[受注番号\]/",$VAL)) { $check = 1; $i=1; }
##				elseif ($check == 1 && ereg("^\[日時\]",$VAL)) { $check = 2; $i=1; }
				elseif ($check == 1 && preg_match("/^\[日時\]/",$VAL)) { $check = 2; $i=1; }
##				elseif ($check == 2 && ereg("^\[注文者\]",$VAL)) { $check = 3; $i=1; }
				elseif ($check == 2 && preg_match("/^\[注文者\]/",$VAL)) { $check = 3; $i=1; }
##				elseif ($check == 3 && ereg("^〒",$VAL)) { $check = 4; $i=1; }
				elseif ($check == 3 && preg_match("/^〒/",$VAL)) { $check = 4; $i=1; }
##				elseif ($check == 4 && ereg("^\(TEL\)",$VAL)) { $check = 5; $i=1; }
				elseif ($check == 4 && preg_match("/^\(TEL\)/",$VAL)) { $check = 5; $i=1; }
##				elseif ($check == 5 && ereg("^\[支払方法\]",$VAL)) { $check = 6; $i=1; }
				elseif ($check == 5 && preg_match("/^\[支払方法\]/",$VAL)) { $check = 6; $i=1; }
##				elseif ($check == 6 && ereg("^\[ポイント利用方法]",$VAL)) { $check = 7; $i=1; }
				elseif ($check == 6 && preg_match("/^\[ポイント利用方法]/",$VAL)) { $check = 7; $i=1; }
##				elseif ($check == 7 && ereg("^\[配送方法\]",$VAL)) { $check = 8; $i=1; }
				elseif ($check == 7 && preg_match("/^\[配送方法\]/",$VAL)) { $check = 8; $i=1; }
##				elseif ($check == 8 && ereg("^\[配送日時指定\]",$VAL)) { $check = 9; $i=1; }
				elseif ($check == 8 && preg_match("/^\[配送日時指定\]/",$VAL)) { $check = 9; $i=1; }
##				elseif ($check >= 8 && ereg("^\[備考\]",$VAL)) { $check = 10; $i=1; }
				elseif ($check >= 8 && preg_match("/^\[備考\]/",$VAL)) { $check = 10; $i=1; }
##				elseif ($check == 10 && ereg("^\[ショップ名\]",$VAL)) { $check = 11; $i=1; }
				elseif ($check == 10 && preg_match("/^\[ショップ名\]/",$VAL)) { $check = 11; $i=1; }
##				elseif ($check == 11 && ereg("^\[送付先\]",$VAL)) { $check = 12; $i=1; }
				elseif ($check == 11 && preg_match("/^\[送付先\]/",$VAL)) { $check = 12; $i=1; }
##				elseif ($check == 12 && ereg("^〒",$VAL)) { $check = 13; $i=1; }
				elseif ($check == 12 && preg_match("/^〒/",$VAL)) { $check = 13; $i=1; }
##				elseif ($check == 13 && ereg("^\(TEL\)",$VAL)) { $check = 14; $i=1; }
				elseif ($check == 13 && preg_match("/^\(TEL\)/",$VAL)) { $check = 14; $i=1; }
##				elseif ($check == 14 && ereg("^\[商品\]",$VAL)) { $check = 15; $i=1; }
				elseif ($check == 14 && preg_match("/^\[商品\]/",$VAL)) { $check = 15; $i=1; }
##				elseif ($check == 15 && ereg("^\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*",$VAL)) { $check = 16; $i=1; }
				elseif ($check == 15 && preg_match("/^\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*/",$VAL)) { $check = 16; $i=1; }
##				elseif ($check == 16 && ereg("^小計",$VAL)) { $check = 17; $i=1; }
				elseif ($check == 16 && preg_match("/^小計/",$VAL)) { $check = 17; $i=1; }
##				elseif ($check == 17 && ereg("^消費税",$VAL)) { $check = 18; $i=1; }
				elseif ($check == 17 && preg_match("/^消費税/",$VAL)) { $check = 18; $i=1; }
##				elseif ($check >= 17 && ereg("^送料",$VAL)) { $check = 19; $i=1; }
				elseif ($check >= 17 && preg_match("/^送料/",$VAL)) { $check = 19; $i=1; }
##				elseif ($check >= 19 && ereg("^代引料",$VAL)) { $check = 20; $i=1; }
				elseif ($check >= 19 && preg_match("/^代引料/",$VAL)) { $check = 20; $i=1; }
##				elseif ($check >= 19 && ereg("^ポイント利用",$VAL)) { $check = 21; $i=1; }
				elseif ($check >= 19 && preg_match("/^ポイント利用/",$VAL)) { $check = 21; $i=1; }

				if ($check == 1) {
					list($d,$order_num) = explode("\t",$VAL);
					$order_num = trim($order_num);
				}
				elseif ($check == 2) {
					list($d,$date,$d) = explode("\t",$VAL);
					list($year,$mon,$day) = explode("-",$date);
				}
				elseif ($check == 3) {
					list($d,$name_) = explode("\t",$VAL);
				}
				elseif ($check == 4) {
					if ($VAL == "") { continue; }
					list($zip_,$add1_,$add2_) = explode("\t",$VAL);
##					$zip_ = ereg_replace("〒","",$zip_);
					$zip_ = preg_replace("/〒/","",$zip_);
					$add2_ = trim($add2_);
				}
				elseif ($check == 5) {
					list($d,$tel_) = explode("\t",$VAL);
				}
				elseif ($check == 6) {
					list($d,$pay1,$pay2) = explode("\t",$VAL);
					$pay = trim("$pay1 $pay2");
				}
				elseif ($check == 7) {
					list($d,$point_check) = explode("\t",$VAL);
				}
				elseif ($check == 8) {
					list($d,$haisou) = explode("\t",$VAL);
				}
				elseif ($check == 9) {
					if ($i > 1) {
						$VAL = trim($VAL);
						if ($VAL) { $haisou_date .= "$VAL "; }
					}
					$i++;
				}
				elseif ($check == 10) {
					if ($i > 1) {
##						$VAL = ereg_replace("備考欄","",$VAL);
						$VAL = preg_replace("/備考欄/","",$VAL);
						$VAL = trim($VAL);
						if ($VAL) { $bikou .= "$VAL "; }
					}
					$i++;
				}
				elseif ($check == 12) {
					//list($d,$name) = explode("\t",$VAL);		//	del ookawara 2013/10/18
					$LINE_LIST = explode("\t",$VAL);			//	add ookawara 2013/10/18
					$name = $LINE_LIST[1]." ".$LINE_LIST[2];	//	add ookawara 2013/10/18
				}
				elseif ($check == 13) {
					if ($VAL == "") { continue; }
					list($zip,$add1,$add2) = explode("\t",$VAL);
##					$zip = ereg_replace("〒","",$zip);
					$zip = preg_replace("/〒/","",$zip);
					$add2 = trim($add2);
				}
				elseif ($check == 14) {
					list($d,$tel) = explode("\t",$VAL);
				}
				elseif ($check == 15) {
					if ($i > 1) {
##						if (!ereg("サイズ",$VAL) && !ereg("価格",$VAL) && !ereg("獲得ポイント",$VAL) && !ereg("-----",$VAL)) {
						if (!preg_match("/サイズ/",$VAL) && !preg_match("/価格/",$VAL) && !preg_match("/獲得ポイント/",$VAL) && !preg_match("/-----/",$VAL)) {
							list($goods_name,$code) = explode("(",$VAL);
							$code = trim($code);
##							$code = ereg_replace("\)","",$code);
							$code = preg_replace("/\)/","",$code);
							$goods_name = preg_replace("/【(.*)】/","",$goods_name);	//	add ookawara 2010/07/31
							$GOODS_LINE[$g]['goods_name'] = $goods_name;
							$GOODS_LINE[$g]['code'] = $code;
						}
##						elseif (ereg("サイズ",$VAL)) {
						elseif (preg_match("/サイズ/",$VAL)) {
##							$size = ereg_replace("^サイズ:","",$VAL);
							$size = preg_replace("/^サイズ:/","",$VAL);
//							list($d,$size) = explode(":",$VAL);
							$size = trim($size);
							$GOODS_LINE[$g]['goods_name'] .= "/$size";
						}
##						elseif (ereg("価格",$VAL)) {
						elseif (preg_match("/価格/",$VAL)) {
							list($d,$price,$d,$num) = explode("\t",$VAL);
##							$price = ereg_replace("[^0-9]","",$price);
							$price = preg_replace("/[^0-9]/","",$price);
##							$num = ereg_replace("[^0-9]","",$num);
							$num = preg_replace("/[^0-9]/","",$num);

							//	add ookawara 2015/02/09
							if (preg_match("/税別/", $VAL)) {
								$price = floor($price * (1+$TAX_));
							}

							$GOODS_LINE[$g]['price'] = $price;
							$GOODS_LINE[$g]['num'] = $num;
						}
##						elseif (ereg("獲得ポイント",$VAL)) {
						elseif (preg_match("/獲得ポイント/",$VAL)) {
##							$ggpoint = ereg_replace("^獲得ポイント","",$VAL);
							$ggpoint = preg_replace("/^獲得ポイント/","",$VAL);
							$ggpoint = trim($ggpoint);
							$GOODS_LINE[$g]['ggpoint'] = $ggpoint;
						}
##						elseif (ereg("-----",$VAL)) {
						elseif (preg_match("/-----/",$VAL)) {
							$g++;
						}
					}
					else { $g = 1; }
					$i++;
				}
				elseif ($check == 18) {
					list($d,$zeikin) = explode("\t",$VAL);
##					$zeikin = ereg_replace("[^0-9]","",$zeikin);
					$zeikin = preg_replace("/[^0-9]/","",$zeikin);
					$check = 99;
				}
				elseif ($check == 19) {
					list($d,$unchin) = explode("\t",$VAL);
##					$unchin = ereg_replace("[^0-9]","",$unchin);
					$unchin = preg_replace("/[^0-9]/","",$unchin);
					$check = 99;
				}
				elseif ($check == 20) {
					list($d,$tesu) = explode("\t",$VAL);
##					$tesu = ereg_replace("[^0-9]","",$tesu);
					$tesu = preg_replace("/[^0-9]/","",$tesu);
					$check = 99;
				}
				elseif ($check == 21) {
					list($d,$point) = explode("\t",$VAL);
##					$point = ereg_replace("[^0-9]","",$point);
					$point = preg_replace("/[^0-9]/","",$point);
					$check = 99;
				}
			}
		}
	}

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		$mode = "rakuten";
		header ("Location: $PHP_SELF?mode=$mode\n\n");
		exit;
	}

	$USER['order_num'] = $order_num;

	$USER['year'] = date("Y");
	$USER['mon'] = date("m");
	$USER['day'] = date("d");
	$USER['site_type'] = 3;
	$USER['zip'] = $zip;
	list($zip1,$zip2) = explode("-",$zip);
	$USER['zip1'] = $zip1;
	$USER['zip2'] = $zip2;
	$USER['add1'] = $add1;
	$USER['add2'] = $add2;
	$USER['name'] = $name;
	$USER['time'] = time();
	$USER['goods_list'][1]['year'] = $year;
	$USER['goods_list'][1]['mon'] = $mon;
	$USER['goods_list'][1]['day'] = $day;
	$USER['tel'] = $tel;

	$goukei_point = 0;
	if ($GOODS_LINE) {
		$i=1;
		foreach ($GOODS_LINE AS $KEY => $VAL) {
			$goods_name = $GOODS_LINE[$KEY]['goods_name'];
			$num = $GOODS_LINE[$KEY]['num'];
			$price = $GOODS_LINE[$KEY]['price'];
			$code = $GOODS_LINE[$KEY]['code'];
			$ggpoint = $GOODS_LINE[$KEY]['ggpoint'];

			//	add ookawara 2010/07/31
			$g_num = "";
			if ($code) {
				$sql  = "SELECT g_num FROM goods".
						" WHERE code='".$code."';";
				if ($result = pg_query($conn_id,$sql)) {
					$list = pg_fetch_array($result);
					$g_num = $list['g_num'];
				}
			}

			$USER['goods_list'][$i]['goods_name'] = $goods_name;
			$USER['goods_list'][$i]['num'] = $num;
			$USER['goods_list'][$i]['price'] = $price;
			$i++;

			$USER['goods_list'][$i]['goods_name'] = "(" . $code . ")";
			//	add ookawara 2010/07/31
			if ($g_num > 0) {
				$USER['goods_list'][$i]['goods_name'] .= " [g" . $g_num . "]";
			}

			if ($ggpoint) {
				$goukei_point += $ggpoint;
//				$USER['goods_list'][$i]['goods_name'] .= " 獲得ポイント" . $ggpoint;
			}
			$i++;
		}

		//	消費税
		if ($zeikin) {
			$USER['goods_list'][$i]['goods_name'] = "消費税";
			$USER['goods_list'][$i]['price'] = $zeikin;
			$USER['goods_list'][$i]['num'] = 1;
			$i++;
		}

		//	送料
		if ($unchin) {
			$USER['goods_list'][$i]['goods_name'] = "送料";
			$USER['goods_list'][$i]['price'] = $unchin;
			$USER['goods_list'][$i]['num'] = 1;
			$i++;
		}

		//	手数料
		if ($tesu) {
			$USER['goods_list'][$i]['goods_name'] = "代引料";
			$USER['goods_list'][$i]['price'] = $tesu;
			$USER['goods_list'][$i]['num'] = 1;
			$i++;
		}

		//	ポイント利用
		if ($point) {
			$USER['goods_list'][$i]['goods_name'] = "ポイント利用分({$point}pt)";
			$USER['goods_list'][$i]['price'] = -$point;
			$USER['goods_list'][$i]['num'] = 1;
			$i++;
		}

		//	支払い方法
		if ($pay) {
			$USER['goods_list'][$i]['goods_name'] = "支払方法：{$pay}";
			$i++;
		}

		//	お届け方法
		if ($haisou) {
			$USER['goods_list'][$i]['goods_name'] = "配送方法：{$haisou}";
			$i++;
		}

		//	配送日時指定
		if ($haisou_date) {
			$haisou_date = trim($haisou_date);
			$USER['goods_list'][$i]['goods_name'] = "配送日時指定：{$haisou_date}";
			$i++;
		}

		//	備考
		if ($bikou) {
			$bikou = trim($bikou);
			$USER['goods_list'][$i]['goods_name'] = "配送日時指定：{$bikou}";
			$i++;
		}
	}

	return $USER;

}



//	管理画面用ヘッダ
function headers() {

	$html = <<<WAKABA
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<TITLE>請求書作成</TITLE>
<style type="text/css">
<!--
BODY{
  font-size : 12px;
  margin-top : 0px;
  margin-left : 0px;
  margin-right : 0px;
  margin-bottom : 0px;
}
H1{
  color : #000000;
  background-color : #cccccc;
  font-size : 24px;
}
TH{
  color : #000000;
}
.user_data_waku {
  margin-left : 10px;
}
.user_data {
  background-color : #efefef;
  font-size : 16px;
}
.th_line {
  border-right-width : 1px;
  border-right-style : solid;
  border-right-color : #ffffff;
}
.hiduke {
  font-size : 18px;
  text-decoration : underline;
}
.aisatsu {
  font-size : 16px;
}
.ordermsg {
  font-size : 20px;
  text-decoration : underline;
  font-weight : bold;
}
.allprice {
  font-size : 20px;
  text-decoration : underline;
  font-weight : bold;
}
.footer {
  font-size : 16px;
}
.footername {
  font-size : 22px;
  font-weight : bold;
}
-->
</style>
</HEAD>
<BODY>

WAKABA;

	return $html;

}



//	管理画面用フッタ
function footer() {

	$html = <<<WAKABA
</BODY>
</HTML>

WAKABA;

	return $html;

}



//	エラー処理
function ERROR($ERROR) {

	foreach ($ERROR AS $val) {
		if ($val != "") { $error .= "・$val<BR>\n"; }
	}

	$errors = <<<WAKABA
<BR>
<FONT color="#FF0000"><B>エラー</B></FONT><BR>
$error
WAKABA;

	return $errors;

}


function pre($val) {

	echo("<pre>\n");
	print_r($val);
	echo("</pre>\n");

}
?>
