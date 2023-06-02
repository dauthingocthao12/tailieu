<?PHP
//	管理者画面セレクト部分

//	編集カテゴリー		select_main()
//	商品管理カテゴリー	select_order()
//	発注状況			select_send()
//	商品				select_s_order()
//	表示数				select_view()
//	期間				select_data()
//	CSVファイル作成		select_csv()
//	顧客管理カテゴリー	select_kokyaku()
//	都道府県			select_prf()
//	顧客番号 名前(名字)	select_num()
//	E-mail				select_email()
//	顧客登録			select_menber()
//	在庫管理			select_zaiko()
//	商品管理			select_goods()

//	セレクトメイン
function select() {
global $main,$order,$kokyaku,$prf_,$view,$num_k_,$name_s_,$PHP_SELF,$main_r,$order_r,$kokyaku_r,$zaiko,$zaiko_r,$touroku,$hlist,$aff_r,$k_numbe,$aff_number,$sells_number;	
include './array.inc';
	$main_r=$_POST['main_r'];
	$aff=$_POST['aff'];
	$view=$_POST['view'];
	$prf_=$_POST['prf_'];
	$num_k_=$_POST['num_k_'];
	$name_s_=$_POST['name_s_'];
	$hlist=$_POST['hlist'];
	$k_numbe=$_POST['k_numbe'];
	$sells_number=$_POST['sells_number'];
	$touroku=$_POST['touroku'];
	if ($main != $main_r) { $view = ""; }
	if ($order && $order != $order_r) { $view = ""; }
	if ($zaiko && $zaiko != $zaiko_r) { $view = ""; }
	if ($kokyaku && $kokyaku != $kokyaku_r) { $view = ""; $prf_ = ""; $num_k_ = ""; $name_s_ = ""; $hlist = ""; }
	if ($aff && $aff != $aff_r) { unset($k_numbe); unset($aff_number); unset($name_s_); unset($hlist); unset($view); unset($sells_number); }
	echo <<<ALPHA
<BR>
<FORM action='$PHP_SELF' method='POST'>
<INPUT type='hidden' name='main_r' value='$main'>
<INPUT type='hidden' name='order_r' value='$order'>
<INPUT type='hidden' name='kokyaku_r' value='$kokyaku'>
<INPUT type='hidden' name='zaiko_r' value='$zaiko'>
<INPUT type='hidden' name='aff_r' value='$aff'>
<TABLE border="0" cellpadding="2" cellspacing="3">
  <TBODY>
    <TR>

ALPHA;

select_main();
if ($main == 1) {
	select_order();
	if ($order == 1) { select_view(); }
	elseif ($order == 3) { select_csv(); }
	elseif ($order == 4) { select_data(); select_view(); }
}
elseif ($main == 2) {
	select_zaiko();
	if ($zaiko == 1) { select_hinban(); select_cate(); select_view(); }
	elseif ($zaiko == 2) {  }
	elseif ($zaiko == 3) { select_touroku(); select_hinban(); if ($touroku != 2) { select_cate(); } select_view(); }
	elseif ($zaiko == 4) { select_hinban(); select_cate(); select_view(); }
}
elseif ($main == 3) {
	select_aff();
	if ($aff == 1) { select_affm(); select_member(); select_aff_member(); select_users(); select_hlist(); select_view(); }
	elseif ($aff == 2) {  select_aff_member(); select_sells_num(); select_hlist(); select_view(); }
	elseif ($aff == 3) { select_aff_state(); select_aff_type(); select_member(); select_aff_member(); select_users(); select_hlist(); select_view(); }
	elseif ($aff == 4) {  }
}
elseif ($main == 4) {
	select_kokyaku();
	if ($kokyaku == 1) { select_menber2(); select_prf(); select_num(); select_email(); select_hlist(); select_view(); }
	elseif ($kokyaku == 2) { select_menber(); select_prf(); select_num(); select_email(); select_hlist(); select_view(); }
	elseif ($kokyaku == 3) { select_prf(); select_num(); select_email(); select_hlist(); select_view(); }
	elseif ($kokyaku == 4) { select_csv(); }
}
elseif ($main == 5) { select_goods(); }
elseif ($main == 7) { select_meruma(); }

?>
    <TD><BR><INPUT type='submit' value='決定'></TD>
    </TR>
  </TBODY>
</TABLE>
<HR>
</FORM>
<?PHP
	}


//	編集カテゴリー
function select_main() {
include './array.inc';
$main=$_POST['main'];
?>
      <TD>編集カテゴリー<BR>
       <SELECT name='main' onchange='submit();'>
<?PHP
	if (!$main || ($main == 0)) { $select = "selected"; } else { $select = "";}
        	echo ("		<OPTION value='' $select>$MAIN_N[0]</OPTION>\n");
	$count = count($MAIN_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if($i==2||$i==6) continue; // 在庫管理とHPアクセス情報管理ぬける
			if ($main == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>$MAIN_N[$i]</OPTION>\n");
				}
echo ("      </SELECT></TD>\n");
}


//	商品管理カテゴリー
function select_order() {
include './array.inc';
$order=$_POST['order'];
	echo ("      <TD>$MAIN_N[$main]カテゴリー<BR><SELECT name='order' onchange='submit();'>\n");
	if (!$order || ($order == 0)) { $select = "selected"; } else { $select = "";}
        	echo ("		<OPTION value='' $select>$MAIN_N[0]</OPTION>\n");
	$count = count($ORDER_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
			if ($order == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>$ORDER_N[$i]</OPTION>\n");
				}
	echo ("      </SELECT></TD>\n");
}


//	発注状況
function select_send() {
include './array.inc';
$send=$_POST['send'];
	echo ("      <TD>発注状況<BR><SELECT name='send'>\n");
	if (!$send || ($send == 0)) { $select = "selected"; } else { $select = "";}
	    	echo ("		<OPTION value='' $select>$SEND_N[0]</OPTION>\n");
	$count = count($SEND_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($send == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$SEND_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");
}


//	期間
function select_data() {
include './array.inc';
$year_s=$_POST['year_s'];
$mon_s=$_POST['mon_s'];
$day_s=$_POST['day_s'];
$year_e=$_POST['year_e'];
$mon_e=$_POST['mon_e'];
$day_e=$_POST['day_e'];

	$year_t = date("Y");
	$mon_t = date("n");
	$day_t = date("d");
	if (!$year_s) { $year_s = 2003; }
	if (!$mon_s) { $mon_s = 1; }
	if (!$day_s) { $day_s = 1; }
	if (!$year_e) { $year_e = $year_t; }
	if (!$mon_e) { $mon_e = $mon_t; }
	if (!$day_e) { $day_e = $day_t; }

	echo ("      <TD>表示期間<BR><SELECT name='year_s'>\n");
	for ($i=2003; $i<=$year_t; $i++) {
		if ($year_s == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
			}
	echo ("      </SELECT>年<SELECT name='mon_s'>\n");
	for ($i=1; $i<=12; $i++) {
		if ($mon_s == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
			}
	echo ("      </SELECT>月<SELECT name='day_s'>\n");
	for ($i=1; $i<=31; $i++) {
		if ($day_s == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
			}
	echo ("      </SELECT>日～<SELECT name='year_e'>\n");
	for ($i=2003; $i<=$year_t; $i++) {
		if ($year_e == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
			}
	echo ("      </SELECT>年<SELECT name='mon_e'>\n");
	for ($i=1; $i<=12; $i++) {
		if ($mon_e == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
			}
	echo ("      </SELECT>月<SELECT name='day_e'>\n");
	for ($i=1; $i<=31; $i++) {
		if ($day_e == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$i</OPTION>\n");
			}
	echo ("      </SELECT>日</TD>\n");

}

//	表示数
function select_view() {
include './array.inc';
$view=$_POST['view'];

	echo ("      <TD>表示数<BR><SELECT name='view'>\n");
//	if (!$view || ($view == 0)) { $select = "selected"; } else { $select = "";}
//  echo ("		<OPTION value='' $select>$VIEW_N[0]</OPTION>\n");
	if (!$view) { $view = 1; }
	$count = count($VIEW_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($view == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$VIEW_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");
}


//	在庫管理テゴリー
function select_zaiko() {
global $zaiko_r;
include './array.inc';
$zaiko=$_POST['zaiko'];
$cate1=$_POST['cate1'];
$cate2=$_POST['cate2'];
$cate3=$_POST['cate3'];

	echo ("      <TD>$MAIN_N[$main]カテゴリー<BR><SELECT name='zaiko' onchange='submit();'>\n");
	if (!$zaiko || ($zaiko == 0)) { $select = "selected"; } else { $select = "";}
       	echo ("		<OPTION value='' $select>$ZAIKO_N[0]</OPTION>\n");
	$count = count($ZAIKO_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($zaiko == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$ZAIKO_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");

	if ($zaiko != $zaiko_r) { $cate1 = ""; $cate2 = ""; $cate3 = ""; }

}


// 在庫商品登録状況
function select_touroku() {
include './array.inc';
$touroku=$_POST['touroku'];

	echo ("      <TD>登録状況<BR><SELECT name='touroku'>\n");
	if (!$touroku || $touroku == 0) { $select = "selected"; } else { $select = "";}
       	echo ("		<OPTION value='' $select>$TOUROKU_N[0]</OPTION>\n");
	$count = count($TOUROKU_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($touroku == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$TOUROKU_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");

}


// 在庫商品番号
function select_hinban() {
$hinban_=$_POST['hinban_'];

	echo <<<ALPHA
      <TD>在庫商品番号<BR><INPUT size='20' type='text' name='hinban_' value='$hinban_'>
      </TD>
ALPHA;

}


//	在庫商品カテゴリー
function select_cate() {
include './list.inc';
$cate1=$_POST['cate1'];
$cate2=$_POST['cate2'];
$cate3=$_POST['cate3'];
$cate1_r=$_POST['cate1_r'];
$cate2_r=$_POST['cate2_r'];

	if ($cate1 != $cate1_r) { $cate2 = ""; $cate3 = ""; }
	if ($cate2 != $cate2_r) { $cate3 = ""; }

	echo ("      <TD>メインカテゴリー<BR><SELECT name='cate1' onchange='submit();'>\n");
	if (!$cate1 || ($cate1 == 0)) { $select = "selected"; } else { $select = "";}
	echo ("		<OPTION value='' $select>$KATE[0]</OPTION>\n");
	$max = count($KATE);
	for ($i=1; $i<$max; $i++) {
		if ($cate1 == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$KATE[$i]</OPTION>\n");
			}
	echo ("      </SELECT>\n");
	echo ("      <INPUT type=\"hidden\" name=\"cate1_r\" value=\"$cate1\">\n");
	echo ("      </TD>\n");

	if ($cate1 && $cate1 != 0) {

		$KATE_N = "KATE_$cate1";

		echo ("      <TD>カテゴリー2<BR><SELECT name='cate2'>\n");
		if (!$cate2 || ($cate2 == 0)) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='' $select>{${"$KATE_N"}[0]}</OPTION>\n");
		$max = count(${$KATE_N});
		for ($i=1; $i<$max; $i++) {
			if ($cate2 == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>{${"$KATE_N"}[$i]}</OPTION>\n");
				}
		echo ("      </SELECT>\n");
		echo ("      <INPUT type=\"hidden\" name=\"cate2_r\" value=\"$cate2\">\n");
		echo ("      </TD>\n");

		}

	if ($cate2 && $cate2 != 0) {

		$KATE_N = "KATE_$cate1" . "_$cate2";

		echo ("      <TD>カテゴリー3<BR><SELECT name='cate3'>\n");
		if (!$cate2 || ($cate2 == 0)) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='' $select>{${$KATE_N}[0]}</OPTION>\n");
		$max = count(${$KATE_N});
		for ($i=1; $i<$max; $i++) {
			if ($cate3 == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>{${$KATE_N}[$i]}</OPTION>\n");
				}
		echo ("      </SELECT></TD>\n");

		}

}


//	アフェリエイトカテゴリー
function select_aff() {
include './array.inc';
$aff=$_POST['aff'];

	echo ("      <TD>$MAIN_N[$main]カテゴリー<BR><SELECT name='aff' onchange='submit();'>\n");
	if (!$aff || ($aff == 0)) { $select = "selected"; } else { $select = "";}
       	echo ("		<OPTION value='' $select>$KOKYAKU_N[0]</OPTION>\n");
	$count = count($AFR_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($aff == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$AFR_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");
}


//	顧客管理カテゴリー
function select_kokyaku() {
include './array.inc';
$kokyaku=$_POST['kokyaku'];

	echo ("      <TD>$MAIN_N[$main]カテゴリー<BR><SELECT name='kokyaku' onchange='submit();'>\n");
	if (!$kokyaku || ($kokyaku == 0)) { $select = "selected"; } else { $select = "";}
       	echo ("		<OPTION value='' $select>$KOKYAKU_N[0]</OPTION>\n");
	$count = count($KOKYAKU_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($kokyaku == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$KOKYAKU_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");
}


//	都道府県
function select_prf() {
include '../sub/array.inc';
$prf_=$_POST['prf_'];

	echo ("      <TD>都道府県<BR><SELECT name='prf_'>\n");
	if (!$prf_ || ($prf_ == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='' $select>$PRF_N[0]</OPTION>\n");
	$count = count($PRF_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($prf_ == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$PRF_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");
}


// 顧客番号 名前(名字)
function select_num() {
$num_k_=$_POST['num_k_'];
$name_s_=$_POST['name_s_'];
$email_=$_POST['email_'];
?>
      <TD>顧客番号<BR><INPUT size='20' type='text' name='num_k_' value='<?PHP echo ("$num_k_"); ?>'>
      </TD>
      <TD>名前(名字)<BR><INPUT size='20' type='text' name='name_s_' value='<?PHP echo ("$name_s_"); ?>'>
      </TD>
<?PHP
}


// メールアドレス
function select_email() {
$s_email_=$_POST['s_email_'];
?>
      <TD>E-mail<BR><INPUT size='20' type='text' name='s_email_' value='<?PHP echo ("$s_email_"); ?>'>
      </TD>
<?PHP
}


// 顧客登録
function select_menber() {
$menber=$_POST['menber'];
include './array.inc';

	echo ("      <TD>登録状態<BR><SELECT name='menber'>\n");
	if (!$menber || ($menber == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='0' $select>$MENBER_N[0]</OPTION>\n");
	$count = count($MENBER_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($menber == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$MENBER_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");

}


// 顧客登録状態
function select_menber2() {
$menber2=$_POST['menber2'];
include './array.inc';

	echo ("      <TD>会員<BR><SELECT name='menber2'>\n");
	if (!$menber2 || ($menber2 == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='0' $select>$MENBER2_N[0]</OPTION>\n");
	$count = count($MENBER2_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($menber2 == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$MENBER2_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");

}



// 表示並び
function select_hlist() {
$hlist=$_POST['hlist'];
include './array.inc';

	$main = $_POST['main'];
	$aff = $_POST['aff'];
	if ($main == 3 && $aff && $hlist == "") {
		$hlist = 1;
	}

	echo ("      <TD>並び<BR><SELECT name='hlist'>\n");
	if (!$hlist || ($hlist == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='0' $select>$HLIST_N[0]</OPTION>\n");
	$count = count($HLIST_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($hlist == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$HLIST_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");

}


// CSV作成
function select_CSV() {
$csv=$_POST['csv'];
include './array.inc';

	echo ("      <TD>作業<BR><SELECT name='csv'>\n");
	if (!$csv || ($csv == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='0' $select>$CSV_N[0]</OPTION>\n");
	$count = count($CSV_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($csv == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$CSV_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");

}


// メルマガ管理
function select_meruma() {
$mail=$_POST['mail'];
include './array.inc';

	echo ("      <TD>登録状態<BR><SELECT name='mail' onchange='submit();'>\n");
	if (!$mail || ($mail == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='0' $select>$MAIL_N[0]</OPTION>\n");
	$count = count($MAIL_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($mail == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$MAIL_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></TD>\n");

}


//	商品管理カテゴリー
function select_goods() {
include './array.inc';
$goods=$_POST['goods'];
?>
      <TD>作業項目<BR>
       <SELECT name='goods' onchange='submit();'>
<?PHP
	if (!$goods || ($goods == 0)) { $select = "selected"; } else { $select = "";}
        	echo ("		<OPTION value='' $select>$GOODS_N[0]</OPTION>\n");
	$count = count($GOODS_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
			if ($i==7) continue; // Ypath 登録攻める
			if ($goods == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>$GOODS_N[$i]</OPTION>\n");
				}
echo ("      </SELECT></TD>\n");
}


//	アフェリエイト会員状態
function select_affm() {
$affm=$_POST['affm'];
include './array.inc';

	echo <<<WAKABA
      <TD>登録状態<BR>
       <SELECT name='affm'>
WAKABA;

	if ($affm == "") { $affm = 2; }
	else { $affm = (int)$affm; }
	$max = count($AFRM_N);
	for ($i=0; $i<$max; $i++) {
		if ($affm == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$AFRM_N[$i]</OPTION>\n");
	}
	echo ("      </SELECT></TD>\n");

}


//	会員番号
function select_member() {
$k_number=$_POST['k_number'];

	echo <<<WAKABA
      <TD>会員番号<BR>
      <input type="text" size="15" name="k_number" value="$k_number">
      </TD>
WAKABA;

}


//	アフェリエイト番号
function select_aff_member() {
$aff_number=$_POST['aff_number'];
	echo <<<WAKABA
      <TD>アフェリエイト番号<BR>
      <input type="text" size="15" name="aff_number" value="$aff_number">
      </TD>
WAKABA;

}


//	注文番号
function select_sells_num() {
$sells_number=$_POST['sells_number'];

	echo <<<WAKABA
      <TD>注文番号<BR>
      <input type="text" size="20" name="sells_number" value="$sells_number">
      </TD>
WAKABA;

}


//	変換状態
function select_aff_state() {
$affs=$_POST['affs'];

include './array.inc';

	echo <<<WAKABA
      <TD>変換状態<BR>
       <SELECT name='affs'>
WAKABA;

	if ($affs == "") { $affs = 3; }
	else { $affs = (int)$affs; }
	$max = count($AFRS_N);
	for ($i=0; $i<$max; $i++) {
		if ($affs == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$AFRS_N[$i]</OPTION>\n");
	}
	echo ("      </SELECT></TD>\n");

}


//	変換タイプ
function select_aff_type() {
$afft=$_POST['afft'];
include './array.inc';

	echo <<<WAKABA
      <TD>変換タイプ<BR>
       <SELECT name='afft'>
WAKABA;

	if (!$afft) { $select = "selected"; } else { $select = "";}
	echo("		<OPTION value='' $select>$AFRT_N[0]</OPTION>\n");
	$max = count($AFRT_N);
	for ($i=1; $i<$max; $i++) {
		if ($afft == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$AFRT_N[$i]</OPTION>\n");
	}
	echo ("      </SELECT></TD>\n");

}


//	ユーザー名
function select_users() {
$name_s_=$_POST['name_s_'];

	echo<<<WAKABA
      <TD>名前(名字)<BR><INPUT size='20' type='text' name='name_s_' value='$name_s_'>
      </TD>
WAKABA;
}
?>



