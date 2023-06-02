<?PHP

//	初期設定

include './select.inc';
$title = 'ネイバーズスポーツ SITE 管理画面';


	//	db接続
	include ("../../cone.inc");
	define("conn_id",$conn_id);

//	大元画面	************************************************************************
include './array.inc';
	//	セッション設定
	session_start();
	$_SESSION["SEARCH"];

	global $main,$order,$zaiko;
	$main=$_POST['main'];
    $order=$_POST['order'];
	$zaiko=$_POST['zaiko'];
	$aff=$_POST['aff'];
	$kokyaku=$_POST['kokyaku'];
	$goods=$_POST['goods'];
	$mail=$_POST['mail'];
	echo (headers());
	echo (select());

	if ($main != 1 || $order != 2) { unset($_SESSION['SEARCH']); }

	if ($main == 1) {
		if ($order == 1)	{ include './order_1.inc'; order_1(); }
		elseif ($order == 2)	{ include './order_2.inc'; order_2(); }
		elseif ($order == 3)	{ include './order_3.inc'; order_3(); }
		elseif ($order == 4)	{ include './order_4.inc'; order_4(); }
		elseif ($order == 5)	{ include './order_5.inc'; order_5(); }
		elseif ($order == 6)	{ include './order_6.inc'; order_6(); }
	}
	// elseif ($main == 2) {
	// 	if ($zaiko == 1)	{ include './zaiko_1.inc'; zaiko_1(); }
	// 	elseif ($zaiko == 2)	{ include './zaiko_2.inc'; zaiko_2(); }
	// 	elseif ($zaiko == 3)	{ include './zaiko_3.inc'; zaiko_3(); }
	// 	elseif ($zaiko == 4)	{ include './zaiko_4.inc'; zaiko_4(); }
	// }
	elseif ($main == 3) {
		if ($aff == 1)	{ include './aff_1.inc'; aff_1(); }
		elseif ($aff == 2)	{ include './aff_2.inc'; aff_2(); }
		elseif ($aff == 3)	{ include './aff_3.inc'; aff_3(); }
		elseif ($aff == 4)	{ include './aff_4.inc'; aff_4(); }
	}
	elseif ($main == 4) {
		if ($kokyaku == 1)	{ include './kokyaku_1.inc'; kokyaku_1(); }
		elseif ($kokyaku == 2)	{ include './kokyaku_2.inc'; kokyaku_2(); }
		elseif ($kokyaku == 3)	{ include './kokyaku_3.inc'; kokyaku_3(); }
		elseif ($kokyaku == 4)	{ include './kokyaku_4.inc'; kokyaku_4(); }
	}
	elseif ($main == 5) {
		if ($goods == 1)	{ include './goods_1.inc'; goods_1(); }
		elseif ($goods == 2)	{ include './goods_2.inc'; goods_2(); }
		elseif ($goods == 3)	{ include './goods_3.inc'; goods_3(); }
		elseif ($goods == 4)	{ include './goods_4.inc'; goods_4(); }
		elseif ($goods == 5)	{ include './goods_5.inc'; goods_5(); }
		elseif ($goods == 6)	{ include './goods_6.inc'; goods_6(); }
		// elseif ($goods == 7)	{ include './goods_7.inc'; goods_7(); }
		elseif ($goods == 8)	{ include './goods_8.inc'; goods_8(); }
		elseif ($goods == 9)	{ include './goods_9.inc'; goods_9(); }
		elseif ($goods == 10)	{ include './goods_11.inc'; goods_11(); }
		elseif ($goods == 11)	{ include './goods_mool_dir.inc'; goods_mool_dir(); }
		elseif ($goods == 12)	{include './goods_12.inc'; goods_12(); }
	}
	// elseif ($main == 6) {
	// 	include './access.inc'; access();
	// }
	elseif ($main == 7)	{
		if ($mail == 1)	{ include './meruma_1.inc'; meruma_1(); }
		elseif ($mail == 2)	{ include './meruma_2.inc'; meruma_2(); }
		}
	elseif ($main == 8)	{
		include './ends.inc'; ends();
	}

	echo (footer());
	mysqli_close($conn_id);
exit;

//ヘッダー	************************************************************************
function headers() {
global $main,$order,$zaiko,$kokyaku,$title;
include './array.inc';
if (key_exists("title", $_POST)) $title=$_POST['title'];
	echo <<<EOT
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<LINK rel="stylesheet" href="../../style.css?202302151300" type="text/css">
<TITLE>{$title}</TITLE>
<STYLE type="text/css">
<!--
BODY,TABLE{
  font-size : 14px;
}
-->
</STYLE>
</HEAD>
<BODY>
<b>$title</b>
　(<a href="./soldout.php" target="_blank">SoldOut商品一覧</a>)
　(<a href="./picture_check.php" target="_blank">画像登録確認</a>)
　(<a href="./yahoo_non_dir.php" target="_blank">Yahooディレクトリ未設定商品</a>)
　(<a href="./rakuten_non_dir.php" target="_blank">楽天ディレクトリ未設定商品</a>)
　(<a href="./amazon_non_dir.php" target="_blank">Amazonディレクトリ未設定商品</a>)
　(<a href="./wowma_non_dir.php" target="_blank">Wowma!ディレクトリ未設定商品</a>)
<br>

EOT;

	if ($main && ($main != 0)) { echo ("$MAIN_N[$main]"); }
	if ($main == 1) {
		if ($order && ($order != 0)) { echo (" &gt; $ORDER_N[$order]"); }
	}
	if ($main ==2) {
		if ($zaiko && ($zaiko != 0)) { echo (" &gt; $ZAIKO_N[$zaiko]"); }
	}
	if ($main ==4) {
		if ($kokyaku && ($kokyaku != 0)) { echo (" &gt; $KOKYAKU_N[$kokyaku]"); }
	}
	if ($main ==6) {
		if ($mail && ($mail != 0)) { echo (" &gt; $MAIL_N[$mail]"); }
	}
}



//フッター	************************************************************************
function footer() {

	echo <<<ALPHA
</BODY>
</HTML>
ALPHA;

	}


//	ページ処理 商品管理1	************************************************************************
function next_p_o_1() {
global $PHP_SELF,$views,$view_s,$view_e,$max;
global $main,$order,$view,$page;//_POST
	echo <<<ALPHA
<TABLE border="0" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
ALPHA;

if ($view_s >= $views) {
		$f = $page-1;
	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="order" value="$order">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="order_r" value="$order">
			<INPUT type="hidden" name="page" value="$f">
			<INPUT type="submit" value="前の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

if ($max > $view_e) {
		$n = $page+1;

	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="order" value="$order">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="order_r" value="$order">
			<INPUT type="hidden" name="page" value="$n">
			<INPUT type="submit" value="次の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

	echo <<<ALPHA
    </TR>
  </TBODY>
</TABLE>
ALPHA;

}


//	ページ処理 商品管理2	************************************************************************
function next_p_o_2() {
global $PHP_SELF,$views,$view_s,$view_e,$max,$number,$b_num;
global $main,$order,$view,$page,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;//_Post

	echo <<<ALPHA
<TABLE border="0" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
ALPHA;
if ($view_s >= $views) {
		$f = $page-1;

	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="order" value="$order">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="order_r" value="$order">
			<INPUT type="hidden" name="year_s" value="$year_s">
			<INPUT type="hidden" name="mon_s" value="$mon_s">
			<INPUT type="hidden" name="day_s" value="$day_s>">
			<INPUT type="hidden" name="year_e" value="$year_e">
			<INPUT type="hidden" name="mon_e" value="$mon_e">
			<INPUT type="hidden" name="day_e" value="$day_e">
			<INPUT type="hidden" name="page" value="$f">
			<INPUT type="hidden" name="number" value="$number">
			<INPUT type="hidden" name="b_num" value="$b_num">
			<INPUT type="submit" value="前の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

if ($max > $view_e) {
		$n = $page+1;
	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="order" value="$order">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="order_r" value="$order">
			<INPUT type="hidden" name="year_s" value="$year_s">
			<INPUT type="hidden" name="mon_s" value="$mon_s">
			<INPUT type="hidden" name="day_s" value="$day_s>">
			<INPUT type="hidden" name="year_e" value="$year_e">
			<INPUT type="hidden" name="mon_e" value="$mon_e">
			<INPUT type="hidden" name="day_e" value="$day_e">
			<INPUT type="hidden" name="page" value="$n">
			<INPUT type="hidden" name="number" value="$number">
			<INPUT type="hidden" name="b_num" value="$b_num">
			<INPUT type="submit" value="次の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

	echo <<<ALPHA
    </TR>
  </TBODY>
</TABLE>
ALPHA;

}


//	ページ処理 在庫管理	************************************************************************
function next_p_z() {
global $PHP_SELF;
global $views,$view_s,$view_e,$max; // @source zaiko_1.inc, zaiko_3.inc, zaiko_4.inc
global $main,$zaiko,$view,$page,$hinban_,$cate1,$cate2,$cate3,$touroku;// _POST
	echo <<<ALPHA
<TABLE border="0" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
ALPHA;
if ($view_s >= $views) {
		$f = $page-1;

	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="zaiko" value="$zaiko">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="zaiko_r" value="$zaiko">
			<INPUT type="hidden" name="cate1" value="$cate1">
			<INPUT type="hidden" name="cate2" value="$cate2">
			<INPUT type="hidden" name="cate3" value="$cate3">
			<INPUT type="hidden" name="cate1_r" value="$cate1">
			<INPUT type="hidden" name="cate2_r" value="$cate2">
			<INPUT type="hidden" name="hinban_" value="$hinban_">
			<INPUT type="hidden" name="touroku" value="$touroku">
			<INPUT type="hidden" name="page" value="$f">
			<INPUT type="submit" value="前の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

if ($max > $view_e) {
		$n = $page+1;

	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="zaiko" value="$zaiko">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="zaiko_r" value="$zaiko">
			<INPUT type="hidden" name="cate1" value="$cate1">
			<INPUT type="hidden" name="cate2" value="$cate2">
			<INPUT type="hidden" name="cate3" value="$cate3">
			<INPUT type="hidden" name="cate1_r" value="$cate1">
			<INPUT type="hidden" name="cate2_r" value="$cate2">
			<INPUT type="hidden" name="hinban_" value="$hinban_">
			<INPUT type="hidden" name="touroku" value="$touroku">
			<INPUT type="hidden" name="page" value="$n">
			<INPUT type="submit" value="次の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

	echo <<<ALPHA
    </TR>
  </TBODY>
</TABLE>
ALPHA;

}


//	ページ処理 顧客管理	************************************************************************
function next_p_k() {
//  global $PHP_SELF,$main,$kokyaku,$prf_,$menber,$kokyaku,$num_k_,$name_s_,$view,$mode,$page,$max,$view_s,$view_e,$menber2,$hlist;
global $num_k_, // @source kokyaku_[1..3].inc->first_00
	   $page; // @source kokyaku_[1..3].inc->first_00
include './array.inc';
global $PHP_SELF,
		$view_s,// @source kokyaku_[1..3].inc->first_00;
		$view_e,// @source kokyaku_[1..3].inc->first_00;
		$max;// @source kokyaku_[1..3].inc->first_00;
$main=$_POST['main'];
$prf_=$_POST['prf_'];
$menber=$_POST['menber'];
$kokyaku=$_POST['kokyaku'];
$name_s_=$_POST['name_s_'];
$view=$_POST['view'];
$menber2=$_POST['menber2'];
$hlist=$_POST['hlist'];
$views = $VIEW_NUM[$view];
echo <<<ALPHA
<TABLE border="0" cellpadding="0" cellspacing="0">
  <TBODY>
    <TR>
ALPHA;
if ($view_s >= $views) {
		$f = $page-1;

	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="kokyaku" value="$kokyaku">
			<INPUT type="hidden" name="kokyaku_r" value="$kokyaku">
			<INPUT type="hidden" name="menber" value="$menber">
			<INPUT type="hidden" name="menber2" value="$menber2">
			<INPUT type="hidden" name="prf_" value="$prf_">
			<INPUT type="hidden" name="num_k_" value="$num_k_">
			<INPUT type="hidden" name="name_s_" value="$name_s_">
			<INPUT type="hidden" name="hlist" value="$hlist">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="page" value="$f">
			<INPUT type="submit" value="前の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

if ($max > $view_e) {
		$n = $page+1;
		
	echo <<<ALPHA
	   <TD width="100">
		<FORM action="$PHP_SELF" method="POST">
			<INPUT type="hidden" name="main" value="$main">
			<INPUT type="hidden" name="main_r" value="$main">
			<INPUT type="hidden" name="kokyaku" value="$kokyaku">
			<INPUT type="hidden" name="kokyaku_r" value="$kokyaku">
			<INPUT type="hidden" name="menber" value="$menber">
			<INPUT type="hidden" name="menber2" value="$menber2">
			<INPUT type="hidden" name="prf_" value="$prf_">
			<INPUT type="hidden" name="num_k_" value="$num_k_">
			<INPUT type="hidden" name="name_s_" value="$name_s_">
			<INPUT type="hidden" name="hlist" value="$hlist">
			<INPUT type="hidden" name="view" value="$view">
			<INPUT type="hidden" name="page" value="$n">
			<INPUT type="submit" value="次の $views 件">
		</FORM>
	   </TD>
ALPHA;

	}

echo <<<ALPHA
    </TR>
  </TBODY>
</TABLE>
ALPHA;

}


//エラー	************************************************************************
function ERROR($msr) {
	$max = count($msr);
	--$max;
	echo ("<FONT color='#ff0000'>エラー</FONT><BR>\n");
		for ($i=0; $i<=$max; ++$i) {
			echo ("<B>・ $msr[$i] </B><BR>\n");
			}
	echo ("<BR>\n");
}


function pre($val) {

	echo("<pre>\n");
	print_r($val);
	echo("</pre>\n");

}
