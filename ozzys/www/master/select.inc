<?PHP
//	管理者画面セレクト部分

//	編集カテゴリー		select_main()
//	商品管理カテゴリー	select_goods()
//	ポスデーターアップ	select_pos()
//	発注状況			select_send()
//	メーカー選択		select_maker()
//	分類				select_bunrui()
//	商品				select_s_goods()
//	カラー				select_s_color()
//	サイズ				select_s_size()
//	検索状況			select_display()
//	作業内容			select_work()
//	詳細登録			select_regi()
//	表示数				select_view()
//	期間				select_data()
//	在庫				select_stock()	//	add ookawara 2012/10/09
//	送料無料			select_free_postagek()	//	add okabe 2016/07/15
//	竿の送料             select_rod_fee() // add simon 2018-07-03
//
//	顧客管理カテゴリー	select_kokyaku()
//	都道府県			select_prf()
//	顧客番号 名前(名字)	select_num()
//	顧客登録			select_menber()

//	商品番号＆JANコード検索窓	select_goods_jan_num()	//	add ookawara 2011/06/05

//	セレクトメイン
function select() {
    include './array.inc';
    global $main,$goods,$kokyaku,$select,$meruma,$end,$send,$maker,$display,$bunrui,$s_goods,$s_size,$prf_,$view,$num_k_,$name_s_,$PHP_SELF,$work,$regi,$main_r,$goods_r,$menber,$kokyaku_r,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;
	global $stock;	//	add ookawara 2012/10/09

	if ($main != $main_r) { $view = ""; }
	if ($goods && $goods != $goods_r) { $view = ""; }
	if ($kokyaku && $kokyaku != $kokyaku_r) { $view = ""; $prf_ = ""; $num_k_ = ""; $name_s_ = ""; }

?>
<style>
#select_menu div {
	float:left;
	padding:5px;
	height:28px;
}
</style>
<BR>
<FORM action='<? echo ("$PHP_SELF"); ?>' method='POST'>
<INPUT type='hidden' name='main_r' value='<? echo ("$main"); ?>'>
<INPUT type='hidden' name='goods_r' value='<? echo ("$goods"); ?>'>
<INPUT type='hidden' name='kokyaku_r' value='<? echo ("$kokyaku"); ?>'>
<div id="select_menu">
<?
select_main();

if ($main == 1) {
	select_goods();
	if ($goods == 1) {  }
	if ($goods == 2) { select_goods_sellsnum(); select_goods_kojinname(); select_goods_mailaddress();  select_view(); }
	if ($goods == 3) { select_maker(); select_bunrui(); select_s_goods(); select_regi(); select_goods_jan_num(); select_display(); select_stock(); select_free_postage(); select_rod_fee(); select_view(); }	//	add ookawara 2011/06/15	select_goods_jan_num	//	add  select_display(); select_stock();	ookawara 2012/10/10	// add select_free_postage() okabe 2016/07/15
	if ($goods == 4) { select_maker(); select_bunrui(); select_s_goods2(); select_display(); select_stock(); select_free_postage(); select_rod_fee(); select_view(); }	//	add select_zaiko();	ookawara 2012/10/09	// add select_free_postage() okabe 2016/07/15
	if ($goods == 5) { }
	//if ($goods == 5) { select_work(); }
	if ($goods == 6) { select_work2(); }
	if ($goods == 7) { select_maker(); select_bunrui(); select_data(); select_view(); }
	if ($goods == 8) { select_maker(); select_bunrui(); select_s_goods2(); select_goods_jan_num(); select_view(); }	//	add ohkawara  select_goods_jan_num(); 2020/03/30
	if ($goods == 9) {  }
	if ($goods == 10) {  }
	if ($goods == 11) { select_maker(); select_bunrui(); select_view(); }	//	add ookawara 2009/09/08
	//if ($goods == 11) { select_maker(); select_bunrui(); select_s_goods(); select_regi(); select_view(); }
	//if ($goods == 12) { select_view(); }	//	del ookawara 2014/02/07
	if ($goods == 12) { select_maker(); select_bunrui(); select_goods_jan_num(); select_view(); }	//	add ookawara 2014/02/07
}
elseif ($main == 2) {
	select_kokyaku();
	if ($kokyaku == 1) { select_prf(); select_num(); select_view(); }
	if ($kokyaku == 2) { select_menber(); select_prf(); select_num(); select_view(); }
	if ($kokyaku == 3) { select_prf(); select_num(); select_view(); }
	if ($kokyaku == 4) { select_prf(); select_num(); select_view(); }
}
elseif ($main == 4) { select_meruma(); }

?>
      <div style="padding-top:15px;"><INPUT type='submit' value='決定'></div>
<HR style="clear:both;">
</div>
</FORM>
<?PHP
}



//	編集カテゴリー
function select_main() {
include './array.inc';
global $main;
?>
      <div>編集カテゴリー<BR>
       <SELECT name='main'>
<?
	if (!$main || ($main == 0)) { $select = "selected"; } else { $select = "";}
        	echo ("		<OPTION value='' $select>$MAIN_N[0]</OPTION>\n");
	$count = count($MAIN_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
			if ($main == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>$MAIN_N[$i]</OPTION>\n");
				}
echo ("      </SELECT></div>\n");
}


//	商品管理カテゴリー
function select_goods() {
include './array.inc';
global $goods;
	echo ("      <div>$MAIN_N[$main]カテゴリー<BR><SELECT name='goods'>\n");
	if (!$goods || ($goods == 0)) { $select = "selected"; } else { $select = "";}
        	echo ("		<OPTION value='' $select>$MAIN_N[0]</OPTION>\n");
	$count = count($GOODS_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
			if ($goods == $i) { $select = "selected"; } else { $select = "";}
			echo ("		<OPTION value='$i' $select>$GOODS_N[$i]</OPTION>\n");
				}
	echo ("      </SELECT></div>\n");
}


//	ポスデーターアップ
function select_pos() {

}


//	発注状況
function select_send() {
include './array.inc';
global $send;
	echo ("      <div>発注状況<BR><SELECT name='send'>\n");
	if (!$send || ($send == 0)) { $select = "selected"; } else { $select = "";}
	    	echo ("		<OPTION value='' $select>$SEND_N[0]</OPTION>\n");
	$count = count($SEND_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($send == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$SEND_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}


//	メーカー選択
function select_maker() {
include './array.inc';
global $maker;
include "../../cone.inc";

	$sql = "select distinct maker_num, maker_name from maker ORDER BY maker_name;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	$max = $count - 1;
	echo ("      <div>メーカー選択<BR><SELECT name='maker'>\n");
	if (!$maker || ($maker == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='' $select>全て表示</OPTION>\n");
	for ($i=0; $i<=$max; $i++) {
		list($maker_num,$maker_name) = pg_fetch_array($sql1,$i);

		if ($maker == $maker_num) { $select = "selected"; } else { $select = "";}
		if ($maker_name) {
				echo ("		<OPTION value='$maker_num' $select>$maker_name</OPTION>\n");
					}
			}
	echo ("      </SELECT></div>\n");

}


//	分類
function select_bunrui() {
include './array.inc';
global $bunrui;
include "../../cone.inc";

	$sql = "select * from class order by class_m;";
	$sql1 = pg_exec($conn_id,$sql);
	$count = pg_numrows($sql1);
	echo ("      <div>分類<BR><SELECT name='bunrui'>\n");
	if (!$bunrui || ($bunrui == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='' $select>全て表示</OPTION>\n");
	$max = $count-1;
	for ($i=0; $i<=$max; $i++) {
		$cla = pg_fetch_array($sql1,$i);
		$CLASS_L = $cla[0];
		$CLASS_M = $cla[1];
		$CLASS_M_N = $cla[2];
		if ($bunrui == $CLASS_M) { $select = "selected"; } else { $select = "";}
		if ($CLASS_M_N) {
				echo ("		<OPTION value='$CLASS_M' $select>$CLASS_N[$CLASS_L]：$CLASS_M_N</OPTION>\n");
					}
			}
	echo ("      </SELECT></div>\n");
}


//	商品
function select_s_goods() {
	include './array.inc';
	global $maker,$bunrui,$s_goods,$s_size;
	include "../../cone.inc";

	if ($maker && $bunrui) {

	//	2009/04/25 del ookawara
	//	$sql  = "select a.goods from goods a, maker b" .
	//			" where b.maker_num='$maker' and a.maker_id=b.maker_id and a.class_m='$bunrui' GROUP BY a.goods;";

		//	2009/04/25	add ookawara
		$sql  = "SELECT a.goods FROM goods a, maker b" .
				" WHERE b.maker_num='".$maker."'".
				" AND a.maker=b.maker".
				" AND a.class_m='".$bunrui."'".
				" GROUP BY a.goods".
				" ORDER BY a.goods".	//	add ookawara 2013/02/18
				";";
//echo $sql;
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);
		if ($count >= 2) {
			echo ("      <div>商品名<BR><SELECT name='s_goods'>\n");
			if (!$s_goods || ($s_goods == 0)) { $select = "selected"; } else { $select = "";}
		    echo ("		<OPTION value='' $select>全て表示</OPTION>\n");
			$max = $count-1;
			for ($i=0; $i<=$max; $i++) {
				list($S_GOODS_N) = pg_fetch_array($sql1,$i);
				if ($s_goods == $S_GOODS_N) { $select = "selected"; } else { $select = "";}
				if ($S_GOODS_N) {
					echo ("		<OPTION value='$S_GOODS_N' $select>$S_GOODS_N</OPTION>\n");
				}
			}
			echo ("      </SELECT></div>\n");
		}
	}

	if ($maker && $bunrui && $s_goods) {

	//	2009/04/25	del ookawara
	//	$sql  = "select a.size from goods a, maker b" .
	//			" where b.maker_num='$maker' and a.maker_id=b.maker_id and a.class_m='$bunrui' AND a.goods='$s_goods'" .
	//			" GROUP BY a.size ORDER BY a.size;";

		//	2009/04/25	add ookawara
		$sql  = "SELECT a.size FROM goods a, maker b" .
				" WHERE b.maker_num='".$maker."'".
				" AND a.maker=b.maker".
				" AND a.class_m='".$bunrui."'".
				" AND a.goods='".$s_goods."'" .
				" GROUP BY a.size ORDER BY a.size;";

		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);
		if ($count >= 2) {
			echo ("      <div>サイズ<BR><SELECT name='s_size'>\n");
			if (!$s_size || ($s_size == 0)) { $select = "selected"; } else { $select = "";}
		    echo ("		<OPTION value='' $select>全て表示</OPTION>\n");
			$max = $count-1;
			for ($i=0; $i<=$max; $i++) {
				list($S_SIZE_N) = pg_fetch_array($sql1,$i);
				if ($s_size == $S_SIZE_N) { $select = "selected"; } else { $select = "";}
				if ($S_SIZE_N) {
					echo ("		<OPTION value='$S_SIZE_N' $select>$S_SIZE_N</OPTION>\n");
				}
			}
			echo ("      </SELECT></div>\n");
		}
	}

}


//	商品2
function select_s_goods2() {
	include './array.inc';
	global $maker,$bunrui,$s_goods,$s_size;
	include "../../cone.inc";

	if ($maker && $bunrui) {
		$sql  = "select goods_name from list" .
				" where maker_num='$maker' and class_m='$bunrui'".
				" AND state='0'".
				" GROUP BY goods_name;";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);
		if ($count >= 2) {
			echo ("      <div>商品名<BR><SELECT name='s_goods'>\n");
			if (!$s_goods || ($s_goods == 0)) { $select = "selected"; } else { $select = "";}
		    echo ("		<OPTION value='' $select>全て表示</OPTION>\n");
			$max = $count-1;
			for ($i=0; $i<=$max; $i++) {
				list($S_GOODS_N) = pg_fetch_array($sql1,$i);
				if ($s_goods == $S_GOODS_N) { $select = "selected"; } else { $select = "";}
				if ($S_GOODS_N) {
						echo ("		<OPTION value='$S_GOODS_N' $select>$S_GOODS_N</OPTION>\n");
				}
			}
			echo ("      </SELECT></div>\n");
		}
	}

	if ($maker && $bunrui && $s_goods) {
		$sql =  "select size from list" .
				" where maker_num='$maker' AND class_m='$bunrui' AND goods_name='$s_goods'" .
				" AND state='0'".
				" GROUP BY size ORDER BY size;";
		$sql1 = pg_exec($conn_id,$sql);
		$count = pg_numrows($sql1);
		if ($count >= 2) {
			echo ("      <div>サイズ<BR><SELECT name='s_size'>\n");
			if (!$s_size || ($s_size == 0)) { $select = "selected"; } else { $select = "";}
		    echo ("		<OPTION value='' $select>全て表示</OPTION>\n");
			$max = $count-1;
			for ($i=0; $i<=$max; $i++) {
				list($S_SIZE_N) = pg_fetch_array($sql1,$i);
				if ($s_size == $S_SIZE_N) { $select = "selected"; } else { $select = "";}
				if ($S_SIZE_N) {
					echo ("		<OPTION value='$S_SIZE_N' $select>$S_SIZE_N</OPTION>\n");
				}
			}
			echo ("      </SELECT></div>\n");
		}
	}

}


//	検索状況
function select_display() {
include './array.inc';
global $display;
	echo ("      <div>検索状況<BR><SELECT name='display'>\n");
	if (!$display || ($display == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='' $select>$DISPLAY_N[0]</OPTION>\n");
	$count = count($DISPLAY_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($display == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$DISPLAY_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}


//	作業内容
function select_work() {
include './array.inc';
global $work,$goods;
	echo ("      <div>作業内容<BR><SELECT name='work'>\n");
	if (!$work || ($work == 0)) {  $select = 'selected'; } else { $select = '';}
    echo ("		<OPTION value='' $select>$WORK_N[0]</OPTION>\n");
	$count = count($WORK_N);
	$max = $count-1;
	if ($goods == 6) { $max = $max-1; }
	for ($i=1; $i<=$max; $i++) {
		if ($work == $i) { $select = 'selected'; } else { $select = '';}
		echo ("		<OPTION value='$i' $select>$WORK_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}


//	作業内容
function select_work2() {
include './array.inc';
global $work,$goods;
	echo ("      <div>作業内容<BR><SELECT name='work'>\n");
	if (!$work || ($work == 0)) {  $select = 'selected'; } else { $select = '';}
    echo ("		<OPTION value='' $select>$WORK_N2[0]</OPTION>\n");
	$count = count($WORK_N2);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($work == $i) { $select = 'selected'; } else { $select = '';}
		echo ("		<OPTION value='$i' $select>$WORK_N2[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}


//	詳細登録
function select_regi() {
include './array.inc';
global $regi;
	echo ("      <div>詳細登録<BR><SELECT name='regi'>\n");
	if (!$regi || ($regi == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='' $select>$REGI_N[0]</OPTION>\n");
	$count = count($REGI_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($regi == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$REGI_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}


//	期間
function select_data() {
include './array.inc';
global $year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;


	$year_t = date("Y");
	$mon_t = date("n");
	$day_t = date("d");
	if (!$year_s) { $year_s = 2003; }
	if (!$mon_s) { $mon_s = 1; }
	if (!$day_s) { $day_s = 1; }
	if (!$year_e) { $year_e = $year_t; }
	if (!$mon_e) { $mon_e = $mon_t; }
	if (!$day_e) { $day_e = $day_t; }

	echo ("      <div>表示期間<BR><SELECT name='year_s'>\n");
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
	echo ("      </SELECT>日</div>\n");

}


//	表示数
function select_view() {
include './array.inc';
global $view;

	//	2009/03/31
	if ($view < 1) { $view = 1; }

	echo ("      <div>表示数<BR><SELECT name='view'>\n");
	if (!$view || ($view == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='' $select>$VIEW_N[0]</OPTION>\n");
	$count = count($VIEW_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($view == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$VIEW_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}

// 注文用 >>>

// 注文番号で検索
function select_goods_sellsnum() {
    global $goods_sellsnum;

    if(isset($goods_sellsnum)) {
        $_SESSION['master_goods_sellsnum'] = $goods_sellsnum;
    }
    elseif($_SESSION['master_goods_sellsnum']) {
        $goods_sellsnum = $_SESSION['master_goods_sellsnum'];
    }

    echo("<div><label for='goods_sellsnum'>注文番号</label><br><input id='goods_sellsnum' name='goods_sellsnum' type='text' value='$goods_sellsnum' /></div>");
}

// 公人名で検索
function select_goods_kojinname() {
    global $goods_kojinname;

    if(isset($goods_kojinname)) {
        $_SESSION['master_goods_kojinname'] = $goods_kojinname;
    }
    elseif($_SESSION['master_goods_kojinname']) {
        $goods_kojinname = $_SESSION['master_goods_kojinname'];
    }

    echo("<div><label for='goods_kojinname'>お客さん名</label><br><input id='goods_kojinname' name='goods_kojinname' type='text' value='$goods_kojinname' /></div>");
}

// メールアドレス 絞り込み
function select_goods_mailaddress() {
    global $goods_mailaddress;

    if(isset($goods_mailaddress)) {
        $_SESSION['master_goods_mailaddress'] = $goods_mailaddress;
    }
    elseif($_SESSION['master_goods_mailaddress']) {
        $goods_mailaddress = $_SESSION['master_goods_mailaddress'];
    }

    echo("<div><label for='goods_mailaddress'>メールアドレス</label><br><input id='goods_mailaddress' name='goods_mailaddress' type='text' value='$goods_mailaddress' /></div>");
}

// <<<

//	顧客管理カテゴリー
function select_kokyaku() {
include './array.inc';
global $kokyaku;
	echo ("      <div>$MAIN_N[$main]カテゴリー<BR><SELECT name='kokyaku'>\n");
	if (!$kokyaku || ($kokyaku == 0)) { $select = "selected"; } else { $select = "";}
       	echo ("		<OPTION value='' $select>$KOKYAKU_N[0]</OPTION>\n");
	$count = count($KOKYAKU_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($kokyaku == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$KOKYAKU_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}


//	都道府県
function select_prf() {
include './array.inc';
global $prf_;
	echo ("      <div>都道府県<BR><SELECT name='prf_'>\n");
	if (!$prf_ || ($prf_ == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='' $select>$PRF_N[0]</OPTION>\n");
	$count = count($PRF_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($prf_ == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$PRF_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");
}


// 顧客番号 名前(名字)
function select_num() {
global $num_k_,$name_s_;
?>
      <div>顧客番号<BR><INPUT size='20' type='text' name='num_k_' value='<? echo ("$num_k_"); ?>'>
      </div>
      <div>名前(名字)<BR><INPUT size='20' type='text' name='name_s_' value='<? echo ("$name_s_"); ?>'>
      </div>
<?
}


// 顧客登録
function select_menber() {
global $menber;
include './array.inc';

	echo ("      <div>登録状態<BR><SELECT name='menber'>\n");
	if (!$menber || ($menber == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='0' $select>$MENBER_N[0]</OPTION>\n");
	$count = count($MENBER_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($menber == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$MENBER_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");

}


// メルマガ管理
function select_meruma() {
global $mail;
include './array.inc';

	echo ("      <div>登録状態<BR><SELECT name='mail'>\n");
	if (!$mail || ($mail == 0)) { $select = "selected"; } else { $select = "";}
    echo ("		<OPTION value='0' $select>$MAIL_N[0]</OPTION>\n");
	$count = count($MAIL_N);
	$max = $count-1;
	for ($i=1; $i<=$max; $i++) {
		if ($mail == $i) { $select = "selected"; } else { $select = "";}
		echo ("		<OPTION value='$i' $select>$MAIL_N[$i]</OPTION>\n");
			}
	echo ("      </SELECT></div>\n");

}



//	商品番号＆JANコード検索窓	//	add ookawara 2011/06/05
function select_goods_jan_num() {

	$html  = "";
	$html .= "<div>商品番号＆JANコード<br><input type=\"text\" size=\"20\" name=\"goods_jan_num\" value=\"".$_POST['goods_jan_num']."\" /></div>\n";

	echo $html;
}


//	在庫状況
//	add ookawara 2012/10/09
function select_stock() {
include './array.inc';

	$stock = $_POST['stock'];

	echo "<div>\n";
	echo "在庫状況<br>\n";
	echo "<select name='stock'>\n";
	if ($stock == "") { $stock = "0"; }
	foreach ($STOCK AS $key => $val) {
		$selected = "";
		if ($stock == $key) {
			$selected = " selected";
		}
	    echo "<option value='".$key."'".$selected.">".$val."</option>\n";
	}
	echo "</select>\n";
	echo "</div>\n";
}


//	送料無料
//	add okabe 2016/07/15
function select_free_postage() {
include './array.inc';

	$free_postage = $_POST['sel_free_postage'];

	echo "<div>\n";
	echo "送料無料<br>\n";
	echo "<select name='sel_free_postage'>\n";
	if ($free_postage == "") { $free_postage = "0"; }
	foreach ($FREE_POSTAGE AS $key => $val) {
		$selected = "";
		if ($free_postage == $key) {
			$selected = " selected";
		}
	    echo "<option value='".$key."'".$selected.">".$val."</option>\n";
	}
	echo "</select>\n";
	echo "</div>\n";
}


function select_rod_fee() {
    require_once '../sub/souryou.inc';

    // input
    $sel_rod_fee = $_POST['sel_rod_fee'];

    // UI
    $html = "<div>";
    $html .= "竿の送料<br>";
    $html .= "<select name='sel_rod_fee'>";
    $html .= "<option value=''>全て</option>";
    $all_rods_fee_selected = ($sel_rod_fee=='rod_fee_all')? 'selected': '';
    $html .= "<option value='rod_fee_all' $all_rods_fee_selected>送料のある竿のみ</option>";
    foreach(Souryou::$ROD_SOURYOU_L as $k => $v) {
        $selected = ($k==$sel_rod_fee)? 'selected': '';
        $html .= "<option value='$k' $selected>$k ($v 円)</option>";
    }
    $html .= "</select>";
    $html .= "</div>";

    echo $html;
}
