<?PHP

	include "../cone.inc";
	include "./sub/cago.inc";
	include "./sub/menu_p.inc";
	include "./sub/array.inc";
	include "./sub/head.inc";
	include "./sub/foot.inc";
	include "./sub/base.php";
	include "./sub/souryou_muryou.php";

	$script = "cago.php";
	$dir = "";

	session_start();
	$_SESSION["idpass"];
	$_SESSION["addr"];
	$_SESSION["customer"];
	$_SESSION["refere"];
	$_SESSION["opt"];
	$_SESSION["enter"];

	$mode = trim($_POST['mode']);
	$hinban = trim($_POST['hinban']);
	$title = trim($_POST['title']);
	$kakaku = trim($_POST['kakaku']);
	$code = trim($_POST['code']);
	$name = trim($_POST['name']);
	$size = trim($_POST['size']);
	$op_num = trim($_POST['op_num']);

unset($ERROR);

	$refere_ = $_SERVER[HTTP_REFERER];

	//	アフェリエイトidが存在すればセッション埋め込み
	if ($_COOKIE['affid']) {
		$_SESSION['affid'] = $_COOKIE['affid'];
	}

	if (!$hinban && !$title && $code && $name && $size) {
		$hinban = "$code" . "-$size";
		$title = "$name/$size";
	}

	if ($hinban && $title && $kakaku) {
		$flags = 0;
##		if (ereg($admin_url,$refere_) || ereg($admin_url2,$refere_))  { $flags = 1; }
		if (preg_match($admin_url,$refere_) || preg_match($admin_url2,$refere_))  { $flags = 1; }
		if ($flags == 0) {
			$hinban = ""; $title = ""; $kakaku = "";
		}
	}

	$domein = DOMEIN;	//	add ookawara 2014/01/24
	if (!preg_match("/$script/", $refere_) && !preg_match("/order/", $refere_) && preg_match("/$domein/" , $refere_)) {	//	add ookawara 2014/01/24
	//if (!preg_match($script,$refere_) && !preg_match("/order/",$refere_)) {	//	del ookawara 2014/01/24
		$refere = $refere_;
		$_SESSION['refere'] = $refere;
	}
//echo("mode<>$mode<>$hinban,$title,$kakaku<BR>\n");
	unset($KAGOS);
	unset($OPTIONS);
	if ($mode == "check" || (!$hinban && !$title && !$kakaku && !$op_num)) { list($KAGOS,$OPTIONS) = check($KAGOS,$OPTIONS); }
	elseif ($mode == "") { list($KAGOS,$OPTIONS,$ERROR) = add($hinban,$title,$kakaku); }
	elseif ($mode == "hen") { list($KAGOS,$OPTIONS,$ERROR) = hen($hinban); }
	elseif ($mode == "del") { list($KAGOS,$OPTIONS) = del($hinban); }
	elseif ($mode == "del_op") { list($KAGOS,$OPTIONS) = del_op($op_num); }

//	$menu = menu($dir);

	$html = headers();
//	$html .= $menu;
	$html .= hyouji($KAGOS,$OPTIONS,$ERROR,$refere);
	$html .= footer();

	echo("$html");

	exit();


//	表示	************************************************************************
function hyouji($KAGOS,$OPTIONS,$ERROR,$refere) {
global $PHP_SELF,$TAX_,$waribiki,$waribiki2,$wa_member,$mochi_pri,
		$SEBAN_P_N,$SEBAN_N,$SENAME_P_N,$SENAME_N,$MUNEBAN_P_N,$MUNEBAN_N,$PANT_P_N,$PANT_N,
		$BACH_P_N,$BACH_N,$DISCOUNT_C,$DISCOUNT,$free_shipping,$SOURYOU_MURYOU;

	$idpass = $_SESSION['idpass'];
	$addr = $_SESSION['addr'];

	if ($ERROR) { $errors = ERROR($ERROR); }

	$html = <<<WAKABA
<!-- コンテンツ -->
<div class="con_name"><div class="con_text"><B>買い物かご</B></div></div>
$errors
WAKABA;

	if (!$KAGOS && !$OPTIONS) {
		$html .= <<<WAKABA
<table width="750px">
	<TR>
		<th class="cate2">
			現在選択されている商品はありません。
		</th>
	</TR>
</TABLE>
<table width="750px">
	<TR>
		<TD class="cate3" id="cate3in">
			もし、お買い物商品が買い物かごに入らない場合は、<B><A href="/cautions.htm">問題解決方法</A></B>のページをご覧下さい。<BR>
			<p>
			<B><A href="/cautions.htm">問題解決方法</A></B>
			</p>
			それでもだめな場合はお手数ですが、ご注文されたい商品名と数量及び、お名前、ご住所、電話番号を明記の上、メールでご連絡下さい。
			<p>
			ご注文用メールアドレス：<B><A href="mailto:orders@futboljersey.com?Subject=order">orders@futboljersey.com</A></B>
			</p>
		</TD>
	</TR>
</TABLE>
      <BR>
      <BR>
      <BR>
      <BR>
WAKABA;
	}
	else {
		$html .= <<<WAKABA
      <table width="780px">
          <TR>
            <th class="cate2" height="20">　<FONT color="#ff0000" style="font-size:12pt;"><B>●注意</B></FONT></TD>
          </TR>
          <TR>
            <TD class="cate3" id="cate3in">
            　同じ商品を複数
			　ご購入される場合は、その数量を変更し数量変更ボタンを押して下さい。<BR>
            　お求めの商品が決まり、マーキングする方は、マーキングを利用するボタンを。<BR>
            　商品のみご購入の方は、ご購入ボタンを押して下さい。<BR>
            　マーキングを選択致しますと、背番号やネームプリントなどの入力ページに移ります。<BR>
            　＊複数のバッジを希望される方は同じ操作を繰り返してください。<BR>
            <BR>
            　<font color="#ff000"><b>●商品在庫について</b></font><BR>
            　当店は実店舗と同一の在庫であります関係上、ご注文後に在庫切れが発生する場合がございます。<BR>
            　誠にご迷惑をお掛け致しますが、あらかじめご了承の頂きますようお願い申し上げます。<BR>
            <BR>
            　ご購入を選択いたしますと、送り先住所の入力ページへ移ります。
            </TD>
          </TR>
      </TABLE>
      <BR>
      <TABLE style="border:0;" width="600">
          <TR>
            <TD style="border:0;">買い物をつづける方は”買い物をつづける”ボタンをクリックして下さい。</TD>
            <TD align="right" style="border:0;">
            <FORM action="$refere">
            <INPUT type="submit" value="買い物をつづける" class="button">
            </TD></FORM>
          </TR>
      </TABLE>
      <BR>
      <table width="780px">
          <TR>
            <th class="cate2" align="center" height="20"><B>商品番号</B></th>
            <th class="cate2" rowspan="2" style="text-align:center;text-indent: 0px;" width="90"><B>単価</B></th>
            <th class="cate2" rowspan="2" style="text-align:center;text-indent: 0px;" width="90"><B>数量</B></th>
            <th class="cate2" rowspan="2" style="text-align:center;text-indent: 0px;" width="90"><B>小計</B></th>
            <th class="cate2" rowspan="2" style="text-align:center;text-indent: 0px;" width="90"><B>削除</B></th>
          </TR>
          <TR>
            <th class="cate2" align="center" height="20"><B>商品名</B></th>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
WAKABA;

		unset($customer);
		$price_all = 0;
		$souryou_muryou_flag = 0;
		if ($KAGOS) {
			foreach ($KAGOS AS $val) {
				$syoukei = 0;
##				list($hinban_,$title_,$kakaku_,$num_) = split("::",$val);
				list($hinban_,$title_,$kakaku_,$num_) = explode("::",$val);
				if ($hinban_ == "") { continue; }
				else { $customer .= "$val<>"; }
				//	送料無料チェック
				$hinban_id = "";
				$GDNM = explode("-",$hinban_);
				$gdnm_max = count($GDNM) - 1;
				for ($i=0; $i<$gdnm_max; $i++) {
					if ($hinban_id) { $hinban_id .= "-"; }
					$hinban_id .= $GDNM[$i];
				}
				if ($SOURYOU_MURYOU[$hinban_id]) { $souryou_muryou_flag = 1; }

				$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
				$syoukei = $kakaku_ * $num_;
				$price_all = $price_all + $syoukei;
				$kakaku_h = number_format($kakaku_);
				$syoukei = number_format($syoukei);

				$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD align="center" height="20">$hinban_</TD>
            <TD rowspan="2" align="right">$kakaku_h 円</TD>
            <TD rowspan="2" align="center">
             <FORM action="$PHP_SELF" method="POST">
             <INPUT type="hidden" name="mode" value="hen">
             <INPUT type="hidden" name="hinban" value="$hinban_">
             <INPUT size="4" type="text" name="num" value="$num_"><BR>
             <INPUT type="submit" value="数量変更" class="button">
            </TD></FORM>
            <TD rowspan="2" align="right">$syoukei 円</TD>
            <TD rowspan="2" align="center">
             <FORM action="$PHP_SELF" method="POST">
             <INPUT type="hidden" name="mode" value="del">
             <INPUT type="hidden" name="hinban" value="$hinban_">
             <INPUT type="submit" value="削除" class="button">
            </TD></FORM>
          </TR>
          <TR bgcolor="#ffffff">
            <TD align="center" height="20">$title_</TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
WAKABA;
			}
			$_SESSION['customer'] = $customer;
		}

		if ($OPTIONS) {
			unset($opt);
			foreach ($OPTIONS AS $val) {
				$syoukei = 0;
##				list($op_num_,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = split("::",$val);
				list($op_num_,$hinban_,$title_,$seban_l_,$seban_num_,$sename_l_,$sename_name_,$muneban_l_,$muneban_num_,$pant_l_,$pant_num_,$bach_l_) = explode("::",$val);
				if ($hinban_ == "") { continue; }
				else { $opt .= "$val<>"; }

				$html .= <<<WAKABA
          <TR bgcolor="#efefcf">
            <TD colspan="4" height="20">
            <B>マーキング 商品名：$title_</B>
            </TD>
            <TD align="center">
             <FORM action="$PHP_SELF" method="POST">
             <INPUT type="hidden" name="mode" value="del_op">
             <INPUT type="hidden" name="op_num" value="$op_num_">
             <INPUT type="submit" value="削除" class="button">
            </TD></FORM>
          </TR>
WAKABA;

				//	持ち込み手数料
				if ($hinban_ == "mochikomi") {
					$kakaku_ = $mochi_pri;
					$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
					$syoukei = $kakaku_;
					$price_all = $price_all + $syoukei;
					$kakaku_ = number_format($kakaku_);
					$syoukei = number_format($syoukei);

					$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD width="240" height="20">持ち込み手数料</TD>
            <TD align="right">$kakaku_ 円</TD>
            <TD align="center">1</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
WAKABA;
				}

				//	背番号
				if ($seban_l_) {
					$moji_num = strlen($seban_num_);
					$kakaku_ = $SEBAN_P_N[$seban_l_];
					$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
					$syoukei = $kakaku_ * $moji_num;
					$price_all = $price_all + $syoukei;
					$kakaku_ = number_format($kakaku_);
					$syoukei = number_format($syoukei);

					$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD width="240" height="20">背番号 $SEBAN_N[$seban_l_] 番号：$seban_num_</TD>
            <TD align="right">$kakaku_ 円</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
WAKABA;
				}

				//	背ネーム
				if ($sename_l_) {
    				$sename_name_ = str_replace('\\', '', $sename_name_);
					$sename_name_m = str_replace(' ', '', $sename_name_);
					$moji_num = strlen($sename_name_m);
					$kakaku_ = $SENAME_P_N[$sename_l_];
					$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
					$syoukei = $kakaku_ * $moji_num;
					$price_all = $price_all + $syoukei;
					$kakaku_ = number_format($kakaku_);
					$syoukei = number_format($syoukei);

					$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD width="240" height="20">背ネーム $SENAME_N[$sename_l_] ネーム：$sename_name_</TD>
            <TD align="right">$kakaku_ 円</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
WAKABA;
				}

//	胸番号
				if ($muneban_l_) {
					$moji_num = strlen($muneban_num_);
					$kakaku_ = $MUNEBAN_P_N[$muneban_l_];
					$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
					$syoukei = $kakaku_ * $moji_num;
					$price_all = $price_all + $syoukei;
					$kakaku_ = number_format($kakaku_);
					$syoukei = number_format($syoukei);

					$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD width="240" height="20">胸番号 $MUNEBAN_N[$muneban_l_] 番号：$muneban_num_</TD>
            <TD align="right">$kakaku_ 円</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
WAKABA;
				}

//	パンツ番号
				if ($pant_l_) {
					$moji_num = strlen($pant_num_);
					$kakaku_ = $PANT_P_N[$pant_l_];
					$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
					$syoukei = $kakaku_ * $moji_num;
					$price_all = $price_all + $syoukei;
					$kakaku_ = number_format($kakaku_);
					$syoukei = number_format($syoukei);

					$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD width="240" height="20">パンツ番号 $PANT_N[$pant_l_] 番号：$pant_num_</TD>
            <TD align="right">$kakaku_ 円</TD>
            <TD align="center">$moji_num</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
WAKABA;
				}

				//	バッジ
				if ($bach_l_) {
					$kakaku_ = $BACH_P_N[$bach_l_];
					$kakaku_ = floor($kakaku_ * ($TAX_ + 1) + 0.5);
					$syoukei = $kakaku_;
					$price_all = $price_all + $syoukei;
					$kakaku_ = number_format($kakaku_);
					$syoukei = number_format($syoukei);

					$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD width="240" height="20">バッジ $BACH_N[$bach_l_]</TD>
            <TD align="right">$kakaku_ 円</TD>
            <TD align="center">1</TD>
            <TD align="right">$syoukei 円</TD>
            <TD></TD>
          </TR>
WAKABA;
				}

				$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
WAKABA;

			}
			$_SESSION['opt'] = $opt;
		}

		if ($idpass && $waribiki > 0) {
			list($id,$pass) = explode("<>",$idpass);
			include "../cone.inc";
			$sql = "SELECT kojin_num FROM kojin WHERE email='$id' AND pass='$pass' AND saku!='1' AND kojin_num<'100000';";
			$sql1 = mysqli_query($conn_id,$sql);
			$count = mysqli_num_rows($sql1);
			if ($count >= 1) {
				list($kojin_num) = mysqli_fetch_array($sql1);
			}
			if ($kojin_num <= $wa_member) {
				$nebiki = $price_all * $waribiki / 100;
				$price_all = $price_all - $nebiki;
				$nebiki = number_format($nebiki);

				$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <th colspan="3" align="right" class="cate2" height="20"><FONT color="#FF0000"><B>特別会員割引( {$waribiki}% )</B></FONT></th>
            <th align="right" class="cate2"><FONT color="#FF0000">-$nebiki</FONT> 円</TD>
            <th class="cate2"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
WAKABA;
			}
		}
		elseif (!$idpass && $waribiki2 > 0) {
			$nebiki = $price_all * $waribiki2 / 100;
			$price_all = $price_all - $nebiki;
			$nebiki = number_format($nebiki);

			$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <th colspan="3" align="right" class="cate2" height="20"><FONT color="#FF0000"><B>割引( {$waribiki2}% )</B></FONT></th>
            <th align="right" class="cate2"><FONT color="#FF0000">-$nebiki</FONT> 円</th>
            <th class="cate2"></th>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
WAKABA;
		}
		elseif ($DISCOUNT_C == 1) {
			unset($nebiki);
			$DISCOUNT = array_reverse($DISCOUNT);
			foreach ($DISCOUNT AS $VAL) {
				$totalprice_ = $VAL[0];
				$paersent_ = $VAL[1];
				if ($price_all > $totalprice_) {
					$paersent = $paersent_;
					break;
				}
			}
			$nebiki = $price_all * $paersent / 100;
			$price_all = $price_all - $nebiki;
			$nebiki = number_format($nebiki);

			if ($nebiki) {
				$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <th colspan="3" align="right" class="cate2" height="20"><FONT color="#FF0000"><B>割引( {$paersent}% )</B></FONT></th>
            <th align="right" class="cate2"><FONT color="#FF0000">-$nebiki</FONT> 円</th>
            <th class="cate2"></th>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
WAKABA;
			}
		}

		$price_all_ = $price_all;
		$tax = number_format($tax);
		$price_all = number_format($price_all);

		$shipping = "送料・";
//		if ($free_shipping != "" && $free_shipping < $price_all_) {
//			$shipping = "";
//		}

		if ($souryou_muryou_flag == 1) { 
			$shipping = "";
			$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <th colspan="3" align="right" class="cate2" height="20"><B>送料</B></th>
            <th  style="text-align:center;text-indent: 0px;" class="cate2"><font color="#ff0000"><b>サービス</b></font></th>
            <th class="cate2"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <th colspan="3" align="right" class="cate2" height="20"><B>手数料</B></th>
            <th  style="text-align:center;text-indent: 0px;" class="cate2">未定</th>
            <th class="cate2"></TD>
          </TR>
          <TR bgcolor="#ffffff">
            <TD colspan="5"></TD>
          </TR>
WAKABA;
		} else {
			$html .= <<<WAKABA
          <TR bgcolor="#ffffff">
            <th colspan="3" align="right" class="cate2" height="20"><B>送料・手数料</B></th>
            <th  style="text-align:center;text-indent: 0px;" class="cate2">未定</th>
            <th class="cate2"></TD>
          </TR>
WAKABA;
		}

		$html .= <<<WAKABA
          <TR bgcolor="#cccc66">
            <th class="cate2" colspan="3" align="right" height="20"><B>合計金額</B></TD>
            <th class="cate2" colspan="2"  style="text-align:center;text-indent: 0px;" width="180"><B>$price_all 円 + {$shipping}手数料</B></TD>
          </TR>
        </TBODY>
      </TABLE>
      <BR>
	<div style="width:750px; text-align:center;">
      <FORM action="marking.php" method="POST">
      <INPUT type="submit" value="マーキングをご利用する方はこちらからご注文下さい。" class="button">
      </FORM>
	</div>

WAKABA;

		if(!$idpass) {

			$html .= <<<WAKABA
      <TABLE style="border:0; width:750px;" cellpadding="5">
          <TR>
            <TD style="border:0; text-align:center;">
             <FORM action="./member/" method="POST">
             <FONT color="#FF0000">
             割引ポイントが貯まる、お得な会員になりませんか？<BR>
<!--
             今登録しても買い物中のかごの商品は消えません。<BR>
-->
             </FONT>
             <INPUT type="submit" value="会員登録はこちらから登録できます。" class="button">
            </TD></FORM>
          </TR>
      </TABLE>
      <BR>
WAKABA;

		}
		else {
			$html .= "<BR>\n";
		}

		$html .= <<<WAKABA
	<div style="width:750px; text-align:center;">
      以上でよろしければ下記をクリックして下さい。<BR>
      <BR>
	</div>
	<div style="width:750px; padding-left: 175px;">

      <TABLE  style="border:0; width:375px;" cellpadding="5">
WAKABA;

		if($idpass || $addr) {
			$html .= <<<WAKABA
          <TR>
            <TD style="border:0px;">ご購入はこちら</TD>
            <TD style="border:0px;">→</TD>
           <TD align="center"style="border:0px;">
              <FORM action="./order/address.php" method="POST">
             <INPUT type="submit" value="ご購入" class="button">
            </TD></FORM>
          </TR>
WAKABA;
		}
		else {
			$html .= <<<WAKABA
          <TR>
            <TD style="border:0;">会員の方で、ご購入はこちら</TD>
            <TD style="border:0;">→</TD>
            <TD align="center" style="border:0;">
             <FORM action="./order/idpass.php" method="POST">
             <INPUT type="submit" value="ご購入（会員）" class="button" STYLE="width:130;">
            </TD></FORM>
          </TR>
          <TR>
            <TD style="border:0;">非会員の方で、ご購入はこちら</TD>
            <TD style="border:0;">→</TD>
            <TD align="center" style="border:0;">
             <FORM action="./order/address.php" method="POST">
             <INPUT type="submit" value="ご購入（非会員）" class="button" STYLE="width:130;">
            </TD></FORM>
          </TR>
WAKABA;
		}

		$html .= <<<WAKABA
          <TR>
            <TD align="center" colspan="3" style="border:0;">
             <BR>
             <FORM action="./team.php" method="POST">
             <INPUT type="submit" value="チームオーダーの御見積はこちら。" class="button"">
            </TD></FORM>
          </TR>
      </TABLE>
		</div>
      <BR>
      <BR>
      <BR>
WAKABA;

	}

	return $html;

}


//	ヘッター	************************************************************************
function headers() {

	$html = head_html($title);
	$html .= head_menu_html();
	$html .= head_login_html();
	$html .= special_html();
	$html .= side_menu_html();


	return $html;

}


//	フッター	************************************************************************
function footer() {
/*
	$footmsg = footmsg();
	$html = <<<WAKABA
<!-- コンテンツ終了 -->
      </td>
    </tr>
    <tr>
      <td colspan="2">$footmsg</td>
    </tr>
</table>
</body>
</html>

WAKABA;
*/
	$html = foot_html($title);

	return $html;

}



//エラー	************************************************************************
function ERROR($ERROR) {

	$max = count($ERROR);

	$errors = "";
	for ($i=0; $i<$max; ++$i) {
		$errors .=  "・ $ERROR[$i] <BR>\n";
	}

	$error = <<<WAKABA
      <TABLE border="0" width="600" cellspacing="1" bgcolor="#666600">
          <TR>
            <th class="cate2" height="20">　<FONT color="#ff0000"><B>エラー</B></FONT></th>
          </TR>
          <TR bgcolor="#ffffff">
            <TD class="cate3" id="cate3in">
$errors
            </TD>
          </TR>
      </TABLE>
      <BR>
WAKABA;

	return $error;

}
?>
