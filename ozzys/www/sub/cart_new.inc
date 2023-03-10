<?PHP


//add okabe start 2016/07/15
//	ポスデーターテーブル
define("TABLE_GOODS", "goods");
//add okabe end 2016/07/15

// add simon 2018-11-12
// 新しいsave/send仕組みファイルをインクルードする
require dirname(__FILE__).'/order_mail.class.inc';
require dirname(__FILE__).'/webcollect.class.inc';
require dirname(__FILE__).'/cart_save_send.inc';

//	買い物かご
function cart() {

	$idpass = $_SESSION['idpass'];

	$referer = $_SERVER['HTTP_REFERER'];
##	if ($referer && !eregi(".php",$referer)) {
	if ($referer && !preg_match("/\.php/i",$referer)) {
		$_SESSION['b_url'] = $referer;
	}

	$check = 0;
	if ($_POST['mode']) {
		$mode = $_POST['mode'];
		$check = 1;
	}
	elseif ($_GET['mode']) {
		$mode = $_GET['mode'];
	}

	if ($mode) {
		if ($mode == "member" && $idpass) { $mode = "enter"; }		//	ログインチェック

		if ($check == 1) {
			if ($mode == "mcheck") { mcheck(); }					//	ログインチェック
			elseif ($mode == "listcheck") { listcheck($idpass); }	//	住所入力チェック
			elseif ($mode == "send") {
				// 注文保存
				$pay_type = order_save($idpass);

				// 注文記録メール送信
				order_send($pay_type);
				exit;
			}
		}
		else {
			if ($mode == "member") {		//	会員ログイン画面
				$html = member();
				$title = "ご注文";
			}
			elseif ($mode == "enter") {		//	住所入力画面
				$html = enter($idpass);
				$title = "ご注文";
			}
			elseif ($mode == "check") {		//	確認画面
				$html = check($idpass);
				$title = "ご注文確認";
			}
		}
	}
	else {									//	買い物かご
		$html = first($idpass);
		$title = "買い物かご";
	}

	$html = <<<WAKABA
	  <h2 class="sub-title-prod">$title</h2>

$html

WAKABA;

	return array($html,$title);

}


//	買い物かご
function first($idpass) {
	global $PHP_SELF,$db,$gpx,$gpy,$tax,$h_tax,$URL;

	unset($_SESSION['customer_check']);

	$action = $_POST['action'];

	if ($_POST['list_num']) { $list_num = $_POST['list_num']; }
	if ($_POST['buy_num']) { $buy_num = $_POST['buy_num']; }

	//	送料無料設定定数変数設定
	//	add ookawara 2016/07/25
	$souryoufree = 0;
	if (defined("SOURYOUFREE")) {
		$souryoufree = SOURYOUFREE;
	}
	$souryoufreeprice = 0;
	if (defined("SOURYOUFREEPRICE")) {
		$souryoufreeprice = SOURYOUFREEPRICE;
	}

	$flag = 0;
	$CART = array();
	if ($_SESSION['customer']) { $CART = $_SESSION['customer']; }
	if ($action == "del" && $list_num) {
		unset($CART[$list_num]);
		$_SESSION['customer'] = $CART;
		$flag = 1;
	} elseif ($list_num && $buy_num) {
		$stock = "";
		$sql = "SELECT a.stock from goods a, list d WHERE d.list_num='$list_num' AND a.pluid=d.pluid LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$stock = $list['stock'];
		}

		$buy_num_ = $buy_num + $CART[$list_num];
		if ($stock < $buy_num_) {
			//$ERROR = "申し訳御座いません、在庫数が \"$stock\" になってしまいました。お手数ですが商品数を選択しなおして下さい。";	//	del ookawara 2010/02/01
			$ERROR = "申し訳御座いません、在庫数が全部で \"$stock\" となります。<br>\nお手数ですが商品数を選択しなおして下さい。";	//	add ookawara 2010/02/01
			if ($CART[$list_num]) {
				$CART[$list_num] = $stock;
				$_SESSION['customer'] = $CART;
			}
		} else {
			$CART[$list_num] += $buy_num;
			$_SESSION['customer'] = $CART;
			$flag = 1;
			//$_SESSION['b_url'] = $_SERVER['HTTP_REFERER'];
		}
	} elseif ($list_num && !$buy_num) {
		$ERROR = "購入数が選択されておりません。";
	}

	if (!$CART) { $ERROR = "現在商品は選択されておりません。"; }

	if ($_SESSION['ERROR']) {
		$sys_error = ERROR($_SESSION['ERROR'])."<br>\n";
		unset($_SESSION['ERROR']);
	}

	if ($ERROR) {
		$html = <<<WAKABA
       		<div class="alert alert-danger">$ERROR</div>
			<BR>

WAKABA;

		if ($_SESSION['b_url']) {
			$b_url = $_SESSION['b_url'];
			$html .= <<<WAKABA
      <FORM action="$b_url" method="GET"><INPUT type="submit" value="戻る"></FORM><BR>
      <BR>

WAKABA;

		} else {
			$html .= <<<WAKABA
      <FROM><INPUT onclick="history.back()" type="button" value="戻る"></FORM><BR>

WAKABA;
		}
	} elseif ($flag == 1) {
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if (!$ERROR) {
		$html = $sys_error;


		$price_all = 0;
        $cart_items = array();      // add simon 2018-07-05
		$ZAIKO = array();							//	add ohkawara 2020/03/31
		foreach ($CART AS $list_num => $buy_num) {
			$sql  = "SELECT d.list_num, d.goods_name, b.maker_name, d.size, d.color, d.picture, a.price, a.stock" .
					", d.free_postage".					//	add ookawara 2016/07/25
                    ", a.set_flag, a.class_m".          //  add simon 2018-07-05
					", a.pluid".						//	add ohkawara 2020/03/31
					" FROM goods a, maker b, list d " .
					" WHERE d.list_num='$list_num' AND a.pluid=d.pluid AND b.maker_num=d.maker_num LIMIT 1;";
			if ($result = pg_query($db,$sql)) {
				$list = pg_fetch_array($result);
                $cart_items[] = $list;   // add simon 2018-07-05
				$list_num = $list['list_num'];
				$goods_name = $list['goods_name'];
				$maker_name = $list['maker_name'];
				$size = $list['size'];
				$color = $list['color'];
				$picture = $list['picture'];
				$price = $list['price'];
				$stock = $list['stock'];
				//add okabe start 2016/07/15
				$postage_fee = "送料";
				$postareg_align = "center";
				$free_postage = $list['free_postage'];
				//add okabe end 2016/07/15
				$pluid = $list['pluid'];				//	add ohkawara 2020/03/31
			}

			$stock -= $ZAIKO[$pluid];					//	add ohkawara 2020/03/31

			//if ($buy_num > $stock) {					//	del ohkawara 2020/03/31
			if ($stock > 0 && $buy_num > $stock) {		//	add ohkawara 2020/03/31
				$buy_num = $stock;
				$CART[$list_num] = $buy_num;
				$COM .= "$goods_name($list_num)は在庫数が減ったため $buy_num までしか購入できません。<BR>\n";
			}
			if ($stock <= 0) {
				$buy_n = 0;
				$COM .= "$goods_name($list_num)は在庫が無くなってしまったため購入できません。<BR>\n";
				unset($CART[$list_num]);
				continue;
			}

			$ZAIKO[$pluid] += $buy_num;					//	add ohkawara 2020/03/31

			$check_price = 0;	//	add ookawara 2016/07/28
			if ($h_tax == 1) {
				$price_h = $price + floor(($price * $tax) + 0.5);
			} else {
				$price_h = $price;
			}
			$check_price = $price_h;	//	add ookawara 2016/07/28

			$price_s = $price_h * $buy_num;
			$price_sh = $price_h * $buy_num;
			$price_all += $price_s;
			$price_h = number_format($price_h) . "円";
			$price_sh = number_format($price_sh) . "円";
			if (!$size) { $size = "--"; }
			if (!$color) { $color = "--"; }
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
				} else {
					$width_ = $wid;
					$height_ = $hig;
				}
				$img_msg = "<IMG src=\"$pic_url\" width=\"$width_\" height=\"$height_\" border=\"0\" alt=\"$goods_name\">\n";
			} else {
				$img_msg = "";
			}

			// add okabe start 2016/07/15
			$img_free_postage = "";
			if ($souryoufree > 0 && ($free_postage == 1 || ($souryoufreeprice > 0 && $souryoufreeprice <= $check_price))) {
				$img_free_postage = "<div class=\"blinking\" style=\"color: red; font-weight: bold;\">基本送料無料</div>\n";	//	add	ohkawara	2020/04/22	"基本"を追加
				//$postage_fee = "<span style=\"color:red; font-weight:bold;\">送料無料</span>";								//	del ohkawara 2020/04/23	項目名なので無料表示する必要は無し
			}
			// add okabe end 2016/07/15

			//	add ohkawara 2020/04/23
			$rod_fee_block = "";
            if(Souryou::prod_has_rod_fee($list)) {
                $fee = Souryou::cart_rod_shipping_fee(array($list));
                $rod_fee_block = "<br><div>追加送料<br>".number_format($fee)."円</div>";
            }

			//	add $rod_fee_block ohkawara 2020/04/23

			$html .= <<<WAKABA
		<div class="cart-line cart-line-resp clearfix">
			<div class="cart-cell cart-name">
				$goods_name (No.$list_num)
			</div>
			<div class="cart-group cart-group-1 text-center">
				<div class="cart-cell">
					{$img_msg}{$img_free_postage}{$rod_fee_block}
				</div>
			</div>

			<div class="cart-group cart-group-2">
				<table class="table-cart">
					<tr>
						<th>メーカー</th>
						<td>$maker_name</td>
					</tr>
					<tr>
						<th>サイズ</th>
						<td>$size</td>
					</tr>
					<tr>
						<th>カラー</th>
						<td>$color</td>
					</tr>
					<tr>
						<th>値段</th>
						<td>$price_h</td>
					</tr>
					<tr>
						<th>数量</th>
						<td>$buy_num</td>
					</tr>
					<tr>
						<th>小計</th>
						<td class="cart-sub-total">$price_sh</td>
					</tr>
				</table>
			</div>

			<div class="cart-group cart-group-3">
				<div class="cart-cell">
					<FORM action="$PHP_SELF" method="POST">
						<INPUT type="hidden" name="action" value="del">
						<INPUT type="hidden" name="list_num" value="$list_num">
						<button class="btn btn-details" type="subimt">削除</button>
					</FORM>
				</div>
			</div>
		</div>

WAKABA;
		} // end foreach loop

		$html .= <<<WAKABA
      <TABLE class="table-cart-check">
WAKABA;

		//	消費税表示
		//if ($h_tax != 1 || true) { // TEST
		if ($h_tax != 1) {
			$tax_num = $tax * 100;
			$syouhi = 0;
			$syouhi = $price_all * $tax;
			$syouhi = ceil($syouhi);
			$price_all = $price_all + $syouhi;
			$syouhi = number_format($syouhi) . "円";

			$html .= <<<WAKABA
		  <tr>
			<td class="cart-tax">消費税($tax_num%)</td>
			<td align="right">$syouhi</td>
		  </tr>

WAKABA;
		}

		// add okabe start 2016/07/11
		//送料無料商品が含まれているかチェック
        $price_checkx = Souryou::cart_amount_until_free_shipping($price_all);

		$free_postage_msg1 = "";								//	add ookawara 2016/07/25
		$free_postage_msg2 = "未確定";							//	add ookawara 2016/07/25
		$postage_fee = "送料";										//	add ookawara 2016/07/25
		$postareg_align = "right";									//	add ookawara 2016/07/25
        $is_free_postage = false;
		if (Souryou::can_be_free()) {
			if (($price_checkx !== null && $price_checkx <= 0) || Souryou::cart_has_free_shipping_items($cart_items)) {	//所定金額以上か、送料無料商品が有る場合	//	add ookawara 2016/07/29
				//$postage_fee = "<span style=\"color:red; font-weight:bold;\">基本送料無料</span>";																//	add	ohkawara	2020/04/22	"基本"を追加	//	del ohkawara 2020/04/23	項目名なので無料表示する必要は無し
				$free_postage_msg2 = "0円";
				$postareg_align = "right";
                $is_free_postage = true;
            } elseif ($price_checkx !== null && $price_checkx > 0) {	//	add ookawara 2016/07/29
				$free_postage_msg1 = "<span style=\"color:red; font-weight:bold;\">あと ".number_format($price_checkx)."円で基本送料が無料になります。</span>";	//	add	ohkawara	2020/04/22	"送料無料"を"基本送料が無料"に変更
			}
		}

        // 竿の場合は、送料追加
        if(Souryou::cart_has_rods_with_fee($cart_items)) {
            $free_postage_msg2 = "基本送料<br>+".number_format(Souryou::cart_rod_shipping_fee($cart_items))."円";
        }
		// test add okabe end 2016/07/11

		$price_all = number_format($price_all) . "円";

		//	add ※追加 ohkawara 2020/04/23
		$html .= <<<WAKABA
		  <tr>
			<td class="cart-post" align="${postareg_align}">{$free_postage_msg1}<B>${postage_fee}</B></td>
			<td align="right">${free_postage_msg2} </td>
		  </tr>

		  <tr>
			<td class="cart-total-title">商品合計金額</td>
			<td class="cart-total-value" align="right">${price_all}</td>
		  </tr>

		</table>
※追加送料の商品が複数有る場合、追加送料が一番高い金額が追加送料として適応されます。<br>
WAKABA;

	}

	if ($COM) {
		$html = <<<WAKABA
      <TABLE border="0" width="95%">
        <TBODY>
          <TR align="center">
            <TD>
            <BR>
            <TABLE border="0">
              <TBODY>
                <TR>
                  <TD>申し訳御座いません。<BR>
                  $COM
                  </TD>
                </TR>
              </TBODY>
            </TABLE>
            <BR>
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>
$html

WAKABA;
	}

	$_SESSION['customer'] = $CART;

	if (!$ERROR) {
		$html .= <<<WAKABA
      <BR>
	  <center>
      <FONT color="#333333">＝<B>以上でよろしければ、下記のボタンをクリックしてください</B>＝</FONT><BR>
      <TABLE cellspacing="6" cellpadding="0" border="0">
        <TBODY>
          <TR>
            <TD><FONT color="#ffcc00">●</FONT><FONT color="#333333">会員の方はこちら　→</FONT></TD>
            <TD>
            <A href="$PHP_SELF?mode=member"><IMG src="/images/next_k.gif" width="79" height="22" alt="次へ(会員)" border="0"></A>
            </TD>
          </TR>

WAKABA;

		if (!$idpass) {
			$html .= <<<WAKABA
          <TR>
            <TD><FONT color="#ffcc00">●</FONT><FONT color="#333333">一般の方はこちら　→</FONT></TD>
            <TD>
            <A href="$PHP_SELF?mode=enter"><IMG src="/images/next_i.gif" width="79" height="22" alt="次へ(一般)" border="0"></A>
            </TD>
          </TR>

WAKABA;
		}

		$html .= <<<WAKABA
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

		if ($_SESSION['b_url']) {
			$b_url = $_SESSION['b_url'];
			$html .= <<<WAKABA
				<A href="$b_url"><IMG height="18" alt="買い物を続ける" src="/images/kaimono_t.gif" width="101" border="0"></A><BR>
				<FONT color="#ff6600">買い物を続ける場合は、上のボタンをクリックしてください。</FONT>

WAKABA;
		}

		$html .= "</center>";
	}

	return $html;

}



//	会員ログイン画面
function member() {
	global $PHP_SELF;

	if (!$_SESSION['customer']) {
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if ($_SESSION['ERROR']) {
		$ERROR = $_SESSION['ERROR'];
		$errors = ERROR($ERROR);
		unset($_SESSION['ERROR']);
		$id = $_SESSION['id'];
		unset($_SESSION['id']);
	}

	$html = <<<WAKABA
$errors
	  <div class="login-form">
	  	<p>
		会員登録時のメールアドレス、パスワードを入力して次へを押して下さい。
		</p>

		  <FORM action"$PHP_SELF" method="POST">
			<INPUT type="hidden" name="mode" value="mcheck">
			<TABLE class="table-resp">
				<TBODY>
				<TR>
					<th>メールアドレス（半角）</th>
					<TD><INPUT class="input-block" type="text" name="id" value="$id"></TD>
				</TR>
				<TR>
					<th>パスワード（半角）</th>
					<TD><INPUT class="input-block" type="password" name="pass"></TD>
				</TR>
				<TR>
					<TD colspan="3" align="center">
						<button class="btn btn-submit" type="submit">次へ</button>
						　
						<button class="btn btn-reset" type="reset">クリア</button>
					</TD>
				</TR>
				</TBODY>
			</TABLE>
		  </FORM>

		  <TABLE width="100%" border="0" cellpadding="0" cellspacing="0">
			<TBODY>
			  <TR>
				<TD align="right">
				<br>
				<A href="$PHP_SELF"><IMG src="/images/kaimono_m.gif" width="118" height="18" border="0" alt="買い物かごへ戻る"></A>
				</TD>
			  </TR>
			</TBODY>
		  </TABLE>
	  </div>

WAKABA;

	return $html;

}



//	ログインチェック
function mcheck() {
global $PHP_SELF,$db;

	$id = $_POST['id'];
	## $id = mb_convert_kana($id,"as","EUC-JP");
	$id = mb_convert_kana($id,"as","UTF-8");
	$id = trim($id);
	$pass = $_POST['pass'];
	## $pass = mb_convert_kana($pass,"as","EUC-JP");
	$pass = mb_convert_kana($pass,"as","UTF-8");
	$pass = trim($pass);
	if (!$id) { $ERROR[] = "メールアドレスが入力されておりません。"; }
	if (!$pass) { $ERROR[] = "パスワードが入力されておりません。"; }

	if (!$ERROR) {
		$check = 0;
		$sql  = "SELECT kojin_num FROM kojin" .
				" WHERE email='$id' AND pass='$pass' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			if ($kojin_num) { $check = 1; }
		}
		if ($check == 0) { $ERROR[] = "入力された情報が間違っているか登録されておりません。"; }
	}

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		$_SESSION['id'] = $id;
		header ("Location: $PHP_SELF?mode=member\n\n");
	} else {
		$_SESSION['idpass'] = "$id<>$pass<><>";
		header ("Location: $PHP_SELF?mode=enter\n\n");
	}

	exit;

}



//	住所入力画面
function enter($idpass) {
	global $PHP_SELF,$db,$PRF_L,$T_TIME_L,
		$PAY_L,$point_b,$point_c,$h_tax,$tax;

	if (!$_SESSION['customer']) {
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if ($_SESSION['check_list']) {
		$check_list = $_SESSION['check_list'];
		list($kojin_num,$name_s,$name_n,$kana_s,$kana_n,$email,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$point,$t_time,$r_point,$g_point,$msg,$pay_type,$delivery) = explode("<>",$check_list);
	} elseif ($idpass) {
		list($email,$pass,$memo) = explode("<>",$idpass);
		$sql  = "SELECT * FROM kojin" .
				" WHERE email='$email' AND pass='$pass' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$name_s = $list['name_s'];
			$name_n = $list['name_n'];
			$kana_s = $list['kana_s'];
			$kana_n = $list['kana_n'];
			$email = $list['email'];
			$zip1 = $list['zip1'];
			$zip2 = $list['zip2'];
			$prf = $list['prf'];
			$city = $list['city'];
			$add1 = $list['add1'];
			$add2 = $list['add2'];
			$tel1 = $list['tel1'];
			$tel2 = $list['tel2'];
			$tel3 = $list['tel3'];
			$fax1 = $list['fax1'];
			$fax2 = $list['fax2'];
			$fax3 = $list['fax3'];
			$point = $list['point'];
		}
		if (!$kojin_num) { unset($_SESSION['idpass']); }
	}

	if ($_SESSION['ERROR']) {
		$ERROR = $_SESSION['ERROR'];
		$errors .= ERROR($ERROR);
		unset($ERROR);
		unset($_SESSION['ERROR']);
	}

	$html = <<<WAKABA
$errors
      <TABLE border="0" bgcolor="#000000" cellspacing="1" cellpadding="0" width="100%">
        <TBODY>
          <TR align="center" bgcolor="#ffffff">
            <TD>
            <BR>
            <TABLE border="0">
              <TBODY>
                <TR>
                  <TD>
                  ご注文有り難う御座います。<BR>
                  下記にお名前、発送先の住所や必要事項を入力し”確認画面へ”ボタンを押して下さい。
                  </TD>
                </TR>
              </TBODY>
            </TABLE>
            <BR>
            </TD>
          </TR>
        </TBODY>
      </TABLE>

      <BR>

      <TABLE border="0" width="100%" cellpadding="0" cellspacing="0">
        <TBODY>
          <TR>
            <TD align="right">
            <A href="$PHP_SELF"><IMG src="/images/kaimono_m.gif" width="118" height="18" border="0" alt="買い物かごへ戻る"></A>
            </TD>
          </TR>
        </TBODY>
      </TABLE>

WAKABA;

	//	都道府県
	if (!$prf) { $selected = "selected"; } else { $selected = ""; }
	$l_prf = "              <OPTION value=\"\" $selected>選択してください</OPTION>\n";
	for ($i=1; $i<=47; $i++) {
		if ($prf == $i) { $selected = "selected"; } else { $selected = ""; }
		$l_prf .= "              <OPTION value=\"$i\" $selected>$PRF_L[$i]</OPTION>\n";
	}

	//	ポイントの御利用
	$checked = $checked1 = "checked";
	if ($r_point == 1) { $checked = ""; } else { $checked1 = ""; }

	//	購入金額
	$price_all = 0;
	if ($_SESSION['customer']) { $CART = $_SESSION['customer']; }
    $cart_items = array();
	foreach ($CART AS $list_num => $buy_num) {
        // edit simon 2019-03-18 set_flag 追加
		$sql  = "SELECT d.list_num, d.goods_name, b.maker_name, d.size, d.color, d.picture, a.price, a.stock, a.set_flag" .
				" FROM goods a, maker b, list d " .
				" WHERE d.list_num='$list_num' AND a.pluid=d.pluid AND b.maker_num=d.maker_num LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
            $cart_items[] = $list;
			$list_num = $list['list_num'];
			$goods_name = $list['goods_name'];
			$maker_name = $list['maker_name'];
			$size = $list['size'];
			$color = $list['color'];
			$picture = $list['picture'];
			$price = $list['price'];
			$stock = $list['stock'];
		}

		if ($h_tax == 1) {
			$price_h = $price + floor(($price * $tax) + 0.5);
		} else {
			$price_h = $price;
		}
		$price_sh = $price_h * $buy_num;
		$price_s = $price_h * $buy_num;
		$price_all += $price_s;
	}

    // add simon 2019-03-18 >>>
    if(!Souryou::canDaibiki($cart_items)) {
        unset($PAY_L[0]); // 着払いが使えません
    }
    // <<<

	//	支払い方法
	$p_count = count($PAY_L);
	$msg1 = "none";
	$msg2 = "block";
	if (!isset($pay_type)) { $pay_type = "4"; }
	foreach ($PAY_L AS $key => $val) {
		if ($pay_type == $key) {
			$pchecked = "checked";
			if ($key > 0 && $key < 3 && $price_all <= 10500) {
				// メール便一時非対応の為非表示	del ookawara 2011/04/17
				//$msg1 = "block";
				//$msg2 = "none";
			}
		} else {
			$pchecked = "";
		}

		if ($key < 1 || $key > 2) {
			$onclick = "onclick=\"hyouji('on');\"";
		} else {
			if ($price_all > 10500) {
				$onoff = "on";
			} else {
				$onoff = "off";
			}
			$onclick = "onclick=\"hyouji('{$onoff}');\"";
		}
		//	メール便一時停止の為の処理	add ookawara 2011/04/07
		$onclick = "";

		$pay_msg .= "<label class='nowrap'><INPUT type=\"radio\" name=\"pay_type\" value=\"$key\" $pchecked $onclick>：{$val} </label> ";
	}
	$visibility1 = $visibility2 = "hidden";


	//	配送方法
	if ($pay_type == 0 || $pay_type == 3 || $price_all > 10500) { $delivery = ""; }
	if ($delivery == 1) { $deli_checked = "checked"; } else { $deli_checked = ""; }

	//	お届け希望時間
	if ($delivery == 1) {
        // simon 2019-03-19 メール便が使わないので、このブロックは未使用です。
		$t_time = "";
		$msg3 = "block";
		$msg4 = "none";
		$msg5 = "block";
	} else {
		$msg3 = "none";
		$msg4 = "block";
		$msg5 = "none";
	}

    // add simon 2019-03-18 >>>
//    $cart = Souryou::fetchCartDetails();
    $delivery_company = Souryou::getDeliveryCompany($cart_items);
    // <<<

	if ($T_TIME_L) {
		if (!$t_time) { $t_time = "0"; }
		//foreach ($T_TIME_L AS $key => $val) {
		foreach (getDeliveryHoursFromCart($cart) AS $key) {
			if ($t_time == $key) { $selected = "selected"; } else { $selected = ""; }
			$l_t_time .= "<OPTION value=\"$key\" $selected>$T_TIME_L[$key]</OPTION>\n";
		}
	}

	$html .= <<<WAKABA
      <FORM action="$PHP_SELF" method="POST" name="cart">
      <INPUT type="hidden" name="mode" value="listcheck">
      <TABLE class="table-resp table-enter">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD>●<B>お客様の情報フォーム</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD><IMG height="5" src="/images/1pixel.gif" width="1" border="0"></TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>漢字氏名</B>(全角)：(例　山田　太郎)<FONT color="#ff0000">(必記)</FONT></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<span class="nowrap">
					姓：<INPUT type="text" class="input-inline" size="20" name="name_s" value="$name_s">　
				</span>
				<span class="nowrap">
					名：<INPUT type="text" class="input-inline" size="20" name="name_n" value="$name_n">
				</span>
			</TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>ふりがな</B>(全角)：(例　やまだ　たろう)<FONT color="#ff0000">(必記)</FONT></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<span class="nowrap">
					姓：<INPUT type="text" class="input-inline" size="20" name="kana_s" value="$kana_s">　
				</span>
				<span class="nowrap">
					名：<INPUT type="text" class="input-inline" size="20" name="kana_n" value="$kana_n">
				</span>
			</TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>E-mailアドレス</B>（半角）：（例　info@ozzys.jp）<FONT color="#ff0000">(必記)</FONT></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<INPUT type="text" class="input-block" name="email" value="$email">
			</TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>発送先の住所</B><FONT color="#ff0000">(必記)</FONT></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<div class="form-group">
					<FONT color="#ffcc00">●</FONT><B>郵便番号</B>(半角)：(例　123-456)<BR>
					〒<INPUT type="text" class="input-inline" size="6" name="zip1" value="$zip1">-<INPUT type="text" class="input-inline" size="8" name="zip2" value="$zip2">
				</div>

				<div class="form-group">
					<FONT color="#ffcc00">●</FONT><B>都道府県</B><BR>
					<SELECT name="prf" class="input-inline">
		$l_prf
					</SELECT>
				</div>

				<div class="form-group">
					<FONT color="#ffcc00">●</FONT><B>市区町村</B>(全角)：(例　太田市)<BR>
					<INPUT type="text" class="input-block" name="city" value="$city">
				</div>

				<div class="form-group">
					<FONT color="#ffcc00">●</FONT><B>所番地</B>(全角)：(例　浜町　６３-３１)<BR>
					<INPUT type="text" class="input-block" name="add1" value="$add1">
				</div>

				<div class="form-group">
					<FONT color="#ffcc00">●</FONT><B>マンション名など</B>（全角）：（例　OZZYSマンション３０２号）<BR>
					<INPUT type="text" class="input-block" name="add2" value="$add2">
				</div>
			</TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>電話番号</B>（半角）：（例　0276-49-2021）<FONT color="#ff0000">(必記)</FONT></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<INPUT type="text" class="input-inline input-phone" size="6" name="tel1" value="$tel1">
				<span class="sep-phone">-</span>
				<INPUT type="text" class="input-inline input-phone" size="6" name="tel2" value="$tel2">
				<span class="sep-phone">-</span>
				<INPUT type="text" class="input-inline input-phone" size="6" name="tel3" value="$tel3">
			</TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>FAX番号</B>（半角）：（例　0276-49-2021）</TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				<INPUT type="text" class="input-inline input-phone" size="6" name="fax1" value="$fax1">
				<span class="sep-phone">-</span>
				<INPUT type="text" class="input-inline input-phone" size="6" name="fax2" value="$fax2">
				<span class="sep-phone">-</span>
				<INPUT type="text" class="input-inline input-phone" size="6" name="fax3" value="$fax3">
			</TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>支払い方法</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
			<TD>
				$pay_msg
				<br>
				<A href="javascript:var junk=window.open('/riyou/payment.htm','riyou','width=400,height=500,scrollbars=yes')">お支払い方法詳細</A>
				<br>
<!--
				<font color="#ff0000">※会員の方が「カード払い」をお選びいただいた場合には、ポイントの加算は出来ません。予めご了承下さい。</font>
-->
            </TD>
          </TR>
          <TR bgcolor="#ffcc00">
            <TD><B>配送方法</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>
				<B>$delivery_company</B><BR>
<SCRIPT language="JavaScript">
<!--
function hyouji(lId) {
	if (lId == 'off') {
		msg1.style.display = "block";
		msg2.style.display = "none";
	}
	else {
		document.cart.delivery.checked=false;
		msg1.style.display = "none";
		msg2.style.display = "block";
		hyouji2('on');
	}
}
//-->
</SCRIPT>

WAKABA;

/*
<SPAN style="display:$msg1;" id="msg1">　<INPUT type="checkbox" name="delivery" value="1" $deli_checked onClick="hyouji2()">：メール便、利用可能なら利用する。<br>
</SPAN>
*/
	$html .= <<<WAKABA
<SPAN style="display:$msg1;" id="msg1">$mailbin</SPAN>
WAKABA;

	$html .= <<<WAKABA
<SPAN style="display:$msg2;" id="msg2">$mailbin</SPAN>

<!--
            　<A href="javascript:var junk=window.open('/riyou/mail.htm','riyou','width=400,height=320,scrollbars=yes')">メール便の送料、注意点</A>
-->
<div style="display:$msg5;" id="msg5">
<hr>
<center><b><font color="#ff0000">■クロネコヤマト・メール便の送料、注意点について■</font></b></center><br>

<b><font color="#ff0000">
○ 送料は、メール便で発送出来るか確定しておりませんので、確定次第ご連絡させて頂きます。<br>
○ 商品の数量、大きさによってはメール便での発送が出来ない場合もございます。予めご了承下さい。<br>
</font></b>
<b>【メリット】</b><br>
スプーンやフックなどの小さい商品や小額の商品でご利用いただきますと、送料が大変お安くなります。<br>
<b>【デメリット】</b><br>
・商品が、お客様の郵便受けに投函されますので、宅急便に比べて事故の可能性が高くなります。<br>
・大きな商品や高額の商品ではご利用できません。<br>
・お支払い方法は、銀行振込又は現金書留の前払いとなります。<br>
・宅急便に比べ、配達に時間がかかる場合があります。<font color="#ff0000">最長で2日?4日程度かかる場合もございます。</font><br>
・梱包は緩衝材入りの封筒になりますので、宅急便に比べ、配送中に商品の破損・汚損の可能性が高くなります。<br>
・ご注文時に送料が確定できませんので、ご注文後<font color="#ff0000">（1営業日以内）</font>に合計金額のご連絡をいたします。お客様からの了承の確認メールをいただいてからの発送となりますので、発送までに、お時間がかかる場合がございます。<br>
<br>
<table class="table-resp table-noborder">
  <tr valign="top">
	<td>
●メール便がご利用できる商品は<br>
　・厚さ2cm以下の商品。（封筒を含む）<br>
　・重さ1,000g以下の商品。（封筒を含む）<br>
　・大きさA4サイズ以下の商品。（封筒を含む）<br>
	</td>
	<td>
		<table class="table-resp table-resp-nogap table-mailsize">
			<tr>
				<th colspan="2" align="center">料金（全国一律）</th>
			</tr>
			<tr>
				<td>A4（角2封筒以内）厚さ=1cmまで</td>
				<td class="nowrap" align="right">80円</td>
			</tr>
			<tr>
				<td>A4（角2封筒以内）厚さ=2cmまで</td>
				<td class="nowrap" align="right">160円</td>
			</tr>
			<tr>
				<td>B4厚さ=1cmまで</td>
				<td class="nowrap" align="right">160円</td>
			</tr>
			<tr>
				<td>B4厚さ=2cmまで</td>
				<td class="nowrap" align="right">240円</td>
			</tr>
		</table>
	</td>
  </tr>
</table>
</div>
			</TD>
          </TR>

          <TR bgcolor="#ffcc00">
            <TD><B>お届け希望時間</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>
<SCRIPT language="JavaScript">
<!--
function hyouji2() {

	if (document.cart.delivery.checked) {
		msg3.style.display = "block";
		msg4.style.display = "none";
		msg5.style.display = "block";
	}
	else {
		msg3.style.display = "none";
		msg4.style.display = "block";
		msg5.style.display = "none";
	}
}
//-->
</SCRIPT>

<span style="display:$msg3;" id="msg3">　メール便をご利用する場合はお届け希望時間の指定はできません。</span>

<span style="display:$msg4;" id="msg4">
　<SELECT class="input-inline" name="t_time">
$l_t_time
</SELECT>
</span>

            </TD>
          </TR>

WAKABA;

	$html .= <<<WAKABA
          <TR bgcolor="#ffcc00">
            <TD><B>ご意見ご要望など</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　<TEXTAREA name="msg" rows="5" class="input-block">$msg</TEXTAREA><BR>
            </TD>
          </TR>
          <TR bgcolor="#ffffff" align="center">
            <TD align="middle"><INPUT type="image" alt="確認画面へ" src="/images/kakunin.gif" value="確認画面へ" border="0" onClick="return check_email();"></TD>
          </TR>
        </TBODY>
      </TABLE>
      </FORM>
<!--モバイルメールアドレスチェック 2013/02/07-->
<script type="text/javascript">
<!--
function check_email(){
	var flag = 0;
	// 設定開始（チェックする項目を設定してください）
	if(document.cart.email.value.match(/@(docomo|softbank|disney|ezweb|[dhtkrsnqc]\.vodafone|pdx|d[kij]\.pdx|wm\.pdx|'.'em\.nttpnet|pipopa|.*sky\.tu-ka|.*sky\.tk[ck]|jp-[dhtkrsnqc]|t[2-9]\.ezweb)\.ne\.jp$/i) || document.cart.email.value.match(/@(bandai\.jp|i\.softbank\.jp|willcom\.com)$/i)){
		flag = 1;
	}
	// 設定終了
	if(flag){
		if (confirm("お客様の入力したメールアドレスは、携帯のアドレスですので、当店からのメール（自動返信メールや出荷メール等）が\\n「迷惑メール設定」により、届かない可能性がございます。「迷惑メール設定」の解除をしてから、再度ご入力ください。\\n（「迷惑メール設定」の解除方法につきましては、お手持ちのキャリア会社までお問い合わせください。）\\nこの、メールアドレスで、注文処理を進めても、よろしいでしょうか。")) {
			return true; // 送信を実行
		} else {
			return false; // 送信を中止
		}
	} else{
		return true; // 送信を実行
	}
}

// -->
</script>

<SCRIPT language="JavaScript">
<!--HPB_SCRIPT_CODE_40
function _HpbShowObj(lId)
{
  var ob;ob=new Array;
  var appVer=parseInt(navigator.appVersion);
  var isNC=false,isN6=false,isIE=false;
  if (document.all && appVer >= 4) isIE=true; else
    if (document.getElementById && appVer > 4) isN6=true; else
      if (document.layers && appVer >= 4) isNC=true;
  if (isNC)
  {
    w_str = "document." + lId;ob[lId] = eval(w_str);
    if (!ob[lId]) ob[lId] = _HpbFindHiddenObj(document, lId);
    if (ob[lId]) ob[lId].visibility = "show";
  }
  if (isN6)
  {
    ob[lId] = document.getElementById(lId);
    ob[lId].style.visibility = "visible";
  }
  if (isIE)
  {
    w_str = "document.all.item(\"" + lId + "\").style";ob[lId] = eval(w_str);
    ob[lId].visibility = "visible";
  }
}

function _HpbFindHiddenObj(doc, lId)
{
  for (var i=0; i < doc.layers.length; i++)
  {
    var w_str = "doc.layers[i].document." + lId;
    var obj;obj=new Array;
    obj[lId] = eval(w_str);
    if (!obj[lId]) obj[lId] = _HpbFindHiddenObj(doc.layers[i], lId);
    if (obj[lId]) return obj[lId];
  }
  return null;
}
function _HpbHideObj(lId)
{
  var ob;ob=new Array;
  var appVer=parseInt(navigator.appVersion);
  var isNC=false,isN6=false,isIE=false;
  if (document.all && appVer >= 4) isIE=true; else
    if (document.getElementById && appVer > 4) isN6=true; else
      if (document.layers && appVer >= 4) isNC=true;
  if (isNC)
  {
    w_str = "document." + lId;ob[lId] = eval(w_str);
    if (!ob[lId]) ob[lId] = _HpbFindShownObj(document, lId);
    if (ob[lId]) ob[lId].visibility = "hide";
  }
  if (isN6)
  {
    ob[lId] = document.getElementById(lId);
    ob[lId].style.visibility = "hidden";
  }
  if (isIE)
  {
    w_str = "document.all.item(\"" + lId + "\").style";ob[lId] = eval(w_str);
    ob[lId].visibility = "hidden";
  }
}

function _HpbFindShownObj(doc, lId)
{
  for (var i=0; i < doc.layers.length; i++)
  {
    var w_str = "doc.layers[i].document." + lId;
    var obj;obj=new Array;
    obj[lId] = eval(w_str);
    if (!obj[lId]) obj[lId] = _HpbFindShownObj(doc.layers[i], lId);
    if (obj[lId]) return obj[lId];
  }
  return null;
}
//-->
</SCRIPT>

WAKABA;

	return $html;

}



//	住所入力チェック
function listcheck($idpass) {
global $PHP_SELF,$db,$point_c;

	unset($_SESSION['customer_check']);

	$name_s = $_POST['name_s'];
	## $name_s = mb_convert_kana($name_s,"AsKV","EUC-JP");
	$name_s = mb_convert_kana($name_s,"AsKV","UTF-8");
	$name_s = trim($name_s);
	$name_n = $_POST['name_n'];
	## $name_n = mb_convert_kana($name_n,"AsKV","EUC-JP");
	$name_n = mb_convert_kana($name_n,"AsKV","UTF-8");
	$name_n = trim($name_n);
	$kana_s = $_POST['kana_s'];
	## $kana_s = mb_convert_kana($kana_s,"AsHV","EUC-JP");
	$kana_s = mb_convert_kana($kana_s,"AsHV","UTF-8");
	$kana_s = trim($kana_s);
	$kana_n = $_POST['kana_n'];
	## $kana_n = mb_convert_kana($kana_n,"AsHV","EUC-JP");
	$kana_n = mb_convert_kana($kana_n,"AsHV","UTF-8");
	$kana_n = trim($kana_n);
	$email = $_POST['email'];
	## $email = mb_convert_kana($email,"as","EUC-JP");
	$email = mb_convert_kana($email,"as","UTF-8");
	$email = trim($email);
	$email = strtolower($email);
	$zip1 = $_POST['zip1'];
	## $zip1 = mb_convert_kana($zip1,"na","EUC-JP");
	$zip1 = mb_convert_kana($zip1,"na","UTF-8");
	$zip1 = trim($zip1);
	$zip2 = $_POST['zip2'];
	## $zip2 = mb_convert_kana($zip2,"na","EUC-JP");
	$zip2 = mb_convert_kana($zip2,"na","UTF-8");
	$zip2 = trim($zip2);
	$prf = $_POST['prf'];
	$city = $_POST['city'];
	## $city = mb_convert_kana($city,"AsKV","EUC-JP");
	$city = mb_convert_kana($city,"AsKV","UTF-8");
	$city = trim($city);
	$add1 = $_POST['add1'];
	## $add1 = mb_convert_kana($add1,"AsKV","EUC-JP");
	$add1 = mb_convert_kana($add1,"AsKV","UTF-8");
	$add1 = trim($add1);
	$add2 = $_POST['add2'];
	## $add2 = mb_convert_kana($add2,"AsKV","EUC-JP");
	$add2 = mb_convert_kana($add2,"AsKV","UTF-8");
	$add2 = trim($add2);
	$tel1 = $_POST['tel1'];
	## $tel1 = mb_convert_kana($tel1,"ns","EUC-JP");
	$tel1 = mb_convert_kana($tel1,"ns","UTF-8");
	$tel1 = trim($tel1);
	$tel2 = $_POST['tel2'];
	## $tel2 = mb_convert_kana($tel2,"ns","EUC-JP");
	$tel2 = mb_convert_kana($tel2,"ns","UTF-8");
	$tel2 = trim($tel2);
	$tel3 = $_POST['tel3'];
	## $tel3 = mb_convert_kana($tel3,"ns","EUC-JP");
	$tel3 = mb_convert_kana($tel3,"ns","UTF-8");
	$tel3 = trim($tel3);
	$fax1 = $_POST['fax1'];
	## $fax1 = mb_convert_kana($fax1,"ns","EUC-JP");
	$fax1 = mb_convert_kana($fax1,"ns","UTF-8");
	$fax1 = trim($fax1);
	$fax2 = $_POST['fax2'];
	## $fax2 = mb_convert_kana($fax2,"ns","EUC-JP");
	$fax2 = mb_convert_kana($fax2,"ns","UTF-8");
	$fax2 = trim($fax2);
	$fax3 = $_POST['fax3'];
	## $fax3 = mb_convert_kana($fax3,"ns","EUC-JP");
	$fax3 = mb_convert_kana($fax3,"ns","UTF-8");
	$fax3 = trim($fax3);
	$t_time = $_POST['t_time'];
	$r_point = $_POST['r_point'];
	$g_point = $_POST['g_point'];
	## $g_point = mb_convert_kana($g_point,"na","EUC-JP");
	$g_point = mb_convert_kana($g_point,"na","UTF-8");
	$g_point = trim($g_point);
	$msg = $_POST['msg'];
	## $msg = mb_convert_kana($msg,"AsKV","EUC-JP");
	$msg = mb_convert_kana($msg,"AsKV","UTF-8");
	$msg = trim($msg);

	$pay_type = $_POST['pay_type'];
	$delivery = $_POST['delivery'];
	if ($pay_type == 0 || $pay_type == 3) { unset($delivery); }

	if ($idpass) {
		list($email_,$pass_,$memo_) = explode("<>",$idpass);
		$sql  = "SELECT kojin_num, point FROM kojin" .
				" WHERE email='$email_' AND pass='$pass_' AND kojin_num BETWEEN 100001 AND 600000 LIMIT 1;";
		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
			$kojin_num = $list['kojin_num'];
			$point = $list['point'];
		}
		if (!$kojin_num) { unset($_SESSION['idpass']); }
	}

	$_SESSION['check_list'] = "$kojin_num<>$name_s<>$name_n<>$kana_s<>$kana_n<>$email<>$zip1<>$zip2<>$prf<>$city<>$add1<>$add2<>$tel1<>$tel2<>$tel3<>$fax1<>$fax2<>$fax3<>$point<>$t_time<>$r_point<>$g_point<>$msg<>$pay_type<>$delivery<>";

	if (!$name_s) { $ERROR[] = "漢字氏名（姓）が入力されておりません。"; }
	if (!$name_n) { $ERROR[] = "漢字氏名（名）が入力されておりません。"; }
	if (!$kana_s) { $ERROR[] = "ひらがな氏名（姓）が入力されておりません。"; }
	if (!$kana_n) { $ERROR[] = "ひらがな氏名（名）が入力されておりません。"; }
	if (!$zip1) { $ERROR[] = "郵便番号３桁が入力されておりません。"; }
	$zip1_n = strlen($zip1);
##	if ($zip1 && (!eregi("[0-9]",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }
	if ($zip1 && (!preg_match("/[0-9]/i",$zip1) || ($zip1_n != 3))) { $ERROR[] = "郵便番号３桁が不正です。"; }	
	if (!$zip2) { $ERROR[] = "郵便番号４桁が入力されておりません。"; }
	$zip2_n = strlen($zip2);
##	if ($zip2 && (!eregi("[0-9]",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }
	if ($zip2 && (!preg_match("/[0-9]/i",$zip2) || ($zip2_n != 4))) { $ERROR[] = "郵便番号４桁が不正です。"; }	
	if (!$prf) { $ERROR[] = "都道府県名が選択されておりません。"; }
	if (!$city) { $ERROR[] = "市区町村名が入力されておりません。"; }
	if (!$add1) { $ERROR[] = "所番地が入力されておりません。"; }
	if (!$tel1 || !$tel2 || !$tel3) { $ERROR[] = "電話番号が入力されておりません。"; }
    if (strlen($tel1.$tel2.$tel3)>11) { $ERROR[] = "電話番号が11桁しか入力できません。"; } // add simon 20190222　FIXME　メッセージ
	//if (!$fax1 || !$fax2 || !$fax3) { $ERROR[] = "FAX番号が入力されておりません。"; }
	if (!$email) { $ERROR[] = "E-mailアドレスが入力されておりません。"; }
##	if ($email && !ereg("^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }
	if ($email && !preg_match("/^[a-zA-Z0-9_\.\-]+@(([a-zA-Z0-9_\-]+\.)+[a-zA-Z0-9]+$)/",$email,$regs)) { $ERROR[] = "E-mailアドレスが不正です。"; }	
	//$mail_host = $regs[1];
	//if ($email &&!getmxrr($mail_host,$mxhostarr)) { $ERROR[] = "E-mailアドレスのホスト名が見つかりませんでした。"; }
	//if ($t_time) { $ERROR[] = "お届け希望時間が選択されておりません。"; }
	if ($r_point == 1 && !$g_point) { $ERROR[] = "御利用ポイントが入力されておりません。"; }
	if ($point < $g_point) { $ERROR[] = "御利用できるポイントがオーバーしています。"; }
	if ($r_point == 1 && $g_point) {
		$amari = $g_point % $point_c;
		$point_c_ = number_format($point_c);
		if ($amari != 0) { $ERROR[] = "ポイントは<B>$point_c_</B>ポイント単位で御利用してください。"; }
	}
	//if (!$msg) { $ERROR[] = "ご意見ご要望などが入力されておりません。"; }

	if ($pay_type == "") { $ERROR[] = "支払い方法が確認出来ません。"; }
	if (($pay_type == 0 || $pay_type == 3) && $delivery == 1) { $ERROR[] = "着払いでは、メール便をご利用する事はできません。"; }

	if ($delivery > 0) { $ERROR[] = "只今メール便はご利用できません。"; }	//	add ookawara 2011/05/25

	if ($ERROR) {
		$_SESSION['ERROR'] = $ERROR;
		header ("Location: $PHP_SELF?mode=enter\n\n");
	} else {
		$_SESSION['customer_check'] = 1;
		header ("Location: $PHP_SELF?mode=check\n\n");
	}

	exit;

}



//	確認画面
function check($idpass) {
	global $PHP_SELF,$db,$PRF_L,$T_TIME_L,
		$R_POINT_L,$UN_L,$T_SOU_L,$PAY_L,$tui_sou,$daibiki,
		$tax,$point_b,$point_c,$gpx,$gpy,$h_tax;

	if (!$_SESSION['customer'] || $_SESSION['customer_check'] != 1) {
		header ("Location: $PHP_SELF\n\n");
		exit;
	}

	if ($_SESSION['check_list']) {
		$check_list = $_SESSION['check_list'];
		list($kojin_num,$name_s,$name_n,$kana_s,$kana_n,$email,$zip1,$zip2,$prf,$city,$add1,$add2,$tel1,$tel2,$tel3,$fax1,$fax2,$fax3,$point,$t_time,$r_point,$g_point,$msg,$pay_type,$delivery) = explode("<>",$check_list);
	}

	//	送料無料設定定数変数設定
	//	add ookawara 2016/07/25
	$souryoufree = 0;
	if (defined("SOURYOUFREE")) {
		$souryoufree = SOURYOUFREE;
	}
	$souryoufreeprice = 0;
	if (defined("SOURYOUFREEPRICE")) {
		$souryoufreeprice = SOURYOUFREEPRICE;
	}

	$html = <<<WAKABA
      <TABLE border="0" bgcolor="#000000" cellspacing="1" cellpadding="0" width="100%">
        <TBODY>
          <TR align="center" bgcolor="#ffffff">
            <TD>
            <BR>
            <TABLE border="0">
              <TBODY>
                <TR>
                  <TD>
                  ご注文有り難う御座います。<BR>
                  下記の内容で間違いがないようでしたら下方にあります注文のボタンを押して下さい。<BR>
                  もし、ご注文の商品を変更するようでしたら商品の一覧の下方にある商品変更ボタンを、<BR>
                  住所などお客様の情報が違っている場合はお客様情報編集ボタンを押して下さい。
                  </TD>
                </TR>
              </TBODY>
            </TABLE>
            <BR>
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	//	都道府県
	if (!$prf) { $selected = "selected"; } else { $selected = ""; }
	$l_prf = "              <OPTION value=\"\" $selected>選択してください</OPTION>\n";
	for ($i=1; $i<=47; $i++) {
		if ($prf == $i) { $selected = "selected"; } else { $selected = ""; }
		$l_prf .= "              <OPTION value=\"$i\" $selected>$PRF_L[$i]</OPTION>\n";
	}

	// 未使用 simon 2017-06-15
	////	お届け時間
	//if ($T_TIME_L) {
	//	if (!$t_time) { $t_time = "0"; }
	//	foreach ($T_TIME_L AS $key => $val) {
	//		if ($t_time == $key) { $selected = "selected"; } else { $selected = ""; }
	//		$l_t_time .= "<OPTION value=\"$key\" $selected>$T_TIME_L[$key]</OPTION>\n";
	//	}
	//}

	//	ポイントの御利用
	$checked = $checked1 = "checked";
	if ($r_point == 1) { $checked = ""; } else { $checked1 = ""; }

	$price_all = 0;
	$CART = $_SESSION['customer'];
    $cart_items = array();
	foreach ($CART AS $list_num => $buy_num) {
		$sql  = "SELECT d.list_num, d.goods_name, b.maker_name, d.size, d.color, d.picture, a.price, a.stock, a.class_m, a.set_flag" .
				", d.free_postage".					//	add ookawara 2016/07/25
				" FROM goods a, maker b, list d " .
				" WHERE d.list_num='$list_num' AND a.pluid=d.pluid AND b.maker_num=d.maker_num LIMIT 1;";

		if ($result = pg_query($db,$sql)) {
			$list = pg_fetch_array($result);
            $cart_items[] = $list; // add simon 2018-07-05
			$list_num = $list['list_num'];
			$goods_name = $list['goods_name'];
			$maker_name = $list['maker_name'];
			$size = $list['size'];
			$color = $list['color'];
			$picture = $list['picture'];
			$price = $list['price'];
			$stock = $list['stock'];
			$free_postage = $list['free_postage'];	//add okabe 2016/07/15
			$class_m = $list['class_m'];
            $set_flag = $list['set_flag']; // add simon 2018-07-03
		}

		if ($buy_num > $stock) {
			$buy_num = $stock;
			$CART[$list_num] = $buy_num;
			$COM .= "$goods_name($list_num)は在庫数が減ったため $buy_num までしか購入できません。<BR>\n";
		}
		if ($stock <= 0) {
			$buy_n = 0;
			$COM .= "$goods_name($list_num)は在庫が無くなってしまったため購入できません。<BR>\n";
			unset($CART[$list_num]);
			continue;
		}

		$check_price = 0;	//	add ookawara 2016/07/28
		if ($h_tax == 1) {
			$price_h = $price + floor(($price * $tax) + 0.5);
		}
		else {
			$price_h = $price;
		}
		$check_price = $price_h;	//	add ookawara 2016/07/28

		$price_sh = $price_h * $buy_num;
		$price_s = $price_h * $buy_num;
		$price_all += $price_s;
		$price_h = number_format($price_h) . "円";
		$price_sh = number_format($price_sh) . "円";
		if (!$size) { $size = "--"; }
		if (!$color) { $color = "--"; }
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
			$img_msg = "<IMG src=\"$pic_url\" width=\"$width_\" height=\"$height_\" border=\"0\" alt=\"$goods_name\">\n";
		} else {
			$img_msg = "";
		}

		// add okabe start 2016/07/15
		$img_free_postage = "";
		if ($souryoufree == 1 && ($free_postage == 1 || ($souryoufreeprice > 0 && $souryoufreeprice <= $check_price))) {
				$img_free_postage = "<div class=\"blinking\" style=\"color: red; font-weight: bold; width: 100%;\">基本送料無料</div>\n";	//	add	ohkawara	2020/04/22	"基本"を追加	//	del  text-align:left; ohkawara 2020/04/23
		}
		// add okabe end 2016/07/15

		//	add ohkawara 2020/04/23
		$rod_fee_block = "";
		if(Souryou::prod_has_rod_fee($list)) {
			$fee = Souryou::cart_rod_shipping_fee(array($list));
			$rod_fee_block = "<br><div>追加送料<br>".number_format($fee)."円</div>";
		}

		//	add $rod_fee_block ohkawara 2020/04/23

		$html .= <<<WAKABA

		<div class="cart-line cart-line-resp clearfix">
			<div class="cart-cell cart-name">
				$goods_name (No.$list_num)
			</div>
			<div class="cart-group cart-group-1 text-center">
				<div class="cart-cell">
					{$img_msg}{$img_free_postage}{$rod_fee_block}
				</div>
			</div>

			<div class="cart-group cart-group-2">
				<table class="table-cart">
					<tr>
						<th>メーカー</th>
						<td>$maker_name</td>
					</tr>
					<tr>
						<th>サイズ</th>
						<td>$size</td>
					</tr>
					<tr>
						<th>カラー</th>
						<td>$color</td>
					</tr>
					<tr>
						<th>値段</th>
						<td>$price_h</td>
					</tr>
					<tr>
						<th>数量</th>
						<td>$buy_num</td>
					</tr>
				</table>
			</div>

			<div class="cart-group cart-group-3">
				<div class="cart-cell text-right">$price_sh</div>
			</div>
		</div>

WAKABA;
	}

	//	消費税表示

	$html .= <<<WAKABA
		<br>
      <TABLE class="table-cart-check">
WAKABA;

	//if ($h_tax != 1 || true) { // TEST
	if ($h_tax != 1) {
		$tax_num = $tax * 100;
		$syouhi = 0;
		$syouhi = $price_all * $tax;
		$syouhi = ceil($syouhi);
		$price_all = $price_all + $syouhi;
		$syouhi = number_format($syouhi) . "円";

		$html .= <<<WAKABA
          <TR>
            <TD class="cart-tax">消費税($tax_num%)</TD>
            <TD align="right">$syouhi</TD>
          </TR>

WAKABA;
	}

	//	送料表示
	//add okabe start 2016/07/15

	$postage_fee = "送料";
	$free_postage_msg = "";			//	add ookawara 2016/07/25

    $unchin = Souryou::calculate($cart_items, $UN_L[$prf], $price_all);

    if (Souryou::can_be_free()) {
        $price_checkx = Souryou::cart_amount_until_free_shipping($price_all);
        if ($unchin > 0 && $price_checkx !== null && $price_checkx > 0) {
            $free_postage_msg = "<span style=\"color:red; font-weight:bold;\">あと ".number_format($price_checkx)."円で基本送料が無料になります。</span>";	//	add	ohkawara	2020/04/22	"送料無料"を"基本送料が無料"に変更
        }
    }

	if ($pay_type != 0 && $pay_type != 3) { $daibiki = 0; }

//    if(Souryou::cart_has_rods_with_fee($cart_items)) {
//        // add simon 2020-04-23 竿の場合は、送料追加
//        $unchin_ = "基本送料<br>+".number_format(Souryou::cart_rod_shipping_fee($cart_items))."円";
//    }
//    else {
        $unchin_ = number_format($unchin) . "円";
//    }

	$daibiki_ = number_format($daibiki) . "円";

	if ($pay_type != 0 && $pay_type != 3) {
		unset($daibiki_);
		if ($delivery == 1) {
			unset($unchin);
			$unchin_ = "未確定";
			$all_price_msg = "<BR>\n<b>+送料</b>";
			$souryou_msg = "<FONT color=\"#ff0000\"><B>※送料決定後、お支払い確定金額をご連絡させていただきます。</B></FONT><BR>\n";
		}
	} else {
		$daibiki_html = <<<WAKABA
          <TR>
            <TD class="cart-tax">代引き手数料</TD>
            <TD align="right">$daibiki_</TD>
          </TR>
WAKABA;
	}
	$price_all += $unchin + $daibiki;

	$html .= <<<WAKABA
          <TR>
            <TD class="cart-tax">{$free_postage_msg}{$postage_fee}</TD>	<!-- edit okb 2016/07/15 -->
            <TD align="right">$unchin_</TD>
          </TR>
$daibiki_html

WAKABA;

	//	ポイント利用
	//if ($r_point == 1 && $g_point > 0 || true) { // TEST
	if ($r_point == 1 && $g_point > 0) {
		$money = ($g_point / $point_c) * $point_b;
		$price_all_ = $price_all - $money;
		if ($price_all_ < 0) {
			$amari = 0;
			$over_p = abs($price_all_);
			if ($over_p >= $point_b) {
				$over_pri = $over_p / $point_b;
				$hen_point = floor($over_pri);
				$hen_point = $hen_point * $point_c;
			}
			$g_point = $g_point - $hen_point;
			$money = ($g_point / $point_c) * $point_b;
			$price_all -= $money;
		} else {
			$price_all = $price_all_;
		}
		$g_point_ = number_format($g_point) . "ポイント";
		$h_money = number_format($money);
		$html .= <<<WAKABA
          <TR>
            <TD class="cart-tax">御利用ポイント：$g_point_</TD>
            <TD align="right">-$h_money円</TD>
          </TR>

WAKABA;
	}

	//	合計金額
	//	add ※追加 ohkawara 2020/04/23
	if ($price_all < 0) { $price_all = 0; }
	$price_all = number_format($price_all) . "円";
	$html .= <<<WAKABA
          <TR>
            <TD class="cart-total-title">合計金額</TD>
            <TD class="cart-total-value" align="right">$price_all$all_price_msg</TD>
          </TR>
        </TBODY>
      </TABLE>
※追加送料の商品が複数有る場合、追加送料が一番高い金額が追加送料として適応されます。<br><br>

WAKABA;

	$html .= <<<WAKABA
      <TABLE border="0" width="100%" cellpadding="0" cellspacing="0">
        <TBODY>
          <TR>
            <TD align="right">
            <A href="$PHP_SELF"><IMG src="/images/kaimono_m.gif" width="118" height="18" border="0" alt="買い物かごへ戻る"></A>
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	//	お客様情報確認
	$html .= <<<WAKABA
      <TABLE class="table-resp table-resp-nogap" cellspacing="1" cellpadding="2" border="0">
        <TBODY>
          <TR bgcolor="#ffcc00">
            <TD>●<B>お客様の情報</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD><IMG height="5" src="/images/1pixel.gif" width="1" border="0"></TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>漢字氏名</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$name_s $name_n</TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>ふりがな</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$kana_s $kana_n</TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>E-mailアドレス</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$email</TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>発送先の住所</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　〒$zip1-$zip2<BR>
            　$PRF_L[$prf] $city $add1 $add2</TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>電話番号</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$tel1-$tel2-$tel3</TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>FAX番号</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$fax1-$fax2-$fax3</TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>支払い方法</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　{$PAY_L[$pay_type]}</TD>
          </TR>
          <TR bgcolor="#cccccc">
            <TD><B>配送方法</B></TD>
          </TR>

WAKABA;

    // add simon 2019-03-18
    $delivery_company = Souryou::getDeliveryCompany($cart_items);

	if ($delivery == 1) {
        // simon 2019-03-118 未使用
		$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD>
            　$delivery_company<BR>
            　メール便、利用可能なら利用する。
            </TD>
          </TR>

WAKABA;
	} else {
		$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD>
            　$delivery_company<BR>
            </TD>
          </TR>

WAKABA;
	}

	if ($delivery != 1) {
 		$html .= <<<WAKABA
         <TR bgcolor="#cccccc">
            <TD><B>お届け希望時間</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$T_TIME_L[$t_time]</TD>
          </TR>

WAKABA;
	}

	if ($r_point == 1 && $g_point > 0) {
		$m_point = "御利用ポイント：" . number_format($g_point);
		$html .= <<<WAKABA
          <TR bgcolor="#cccccc">
            <TD><B>ポイントの御利用</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$R_POINT_N[$r_point] $m_point</TD>
          </TR>

WAKABA;
	}

	if ($msg) {
		$msg_ = nl2br($msg);
##		$msg_ = eregi_replace("<BR />","<BR />　",$msg_);
		$msg_ = preg_replace("/<BR \/>/i","<BR />　",$msg_);	
		$html .= <<<WAKABA
          <TR bgcolor="#cccccc">
            <TD><B>ご意見ご要望など</B></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD>　$msg_</TD>
          </TR>
WAKABA;
	}

	$html .= <<<WAKABA
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	$html .= <<<WAKABA
      <TABLE border="0" width="100%" cellpadding="0" cellspacing="0">
        <TBODY>
          <TR>
            <TD align="right">
            <A href="$PHP_SELF?mode=enter"><IMG src="/images/hensyu.gif" width="107" height="24" border="0" alt="お客様情報編集"></A>
            </TD>
          </TR>
        </TBODY>
      </TABLE>

	  <div class="text-center">
		  <FORM action="$PHP_SELF" method="POST">
		  <INPUT type="hidden" name="mode" value="send">
		   <INPUT type="image" value="ご注文" src="/images/order.gif" alt="ご注文">
		  </FORM>
	  </div>

WAKABA;

	$_SESSION['check_list'] = "$kojin_num<>$name_s<>$name_n<>$kana_s<>$kana_n<>$email<>$zip1<>$zip2<>$prf<>$city<>$add1<>$add2<>$tel1<>$tel2<>$tel3<>$fax1<>$fax2<>$fax3<>$point<>$t_time<>$r_point<>$g_point<>$msg<>$pay_type<>";

	return $html;

}


//       _____
//      / / \ \
//     / /| |\ \
//    / / | | \ \
//   / /  |_|  \ \
//  /_/   (_)   \_\

// シモン 2018-11-12 "send()" の仕組みは新しい "cart_save_send.inc" に移動しました


//	エラー表示
function ERROR($ERROR) {

	foreach ($ERROR AS $VAL) {
		if ($VAL) { $err .= "・$VAL<BR>\n"; }
	}

	$errors = <<<WAKABA
      <TABLE border="0" width="95%">
        <TBODY>
          <TR>
            <TD>
            <B><FONT color="#ff0000">エラー</FONT></B><BR>
$err
            </TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>

WAKABA;

	return $errors;

}


/**
 * カートの中身によって、お届け時間の一覧を作ります
 * 簡単に言うと竿がありかかどうか。
 *
 * @global connection $db
 * @global array $T_SOU_L rods class_m ids
 * @global array $T_TIME_L_CLIENT_DISPLAY お届け時間の一覧
 * @global array $T_TIME_L_ROD_DISPLAY 竿用のお届け時間設定（できませんというメッセージ）
 * @param array $cart_  カート内容（Souryou::fetchCartDetails()の値）
 * @return array
 */
function getDeliveryHoursFromCart($cart_) {
	global $T_SOU_L, $T_TIME_L_CLIENT_DISPLAY, $T_TIME_L_ROD_DISPLAY;

	foreach($cart_ as $item) {
        if(in_array($item['class_m'], $T_SOU_L)) {
            return $T_TIME_L_ROD_DISPLAY;
        }
	}

	// default
	return $T_TIME_L_CLIENT_DISPLAY;
}
?>
